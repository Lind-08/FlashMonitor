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
    QString secret;
    QString name;
    quint32 port;
    void send(QJsonObject request);
    QJsonObject recieve();

    bool isConnected = false;

    bool connectToServer(QString address, quint32 port);
    QJsonObject authenticate(QString secret, QString name);

private slots:
    void on_clientStateChanged(bool state);

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

    inline void setSecret(QString Secret)
    {
        secret = Secret;
    }

    inline void setName(QString Name)
    {
        name = Name;
    }

    QJsonObject ConnectToServer();
    QJsonObject GetRule(QString VID, QString PID);
    QJsonObject GetRulesHash();
    QJsonObject Disconnect();

signals:

public slots:
};
