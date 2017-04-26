#pragma once
#include <irepository.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include "dbfacade.h"
#include <stdexcept>

template <class T>
class BaseSqlRepository : public IRepository<T>
{
protected:
    const QString GET_ID_QUERY = "SELECT id FROM %1 ORDER BY id DESC;";

    virtual QString getInsertQuery(T *object) = 0;
    virtual QString getUpdateQuery(T *object) = 0;
    virtual QString getDeleteQuery(T *object) = 0;
    virtual QString getQueryForID() = 0;

    void execQuery(QString queryString);
    QSqlQuery *execQueryWithResult(QString queryString);

    void create(T *object);
    int getIdAfterInsert();

    void update(T *object);
    void remove(T *object);

public:
    explicit BaseSqlRepository();
};

template<class T>
BaseSqlRepository<T>::BaseSqlRepository()
{

}

template<class T>
void BaseSqlRepository<T>::execQuery(QString queryString)
{
    auto query = execQueryWithResult(queryString);
}

template<class T>
QSqlQuery *BaseSqlRepository<T>::execQueryWithResult(QString queryString)
{
    auto db = DbFacade::Instance();
    QSqlQuery *query = db->CreateQuery();
    if(!query->exec(queryString))
    {
        throw std::runtime_error(query->lastError().text().toStdString());
    }
    return query;
}

template<class T>
void BaseSqlRepository<T>::create(T *object)
{
    QString queryString = getInsertQuery(object);
    execQueryWithResult(queryString);
}

template<class T>
int BaseSqlRepository<T>::getIdAfterInsert()
{
    try
    {
        auto query = execQueryWithResult(getQueryForID());
        if (!query->isValid())
            query->next();
        int result = query->value(0).toInt();
        return result;
    }
    catch(...)
    {
        return -1;
    }
}

template<class T>
void BaseSqlRepository<T>::update(T *object)
{
    QString queryString = getUpdateQuery(object);
    execQueryWithResult(queryString);
}

template<class T>
void BaseSqlRepository<T>::remove(T *object)
{
    QString queryString = getDeleteQuery(object);
    execQueryWithResult(queryString);
}
