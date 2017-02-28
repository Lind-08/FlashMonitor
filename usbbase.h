#ifndef USBBASE_H
#define USBBASE_H

#include <QObject>
#include "usbinfo.h"
#include "usbinfofinder.h"
#include <QHash>
#include <QVector>
#include "usbclient.h"

class UsbBase : public QObject
{

    Q_OBJECT
    static UsbBase *_instance;
    QHash<QChar,UsbInfo*> connectedDevices;
    QVector< UsbInfo*> base;


    UsbInfo * getInfo(UsbInfo* info);
    void unlockDevice(UsbInfo *info);
    void addRule(UsbInfo* info);
    UsbClient *client;

    void newDevice(UsbInfo* info);
signals:
    void deviceConnected(UsbInfo*);
    void deviceDisconnected(UsbInfo*);
    void error(QString);

public:
    explicit UsbBase(QObject *parent = 0);
    static UsbBase *Instance()
    {
        if (_instance == nullptr)
        {
            _instance = new UsbBase();
        }
        return _instance;
    }

public slots:
    void addUsbStorage(HANDLE handle, QChar letter);
    void removeUsbStorage(QChar letter);
    void informationFinded(UsbInfoFinder* finder);

};

#endif // USBBASE_H
