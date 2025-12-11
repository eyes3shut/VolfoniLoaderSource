#ifndef SendServer_H
#define SendServer_H

#include <QtGui>
#include <iostream>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QList>
#include <QtNetwork>
#include <QFtp>

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;

#include "mainwindow.h"
class MainWindow;

#include "progress.h"
class Progress;

class SendServer : public QDialog
{
    Q_OBJECT

public:
    SendServer(QWidget* parent, MainWindow *mW, QString rootFolder, QString m_url, QString m_login, QString m_pwd, QString trigrams[100], int nDevices);
    ~SendServer();

    void startUpload();
    void initialisation();


private:
  MainWindow *_mW;
  QString _rootFolder;
  QString _url;
  QString _login;
  QString _pwd;
  QString _trigrams[100];
  int _nDevices;

  bool closeAccepted;

  QString fileNameToSend[1000];
  QString shortfileNameToSend[1000];
  QString fileNameToUpload[1000];
  QString folderToUpload[1000];
  int nFound;
  int currentN;
  int space;
  bool errorOccured;

  QFtp *_ftp;
  QFile *_file[1000];
  bool fileOpened[1000];
  QFile *file;
  QString foldertoCheck;

  Progress *progress;

  QStringList _list;

  int nUploaded;

protected:

public slots:

private slots:

  void ftpCommandFinished(int  , bool );
  void listinfo(const QUrlInfo &);


};

#endif // SendServer_H
