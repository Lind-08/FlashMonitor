#include "testwindow.h"
#include "ui_testwindow.h"
#include "mainwindow.h"

testwindow::testwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::testwindow)
{
    ui->setupUi(this);
    createTrayActions();
    createTrayMenu();
    trayIcon->show();
    detector = new MainWindow(this);
}

testwindow::~testwindow()
{
    delete ui;
}

void testwindow::createTrayActions()
{
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void testwindow::createTrayMenu()
{
    trayMenu = new QMenu(this);
    trayMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);
}

void testwindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}
