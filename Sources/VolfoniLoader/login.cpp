#include <QtGui>


#include "login.h"



Login::Login(QWidget* parent, MainWindow *mW)
    : QDialog(parent, Qt::WindowCloseButtonHint),
    _mW(mW)
{
    this->setWindowIcon(QIcon(":/MainWindow/ressources/volfo160.ico"));
    this->setWindowTitle(tr("Log on"));

    // Message de confirmation
    QLabel *message = new QLabel(tr("Confirm access Advanced Option"));
    message->setAlignment(Qt::AlignCenter);

    // Boutons Oui / Non
    QPushButton *btnConnect = new QPushButton(tr("Yes"));
    btnConnect->setCursor(Qt::PointingHandCursor);
    connect(btnConnect, &QPushButton::clicked, this, &Login::save);

    QPushButton *btnCancel = new QPushButton(tr("No"));
    btnCancel->setCursor(Qt::PointingHandCursor);
    connect(btnCancel, &QPushButton::clicked, this, &Login::close);

    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->addWidget(btnConnect);
    layout_buttons->addWidget(btnCancel);

    QVBoxLayout *layout_principal = new QVBoxLayout;
    layout_principal->addWidget(message);
    layout_principal->addLayout(layout_buttons);

    setLayout(layout_principal);
}


Login::~Login()
{

}

void Login::save(){


    close();
    _mW->logUser();

}
