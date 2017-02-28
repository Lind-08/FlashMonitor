#include "usbbase.h"
#include <QThreadPool>
#include <usbinfofinder.h>
#include <QMessageBox>
#include <QSettings>
#include <QJsonObject>
#include <QApplication>

UsbBase* UsbBase::_instance;

UsbBase::UsbBase(QObject *parent) : QObject(parent)
{
    client = new UsbClient(this);
    client->setAddress(qApp->property("address").toString());
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
    QString code;
    auto answ = client->ConnectToServer();
    code = answ["code"].toString();
    if (code == SUCCES_STRING)
    {
        answ = client->GetRule(info->VID, info->PID);
        code = answ["code"].toString();
        if (code == SUCCES_STRING)
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
        else
            emit error(code);
    }
    else
        emit error(code);
}

void UsbBase::informationFinded(UsbInfoFinder *finder)
{
    UsbInfo *info = finder->getInfo();
    finder->deleteLater();
    info->state = UsbState::full;
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
