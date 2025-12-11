#include <QtGui>
#include <QList>
#include <QtNetwork>
#include <QTranslator>
#include <QApplication>
#include <QWidget>
#include <QtWidgets>

#include "tools.h"

SelectDevice::SelectDevice(QWidget* parent)
    : QDialog(parent,Qt::WindowCloseButtonHint)

 {
    this->setWindowTitle(tr("Select your product"));
    this->resize(250,80);


    QPushButton *m_save = new QPushButton(tr("OK"));
    m_save->setCursor(Qt::PointingHandCursor);
    m_save->setFixedWidth(70);
    connect(m_save, SIGNAL(clicked()), this, SLOT(valid()));

    products = new QComboBox();
    products->addItem("Activ Me");
    products->addItem("Activ Hub");
    products->addItem("Smart Crystal Cinema");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(products);
    mainLayout->addWidget(m_save,0,Qt::AlignRight);

    setLayout(mainLayout);
}

void SelectDevice::valid(){
    SelectDevice::close();
    if(products->currentIndex()==0){
        HelpBootLoader helpBoot(this,"AME");
        helpBoot.exec();
    }
    if(products->currentIndex()==1){
        HelpBootLoader helpBoot(this,"AHU");
        helpBoot.exec();
    }
    if(products->currentIndex()==2){
        HelpBootLoader helpBoot(this, "SCC");
        helpBoot.exec();
    }

}

HelpBootLoader::HelpBootLoader(QWidget* parent, QString trigram)
    : QDialog(parent,Qt::WindowCloseButtonHint),
      _trigram(trigram)
 {

    this->setWindowTitle(tr("Help to turn your device in update mode"));

    QPushButton *m_save = new QPushButton(tr("OK"));
    m_save->setCursor(Qt::PointingHandCursor);
    m_save->setFixedWidth(100);
    connect(m_save, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->addWidget(m_save, 0, Qt::AlignCenter);

    QMovie *animatedAHU = new QMovie(":/MainWindow/ressources/turnBootloaderAHU.gif", QByteArray(), this);
    QMovie *animatedAME = new QMovie(":/MainWindow/ressources/turnBootloaderAME.gif", QByteArray(), this);
    QMovie *animatedSCC = new QMovie(":/MainWindow/ressources/turnBootloaderSCC.gif", QByteArray(), this);
    QMovie *animatedSCP = new QMovie(":/MainWindow/ressources/turnBootloaderSCP.gif", QByteArray(), this);

    QLabel *label_picture = new QLabel();
    label_picture->setFixedSize(400,270);
    label_picture->setAlignment(Qt::AlignCenter);
    if(trigram == "AME"){
        label_picture->setMovie(animatedAME);
        animatedAME->start();
    }
    else if(trigram == "AHU"){
        label_picture->setMovie(animatedAHU);
        animatedAHU->start();
    }
    else if(trigram == "SCC"){
        label_picture->setMovie(animatedSCC);
        animatedSCC->start();
    }
    else if(trigram == "SCP"){
        label_picture->setMovie(animatedSCP);
        animatedSCP->start();
    }
    label_picture->setStyleSheet("background: white");

    QString textHelp = "";
    if(trigram == "AME")textHelp = tr("Keep pushed the button under the device and then turn on the product with the switch.");
    else if(trigram == "AHU")textHelp = tr("Keep pushed the button under the device and plug device to an free USB port.");
    else if(trigram == "SCC")textHelp = tr("Keep pushed the button under the device and plug device to an free USB port.");
    else if(trigram == "SCP")textHelp = tr("Keep pushed the button under the device and plug device to an free USB port.");

    QLabel *label_pictureText = new QLabel(textHelp);
    QVBoxLayout *helpLayout = new QVBoxLayout;
    helpLayout->addWidget(label_picture, 0, Qt::AlignCenter);
    helpLayout->addWidget(label_pictureText, 0, Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(helpLayout);
    mainLayout->addLayout(layout_buttons);

    setLayout(mainLayout);

 }

SelectLanguage::SelectLanguage(QWidget* parent, MainWindow *mW,QString filename)
    : QDialog(parent,Qt::WindowCloseButtonHint),
      _mW(mW),
      _filename(filename)

 {
    QString englishIcon = ":/MainWindow/ressources/english.png";
    QString frenchIcon = ":/MainWindow/ressources/french.png";
    QString germanIcon = ":/MainWindow/ressources/german.png";
    QString spanishIcon = ":/MainWindow/ressources/spanish.png";

    this->setWindowTitle(tr("Select language"));
    this->resize(250,80);


    QPushButton *m_save = new QPushButton(tr("OK"));
    m_save->setCursor(Qt::PointingHandCursor);
    m_save->setFixedWidth(70);
    connect(m_save, SIGNAL(clicked()), this, SLOT(valid()));

    languages = new QComboBox();
    languages->addItem(QIcon(englishIcon)," English");
    languages->addItem(QIcon(frenchIcon)," French");
    languages->addItem(QIcon(germanIcon)," German");
    languages->addItem(QIcon(spanishIcon)," Spanish");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(languages);
    mainLayout->addWidget(m_save,0,Qt::AlignRight);

    setLayout(mainLayout);
#ifdef Q_WS_WIN32
    QString pathName = QString("../Config");
    if(QDir(pathName).exists()==false){
        QDir dir;
        dir.mkpath(pathName);
    }
#endif
    QString fileName = QString(_filename);
    QFile file(fileName);

    if(QFile::exists(fileName)==true){

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

        oldLanguage = "";
        if(langSelected=="english"){languages->setCurrentIndex(0);oldLanguage = "english";}
        else if(langSelected=="french"){languages->setCurrentIndex(1);oldLanguage = "french";}
        else if(langSelected=="german"){languages->setCurrentIndex(2);oldLanguage = "german";}
        else if(langSelected=="spanish"){languages->setCurrentIndex(3);oldLanguage = "spanish";}
    }
}

void SelectLanguage::valid(){
    SelectLanguage::close();
    QString language = "";
    if(languages->currentIndex()==0){
        language="english";
    }
    if(languages->currentIndex()==1){
        language="french";
    }
    if(languages->currentIndex()==2){
        language="german";
    }
    if(languages->currentIndex()==3){
        language="spanish";
    }
#ifdef Q_WS_WIN32
    QString pathName = QString("../Config");
    if(QDir(pathName).exists()==false){
        QDir dir;
        dir.mkpath(pathName);
    }
#endif
    QString fileName = QString(_filename);
    QFile file(fileName);

    // sauvegarde du fichier local de config
    QDomDocument settings("language");
    settings.appendChild(settings.createComment(" Copyright (c) 2011 Volfoni. "));
    settings.appendChild(settings.createComment(" All rights reserved. "));
    QDomElement root = settings.createElement("language");
    settings.appendChild(root);

    QDomElement _language = settings.createElement("langSelected");
    root.appendChild(_language);
    QDomText language_ = settings.createTextNode(language);
    _language.appendChild(language_);

    QString xml = settings.toString();

    if(!file.open(QIODevice::WriteOnly)){
      // return;
    }

    QTextStream out(&file);
    out << xml;
    out.flush();
    file.close();

    _mW->retranslateApp(language, oldLanguage);

}


SelectLanguageInit::SelectLanguageInit(QString filename)
    : QDialog(),
      _filename(filename)
 {
    QString englishIcon = ":/MainWindow/ressources/english.png";
    QString frenchIcon = ":/MainWindow/ressources/french.png";
    QString germanIcon = ":/MainWindow/ressources/german.png";
    QString spanishIcon = ":/MainWindow/ressources/spanish.png";

    closeAccepted = false;
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowTitle("Select language for Volfoni Loader");
    this->resize(300,80);

    QPushButton *m_save = new QPushButton("OK");
    m_save->setCursor(Qt::PointingHandCursor);
    m_save->setFixedWidth(70);
    connect(m_save, SIGNAL(clicked()), this, SLOT(valid()));

    languages = new QComboBox();
    languages->addItem(QIcon(englishIcon)," English");
    languages->addItem(QIcon(frenchIcon)," French");
    languages->addItem(QIcon(germanIcon)," German");
    languages->addItem(QIcon(spanishIcon)," Spanish");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(languages);
    mainLayout->addWidget(m_save,0,Qt::AlignRight);

    setLayout(mainLayout);
}

void SelectLanguageInit::valid(){

    QString language = "";
    if(languages->currentIndex()==0){
        language="english";
    }
    if(languages->currentIndex()==1){
        language="french";
    }
    if(languages->currentIndex()==2){
        language="german";
    }
    if(languages->currentIndex()==3){
        language="spanish";
    }
#ifdef Q_WS_WIN32
    QString pathName = QString("../Config");
    if(QDir(pathName).exists()==false){
        QDir dir;
        dir.mkpath(pathName);
    }
#endif
    QString fileName = QString(_filename);
    QFile file(fileName);

    // sauvegarde du fichier local de config
    QDomDocument settings("language");
    settings.appendChild(settings.createComment(" Copyright (c) 2011 Volfoni. "));
    settings.appendChild(settings.createComment(" All rights reserved. "));
    QDomElement root = settings.createElement("language");
    settings.appendChild(root);

    QDomElement _language = settings.createElement("langSelected");
    root.appendChild(_language);
    QDomText language_ = settings.createTextNode(language);
    _language.appendChild(language_);

    QString xml = settings.toString();

    if(!file.open(QIODevice::WriteOnly)){
      // return;
    }

    QTextStream out(&file);
    out << xml;
    out.flush();
    file.close();
    closeAccepted = true;
    SelectLanguageInit::close();

}
void SelectLanguageInit::closeEvent(QCloseEvent *event){

    if(closeAccepted == false){
        QMessageBox::warning(this,"Warning", "Select language first.");
        event->ignore();
    }else event->accept();
}
