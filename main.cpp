#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator *qt_translator = new QTranslator();
    if(qt_translator->load(":/Tr/qtbase_ru.qm"))
        {
            a.installTranslator(qt_translator);
        }

    if(!createConnection())
        return 1;
    MainWindow w;
    w.show();

    return a.exec();
}
