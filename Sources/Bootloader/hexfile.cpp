#include "hexfile.h"
#include <QtNetwork>
#include <QMessageBox>

HexFile::HexFile(Device *device)
{
    m_keyDecrypt = "volfoni123";
    m_debutVersion = ":102000";
    m_hexData = new DeviceData();
    m_device = device;
    m_encrypt = new Encrypt();
    m_decrypt = new Decrypt();

    connect(&m_qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(&m_qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
}

HexFile::~HexFile()
{
    delete m_hexData;
    delete m_encrypt;
    delete m_decrypt;
}

DeviceData* HexFile::getHexFileHexData()
{
    return m_hexData;
}

void HexFile::CheckUpdate(QString file){//Verify if a new version is available
    //m_url = tr("http://www.volfoni.com/VolfoniLoader/Serveur/versioning.xml");
    m_url = tr("http://vl.volfoni.com/Serveur/versioning.xml");
    _fileLang = file;

    QFileInfo fileInfo(m_url.path());
    QString fileName = fileInfo.fileName();

#ifdef Q_WS_MAC
        QStringList listM = QString(_fileLang).split("/");
        fileName ="";
        for(int i = 0; i<listM.size()-1;i++)fileName.append(listM.at(i)+"/");
        fileName.append("versioning.xml");
#endif
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    m_file = new QFile(fileName);
    if (!m_file->open(QIODevice::WriteOnly)) {
        qWarning("Unable to save the file versioning.xml");
        delete m_file;
        m_file = 0;
        return;
    }

    // schedule the request
    m_httpRequestAborted = false;

    startRequest(m_url);

}

void HexFile::startRequest(QUrl url)
{
    qWarning("send get to http serveur" + url.path().toLatin1());
    m_reply = m_qnam.get(QNetworkRequest(url));
    connect(m_reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(m_reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}


void HexFile::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (m_file)
        m_file->write(m_reply->readAll());
    else if (m_fileFirmware)
        m_fileFirmware->write(m_reply->readAll());
}

int HexFile::httpFinished()
{
    int result=0;

    if (m_httpRequestAborted) {
        if (m_file) {
            m_file->close();
            m_file->remove();
            delete m_file;
            m_file = 0;
        }
        m_reply->deleteLater();
        return 1;
    }

    m_file->flush();
    m_file->close();

    QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (m_reply->error()) {
        m_file->remove();
        qWarning("Download failed");
        result = 2;
    }
    else if (!redirectionTarget.isNull()) {
        QUrl newUrl = m_url.resolved(redirectionTarget.toUrl());
        m_url = newUrl;
        m_reply->deleteLater();
        m_file->open(QIODevice::WriteOnly);
        m_file->resize(0);
        startRequest(m_url);
        result = 3;
    }
    else
    {
        qWarning("Informations downloaded from server.");
    }

    m_reply->deleteLater();
    m_reply = 0;
    delete m_file;
    m_file = 0;

    emit httpTransferFinished();

    return result;
}

void HexFile::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
{
    authenticator->setUser("VolfoniLoader");
    authenticator->setPassword("vol123");
}

void HexFile::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }
    m_reply->ignoreSslErrors();
}

void HexFile::httpFirmwareUpdate(QUrl url){//Download firmware from internet

    QFileInfo fileInfo(url.path());
    QString fileHex = fileInfo.fileName();
    QString fileName = "";

#ifdef Q_WS_MAC
        QStringList listM = QString(_fileLang).split("/");
        for(int i = 0; i<listM.size()-1;i++)fileName.append(listM.at(i)+"/");
        fileName.append(fileHex);
#else
        fileName = fileHex;
#endif

    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    m_fileFirmware = new QFile(fileName);
    if (!m_fileFirmware->open(QIODevice::WriteOnly)) {
        qWarning("Unable to save the file hex");
        delete m_fileFirmware;
        m_fileFirmware = 0;
        return;
    }

    // schedule the request
    m_httpRequestAborted = false;
    startRequestFirmware(url);

}

void HexFile::startRequestFirmware(QUrl url)
{
    qWarning("send get firmware to http serveur");
    m_reply = m_qnam.get(QNetworkRequest(url));
    connect(m_reply, SIGNAL(finished()),
            this, SLOT(httpFirmwareFinished()));
    connect(m_reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

int HexFile::httpFirmwareFinished()
{
    int result = 0;

    if (m_httpRequestAborted) {
        if (m_fileFirmware) {
            m_fileFirmware->close();
            m_fileFirmware->remove();
            delete m_fileFirmware;
            m_fileFirmware = 0;
        }
        m_reply->deleteLater();
        return result;
    }

    m_fileFirmware->flush();
    m_fileFirmware->close();

    if (m_reply->error()) {
        m_fileFirmware->remove();
        qWarning("Download failed");
    }
     else
    {
        qWarning("Informations downloaded from server.");
    }

    m_reply->deleteLater();
    m_reply = 0;

    result = LoadFile(m_fileFirmware->fileName());

    m_fileFirmware->remove();
    delete m_fileFirmware;
    m_fileFirmware = 0;

    emit httpFirmwareTransferFinished();

    return result;
}

int HexFile::FirmwareFinished(){

    QString NewFileName = "";
    if(m_localFile == false){
#ifdef Q_WS_MAC
        QStringList listv = QString(_fileLang).split("/");
        NewFileName ="";
        for(int i = 0; i<listv.size()-1;i++)NewFileName.append(listv.at(i)+"/");
        NewFileName.append(m_updateFilename);
#else
        NewFileName = m_updateFilename;
#endif
    }
    else{
        QStringList list = QString(m_updateFilename).split("/");
#ifdef Q_WS_MAC
        QStringList listv = QString(_fileLang).split("/");
        NewFileName ="";
        for(int i = 0; i<listv.size()-1;i++)NewFileName.append(listv.at(i)+"/");
        NewFileName.append(list.at(list.size()-1));
#else
        NewFileName = list.at(list.size()-1);
#endif
    }

    qWarning("Firmware downloaded.");

    int success = LoadFile(NewFileName);

    if(success== 1){
#ifdef Q_WS_MAC
        QFile::remove(NewFileName);
#else
        QStringList list = QString(NewFileName).split("/");
        QFile::remove(list.at(list.size()-1));// supprime le fichier temporaire local
#endif

        return 1;
    }else{
        return 0;
    }
}

int HexFile::LoadFile(QString newFileName)
{
    ///////////////////////////////
    // decryptage si besoin      //
    ///////////////////////////////
    QString cryptedDebutVersion = m_encrypt->encryptLine(m_debutVersion,m_keyDecrypt);

    QFile fichier(newFileName);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    flux.setCodec("ISO 8859-1");
    QString ligne;
    while(! flux.atEnd())// si fichier crypté version simple
    {
        ligne = flux.readLine();
        //traitement de la ligne
        if(ligne.startsWith(m_debutVersion)){// cas fichier normal pas crypté
            break;
        }
        if(ligne.startsWith(cryptedDebutVersion)){// cas fichier crypté avec une clé
            m_decrypt->decryption(newFileName, newFileName, m_keyDecrypt);
            break;
        }
    }
    // fin du decryptage ou non.

    HexImporter import;
    HexImporter::ErrorCode result;

    result = import.ImportHexFile(newFileName, m_hexData, m_device);
    switch(result)
    {
    case HexImporter::Success:
        break;

    case HexImporter::CouldNotOpenFile:
        qWarning("Could not open file");
        return 0;

    case HexImporter::NoneInRange:
        qWarning("No address within range in file");
        return 0;

    case HexImporter::ErrorInHexFile:
        qWarning("Error in hex file");
        return 0;
    case HexImporter::InsufficientMemory:
        qWarning("Memory allocation failed");
        return 0;

    default:
        qWarning("Failed to import file");
        return 0;
    }

    qWarning("Opened hex file");

    return 1;
}
