#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
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

private:
    Ui::testwindow *ui;
};

#endif // TESTWINDOW_H
