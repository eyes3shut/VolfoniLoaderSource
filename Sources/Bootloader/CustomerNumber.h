#ifndef CUSTOMERNUMBER_H
#define CUSTOMERNUMBER_H

#include <QDialog>
#include "MainWindow.h"

namespace Ui {
    class CustomerNumberBox;
}

class CustomerNumberBox : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerNumberBox(MainWindow *mW, QWidget *parent = 0, QString oldCustomerNumber = "", QString trigram ="", QString firmwareRev = "", QString readCustomerNumber = "");
    ~CustomerNumberBox();

private:

    Ui::CustomerNumberBox *ui;
    MainWindow *_mW;
    QString _trigram;
    QString _firmwareRev;
    QString _readCustomerNumber;

private slots:
    void apply();
};

#endif // CUSTOMERNUMBER_H
