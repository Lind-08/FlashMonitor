#ifndef USBINFOFINDER_H
#define USBINFOFINDER_H

#include <QObject>
#include <usbinfo.h>
#include <QRunnable>

class UsbInfoFinder : public QObject
{
    Q_OBJECT
    UsbInfo *infoObject;
public:
    void run();
    explicit UsbInfoFinder(QObject *parent = 0, UsbInfo *info = nullptr);
    UsbInfo *getInfo();
signals:
    void informationFinded(UsbInfoFinder*);
public slots:

};

#endif // USBINFOFINDER_H
