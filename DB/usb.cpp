#include "usb.h"


Usb::Usb(QObject *parent) : QObject(parent)
{

}

Usb* Usb::Create(QObject *parent)
{
    Usb *info = new Usb(parent);
    info->id = Usb::INVALID_ID;
    return info;
}

void Usb::setID(const qint32 ID)
{
    id = ID;
}

void Usb::setVID(const QString VID)
{
    vid = VID;
}

void Usb::setPID(const QString PID)
{
    pid = PID;
}

void Usb::setSerial(const QString Serial)
{
    serial = Serial;
}

void Usb::setName(const QString Name)
{
    name = Name;
}

