#ifndef PASSWORD_H
#define PASSWORD_H

#include <QtGui>

class password : public QDialog
{
    Q_OBJECT
public:
    explicit password(QWidget *parent = 0);

private:
    QPushButton *b_ok;
    QPushButton *b_cancel;
    QLineEdit *pwdEdit;
    QLabel *label;

signals:
    void pwdValid();

public slots:
    void checkPwd();

};

#endif // PASSWORD_H
