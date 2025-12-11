#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMenu>
#include <iostream>
#include <QStatusBar>
#include <QComboBox>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>


class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void setProgress(int value);


private:

    QGroupBox *encrypt;
    QGridLayout *grid_encrypt;
    QLineEdit *folder_encrypt;
    QLineEdit *filenameFrom_encrypt;
    QLineEdit *filenameTo_encrypt;
    QLineEdit *key_encrypt;
    QVBoxLayout *layoutPrincipal;

    QGroupBox *decrypt;
    QGridLayout *grid_decrypt;
    QLineEdit *folder_decrypt;
    QLineEdit *filenameTo_decrypt;
    QLineEdit *filenameFrom_decrypt;
    QLineEdit *key_decrypt;

    QProgressBar *progressbar;

protected:


public slots:


private slots:
    void EncryptbrowseFileFrom();
    void EncryptbrowseFolder();
    void EncryptbrowseFileTo();
    void encryptFile();

    void DecryptbrowseFileFrom();
    void DecryptbrowseFolder();
    void DecryptbrowseFileTo();
    void decryptFile();


};

#endif // MAINWINDOW_H
