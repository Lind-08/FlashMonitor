#pragma once

#include <QObject>
#include <QTcpSocket>

class MyClient : public QObject
{
    Q_OBJECT
protected:
    QTcpSocket *socket;
    quint16 _port;
    QString _address;
    bool _status;

private:
    void sendMessage();

public:
    explicit MyClient(QObject *parent = 0);
    QString Read();

private slots:
    void socketStateChanged(QAbstractSocket::SocketState);

public slots:
    bool Connect(QString address, quint16 port);
    void Disconnect();
    void Send(QString);
    void Error();

signals:
    void logMessage(QString);
    void changeStatus(bool);
    void error(QString);
};

