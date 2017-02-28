#include "testwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSettings>


void loadSettings()
{
    QSettings settings("client.ini", QSettings::IniFormat);
    auto a = qApp;
    a->setProperty("address", settings.value("server/address"));
    a->setProperty("port", settings.value("server/port"));
    a->setProperty("secret", settings.value("client/secret"));
    a->setProperty("name", settings.value("client/name"));
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadSettings();

    testwindow w;
    return a.exec();
}
