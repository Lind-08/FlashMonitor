#include "usbbase.h"
#include <QThreadPool>
#include <usbinfofinder.h>
#include <QMessageBox>

UsbBase* UsbBase::_instance;

UsbBase::UsbBase(QObject *parent) : QObject(parent)
{

}

bool UsbBase::checkInBase(UsbInfo *info)
{
    foreach (auto item, base)
    {
        if (item.first == info)
            return true;
    }
    return false;
}

QPair<UsbInfo *, bool> UsbBase::getPair(UsbInfo *info)
{
    foreach (auto item, base)
    {
        if (item.first == info)
            return item;
    }
}


void UsbBase::addUsbStorage(HANDLE handle, QChar letter)
{
    UsbInfo *info = new UsbInfo();
    info->letter = letter;
    info->handle = handle;
    info->state = UsbState::empty;
    emit deviceConnected(info);
    connectedDevices[letter] = info;
    UsbInfoFinder *finder = new UsbInfoFinder(this, info);
    connect(finder, &UsbInfoFinder::informationFinded, this, &UsbBase::informationFinded);
    finder->run();
}

void UsbBase::removeUsbStorage(QChar letter)
{
    UsbInfo *info = connectedDevices[letter];
    emit deviceDisconnected(info);
    unlockDevice(info);
}

void UsbBase::unlockDevice(UsbInfo *info)
{
    DWORD byteReturned = 0;
    DeviceIoControl(info->handle, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0, &byteReturned,0);
    CloseHandle(info->handle);
    connectedDevices.remove(info->letter);
}

void UsbBase::informationFinded(UsbInfoFinder *finder)
{
    UsbInfo *info = finder->getInfo();
    finder->deleteLater();
    emit deviceConnected(info);
    if(checkInBase(info))
    {
        auto pair = getPair(info);
        if (pair.second)
        {
            info->state = UsbState::apply;
            unlockDevice(info);
        }
        info->state = UsbState::blocked;
        emit deviceConnected(info);
    }
    else
    {
        emit newDevice(info);
    }
}

void UsbBase::newDeviceResult(int result, UsbInfo * info)
{
    if (result != QMessageBox::Ignore)
    {
        QPair<UsbInfo*, bool> pair(info, true);
        if (result == QDialog::Accepted)
        {
            base.append(pair);
            unlockDevice(pair.first);
        }
        else
        {
            pair.second = false;
            base.append(pair);
        }
    }
}
