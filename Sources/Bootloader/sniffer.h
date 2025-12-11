#ifndef SNIFFER_H
#define SNIFFER_H

#include <QDialog>
#include <QStandardItemModel>

#define COL_DATE    0
#define COL_ID      1
#define COL_TYPE    2
#define COL_MSG     3

namespace Ui {
    class Sniffer;
}

class Sniffer : public QDialog
{
    Q_OBJECT

public:
    explicit Sniffer(QWidget *parent = 0);
    ~Sniffer();

private:
    Ui::Sniffer *ui;
    QStandardItemModel *model;
    void fillData(QString id, QString type, QString msg);
    QList<QStandardItem *> constructRow(QString &first, QString &second, QString &third, QString &fourth);
    QString getStrType(unsigned char data);
    QString hex2String(unsigned char* data, int length);
    unsigned char m_messageToSend[21];

public slots:
    void displayData(unsigned char *);
    void prepareRFData();

signals:
    void sendRFData(int, unsigned char*);
};

#endif // SNIFFER_H
