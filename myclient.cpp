#include "myclient.h"


MyClient::MyClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Error()));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
}

void MyClient::Send(QString message)
{
    QTextStream stream(socket);
    stream << message;
}

bool MyClient::Connect(QString address, quint16 port)
{
    if (!_status)
    {
        _address = address;
        _port = port;
        socket->connectToHost(_address, _port);
        if (socket->waitForConnected())
            return true;
    }
    else
    {
        QString msg = tr("Client has already connected");
        emit logMessage(msg);
        return false;
    }
}

void MyClient::Disconnect()
{
    socket->disconnectFromHost();
    if (socket->waitForDisconnected())
        socket->close();
}

QString MyClient::Read()
{
    if (socket->waitForReadyRead())
    {
        QTextStream stream(socket);
        QString message = stream.readAll();
        return message;
    }
}

void MyClient::Error()
{
    emit error(socket->errorString());
}


void MyClient::socketStateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::ConnectedState)
    {
        _status = true;
        emit logMessage("Connected to server.");
    }
    else
    if (state == QAbstractSocket::UnconnectedState)
    {
        _status = false;
        emit logMessage("Disconnected from server.");
    }
    emit changeStatus(_status);
}
