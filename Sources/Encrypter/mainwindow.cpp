#include <QtGui>
#include <QList>

#include "mainwindow.h"

#include "encrypt.h"
#include "decrypt.h"


MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    resize(800,500);
    setWindowTitle("Encrypter / Decrypter");
    setWindowIcon(QIcon(":/MainWindow/ressources/volfo160.ico"));
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    ////////////////////////////////////////////////////////////////////////
    // encryption

    QLabel *l_fileFrom_encrypt = new QLabel("Filename to encrypt:");
    filenameFrom_encrypt = new QLineEdit("");
    QPushButton *browseEncryptFilenameFrom =  new QPushButton("Browse");
    connect(browseEncryptFilenameFrom, SIGNAL(clicked()), this, SLOT(EncryptbrowseFileFrom()));

    QLabel *l_folder_encrypt = new QLabel("Folder:");
    folder_encrypt = new QLineEdit("");
    QPushButton *browseEncryptFilenameFolder =  new QPushButton("Browse");
    connect(browseEncryptFilenameFolder, SIGNAL(clicked()), this, SLOT(EncryptbrowseFolder()));

    QLabel *l_fileTo_encrypt = new QLabel("Encrypted filename:");
    filenameTo_encrypt = new QLineEdit("");
    QPushButton *browseEncryptFilenameTo =  new QPushButton("Browse");
    connect(browseEncryptFilenameTo, SIGNAL(clicked()), this, SLOT(EncryptbrowseFileTo()));

    QLabel *l_key_encrypt = new QLabel("Key to encrypt:");
    key_encrypt = new QLineEdit("");
    key_encrypt->setText("volfoni123");

    grid_encrypt = new QGridLayout();
    grid_encrypt->addWidget(l_fileFrom_encrypt,0,0);
    grid_encrypt->addWidget(filenameFrom_encrypt,0,1);
    grid_encrypt->addWidget(browseEncryptFilenameFrom,0,2);
    //grid_encrypt->addWidget(l_folder_encrypt,1,0);
    //grid_encrypt->addWidget(folder_encrypt,1,1);
    //grid_encrypt->addWidget(browseEncryptFilenameFolder,1,2);
    grid_encrypt->addWidget(l_fileTo_encrypt,2,0);
    grid_encrypt->addWidget(filenameTo_encrypt,2,1);
    grid_encrypt->addWidget(browseEncryptFilenameTo,2,2);
    grid_encrypt->addWidget(l_key_encrypt,3,0);
    grid_encrypt->addWidget(key_encrypt,3,1);

    QPushButton *validEncryption = new QPushButton("Encrypt");
    validEncryption->setFixedWidth(100);
    connect(validEncryption, SIGNAL(clicked()), this, SLOT(encryptFile()));

    QVBoxLayout *layoutEncryption = new QVBoxLayout();
    layoutEncryption->addLayout(grid_encrypt);
    layoutEncryption->addWidget(validEncryption,0,Qt::AlignRight);

    encrypt = new QGroupBox("Encryption");
    encrypt->setLayout(layoutEncryption);



    ////////////////////////////////////////////////////////////////
    // decryption
    QLabel *l_fileFrom_decrypt = new QLabel("Filename to decrypt:");
    filenameFrom_decrypt = new QLineEdit("");
    QPushButton *browseDecryptFilenameFrom =  new QPushButton("Browse");
    connect(browseDecryptFilenameFrom, SIGNAL(clicked()), this, SLOT(DecryptbrowseFileFrom()));

    QLabel *l_folder_decrypt = new QLabel("Folder:");
    folder_decrypt = new QLineEdit("");
    QPushButton *browseDecryptFilenameFolder =  new QPushButton("Browse");
    connect(browseDecryptFilenameFolder, SIGNAL(clicked()), this, SLOT(DecryptbrowseFolder()));

    QLabel *l_fileTo_decrypt = new QLabel("Decrypted filename:");
    filenameTo_decrypt = new QLineEdit("");
    QPushButton *browseDecryptFilenameTo =  new QPushButton("Browse");
    connect(browseDecryptFilenameTo, SIGNAL(clicked()), this, SLOT(DecryptbrowseFileTo()));

    QLabel *l_key_decrypt = new QLabel("Key to decrypt:");
    key_decrypt = new QLineEdit("");
    key_decrypt->setText("volfoni123");

    grid_decrypt = new QGridLayout();
    grid_decrypt->addWidget(l_fileFrom_decrypt,0,0);
    grid_decrypt->addWidget(filenameFrom_decrypt,0,1);
    grid_decrypt->addWidget(browseDecryptFilenameFrom,0,2);
    //grid_decrypt->addWidget(l_folder_decrypt,1,0);
    //grid_decrypt->addWidget(folder_decrypt,1,1);
    //grid_decrypt->addWidget(browseDecryptFilenameFolder,1,2);
    grid_decrypt->addWidget(l_fileTo_decrypt,2,0);
    grid_decrypt->addWidget(filenameTo_decrypt,2,1);
    grid_decrypt->addWidget(browseDecryptFilenameTo,2,2);
    grid_decrypt->addWidget(l_key_decrypt,3,0);
    grid_decrypt->addWidget(key_decrypt,3,1);

    QPushButton *validDecryption = new QPushButton("Decrypt");
    validDecryption->setFixedWidth(100);
    connect(validDecryption, SIGNAL(clicked()), this, SLOT(decryptFile()));


    QVBoxLayout *layoutDecryption = new QVBoxLayout();
    layoutDecryption->addLayout(grid_decrypt);
    layoutDecryption->addWidget(validDecryption,0,Qt::AlignRight);

    decrypt = new QGroupBox("Decryption");
    decrypt->setLayout(layoutDecryption);
    //////////////////////////////////////////////////////////////////////


    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::HLine | QFrame::Raised);
    frame->setLineWidth(5);
    frame->setMidLineWidth(10);

    progressbar = new QProgressBar();
    progressbar->setValue(0);

    layoutPrincipal = new QVBoxLayout();
    layoutPrincipal->addWidget(encrypt);
    layoutPrincipal->addSpacing(10);
    layoutPrincipal->addWidget(frame);
    layoutPrincipal->addSpacing(10);
    layoutPrincipal->addWidget(decrypt);
    layoutPrincipal->addSpacing(20);
    layoutPrincipal->addWidget(progressbar);

    setLayout(layoutPrincipal);


}

//////////////////////////////////////////////////////////////////////////////////////////
// Encryption
void MainWindow::EncryptbrowseFileFrom(){
  QString fileName = QFileDialog::getOpenFileName(this,tr("Choose a file to encrypt"),"./", tr("Document (*.hex)"));
  if(fileName.isEmpty()){
    //the user did not choose a  file
    return;
  }
  filenameFrom_encrypt->setText(fileName);

  QStringList list = QString(fileName).split("/");//découpe le chemin selon les "/"
  QString pathFolder = "";
  if(list.size()>0)for(int i = 0; i<list.size()-1;i++)pathFolder.append(list.at(i)+"/");
  if(folder_encrypt->text()=="")folder_encrypt->setText(pathFolder);
  filenameTo_encrypt->setText("encrypted_" +list.at(list.size()-1));
}

void MainWindow::EncryptbrowseFolder(){
    QString fileName = QFileDialog::getExistingDirectory(this,tr("Choose a folder"),"./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    fileName.replace("\\","/");
    fileName.append("/");
    folder_encrypt->setText(fileName);
}

void MainWindow::EncryptbrowseFileTo(){
    QString path = "./../../../";
    if(folder_encrypt->text()!="")path = folder_encrypt->text();

    QString fileName = QFileDialog::getSaveFileName(this,tr("Encrypted file"),path, tr("Documents (*.hex)") );
    if( !fileName.isNull() )
    {
        QStringList list = QString(fileName).split("/");//découpe le chemin selon les "/"
        QString pathFolder = "";
        if(list.size()>0)for(int i = 0; i<list.size()-1;i++)pathFolder.append(list.at(i)+"/");
        folder_encrypt->setText(pathFolder);
        filenameTo_encrypt->setText(list.at(list.size()-1));
    }
}

void MainWindow::encryptFile(){
    if(folder_encrypt->text()!="" && filenameTo_encrypt->text()!="" && key_encrypt->text()!=""){
        progressbar->setValue(0);
        // copy temporaire
        QString fileName = "";
        QStringList list = QString(filenameFrom_encrypt->text()).split("/");
        fileName = list.at(list.size()-1);


        if (QFile::exists(fileName))QFile::remove(fileName);
        progressbar->setValue(10);

        QFile::copy(filenameFrom_encrypt->text(),fileName);// pour harmoniser les methodes avec PC Loader avec un fichier temporaire

        progressbar->setValue(30);
        Encrypt encryptProcess;
        encryptProcess.encryption(filenameFrom_encrypt->text(), folder_encrypt->text()+filenameTo_encrypt->text(), key_encrypt->text());
        progressbar->setValue(90);

        QFile::remove(fileName);
        progressbar->setValue(100);

    }else QMessageBox::about(this,"Warning","Something is not set.");
}



///////////////////////////////////////////////////////////////////////////////////////////
// Decryption
void MainWindow::DecryptbrowseFileFrom(){
  QString fileName = QFileDialog::getOpenFileName(this,tr("Choose a file to decrypt"),"./", tr("Document (*.hex)"));
  if(fileName.isEmpty()){
    //the user did not choose a  file
    return;
  }
  filenameFrom_decrypt->setText(fileName);

  QStringList list = QString(fileName).split("/");//découpe le chemin selon les "/"
  QString pathFolder = "";
  if(list.size()>0)for(int i = 0; i<list.size()-1;i++)pathFolder.append(list.at(i)+"/");
  if(folder_decrypt->text()=="")folder_decrypt->setText(pathFolder);
  filenameTo_decrypt->setText("decrypted_" +list.at(list.size()-1));
}

void MainWindow::DecryptbrowseFolder(){
    QString fileName = QFileDialog::getExistingDirectory(this,tr("Choose a folder"),"./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    fileName.replace("\\","/");
    fileName.append("/");
    folder_decrypt->setText(fileName);
}

void MainWindow::DecryptbrowseFileTo(){
    QString path = "./../../../";
    if(folder_decrypt->text()!="")path = folder_decrypt->text();

    QString fileName = QFileDialog::getSaveFileName(this,tr("Encrypted file"),path, tr("Documents (*.hex)") );
    if( !fileName.isNull() )
    {
        QStringList list = QString(fileName).split("/");//découpe le chemin selon les "/"
        QString pathFolder = "";
        if(list.size()>0)for(int i = 0; i<list.size()-1;i++)pathFolder.append(list.at(i)+"/");
        folder_decrypt->setText(pathFolder);
        filenameTo_decrypt->setText(list.at(list.size()-1));
    }

}

void MainWindow::decryptFile(){
    if(folder_decrypt->text()!="" && filenameTo_decrypt->text()!="" && key_decrypt->text()!=""){
        progressbar->setValue(0);
        // copy temporaire
        QString fileName = "";
        QStringList list = QString(filenameFrom_decrypt->text()).split("/");
        fileName = list.at(list.size()-1);


        if (QFile::exists(fileName))QFile::remove(fileName);
        progressbar->setValue(10);

        QFile::copy(filenameFrom_decrypt->text(),fileName);// pour harmoniser les methodes avec PC Loader avec un fichier temporaire

        progressbar->setValue(30);
        Decrypt decryptProcess;
        decryptProcess.decryption(filenameFrom_decrypt->text(), folder_decrypt->text()+filenameTo_decrypt->text(), key_decrypt->text());
        progressbar->setValue(90);

        QFile::remove(fileName);
        progressbar->setValue(100);

    }else QMessageBox::about(this,"Warning","Something is not set.");
}


void MainWindow::setProgress(int value){
    progressbar->setValue(value);
}
