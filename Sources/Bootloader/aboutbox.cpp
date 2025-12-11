#include <QTextStream>

#include "aboutbox.h"
#include "ui_aboutbox.h"

#include "version.h"

AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent,Qt::WindowCloseButtonHint),
    ui(new Ui::AboutBox)
{
    ui->setupUi(this);

    setWindowTitle(QString("About ") + APPLICATION + QString(" v") + VERSION);

    /*QPixmap *AboutImage = new QPixmap(":/MainWindow/img/logoVolfoni.jpg");
    ui->AboutPicture->setPixmap(*AboutImage);*/

    QString msg;
    QTextStream stream(&msg);

    stream << APPLICATION << " v" << VERSION << "\n\n\n";
    stream << "This software is provided \"as is\" without\n";
    stream << "any warranty of any kind, whether express or \n";
    stream << "implied, including but not limited to, the\n";
    stream << "implied warranties of merchantability, fitness\n";
    stream << "for a particular purpose, or non-infringement";
    ui->AboutVersion->setText(msg);


    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
}

AboutBox::~AboutBox()
{
    delete ui;
}
