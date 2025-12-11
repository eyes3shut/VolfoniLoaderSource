#include <QtGui>


#include "login.h"



Login::Login(QWidget* parent, bool read)
: QDialog(parent,Qt::WindowCloseButtonHint),
  _read(read)
{

    resize(500,150);
    this->setWindowIcon(QIcon(":/MainWindow/ressources/volfo160.ico"));
    this->setWindowTitle("Identification parameters");

    m_apply = new QPushButton(tr("OK"));
    m_apply->setCursor(Qt::PointingHandCursor);
    m_apply->setFixedWidth(100);
    connect(m_apply, SIGNAL(clicked()), this, SLOT(save()));

    layout_buttons = new QHBoxLayout;
    layout_buttons->addWidget(m_apply,0,Qt::AlignCenter);

    l_log = new QLabel(tr("Username:"));
    l_pwd = new QLabel(tr("Password:"));
    l_url = new QLabel(tr("Url:"));

    log = new QLineEdit();
    if(_read==true)log->setText("VolfoniLoader");
    else log->setText("volfoni-ldr");

    url = new QLineEdit();
    if(_read==true)url->setText("http://vl.volfoni.com/Serveur/");
    else url->setText("ftp.cluster005.hosting.ovh.net");

    pwd = new QLineEdit();
    if(_read==true)pwd->setText("vol123");
    else pwd->setText("vOlfOnI123");

    gridLogin = new QGridLayout;
    gridLogin->addWidget(l_url,0,0);
    gridLogin->addWidget(url,0,1);
    gridLogin->addWidget(l_log,1,0);
    gridLogin->addWidget(log,1,1);
    gridLogin->addWidget(l_pwd,2,0);
    gridLogin->addWidget(pwd,2,1);


    layout_principal = new QVBoxLayout;
    layout_principal->addLayout(gridLogin);
    layout_principal->addLayout(layout_buttons);

    setLayout(layout_principal);


}

void Login::setGT(GeneralTab *gT){
    _gT = gT;
}

void Login::setMW(MainWindow *mW){
    _mW = mW;
}

Login::~Login()
{

}

void Login::save(){


    close();
    if(_read==true)_gT->logUser(url->text(),log->text(),pwd->text());
    else _mW->logUser(url->text(),log->text(),pwd->text());

}
