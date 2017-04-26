#pragma once
#include <QList>

template<typename T>
class IRepository
{
public:
    virtual QList<T*> GetAll() = 0;
    virtual void Save(T* object) = 0;
    virtual void Delete(T* object) = 0;
};

//TODO: Попробовать создать базовый класс для репозиториев
