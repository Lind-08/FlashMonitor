#include "testwindow.h"
#include "ui_testwindow.h"
#include "mainwindow.h"
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include "usbbase.h"

testwindow::testwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::testwindow)
{
    ui->setupUi(this);
    createTrayActions();
    createTrayMenu();
    trayIcon->show();
    detector = new MainWindow(this);
    auto base = UsbBase::Instance();
    //connect(base, &UsbBase::newDevice, this, &testwindow::onDeviceArrived);
}

testwindow::~testwindow()
{
    delete ui;
}

void testwindow::onDeviceArrived(HANDLE ptr, QChar letter)
{
    trayIcon->showMessage("Новое устройство", "Подключено новое устройство. Получение информации об устройстве...");
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
