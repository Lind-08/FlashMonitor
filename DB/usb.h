#pragma once
#include <QObject>

class Usb : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint32 ID READ ID WRITE setID)
    Q_PROPERTY(QString VID READ VID WRITE setVID)
    Q_PROPERTY(QString PID READ PID WRITE setPID)
    Q_PROPERTY(QString Serial READ Serial WRITE setSerial)
    Q_PROPERTY(QString Name READ Name WRITE setName)

    qint32 id;
    QString vid;
    QString pid;
    QString serial;
    QString name;

    explicit Usb(QObject *parent = 0);
public:
    static Usb* Create(QObject *parent = 0);

    static const int INVALID_ID = -1;

    void setID(const qint32 ID);
    inline qint32 ID()
    {
        return id;
    }


    void setVID(const QString VID);
    inline QString VID()
    {
        return vid;
    }

    void setPID(const QString PID);
    inline QString PID()
    {
        return pid;
    }

    void setSerial(const QString Serial);
    inline QString Serial()
    {
        return serial;
    }

    void setName(const QString Name);
    inline QString Name()
    {
        return name;
    }
};


