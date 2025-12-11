#ifndef GENERALTAB_H
#define GENERALTAB_H

#include <QtGui>
#include <QMenu>
#include <iostream>
#include <QStatusBar>
#include <QComboBox>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QList>
#include <QtNetwork>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QSignalMapper>
#include <QScrollArea>

#include "mainwindow.h"
class MainWindow;

#include "progress.h"
class Progress;

class GeneralTab : public QDialog
{
    Q_OBJECT

public:
    GeneralTab(QString trigram, MainWindow *mW, QString trigrams[100], int nDevices);

    QString getFilename();
    QString getFolder();

    void logUser(QString m_url, QString m_login, QString m_pwd);
    void CheckUpdates();
    void startRequest(QUrl url);
    void startRequest2(QUrl url);

    void DownloadVersions();
    void setCancelDownload();

private:


    QString _trigrams[100];
    int _nDevices;
    QLineEdit *versioningFile;
    QLineEdit *workingDirectory;

    QPushButton *create;
    QPushButton *m_browseFile;
    QPushButton *m_ftp;

    //////////////////////////////////////////

    QVBoxLayout *layout_principal;
    QString _trigram;
    MainWindow *_mW;
    Progress *progress;

    bool fieldsOK;

    QUrl url;
    QString _url;
    QString _login;
    QString _pwd;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;

    int httpGetId;
    bool httpRequestAborted;

    QString shortfileNameToDownload[1000];
    QString fileNameToDownload[1000];
    QString fileNameToStore[1000];
    QString folderToStore[1000];
    int nFound;
    int currentN;
    int space;
    bool errorOccured;

    bool projectOpen;

    void applyValues();
    int checkAlreadyOpen();



protected:


public slots:


private slots:
    void browseFolder();
    void browseFile();

    void ftpMode();

    void httpFinished();
    void httpFinished2();
    void httpReadyRead();
    void httpReadyRead2();
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);



};

#endif // GENERALTAB_H
