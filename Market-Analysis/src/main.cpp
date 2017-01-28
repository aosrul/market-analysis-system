#include <QApplication>
#include <QDir>
#include "include/presenter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setOrganizationName("Terentew Aleksey");
    qApp->setOrganizationDomain("https://www.mql5.com/ru/users/terentjew23");
    qApp->setApplicationName("Market Analysis System");
    qApp->setApplicationVersion("1.2.0-alpha");

    QString mDir = a.applicationDirPath();
    mDir += "/Market Kits";
    if( !QDir().exists(mDir) )
        QDir().mkdir(mDir);

    Presenter p;
    p.openMainWindow();

    return a.exec();
}
