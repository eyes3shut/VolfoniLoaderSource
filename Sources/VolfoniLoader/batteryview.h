#ifndef BATTERYVIEW_H
#define BATTERYVIEW_H

#include <QtGui>
#include <QLabel>
#include <QProgressBar>
#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>

class BatteryView : public QDialog
{
    Q_OBJECT

public:
    BatteryView(QWidget *parent = 0);

private:
    QLabel *label0;
    QProgressBar *battBar0;
    QLabel *label1;
    QProgressBar *battBar1;
    QLabel *label2;
    QProgressBar *battBar2;
    QLabel *label3;
    QProgressBar *battBar3;
    QLabel *label4;
    QProgressBar *battBar4;
    QLabel *label5;
    QProgressBar *battBar5;
    QLabel *label6;
    QProgressBar *battBar6;
    QLabel *label7;
    QProgressBar *battBar7;
    QLabel *label8;
    QProgressBar *battBar8;
    QLabel *label9;
    QProgressBar *battBar9;
    QPushButton *refresh;
    QPushButton *quit;

signals:
    void refresh_clicked(bool);

public slots:
    void displayAll(int, QString, int);
    void on_refresh_trigerred();

};

#endif // BATTERYVIEW_H
