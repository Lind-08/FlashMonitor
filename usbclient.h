#pragma once

#include <QObject>
class QTcpSocket;
class QJsonObject;
class MyClient;

class UsbClient : public QObject
{
    Q_OBJECT
    MyClient *client;
    QString address;
    quint32 port;
    void send(QJsonObject request);
    QJsonObject recieve();


    bool connectToServer(QString address, quint32 port);
    QJsonObject authenticate(QString secret, QString name);


public:
    explicit UsbClient(QObject *parent = 0);

    inline void setAddress(QString Address)
    {
        address = Address;
    }

    inline void setPort(quint32 Port)
    {
        port = Port;
    }


signals:

public slots:
    QJsonObject ConnectToServer(QString secret, QString name);
    QJsonObject GetRule(QString VID, QString PID);
    QJsonObject GetRulesHash();
    QJsonObject Disconnect();
};
