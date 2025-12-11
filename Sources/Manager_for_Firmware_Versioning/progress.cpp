#include <QtGui>


#include "progress.h"



Progress::Progress(QWidget* parent, int caseProgress)
: QDialog(parent,Qt::WindowCloseButtonHint),
  _caseProgress(caseProgress)
{

    resize(450,50);
    this->setWindowIcon(QIcon(":/MainWindow/ressources/volfo160.ico"));
    this->setWindowTitle("Progress");

    progressbar = new QProgressBar();
    progressbar->setFixedWidth(400);

    label = new QLabel("");

    layout = new QVBoxLayout;
    layout->addWidget(progressbar,0,Qt::AlignCenter);
    layout->addWidget(label,0,Qt::AlignCenter);

    setLayout(layout);
    closeAccepted = false;

}

Progress::~Progress()
{

}

void Progress::setGT(GeneralTab *gT){
    _gT = gT;
    _gT->CheckUpdates();
}

void Progress::setMW(MainWindow *mW){
    _mW = mW;
}

void Progress::setSS(SendServer *sS){
    _sS = sS;
    _sS->initialisation();
}

void Progress::setValueProgress(int value){

    progressbar->setValue(value);

}

void Progress::setValueTitle(QString value){

    this->setWindowTitle(value);

}

void Progress::setValueLabel(QString value){

    label->setText(value);

}

void Progress::setClosable(bool accepted){

    closeAccepted = accepted;

}

void Progress::closeEvent(QCloseEvent *event){

    if(_caseProgress==0){
        if(closeAccepted == false){
            QMessageBox::StandardButton button =
                    QMessageBox::question(this,
                                          tr("Stop downloading"),
                                          tr("Are you sure to stop downloading?"),
                                          QMessageBox::Yes | QMessageBox::No);
            if (button == QMessageBox::Yes){
                event->accept();
                _gT->setCancelDownload();
            }else {
                if(closeAccepted == false)event->ignore();
                else event->accept();
            }

        }else event->accept();
    }
    if(_caseProgress==1){
        if(closeAccepted == false){

            QMessageBox::StandardButton button =
                    QMessageBox::question(this,
                                          tr("Stop uploading"),
                                          tr("Are you sure to stop uploading?\nMaybe Volfoni Loader will not work after that."),
                                          QMessageBox::Yes | QMessageBox::No);
            if (button == QMessageBox::Yes){
                event->accept();
            }else {
                if(closeAccepted == false)event->ignore();
                else event->accept();
            }
        }else event->accept();
    }
}
