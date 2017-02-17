#include "myclient.h"

void MyClient::sendMessage()
{
    _isWaitingAnswer = true;
}

MyClient::MyClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;
    connect(socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Error()));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    _isWaitingAnswer = false;
}

void MyClient::Send(QString message)
{
    QTextStream stream(socket);
    stream << message;
}

void MyClient::Connect(QString address, quint16 port)
{
    if (!_status)
    {
        _address = address;
        _port = port;
        socket->connectToHost(_address, _port);
    }
    else
    {
        QString msg = tr("Client has already connected");
        emit logMessage(msg);
    }
}

void MyClient::Disconnect()
{
    socket->close();
}

void MyClient::ReadyRead()
{
    QTextStream stream(socket);
    QString message = stream.readAll();
    emit recievedMessage(message);
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
