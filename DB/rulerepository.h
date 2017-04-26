#pragma once
#include <QObject>
#include "basesqlrepository.h"
class Rule;
class Host;
class Usb;

class RuleRepository : public QObject, BaseSqlRepository<Rule>
{
    typedef BaseSqlRepository<Rule> Parent;

    Q_OBJECT

    const QString TABLE_NAME = "rules";

    const QString INSERT_QUERY_STRING = "INSERT INTO %1 (usb_id, host_id, value) VALUES ('%2','%3', '%4')";
    const QString UPDATE_QUERY_STRING = "UPDATE %1 SET usb_id='%2', host_id='%3', value='%4' where id=%5";
    const QString DELETE_QUERY_STRING = "DELETE FROM %1 WHERE id=%2";

    QString getInsertQuery(Rule *object);
    QString getUpdateQuery(Rule *object);
    QString getDeleteQuery(Rule *object);
    QString getQueryForID();
    QList<Rule *> getList(QString queryString);

    static RuleRepository *instance;
    explicit RuleRepository(QObject *parent = 0);


public:
    static RuleRepository *Instance();
    QList<Rule *> GetAll();
    void Save(Rule *object);
    void Delete(Rule *object);
    Rule *GetByHostAndUsb(Host *host, Usb *usb);
    QList<Rule *> GetByHost(Host *host);
    QList<Rule *> GetByUsb(Usb *usb);


};
