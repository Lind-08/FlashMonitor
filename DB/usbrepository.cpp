#include "usbrepository.h"
#include "usb.h"
#include "dbfacade.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QException>
#include <QVariant>
#include <stdexcept>
#include <QSqlRecord>
#include "usb.h"
#include <QDebug>

UsbRepository* UsbRepository::instance = nullptr;

UsbRepository::UsbRepository(QObject *parent) :
    QObject(parent),
    Parent()
{

}

QString UsbRepository::getInsertQuery(Usb *object)
{
    return INSERT_QUERY_STRING.arg(TABLE_NAME).arg(object->VID()).arg(object->PID()).arg(object->Serial()).arg(object->Name());
}

QString UsbRepository::getUpdateQuery(Usb *object)
{
    return UPDATE_QUERY_STRING.arg(TABLE_NAME).arg(object->VID()).arg(object->PID()).arg(object->Serial()).arg(object->Name()).arg(object->ID());
}

QString UsbRepository::getDeleteQuery(Usb *object)
{
    return DELETE_QUERY_STRING.arg(TABLE_NAME).arg(object->ID());
}

QString UsbRepository::getQueryForID()
{
    return Parent::GET_ID_QUERY.arg(TABLE_NAME);
}


UsbRepository *UsbRepository::Instance()
{
    if (instance == nullptr)
        instance = new UsbRepository();
    return instance;
}

QList<Usb *> UsbRepository::GetAll()
{
    auto query = execQueryWithResult(getQueryForID());
    QList<Usb*> result;
    do
    {
        if (!query->isValid())
            query->next();
        Usb *obj = Usb::Create();
        obj->setID(query->value(0).toInt());
        obj->setVID(query->value(1).toString());
        obj->setPID(query->value(2).toString());
        obj->setSerial(query->value(3).toString());
        obj->setName(query->value(4).toString());
        result.push_back(obj);
    } while(query->next());
    return result;
}

void UsbRepository::Save(Usb *object)
{
    if (object->ID() == Usb::INVALID_ID)
    {
        create(object);
        object->setID(getIdAfterInsert());
    }
    else
    {
        update(object);
    }
}

void UsbRepository::Delete(Usb *object)
{
    if (object->ID() != Usb::INVALID_ID)
        remove(object);
}

Usb *UsbRepository::GetByVIDandPID(QString VID, QString PID)
{
    QString queryString = QString("SELECT * FROM %1 WHERE VID='%2' AND PID='%3';")\
            .arg(TABLE_NAME).arg(VID).arg(PID);
    auto query = execQueryWithResult(queryString);
    qDebug() << query->lastError().text();
    auto rec = query->record();
    Usb *usb = Usb::Create();
    if (query->size() == 0)
        return usb;
    if (!query->isValid())
        query->next();
    usb->setID(query->value(rec.indexOf("id")).toInt());
    usb->setVID(query->value(rec.indexOf("VID")).toString());
    usb->setPID(query->value(rec.indexOf("PID")).toString());
    usb->setSerial(query->value(rec.indexOf("serial")).toString());
    usb->setName(query->value(rec.indexOf("name")).toString());
    return usb;
}
