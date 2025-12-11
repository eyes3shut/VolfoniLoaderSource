#include <QtGui>
#include <QList>

#include "viewTab.h"


ViewTab::ViewTab(MainWindow *mW, int nDevice)
    : QDialog(),
      _mW(mW),
      _nDevice(nDevice)
{

    model = new QStandardItemModel(0,8);

    QStringList entetes;
    entetes <<"Product"<<"Trigram" <<"Filename" << "Updates name" <<"Description" << "Date of publication" << "Changes with previous version" << "CRC";
    model->setHorizontalHeaderLabels(entetes);

    table = new QTableView();

    table->setModel(model);
    table->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true); // dernière colonne adapté a longueur du texte
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);// cases non modifiables
    table->setSelectionBehavior(QAbstractItemView::SelectRows);// focus (fond bleu) sur toute une ligne et non juste la case sous la souris

    QLabel *l_filter = new QLabel("Filter:");
    l_filter->setFixedWidth(60);

    comboFilterDevice = new QComboBox;
    comboFilterDevice->addItem("All");
    for(int i =0;i<_nDevice;i++)comboFilterDevice->addItem(_mW->nameDevices[i]);
    comboFilterDevice->setFixedWidth(150);
    connect(comboFilterDevice, SIGNAL(activated(int)), this, SLOT(validFilterDevice(int)));

    comboFilterVersion = new QComboBox;
    comboFilterVersion->addItem("All");
    comboFilterVersion->setEnabled(false);
    comboFilterVersion->setFixedWidth(150);
    connect(comboFilterVersion, SIGNAL(activated(int)), this, SLOT(validFilterVersion(int)));

    QGridLayout *layout_filter = new QGridLayout;
    layout_filter->addWidget(l_filter,0,0);
    layout_filter->addWidget(comboFilterDevice,0,1,1,1,Qt::AlignLeft);
    layout_filter->addWidget(comboFilterVersion,1,1,1,1,Qt::AlignLeft);

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::HLine | QFrame::Raised);
    frame->setLineWidth(0);
    frame->setMidLineWidth(3);

    layout_principal = new QVBoxLayout;
    layout_principal->addLayout(layout_filter);
    layout_principal->addWidget(frame);
    layout_principal->addWidget(table);

    setLayout(layout_principal);

}

ViewTab::~ViewTab()
{

}

void ViewTab::validFilterDevice(int a){
    if(a==0){
        comboFilterVersion->setCurrentIndex(0);
        comboFilterVersion->setEnabled(false);
    }else comboFilterVersion->setEnabled(true);
    if(a>0){
        QSortFilterProxyModel *modeleFiltre = new QSortFilterProxyModel() ;

        modeleFiltre->setSourceModel(model);
        modeleFiltre->setFilterKeyColumn(1) ;
        modeleFiltre->setFilterFixedString(_mW->trigramDevices[a-1]) ;

        table->setModel(modeleFiltre) ;

        comboFilterVersion->clear();
        comboFilterVersion->addItem("All");
        for(int i =0; i<nVersionDiff[a-1];i++)comboFilterVersion->addItem(nVersionDiffValue[a-1][i]);
    }else table->setModel(model) ;
}

void ViewTab::validFilterVersion(int a){
    if(a>0){
        QSortFilterProxyModel *modeleFiltre = new QSortFilterProxyModel() ;

        modeleFiltre->setSourceModel(model);
        modeleFiltre->setFilterKeyColumn(3) ;
        modeleFiltre->setFilterFixedString(comboFilterVersion->currentText()) ;

        table->setModel(modeleFiltre) ;

    }else {
        QSortFilterProxyModel *modeleFiltre = new QSortFilterProxyModel() ;

        modeleFiltre->setSourceModel(model);
        modeleFiltre->setFilterKeyColumn(1) ;
        modeleFiltre->setFilterFixedString(_mW->trigramDevices[comboFilterDevice->currentIndex()-1]) ;

        table->setModel(modeleFiltre) ;
    }
}

void ViewTab::drawTable(){
    model->clear();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    int nTotalUpdate = 0;
    for(int i = 0; i < _nDevice; i++){
        nTotalUpdate += _mW->edittab[i]->getNUpdate();
    }
    QStringList entetes;
    entetes <<"Product"<<"Trigram" <<"Filename" << "Updates name" <<"Description" << "Date of publication" << "Changes with previous version" << "CRC";
    model->setHorizontalHeaderLabels(entetes);

    int startLine = 0;

    for(int j =0; j<_nDevice; j++){

        if(j>0)startLine+=_mW->edittab[j-1]->getNUpdate();

        nVersionDiff[j]=_mW->edittab[j]->getNVersionDiff();
        QStringList list2 = QString(_mW->edittab[j]->getNVersionDiffValue()).split("**");
        for(int i =0; i<nVersionDiff[j];i++){
            nVersionDiffValue[j][i]=list2.at(i);
        }
        int nUpdate = _mW->edittab[j]->getNUpdate();

        for (int row = startLine; row < startLine + nUpdate; ++row) {

            QStringList list2 = QString(_mW->edittab[j]->getName()).split("**");
            QStandardItem *item4 = new QStandardItem(list2.at(row - startLine));
            ////////////////////////////////////
            // get color for background
            QColor background(0,0,0,120);
            if(list2.at(row - startLine).contains(".")){
                QStringList tempo = QString(list2.at(row - startLine)).split(".");
                QString tempo2 = tempo.at(0);
                int somme = 0;
                for(int j =0; j<tempo2.size();j++){somme+=tempo2.at(j).toAscii();}
                somme = somme*somme*somme;
                int r = somme%251;
                int g = somme%201;
                int b = somme%187;
                background = QColor(r,g,b,120);
            }
            ///////////////////////////////////////

            item4->setTextAlignment(Qt::AlignCenter);
            item4->setBackground(background);
            model->setItem(row,3,item4);

            QStandardItem *item1 = new QStandardItem(_mW->nameDevices[j]);
            item1->setTextAlignment(Qt::AlignCenter);
            item1->setBackground(background);
            model->setItem(row,0,item1);

            QStandardItem *item2 = new QStandardItem(_mW->trigramDevices[j]);
            item2->setTextAlignment(Qt::AlignCenter);
            item2->setBackground(background);
            model->setItem(row,1,item2);

            QStringList list1 = QString(_mW->edittab[j]->getFileName()).split("**");
            QStandardItem *item3 = new QStandardItem(list1.at(row - startLine));
            item3->setTextAlignment(Qt::AlignCenter);
            item3->setBackground(background);
            model->setItem(row,2,item3);

            QStringList list3 = QString(_mW->edittab[j]->getDescription()).split("**");
            QStandardItem *item5 = new QStandardItem(list3.at(row - startLine));
            item5->setTextAlignment(Qt::AlignCenter);
            item5->setBackground(background);
            model->setItem(row,4,item5);

            QStringList list4 = QString(_mW->edittab[j]->getDate()).split("**");
            QStandardItem *item6 = new QStandardItem(list4.at(row - startLine));
            item6->setTextAlignment(Qt::AlignCenter);
            item6->setBackground(background);
            model->setItem(row,5,item6);

            QStringList list5 = QString(_mW->edittab[j]->getModifs()).split("**");
            QStandardItem *item7 = new QStandardItem(list5.at(row - startLine));
            item7->setTextAlignment(Qt::AlignCenter);
            item7->setBackground(background);
            model->setItem(row,6,item7);

            QStringList list6 = QString(_mW->edittab[j]->getCRC()).split("**");
            QStandardItem *item8 = new QStandardItem(list6.at(row - startLine));
            item8->setTextAlignment(Qt::AlignCenter);
            item8->setBackground(background);
            model->setItem(row,7,item8);
        }
    }

    if(comboFilterDevice->currentIndex()>0){
        if(comboFilterVersion->currentIndex()==0)validFilterDevice(comboFilterDevice->currentIndex());
        else validFilterVersion(comboFilterVersion->currentIndex());
    }
    else table->setModel(model);
    QApplication::restoreOverrideCursor();
}
