#pragma once
#include <QObject>
#include "logmessage.h"

class ILogMessagesCreator
{
    Q_SIGNAL virtual void logMessage(LogMessage message) = 0;
};
