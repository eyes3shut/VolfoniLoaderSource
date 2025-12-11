#ifndef CRYPTO_H
#define CRYPTO_H

#include <QtGui>

class Crypto : public QDialog
{
    Q_OBJECT
public:
    explicit Crypto(bool limited = true, QString filekey = "", QWidget *parent = 0);
    QString getregistrationID(void);
    bool sameHashInFile();
    QString hashRegistrationID(QString);

private:
    QString m_registrationID;
    QString m_activationKey;
    void hashAll(void);

    QPushButton* b_ok;
    QPushButton* b_cancel;
    QPushButton* b_generate;
    QLabel* labelRegistrationID;
    QLineEdit* leRegistrationID;
    QLabel* labelActivationKey;
    QLineEdit* leActivationKey;

    bool _limited;
    QString _fileKey;

signals:

public slots:
    void activate();
    void generate();

};

#endif // CRYPTO_H
