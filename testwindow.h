#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include "windows.h"
class QSystemTrayIcon;
class QCloseEvent;

class MainWindow;

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
    void onDeviceArrived(HANDLE ptr, QChar letter);
private:
    Ui::testwindow *ui;
};

#endif // TESTWINDOW_H
