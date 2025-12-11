/************************************************************************
* Copyright (c) 2009-2011,  Microchip Technology Inc.
*
* Microchip licenses this software to you solely for use with Microchip
* products.  The software is owned by Microchip and its licensors, and
* is protected under applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
* MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
* EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
* OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
* TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
* OR OTHER SIMILAR COSTS.
*
* To the fullest extent allowed by law, Microchip and its licensors
* liability shall not exceed the amount of fees, if any, that you
* have paid directly to Microchip to use this software.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*
* Author        Date        Comment
*************************************************************************
* E. Schlunder  2009/05/07  CRC verify added.
* E. Schlunder  2009/04/14  Initial code ported from VB app.
* T. Lawrence   2011/01/14  Initial implementation of USB version of this
*                           bootloader application.
************************************************************************/
#include <QTextStream>
#include <QByteArray>
#include <QList>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopWidget>
#include <QtConcurrentRun>
#include <QtNetwork>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <stdlib.h>
#include <stdio.h>
#include "Settings.h"
#include "manager.h"
#include "aboutbox.h"
#include "PartNumber.h"
#include "CustomerNumber.h"
#include "versionwindow.h"
#include "tools.h"
#include "sniffer.h"

#include "../version.h"

//Surely the micro doesn't have a programmable memory region greater than 268 Megabytes...
//Value used for error checking device reponse values.
#define MAXIMUM_PROGRAMMABLE_MEMORY_SEGMENT_SIZE 0x0FFFFFFF


MainWindow::MainWindow(QWidget *parent, QString pathResource)
    : QMainWindow(parent),
      ui(new Ui::MainWindowClass),
      KEY_Decrypt("volfoni123"),
      debutVersion(":102000"),
      _pathResource(pathResource)
{

    busy = false;
    ActivatedDebug = false;
    writeFlash = true;
    writeEeprom = false;
    writeConfig = false;

//    encrypt = new Encrypt();
//    decrypt = new Decrypt();
    wait  = new QTimer();
    connect(wait, SIGNAL(timeout()), this, SLOT(StopWait()));
    firmwareDownloaded = false;

    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateStatusBar()));
    _timer->start(100); // influe sur la vitesse de défilement de QStatusBar

    ui->statusBar = statusBar();
    ui->statusBar->show();
    ui->statusBar->setFixedHeight(15);

    int i;
    trigrammeProduct = "";
    customerNumber = "";

    hexOpen = false;
    fileWatcher = NULL;
    timer = new QTimer();
    DebugTimer = new QTimer();


    ui->setupUi(this);

    setWindowTitle(APPLICATION + QString(" v") + VERSION);
    //ui->plainTextEdit->setStyleSheet("background-image: url(windowlogo.png); background-repeat: repeat-n; background-position: center" );
    //ui->progressBar->setStyleSheet("color: blue");

    QSettings settings;
    settings.beginGroup("MainWindow");
    fileName = settings.value("fileName").toString();

    for(i = 0; i < MAX_RECENT_FILES; i++)
    {
        recentFiles[i] = new QAction(this);
        connect(recentFiles[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
        recentFiles[i]->setVisible(false);
        ui->menuFile->insertAction(ui->actionExit, recentFiles[i]);
    }
    ui->menuFile->insertSeparator(ui->actionExit);

    settings.endGroup();

    settings.beginGroup("WriteOptions");
    writeFlash = settings.value("writeFlash", true).toBool();
    //writeConfig = settings.value("writeConfig", false).toBool();
    writeConfig = false; //Force user to manually re-enable it every time they re-launch the application.  Safer that way.
    writeEeprom = settings.value("writeEeprom", false).toBool();
    eraseDuringWrite = true;
    settings.endGroup();

    product = new Product();

    qRegisterMetaType<Comm::ErrorCode>("Comm::ErrorCode");

    connect(timer, SIGNAL(timeout()), this, SLOT(Connection()));
    connect(this, SIGNAL(IoWithDeviceCompleted(QString,Comm::ErrorCode,double)), this, SLOT(IoWithDeviceComplete(QString,Comm::ErrorCode,double)));
    connect(this, SIGNAL(IoWithDeviceStarted(QString)), this, SLOT(IoWithDeviceStart(QString)));
    connect(this, SIGNAL(AppendString(QString)), this, SLOT(AppendStringToTextbox(QString)));
    connect(this, SIGNAL(SetProgressBar(int)), this, SLOT(UpdateProgressBar(int)));
    connect(product, SIGNAL(setProgress(int)), this, SLOT(UpdateProgressBar(int)));
    connect(product, SIGNAL(setMsgToIHM(QString)), this, SLOT(displayMsgFromProduct(QString)));
    connect(product, SIGNAL(newDataAvailable()), this, SLOT(readedDebug()));
    connect(DebugTimer, SIGNAL(timeout()), this, SLOT(on_actionGet_Msg_triggered()));
    connect(this, SIGNAL(getDebugMsg()), product, SLOT(getDebugMsg()));
    //connect(this, SIGNAL(getDebugMsg(int, unsigned char *)), product, SLOT(getDebugMsg(int, unsigned char *)));
    connect(&qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(&qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));

    _msg = adaptTextQStatusBar("Disconnected");
    statusMsg = 1;

    //Initialisation des variables qstring des msg de debug
    GetMessages();
    //Make initial check to see if the USB device is attached
    product->pollUSB();
    product->pollUSBApp();
    if(product->isConnected())
    {
        ui->checkBoxDebug->setEnabled(false);
        ui->label_mode->setText("Bootloader Mode");
        ui->label_Connected->setPixmap(QPixmap(":/MainWindow/img/Connected.png"));
        opened = true;
        qWarning("Attempting to open device...");
        product->open();
        ui->plainTextEdit->setPlainText("Device Attached.");
        ui->plainTextEdit->appendPlainText("Connecting...");
        GetQuery();
    }
    else if(product->isConnectedApp())
    {
        ui->checkBoxDebug->setEnabled(true);
        ui->label_mode->setText("Applicative Mode");
        ui->label_Connected->setPixmap(QPixmap(":/MainWindow/img/Connected.png"));
        opened = true;
        qWarning("Attempting to open device...");
        product->openApp();
        ui->plainTextEdit->setPlainText("Device Attached.");
        ui->plainTextEdit->appendPlainText("Connecting...");
        GetQuery();
    }
    else
    {
        ui->checkBoxDebug->setEnabled(false);
        if(ui->checkBoxDebug->isChecked()==true)ui->checkBoxDebug->setChecked(false);
        ActivatedDebug = false;

        if(ActivatedDebug)
        {
            DebugTimer->start(100);
        }
        else
        {
            DebugTimer->stop();
        }

        printSettings();
        ui->label_mode->setText("");
        ui->label_Connected->setPixmap(QPixmap(":/MainWindow/img/notConnected.png"));
        opened = false;
        ui->plainTextEdit->appendPlainText("Device not detected.  Verify device is attached and in firmware update mode.");
        _msg = adaptTextQStatusBar("Disconnected");
        statusMsg = 1;
        hexOpen = false;
        setBootloadEnabled(false);
        emit SetProgressBar(0);
    }

    //Update the file list in the File-->[import files list] area, so the user can quickly re-load a previously used .hex file.
    UpdateRecentFileList();
    printSettings();

    timer->start(1000); //Check for future USB connection status changes every 1000 milliseconds.
}

MainWindow::~MainWindow()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("fileName", fileName);
    settings.endGroup();

    settings.beginGroup("WriteOptions");
    settings.setValue("writeFlash", writeFlash);
    settings.setValue("writeConfig", writeConfig);
    settings.setValue("writeEeprom", writeEeprom);
    settings.endGroup();

    product->close();
    setBootloadEnabled(false);

    delete timer;
    delete ui;
    delete product;

}
void MainWindow::printSettings(){

    if(ActivatedDebug == true){
        ui->label_4P->setPixmap(QPixmap(":/MainWindow/img/green.png"));
        ui->checkBoxDebug->setChecked(true);
    }

    else {
        ui->label_4P->setPixmap(QPixmap(":/MainWindow/img/red.png"));
        ui->checkBoxDebug->setChecked(false);
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions associées à la barre de statut
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QString MainWindow::adaptTextQStatusBar(QString text){
    // ajout d'espaces pour combler la largeur de la fenetre
    bool stop = false;
    QLabel *labelTest = new QLabel(text);// utilisé pour connaitre la font du text et sa largeur
    int nSecurity = 0; // utilisé pour fixer un nombre maximum d'espaces et eviter les bugs
    QFontMetrics metrics = labelTest->fontMetrics();

    QRect current_geom = frameGeometry();
    int widthWindow = current_geom.width();
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

  if(_msg.isEmpty())
  {
     ui->statusBar->clearMessage();
  }
  else
  {
    int pos = _msg.length() - 1;
    _msg = _msg.right(pos) + _msg[0];
    ui->statusBar->showMessage(_msg);
  }
}

///////////////////////////////////////////////////////
void MainWindow::Connection(void)
{
    int status;
    Comm::ErrorCode result;
    status = product->connection();

    if(status == 9)
    {
        UpdateRecentFileList();

        if(product->isConnected())
        {
            DebugTimer->stop();
            ui->checkBoxDebug->setEnabled(false);
            ui->label_mode->setText("Bootloader Mode");
            ui->label_Connected->setPixmap(QPixmap(":/MainWindow/img/Connected.png"));
            opened = true;
            ui->plainTextEdit->setPlainText("Device Attached.");
            ui->plainTextEdit->appendPlainText("Connecting...");
            if(writeConfig)
            {
                //ui->plainTextEdit->appendPlainText("Disabling Erase button to prevent accidental erasing of the configuration words without reprogramming them\n");
                writeConfig = true;
                result = product->lockUnlockConfig(false);
                if(result == Comm::Success)
                {
                    ui->plainTextEdit->appendPlainText("Unlocked Configuration bits successfully\n");
                }
            }
            else
            {
                writeConfig = false;
            }
            GetQuery();
            printSettings();
        }
        else if(product->isConnectedApp())
        {
            bool wasDebug = ActivatedDebug;
            DebugTimer->stop();
            ui->checkBoxDebug->setEnabled(true);
            ui->label_mode->setText("Applicative Mode");
            ui->label_Connected->setPixmap(QPixmap(":/MainWindow/img/Connected.png"));
            opened = true;
            ui->plainTextEdit->setPlainText("Device Attached.");
            ui->plainTextEdit->appendPlainText("Connecting...");
            if(writeConfig)
            {
                //ui->plainTextEdit->appendPlainText("Disabling Erase button to prevent accidental erasing of the configuration words without reprogramming them\n");
                writeConfig = true;
                result = product->lockUnlockConfig(false);
                if(result == Comm::Success)
                {
                    ui->plainTextEdit->appendPlainText("Unlocked Configuration bits successfully\n");
                }
            }
            else
            {
                writeConfig = false;
            }
            GetQuery();
            printSettings();

            if(wasDebug)
            {
                DebugTimer->start(100);
            }
            else
            {
                DebugTimer->stop();
            }
        }
        else
        {
            ui->checkBoxDebug->setEnabled(false);
            if(ui->checkBoxDebug->isChecked()==true)ui->checkBoxDebug->setChecked(false);
            ActivatedDebug = false;

            DebugTimer->stop();

            printSettings();
            ui->label_mode->setText("");
            ui->label_Connected->setPixmap(QPixmap(":/MainWindow/img/notConnected.png"));
            opened = false;
//            qWarning("Closing device.");
//            product->close();
            _msg = adaptTextQStatusBar("Disconnected");
            statusMsg = 1;
            ui->plainTextEdit->setPlainText("Device Detached.");
            ui->PartNumberTextEdit->setText("");
            ui->FirmwareRevTextEdit->setText("");
            ui->BootloaderRevTextEdit->setText("");
            ui->InitialFirmwareTextEdit->setText("");
            //hexOpen = false;
            setBootloadEnabled(false);
            emit SetProgressBar(0);
        }
    }
    if(product->isConnected()==false && product->isConnectedApp()==false && opened==true){
        DebugTimer->stop();
        std::cout<<"pas connecté, probleme"<<std::endl;
        ui->label_mode->setText("");
        ui->label_Connected->setPixmap(QPixmap(":/MainWindow/img/notConnected.png"));
        opened = false;
//        qWarning("Closing device.");
//        product->close();
        _msg = adaptTextQStatusBar("Disconnected");
        statusMsg = 1;
        ui->plainTextEdit->setPlainText("Device Detached.");
        ui->PartNumberTextEdit->setText("");
        ui->FirmwareRevTextEdit->setText("");
        emit SetProgressBar(0);
    }
}

void MainWindow::setBootloadEnabled(bool enable)
{
    ui->action_Settings->setEnabled(enable);
    ui->action_Manager->setEnabled(enable);
    ui->actionErase_Device->setEnabled(enable && !writeConfig);
    ui->actionWrite_Device->setEnabled(enable && hexOpen);
    //ui->actionExit->setEnabled(enable);
    ui->action_Verify_Device->setEnabled(enable && hexOpen);
    ui->actionOpen->setEnabled(enable);

    ui->actionDownload->setEnabled(enable);
    ui->actionBlank_Check->setEnabled(enable && !writeConfig);
    ui->actionReset_Device->setEnabled(enable);
    ui->actionGet_Msg->setEnabled(enable); //Modif DDE
    //ui->actionEdit_Part_Number->setEnabled(enable);
    EnableEdit_Part_Number(enable);
    ui->actionEdit_Customer_Number_First_Firmware_Revision->setEnabled(enable);
    ui->actionRead_Memory->setEnabled(enable);
    ui->actionWrite_Memory->setEnabled(enable);
}

void MainWindow::setBootloadBusy(bool busy)
{
    if(busy)
    {
        QApplication::setOverrideCursor(Qt::BusyCursor);
        timer->stop();
        qWarning("stop device detection");
    }
    else
    {
        QApplication::restoreOverrideCursor();
        timer->start(1000);
        qWarning("start device detection");
    }

    ui->action_Settings->setEnabled(!busy);
    ui->action_Manager->setEnabled(!busy);
    ui->actionErase_Device->setEnabled(!busy && !writeConfig);
    ui->actionWrite_Device->setEnabled(!busy && hexOpen);
    ui->actionExit->setEnabled(!busy);
    ui->action_Verify_Device->setEnabled(!busy && hexOpen);
    ui->actionOpen->setEnabled(!busy);
    ui->actionDownload->setEnabled(!busy);
    ui->action_Settings->setEnabled(!busy);
    //ui->actionEdit_Part_Number->setEnabled(!busy);
    EnableEdit_Part_Number(!busy);
    ui->actionEdit_Customer_Number_First_Firmware_Revision->setEnabled(!busy);
    ui->actionBlank_Check->setEnabled(!busy && !writeConfig);
    ui->actionReset_Device->setEnabled(!busy);
    ui->actionGet_Msg->setEnabled(!busy);
    ui->actionRead_Memory->setEnabled(!busy);
    ui->actionWrite_Memory->setEnabled(!busy);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::displayMsgFromProduct(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

void MainWindow::IoWithDeviceStart(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
    setBootloadBusy(true);
}

//Useful for adding lines of text to the main window from other threads.
void MainWindow::AppendStringToTextbox(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

void MainWindow::UpdateProgressBar(int newValue)
{
    ui->progressBar->setValue(newValue);
}



void MainWindow::IoWithDeviceComplete(QString msg, Comm::ErrorCode result, double time)
{
    QTextStream ss(&msg);

    switch(result)
    {
        case Comm::Success:
            ss << " Complete (" << time << "s)\n";
            setBootloadBusy(false);
            busy = false;
            break;
        case Comm::NotConnected:
            ss << " Failed. Device not connected.\n";
            setBootloadBusy(false);
            busy = false;
            break;
        case Comm::Fail:
            ss << " Failed.\n";
            setBootloadBusy(false);
            busy = false;
            break;
        case Comm::IncorrectCommand:
            ss << " Failed. Unable to communicate with device.\n";
            setBootloadBusy(false);
            busy = false;
            break;
        case Comm::Timeout:
            ss << " Timed out waiting for device (" << time << "s)\n";
            setBootloadBusy(false);
            busy = false;
            break;
        default:
            break;
    }

    ui->plainTextEdit->appendPlainText(msg);
}

void MainWindow::on_action_Verify_Device_triggered()
{
    //future = QtConcurrent::run(this, &MainWindow::VerifyDevice);
}



//This thread programs previously parsed .hex file data into the device's programmable memory regions.
void MainWindow::WriteDevice(void)
{
    Comm::ErrorCode result;
    QTime elapsed;

    elapsed.start();
    emit IoWithDeviceStarted("Writing Device...");
    result = product->writeDevice();
    emit IoWithDeviceCompleted("Writing", result, ((double)elapsed.elapsed()) / 1000);
}


//Gets called when the user clicks to program button in the GUI.
void MainWindow::on_actionWrite_Device_triggered()
{
    busy = true;
    if(product->isConnectedApp() && !(trigrammeProduct=="SCP")&& !(trigrammeProduct=="SCC")){
        QMessageBox::warning(this,"Warning","You need to be in Bootloader mode.");
        busy = false;
        return;
    }
    ui->plainTextEdit->appendPlainText("Starting Erase/Program/Verify Sequence.");
    ui->plainTextEdit->appendPlainText("Do not unplug device or disconnect power until the operation is fully complete.");
    ui->plainTextEdit->appendPlainText(" ");

    if(product->isConnectedApp())
    {
        product->resetDevice();
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(WriteDevice()));
        timer->setSingleShot(true);
        timer->start(5000);
    }
    else
    {
        product->writeDevice();
    }
}

void MainWindow::on_actionErase_Device_triggered()
{
    future = QtConcurrent::run(this, &MainWindow::EraseDevice);
}

void MainWindow::EraseDevice(void)
{
    Comm::ErrorCode result;
    QTime elapsed;

    elapsed.start();
    emit IoWithDeviceStarted("Erasing Device... (no status update until complete, may take several seconds)");
    result = product->eraseDevice();
    emit IoWithDeviceCompleted("Erase", result, ((double)elapsed.elapsed()) / 1000);
}

//Executes when the user clicks the open hex file button on the main form.
void MainWindow::on_actionOpen_triggered()
{
    QString msg, newFileName;
    QTextStream stream(&msg);

    //Create an open file dialog box, so the user can select a .hex file.
    newFileName =
        QFileDialog::getOpenFileName(this, "Open Hex File", fileName, "Hex Files (*.hex *.ehx)");

    if(newFileName.isEmpty())
    {
        return;
    }

    firmwareDownloaded = false;
    LoadFile(newFileName);
}

void MainWindow::LoadFile(QString newFileName)
{
    QString msg;
    QTextStream stream(&msg);
    int result;

    qWarning("Loading file : " + newFileName.toAscii());

    result = product->LoadFile(newFileName);
    if(!result)
    {
        QApplication::restoreOverrideCursor();
        stream << "Failed to import: " << result << "\n";
        ui->plainTextEdit->appendPlainText(msg);
        return;
    }

    fileName = newFileName;
    if(firmwareDownloaded == false){
        QSettings settings;
        settings.beginGroup("MainWindow");

        QStringList files = settings.value("recentFileList").toStringList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while(files.size() > MAX_RECENT_FILES)
        {
            files.removeLast();
        }
        settings.setValue("recentFileList", files);
        UpdateRecentFileList();
    }
    stream.setIntegerBase(10);

    msg.clear();

    QFileInfo fi(fileName);
    //QString name = fi.fileName();
    QString name = fi.absoluteFilePath();
    stream << "Opened: " << name;
    if(firmwareDownloaded == true)stream<< " (temporary local copy downloaded from Volfoni server)";
    stream<< "\n";
    ui->plainTextEdit->appendPlainText(msg);
    hexOpen = true;
    setBootloadEnabled(true);
    if(firmwareDownloaded == false)ui->FileNameTextEdit->setText(name);
    QApplication::restoreOverrideCursor();

    printSettings();

    return;

}

void MainWindow::openRecentFile(void)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        firmwareDownloaded = false;
        LoadFile(action->data().toString());
    }
}

void MainWindow::EnableEdit_Part_Number(bool enable)
{
    //Always enabled : USB Tool debug
    ui->actionEdit_Part_Number->setEnabled(enable);

/*    QString fileName = "partnum.xml";
#ifdef Q_WS_MAC
        fileName =_pathResource;
        fileName.append("partnum.xml");
#endif
    QFile *file = new QFile(fileName);

    if(file->exists())
    {
        ui->actionEdit_Part_Number->setEnabled(enable);
    }
    else
        ui->actionEdit_Part_Number->setEnabled(false);
*/
}

void MainWindow::GetMessages(void)
{
    Messages getMessages;

    QString fileName = "messages.xml";
#ifdef Q_WS_MAC
        fileName =_pathResource;
        fileName.append("messages.xml");
#endif
    QFile *file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Unable to open messages list");
        return;
    }

    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
        std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
        file->close();
    }
    file->close();

    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    QDomElement b = root.firstChildElement("code");

    // exemple de nom de version : IR_IR V01.17.01
    while(!b.isNull()){// balayage
        QDomElement c = b.firstChildElement("value");
        getMessages.code = c.text();
        c = b.firstChildElement("label");
        getMessages.text_value = c.text();
        //std::cout<<"Code = "<<getMessages.code.toStdString()<<"  description = "<<getMessages.text_value.toStdString()<<std::endl;
        listeMessages.append(getMessages);

        b = b.nextSiblingElement("code");
    }

    file->close();
}

void MainWindow::UpdateRecentFileList(void)
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    QStringList files;

    files = settings.value("recentFileList").toStringList();

    int recentFileCount = qMin(files.size(), MAX_RECENT_FILES);
    QString text;
    int i;
    int j=0;

    for(i = 0; i < recentFileCount; i++)
    {

        if(QFile::exists(QFileInfo(files[i]).absoluteFilePath())){
            text = tr("&%1 %2").arg(j + 1).arg(QFileInfo(files[i]).fileName());
            recentFiles[j]->setText(text);
            recentFiles[j]->setData(files[i]);
            recentFiles[j]->setVisible(product->isConnected());
            j++;
        }
    }

    for(; j < MAX_RECENT_FILES; j++)
    {
        recentFiles[j]->setVisible(false);
    }
}

void MainWindow::on_action_About_triggered()
{
    AboutBox *AboutDlg = new AboutBox(this);
    AboutDlg->show();
}

void MainWindow::GetQuery()
{
    product->getQuery();
    ReadFirmwareData();
}

void MainWindow::on_action_Settings_triggered()
{
    Comm::ErrorCode result;
    Settings* dlg = new Settings(this,product->isConnectedApp());

    dlg->enableEepromBox(device->hasEeprom());

    dlg->setWriteFlash(writeFlash);
    dlg->setWriteConfig(writeConfig);
    dlg->setWriteEeprom(writeEeprom);
    dlg->setActivatedDebug(ActivatedDebug);


    if(dlg->exec() == QDialog::Accepted)
    {
        writeFlash = dlg->writeFlash;
        writeEeprom = dlg->writeEeprom;
        ActivatedDebug = dlg->ActivatedDebug;

        if(!writeConfig && dlg->writeConfig)
        {
            ui->plainTextEdit->appendPlainText("Disabling Erase button to prevent accidental erasing of the configuration words without reprogramming them\n");
            writeConfig = true;
            hexOpen = false;
            result = product->lockUnlockConfig(false);
            if(result == Comm::Success)
            {
                ui->plainTextEdit->appendPlainText("Unlocked Configuration bits successfully\n");
                GetQuery();
            }
        }
        else if(writeConfig && !dlg->writeConfig)
        {
            writeConfig = false;
            hexOpen = false;
            result = product->lockUnlockConfig(true);
            if(result == Comm::Success)
            {
                ui->plainTextEdit->appendPlainText("Locked Configuration bits successfully\n");
                GetQuery();
            }
        }

        if(!(writeFlash || writeEeprom || writeConfig))
        {
            setBootloadEnabled(false);
            ui->action_Settings->setEnabled(true);
            ui->action_Manager->setEnabled(true);
            //ui->actionEdit_Part_Number->setEnabled(true);
            EnableEdit_Part_Number(true);
            ui->actionEdit_Customer_Number_First_Firmware_Revision->setEnabled(true);
        }
        else
        {
            setBootloadEnabled(true);
        }

        if(ActivatedDebug)
        {
            DebugTimer->start(100);
        }
        else
        {
            DebugTimer->stop();
        }

    }

    printSettings();

    delete dlg;
}

void MainWindow::setTrigramm(QString trigram){
    trigrammeProduct = trigram;
}

void MainWindow::setAcceptManager(bool valid){
    AcceptManager = valid;
}

void MainWindow::on_action_Manager_triggered()
{
    AcceptManager = true;
    if(trigrammeProduct==""){
        SelectDevice sd(this, this);
        sd.exec();
    }
    if((trigrammeProduct == "SCC" || trigrammeProduct == "SCP") && product->isConnected()){
        QMessageBox::warning(this,"Warning","You need to be in Applicative mode.");
        return;
    }

    if((trigrammeProduct == "AHU" || trigrammeProduct == "AME" || trigrammeProduct == "EDG") && product->isConnectedApp()){
        QMessageBox::warning(this,"Warning","You need to be in Bootloader mode.");
        return;
    }
    if(AcceptManager==true){
        QApplication::setOverrideCursor(Qt::BusyCursor);
        Comm::ErrorCode result;
        unsigned char readed[64];
        unsigned char readed2[64];
        Manager* dlg_mng = new Manager(this,trigrammeProduct,_pathResource);

        connect(dlg_mng, SIGNAL(Mng_apply_changes(unsigned char *)), this, SLOT(on_mng_apply_triggered(unsigned char *)));
        connect(dlg_mng, SIGNAL(Mng_sync_changes()), this, SLOT(on_mng_sync_changes()));
        connect(this, SIGNAL(send_data_for_manager(unsigned char*)), dlg_mng, SLOT(display_data(unsigned char*)));
        connect(this, SIGNAL(send_Channel_for_manager(unsigned char*)), dlg_mng, SLOT(display_channel(unsigned char*)));

        if(trigrammeProduct == "SCC" || trigrammeProduct == "SCP"){
            result = product->readParam(readed);
            if(result != Comm::Success)
                ui->plainTextEdit->appendPlainText("Reading parameters ERROR");
            if(readed[0]==255){// cas illisible, 0xFF
                readed[0] = 15;
                for(int i=1; i<=23; i++) readed[i] = 0 ;
            }
            emit send_data_for_manager(readed);
        }else{ // AHU et AME et EDG
            // readed = data 0x1011 0x1012 0x1013 0x1014, readed2 = data 0x1024 0x1025
            result = product->readParamFlash(readed, readed2);
            if(result != Comm::Success)
            {
                ui->plainTextEdit->appendPlainText("Reading parameters ERROR");
            }
            readed[5]=0x00;
            if(readed[0]==255){// cas illisible, 0xFF
                readed[0] = 15;
                readed[1] = 0 ;
                readed[2] = 0 ;
                readed[3] = 0 ;
                readed[4] = 0 ;
            }

            emit send_data_for_manager(readed);

            // channel frequency
            readed2[MEMORY_SIZE_MANAGER_RF]=0x00;
            if((trigrammeProduct == "EDG")&&(readed2[0]==0xFF)&&(readed2[1]==0xFF)){ // par defaut, la mémoire est vide dans la EDGE
                readed2[0] = 0x20;                                                   // et dans ce cas, on a juste le channel 8 !
                readed2[1] = 0x00;
            }

            emit send_Channel_for_manager(readed2);
        }

        dlg_mng->exec();

        delete dlg_mng;
    }else QMessageBox::warning(this,"Warning","No type of product selected, unable to open manager.");
    product->readFirmwareData();
}

void MainWindow::on_mng_apply_triggered(unsigned char *param)
{
    unsigned char readed[64], readed2[64];
    Comm::ErrorCode result;

    if(trigrammeProduct == "SCC" || trigrammeProduct == "SCP"){
        result = product->programParam(param);
        if(result != Comm::Success)
            ui->plainTextEdit->appendPlainText("Programming parameters ERROR");

        result = product->readParam(readed);
        if(result != Comm::Success)
            ui->plainTextEdit->appendPlainText("Reading parameters ERROR");
        ui->plainTextEdit->appendPlainText("Success to set new parameters");

        emit send_data_for_manager(readed);
    }else{
        // write data 0x1011 0x1012 0x1013 0x1014  puis  0x1024 0x1025
        result = product->programParamFlash(param);
        if(result != Comm::Success)
            ui->plainTextEdit->appendPlainText("Programming parameters ERROR");

        result = product->readParamFlash(readed, readed2);
        if(result != Comm::Success)
            ui->plainTextEdit->appendPlainText("Reading parameters ERROR");
        ui->plainTextEdit->appendPlainText("Success to set new parameters");

        readed[5]=0x00;
        emit send_data_for_manager(readed);

        readed2[MEMORY_SIZE_MANAGER_RF]=0x00;
        if((trigrammeProduct == "EDG")&&(readed2[0]==0xFF)&&(readed2[1]==0xFF)){ // par defaut, la mémoire est vide dans la EDGE
            readed2[0] = 0x20;                                                   // et dans ce cas, on a juste le channel 8 !
            readed2[1] = 0x00;
        }

        ui->progressBar->setMaximum(100);
        ui->plainTextEdit->appendPlainText("Success to set new parameters");
        emit SetProgressBar(100);

        emit send_Channel_for_manager(readed2);
    }
}

void MainWindow::on_mng_sync_changes()
{
    Comm::ErrorCode result;

    if(trigrammeProduct == "SCC" || trigrammeProduct == "SCP"){
    result = product->inversionGD();
    if(result != Comm::Success)
        ui->plainTextEdit->appendPlainText("Inversion G/D ERROR");
    }
}

void MainWindow::on_actionReset_Device_triggered()
{
        product->resetDevice();
}

void MainWindow::on_actionSniffer_triggered()
{
    Sniffer *sniffer = new Sniffer(this);
    sniffer->show();
}

void MainWindow::on_actionGet_Msg_triggered()
{
    product->pollUSBApp();
    if(product->isConnectedApp())emit getDebugMsg();
    else{
        ui->checkBoxDebug->setEnabled(false);
        if(ui->checkBoxDebug->isChecked()==true)ui->checkBoxDebug->setChecked(false);
        ActivatedDebug = false;
        DebugTimer->stop();

        printSettings();
        ui->label_mode->setText("");
        ui->label_Connected->setPixmap(QPixmap(":/MainWindow/img/notConnected.png"));
        opened = false;
        qWarning("Closing device.");
        product->close();
        _msg = adaptTextQStatusBar("Disconnected");
        statusMsg = 1;
        ui->plainTextEdit->setPlainText("Device Detached.");
        ui->PartNumberTextEdit->setText("");
        ui->FirmwareRevTextEdit->setText("");
        ui->BootloaderRevTextEdit->setText("");
        ui->InitialFirmwareTextEdit->setText("");
        //hexOpen = false;
        setBootloadEnabled(false);
        emit SetProgressBar(0);
    }


}

void MainWindow::readedDebug(){

    ////// A REPRENDRE ENTIEREMENT
    return;
}

void MainWindow::readedDebug(unsigned char *dataDebug){

    if((int)dataDebug[0]!=58){
        return;
    }
    if(((dataDebug[2]^dataDebug[3])^dataDebug[4]) != dataDebug[57]){
        return;
    }
    int longueur = (int)dataDebug[1];
    QString str = "";
    int startIndex = 2;

    QDate date=QDate::currentDate();
    QString dateString = date.toString("(MM/dd/yyyy ");
    QTime time=QTime::currentTime();
    dateString.append(time.toString("hh:mm:ss) "));
    int safe = 0;
    while(startIndex!=(longueur+2) && safe < 21){
        if((int)dataDebug[startIndex]==99){ //cxx
            unsigned char tempo[64];
            QString tempoString = "";

            tempo[0]=dataDebug[startIndex];
            tempo[1]=dataDebug[startIndex+1];
            tempo[2]=dataDebug[startIndex+2];
            tempo[3]= 0x00;
            tempoString = (const char*)&tempo[0];
            for(int i=0;i<listeMessages.size();i++){
                if(tempoString.contains(listeMessages[i].code)){
                    tempoString.replace(listeMessages[i].code, listeMessages[i].text_value);
                    break;
                }
            }
            str.append(dateString);
            str.append(tempoString);
            str.append("\n");

            startIndex+=3;
        }
        else if((int)dataDebug[startIndex]==112 ||
                (int)dataDebug[startIndex]==108 ||
                (int)dataDebug[startIndex]==114){// p, l ou r
            unsigned char tempo[64];
            QString tempoString = "";

            tempo[0]=dataDebug[startIndex];
            tempo[1]= 0x00;
            int a = (int)dataDebug[startIndex+1];
            int b = (int)dataDebug[startIndex+2];
            double total = a*256+b;
            if((int)dataDebug[startIndex]==112)total = 1000000/((total/3)*2);

            tempoString = (const char*)&tempo[0];
            if(tempoString.contains("p"))tempoString = "signal period";
            else if(tempoString.contains("l"))tempoString = "lux";
            else if(tempoString.contains("r"))tempoString = "RSSI";
            str.append(dateString);
            str.append(tempoString);
            str.append(" = ");
            str.append(QString::number(total));
            if((int)dataDebug[startIndex]==112)str.append(" Hz");
            str.append("\n");

            startIndex+=3;
        }
        else if((int)dataDebug[startIndex]==102){ //f
            unsigned char tempo[64];
            QString tempoString = "";
            for(int i = 0; i<(longueur + 2 -startIndex);i++)tempo[i]=dataDebug[startIndex+i];
            tempo[longueur + 2 -startIndex]= 0x00;
            tempoString = (const char*)&tempo[0];
            str.append(dateString);
            str.append(tempoString);
            str.append("\n");

            startIndex+=longueur + 2 -startIndex;
        }
        else if((int)dataDebug[startIndex]==105){ //i
            int nTrame = dataDebug[startIndex+1];
            str.append(dateString);
            str.append("Trame ");
            str.append(QString::number(nTrame));
            str.append(" : ");

            for(int i = 0; i<25;i++){
                int a = (int)dataDebug[startIndex+2+2*i];
                int b = (int)dataDebug[startIndex+2+2*i+1];
                double total = a*256+b;
                total = ((total/3)*2);

                str.append(QString::number(total));
                if(i!=24)str.append(" | ");
            }
            str.append("\n");

            startIndex+=longueur + 2 -startIndex;
        }
        else {
            std::cout<<"rien "<<std::endl;
        }
        safe++;

    }

    if(str!="")emit AppendStringToTextbox(str);

}

void MainWindow::on_actionDownload_triggered()
{
    wait->start(5000);
    //url = tr("http://www.volfoni.com/VolfoniLoader/Serveur/versioning.xml");
    url = tr("http://vl.volfoni.com/Serveur/versioning.xml");

    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
#ifdef Q_WS_MAC
        fileName =_pathResource;
        fileName.append("versioning.xml");
#endif
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }

    // schedule the request
    httpRequestAborted = false;
    startRequest(url);

}

void MainWindow::on_actionGetFile_triggered()
{
    wait->start(5000);
    url = ui->FileNameTextEdit->toPlainText();

    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
#ifdef Q_WS_MAC
        fileName =_pathResource;
        fileName.append(fileInfo.fileName());
#endif
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Unable to download the file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }

    // schedule the request
    httpRequestAborted = false;
    startRequestFirmware(url);

}

void MainWindow::startRequest(QUrl url)
{
    qWarning("send get to http serveur");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void MainWindow::startRequestFirmware(QUrl url)
{
    qWarning("send get firmware to http serveur");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFirmwareFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void MainWindow::httpFirmwareFinished()
{
    wait->stop();
    QString NewFileName;

    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        return;
    }

    file->flush();
    file->close();

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed: %1.")
                                 .arg(reply->errorString()));
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequestFirmware(url);
            return;
        }
    }
    else
    {
        ui->plainTextEdit->appendPlainText(tr("Firmware downloaded from server."));
    }

    NewFileName = file->fileName();

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;

    LoadFile(NewFileName);

    QFile::remove(NewFileName);
}

void MainWindow::httpFinished()
{
    wait->stop();
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        return;
    }

    file->flush();
    file->close();

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed: %1.")
                                 .arg(reply->errorString()));
        return;
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url);
            return;
        }
    }
    else
    {
        ui->plainTextEdit->appendPlainText(tr("Informations downloaded from server."));
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;

    if(trigrammeProduct==""){
        SelectDevice sd(this, this);
        sd.exec();
    }
    busy = true;
    VersionWindow *filesdlg = new VersionWindow(this, trigrammeProduct, FirmwareRev, customerNumber, _pathResource);
    filesdlg->exec();
    product->readFirmwareData();
    busy = false;

}

void MainWindow::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

void MainWindow::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;

    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}

void MainWindow::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
{
    authenticator->setUser("VolfoniLoader");
    authenticator->setPassword("vol123");
}

void MainWindow::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    if (QMessageBox::warning(this, tr("HTTP"),
                             tr("One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        reply->ignoreSslErrors();
    }
}

void MainWindow::Set_version_p(QString version){
    //url = "http://www.volfoni.com/VolfoniLoader/Serveur/"+ version;
    url = "http://vl.volfoni.com/Serveur/"+ version;
    ui->FileNameTextEdit->setText(url.toString());

    QStringList list = QString(version).split("/");
    QString localCopy = QString("%1").arg(list.at(list.size()-1));
#ifdef Q_WS_MAC
        localCopy =_pathResource;
        localCopy.append(list.at(list.size()-1));
#endif

    firmwareDownloaded = true;
    LoadFile(localCopy);// evite l'étape de cliquer sur télécharger
    QFile::remove(localCopy);
}

void MainWindow::closeEvent(QCloseEvent *event){

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
#ifdef Q_WS_MAC
    fileName =_pathResource;
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

void MainWindow::StopWait(){ //si pas de reponse de téléchargement

  wait->stop();
  QApplication::restoreOverrideCursor();
  QMessageBox::critical(this,"Reading time exceeded","Unable to download file. \nTry it again later.\n\n");
  ui->plainTextEdit->appendPlainText(tr("Error when downloading versioning file."));

}


void MainWindow::on_actionEdit_Part_Number_triggered()
{
    if((trigrammeProduct == "AME" || trigrammeProduct == "EDG" || trigrammeProduct == "AHU" || trigrammeProduct == "SCC" || trigrammeProduct == "SCP") && product->isConnectedApp()){
        QMessageBox::warning(this,"Warning","You need to be in Bootloader mode.");
        return;
    }
    PartNumberBox *partnum = new PartNumberBox(this, this, SerialNumber, trigrammeProduct);
    partnum->show();
}

void MainWindow::on_actionRead_Memory_triggered()
{
    if((trigrammeProduct == "AME" || trigrammeProduct == "EDG"  || trigrammeProduct == "AHU" || trigrammeProduct == "SCC" || trigrammeProduct == "SCP") && product->isConnectedApp()){
        QMessageBox::warning(this,"Warning","You need to be in Bootloader mode.");
        return;
    }
    ReadMemory();
}

void MainWindow::on_actionWrite_Memory_triggered()
{
    if((trigrammeProduct == "AME" || trigrammeProduct == "EDG"  || trigrammeProduct == "AHU" || trigrammeProduct == "SCC" || trigrammeProduct == "SCP") && product->isConnectedApp()){
        QMessageBox::warning(this,"Warning","You need to be in Bootloader mode.");
        return;
    }
    WriteMemory();
}

void MainWindow::changePartNumber(QString newOne){

    Comm::ErrorCode result;

    emit SetProgressBar(3);

    result = product->changePartNumber(newOne);

    if(result != Comm::Success) return;

    if( (trigrammeProduct=="SCP") || (trigrammeProduct=="SCC") )
        product->resetDevice();
    else
        product->readFirmwareData();

    ui->progressBar->setMaximum(100);
    emit SetProgressBar(100);
    return;
}

void MainWindow::on_actionEdit_Customer_Number_First_Firmware_Revision_triggered()
{
    if((trigrammeProduct == "AHU" || trigrammeProduct == "SCC" || trigrammeProduct == "SCP") && product->isConnectedApp()){
        QMessageBox::warning(this,"Warning","You need to be in Bootloader mode.");
        return;
    }
    CustomerNumberBox *custnum = new CustomerNumberBox(this, this, customerNumber, trigrammeProduct, FirmwareRev, customerNumberRead);
    custnum->show();

}

void MainWindow::changeCustomerNumber(QString oldOne, QString newOne){

    Comm::ErrorCode result;

    result = product->changeCustomerNumber(newOne);

    if( (trigrammeProduct=="SCP") || (trigrammeProduct=="SCC") )
        product->resetDevice();
    else
        product->readFirmwareData();

    ui->progressBar->setMaximum(100);
    emit SetProgressBar(100);
    return;
}

void MainWindow::on_checkBoxDebug_toggled(bool checked)
{
    Sniffer *sniffer;
    ActivatedDebug = checked;

    if(ActivatedDebug)
    {
        DebugTimer->start(100);
        //Ouverture de la fenetre sniffer
        sniffer = new Sniffer(this);
        connect(product, SIGNAL(newMsgRFAvailable(unsigned char*)), sniffer, SLOT(displayData(unsigned char*)));
        connect(sniffer, SIGNAL(sendRFData(int,unsigned char*)), product, SLOT(setDebugMsg(int,unsigned char*)));
        sniffer->show();
    }
    else
    {
        DebugTimer->stop();
    }

    printSettings();

}

void MainWindow::ReadMemory()
{
    QTime elapsed;
    Comm::ErrorCode result;

    elapsed.start();
    emit IoWithDeviceStarted("Dumping Device's Program Memory...");

    result = product->readMemory();

    if(result != Comm::Success)
    {
        QMessageBox::about(this,"Error","Error during transfer, operation canceled.");
        emit IoWithDeviceCompleted("Dumping", result, ((double)elapsed.elapsed()) / 1000);
        ui->progressBar->setMaximum(100);
        emit SetProgressBar(100);   //Set progress bar to 100%
        return;
    }

    emit IoWithDeviceCompleted("Dumping", result, ((double)elapsed.elapsed()) / 1000);

    ui->progressBar->setMaximum(100);
    emit SetProgressBar(100);   //Set progress bar to 100%
}

//Get the device firmware revision
void MainWindow::ReadFirmwareData()
{
    Comm::ErrorCode result;
    QTime elapsed;

    elapsed.start();
    emit IoWithDeviceStarted("Checking Device's Memory...");

    //result = product->ReadFirmwareData(); // : readfirmwaredata() est fait a la fin du product->getquery

    FirmwareRev = product->getProductFirmwareRev();
    ui->FirmwareRevTextEdit->setText("<center>"+FirmwareRev+"</center>");

    BootloaderRev = product->getProductBootloaderRev();
    ui->BootloaderRevTextEdit->setText("<center>"+BootloaderRev+"</center>");

    QString tempo = product->getProductCustomerNumber();
    customerNumberRead = tempo;
    ui->InitialFirmwareTextEdit->setText("<center>"+tempo+"</center>");

    if(tempo.contains(".")==true){
        //ex de nom = IR_RF_V01.18.0088
        QStringList list = QString(tempo).split(".");//découpe le nom de la version selon les "."

        customerNumber = list.at(0);

    }else {
        if(FirmwareRev.contains(".")){// lecture possible
            QStringList list = QString(FirmwareRev).split(".");//découpe le nom de la version selon les "."
            customerNumber = list.at(0);
        }else  {
            customerNumber = "";
        }
    }

    SerialNumber = product->getProductSerialNumber();
    ui->PartNumberTextEdit->setText("<center>"+SerialNumber+"</center>");
    if(SerialNumber.startsWith("AME")) {
        trigrammeProduct = "AME";
        _msg = adaptTextQStatusBar("Activ Me connected");
        statusMsg = 2;
    }
    else if(SerialNumber.startsWith("EDG")) {
        trigrammeProduct = "EDG";
        _msg = adaptTextQStatusBar("EDGE 3D Glasses connected");
        statusMsg = 8;
    }
    else if(SerialNumber.startsWith("AHU")) {
        trigrammeProduct = "AHU";
        _msg = adaptTextQStatusBar("Activ Hub connected");
        statusMsg = 3;
    }
    else if(SerialNumber.startsWith("SCC")) {
        trigrammeProduct = "SCC";
        _msg = adaptTextQStatusBar("Smart crystal cinema connected");
        statusMsg = 4;
    }
    else if(SerialNumber.startsWith("SCP")) {
        trigrammeProduct = "SCP";
        _msg = adaptTextQStatusBar("Smart crystal pro connected");
        statusMsg = 5;
    }
    else  {
        trigrammeProduct = "";
        _msg = adaptTextQStatusBar("Unknown device connected");
        statusMsg = 6;
        if(busy == false){
            SelectDevice sd(this, this);
            sd.exec();
            if(trigrammeProduct=="AME") {
                _msg = adaptTextQStatusBar("Activ Me connected");
                statusMsg = 2;
            }
            if(trigrammeProduct=="EDG") {
                _msg = adaptTextQStatusBar("EDGE 3D Glasses connected");
                statusMsg = 8;
            }
            else if(trigrammeProduct=="AHU") {
                _msg = adaptTextQStatusBar("Activ Hub connected");
                statusMsg = 3;
            }
            else if(trigrammeProduct=="SCC") {
                _msg = adaptTextQStatusBar("Smart crystal cinema connected");
                statusMsg = 4;
            }
            else if(trigrammeProduct=="SCP") {
                _msg = adaptTextQStatusBar("Smart crystal pro connected");
                statusMsg = 5;
            }
        }
    }

    UpdateRecentFileList();

    emit IoWithDeviceCompleted("Checking", result, ((double)elapsed.elapsed()) / 1000);

    ui->progressBar->setMaximum(100);
    emit SetProgressBar(100);   //Set progress bar to 100%
    return;
}

void MainWindow::WriteMemory()
{
    Comm::ErrorCode result;

    QTime elapsed;

    elapsed.start();
    emit IoWithDeviceStarted("Writing Device's Program Memory...");

    result = product->writeMemory(SerialNumber);

    if(result != Comm::Success)
    {
        QMessageBox::about(this,"Error","Error during transfer, operation canceled.");
        emit IoWithDeviceCompleted("Writing", result, ((double)elapsed.elapsed()) / 1000);
        ui->progressBar->setMaximum(100);
        emit SetProgressBar(100);   //Set progress bar to 100%
        return;
    }

    emit IoWithDeviceCompleted("Writing", result, ((double)elapsed.elapsed()) / 1000);

    product->readFirmwareData();

    ui->progressBar->setMaximum(100);
    emit SetProgressBar(100);   //Set progress bar to 100%
}
