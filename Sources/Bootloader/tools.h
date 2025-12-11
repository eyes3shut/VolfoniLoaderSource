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

#include "MainWindow.h"
class SelectDevice : public QDialog
 {
     Q_OBJECT

 public:
     SelectDevice(QWidget* parent, MainWindow *mW);

 private slots:
     void valid();

protected:
     void closeEvent(QCloseEvent *event);

 private:
     QComboBox *products;
     MainWindow *_mW;
     bool closeAccepted;

 };

#endif // TOOLS_H
