#include <QTextStream>

#include "CustomerNumber.h"
#include "ui_customernumberbox.h"

CustomerNumberBox::CustomerNumberBox(MainWindow *mW, QWidget *parent, QString oldCustomerNumber, QString trigram, QString firmwareRev, QString readCustomerNumber) :
    QDialog(parent,Qt::WindowCloseButtonHint),
    ui(new Ui::CustomerNumberBox),
    _mW(mW),
    _trigram(trigram),
    _firmwareRev(firmwareRev),
    _readCustomerNumber(readCustomerNumber)
{
    ui->setupUi(this);

    ui->lineEditOld->setText(readCustomerNumber);
    ui->lineEditOld->setEnabled(false);

    QStringList list = QString(_readCustomerNumber).split(".");
    if(list.size()==0)ui->lineEditNew->setText(_firmwareRev);
    else{QString readCustomer = list.at(0);
        if(oldCustomerNumber==readCustomer)ui->lineEditNew->setText(readCustomerNumber);
        else ui->lineEditNew->setText(_firmwareRev);
    }

    QObject::connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(apply()));
}

CustomerNumberBox::~CustomerNumberBox()
{
    delete ui;
}

void CustomerNumberBox::apply(){
    int n = ui->lineEditNew->text().count(".");
    if(ui->lineEditOld->text() == ui->lineEditNew->text()){
        QMessageBox::warning(this,"Error", "New customer number is the same than the previous.");
        return;
    }
    else if(ui->lineEditNew->text().size()<15){
        QMessageBox::warning(this,"Error", "New customer number is too short.");
        return;
    }
    else if(ui->lineEditNew->text().size()>15){
        QMessageBox::warning(this,"Error", "New customer number is too long.");
        return;
    }
    else if(ui->lineEditNew->text().contains("_V")== false || n!=2){
        QMessageBox::warning(this,"Error", "Syntax error for customer number.");
        return;
    }
    else{
        close();
        _mW->changeCustomerNumber(ui->lineEditOld->text(), ui->lineEditNew->text());
    }
}
