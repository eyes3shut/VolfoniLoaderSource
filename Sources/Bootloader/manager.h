#ifndef MANAGER_H
#define MANAGER_H

#include <QDialog>
#include <QAbstractButton>
#include <QtXml>
#include <iostream>
#include <QFile>

namespace Ui {
    class Manager;
}

class Manager : public QDialog
{
    Q_OBJECT


public:
    explicit Manager(QWidget *parent = 0, QString trigrammeProduct = "", QString pathResource ="");
    ~Manager();

private:
    Ui::Manager *ui;
    QString _trigrammeProduct;
    QString _pathResource;
    unsigned char sync_LR;

public slots:
    void Apply_clicked();
    void display_data(unsigned char *);
    void display_channel(unsigned char *);
    void Inversion_clicked();
    void display_sync(unsigned char);

signals:
    void Mng_apply_changes(unsigned char *);
    void Mng_sync_changes();
};

#endif // MANAGER_H
