#ifndef TOOLS_H
#define TOOLS_H

#include <QtGui>
#include <QMenu>
#include <iostream>
#include <QStatusBar>
#include <QComboBox>
#include <QtXml>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QSignalMapper>
#include <QScrollArea>

#include "mainwindow.h"

#include "../Encrypter/decrypt.h"
#include "../Encrypter/encrypt.h"

class MainWindow;


class CRC32 : public QDialog
{
    Q_OBJECT

public:
    CRC32(QWidget *parent, MainWindow *mW, QString pathResource);

private:
    MainWindow *_mW;
    QString getCRC32(QString filePath);

    QLineEdit *CRC32File;
    QLineEdit *CRC32Value;
    QVBoxLayout *layout_principal;

    QString KEY_Decrypt;
    QString debutVersion;
    QString _pathResource;


protected:
    Decrypt *decrypt;
    Encrypt *encrypt;


public slots:


private slots:

    void apply();
    void browseFile();

};

class DecryptFunction : public QDialog
{
    Q_OBJECT

public:
    DecryptFunction(QWidget *parent, MainWindow *mW);

private:
    MainWindow *_mW;
    QGroupBox *decrypt;
    QGridLayout *grid_decrypt;
    QLineEdit *folder_decrypt;
    QLineEdit *filenameFrom_decrypt;
    QLineEdit *filenameTo_decrypt;
    QLineEdit *key_decrypt;

protected:


public slots:


private slots:
    void DecryptbrowseFileFrom();
    void DecryptbrowseFileTo();
    void decryptFile();

};

class EncryptFunction : public QDialog
{
    Q_OBJECT

public:
    EncryptFunction(QWidget *parent, MainWindow *mW);

private:
    MainWindow *_mW;
    QGroupBox *encrypt;
    QGridLayout *grid_encrypt;
    QLineEdit *folder_encrypt;
    QLineEdit *filenameFrom_encrypt;
    QLineEdit *filenameTo_encrypt;
    QLineEdit *key_encrypt;

protected:


public slots:


private slots:
    void EncryptbrowseFileFrom();
    void EncryptbrowseFileTo();
    void encryptFile();

};

#endif // TOOLS_H
