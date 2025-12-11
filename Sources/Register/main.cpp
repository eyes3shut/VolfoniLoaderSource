#include <QtGui/QApplication>
#include "../Optical/crypto.h"

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

    Crypto c(false, fileName);
    c.show();

    return a.exec();
}
