#include "sniffer.h"
#include "ui_sniffer.h"

#include <QStandardItemModel>
#include <QTime>

Sniffer::Sniffer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sniffer)
{
    ui->setupUi(this);

    setWindowTitle(QString("Sniffer"));

    model = new QStandardItemModel(0,4,this);    //0 Rows and 4 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Date")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("ID")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Type")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Message")));
    ui->snifferTableView->setModel(model);

    connect(ui->pushButtonGo, SIGNAL(clicked()), this, SLOT(prepareRFData()));
}

Sniffer::~Sniffer()
{
    delete ui;
}

void Sniffer::fillData(QString id, QString type, QString msg)
{
    //QDate date=QDate::currentDate();
    QTime time=QTime::currentTime();
    QString dateString = time.toString("hh:mm:ss");
    //dateString.append(date.toString("MM/dd/yyyy "));

    //QStandardItem *msg = new QStandardItem(dateString);
    QList<QStandardItem *> nextRow = constructRow(dateString, id, type, msg);
    model->appendRow(nextRow);
}

QList<QStandardItem *> Sniffer::constructRow(QString &first, QString &second, QString &third, QString &fourth)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    rowItems << new QStandardItem(fourth);
    return rowItems;
}

void Sniffer::displayData(unsigned char *data)
{
    QString type = getStrType(data[0]);
    QString id = "";
    QString message = "";

    id = QString((const char *)&data[1]).left(8);
    message = hex2String(&data[9], 13);

    fillData(id, type, message);

    qWarning("display data 0x%x 0x%x 0x%x", data[0], data[1], data[2]);
    if( (data[0] == m_messageToSend[0]) && (data[7] == m_messageToSend[7]) && (data[8] == m_messageToSend[8]) \
            && (data[6] == m_messageToSend[6]) && (data[5] == m_messageToSend[5]) && (data[4] == m_messageToSend[4]) \
            && (data[3] == m_messageToSend[3]) && (data[2] == m_messageToSend[2]) && (data[1] == m_messageToSend[1]) )
        emit sendRFData(0, m_messageToSend);

}

QString Sniffer::getStrType(unsigned char data)
{
    QString result=QString("");

    switch(data)
    {
    case 0x0A:
        result = QString("Pairing");
        break;
    case 0x04:
        result = QString("Battery");
        break;
    case 0x05:
        result = QString("Optical Mode");
        break;
    case 0x06:
        result = QString("DarkTime");
        break;
    case 0x07:
        result = QString("DeltaSync");
        break;
    case 0x08:
        result = QString("ON Accelero");
        break;
    case 0x09:
        result = QString("OFF Accelero");
        break;
    case 0x0B:
        result = QString("Diode");
        break;
    case 0x0C:
        result = QString("Switch OFF");
        break;
    case 0x0D:
        result = QString("Freewheel");
        break;
    default:
        result = QString("Unknown");
        break;
    }

    return result;
}

QString Sniffer::hex2String(unsigned char* data, int length)
{
    QString result="";
    QString temp;

    for(int i=0; i<length; i++)
    {
        temp = QString().sprintf(" 0x%02x", data[i]);
        result.append(temp);
    }

    return result;
}

void Sniffer::prepareRFData()
{
    m_messageToSend[0] = ui->snifferEdit_Type->toPlainText().toInt(0,16);
    for(int i=0; i<8; i++) m_messageToSend[i+1] = ui->snifferEdit_ID->toPlainText().at(i).toAscii();
    m_messageToSend[9] = ui->snifferEdit_M1->toPlainText().toInt(0,16);
    m_messageToSend[10] = ui->snifferEdit_M2->toPlainText().toInt(0,16);

    emit sendRFData(11, m_messageToSend);
}
