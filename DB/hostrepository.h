#pragma once

#include <QObject>
#include "basesqlrepository.h"
class Host;

class HostRepository : public QObject, BaseSqlRepository<Host>
{
    typedef BaseSqlRepository<Host> Parent;

    Q_OBJECT

    const QString TABLE_NAME = "host";

    const QString INSERT_QUERY_STRING = "INSERT INTO %1 (secret, name) VALUES ('%2','%3')";
    const QString UPDATE_QUERY_STRING = "UPDATE %1 SET secret='%2', name='%3', status='%4' WHERE id=%5";
    const QString DELETE_QUERY_STRING = "DELETE FROM %1 WHERE id=%2";

    QString getInsertQuery(Host *object);
    QString getUpdateQuery(Host *object);
    QString getDeleteQuery(Host *object);
    QString getQueryForID();

    static HostRepository *instance;
    explicit HostRepository(QObject *parent = 0);
public:
    static HostRepository *Instance();
    QList<Host *> GetAll();
    void Save(Host *object);
    void Delete(Host *object);
    Host *GetBySecret(QString secret);
};

