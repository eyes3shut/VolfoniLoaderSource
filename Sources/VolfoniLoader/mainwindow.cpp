#include <QtGui>
#include <QList>
#include <QtNetwork>

#include "mainwindow.h"

#include "login.h"
#include "listUpdate.h"
#include "tools.h"
#include "parameters.h"
#include "parameters_AME.h"
#include "parameters_SCP.h"
#include "batteryview.h"
#include "proglcd.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include <QObject>

#include <QAction>
#include <QTranslator>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent, QString fileLang)
    : QMainWindow(parent),
      log_advancedUser("admin"),
      pwd_advancedUser("volfoni"),
      log_adminUser("administrator"),
      pwd_adminUser("volfoni123"),
      widthWindow(900),
//Modif AT      versionSoftWare("03.08.09"),// a updater si nouvelle version publiée
      versionSoftWare("03.08.AT"),// a updater si nouvelle version publiée
      _fileLang(fileLang)

{
    qDebug("MainWindow");

    QString fileName = QString(_fileLang);
    QFile fileD(fileName);

    if (!fileD.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Unable to open versioning information");
    }

    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(&fileD, &errorMsg)){ //handle error (print error message on pane for instance)
        std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
        fileD.close();
    }
    fileD.close();

    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    QDomElement aa = root.firstChildElement("langSelected");
    QString langSelected = aa.text();

    nLang = 0;
    translatorFR = new QTranslator(this);
    translatorFR->load(":/MainWindow/trad_French",".");

    translatorEN = new QTranslator(this);
    translatorEN->load(":/MainWindow/trad_English",".");

    translatorSP = new QTranslator(this);
    translatorSP->load(":/MainWindow/trad_Spanish",".");

    translatorGR = new QTranslator(this);
    translatorGR->load(":/MainWindow/trad_German",".");

    if(langSelected=="english"){
        qApp->installTranslator(translatorEN);
        nLang = 1;
    }
    else if(langSelected=="french"){
        qApp->installTranslator(translatorFR);
        nLang = 2;
    }
    else if(langSelected=="german"){
        qApp->installTranslator(translatorGR);
        nLang = 3;
    }
    else if(langSelected=="spanish"){
        qApp->installTranslator(translatorSP);
        nLang = 4;
    }

    setFixedSize(widthWindow,475);
    setWindowTitle(tr("Volfoni Loader - Updater for Volfoni products"));
    setWindowIcon(QIcon(":/MainWindow/ressources/volfo160.ico"));

    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateStatusBar()));
    _timer->start(10000); // influe sur la vitesse de défilement de QStatusBar

    status = statusBar();
    status->show();
    status->setFixedHeight(15);

    _msg = adaptTextQStatusBar(tr("Volfoni Loader, updater for Volfoni products"));
    statutmsg = 1;

    trigrammeProduct = "";
    customerNumber = "";
    enableCheckUpdate = false;
    previousRev = "";
    selectedRev = "";
    for(int i=0; i<10; i++) glassesID[i] = "";
    batterieIndex=0;

    showMenu();
    _localFile = false; // le mode admin si on prend un fichier local

    zonecentrale = new QWidget;
    zonecentrale->setFixedSize(widthWindow,470);

    batterieBar = new QProgressBar(zonecentrale);
    batterieBar->setTextVisible(false);
    batterieBar->setFixedSize(380,40);
    batterieBar->move(485,10);
    label_maj = new QLabel(zonecentrale);
    label_maj->setText("");
    statutLabelmaj = 1;
    label_maj->setFixedSize(400,250);
    label_maj->move(485,10);
    label_maj->setAlignment(Qt::AlignCenter);
    widget = new QWidget(zonecentrale);
    label = new QLabel(widget);
    animatedgif = new QMovie(":/MainWindow/ressources/loading.gif", QByteArray(), this);
    label->setMovie(animatedgif);
    label->setFixedSize(100,50);
    label->setAlignment(Qt::AlignCenter);
    animatedgif->start();
    widget->move(640,260);
    button = new QPushButton(zonecentrale);
    button->setText(tr("Quit"));
    button->setCursor(Qt::PointingHandCursor);
    button->setShortcut(tr("Ctrl+Q"));
    button->setFixedSize(200,25);
    button->move(600,410);

    frame = new QFrame(zonecentrale);
    frame->setFrameStyle(QFrame::VLine | QFrame::Raised);
    frame->setLineWidth(0);
    frame->setMidLineWidth(3);
    frame->setGeometry(450, 20, 3, 410);

    label_pic = new QLabel(zonecentrale);
    label_pic->setFixedSize(210,100);
    label_pic->setAlignment(Qt::AlignCenter);
    animatedUSB = new QMovie(":/MainWindow/ressources/connectUSB.gif", QByteArray(), this);
    animatedUSB->start();
    label_pic->setMovie(animatedUSB);
    label_pic->setStyleSheet("background: white");
    label_pic->move(25,10);

    label_picture = new QLabel(zonecentrale);
    label_picture->setFixedSize(400,300);
    label_picture->setAlignment(Qt::AlignCenter);
    label_picture->setPixmap(QPixmap(":/MainWindow/ressources/default.png"));
    label_picture->setStyleSheet("background: white");
    label_picture->move(25,110);

    label_desc = new QLabel(zonecentrale);
    label_desc->setFixedSize(190,100);
    label_desc->setAlignment(Qt::AlignCenter);
    label_desc->setText(tr("Please connect your \nVolfoni device \nin update mode\nto a free USB port."));
    statutLabelDesc =1;
    label_desc->setStyleSheet("background: white");
    label_desc->move(235,10);

    label_help = new QLabel(zonecentrale);
    label_help->setFixedSize(30,30);
    label_help->setAlignment(Qt::AlignCenter);
    label_help->setCursor(Qt::PointingHandCursor);
    label_help->setPixmap(QPixmap(":/MainWindow/ressources/helpSmall.png"));
    label_help->setStyleSheet("background: white");
    label_help->move(320,90);

    progressbar = new QProgressBar(zonecentrale);
    progressbar->setTextVisible(false);
    progressbar->setFixedSize(400,20);
    progressbar->move(25,415);

    setCentralWidget(zonecentrale);

    button_maj = new QPushButton(zonecentrale);
    button_maj->setFixedSize(200,25);
    button_maj->setText(tr("Update now !"));
    button_maj->setCursor(Qt::PointingHandCursor);
    button_maj->move(600,260);
    button_maj->hide();     //Attention, non affiché !!!
    connect(button_maj, SIGNAL(clicked()), this, SLOT(UpdateFirmwareClicked()) );

    button_checkupdate = new QPushButton(zonecentrale);
    button_checkupdate->setFixedSize(200,25);
    button_checkupdate->setText(tr("Check update !"));
    button_checkupdate->move(600,370);
    button_checkupdate->setCursor(Qt::PointingHandCursor);
    button_checkupdate->hide(); // hide until no device connected
    connect(button_checkupdate, SIGNAL(clicked()), this, SLOT(checkupdaterequest()));

    waitConnection  = new QTimer();
    connect(waitConnection, SIGNAL(timeout()), this, SLOT(StopConnection()));

    timerDebugMsg = new QTimer();
    connect(timerDebugMsg, SIGNAL(timeout()), this, SLOT(askDebugMsg()));

    timer_usb = new QTimer();

    connect(button, SIGNAL(clicked()), this, SLOT(close()));
    connect(timer_usb, SIGNAL(timeout()), this, SLOT(Connection()));

    _pendingMouseButtonEvent = 0;
    //Make initial check to see if the USB device is attached
    product = new Product();
    connect(product, SIGNAL(httpFinished()), this, SLOT(GetLastVersion()));
    connect(product, SIGNAL(httpUpdateFinished()), this, SLOT(WriteDeviceFinished()));
    connect(product, SIGNAL(newMsgRFAvailable(unsigned char*)), this, SLOT(checkMsgRF(unsigned char*)));

    CheckConnected();

    timer_usb->start(1000); //Check connection toutes les secondes
}

MainWindow::~MainWindow()
{
    qDebug("~MainWindow");
    product->close();

    delete product;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::showMenu(){
    qDebug("showMenu");
      QString connectIcon = ":/MainWindow/ressources/login.png";
      QString helpIcon = ":/MainWindow/ressources/help.png";
      QString aboutIcon = ":/MainWindow/ressources/about.png";
      QString toolsIcon = ":/MainWindow/ressources/tools.png";
      QString returnIcon = ":/MainWindow/ressources/return.png";
      QString updateIcon = ":/MainWindow/ressources/update.png";
      QString internetIcon = ":/MainWindow/ressources/internet.png";
      QString languageIcon = ":/MainWindow/ressources/languages.png";

     _EditAdmin = new QAction(QIcon(connectIcon), tr("Log on"), this);
     connect(_EditAdmin, SIGNAL(triggered()), this, SLOT(log_onUser()));
     _EditAdmin->setEnabled(true);


     _HelpOnline = new QAction(QIcon(internetIcon), tr("Online support"), this);
     connect(_HelpOnline, SIGNAL(triggered()), this, SLOT(aboutVolfoni()));
     _HelpOnline->setShortcut(tr("Ctrl+F1"));

     _HelpOffline = new QAction(QIcon(helpIcon), tr("Help"), this);
     connect(_HelpOffline, SIGNAL(triggered()), this, SLOT(openHelp()));
     _HelpOffline->setShortcut(tr("F1"));

     _HelpAbout = new QAction(QIcon(aboutIcon), tr("About Volfoni Loader"), this);
     connect(_HelpAbout, SIGNAL(triggered()), this, SLOT(aboutPCLoader()));

     _HelpBootloader = new QAction(QIcon(aboutIcon), tr("How to switch the product in update mode?"), this);
     connect(_HelpBootloader, SIGNAL(triggered()), this, SLOT(helpBootloader()));

     _backup = new QAction(QIcon(returnIcon), tr("Return to previous version"), this);
     connect(_backup, SIGNAL(triggered()), this, SLOT(returnLastVersion()));
     _backup->setEnabled(false);
     _backup->setShortcut(tr("Ctrl+z"));

     _reset = new QAction(QIcon(returnIcon), tr("Reinitialize device"), this);
     connect(_reset, SIGNAL(triggered()), this, SLOT(resetDevice()));
     _reset->setEnabled(false);

     _parameters = new QAction(QIcon(returnIcon), tr("Parameters"), this);
     connect(_parameters, SIGNAL(triggered()), this, SLOT(setParameters()));
     _parameters->setEnabled(false);

     _battery = new QAction(QIcon(returnIcon), tr("Battery"), this);
     connect(_battery, SIGNAL(triggered()), this, SLOT(getBattery()));
     _battery->setEnabled(false);

     _progLCD = new QAction(QIcon(returnIcon), tr("Program"), this);
     connect(_progLCD, SIGNAL(triggered()), this, SLOT(setProgLCD()));
     _progLCD->setEnabled(false);

  /*   _dump_log = new QAction(QIcon(toolsIcon), tr("Logging"), this);
     connect(_dump_log, SIGNAL(triggered()), this, SLOT(dumpLog()));
     _dump_log->setEnabled(false);

     _erase_log = new QAction(QIcon(toolsIcon), tr("Erase Log"), this);
     connect(_erase_log, SIGNAL(triggered()), this, SLOT(EraseEeprom()));
     _erase_log->setEnabled(false);
*/
     _updatePCLOader = new QAction(QIcon(updateIcon), tr("Search for Volfoni Loader update..."), this);
     connect(_updatePCLOader, SIGNAL(triggered()), this, SLOT(updatePCLoader()));

     _language = new QAction(QIcon(languageIcon), tr("Select language"), this);
     connect(_language, SIGNAL(triggered()), this, SLOT(selectLanguage()));

     if(nLang == 1){
         QString languagesIcon = ":/MainWindow/ressources/english.png";
         _language->setIcon(QIcon(languagesIcon));
     }
     else if(nLang == 2){
         QString languagesIcon = ":/MainWindow/ressources/french.png";
         _language->setIcon(QIcon(languagesIcon));
     }
     else if(nLang == 3){
         QString languagesIcon = ":/MainWindow/ressources/german.png";
         _language->setIcon(QIcon(languagesIcon));
     }
     else if(nLang == 4){
         QString languagesIcon = ":/MainWindow/ressources/spanish.png";
         _language->setIcon(QIcon(languagesIcon));
     }

     logMenu = menuBar()->addMenu(tr("&Advanced User"));
     logMenu->addAction(_EditAdmin);

     menuBar()->addSeparator();

     editMenu = menuBar()->addMenu(tr("&Edit"));
     editMenu->addAction(_backup);
     editMenu->addAction(_reset);
     editMenu->addAction(_parameters);
     editMenu->addAction(_battery);
     editMenu->addSeparator();
     editMenu->addAction(_progLCD);
//     editMenu->addAction(_dump_log);
//     editMenu->addAction(_erase_log);

     menuBar()->addSeparator();

     helpMenu = menuBar()->addMenu(tr("&Help"));
     helpMenu->addAction(_language);
     helpMenu->addAction(_HelpBootloader);
     helpMenu->addAction(_HelpOnline);
     helpMenu->addAction(_HelpOffline);
//#ifdef Q_WS_WIN32
//     helpMenu->addAction(_updatePCLOader);
//#endif
     helpMenu->addAction(_HelpAbout);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions du menu
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::retranslateUi(){
    qDebug("retranslateUi");
    setWindowTitle(tr("Volfoni Loader - Updater for Volfoni products"));
    _EditAdmin->setText(tr("Log on"));
    _HelpOnline->setText(tr("Online support"));
    _HelpOffline->setText(tr("Help"));
    _HelpAbout->setText(tr("About Volfoni Loader"));
    _HelpBootloader->setText( tr("How to switch the product in update mode?"));
    _backup->setText(tr("Return to previous version"));
    _reset->setText(tr("Reinitialize device"));
    _parameters->setText(tr("Parameters"));
    _battery->setText(tr("Battery"));
//    _dump_log->setText(tr("Logging"));
//    _erase_log->setText(tr("Erase Log"));
    _updatePCLOader->setText( tr("Search for Volfoni Loader update..."));
    _language->setText( tr("Select language"));
    editMenu->setTitle(tr("&Edit"));
    logMenu->setTitle(tr("&Advanced User"));
    helpMenu->setTitle(tr("&Help"));
    button->setText(tr("Quit"));
    button_maj->setText(tr("Update now !"));
    button_checkupdate->setText(tr("Check update !"));
    if(statutLabelDesc==1)label_desc->setText(tr("Please connect your \nVolfoni device \nin update mode\nto a free USB port."));
    else if(statutLabelDesc==2)label_desc->setText(tr("A device is detected, \nplease wait..."));
    else if(statutLabelDesc==4)label_desc->setText(tr("Please connect only one \nVolfoni product \non USB ports."));
    else if(statutLabelDesc==5)label_desc->setText("ActivME\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
    else if(statutLabelDesc==6)label_desc->setText("ActivHub\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
    else if(statutLabelDesc==7)label_desc->setText("Smart Crystal Cinema\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
    else if(statutLabelDesc==8)label_desc->setText("Smart Crystal Pro\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
    else if(statutLabelDesc==9)label_desc->setText(tr("Device Attached."));
    else if(statutLabelDesc==10)label_desc->setText(tr("Unable to communicate with device"));
    else if(statutLabelDesc==11)label_desc->setText(tr("Unable to communicate with device"));
    else if(statutLabelDesc==12)label_desc->setText(tr("Error reading device."));
    else if(statutLabelDesc==13)label_desc->setText(tr("Unable to read type of device, \nmaybe your product is \ntoo old for Volfoni Loader."));
    else if(statutLabelDesc==14)label_desc->setText(tr("Please wait.\n\nUpdate in progress..."));
    else if(statutLabelDesc==15)label_desc->setText("EDGE 3D Glasses\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
    else if(statutLabelDesc==16)label_desc->setText("Smart Crystal DIAMOND\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);

    if(statutmsg==1)_msg = adaptTextQStatusBar(tr("Volfoni Loader, updater for Volfoni products"));
    else if(statutmsg==2)_msg = adaptTextQStatusBar(tr("Activ Me detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
    else if(statutmsg==3)_msg = adaptTextQStatusBar(tr("Activ Hub detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
    else if(statutmsg==4)_msg = adaptTextQStatusBar(tr("Smart Crystal Cinema detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
    else if(statutmsg==5)_msg = adaptTextQStatusBar(tr("Smart Crystal Pro detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
    else if(statutmsg==6)_msg = adaptTextQStatusBar(tr("Impossible to update this product using Volfoni Loader"));
    else if(statutmsg==7)_msg = adaptTextQStatusBar(tr("EDGE 3D Glasses detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
    else if(statutmsg==8)_msg = adaptTextQStatusBar(tr("Smart Crystal DIAMOND detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));

    if(statutLabelmaj==1)label_maj->setText("");
    else if(statutLabelmaj==2)label_maj->setText(tr("An update is \navailable:\n") + strLastVersion + tr("\n\nChanges from current version:\n")+changeWithPrevious(FirmwareRev,strLastVersion));
    else if(statutLabelmaj==3)label_maj->setText(tr("This device is \nup to date"));

}

void MainWindow::retranslateApp(QString langSelected, QString oldLanguage)
{// tous les textes actuels ne sont pas mis a jour automatiquement, seul les nouveaux
    // il faut faire un event pour detecter le changement de langue et changer tous les widget actuellement affichés
    qDebug("retranslateApp");
    if(nLang == 1){qApp->removeTranslator(translatorEN);}
    else if(nLang == 2){qApp->removeTranslator(translatorFR);}
    else if(nLang == 3){qApp->removeTranslator(translatorGR);}
    else if(nLang == 4){qApp->removeTranslator(translatorSP);}

    if(langSelected=="english"){
        qApp->installTranslator(translatorEN);
        nLang = 1;
        QString languagesIcon = ":/MainWindow/ressources/english.png";
        _language->setIcon(QIcon(languagesIcon));
    }
    else if(langSelected=="french"){
        qApp->installTranslator(translatorFR);
        nLang = 2;
        QString languagesIcon = ":/MainWindow/ressources/french.png";
        _language->setIcon(QIcon(languagesIcon));
    }
    else if(langSelected=="german"){
        qApp->installTranslator(translatorGR);
        nLang = 3;
        QString languagesIcon = ":/MainWindow/ressources/german.png";
        _language->setIcon(QIcon(languagesIcon));
    }
    else if(langSelected=="spanish"){
        qApp->installTranslator(translatorSP);
        nLang = 4;
        QString languagesIcon = ":/MainWindow/ressources/spanish.png";
        _language->setIcon(QIcon(languagesIcon));
    }
    // update des widget avec event et retranslateUI

}

void MainWindow::selectLanguage(){
    qDebug("selectLanguage");
    SelectLanguage selectLanguage(this,this,_fileLang);
    selectLanguage.exec();
}

void MainWindow::updatePCLoader(){
    qDebug("updatePCLoader");
    QProcess *process1 = new QProcess;
    process1->start("../updater.exe /checknow");
}

void MainWindow::returnLastVersion(){
    qDebug("returnLastVersion");

    QString temp = QString(tr("Are you sure to return to %1 version?")).arg(getVersionLastUpdate(SerialNumber).replace(", previous update version: ",""));
    if(getVersionLastUpdate(SerialNumber).contains("_V")){
        QMessageBox::StandardButton button =QMessageBox::question(this,
                      tr("Install version"),
                      temp,
                      QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes){
            product->checkUpdate(_fileLang);   // pour mettre a jour les tableaux
            QApplication::restoreOverrideCursor();
        }
    }else QMessageBox::about(this,
                             tr("Problem"),
                             tr("Impossible to read previous version name"));
}

void MainWindow::openHelp(){
    qDebug("openHelp");
#ifdef Q_WS_WIN32
    if(!QFile::exists("../Docs/help.pdf"))return;
    QString path = QString("file:///%1/../Docs/help.pdf").arg(QDir::currentPath());
    bool ok = QDesktopServices::openUrl(QUrl(path,QUrl::TolerantMode));
    if(ok == false)QMessageBox::about(this,tr("Error opening file"), tr("Sorry, an error occurred, impossible to open help file."));
#else
    QStringList list = QString(_fileLang).split("/");
    QString fileName;
    for(int i = 0; i<list.size()-1;i++)fileName.append(list.at(i)+"/");
    fileName.append("help.pdf");

    bool ok = QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    if(ok == false)QMessageBox::about(this,tr("Error opening file"), tr("Sorry, an error occurred, impossible to open help file."));
#endif
}

void MainWindow::helpBootloader(){
    qDebug("helpBootloader");
    SelectDevice selectProduct(this);
    selectProduct.exec();

}

void MainWindow::aboutPCLoader(){
    qDebug("aboutPCLoader");
    QMessageBox::about(this,
                 tr("About Volfoni Loader"),
                 tr("<p align='center'>Volfoni Loader Version %1<br>"
                    "Copyright (c) 2012 Volfoni.</p>").arg(versionSoftWare));
}

void MainWindow::aboutVolfoni(){
    qDebug("aboutVolfoni");
    QDesktopServices::openUrl(QUrl("http://www.volfoni.com/mises-a-jour-du-logiciel.html",QUrl::TolerantMode));

}

void MainWindow::helpUser(){
    qDebug("helpUser");
    QMessageBox::about(this,
                 tr("Have an advanced user account"),
                 tr("<p align='center'>Please contact Volfoni support to have administrator privileges.<br></p"));
}

void MainWindow::log_onUser(){
    qDebug("log_onUser");
    Login log_in(this,this);
    log_in.exec();
}

void MainWindow::resetDevice(){
    qDebug("resetDevice");
    QMessageBox::StandardButton button =QMessageBox::question(this, tr("Install version"), tr("Are you sure to reinitialize your device?\n"
                                                              "It will install an early update version"),
                  QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::Yes){
 //       reinitializeProduct(trigrammeProduct);
    }
}

void MainWindow::setParameters(){
    qDebug("setParameters");
    if(SerialNumber.startsWith("AME")) {
        Parameters_AME *parameters_ame = new Parameters_AME(this, FirmwareRev);
        product->setProductUpdating(true);
        parameters_ame->show();
        connect(this, SIGNAL(display_data(unsigned char *)), parameters_ame, SLOT(displayAll(unsigned char *)));
        connect(parameters_ame, SIGNAL(apply_clicked(unsigned char*)), this, SLOT(applyChanges(unsigned char*)));
        connect(parameters_ame, SIGNAL(finished(int)), this, SLOT(acceptParameters()));
        emit display_data(&product->getProductZoneInfo()[17]);
     }
    else if( SerialNumber.startsWith("SCP") || SerialNumber.startsWith("SCC") || SerialNumber.startsWith("SCD") ) {
        unsigned char readed[64];
        Parameters_SCP *parameters_scp = new Parameters_SCP(this, FirmwareRev);
        product->setProductUpdating(true);
        parameters_scp->show();
        connect(this, SIGNAL(display_data(unsigned char *)), parameters_scp, SLOT(displayAll(unsigned char *)));
        connect(parameters_scp, SIGNAL(apply_clicked(unsigned char*)), this, SLOT(applyChanges(unsigned char*)));
        connect(parameters_scp, SIGNAL(finished(int)), this, SLOT(acceptParameters()));
        product->readParam(readed);
        if(readed[0]==255){// cas illisible, 0xFF
            readed[0] = 15;
            for(int i=1; i<=23; i++) readed[i] = 0 ;
        }
        emit display_data(readed);
     }
    else {
        Parameters *parameters = new Parameters(this, FirmwareRev);
        product->setProductUpdating(true);
        parameters->show();
        connect(this, SIGNAL(display_data(unsigned char *)), parameters, SLOT(displayAll(unsigned char *)));
        connect(parameters, SIGNAL(apply_clicked(int, int, unsigned char*)), this, SLOT(applyChanges(int, int, unsigned char*)));
        connect(parameters, SIGNAL(finished(int)), this, SLOT(acceptParameters()));
        emit display_data(&product->getProductZoneInfo()[17]);
    }

}

void MainWindow::setProgLCD(){
    qDebug("setProgLCD");

    ProgLCD *progLCD = new ProgLCD(this);
    product->setProductUpdating(true);
    progLCD->show();
    connect(this, SIGNAL(display_data(int)), progLCD, SLOT(finished(int)));
    connect(progLCD, SIGNAL(apply_clicked(int, unsigned char*)), this, SLOT(applyChanges(int, unsigned char*)));
    connect(progLCD, SIGNAL(apply_clicked(int, int, unsigned char*)), this, SLOT(applyChanges(int, int, unsigned char*)));
    //connect(progLCD, SIGNAL(finished(int)), this, SLOT(acceptParameters()));
}

void MainWindow::getBattery(){
    qDebug("getBattery");
    battery = new BatteryView(this);
    battery->show();
    connect(this, SIGNAL(displayBattData(int,QString,int)), battery, SLOT(displayAll(int,QString,int)));
    connect(battery, SIGNAL(refresh_clicked(bool)), this, SLOT(checkAllGlasses(bool)));

    for(int i=0; i<batterieIndex; i++){
        emit displayBattData(i, glassesID[i], batterieLevel[i]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions d'affichage
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::resetState(){// retour a un affichage normal
    qDebug("resetState");
    QApplication::restoreOverrideCursor();
    //Updating = false;
    button_checkupdate->setEnabled(true);
    label->setMovie(animatedgif);
    animatedgif->start();
    label_maj->setText("");
    statutLabelmaj = 1;
    progressbar->setValue(0);
    label->clear();

    // update de la fenetre, pour un retour en arriere des labels
    if(trigrammeProduct =="AME") {
        label_desc->setText("ActivME\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
        statutLabelDesc = 5;
        if(label_help->isVisible())label_help->hide();
    }
    else if(trigrammeProduct =="AHU"){
        label_desc->setText("ActivHub\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
        statutLabelDesc = 6;
        if(label_help->isVisible())label_help->hide();
    }
    else if(trigrammeProduct =="SCC"){
        label_desc->setText("Smart Crystal Cinema\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
        statutLabelDesc = 7;
        if(label_help->isVisible())label_help->hide();
    }
    else if(trigrammeProduct =="SCP"){
        label_desc->setText("Smart Crystal Pro\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
        statutLabelDesc = 8;
        if(label_help->isVisible())label_help->hide();
    }
    else if(trigrammeProduct =="EDG"){
        label_desc->setText("EDGE 3D Glasses\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
        statutLabelDesc = 15;
        if(label_help->isVisible())label_help->hide();
    }
    else if(trigrammeProduct =="SCD"){
        label_desc->setText("Smart Crystal DIAMOND\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
        statutLabelDesc = 16;
        if(label_help->isVisible())label_help->hide();
    }else{
        label_picture->setPixmap(QPixmap(":/MainWindow/ressources/default.png"));
        label_desc->setText(tr("Please connect your \nVolfoni device \nin update mode\nto a free USB port."));
        statutLabelDesc = 1;
        if(!label_help->isVisible())label_help->show();
        _msg = adaptTextQStatusBar(tr("Volfoni Loader, updater for Volfoni products"));
        statutmsg = 1;
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// fonctions appelées de l'exterieur
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::acceptParameters(){
    qDebug("acceptParameters");
    product->setProductUpdating(false);
}

void MainWindow::applyChanges(int start, int size, unsigned char* pData){
    unsigned char readed[64];

    qDebug("applyChanges");
    if( (trigrammeProduct == "EDG") || (trigrammeProduct == "AME") ){
        product->programParamFlash(start, size, pData);
        product->readFirmwareData();        //Relecture pour maj des variables
        emit display_data(&product->getProductZoneInfo()[17]);
    }
    else if(trigrammeProduct =="AHU"){
        // si l'AHU est connecté, on utilise la surcharge ce applyChanges ci-dessous
    }

    else if( (trigrammeProduct == "SCP") || (trigrammeProduct == "SCC") || (trigrammeProduct == "SCD") ){
        product->programParam(pData);
        product->readParam(readed);
        emit display_data(readed);
    }
}


void MainWindow::applyChanges(int numero, unsigned char* pData){

    unsigned char pBuffer[24];
    int i;

    qDebug("applyChanges AHU");

    timerDebugMsg->start(1000);

    //Au premier message envoyé, on récupère l'intégralité des données à transmettre
    if (numero==1) for(i=0; i<64; i++) informations[i] = pData[i];

    //Mise en forme du message
     pBuffer[0] = 0x15;
     for(i=0; i<8; i++) pBuffer[1+i] = informations[i]; //ID
     pBuffer[9] = numero;
     if(numero==1){
         numeroTrameRF = 1;
         for(i=10; i<20; i++) pBuffer[i] = informations[i-2];
     }
     else if(numero==2) {
         for(i=10; i<20; i++) pBuffer[i] = informations[i+8];
         emit display_data(40);
     }
     else if(numero==3) {
        for(i=10; i<20; i++) pBuffer[i] = informations[i+18];
        emit display_data(60);
     }
     else if(numero==4) {
        for(i=10; i<20; i++) pBuffer[i] = informations[i+28];
        emit display_data(80);
     }
     else if(numero==5) {
        emit display_data(100);
        timerDebugMsg->stop();
        return;
     }

     qDebug() << "pBuffer: " << pBuffer[0] << pBuffer[1] << pBuffer[2] << pBuffer[3] << pBuffer[4] << pBuffer[5] << pBuffer[6] << pBuffer[7] << pBuffer[8] << pBuffer[9];
     qDebug() << "pBuffer: " << pBuffer[10] << pBuffer[11] << pBuffer[12] << pBuffer[13] << pBuffer[14] << pBuffer[15] << pBuffer[16] << pBuffer[17] << pBuffer[18] << pBuffer[19];
     //Envoi
     product->setDebugMsg(24, &pBuffer[0]);
     product->getDebugMsg();
}


void MainWindow::applyChanges(unsigned char* pData){
    unsigned char readed[64];

    qDebug("applyChanges SC");

    if( (trigrammeProduct == "SCP") || (trigrammeProduct == "SCC") || (trigrammeProduct == "SCD") ){
        product->programParam(pData);
        product->readParam(readed);
        emit display_data(readed);
    }
}


void MainWindow::checkAllGlasses(bool getBattLevel){
    int i;
    unsigned char pBuffer[20];

    qDebug("checkAllGlasses");

    //Pour éviter un affichage redondant
    disconnect(this, SIGNAL(displayBattData(int,QString,int)), battery, SLOT(displayAll(int,QString,int)));

    //Récupération des ID mémorisés dans l'AHU
    pBuffer[0] = 0x01;
    //pBuffer[1] = 0x02;
    for(i=2; i<20; i++) pBuffer[i] = 0;
    for(i=0; i<10; i++){
    pBuffer[1] = i;
        product->setDebugMsg(24, &pBuffer[0]);
        product->getDebugMsg();
    }

    //Mise en mémoire des ID glasses courantes
    for(i=0;i<10;i++) memoGlassesID[i] = glassesID[i];
    memoBatterieIndex = batterieIndex;

    //RAZ de l'index batterie
    batterieIndex = 0;

    // On reactive l'affichage
    connect(this, SIGNAL(displayBattData(int,QString,int)), battery, SLOT(displayAll(int,QString,int)));

    if(getBattLevel){
        //Envoi d'une requete niveau batterie a toutes les lunettes connues
        for(i=0; i<memoBatterieIndex; i++) {
            QTimer::singleShot(10000*i+500, this, SLOT(checkOneBattery()));
        }
    }
    memoBatterieIndex = 0;
}

void MainWindow::checkOneBattery(){
    int i;
    unsigned char pBuffer[24];
    qDebug("checkOneBattery");

    //Preparation du msg
    pBuffer[0] = 0x11;
    const char* tempID = memoGlassesID[memoBatterieIndex++].toStdString().c_str();
    pBuffer[1] = tempID[0];   //ID sur 8 octets
    pBuffer[2] = tempID[1];
    pBuffer[3] = tempID[2];
    pBuffer[4] = tempID[3];
    pBuffer[5] = tempID[4];
    pBuffer[6] = tempID[5];
    pBuffer[7] = tempID[6];
    pBuffer[8] = tempID[7];
    for(i=9; i<24; i++) pBuffer[i] = 0;
    //Envoi du msg
    product->setDebugMsg(24, &pBuffer[0]);
    product->getDebugMsg();
 }

void MainWindow::askDebugMsg(){
    qDebug("askDebugMsg");
    product->setDebugMsg(0, NULL);
    product->getDebugMsg();
}

void MainWindow::logUser(){
    qDebug("logAdmin");
    if(product->pollUSB()<=1 || product->pollUSBApp()<=1){
        if(product->isConnected() || product->isConnectedApp())// verifie qu'un produit est connecté avant d'afficher les mises à jour
        {
            ListUpdate listUP(this,this,FirmwareRev, trigrammeProduct, customerNumber,"admin",_fileLang);
            listUP.exec();
        }
        else QMessageBox::warning(this,
                                    tr("No device connected"),
                                    tr("Please connect a Volfoni product first"));

    }else QMessageBox::warning(this,
                            tr("Too many devices connected"),
                            tr("Please connect only one Volfoni product on USB ports"));
}

void MainWindow::requestUpdateFirmware(QString fileName, QString updateName, bool localFile){ // fonction appelée par un utilisateur avancé/admin
    qDebug("requestUpdateFirmware");

    m_updateFilename = fileName;
    selectedRev = updateName;
    strLastVersion = updateName;
    _localFile = localFile;

    if(product->pollUSB()<=1 || product->pollUSBApp()<=1){
        if(product->isConnected() || product->isConnectedApp())// verifie qu'un produit est connecté avant d'afficher les mises à jour
        {
            QApplication::setOverrideCursor(Qt::BusyCursor);
            label->clear();
            button_checkupdate->setEnabled(false);
            UpdateFirmware();
        }
        else QMessageBox::warning(this,
                                  tr("No device connected"),
                                  tr("Please connect a Volfoni product first"));
    }else QMessageBox::warning(this,
                               tr("Too many devices connected"),
                               tr("Please connect only one Volfoni product on USB ports"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions associées à la barre de statut
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QString MainWindow::adaptTextQStatusBar(QString text){
    qDebug("adaptTextQStatusBar");
    // ajout d'espaces pour combler la largeur de la fenetre
    bool stop = false;
    QLabel *labelTest = new QLabel(text);// utilisé pour connaitre la font du text et sa largeur
    int nSecurity = 0; // utilisé pour fixer un nombre maximum d'espaces et eviter les bugs
    QFontMetrics metrics = labelTest->fontMetrics();
    if(metrics.boundingRect(text).width()>=widthWindow)stop = true;
    while(stop == false){
        text.append(" ");
        nSecurity++;
        if(metrics.boundingRect(text).width()<widthWindow)stop = false;
        else if(nSecurity>200)stop = true;
        else if(metrics.boundingRect(text).width()>=widthWindow && nSecurity<=200)stop = true;
    }
    return text;
}


void MainWindow::updateStatusBar()   // update message in QStatusBar
{
    qDebug("updateStatusBar");

  if(_msg.isEmpty())
  {
     status->clearMessage();
  }
  else
  {
    //int pos = _msg.length() - 1;
    //_msg = _msg.right(pos) + _msg[0];
    status->showMessage(_msg);
  }

     unsigned char batteryLevel[64];

    //Lecture du niveau batterie
    if(product->GetBattLevel(batteryLevel) == Comm::Success)
    {
        batterieBuffer[battIndex] = batteryLevel[0];
        if(++battIndex>=10)
            battIndex=0;
        if(++batterieNbValide>=10)
            batterieNbValide=10;

        //calcul de la valeur moyenne
        int temp = 0;
        for(int i=0; i<batterieNbValide; i++) temp += batterieBuffer[i];
        temp = temp / batterieNbValide;

         //Affichage
        temp = 100 * (temp - 206) / (216 - 206) ;
        if(temp>100) temp = 100;
        else if(temp>=80)   battFullCounter++;
        if(battFullCounter>=1800) temp = 100;
        batterieBar->setValue( temp );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions de detection d'un produit sur un port USB
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::Connection(void)
{
    qDebug("Connection");
    statutLabelDesc = product->connection();

    if(statutLabelDesc == 0)
    {
        //Un device est déja connecté, on limite le pooling
        timer_usb->setInterval(5000);
    }
    else if(statutLabelDesc == 9)
    {
        label_desc->setText(tr("Device Attached."));
        if(label_help->isVisible())label_help->hide();
        CheckConnected();
    }
    else if(statutLabelDesc == 1)
    {
        button_maj->hide();
        QApplication::restoreOverrideCursor();
        label_desc->setText(tr("Please connect your \nVolfoni device \nin update mode\nto a free USB port."));
        if(!label_help->isVisible())label_help->show();
        label_picture->setPixmap(QPixmap(":/MainWindow/ressources/default.png"));
        label_pic->setPixmap(QPixmap(":/MainWindow/ressources/emptyName.png"));
        _msg = adaptTextQStatusBar(tr("Volfoni Loader, updater for Volfoni products"));
        statutmsg = 1;
        label_pic->setMovie(animatedUSB);
        label->setMovie(animatedgif);
        animatedgif->start();
        label_maj->setText("");
        statutLabelmaj = 1;
        progressbar->setValue(0);
        button_checkupdate->hide();
        _HelpBootloader->setVisible(true);
        _backup->setEnabled(false);
        _reset->setEnabled(false);

        button_checkupdate->setEnabled(false);
        timerDebugMsg->stop();
        timer_usb->setInterval(1000);
    }

    if(enableCheckUpdate)button_checkupdate->show();
    else button_checkupdate->hide();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions de lecture d'un produit
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::CheckConnected(){

    qDebug("CheckConnected");
    nConnected = product->pollUSB() + product->pollUSBApp();
    if(nConnected<=1){
        if(product->isConnected())
        {
            qWarning("Attempting to open device in bootloader mode...");
            product->open();
            label_desc->setText(tr("A device is detected, \nplease wait..."));
            label_help->hide();
            statutLabelDesc = 2;
            product->getQuery();
            DisplayFirmwareData();
            for(int i=0; i<10; i++) batterieBuffer[i] = 0;
            battIndex = 0;
            batterieNbValide = 0;
            battFullCounter = 0;
        }
        else if(product->isConnectedApp())
        {
            qWarning("Attempting to open device in applicative mode...");
            product->openApp();
            label_desc->setText(tr("A device is detected, \nplease wait..."));
            label_help->hide();
            statutLabelDesc = 2;
            product->getQuery();
            DisplayFirmwareData();
        }
        else
        {
            label_desc->setText(tr("Please connect your \nVolfoni device \nin update mode\nto a free USB port."));
            statutLabelDesc = 1;
            label_picture->setPixmap(QPixmap(":/MainWindow/ressources/default.png"));
            _msg = adaptTextQStatusBar(tr("Volfoni Loader, updater for Volfoni products"));
            statutmsg = 1;
            _backup->setEnabled(false);
            _reset->setEnabled(false);
            _parameters->setEnabled(false);
            _battery->setEnabled(false);
            _progLCD->setEnabled(false);
//            _dump_log->setEnabled(false);
//            _erase_log->setEnabled(false);

        }
    }
    else{
        label_desc->setText(tr("Please connect only one \nVolfoni product \non USB ports."));
        statutLabelDesc = 4;
        if(!label_help->isVisible())label_help->show();
        label_picture->setPixmap(QPixmap(":/MainWindow/ressources/default.png"));
        _msg = adaptTextQStatusBar(tr("Volfoni Loader, updater for Volfoni products"));
        statutmsg = 1;
        _backup->setEnabled(false);
        _reset->setEnabled(false);
        _parameters->setEnabled(false);
        _battery->setEnabled(false);
        _progLCD->setEnabled(false);
//        _dump_log->setEnabled(false);
//        _erase_log->setEnabled(false);

    }
}

void MainWindow::DisplayFirmwareData(){//Get the device firmware revision
    qDebug("DisplayFirmwareData");
    label->clear();

    QString tempo = product->getProductCustomerNumber();
    FirmwareRev = product->getProductFirmwareRev();
    SerialNumber = product->getProductSerialNumber();

    bool ok = true;

    if(tempo.contains(".")==true){
        //ex de nom = IR_RF_V01.18.0088
        QStringList list = QString(tempo).split(".");//découpe le nom de la version selon les "."

        if(list.size()>0)customerNumber = list.at(0);
        ok = true;

    }else {
        if(FirmwareRev.contains(".") && FirmwareRev.contains("_V")==true){// lecture possible
            QStringList list = QString(FirmwareRev).split(".");//découpe le nom de la version selon les "."

            if(list.size()>0)customerNumber = list.at(0);
            ok = true;
        }else  {
            ok = false;
            customerNumber = "";
        }
    }
    if(FirmwareRev.contains(".")==true && FirmwareRev.contains("_V")==true){// lecture possible
        QStringList list = QString(FirmwareRev).split(".");//découpe le nom de la version selon les "."

        if(list.size()>0)customerNumber = list.at(0);
        ok = true;
    }

    if(ok == true){// on a un numéro de client écrit ou lisible
        //----- AFFICHAGE RESULTATS
        if( SerialNumber.startsWith("AME") || FirmwareRev.startsWith("IR_") ) {
            trigrammeProduct = "AME";
            label_desc->setText("ActivME\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
            if(label_help->isVisible())label_help->hide();
            statutLabelDesc = 5;
            label_pic->setPixmap(QPixmap(":/MainWindow/ressources/pic_ame.png"));
            label_picture->setPixmap(QPixmap(":/MainWindow/ressources/activme.png"));
            _HelpBootloader->setVisible(false);
            _backup->setEnabled(true);
            _reset->setEnabled(true);
            _parameters->setEnabled(true);

            _msg = adaptTextQStatusBar(tr("Activ Me detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
            statutmsg = 2;
            // si on a une ancienne version lisible, on affiche le menu de retour en arriere
            if(getVersionLastUpdate(SerialNumber).replace(", previous update version: ","")!="" &&
                    getVersionLastUpdate(SerialNumber).replace(", previous update version: ","").contains("_V")==true)enableCheckUpdate = true;
            else {
                _backup->setEnabled(false);
            }
            enableCheckUpdate = true;
            button_checkupdate->setEnabled(true);
        }
        else if( SerialNumber.startsWith("EDG") || FirmwareRev.startsWith("EDG") ) {
            trigrammeProduct = "EDG";
            label_desc->setText("EDGE 3D Glasses\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
            if(label_help->isVisible())label_help->hide();
            statutLabelDesc = 15;
            label_pic->setPixmap(QPixmap(":/MainWindow/ressources/pic_edge.png"));
            label_picture->setPixmap(QPixmap(":/MainWindow/ressources/EDGE.png"));
            _HelpBootloader->setVisible(false);
            _backup->setEnabled(true);
            _reset->setEnabled(true);
            _parameters->setEnabled(true);

            _msg = adaptTextQStatusBar(tr("EDGE 3D Glasses detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
            statutmsg = 2;
            // si on a une ancienne version lisible, on affiche le menu de retour en arriere
            if(getVersionLastUpdate(SerialNumber).replace(", previous update version: ","")!="" &&
                    getVersionLastUpdate(SerialNumber).replace(", previous update version: ","").contains("_V")==true)enableCheckUpdate = true;
            else {
                _backup->setEnabled(false);
            }
            enableCheckUpdate = true;
            button_checkupdate->setEnabled(true);
            if(FirmwareRev.startsWith("EDGHS")) {
                _progLCD->setEnabled(true);
            }
        }
        else if( SerialNumber.startsWith("AHU") || SerialNumber.startsWith("APO") || FirmwareRev.startsWith("__AHU") ){
            trigrammeProduct = "AHU";
            label_desc->setText("ActivHub\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
            statutLabelDesc = 6;
            if(label_help->isVisible())label_help->hide();
            label_pic->setPixmap(QPixmap(":/MainWindow/ressources/pic_ahu.png"));
            label_picture->setPixmap(QPixmap(":/MainWindow/ressources/activhub.png"));
            _msg = adaptTextQStatusBar(tr("Activ Hub detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
            statutmsg = 3;
            _HelpBootloader->setVisible(false);
            _backup->setEnabled(true);
            _reset->setEnabled(true);
            if(FirmwareRev.startsWith("__AHU_V06")) {
                _parameters->setEnabled(false);
                _battery->setEnabled(true);
                _progLCD->setEnabled(true);
                //timerDebugMsg->start(1000);
            }

            if(getVersionLastUpdate(SerialNumber).replace(", previous update version: ","")!="" &&
                    getVersionLastUpdate(SerialNumber).replace(", previous update version: ","").contains("_V")==true)enableCheckUpdate = true;
            else {
                _backup->setEnabled(false);
            }
            enableCheckUpdate = true;
            button_checkupdate->setEnabled(true);
        }
        else if( SerialNumber.startsWith("SCC") || FirmwareRev.startsWith("GESPA") ){
            trigrammeProduct = "SCC";
            label_desc->setText("Smart Crystal Cinema\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
            statutLabelDesc = 7;
            if(label_help->isVisible())label_help->hide();
            label_pic->setPixmap(QPixmap(":/MainWindow/ressources/pic_scc.png"));
            label_picture->setPixmap(QPixmap(":/MainWindow/ressources/crystalcinema.png"));
            _msg = adaptTextQStatusBar(tr("Smart Crystal Cinema detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
            statutmsg = 4;
            _HelpBootloader->setVisible(false);
            _backup->setEnabled(true);
            _reset->setEnabled(true);
            _parameters->setEnabled(true);

            if(getVersionLastUpdate(SerialNumber).replace(", previous update version: ","")!="" &&
                    getVersionLastUpdate(SerialNumber).replace(", previous update version: ","").contains("_V")==true)enableCheckUpdate = true;
            else {
                _backup->setEnabled(false);
            }
            enableCheckUpdate = true;
            button_checkupdate->setEnabled(true);
        }
        else if( SerialNumber.startsWith("SCP") || FirmwareRev.startsWith("__SCP") ){
            trigrammeProduct = "SCP";
            label_desc->setText("Smart Crystal Pro\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
            statutLabelDesc = 8;
            if(label_help->isVisible())label_help->hide();
            label_pic->setPixmap(QPixmap(":/MainWindow/ressources/pic_scp.png"));
            label_picture->setPixmap(QPixmap(":/MainWindow/ressources/crystalpro.png"));
            _HelpBootloader->setVisible(false);

            _msg = adaptTextQStatusBar(tr("Smart Crystal Pro detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
            statutmsg = 5;
            if(getVersionLastUpdate(SerialNumber).replace(", previous update version: ","")!="" &&
                    getVersionLastUpdate(SerialNumber).replace(", previous update version: ","").contains("_V")==true){
                _backup->setEnabled(true);
                _reset->setEnabled(true);
            }
            else {
                _backup->setEnabled(false);
            }
            enableCheckUpdate = true;
            button_checkupdate->setEnabled(true);
            _parameters->setEnabled(true);
        }
        else if( SerialNumber.startsWith("SCD") || FirmwareRev.startsWith("__SCD") ){
            trigrammeProduct = "SCD";
            label_desc->setText("Smart Crystal DIAMOND\n\nS/N: " + SerialNumber + "\n\nSW: " + FirmwareRev);
            statutLabelDesc = 16;
            if(label_help->isVisible())label_help->hide();
            label_pic->setPixmap(QPixmap(":/MainWindow/ressources/pic_scd.png"));
            label_picture->setPixmap(QPixmap(":/MainWindow/ressources/diamond.png"));
            _HelpBootloader->setVisible(false);

            _msg = adaptTextQStatusBar(tr("Smart Crystal DIAMOND detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
            statutmsg = 8;
            if(getVersionLastUpdate(SerialNumber).replace(", previous update version: ","")!="" &&
                    getVersionLastUpdate(SerialNumber).replace(", previous update version: ","").contains("_V")==true){
                _backup->setEnabled(true);
                _reset->setEnabled(true);
             }
            else {
                _backup->setEnabled(false);
            }
            enableCheckUpdate = true;
            button_checkupdate->setEnabled(true);
            _parameters->setEnabled(true);
        }
        else {
            trigrammeProduct = "";
            label_desc->setText(tr("Unable to read type of device, \nmaybe your product is \ntoo old for Volfoni Loader."));
            statutLabelDesc = 13;
            if(!label_help->isVisible())label_help->show();
            label_pic->setPixmap(QPixmap(":/MainWindow/ressources/white.png"));
            label_picture->setPixmap(QPixmap(":/MainWindow/ressources/default.png"));
            _msg = adaptTextQStatusBar(tr("Impossible to update this product using Volfoni Loader"));
            statutmsg = 6;
            _backup->setEnabled(false);
            _reset->setEnabled(false);
            _parameters->setEnabled(false);
            _battery->setEnabled(false);
            _progLCD->setEnabled(false);
            _dump_log->setEnabled(false);
            _erase_log->setEnabled(false);

            enableCheckUpdate = false;

        }
    }
    else {// cas pas de numero de client écrit
        QMessageBox::warning(this,tr("Error"),"\n\n"+tr("Unable to read update information of the device.")+" " +tr("Your device is not compatible with Volfoni Loader."));
        trigrammeProduct = "";
        label_desc->setText(tr("Unable to read type of device, \nmaybe your product is \ntoo old for Volfoni Loader."));
        statutLabelDesc = 13;
        if(!label_help->isVisible())label_help->show();
        label_pic->setPixmap(QPixmap(":/MainWindow/ressources/white.png"));
        label_picture->setPixmap(QPixmap(":/MainWindow/ressources/default.png"));
        _msg = adaptTextQStatusBar(tr("Impossible to update this product using Volfoni Loader"));
        statutmsg = 6;
        _backup->setEnabled(false);
        _reset->setEnabled(false);
        _parameters->setEnabled(false);
        _battery->setEnabled(false);
        _progLCD->setEnabled(false);
        _dump_log->setEnabled(false);
        _erase_log->setEnabled(false);

        enableCheckUpdate = false;
    }

}

void MainWindow::checkMsgRF(unsigned char* data){
    int i;
    QString strID;
    qDebug("checkMsgRF");

    if( (data[0]==0x0A) || (data[0]==0x11) ) {
        //Verification si place dispo
        if(batterieIndex>9) {
            qDebug("Pas de place dispo pour enregistrer une lunette");
            return;
        }

        //Recuperation de l'ID, memorisation et increment index
        strID = QString((const char *)&data[1]).left(8);
        for(i=0; i<batterieIndex; i++) {
            if (glassesID[i] == strID) break;
        }
        glassesID[i] = strID;
        batterieLevel[i] = ((int) data[9]<<8) + data[10];
        if(i == batterieIndex) batterieIndex++;

        qWarning("display data 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10]);
        emit displayBattData(i, glassesID[i], batterieLevel[i]);
    }
    else if(data[0]==0x15){
            numeroTrameRF++;
            qDebug() << "Trame n°" << numeroTrameRF << ", type : " <<data[9];
            applyChanges(numeroTrameRF, NULL);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour récupérer des infos textes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QString MainWindow::getDateLastUpdate(QString serial_number){// renvoi une phrase pour savoir quelle est la date de la derniere mise à jour
    qDebug("getDateLastUpdate");
    QString sentence = "";

    QString fileName = QString("../Config/config.xml");
#ifdef Q_OS_MAC
    QStringList list = QString(_fileLang).split("/");
    fileName ="";
    for(int i = 0; i<list.size()-1;i++)fileName.append(list.at(i)+"/");
    fileName.append("config.xml");
#endif
    QFile file(fileName);
    // lecture du fichier local et sauvegarde temporaire de ses infos
    if(QFile::exists(fileName)==true){ // si il n'existe pas, on passe à la suite

        QDomDocument settingsF(fileName);
        //handle non-existent file case (print debug message and return)
        if (!file.open(QIODevice::ReadOnly)){
          std::cout << "Error while reading configuration file" << std::endl;
        }
        QString errorMsg;
        if(!settingsF.setContent(&file, &errorMsg)){ //handle error (print error message on pane for instance)
          std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
          file.close();
        }
        file.close();

        //parse the settings
        QDomElement rootF = settingsF.documentElement();
        QDomElement prod = rootF.firstChildElement("product");

        while(!prod.isNull()){
          QDomElement temp = prod.firstChildElement("SerialNumber");
          if(serial_number == temp.text()){
              temp = prod.firstChildElement("lastUpdateDate");
              if(!temp.isNull() && temp.text()!="")sentence = QString(tr(", previous update date: %1")).arg(temp.text());
          }
          prod = prod.nextSiblingElement("product");
        }
    }else sentence = ""; // cela veut dire qu'il n'y a aucune info existante sur l'ordinateur sur ce produit

    return sentence;
}

QString MainWindow::getVersionLastUpdate(QString serial_number){// renvoi une phrase pour savoir quelle est la version de la derniere mise à jour
    qDebug("getVersionLastUpdate");
    QString sentence = "";

    QString fileName = QString("../Config/config.xml");
#ifdef Q_OS_MAC
    QStringList list = QString(_fileLang).split("/");
    fileName ="";
    for(int i = 0; i<list.size()-1;i++)fileName.append(list.at(i)+"/");
    fileName.append("config.xml");
#endif
    QFile file(fileName);
    // lecture du fichier local et sauvegarde temporaire de ses infos
    if(QFile::exists(fileName)==true){ // si il n'existe pas, on passe à la suite

        QDomDocument settingsF(fileName);
        //handle non-existent file case (print debug message and return)
        if (!file.open(QIODevice::ReadOnly)){
          std::cout << "Error while reading configuration file" << std::endl;
        }
        QString errorMsg;
        if(!settingsF.setContent(&file, &errorMsg)){ //handle error (print error message on pane for instance)
          std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
          file.close();
        }
        file.close();

        //parse the settings
        QDomElement rootF = settingsF.documentElement();
        QDomElement prod = rootF.firstChildElement("product");

        while(!prod.isNull()){
          QDomElement temp = prod.firstChildElement("SerialNumber");
          if(serial_number == temp.text()){
              temp = prod.firstChildElement("previousFirmwareRev");
              if(!temp.isNull() && temp.text()!="")sentence = QString(tr(", previous update version: %1")).arg(temp.text());
          }
          prod = prod.nextSiblingElement("product");
        }
    }else sentence = ""; // cela veut dire qu'il n'y a aucune info existante sur l'ordinateur sur ce produit

    return sentence;
}

QString MainWindow::changeWithPrevious(QString current, QString lastOne){//renvoi les differences entres 2 versions
    qDebug("changeWithPrevious");

    QString changes = "";
    if(current.contains("_V")==true){
        QString macChange ="versioning.xml";
#ifdef Q_WS_WIN32
        file = new QFile(macChange);
#endif
#ifdef Q_OS_MAC
        QStringList listM = QString(_fileLang).split("/");
        macChange ="";
        for(int i = 0; i<listM.size()-1;i++)macChange.append(listM.at(i)+"/");
        macChange.append("versioning.xml");
        file = new QFile(macChange);
#endif
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
        }
        QString fileName = macChange;

        QDomDocument settings(fileName);

        QString errorMsg;
        if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
            std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
            file->close();
        }
        file->close();

        QStringList list = QString(current).split(".");//découpe le nom de la version actuelle selon les "."
        QString temp2 = "";
        if(list.size()>1)temp2 = list.at(list.size()-1); // temp2 = 17.02, dernier élément de la liste

        QString allowedUpdate = list.at(0); // allowedUpdate = 01
        QString tmp = QString("%1%2").arg(list.at(1)).arg(list.at(2));
        int nCurrent = tmp.toInt(); // int de la version courante

        //parse the settings tree and restore state
        QDomElement root = settings.documentElement();
        QDomElement a = root.firstChildElement(trigrammeProduct);
        QDomElement b = a.firstChildElement("new-version");

        while(!b.isNull()){// balaye les versions disponibles sur la base de données
            QDomElement c = b.firstChildElement("name");
            QString _temp = c.text();
            // exemple de nom de version : IR_IR_V01.17.02
            QStringList _list = QString(_temp).split(".");//découpe le nom de la version selon les "_V"
            QString _temp2 = "";
            if(_list.size()>1)_temp2 = _list.at(_list.size()-1); // temp2 = 17.02, dernier élément de la liste

            bool allowedUpdateValue = false;
            QString _allowedUpdate = _list.at(0); // allowedUpdate = 01

            if( (_allowedUpdate == allowedUpdate) || (allowedUpdate.startsWith("IR_")) )allowedUpdateValue = true;
            else allowedUpdateValue = false;
            if(allowedUpdateValue==true){
                tmp = QString("%1%2").arg(_list.at(1)).arg(_list.at(2));
                int nVersion = tmp.toInt();
                if(nVersion>nCurrent){// version superieure a l'actuelle -> enregistrer les modifs
                    c = b.firstChildElement("modification");
                    while(!c.isNull()){
                        changes.append("- ");
                        changes.append(c.text());
                        changes.append("\n");
                        c =c.nextSiblingElement("modification");
                    }
                }
            }
            b = b.nextSiblingElement("new-version");
        }

        file->close();

    }else changes.append(tr("Impossible to read \ncurrent version to compare them"));

    return changes;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour récupérer des fichiers en ligne
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::GetPreviousVersion(){// recupère les infos de la derniere version installée a partir du nom enregistré en local
    qDebug("GetPreviousVersion");

    label->clear();
    QApplication::setOverrideCursor(Qt::BusyCursor);
    QString macChange ="versioning.xml";
#ifdef Q_WS_WIN32
    file = new QFile(macChange);
#endif
#ifdef Q_OS_MAC
    QStringList listM = QString(_fileLang).split("/");
    macChange ="";
    for(int i = 0; i<listM.size()-1;i++)macChange.append(listM.at(i)+"/");
    macChange.append("versioning.xml");
    file = new QFile(macChange);
#endif
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Unable to open local file versioning.xml");
        waitConnection->stop();
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this,tr("Error"),tr("\nUnable to acces the information about updates.\n\nPlease try again later"));
        //MainWindow::close();
        QApplication::restoreOverrideCursor();
        return;
    }

    QString fileName = macChange;

    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
      std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
      file->close();
    }
    file->close();

    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    QDomElement a = root.firstChildElement(trigrammeProduct);
    QDomElement b = a.firstChildElement("new-version");

    tmpV = 0;
    while(!b.isNull()){// balaye les versions disponibles sur la base de données
      QDomElement c = b.firstChildElement("filename");
      if(tmpV<1000)tmpFilenameV[tmpV]= c.text(); // utilisé plus tard pour un retour en arriere des versions
      c = b.firstChildElement("name");
      if(tmpV<1000)tmpNameV[tmpV]= c.text(); // utilisé plus tard pour un retour en arriere des versions
      if(tmpV<1000)tmpV++;
      b = b.nextSiblingElement("new-version");
    }

    file->close();
    file->remove();

    int indexCorrect = -1;
    for(int i = 0; i<tmpV;i++){
        QString tmp = getVersionLastUpdate(SerialNumber).replace(", previous update version: ","");
        if(tmpNameV[i] == tmp)indexCorrect = i;
    }

    if(indexCorrect>-1)requestUpdateFirmware(tmpFilenameV[indexCorrect], tmpNameV[indexCorrect], false);
    else QMessageBox::about(this,tr("Problem"),tr("Impossible to find previous version"));
    QApplication::restoreOverrideCursor();

}

void MainWindow::GetLastVersion(){
    qDebug("GetLastVersion");

    label->clear();
    QApplication::setOverrideCursor(Qt::BusyCursor);
    QString macChange ="versioning.xml";
#ifdef Q_WS_WIN32
    file = new QFile(macChange);
#endif
#ifdef Q_OS_MAC
    QStringList listM = QString(_fileLang).split("/");
    macChange ="";
    for(int i = 0; i<listM.size()-1;i++)macChange.append(listM.at(i)+"/");
    macChange.append("versioning.xml");
    file = new QFile(macChange);
#endif

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text) || !file->size())
    {
        qWarning("Unable to open local file : " + file->fileName().toLatin1());
        waitConnection->stop();
        QApplication::restoreOverrideCursor();
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this,tr("Error"),tr("\nUnable to acces the information about updates.\n\nPlease try again later"));
        //MainWindow::close();
        button_checkupdate->setEnabled(true);
        QApplication::restoreOverrideCursor();
        return;

    }

    QString fileName = macChange;

    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
      qWarning("Error: couldn't read " + file->fileName().toLatin1() + " , error message: " + errorMsg.toLatin1());
      file->close();
    }
    file->close();

    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    QDomElement a = root.firstChildElement(trigrammeProduct);
    QDomElement b = a.firstChildElement("new-version");

    // exemple de nom de version : IR_IR_V01.17.01
    QStringList list = product->getProductFirmwareRev().split(".");//découpe le nom de la version selon les "."

    int nUp=0;

    QString nUpdate = "";
    if(list.size()>2){
        nUpdate = QString("%1%2").arg( list.at(1)).arg( list.at(2));
        nUp = nUpdate.toInt();//convertion en int des 4 derniers chiffres du numéro de version
    }
    else nUp = -1; //version du soft sur le produit foireuse
    QString lastUpdate = ""; // nom du fichier de la derniere version
    int nLastUp = -2; //convertion en int des 4 derniers chiffres des numéros de versions de la base de données pour voir le plus grand (derniere version)
    QString nUpdate2;

    while(!b.isNull()){// balaye les versions disponibles sur la base de données
      QDomElement c = b.firstChildElement("name");
      QString _temp = c.text();

      // exemple de nom de version : IR_IR_V01.17.01
      QStringList _list = QString(_temp).split(".");//découpe le nom de la version selon les "."

      bool allowedUpdateValue = false;
      QString _allowedUpdate = "";
      if(_list.size()>1)_allowedUpdate = _list.at(0); // allowedUpdate = 01

      if( (_allowedUpdate == customerNumber) || customerNumber.startsWith("IR_") ) allowedUpdateValue = true;
      else allowedUpdateValue = false;

      if(allowedUpdateValue==true){ // si il y a le même numéro de modification majeure alors on l'affiche

        nUpdate2 = QString("%1%2").arg( _list.at(1)).arg( _list.at(2));
        int tempN = nUpdate2.toInt(); // conversion de la version en int
        if(tempN>nLastUp){
            nLastUp = tempN; // si c'est une version plus récente, on actualise le numéro de la derniere version
            c = b.firstChildElement("filename");
            lastUpdate =  c.text(); // on sauve le nom de fichier de la derniere version
            m_updateFilename = lastUpdate;
            strLastVersion = _temp;
        }
      }
      b = b.nextSiblingElement("new-version");
    }
    selectedRev = strLastVersion;

    if(nLastUp > nUp)// detection d'une nouvelle mise a jour
    {
        label_maj->setText(tr("An update is \navailable:\n") + strLastVersion + tr("\n\nChanges from current version:\n")+changeWithPrevious(FirmwareRev,strLastVersion));
        statutLabelmaj = 2;
        //animatedgif->stop();
        label->clear();
        if(button_maj->isVisible()==false)button_maj->show();
        QApplication::restoreOverrideCursor();


    }
    else
    {
        label_maj->setText(tr("This device is \nup to date"));
        statutLabelmaj = 3;
        //animatedgif->stop();
        label->clear();
        if(button_maj->isVisible()==true)button_maj->hide();
        label->setPixmap(QPixmap(":/MainWindow/ressources/apply.png"));
        QApplication::restoreOverrideCursor();
    }

    button_checkupdate->setEnabled(true);
    waitConnection->stop();
    QApplication::restoreOverrideCursor();
    file->close();

}

void MainWindow::checkupdaterequest(){// bouton check update
    qDebug("checkupdaterequest");

    waitConnection->start(5000);// si pas de reponse en moins de 5s -> erreur de connexion
    label_maj->setText("");
    statutLabelmaj = 1;
    if(button_maj->isVisible()==true)button_maj->hide();
    label->clear();
    label->setMovie(animatedgif);
    animatedgif->start();
    QApplication::setOverrideCursor(Qt::BusyCursor);
    button_checkupdate->setEnabled(false);
    product->checkUpdate(_fileLang);
}

void MainWindow::StopConnection(){// fonction si temps de telechargement trop long
    qDebug("StopConnection");

  QApplication::restoreOverrideCursor();
  reply->abort();
  waitConnection->stop();
  QMessageBox::critical(this,"Reading time exceeded","Unable to connect to Volfoni server, maybe your network is too slow or non-existent. \nTry it again later.\n\n");
  resetState();
  product->setProductUpdating(false);

  button_maj->hide();
}

void MainWindow::UpdateFirmwareClicked(){
    qDebug("UpdateFirmwareClicked");
    QApplication::setOverrideCursor(Qt::BusyCursor);
    progressbar->setValue(2);
    button_checkupdate->setEnabled(false);
    _localFile = false;
    timerDebugMsg->stop();

    if(product->pollUSB()<=1 || product->pollUSBApp()<=1){
        if(product->isConnected() || product->isConnectedApp())// verifie qu'un produit est connecté avant d'afficher les mises à jour
        {
            UpdateFirmware();
        }
        else {
            QMessageBox::warning(this,
                                  tr("No device connected"),

                                  tr("Please connect a Volfoni product first"));
            QApplication::restoreOverrideCursor();
        }
    }else {
        QMessageBox::warning(this,
                               tr("Too many devices connected"),
                               tr("Please connect only one Volfoni product on USB ports"));
        QApplication::restoreOverrideCursor();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions de preparation à la mise à jour du produit
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::UpdateFirmware(){
    qDebug("UpdateFirmware");

    label->setMovie(animatedgif);
    animatedgif->start();
    product->setProductUpdating(true);


    progressbar->setValue(5);
    label_desc->setText(tr("Please wait.\n\nUpdate in progress..."));
    statutLabelDesc = 14;
    if(label_help->isVisible())label_help->hide();
    button_maj->hide();
    label_maj->setText("");
    statutLabelmaj = 1;

    //url = "http://www.volfoni.com/VolfoniLoader/Serveur/" + trigrammeProduct +"/"+ m_updateFilename;
    url = "http://vl.volfoni.com/Serveur/" + trigrammeProduct +"/"+ m_updateFilename;

    QFileInfo fileInfo(url.path());
    QString fileName = "";
    if(_localFile == false){
        fileName = fileInfo.fileName();
#ifdef Q_OS_MAC
        QStringList listM = QString(_fileLang).split("/");
        fileName ="";
        for(int i = 0; i<listM.size()-1;i++)fileName.append(listM.at(i)+"/");
        fileName.append(m_updateFilename);
#endif

        if (QFile::exists(fileName))QFile::remove(fileName);

        file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, "HTTP",
                                     tr("Unable to download the file %1: %2.")
                                     .arg(fileName).arg(file->errorString()));
            delete file;
            file = 0;
            QApplication::restoreOverrideCursor();
            return;
        }
        // schedule the request
        httpRequestAborted = false;

        //file is created, we close it
        file->close();
        delete file;
        file=0;

        product->startRequestFirmware(url);

    }else{
        QStringList list = QString(m_updateFilename).split("/");
        fileName = list.at(list.size()-1);
        // Si le fichier n'est pas dans le repertoire de l'executable (fichier temporaire), on le copie dedans
#ifdef Q_OS_MAC
        QStringList listv = QString(_fileLang).split("/");
        fileName ="";
        for(int i = 0; i<listv.size()-1;i++)fileName.append(listv.at(i)+"/");
        fileName.append(list.at(list.size()-1));
        if (!QFile::exists(fileName))QFile::copy(m_updateFilename,fileName);
#else
        if (!QFile::exists(fileName))QFile::copy(m_updateFilename,fileName);
#endif

        //QFile::copy(m_updateFilename,fileName);
        product->FirmwareFinished();
    }
}

void MainWindow::WriteDeviceFinished(void){
    qDebug("WriteDeviceFinished");
    QApplication::restoreOverrideCursor();

    if(product->isConnected())
        product->resetDevice();
    else
    {
        product->readFirmwareData();
        DisplayFirmwareData();
    }

    saveConfigXML();
    label_maj->setText(tr("This device is \nup to date"));
    statutLabelmaj = 3;
    label->setPixmap(QPixmap(":/MainWindow/ressources/apply.png"));

    // update de la qstatusbar
    if(trigrammeProduct =="AME") {
        _msg = adaptTextQStatusBar(tr("Activ Me detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        statutmsg=2;
    }
    else if(trigrammeProduct =="EDG"){
        _msg = adaptTextQStatusBar(tr("EDGE 3D Glasses detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        statutmsg=7;
    }
    else if(trigrammeProduct =="AHU"){
        _msg = adaptTextQStatusBar(tr("Activ Hub detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        statutmsg=3;
    }
    else if(trigrammeProduct =="SCC"){
        _msg = adaptTextQStatusBar(tr("Smart Crystal Cinema detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        statutmsg=4;
    }
    else if(trigrammeProduct =="SCP"){
        _msg = adaptTextQStatusBar(tr("Smart Crystal Pro detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        statutmsg=5;
    }
    else if(trigrammeProduct =="SCD"){
        _msg = adaptTextQStatusBar(tr("Smart Crystal DIAMOND detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        statutmsg=8;
    }
    else {
        _msg = adaptTextQStatusBar(tr("Impossible to update this product using Volfoni Loader"));
        statutmsg=6;
    }
    QApplication::restoreOverrideCursor();

    button_checkupdate->setEnabled(true);
    QApplication::restoreOverrideCursor();
    progressbar->setValue(100);
    button_maj->hide();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions de sauvegarde locale
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::saveConfigXML(){
    qDebug("saveConfigXML");

    QString fileName = QString("../config.xml");
#ifdef Q_OS_MAC
    QStringList list = QString(_fileLang).split("/");
    fileName ="";
    for(int i = 0; i<list.size()-1;i++)fileName.append(list.at(i)+"/");
    fileName.append("config.xml");
#endif
    QFile file(fileName);
    // lecture du fichier local et sauvegarde temporaire de ses infos
    // Variables temporaires

    QString tempoSerialNumber[10000];
    QString tempoVersion[10000];
    QString tempoDate[10000];
    QString tempoPrevious[10000];
    int nProducts = 0;
    bool newOne = true; // comparaison avec les numeros de series pour savoir si ce produit a déjà été mis à jour dans le passé sur cet ordinateur
    if(QFile::exists(fileName)==true){ // si il n'existe pas, on passe à la suite

        QDomDocument settingsF(fileName);
        //handle non-existent file case (print debug message and return)
        if (!file.open(QIODevice::ReadOnly)){
          std::cout << "Error while reading configuration file" << std::endl;
        }
        QString errorMsg;
        if(!settingsF.setContent(&file, &errorMsg)){ //handle error (print error message on pane for instance)
          std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
          file.close();
        }
        file.close();

        //parse the settings
        QDomElement rootF = settingsF.documentElement();
        QDomElement prod = rootF.firstChildElement("product");
        while(!prod.isNull()){
          QDomElement temp = prod.firstChildElement("SerialNumber");
          tempoSerialNumber[nProducts] = temp.text();
          if(SerialNumber == tempoSerialNumber[nProducts])newOne = false; // le produit a deja été mis a jour sur ce PC

          temp = prod.firstChildElement("FirmwareRev");
          tempoVersion[nProducts] = temp.text();

          temp = prod.firstChildElement("previousFirmwareRev");
          tempoPrevious[nProducts] = temp.text();

          temp = prod.firstChildElement("lastUpdateDate");
          tempoDate[nProducts] = temp.text();

          nProducts++;
          prod = prod.nextSiblingElement("product");
        }
    }

    // sauvegarde du fichier local de config
    QDomDocument settings("config");
    settings.appendChild(settings.createComment(" Copyright (c) 2011 Volfoni. "));
    settings.appendChild(settings.createComment(" All rights reserved. "));
    QDomElement root = settings.createElement("body");
    settings.appendChild(root);

    for(int i =0; i<nProducts;i++){// ajout des anciens
        QString tempo ="";
        QDomElement product_ = settings.createElement("product");
        root.appendChild(product_);

        QDomElement _serialNumber = settings.createElement("SerialNumber");
        product_.appendChild(_serialNumber);
        QDomText serialNumber_ = settings.createTextNode(tempoSerialNumber[i]);

        _serialNumber.appendChild(serialNumber_);

        QDomElement _FirmwareRev = settings.createElement("FirmwareRev");
        product_.appendChild(_FirmwareRev);
        tempo = tempoSerialNumber[i];
        if(SerialNumber == tempoSerialNumber[i])tempo = selectedRev;
        else tempo = tempoVersion[i];
        QDomText FirmwareRev_ = settings.createTextNode(tempo);
        _FirmwareRev.appendChild(FirmwareRev_);

        QDomElement _previousFirmwareRev = settings.createElement("previousFirmwareRev");
        product_.appendChild(_previousFirmwareRev);
        if(newOne == false){
            if(SerialNumber == tempoSerialNumber[i])tempo = previousRev;// la version anciennement ecrite en nouvelle devient l'ancienne
            else tempo = tempoPrevious[i];
        }else tempo = tempoPrevious[i];
        QDomText previousFirmwareRev_ = settings.createTextNode(tempo);
        _previousFirmwareRev.appendChild(previousFirmwareRev_);

        QDomElement _lastUpdateDate = settings.createElement("lastUpdateDate");
        product_.appendChild(_lastUpdateDate);
        if(SerialNumber == tempoSerialNumber[i]){
            QDateTime D=QDateTime::currentDateTime();
            tempo = "";
            tempo.append(D.toString("yyyy/MM/dd - hh:mm:ss"));
        }
        else tempo = tempoDate[i];
        QDomText lastUpdateDate_ = settings.createTextNode(tempo);
        _lastUpdateDate.appendChild(lastUpdateDate_);
    }
    if(newOne == true){
        // ajout du nouveau a la suite
        QDomElement product_ = settings.createElement("product");
        root.appendChild(product_);

        QDomElement _serialNumber = settings.createElement("SerialNumber");
        product_.appendChild(_serialNumber);
        QDomText serialNumber_ = settings.createTextNode(SerialNumber);

        _serialNumber.appendChild(serialNumber_);

        QDomElement _FirmwareRev = settings.createElement("FirmwareRev");
        product_.appendChild(_FirmwareRev);
        QDomText FirmwareRev_ = settings.createTextNode(selectedRev);
        _FirmwareRev.appendChild(FirmwareRev_);

        QDomElement _previousFirmwareRev = settings.createElement("previousFirmwareRev");
        product_.appendChild(_previousFirmwareRev);
        QDomText previousFirmwareRev_ = settings.createTextNode(previousRev);
        _previousFirmwareRev.appendChild(previousFirmwareRev_);

        QDomElement _lastUpdateDate = settings.createElement("lastUpdateDate");
        product_.appendChild(_lastUpdateDate);
        QDateTime D=QDateTime::currentDateTime();
        QString S= "";
        S.append(D.toString("yyyy/MM/dd - hh:mm:ss"));
        QDomText lastUpdateDate_ = settings.createTextNode(S);
        _lastUpdateDate.appendChild(lastUpdateDate_);
    }

    QString xml = settings.toString();

    //write configuration to file
    if(!file.open(QIODevice::WriteOnly)){
      // return fileOpenError;
    }

    QTextStream out(&file);
    out << xml;
    out.flush();
    file.close();
}




void MainWindow::closeEvent(QCloseEvent *event){
    qDebug("closeEvent");

    /*QMessageBox::StandardButton button =
            QMessageBox::question(this,
                                  tr("Stop Volfoni Loader"),
                                  tr("Are you sure to quit Volfoni Loader?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::Yes)
    {
        event->accept();
    }
    else event->ignore();*/

    QStringList listFilter;
    listFilter << "*.hex"<< "*ing.xml";
    QString fileName = "./";
#ifdef Q_OS_MAC
    QStringList list = QString(_fileLang).split("/");
    fileName ="";
    for(int i = 0; i<list.size()-1;i++)fileName.append(list.at(i)+"/");
#endif
    QDirIterator dirIterator(fileName, listFilter ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    QStringList fileList;
    while(dirIterator.hasNext())
    {
        fileList << dirIterator.next();
    }
    for(int j = 0; j<fileList.size();j++){
        QFile::remove(fileList.at(j));
    }
}

void MainWindow::mousePressEvent(QMouseEvent * event){
    qDebug("mousePressEvent");
    if((event->buttons() & Qt::LeftButton)){
        _pendingMouseButtonEvent |= (int)event->buttons();
#ifdef Q_OS_MAC
        if(label_help->isVisible())if(QRect(QPoint(320,100), QSize(30,30)).contains(event->pos()))helpBootloader();
#else
       if(label_help->isVisible())if(QRect(QPoint(320,120), QSize(30,30)).contains(event->pos()))helpBootloader();
#endif

    }else{
        // Untreated event
        event->ignore();
    }

}

