#include <QtGui>
#include <QList>

#include "mainwindow.h"
#include "login.h"

MainWindow::MainWindow(QWidget *parent, QString pathResource)
    : QMainWindow(parent),// QDialog(parent),
      init(false),
      _pathResource(pathResource)
{
    resize(800,600);
    setWindowTitle("Firmware Manager V00.04.00");
    setWindowIcon(QIcon(":/MainWindow/ressources/manager.ico"));
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    nDevices= 0;
    for(int i =0; i<100;i++){trigramDevices[i]="";nameDevices[i]="";}

    trigramDevices[0]="AME";
    nameDevices[0]="Active Me";

    trigramDevices[1]="AHU";
    nameDevices[1]="Active Hub";

    trigramDevices[2]="SCC";
    nameDevices[2]="Smart Crystal cinema";

    trigramDevices[3]="SCP";
    nameDevices[3]="Smart Crystal pro";

    trigramDevices[4]="EDG";
    nameDevices[4]="EDGE 3D Glasses";

    trigramDevices[5]="SCD";
    nameDevices[5]="Smart Crystal Diamond";

    trigramDevices[6]="SCL";
    nameDevices[6]="Smart Crystal Lite";

    for(int i =0; i<100;i++){
        if(trigramDevices[i]!="")nDevices++;
        else break;
    }

    closeAccepted = true;

    m_save = new QPushButton(tr("Save"));
    m_save->setCursor(Qt::PointingHandCursor);
    m_save->setFixedWidth(100);
    m_save->setEnabled(false);
    connect(m_save, SIGNAL(clicked()), this, SLOT(save()));

    m_saveQuit = new QPushButton(tr("Save and quit"));
    m_saveQuit->setCursor(Qt::PointingHandCursor);
    m_saveQuit->setFixedWidth(100);
    m_saveQuit->setEnabled(false);
    connect(m_saveQuit, SIGNAL(clicked()), this, SLOT(saveQuit()));

    m_cancel = new QPushButton(tr("Cancel"));
    m_cancel->setCursor(Qt::PointingHandCursor);
    m_cancel->setFixedWidth(100);
    connect(m_cancel, SIGNAL(clicked()), this, SLOT(close()));


    layout_buttons = new QHBoxLayout;
    layout_buttons->addWidget(m_save);
    layout_buttons->addWidget(m_saveQuit);
    layout_buttons->addWidget(m_cancel);

    tabWidget = new QTabWidget;

    generaltab = new GeneralTab("", this,trigramDevices,nDevices);
    tabWidget->addTab(generaltab, tr("General tab"));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));


    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::HLine /*| QFrame::Raised*/);
    frame->setLineWidth(10);
    frame->setMidLineWidth(3);
    frame->setPalette(QColor(255,255,255));

    layout_principal = new QVBoxLayout;
    layout_principal->addWidget(tabWidget);
    layout_principal->addSpacing(20);
    layout_principal->addLayout(layout_buttons);

    QString toolsIcon = ":/MainWindow/ressources/tools.png";
    QString aboutIcon = ":/MainWindow/ressources/about.png";

    _CRC32Action = new QAction(QIcon(toolsIcon), tr("Calculate a CRC32"), this);
    connect(_CRC32Action, SIGNAL(triggered()), this, SLOT(CRC32_Function()));

    _EncryptAction = new QAction(QIcon(toolsIcon), tr("Encrypt a file"), this);
    connect(_EncryptAction, SIGNAL(triggered()), this, SLOT(Encrypt_Function()));

    _DecryptAction = new QAction(QIcon(toolsIcon), tr("Decrypt a file"), this);
    connect(_DecryptAction, SIGNAL(triggered()), this, SLOT(Decrypt_Function()));

    _AboutAction = new QAction(QIcon(aboutIcon), tr("About Manager"), this);
    connect(_AboutAction, SIGNAL(triggered()), this, SLOT(aboutManager()));

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(_CRC32Action);
    toolsMenu->addAction(_EncryptAction);
    toolsMenu->addAction(_DecryptAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(_AboutAction);

    QWidget *widget_principal = new QWidget;
    widget_principal->setLayout(layout_principal);
    //setLayout(layout_principal);
    setCentralWidget(widget_principal);
    setMinimumHeight(270);

}

void MainWindow::aboutManager(){
    QMessageBox::about(this,
                 tr("About"),
                 tr("<p align='center'>Manager for Firmware Versioning V00.00.00<br>"
                    "Copyright (c) 2012 Volfoni.</p>"));
}


void MainWindow::Decrypt_Function(){

    DecryptFunction decr(this,this);
    decr.exec();

}

void MainWindow::Encrypt_Function(){

    EncryptFunction encr(this,this);
    encr.exec();
}

void MainWindow::CRC32_Function(){
    CRC32 crc(this,this, _pathResource);
    crc.exec();

}

void MainWindow::currentTabChanged(int a){

    if(a==nDevices+1)viewtab[0]->drawTable();
}

int MainWindow::CurrentTabIndex(){
    return tabWidget->currentIndex();
}

void MainWindow::resetTabs(){
    for(int n =0; n<=nDevices;n++){
        tabWidget->removeTab(1);
    }
}

void MainWindow::setTabs(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //for(int n =0; n<2;n++){
    for(int n =0; n<nDevices;n++){
        qDebug("Nouveau Tab Device");
        edittab[n] = new EditTab(trigramDevices[n], this, generaltab->getFilename(), generaltab->getFolder(),_pathResource);
        tabWidget->addTab(edittab[n], nameDevices[n]);  
    }
    viewtab[0] = new ViewTab(this, nDevices);
    tabWidget->addTab(viewtab[0], "Table view");
    QApplication::restoreOverrideCursor();
    m_save->setEnabled(true);
    m_saveQuit->setEnabled(true);
    closeAccepted = false;
}

void MainWindow::saveQuit(){
    QDomDocument settings("products_versions");
    QDomElement root = settings.createElement("versions");
    settings.appendChild(root);

    for(int n =0; n<nDevices;n++){
        QDomElement process = edittab[n]->save();
        root.appendChild(process);
    }
    bool ok = true;
    for(int n =0; n<nDevices;n++){
        if(edittab[n]->getSucces()==false)ok = false;
    }
    if(ok==true){
        QString xml = settings.toString();

        QString fileName = generaltab->getFilename();
        //write configuration to file
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly)){
            // return fileOpenError;
        }

        QTextStream out(&file);
        out << xml;
        out.flush();
        file.close();
        QMessageBox::about(this,"Notify", "Save a local copy Finished!");
        closeAccepted = true;
        QMessageBox::StandardButton button =
                QMessageBox::question(this,
                                      tr("Features"),
                                      tr("Do you want to upload changes on server or archive root folder?\nBe sure that there is no error before to do that."),
                                      QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes){
            Features features(this,this,generaltab->getFolder(), trigramDevices, nDevices);
            features.exec();
        }

        MainWindow::close();
    }else {
        QMessageBox::about(this,"Notify", "Fix problems before to save!");
        for(int n =0; n<nDevices;n++){
            edittab[n]->setSucces();
        }
    }

}

void MainWindow::save(){
    QDomDocument settings("products_versions");
    QDomElement root = settings.createElement("versions");
    settings.appendChild(root);

    for(int n =0; n<nDevices;n++){
        QDomElement process = edittab[n]->save();
        root.appendChild(process);
    }
    bool ok = true;
    for(int n =0; n<nDevices;n++){
        if(edittab[n]->getSucces()==false)ok = false;
    }
    if(ok==true){
        QString xml = settings.toString();

        QString fileName = generaltab->getFilename();
        //write configuration to file
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly)){
            // return fileOpenError;
        }

        QTextStream out(&file);
        out << xml;
        out.flush();
        file.close();
        QMessageBox::about(this,"Notify", "Save a local copy Finished!");
        closeAccepted = true;

        //MainWindow::close();
    }else {
        QMessageBox::about(this,"Notify", "Fix problems before to save!");
        for(int n =0; n<nDevices;n++){
            edittab[n]->setSucces();
        }
    }

}

void MainWindow::logUser(QString m_url, QString m_login, QString m_pwd){

    sendserver = new SendServer(this, this, generaltab->getFolder(), m_url, m_login, m_pwd, trigramDevices, nDevices);

}

void MainWindow::closeEvent(QCloseEvent *event){
#ifdef Q_WS_WIN32
    if(QFile::exists("tempo.hex"))QFile::remove("tempo.hex");
#else
    if(QFile::exists(_pathResource+"tempo.hex"))QFile::remove(_pathResource+"tempo.hex");
#endif

    if(closeAccepted == false){
        QMessageBox::StandardButton button =
                QMessageBox::question(this,
                                      tr("Stop Volfoni Loader versioning"),
                                      tr("Are you sure to quit Volfoni Loader versioning without saving?"),
                                      QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes)
        {
            event->accept();
        }
        else event->ignore();
    }else event->accept();
}


void MainWindow::sendToServer(){

    Login log_in(this, false);
    log_in.setMW(this);
    log_in.exec();

}

Features::Features(QWidget* parent,MainWindow *mW, QString rootFolder, QString trigrams[100], int nDevices)
: QDialog(parent,Qt::WindowCloseButtonHint),
  _mW(mW),
  _rootFolder(rootFolder)
{

    _nDevices = nDevices;
    for(int i = 0; i<_nDevices;i++)_trigrams[i]= trigrams[i];
    resize(550,200);
    this->setWindowIcon(QIcon(":/MainWindow/ressources/volfo160.ico"));
    this->setWindowTitle("Features");

    m_apply = new QPushButton(tr("OK"));
    m_apply->setCursor(Qt::PointingHandCursor);
    m_apply->setFixedWidth(100);
    connect(m_apply, SIGNAL(clicked()), this, SLOT(save()));

    m_cancel = new QPushButton(tr("Cancel"));
    m_cancel->setCursor(Qt::PointingHandCursor);
    m_cancel->setFixedWidth(100);
    connect(m_cancel, SIGNAL(clicked()), this, SLOT(close()));

    layout_buttons = new QHBoxLayout;
    layout_buttons->addWidget(m_apply,0,Qt::AlignCenter);
    layout_buttons->addWidget(m_cancel,0,Qt::AlignCenter);

    zipFolder = new QCheckBox("Archive folder");
    zipName = new QLineEdit("");
    sendFolder = new QCheckBox("Send folder to server");

    QLabel *label = new QLabel("Name for archive:");

    QLabel *labelF = new QLabel("Save in folder:");
    workingDirectory = new QLineEdit(_rootFolder);
    workingDirectory->setReadOnly(true);
    browse = new QPushButton(tr("Browse"));
    browse->setCursor(Qt::PointingHandCursor);
    browse->setFixedWidth(100);
    connect(browse, SIGNAL(clicked()), this, SLOT(browseFolder()));
    QHBoxLayout *layout_browse = new QHBoxLayout;
    layout_browse->addWidget(workingDirectory);
    layout_browse->addWidget(browse);

    QDate date = QDate::currentDate();
    QString dateString = date.toString();

    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    timeString.replace(":",".");

    QString text = "Server_copy_"+dateString+"__"+timeString;
    text.replace(" ","_");

    zipName->setText(text);

    QVBoxLayout *layout_zip = new QVBoxLayout;
    layout_zip->addWidget(zipFolder);
    layout_zip->addWidget(label);
    layout_zip->addWidget(zipName);
    layout_zip->addWidget(labelF);
    layout_zip->addLayout(layout_browse);

    QFrame *frame1 = new QFrame;
    frame1->setFrameStyle(QFrame::VLine);
    frame1->setLineWidth(10);
    frame1->setMidLineWidth(3);
    frame1->setPalette(QColor(255,255,255));

    layout_checkboxes = new QHBoxLayout;
    layout_checkboxes->addLayout(layout_zip);
    layout_checkboxes->addWidget(frame1);
    layout_checkboxes->addWidget(sendFolder);

    layout_principal = new QVBoxLayout;
    layout_principal->addLayout(layout_checkboxes);
    layout_principal->addLayout(layout_buttons);

    setLayout(layout_principal);


}

Features::~Features()
{

}

void Features::browseFolder(){
    QString fileName = QFileDialog::getExistingDirectory(this,tr("Select a folder"),workingDirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(fileName.isEmpty()){
      //the user did not choose a  file
      return;
    }
    fileName.replace("\\","/");
    fileName.append("/");
    workingDirectory->setText(fileName);
}

void Features::save(){
    if(zipFolder->isChecked()==true){
        QString name = zipName->text();
        QDir dir(workingDirectory->text().toStdString().c_str());
        if(dir.exists(name.toStdString().c_str())==false){

            bool ok = dir.mkdir(name.toStdString().c_str());
            if(ok==false){
                QMessageBox::warning(this,"Notify", "Incorrect name for archive");
                return;
            }
            QFile::copy(_rootFolder+"versioning.xml",workingDirectory->text()+name+"/versioning.xml");
            for(int i =0;i<_nDevices;i++){
                if(dir.exists((name+"/"+_trigrams[i]).toStdString().c_str())==false)dir.mkdir((name+"/"+_trigrams[i]).toStdString().c_str());
                QStringList listFilter;
                listFilter << "*.hex";
                QDirIterator dirIterator(_rootFolder+_trigrams[i], listFilter ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
                QStringList fileList;
                // Tant qu'on n'est pas arrivé à la fin de l'arborescence...
                while(dirIterator.hasNext())
                {
                    QStringList tempoList = QString(dirIterator.next()).split("/");
                    QString tempo = tempoList.at(tempoList.size()-1);
                    fileList << tempo;

                }
                for(int j = 0; j<fileList.size();j++){
                    QFile::copy(_rootFolder+_trigrams[i]+"/"+fileList.at(j),workingDirectory->text()+name+"/"+_trigrams[i]+"/"+fileList.at(j));
                }
            }
        }
    }

    close();
    if(sendFolder->isChecked()==true){
        _mW->sendToServer();
    }
    if(zipFolder->isChecked()==false && sendFolder->isChecked()==false)QMessageBox::about(this,"Notify", "No features selected.");

}

