#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <QtGui>
#include <QMenu>
#include <iostream>
#include <QStatusBar>
#include <QComboBox>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QDialog>
#include <QMessageBox>

#define MaxCharacterPerLine       2000


class Encrypt : public QDialog
{
    Q_OBJECT

public:
    Encrypt();

    QString encryptLine(QString ligne,QString key);

    void encryption(QString filenameFrom, QString filenameTo, QString key);


private:

    QChar C[200];
    int I[200];
    int nDiff;

    QChar ItoC(int i);
    int CtoI(QChar c);


protected:


public slots:


private slots:


};

#endif // ENCRYPT_H
