#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString fileName = "";
#ifdef Q_OS_MAC
    QDir bundleDir(a.applicationDirPath());
    bundleDir.cdUp();
    bundleDir.cd("Resources");
    fileName = bundleDir.absolutePath()+"/";
#endif
    MainWindow w(0,fileName);
    w.show();

    return a.exec();
}
