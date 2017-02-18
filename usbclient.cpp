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

UsbClient::UsbClient(QObject *parent) : QObject(parent)
{

}

QJsonObject UsbClient::ConnectToServer(QString secret, QString name)
{
    if (connectToServer(address, port))
    {
        return authenticate(secret, name);
    }
    else
    {
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
    QJsonObject request;
    request["code"] = "BYE";
    send(request);
    auto answer = recieve();
    return answer;
}

