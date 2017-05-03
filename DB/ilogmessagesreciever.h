#pragma once

#include <QObject>
#include "logmessage.h"

class ILogMessagesReciever
{
public:
    Q_SLOT virtual void onLogMessage(LogMessage message) = 0;
};
