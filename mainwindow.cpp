#include "mainwindow.h"

#include <QCloseEvent>
#include <QAbstractEventDispatcher>
#include <QAction>
#include <QCoreApplication>
#include <QMessageBox>
#include <QMenu>
#include <windows.h>
#include <dbt.h>
#include <usbbase.h>

wchar_t DriveMaskToLetter(int unitmask)
{
    wchar_t i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
        break;
        unitmask = unitmask >> 1;
    }
    return( i + L'A' );
}

GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72,
                      0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };
GUID ALL_USB_GUID = { 0x53f5630d,0xb6bf,0x11d0,0x94,0xf2,0x00,0xa0,0xc9,0x1e,0xfb,0x8b };


BOOL DoRegisterDeviceInterfaceToHwnd(
    IN GUID InterfaceClassGuid,
    IN HWND hWnd,
    OUT HDEVNOTIFY *hDeviceNotify
)
// Routine Description:
//     Registers an HWND for notification of changes in the device interfaces
//     for the specified interface class GUID.

// Parameters:
//     InterfaceClassGuid - The interface class GUID for the device
//         interfaces.

//     hWnd - Window handle to receive notifications.

//     hDeviceNotify - Receives the device notification handle. On failure,
//         this value is NULL.

// Return Value:
//     If the function succeeds, the return value is TRUE.
//     If the function fails, the return value is FALSE.

// Note:
//     RegisterDeviceNotification also allows a service handle be used,
//     so a similar wrapper function to this one supporting that scenario
//     could be made from this template.
{
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_HANDLE;
    NotificationFilter.dbcc_classguid = InterfaceClassGuid;

    *hDeviceNotify = RegisterDeviceNotification(
        hWnd,                       // events recipient
        &NotificationFilter,        // type of device
        DEVICE_NOTIFY_WINDOW_HANDLE // type of recipient handle
        );

    if ( NULL == *hDeviceNotify )
    {
        //ErrorHandler(TEXT("RegisterDeviceNotification"));
        return FALSE;
    }

    return TRUE;
}




MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    QAbstractNativeEventFilter()
{
    base = UsbBase::Instance();
    //connect(base, &UsbBase::newDevice, this, &MainWindow::newDevice);
    //connect(this, &MainWindow::newDeviceResult, base, &UsbBase::newDeviceResult);
    connect(this, &MainWindow::deviceArrived, base, &UsbBase::addUsbStorage);
    connect(this, &MainWindow::deviceRemoved, base, &UsbBase::removeUsbStorage);
    this->hide();

}

MainWindow::~MainWindow()
{
    QAbstractEventDispatcher::instance()->removeNativeEventFilter(this);
}

void MainWindow::StartMonitoring()
{
    QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    DoRegisterDeviceInterfaceToHwnd(ALL_USB_GUID, reinterpret_cast<HWND>(this->winId()), &hDeviceNotify);
}

void MainWindow::StopMonitoring()
{
    UnregisterDeviceNotification(&hDeviceNotify);
    QAbstractEventDispatcher::instance()->removeNativeEventFilter(this);
}


bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *msg, long *result)
{
    MSG *message = static_cast<MSG *>( msg );
    switch (message->message)
    {
    case WM_DEVICECHANGE:
    {
        switch (message->wParam)
            {
                case DBT_DEVICEARRIVAL:
                    {

                        PDEV_BROADCAST_HDR lpdb = ((PDEV_BROADCAST_HDR)message->lParam);
                        if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
                        {
                            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)message->lParam;
                            wchar_t c = DriveMaskToLetter(lpdbv->dbcv_unitmask);
                            wchar_t str[7];
                            swprintf(str, 7, L"\\\\.\\%c:", c);
                            HANDLE ptr = CreateFileW(str, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
                            if (ptr != NULL)
                            {
                                DWORD byteReturned = 0;
                                DeviceIoControl(ptr, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0, &byteReturned,0);
                                DeviceIoControl(ptr, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &byteReturned,0);
                                DeviceIoControl(ptr, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0, &byteReturned,0);
                                DeviceIoControl(ptr, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &byteReturned,0);
                                emit deviceArrived(ptr, c);
                            }
                        }
                    } break;
                case DBT_DEVICEREMOVECOMPLETE:
                    {
                        PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)message->lParam;
                        wchar_t c = DriveMaskToLetter(lpdbv->dbcv_unitmask);
                        emit deviceRemoved(c);
                    }
                    break;

            }
            return false;
      }
    default:
        return false;
    }
    return false;
}

