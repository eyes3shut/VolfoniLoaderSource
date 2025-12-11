#include <QtGui>
#include <QList>
#include <QtNetwork>
#include <QTranslator>
#include <QtGui/QApplication>

#include "tools.h"

SelectDevice::SelectDevice(QWidget* parent, MainWindow *mW)
    : QDialog(parent,Qt::WindowCloseButtonHint),
      _mW(mW)

 {
    this->setWindowTitle(tr("Unable to recognize product (No serial number) - Select your product"));
    this->resize(500,80);

    closeAccepted = false;

    QPushButton *m_save = new QPushButton(tr("OK"));
    m_save->setCursor(Qt::PointingHandCursor);
    m_save->setFixedWidth(70);
    connect(m_save, SIGNAL(clicked()), this, SLOT(valid()));

    products = new QComboBox();
    products->addItem("Activ Me");
    products->addItem("Activ Hub");
    products->addItem("Smart Crystal Cinema");
    products->addItem("Smart Crystal Pro");
    products->addItem("EDGE 3D Glasses");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(products);
    mainLayout->addWidget(m_save,0,Qt::AlignRight);

    setLayout(mainLayout);
}

void SelectDevice::valid(){
    closeAccepted = true;
    SelectDevice::close();

    if(products->currentIndex()==0){
        _mW->setTrigramm("AME");
    }
    if(products->currentIndex()==1){
        _mW->setTrigramm("AHU");
    }
    if(products->currentIndex()==2){
        _mW->setTrigramm("SCC");
    }
    if(products->currentIndex()==3){
        _mW->setTrigramm("SCP");
    }
    if(products->currentIndex()==4){
        _mW->setTrigramm("EDG");
    }

}

void SelectDevice::closeEvent(QCloseEvent *event){

    if(closeAccepted == false){
        event->accept();
        _mW->setAcceptManager(false);

    }else {
        event->accept();
        _mW->setAcceptManager(true);
    }
}
