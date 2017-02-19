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
    QVector< QPair<UsbInfo*,bool> > base;

    bool checkInBase(UsbInfo *info);
    QPair<UsbInfo *, bool> getPair(UsbInfo* info);
    void unlockDevice(UsbInfo *info);

    UsbClient *client;

signals:
    void newDevice(UsbInfo*);
    void deviceConnected(UsbInfo*);
    void deviceDisconnected(UsbInfo*);

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
    void newDeviceResult(int result, UsbInfo *info);

};

#endif // USBBASE_H
