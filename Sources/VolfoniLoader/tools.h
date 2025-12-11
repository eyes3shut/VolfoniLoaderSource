#ifndef TOOLS_H
#define TOOLS_H

#include <QtGui>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMenu>
#include <iostream>
#include <QtXml>
#include <QStatusBar>
#include <QComboBox>

#include "mainwindow.h"


class HelpBootLoader : public QDialog
 {
     Q_OBJECT

 public:
     HelpBootLoader(QWidget* parent,QString trigram);

 private slots:


 private:
     QString _trigram;

 };
class SelectDevice : public QDialog
 {
     Q_OBJECT

 public:
     SelectDevice(QWidget* parent);

 private slots:
     void valid();


 private:
     QComboBox *products;

 };

class SelectLanguage : public QDialog
 {
     Q_OBJECT

 public:
     SelectLanguage(QWidget* parent, MainWindow *mW, QString filename);

 private slots:
     void valid();


 private:
     QComboBox *languages;
     MainWindow *_mW;
     QString oldLanguage;
     QTranslator *translatorFR;
     QTranslator *translatorEN;
     QTranslator *translatorSP;
     QTranslator *translatorGR;
     int nLang;
     QString _filename;

 };

class SelectLanguageInit : public QDialog
 {
     Q_OBJECT

 public:
     SelectLanguageInit(QString filename);

 private slots:
     void valid();

protected:
     void closeEvent(QCloseEvent *event);

 private:
     QComboBox *languages;
     bool closeAccepted;
     QString _filename;

 };

#endif // TOOLS_H
