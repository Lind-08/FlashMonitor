#ifndef USBINFO_H
#define USBINFO_H
#include <QString>
#include <windows.h>

enum class UsbState
{
    blocked,
    empty,
    full,
    apply,
    waited
};

struct UsbInfo
{
    QString VID;
    QString PID;
    QString serial;
    QString name;
    HANDLE handle;
    QChar letter;
    UsbState state;
};

UsbInfo GetDriveInfo(QChar letter);
#endif // USBINFO_H
