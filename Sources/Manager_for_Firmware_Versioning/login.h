#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui>
#include <iostream>

#include "generalTab.h"
#include "mainwindow.h"

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;

class Login : public QDialog
{
    Q_OBJECT

public:
    Login(QWidget* parent, bool read);
    ~Login();
    void setGT(GeneralTab *gT);

    void setMW(MainWindow *mW);


private:

    GeneralTab *_gT;
    MainWindow *_mW;
    bool _read;

    QPushButton *m_cancel;
    QPushButton *m_apply;
    QHBoxLayout *layout_buttons;


    QLabel *l_url;
    QLineEdit *url;
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
