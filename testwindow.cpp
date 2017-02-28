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
    connect(base, &UsbBase::deviceConnected, this, &testwindow::onDeviceConnected);
    connect(base, &UsbBase::deviceDisconnected, this, &testwindow::onDeviceDisconnected);
    connect(base, &UsbBase::error, this, &testwindow::onError);
    detector->StartMonitoring();
}

testwindow::~testwindow()
{
    delete ui;
}

void testwindow::onDeviceDisconnected(UsbInfo *info)
{
    trayIcon->showMessage("Устройство извлечено", info != nullptr ? info->name : QString());
}

void testwindow::onError(QString error)
{
    trayIcon->showMessage("Ошибка", error, QSystemTrayIcon::Critical);
}

void testwindow::onDeviceConnected(UsbInfo *info)
{
    QString message;
    if (info->state == UsbState::full)
    {
        message = QString("Подключено новое устройство. %1.").arg(info->name);
    }
    else
    if (info->state == UsbState::apply)
    {
        message = QString("Подключено новое устройство. %1. Доступ разрешен.").arg(info->name);
    }
    else
    if (info->state == UsbState::blocked)
    {
         message = QString("Подключено новое устройство. %1. Доступ блокирован.").arg(info->name);
    }
    else
        message = "Подключено новое устройство. Получение информации об устройстве...";
    trayIcon->showMessage("Новое устройство", message);
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
    trayIcon->setIcon(QIcon(":/usb.ico"));
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
