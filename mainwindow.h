#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAbstractNativeEventFilter>
#include <windows.h>
#include <dbt.h>
#include "usbinfo.h"
#include <usbbase.h>


using namespace std;


class MainWindow : public QWidget, QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void newDevice(UsbInfo *info);

private:

    /* This is where all the input to the window goes to */

    UsbBase *base;

    bool nativeEventFilter( QByteArray const& eventType, void* msg, long* result );

signals:
    void deviceArrived(HANDLE, QChar);
    void deviceRemoved(QChar);
    void newDeviceResult(int result, UsbInfo *info);
};

#endif // MAINWINDOW_H
