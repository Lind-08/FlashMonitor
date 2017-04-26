#include "rulerepository.h"
#include "rule.h"
#include "host.h"
#include "usb.h"

RuleRepository *RuleRepository::instance = nullptr;

RuleRepository::RuleRepository(QObject *parent) :
    QObject(parent),
    Parent()
{

}

QString RuleRepository::getInsertQuery(Rule *object)
{
    return INSERT_QUERY_STRING.arg(TABLE_NAME).arg(object->Usb_ID()).arg(object->Host_ID())\
            .arg(object->Value());
}

QString RuleRepository::getUpdateQuery(Rule *object)
{
    return UPDATE_QUERY_STRING.arg(TABLE_NAME).arg(object->Usb_ID()).arg(object->Host_ID())\
            .arg(object->Value()).arg(object->ID());
}

QString RuleRepository::getDeleteQuery(Rule *object)
{
    return DELETE_QUERY_STRING.arg(TABLE_NAME).arg(object->ID());
}

QString RuleRepository::getQueryForID()
{
    return BaseSqlRepository::GET_ID_QUERY.arg(TABLE_NAME);
}

QList<Rule *> RuleRepository::getList(QString queryString)
{
    auto query = execQueryWithResult(queryString);
    QList<Rule*> result;
    do
    {
        if (!query->isValid())
            query->next();
        Rule *obj = Rule::Create();
        obj->setID(query->value(0).toInt());
        obj->setUsb_ID(query->value(1).toInt());
        obj->setHost_ID(query->value(2).toInt());
        obj->setValue(query->value(3).toBool());
        result.push_back(obj);
    } while(query->next());
    return result;
}

RuleRepository *RuleRepository::Instance()
{
    if (instance == nullptr)
        instance = new RuleRepository();
    return instance;
}

QList<Rule *> RuleRepository::GetAll()
{
    return getList(getQueryForID());
}

void RuleRepository::Save(Rule *object)
{
    if (object->ID() == Rule::INVALID_ID)
    {
        create(object);
        object->setID(getIdAfterInsert());
    }
    else
    {
        update(object);
    }
}

void RuleRepository::Delete(Rule *object)
{
    if (object->ID() != Rule::INVALID_ID)
        remove(object);
}

Rule *RuleRepository::GetByHostAndUsb(Host *host, Usb *usb)
{
    if (host->ID() == Host::INVALID_ID || usb->ID() == Usb::INVALID_ID)
        return Rule::Create();
    QString queryString = QString("SELECT * FROM %1 WHERE host_id='%2' AND usb_id='%3';")\
            .arg(TABLE_NAME).arg(host->ID()).arg(usb->ID());
    auto query = execQueryWithResult(queryString);
    auto rec = query->record();
    auto res = Rule::Create();
    if (!query->isValid())
        query->next();
    res->setID(query->value(rec.indexOf("id")).toInt());
    res->setHost_ID(query->value(rec.indexOf("host_id")).toInt());
    res->setUsb_ID(query->value(rec.indexOf("usb_id")).toInt());
    res->setValue(query->value(rec.indexOf("value")).toBool());
    return res;
}

QList<Rule *> RuleRepository::GetByHost(Host *host)
{
    if (host->ID() == Host::INVALID_ID)
        return QList<Rule *>();
    QString queryString = QString("SELECT * FROM %1 WHERE host_id='%2';")\
            .arg(TABLE_NAME).arg(host->ID());
    return getList(queryString);
}

QList<Rule *> RuleRepository::GetByUsb(Usb *usb)
{
    if (usb->ID() == Usb::INVALID_ID)
        return QList<Rule *>();
    QString queryString = QString("SELECT * FROM %1 WHERE usb_id='%2';")\
            .arg(TABLE_NAME).arg(usb->ID());
    return getList(queryString);
}
