#include "usbbaseserver.h"
#include "clienthandler.h"
#include <QTcpServer>

UsbBaseServer::UsbBaseServer(quint16 ServerPort, QObject *parent) :
    QObject(parent),
    serverPort(ServerPort)
{
    listener = new QTcpServer;
    handlers = new QList<ClientHandler*>;
    connect(listener, &QTcpServer::newConnection, this, &UsbBaseServer::onNewConnection);
}

void UsbBaseServer::StartWorking()
{
    if (!listener->listen(QHostAddress::Any, serverPort))
    {
        listener->close();
        throw listener->errorString();
    }
    else
    {
        QString message = QString("Server started at %1 port").arg(serverPort);
        emit logMessage(LogMessage::Create(message));
    }
}

void UsbBaseServer::StopWorking()
{
    listener->close();
    emit logMessage(LogMessage::Create("Server stopped"));
}

void UsbBaseServer::onNewConnection()
{
    QTcpSocket *client = listener->nextPendingConnection();
    ClientHandler *handler = new ClientHandler(client, this);
    handlers->push_back(handler);
    connect(handler, &ClientHandler::clientDisconnected, this, &UsbBaseServer::onClientDisconnected);
}

void UsbBaseServer::onClientDisconnected()
{
    ClientHandler *handler = reinterpret_cast<ClientHandler*>(sender());
    handlers->removeOne(handler);
    handler->Close();
    disconnect(handler, &ClientHandler::clientDisconnected, this, &UsbBaseServer::onClientDisconnected);
    handler->deleteLater();
}
