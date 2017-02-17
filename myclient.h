#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QThread>
#include <QTcpSocket>
#include <QQueue>

class MyClient : public QObject
{
    Q_OBJECT
protected:
    QTcpSocket *socket;
    quint16 _port;
    QString _address;
    bool _status;
    bool _isWaitingAnswer;
    QQueue<QString> _messages;

private:
    void sendMessage();

public:
    explicit MyClient(QObject *parent = 0);

private slots:
    void socketStateChanged(QAbstractSocket::SocketState);

public slots:
    void Connect(QString, quint16);
    void Disconnect();
    void ReadyRead();
    void Error();
    void Send(QString);

signals:
    void recievedMessage(QString);
    void logMessage(QString);
    void changeStatus(bool);
    void error(QString);
};

#endif // MYCLIENT_H
