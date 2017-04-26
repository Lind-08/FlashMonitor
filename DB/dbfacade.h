#pragma once

#include <QObject>
#include <QSqlDatabase>
class QSqlQuery;

class DbFacade : public QObject
{
    Q_OBJECT
    QSqlDatabase db;
    static QString dbName;
    static QString driver;

    explicit DbFacade(QObject *parent = 0);
    static DbFacade *instance;
public:
    static void InitDbFacade(QString Driver = "QSQLITE", QString DbName = "test.sqlite");
    static DbFacade *Instance();
    QSqlQuery *CreateQuery();
signals:

public slots:
};
