#include "testwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include "DB/dbfacade.h"


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
        if (settings.contains("db/path"))
            a->setProperty("dbpath", settings.value("db/path"));
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
    try
    {
        auto dbPath = a.property("dbpath").toString();
        if (QFile::exists(dbpath))
            DbFacade::InitDbFacade("QSQLITE", dbPath);
        else
        {
            qDebug() << QObject::tr("Не найден файл базы данных");
            return 0;
        }
    }
    catch (std::exception &e)
    {
        qDebug() << QString(e.what());
        return a.exec();
    }
    testwindow w;
    return a.exec();
}
