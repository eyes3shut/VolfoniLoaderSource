#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

#include "../Bootloader/product.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

signals:
    void updateProgress(int);

private slots:
    void Connection();
    void changeSoftware();
    void changeSerial();
    void testClicked();

private:
    Product* product;

    QLabel *labelProdID;
    QLineEdit *editProdID;
    QPushButton *modifyProdID;
    QLabel *labelSerialBoard;
    QLineEdit *editSerialBoard;
    QPushButton *modifySerialBoard;
    QLabel *labelSerialSystem;
    QLineEdit *editSerialSystem;
    QPushButton *modifySerialSystem;
    QLabel *labelSWRelease;
    QLineEdit *editSWRelease;
    QPushButton *modifySWRelease;
    QLabel *labelBootLoader;
    QLineEdit *editBootLoader;
    QPushButton *modifyBootLoader;
    QLabel *labelTestResult;
    QPushButton *test;
    QProgressBar *progBar;
    QTimer *timerConnect;

    void programProduct();
    void displayData();
    void eraseDisplay();
    void enableAll();
    void disableAll();
    void changePartNumber(QString newOne);
};

#endif // MAINWINDOW_H
