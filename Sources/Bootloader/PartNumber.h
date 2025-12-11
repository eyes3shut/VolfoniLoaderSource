#ifndef PARTNUMBER_H
#define PARTNUMBER_H

#include <QDialog>
#include "MainWindow.h"
#include <QDate>

namespace Ui {
    class PartNumberBox;
}

class PartNumberBox : public QDialog
{
    Q_OBJECT

public:
    explicit PartNumberBox(MainWindow *mW, QWidget *parent = 0, QString oldPartNumber = "", QString trigram ="");
    ~PartNumberBox();

private:

    Ui::PartNumberBox *ui;
    MainWindow *_mW;
    QString _trigram;

private slots:
    void apply();
    void setPartNumber();
};

#endif // PARTNUMBER_H
