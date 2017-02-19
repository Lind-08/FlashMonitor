#include "usbinfofinder.h"

UsbInfoFinder::UsbInfoFinder(QObject *parent, UsbInfo* info) : QObject(parent)
{
    infoObject = info;
}

void UsbInfoFinder::run()
{
    UsbInfo info = GetDriveInfo(infoObject->letter);
    infoObject->name = info.name;
    infoObject->PID = info.PID;
    infoObject->VID = info.VID;
    infoObject->serial = info.serial;
    infoObject->state = UsbState::full;
    emit informationFinded(this);
}

UsbInfo *UsbInfoFinder::getInfo()
{
    return infoObject;
}
