#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../Bootloader/PartNumber.h"

MainWindow::MainWindow()
{
    setWindowTitle("VOLFONI PROD ASSISTANT V00.01.00");

    QGridLayout *layoutFenetre = new QGridLayout;

    labelProdID = new QLabel("Device Prod ID");
    layoutFenetre->addWidget(labelProdID,0,0);
    editProdID = new QLineEdit();
    editProdID->setAlignment(Qt::AlignCenter);
    layoutFenetre->addWidget(editProdID,0,1);

    labelSerialBoard = new QLabel("Board S/N");
    layoutFenetre->addWidget(labelSerialBoard,1,0);
    editSerialBoard = new QLineEdit();
    editSerialBoard->setEnabled(false);
    editSerialBoard->setAlignment(Qt::AlignCenter);
    layoutFenetre->addWidget(editSerialBoard,1,1);
    modifySerialBoard = new QPushButton("Modify");
    layoutFenetre->addWidget(modifySerialBoard,1,2);

    labelSerialSystem = new QLabel("System S/N");
    layoutFenetre->addWidget(labelSerialSystem,2,0);
    editSerialSystem = new QLineEdit();
    editSerialSystem->setAlignment(Qt::AlignCenter);
    layoutFenetre->addWidget(editSerialSystem,2,1);

    labelBootLoader = new QLabel("BootLoader Rev.");
    layoutFenetre->addWidget(labelBootLoader,3,0);
    editBootLoader = new QLineEdit();
    editBootLoader->setEnabled(false);
    editBootLoader->setAlignment(Qt::AlignCenter);
    layoutFenetre->addWidget(editBootLoader,3,1);

    labelSWRelease = new QLabel("SW Release");
    layoutFenetre->addWidget(labelSWRelease,4,0);
    editSWRelease = new QLineEdit();
    editSWRelease->setEnabled(false);
    editSWRelease->setAlignment(Qt::AlignCenter);
    layoutFenetre->addWidget(editSWRelease,4,1);
    modifySWRelease = new QPushButton("Modify");
    layoutFenetre->addWidget(modifySWRelease,4,2);

    labelTestResult = new QLabel("<font color=\"#FF0000\">TEST NOT DONE</font>");
    labelTestResult->setFont(QFont("Comic Sans MS", 20));
    labelTestResult->setAlignment(Qt::AlignCenter);
    layoutFenetre->addWidget(labelTestResult,5,0,1,2);
    test = new QPushButton("Test !");
    test->setFont(QFont("Comic Sans MS", 16));
    test->setFixedSize(100,50);
    layoutFenetre->addWidget(test,5,2);

    progBar = new QProgressBar();
    progBar->setFixedWidth(400);
    progBar->setTextVisible(false);
    layoutFenetre->addWidget(progBar, 6,0,1,3, Qt::AlignCenter);

    setLayout(layoutFenetre);

    timerConnect = new QTimer();

    product = new Product();

    //CONNECTIONS
    connect(timerConnect, SIGNAL(timeout()), this, SLOT(Connection()));
    connect(modifySWRelease, SIGNAL(clicked()), this, SLOT(changeSoftware()));
    connect(product, SIGNAL(setProgress(int)), progBar, SLOT(setValue(int)));
    connect(modifySerialBoard, SIGNAL(clicked()), this, SLOT(changeSerial()));
    connect(test, SIGNAL(clicked()), this, SLOT(testClicked()));

    timerConnect->start(1000);
}

MainWindow::~MainWindow()
{
}


void MainWindow::programProduct()
{
//    unsigned char *param;
    int status = 0;

    //Connexion à l'AME
    qDebug("start the programming...");
    if( !product->isConnected() )
    {
        qDebug("product not connected, try to connect...");
        status = product->connection();
    }
    if(status!=9)
    {
        QMessageBox::critical(this, "Error", "Impossible to communicate with the ActivEyes.");
        return ;
    }

    //Mise à jour des variables microcontroleur
    qDebug("product connected, try to get informations...");
    product->getQuery();

//    free(param);
    product->close();
    delete(product);
}


void MainWindow::Connection(void)
{
    bool currStatus = product->isConnected();
    bool currStatusApp = product->isConnectedApp();

    product->pollUSB();
    product->pollUSBApp();

    if(currStatus != product->isConnected() || currStatusApp != product->isConnectedApp())
    {
        if(product->isConnected())
        {
            qWarning("Attempting to open device...");
            product->open();
            product->getQuery();
            displayData();
        }
        else if(product->isConnectedApp())
        {
            qWarning("Attempting to open device...");
            product->openApp();
            product->getQuery();
            displayData();
        }
        else
        {
            qWarning("Closing device.");
            product->close();
            eraseDisplay();
        }
    }
}


void MainWindow::displayData(){
    editSerialBoard->setText(product->getProductSerialNumber());
    if(product->isConnectedApp()) modifySerialBoard->setEnabled(false);
    else modifySerialBoard->setEnabled(true);
    editBootLoader->setText(product->getProductBootloaderRev());
    editSWRelease->setText(product->getProductFirmwareRev());
}


void MainWindow::eraseDisplay(){
    editSerialBoard->clear();
    editBootLoader->clear();
    editSWRelease->clear();
}

void MainWindow::changeSoftware()
{
    QString newFileName;
    int result;

    //Create an open file dialog box, so the user can select a .hex file.
    newFileName = QFileDialog::getOpenFileName(this, "Open Hex File", ".", "Hex Files (*.hex *.ehx)");

    if(newFileName.isEmpty())
    {
        return;
    }

    disableAll();

    qWarning("Loading file : " + newFileName.toAscii());

    result = product->LoadFile(newFileName);
    if(!result)
    {
        qWarning("Failed to import: " + result);
        return;
    }

    result = product->writeDevice();
    if(result)
    {
        qWarning("Failed to program device");
        return;
    }

    product->getQuery();
    displayData();

    enableAll();
}

void MainWindow::disableAll(){
    QApplication::setOverrideCursor(Qt::BusyCursor);
    labelProdID->setEnabled(false);
    editProdID->setEnabled(false);
    labelSerialBoard->setEnabled(false);
    modifySerialBoard->setEnabled(false);
    labelSerialSystem->setEnabled(false);
    editSerialSystem->setEnabled(false);
    labelBootLoader->setEnabled(false);
    labelSWRelease->setEnabled(false);
    modifySWRelease->setEnabled(false);
    labelTestResult->setEnabled(false);
    test->setEnabled(false);
}

void MainWindow::enableAll(){
    QApplication::restoreOverrideCursor();
    labelProdID->setEnabled(true);
    editProdID->setEnabled(true);
    labelSerialBoard->setEnabled(true);
    modifySerialBoard->setEnabled(true);
    labelSerialSystem->setEnabled(true);
    editSerialSystem->setEnabled(true);
    labelBootLoader->setEnabled(true);
    labelSWRelease->setEnabled(true);
    modifySWRelease->setEnabled(true);
    labelTestResult->setEnabled(true);
    test->setEnabled(true);
}

void MainWindow::changeSerial(){
    PartNumberBox *partnum = new PartNumberBox(this, this, product->getProductSerialNumber(), QString("SCC"));
    partnum->show();
}

void MainWindow::changePartNumber(QString newOne){

    product->changePartNumber(newOne);
    product->getQuery();
    displayData();
}

void MainWindow::testClicked(){

    //verification conditions de départ
    //présence device prod ID
    //présence carte
    if((!editProdID->text().length()) || (!product->isConnectedApp()))
    {
        QMessageBox::critical(this,"ERROR","Invalid Prod Device ID or SW Release");
        return;
    }

    //demande test led
    if ( QMessageBox::question(this,"Hardware tests","Hardware tests OK?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
        qDebug("YES");
    else qDebug("NO");

    //lancement test soft
//    product->launchTest();

    //recuperation resultats

    //Archivage
}
