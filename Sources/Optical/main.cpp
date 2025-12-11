#include <QtGui/QApplication>
#include "mainwindow.h"
#include "crypto.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString fileName = QString("key.txt");

#ifdef Q_OS_MAC
    QDir bundleDir(a.applicationDirPath());
    bundleDir.cdUp();
    bundleDir.cd("Resources");
    fileName = bundleDir.absolutePath()+"/key.txt";
#endif

    Crypto c(true, fileName);
    if(!c.sameHashInFile()) {
        c.exec();
        if(!c.sameHashInFile()) return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
