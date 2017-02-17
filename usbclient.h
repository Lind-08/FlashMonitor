#ifndef USBCLIENT_H
#define USBCLIENT_H

#include <QObject>

class UsbClient : public QObject
{
    Q_OBJECT
public:
    explicit UsbClient(QObject *parent = 0);

signals:

public slots:
};

#endif // USBCLIENT_H