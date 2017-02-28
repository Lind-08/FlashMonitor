#include "usbbase.h"
#include <QThreadPool>
#include <usbinfofinder.h>
#include <QMessageBox>
#include <QSettings>
#include <QJsonObject>

UsbBase* UsbBase::_instance;

UsbBase::UsbBase(QObject *parent) : QObject(parent)
{
    QSettings settings("client.ini", QSettings::IniFormat);
    client = new UsbClient(this);
    client->setAddress(settings.value("server/address").toString());
    client->setPort(settings.value("server/port").toInt());
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
    unlockDevice(info);
}

void UsbBase::unlockDevice(UsbInfo *info)
{
    DWORD byteReturned = 0;
    DeviceIoControl(info->handle, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0, &byteReturned,0);
    CloseHandle(info->handle);
    connectedDevices.remove(info->letter);
}

void UsbBase::addRule(UsbInfo *info)
{
    CloseHandle(info->handle);
    base.append(info);
}

void UsbBase::newDevice(UsbInfo *info)
{
    QString SUCCES_STRING = "SUCCES";
    auto answ = client->ConnectToServer();
    if (answ["code"].toString() == SUCCES_STRING)
    {
        answ = client->GetRule(info->VID, info->PID);
        if (answ["code"].toString() == SUCCES_STRING)
        {
            if(answ["value"].toBool())
            {
                info->state = UsbState::apply;
                unlockDevice(info);
            }
            else
                info->state = UsbState::blocked;
            addRule(info);
            client->Disconnect();
        }
    }
}

void UsbBase::informationFinded(UsbInfoFinder *finder)
{
    UsbInfo *info = finder->getInfo();
    finder->deleteLater();
    emit deviceConnected(info);
    auto newInfo = getInfo(info);
    if(newInfo != nullptr)
    {
        if (newInfo->state == UsbState::apply)
        {
            info->state = UsbState::apply;
            unlockDevice(info);
        }
        info->state = UsbState::blocked;
        emit deviceConnected(info);
    }
    else
    {
        info->state = UsbState::waited;
        newDevice(info);
    }
}
