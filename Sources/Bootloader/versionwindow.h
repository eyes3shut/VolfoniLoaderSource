#ifndef VERSIONWINDOW_H
#define VERSIONWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QFile>
#include <QtXml>
#include <QtGui>
#include <iostream>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QList>
#include <QtNetwork>

class VersionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit VersionWindow(QWidget *parent = 0, QString trigram = "", QString firmware = "", QString customerNumber = "", QString pathResource ="");
    void startRequest(QUrl _url);
    QString changeWithPrevious(QString current, QString lastOne);
    QString getDescription(QString current);

public slots:
    void selectFile();
    void RemoveAndQuit();

private slots:
    void httpFinished();
    void httpReadyRead();
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);

    void StopWait();
    void showDiff(int a);

signals:
    void Set_version(QString version);

private:
    QLabel      *m_Label;
    QLabel      *m_LabelDiff;
    QPushButton *m_OK;
    QPushButton *m_CANCEL;
    QComboBox   *m_versionsCombo;
    QFile       *file;
    QFile       *fileFirmware;
    QString     _trigram;
    int nAvailableUpdates;
    QString fileNameUpdate[1000];
    QString urlUpdate[1000];
    QString _firmware;
    QString _customerNumber;
    QString _pathResource;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    int httpGetId;
    bool httpRequestAborted;
    bool errorOccured;

    QTimer *wait;

};

#endif // VERSIONWINDOW_H
