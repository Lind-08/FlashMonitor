#include "testwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QMessageBox>


bool loadSettings()
{
    if (!QFile::exists("client.ini"))
        return false;
    else
    {
        QSettings settings("client.ini", QSettings::IniFormat);
        auto a = qApp;
        bool result = true;
        if (settings.contains("server/address"))
            a->setProperty("address", settings.value("server/address"));
        else
            result = false;
        if (settings.contains("server/port"))
            a->setProperty("port", settings.value("server/port"));
        else
            result = false;
        if (settings.contains("client/secret"))
            a->setProperty("secret", settings.value("client/secret"));
        else
            result = false;
        if (settings.contains("client/name"))
            a->setProperty("name", settings.value("client/name"));
        else
            result = false;
        return result;
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!loadSettings())
    {
        QMessageBox::critical(nullptr,"Ошибка","Не удалось загрузить настройки");
        return 0;
    }
    testwindow w;
    return a.exec();
}
