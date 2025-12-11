#include <QTextStream>

#include "PartNumber.h"
#include "ui_partnumberbox.h"

PartNumberBox::PartNumberBox(MainWindow *mW, QWidget *parent, QString oldPartNumber, QString trigram) :
    QDialog(parent,Qt::WindowCloseButtonHint),
    ui(new Ui::PartNumberBox),
    _mW(mW),
    _trigram(trigram)
{
    ui->setupUi(this);

    QDate date=QDate::currentDate();
    ui->lineEditYY->setText(date.toString("yy"));
    ui->lineEditWW->setText(QString::number(date.weekNumber()));
    ui->lineEditOld->setText(oldPartNumber);
    ui->lineEditOld->setEnabled(false);
    ui->lineEditNew->setText(oldPartNumber);
    QObject::connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(apply()));
    QObject::connect(ui->SetPN, SIGNAL(clicked()), this, SLOT(setPartNumber()));
}

PartNumberBox::~PartNumberBox()
{
    delete ui;
}

void PartNumberBox::setPartNumber(){
    if(ui->lineEditYY->text()=="0")ui->lineEditYY->setText("00");
    if(ui->lineEditYY->text()=="1")ui->lineEditYY->setText("01");
    if(ui->lineEditYY->text()=="2")ui->lineEditYY->setText("02");
    if(ui->lineEditYY->text()=="3")ui->lineEditYY->setText("03");
    if(ui->lineEditYY->text()=="4")ui->lineEditYY->setText("04");
    if(ui->lineEditYY->text()=="5")ui->lineEditYY->setText("05");
    if(ui->lineEditYY->text()=="6")ui->lineEditYY->setText("06");
    if(ui->lineEditYY->text()=="7")ui->lineEditYY->setText("07");
    if(ui->lineEditYY->text()=="8")ui->lineEditYY->setText("08");
    if(ui->lineEditYY->text()=="9")ui->lineEditYY->setText("09");

    if(ui->lineEditWW->text()=="0")ui->lineEditWW->setText("00");
    if(ui->lineEditWW->text()=="1")ui->lineEditWW->setText("01");
    if(ui->lineEditWW->text()=="2")ui->lineEditWW->setText("02");
    if(ui->lineEditWW->text()=="3")ui->lineEditWW->setText("03");
    if(ui->lineEditWW->text()=="4")ui->lineEditWW->setText("04");
    if(ui->lineEditWW->text()=="5")ui->lineEditWW->setText("05");
    if(ui->lineEditWW->text()=="6")ui->lineEditWW->setText("06");
    if(ui->lineEditWW->text()=="7")ui->lineEditWW->setText("07");
    if(ui->lineEditWW->text()=="8")ui->lineEditWW->setText("08");
    if(ui->lineEditWW->text()=="9")ui->lineEditWW->setText("09");

    if(ui->lineEditYY->text()!="" &&
            ui->lineEditYY->text().size()==2 &&
            ui->lineEditWW->text()!="" &&
            ui->lineEditWW->text().size()==2){
        QString text = "";
        text.append(_trigram);
        text.append("-1-");
        text.append(ui->lineEditYY->text());
        text.append("-");
        text.append(ui->lineEditWW->text());
        text.append("-0nnnn");
        ui->lineEditNew->setText(text);

    }else QMessageBox::warning(this,"Error", "Fields are not correct.");

}

void PartNumberBox::apply(){
    int n = ui->lineEditNew->text().count("-");
    if(ui->lineEditOld->text() == ui->lineEditNew->text()){
        QMessageBox::warning(this,"Error", "New serial number is the same than the previous.");
        return;
    }
    else if(ui->lineEditNew->text().size()<17){
        QMessageBox::warning(this,"Error", "New serial number is too short.");
        return;
    }
    else if(ui->lineEditNew->text().size()>17){
        QMessageBox::warning(this,"Error", "New serial number is too long.");
        return;
    }
    else if(n !=4){
        QMessageBox::warning(this,"Error", "Syntax error for serial number.");
        return;
    }
    else{
        close();
        _mW->changePartNumber(ui->lineEditNew->text());
    }
}
