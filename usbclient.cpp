#include "usbclient.h"
#include <QTcpSocket>
#include "myclient.h"
#include <QJsonDocument>
#include <QJsonObject>

void UsbClient::send(QJsonObject request)
{
    client->Send(QString(QJsonDocument(request).toJson()));
}

QJsonObject UsbClient::recieve()
{
    QString answer = client->Read();
    return QJsonDocument::fromJson(answer.toLocal8Bit()).object();
}

bool UsbClient::connectToServer(QString address, quint32 port)
{
    return client->Connect(address,port);
}

QJsonObject UsbClient::authenticate(QString secret, QString name)
{
    QJsonObject request;
    request["code"] = "HELLO";
    request["secret"] = secret;
    request["name"] = name;
    send(request);
    auto answer = recieve();
    return answer;
}

void UsbClient::on_clientStateChanged(bool state)
{
    isConnected = state;
}

UsbClient::UsbClient(QObject *parent) : QObject(parent)
{
    client = new MyClient(this);
    connect(client, &MyClient::changeStatus, this, &UsbClient::on_clientStateChanged);
}

QJsonObject UsbClient::ConnectToServer()
{
    if (connectToServer(address, port))
    {
        isConnected = true;
        return authenticate(secret, name);
    }
    else
    {
        isConnected = false;
        QJsonObject answer;
        answer["code"] = "ERROR_CON";
        return answer;
    }
}

QJsonObject UsbClient::GetRule(QString VID, QString PID)
{
    QJsonObject request;
    request["code"] = "GET_RULE";
    request["VID"] = VID;
    request["PID"] = PID;
    send(request);
    auto answer = recieve();
    return answer;
}

QJsonObject UsbClient::GetRulesHash()
{
    QJsonObject request;
    request["code"] = "GET_RULES_HASH";
    send(request);
    auto answer = recieve();
    return answer;
}

QJsonObject UsbClient::Disconnect()
{
    isConnected = true;
    QJsonObject request;
    request["code"] = "BYE";
    send(request);
    auto answer = recieve();
    return answer;
}

