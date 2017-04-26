#pragma once
#include <QObject>
#include "ilogmessagescreator.h"

class QTcpServer;
class ClientHandler;

class UsbBaseServer : public QObject, public ILogMessagesCreator
{
    Q_OBJECT
    quint16 serverPort;
    QTcpServer *listener;
    QList<ClientHandler*> *handlers;

public:
    explicit UsbBaseServer(quint16 ServerPort, QObject *parent = 0);
    void StartWorking();
    void StopWorking();

private slots:
    void onNewConnection();
    void onClientDisconnected();

signals:
    void logMessage(LogMessage);
};
