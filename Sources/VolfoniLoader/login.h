#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui>
#include <iostream>
#include <QLineEdit>
#include <QPushButton>

#include "mainwindow.h"

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;

class Login : public QDialog
{
    Q_OBJECT

public:
    Login(QWidget* parent,MainWindow *mW);
    ~Login();


private:

    MainWindow *_mW;

    QPushButton *m_cancel;
    QPushButton *m_apply;
    QHBoxLayout *layout_buttons;


    QLineEdit *log;
    QLabel *l_log;
    QLineEdit *pwd;
    QLabel *l_pwd;
    QGridLayout *gridLogin;

    QVBoxLayout *layout_principal;



protected:

public slots:


private slots:

    void save();

};

#endif // LOGIN_H
