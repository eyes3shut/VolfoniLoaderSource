#include <QApplication>
#include <QWidget>
#include <QtWidgets>
#include "mainwindow.h"
#include "tools.h"
#include <QTranslator>
#include <QtXml>
#include <iostream>
#include <QTextCodec>
#include <QFile>

int main(int argc, char *argv[])
{
    // Fix pour macOS - Active le rendu avec couches (layers) pour éviter fenêtre transparente
    #ifdef Q_OS_MAC
        qputenv("QT_MAC_WANTS_LAYER", "1");
    #endif
    QString fileName = QString("../Config/language.xml");

    QApplication a(argc, argv);

    ///////////////// Verification autre Appli Volfoni en cours
#ifndef Q_OS_MAC
    QProcess x;
    x.start("tasklist");
    x.waitForStarted(1000);
    x.waitForReadyRead(1000);
    x.waitForFinished(1000);
    QString result( x.readAllStandardOutput() );
    if( (result.count("Volfoni_Manager.exe",Qt::CaseInsensitive) >=1 ) || (result.count("VolfoniLoader.exe",Qt::CaseInsensitive) >=2 ) || (result.count("USB_Tool.exe",Qt::CaseInsensitive) >=1 ) )
    {
        QMessageBox msgBox(QMessageBox::Critical,"ERROR","<p align='center'>Another Volfoni software is executing !<br><br>Please close all other Volfoni software before using Volfoni Loader.</p>");
        msgBox.exec();
        return 0;
    }
    
#endif
    ///////////////////////////////////////////////////////////

#ifdef Q_OS_MAC
    QDir bundleDir(a.applicationDirPath());
    bundleDir.cdUp();
    bundleDir.cd("Resources");
    fileName = bundleDir.absolutePath()+"/language.xml";
#endif
    if(QFile::exists(fileName)==false){
        SelectLanguageInit sl(fileName);
        sl.exec();

    }else{
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning("Unable to open versioning information");
        }

        QDomDocument settings(fileName);

        QString errorMsg;
        if(!settings.setContent(&file, &errorMsg)){ //handle error (print error message on pane for instance)
            std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
            file.close();
        }
        file.close();

        //parse the settings tree and restore state
        QDomElement root = settings.documentElement();
        QDomElement aa = root.firstChildElement("langSelected");
        QString langSelected = aa.text();
        if(langSelected==""){
            SelectLanguageInit sl(fileName);
            sl.exec();
        }
    }

    MainWindow w(0,fileName);
    w.show();

    return a.exec();
}
