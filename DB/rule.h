#pragma once
#include <QObject>
#include "property.h"

class Rule : public QObject
{
    Q_OBJECT
    PROPERTY_GS(quint32, ID, id)
    PROPERTY_GS(quint32, Usb_ID, usb_id)
    PROPERTY_GS(quint32, Host_ID, host_id)
    PROPERTY_GS(bool, Value, value)

    explicit Rule(QObject *parent = 0);
public:
    static Rule *Create(QObject *parent = 0);
    static const quint32 INVALID_ID = -1;
};

