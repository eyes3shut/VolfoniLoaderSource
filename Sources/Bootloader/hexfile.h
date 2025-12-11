#ifndef HEXFILE_H
#define HEXFILE_H

#include <QObject>
#include <QtNetwork>
#include "../Bootloader/ImportExportHex.h"
#include "../Encrypter/decrypt.h"
#include "../Encrypter/encrypt.h"

class HexFile : public QObject
{
    Q_OBJECT

public:
    explicit HexFile();
    HexFile(Device *device);
    ~HexFile();

    void CheckUpdate(QString);
    int LoadFile(QString);
    DeviceData* getHexFileHexData();
    void httpFirmwareUpdate(QUrl);

protected:
    QUrl m_url;
    QFile *m_file;
    QFile *m_fileFirmware;
    QNetworkReply *m_reply;
    QNetworkAccessManager m_qnam;
    Decrypt *m_decrypt;
    Encrypt *m_encrypt;
    DeviceData* m_hexData;
    Device* m_device;

    bool m_httpRequestAborted;
    QString m_updateFilename;
    bool m_localFile;
    QString m_keyDecrypt;
    QString m_debutVersion;
    QString _fileLang;

    void startRequestFirmware(QUrl);
    void startRequest(QUrl);

signals:
    void httpTransferFinished();
    void httpFirmwareTransferFinished();

public slots:
    void httpReadyRead();
    int httpFinished();
    int httpFirmwareFinished();
    int FirmwareFinished();
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
    void sslErrors(QNetworkReply*,const QList<QSslError> &);

};

#endif // HEXFILE_H
