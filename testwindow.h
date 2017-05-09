#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include "windows.h"
class QSystemTrayIcon;
class QCloseEvent;

class MainWindow;
class BaseForm;
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
    BaseForm *baseWindow;
    QAction *eventsAction;
    QAction *adminAction;
    QAction *switchAction;
    QAction *baseAction;
    QAction *quitAction;

    void closeEvent(QCloseEvent *event);

    void createTrayIcon();
    void createTrayActions();
    void createTrayMenu();
    void createAdminTrayActions();
    void createAdminTrayMenu();

public:
    explicit testwindow(QWidget *parent = 0);
    ~testwindow();
public slots:
    void onDeviceConnected(UsbInfo* info);
    void onDeviceDisconnected(UsbInfo* info);
    void onError(QString error);
private:
    Ui::testwindow *ui;
private slots:
    void onAdminActionTriggered();
    void onSwitchActionTriggered();
    void onBaseActionTriggered();

};

#endif // TESTWINDOW_H
