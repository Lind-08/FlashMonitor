#pragma once
#include "property.h"
#include <QObject>

class Host : public QObject
{
    Q_OBJECT
    PROPERTY_GS(qint32, ID, id)
    PROPERTY_GS(QString, Secret, secret)
    PROPERTY_GS(QString, Name, name)
    PROPERTY_GS(bool, Status, status)
private:
    explicit Host(QObject *parent = 0);
public:
    static Host *Create(QObject *parent = 0);
    static const int INVALID_ID = -1;
};

