#include "logmessage.h"
#include <QDateTime>

LogMessage LogMessage::Create(QString Message)
{
    LogMessage message(Message, QDateTime::currentDateTime());
    return message;
}

LogMessage::LogMessage(LogMessage &&msg):
    QObject(msg.parent()),
    message(msg.message),
    timestamp(msg.timestamp)
{
    msg.deleteLater();
}

LogMessage::LogMessage(const LogMessage &msg):
    QObject(msg.parent()),
    message(msg.message),
    timestamp(msg.timestamp)
{

}

LogMessage::LogMessage(QString Message, QDateTime Timestamp, QObject *parent):
    QObject(parent),
    message(Message),
    timestamp(Timestamp)
{

}

