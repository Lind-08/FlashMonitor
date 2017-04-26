#include "host.h"

Host::Host(QObject *parent) : QObject(parent)
{

}

Host *Host::Create(QObject *parent)
{
    auto host = new Host(parent);
    host->setID(INVALID_ID);
    return host;
}
