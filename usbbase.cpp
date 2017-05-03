#include "usbbase.h"
#include <QThreadPool>
#include <usbinfofinder.h>
#include <QMessageBox>
#include <QSettings>
#include <QJsonObject>
#include <QApplication>
#include <QHostAddress>
#include "DB/usb.h"
#include "DB/usbrepository.h"
#include "DB/rule.h"
#include "DB/rulerepository.h"

UsbBase* UsbBase::_instance;

UsbBase::UsbBase(QObject *parent) : QObject(parent)
{
    client = new UsbClient(this);
    client->setAddress(QHostAddress(qApp->property("address").toString()).toString());
    client->setPort(qApp->property("port").toInt());
    client->setName(qApp->property("name").toString());
    client->setSecret(qApp->property("secret").toString());
}

UsbInfo *UsbBase::getInfo(UsbInfo *info)
{
    foreach (auto item, base)
    {
        if (item->VID == info->VID && item->PID == info->PID)
            return item;
    }
    return nullptr;
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
    if (info != nullptr)
        unlockDevice(info);
}

void UsbBase::unlockDevice(UsbInfo *info)
{
    DWORD byteReturned = 0;
    DeviceIoControl(info->handle, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0, &byteReturned,0);
    CloseHandle(info->handle);
    //connectedDevices.remove(info->letter);
}

void UsbBase::addRule(UsbInfo *info)
{
    if (info->state == UsbState::apply)
    {
        unlockDevice(info);
    }
    base.append(info);
}

void UsbBase::newDevice(UsbInfo *info)
{
    try
    {
        auto usbRep = UsbRepository::Instance();
        auto usb = usbRep->GetByVIDandPID(info->VID, info->PID);
        auto ruleRep = RuleRepository::Instance();
        auto rule = ruleRep->GetByUsb(usb).first();
        if (rule->Value())
            info->state = UsbState::apply;
        else
            info->state = UsbState::blocked;
        addRule(info);
    }
    catch (...)
    {
        error("Бла бла бла");
    }
}

void UsbBase::informationFinded(UsbInfoFinder *finder)
{
    UsbInfo *info = finder->getInfo();
    finder->deleteLater();
    info->state = UsbState::full;
    auto newInfo = getInfo(info);
    if(newInfo != nullptr)
    {
        if (newInfo->state == UsbState::apply)
        {
            info->state = UsbState::apply;
            unlockDevice(info);
        }
        else
            info->state = UsbState::blocked;
        emit deviceConnected(info);
    }
    else
    {
        info->state = UsbState::waited;
        emit newDevice(info);
    }
}
