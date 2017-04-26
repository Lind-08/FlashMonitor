#include "dbfacade.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <stdexcept>

DbFacade *DbFacade::instance = nullptr;
QString DbFacade::driver;
QString DbFacade::dbName;


DbFacade::DbFacade(QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase(driver, "usb_db");
    db.setDatabaseName(dbName);
    if (!db.open())
        throw std::runtime_error("Can't open db.");
}

void DbFacade::InitDbFacade(QString Driver, QString DbName)
{
    driver = Driver;
    dbName = DbName;
    instance = new DbFacade();
}

DbFacade *DbFacade::Instance()
{
    if (instance == nullptr)
        throw std::runtime_error("Not initialized dbfacade");
    else
        return instance;
}

QSqlQuery *DbFacade::CreateQuery()
{
    db = QSqlDatabase::database("usb_db");
    if (!db.isOpen())
    {
        if (!db.open())
            throw std::runtime_error("Can't open db.");
    }
    QSqlQuery *query = new QSqlQuery(db);
    return query;
}
