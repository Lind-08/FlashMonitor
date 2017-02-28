#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include "windows.h"
class QSystemTrayIcon;
class QCloseEvent;

class MainWindow;
class UsbInfo;

namespace Ui {
class testwindow;
}

class testwindow : public QMainWindow
{
    Q_OBJECT
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    MainWindow *detector;
    QAction *eventsAction;

    QAction *quitAction;

    void closeEvent(QCloseEvent *event);


    void createTrayActions();
    void createTrayMenu();
public:
    explicit testwindow(QWidget *parent = 0);
    ~testwindow();
public slots:
    void onDeviceConnected(UsbInfo* info);
    void onDeviceDisconnected(UsbInfo* info);
    void onError(QString error);
private:
    Ui::testwindow *ui;
};

#endif // TESTWINDOW_H
