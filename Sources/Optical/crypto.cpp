#include "crypto.h"
#ifdef Q_WS_WIN32
#include <qt_windows.h>
#endif

#define HASH_AVANT "azg*apçgcé"
#define HASH_APRES "z*ùrlcg:*a"

#define VOLFO_1 "volfONI"
#define VOLFO_2 "VOLfoni"

Crypto::Crypto(bool limited, QString filekey, QWidget *parent) :
    QDialog(parent),
    _limited(limited),
    _fileKey(filekey)
{
    setWindowTitle("Software Registration");

    b_ok = new QPushButton("Activate");
    b_cancel = new QPushButton("Cancel");
    if(!limited) b_generate = new QPushButton("Generate");

    labelRegistrationID = new QLabel("Registration ID :");
    leRegistrationID = new QLineEdit();
    if(limited) leRegistrationID->setReadOnly(true);
    leRegistrationID->setFixedWidth(400);
    labelActivationKey = new QLabel("Activation Key :");
    leActivationKey = new QLineEdit();
    leRegistrationID->setFixedWidth(400);
    QLabel *labelBlaBla, *label2;
    labelBlaBla = new QLabel("<b>You are using an unregistered software</b>");
    label2 = new QLabel("To activate your product, enter your Activation Key and click the Activate Button.\n");

    QGridLayout *layout = new QGridLayout;
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelBlaBla, 0, 0, 1, 4);
    layout->addWidget(label2, 1, 0, 1, 4);
    layout->addWidget(labelRegistrationID, 2, 0);
    layout->addWidget(leRegistrationID, 2, 1, 1, 3);
    layout->addWidget(labelActivationKey, 3, 0);
    layout->addWidget(leActivationKey, 3, 1, 1, 3);
    layout->addWidget(b_ok, 4, 2);
    layout->addWidget(b_cancel, 4, 3);
    if(!limited) layout->addWidget(b_generate, 4, 1);

    setLayout(layout);

    connect(b_cancel, SIGNAL(clicked()), this, SLOT(accept()));
    connect(b_ok, SIGNAL(clicked()), this, SLOT(activate()));
    if(!limited) connect(b_generate, SIGNAL(clicked()), this, SLOT(generate()));

    if(limited) {
        hashAll();
        leRegistrationID->setText(m_registrationID);
    }
}

void Crypto::hashAll(){
#ifdef Q_WS_WIN32
    QString tdrive = "C:\\";
    DWORD dwSerialNumber = 0;
    bool ret;
    ret = GetVolumeInformation((WCHAR *)tdrive.utf16(),NULL,NULL,&dwSerialNumber,NULL,NULL,NULL,NULL);
    if (!ret) {
        QMessageBox::critical(this, "Error Activation", "Please contact your supplier");
    }
    QString hddSerial = QString::number(dwSerialNumber);
#endif
#ifdef Q_WS_MAC
    QProcess proc;
    QStringList args;
    args << "-c" << "ioreg -rd1 -c IOPlatformExpertDevice |  awk '/IOPlatformUUID/ { print $3; }'";
    proc.start( "/bin/bash", args );
    proc.waitForFinished();
    QString hddSerial = proc.readAll();
#endif

    QByteArray ba = QCryptographicHash::hash(QByteArray(QCryptographicHash::hash(QString(HASH_AVANT).toUtf8(), QCryptographicHash::Md5) +
                                                        QCryptographicHash::hash(hddSerial.toUtf8(), QCryptographicHash::Md5) +
                                                        QCryptographicHash::hash(QString(HASH_APRES).toUtf8(), QCryptographicHash::Md5)), QCryptographicHash::Sha1); // on la hache

    m_registrationID = ba.toHex(); // on convertit le hash en QString

    m_activationKey = hashRegistrationID(m_registrationID);
}

QString Crypto::hashRegistrationID(QString ID) {
    QByteArray ba = QCryptographicHash::hash(QByteArray(QCryptographicHash::hash(QString(VOLFO_1).toUtf8(), QCryptographicHash::Md5) +
                                                       QCryptographicHash::hash(ID.toUtf8(), QCryptographicHash::Md5) +
                                                       QCryptographicHash::hash(QString(VOLFO_2).toUtf8(), QCryptographicHash::Md5)), QCryptographicHash::Sha1); // on la hache

     return ba.toHex(); // on convertit le hash en QString
}

bool Crypto::sameHashInFile(){
    QString fileName = QString("key.txt");
    QString inFile;

    this->hashAll();

#ifdef Q_WS_MAC
    QStringList list = QString(_fileKey).split("/");
    fileName ="";
    for(int i = 0; i<list.size()-1;i++)fileName.append(list.at(i)+"/");
    fileName.append("key.txt");
#endif

    if(QFile::exists(fileName)==false){
        return false;

    }else{
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning("Unable to open registration file");
            return false;
        }

        char* pdata = NULL;
        pdata = (char*)malloc(40);

        QTextStream flux(&file);
        flux >> inFile;
        file.close();
    }

    int varTemp = QString::compare(inFile, m_activationKey);
    if (varTemp == 0) return true;
    else return false;
}

void Crypto::activate(){
    if(QString::compare(leActivationKey->text(), m_activationKey) == 0) {
        QString fileName = QString("key.txt");
#ifdef Q_WS_MAC
    QStringList list = QString(_fileKey).split("/");
    fileName ="";
    for(int i = 0; i<list.size()-1;i++)fileName.append(list.at(i)+"/");
    fileName.append("key.txt");
#endif
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this, "Error Activation", "Please contact your supplier");
            return ;
        }

        QTextStream flux(&file);
        flux << m_activationKey;
        file.close();

        this->accept();
    }
    else QMessageBox::critical(this, "Error", "Invalid Activation Key");
}

void Crypto::generate(){
    leActivationKey->setText(hashRegistrationID(leRegistrationID->text()));
}
