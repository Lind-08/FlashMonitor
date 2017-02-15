#include "testwindow.h"
#include "mainwindow.h"
#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    testwindow w;
    return a.exec();
}
