#ifndef VIEWTAB_H
#define VIEWTAB_H

#include <QtGui>
#include <QMenu>
#include <iostream>
#include <QStatusBar>
#include <QComboBox>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QSignalMapper>
#include <QScrollArea>

#include "mainwindow.h"

class MainWindow;


class ViewTab : public QDialog
{
    Q_OBJECT

public:
    ViewTab(MainWindow *mW, int nDevice);
    ~ViewTab();

    void drawTable();


private:

    QStandardItemModel *model;
    QTableView *table;

    QVBoxLayout *layout_principal;
    MainWindow *_mW;
    int _nDevice;

    QComboBox *comboFilterDevice;
    QComboBox *comboFilterVersion;

    int nVersionDiff[100];
    QString nVersionDiffValue[100][100];

protected:


public slots:


private slots:

    void validFilterDevice(int a);

    void validFilterVersion(int a);

};

#endif // VIEWTAB_H
