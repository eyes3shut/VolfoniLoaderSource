#ifndef LISTUPDATE_H
#define LISTUPDATE_H

#include <QWidget>
#include <iostream>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QList>
#include <QtNetwork>
#include <QtXml>
#include <QTimer>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollArea>
#include <QLineEdit>
#include <QApplication>



#include "mainwindow.h"

class QVBoxLayout;
class QHBoxLayout;
class QScrollArea;

class ListUpdate : public QDialog
{
    Q_OBJECT

public:
    ListUpdate(QWidget* parent,MainWindow *mW, QString currentVersion, QString trigrammeProduct, QString customerNumber, QString user, QString langfile);
    ~ListUpdate();

    void CheckUpdates();
    void startRequest(QUrl url);
    void GetVersions();
    void drawTableUpdate();



private:

    MainWindow *_mW;
    QString _currentVersion;
    QString _trigrammeProduct;
    QString _customerNumber;
    QString _user;
    QString _langfile;


    QPushButton *m_cancel;
    QPushButton *m_apply;
    QHBoxLayout *layout_buttons;

    QVBoxLayout *layout_principal;
    QScrollArea *Scroll;
    QWidget *areaScroll;

    QPushButton *m_browse;

    QStandardItemModel *model;
    QTableView *table;


    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;

    QString fileNameUpdate[200];
    QString nameUpdate[200];
    QString dateUpdate[200];
    QString changesUpdate[200];
    QString descriptionUpdate[200];
    int nAvailableUpdates;

    QLineEdit *selectedUpdate;
    QLabel *l_selectedUpdate;
    QHBoxLayout *layout_selectedUpdate;

    QLabel *l_currentUpdate;
    int indexSelected;


    QTimer *wait;




protected:

public slots:


private slots:

    void save();

    void httpFinished();
    void httpReadyRead();
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);

    void StopWait();
    void editFonction(QModelIndex);
    void enableApply(const QString &);
    void browseUpdate();


};

#endif // LISTUPDATE_H
