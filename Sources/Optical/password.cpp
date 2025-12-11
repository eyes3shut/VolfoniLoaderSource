#include "password.h"

password::password(QWidget *parent) :
    QDialog(parent)
{
    b_ok = new QPushButton("OK");
    b_cancel = new QPushButton("Cancel");

    label = new QLabel("Password :");
    pwdEdit = new QLineEdit();
    pwdEdit->setEchoMode(QLineEdit::Password);

    QGridLayout *layout = new QGridLayout;
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 0, 0);
    layout->addWidget(pwdEdit, 0, 1, 1, 2);
    layout->addWidget(b_ok, 1, 1);
    layout->addWidget(b_cancel, 1, 2);

    setLayout(layout);

    connect(b_cancel, SIGNAL(clicked()), this, SLOT(accept()));
    connect(b_ok, SIGNAL(clicked()), this, SLOT(checkPwd()));
}

void password::checkPwd()
{
    if(pwdEdit->text() == "volFoni")
    {
        emit pwdValid();
        this->accept();
    }
    else QMessageBox::warning(this, "Error", "Invalid Password");
}
