#include "testwindow.h"
#include "ui_testwindow.h"
#include "mainwindow.h"
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include "usbbase.h"
#include "baseform.h"

testwindow::testwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::testwindow)
{
    ui->setupUi(this);
    createTrayIcon();
    createTrayActions();
    createAdminTrayActions();
    createTrayMenu();
    trayIcon->setContextMenu(trayMenu);
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

void testwindow::onAdminActionTriggered()
{
    if (adminAction->isChecked())
    {
        createAdminTrayMenu();
        trayMenu->show();

    }
    else
    {
        createTrayMenu();
    }
    trayIcon->setContextMenu(trayMenu);
}

void testwindow::onSwitchActionTriggered()
{
    if (switchAction->isChecked())
    {
        detector->StartMonitoring();
        switchAction->setText("Защита включена");
    }
    else
    {
        detector->StopMonitoring();
        switchAction->setText("Защита отключена");

    }
}

void testwindow::onBaseActionTriggered()
{
    //
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
    adminAction = new QAction(tr("Режим &администратора"), this);
    adminAction->setCheckable(true);
    connect(adminAction, &QAction::triggered, this, &testwindow::onAdminActionTriggered);
}

void testwindow::createTrayMenu()
{
    trayMenu = new QMenu(this);
    trayMenu->addAction(adminAction);
}

void testwindow::createAdminTrayActions()
{
    switchAction = new QAction(tr("Защита включена"), this);
    switchAction->setCheckable(true);
    switchAction->setChecked(true);
    connect(switchAction, &QAction::triggered, this, &testwindow::onSwitchActionTriggered);

    /*baseAction = new QAction(tr("Список носителей"), this);
    connect(baseAction, &QAction::triggered, this, &testwindow::onBaseActionTriggered);*/
    quitAction = new QAction(tr("&Выход"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void testwindow::createAdminTrayMenu()
{
    trayMenu = new QMenu(this);
    trayMenu->addAction(switchAction);
    //trayMenu->addAction(baseAction);
    trayMenu->addAction(adminAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
}

void testwindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void testwindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/usb.ico"));
}
