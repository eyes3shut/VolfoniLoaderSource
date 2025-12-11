#include "batteryview.h"

BatteryView::BatteryView(QWidget *parent) : QDialog(parent)
{
    label0 = new QLabel("");
    battBar0 = new QProgressBar();
    battBar0->setFixedSize(300,20);
    QHBoxLayout *layoutBatt0 = new QHBoxLayout;
    layoutBatt0->addWidget(label0);
    layoutBatt0->addWidget(battBar0);
    label0->setVisible(false);
    battBar0->setVisible(false);

    label1 = new QLabel("");
    battBar1 = new QProgressBar();
    battBar1->setFixedSize(300,20);
    QHBoxLayout *layoutBatt1 = new QHBoxLayout;
    layoutBatt1->addWidget(label1);
    layoutBatt1->addWidget(battBar1);
    label1->setVisible(false);
    battBar1->setVisible(false);

    label2 = new QLabel("");
    battBar2 = new QProgressBar();
    battBar2->setFixedSize(300,20);
    QHBoxLayout *layoutBatt2 = new QHBoxLayout;
    layoutBatt2->addWidget(label2);
    layoutBatt2->addWidget(battBar2);
    label2->setVisible(false);
    battBar2->setVisible(false);

    label3 = new QLabel("");
    battBar3 = new QProgressBar();
    battBar3->setFixedSize(300,20);
    QHBoxLayout *layoutBatt3 = new QHBoxLayout;
    layoutBatt3->addWidget(label3);
    layoutBatt3->addWidget(battBar3);
    label3->setVisible(false);
    battBar3->setVisible(false);

    label4 = new QLabel("");
    battBar4 = new QProgressBar();
    battBar4->setFixedSize(300,20);
    QHBoxLayout *layoutBatt4 = new QHBoxLayout;
    layoutBatt4->addWidget(label4);
    layoutBatt4->addWidget(battBar4);
    label4->setVisible(false);
    battBar4->setVisible(false);

    label5 = new QLabel("");
    battBar5 = new QProgressBar();
    battBar5->setFixedSize(300,20);
    QHBoxLayout *layoutBatt5 = new QHBoxLayout;
    layoutBatt5->addWidget(label5);
    layoutBatt5->addWidget(battBar5);
    label5->setVisible(false);
    battBar5->setVisible(false);

    label6 = new QLabel("");
    battBar6 = new QProgressBar();
    battBar6->setFixedSize(300,20);
    QHBoxLayout *layoutBatt6 = new QHBoxLayout;
    layoutBatt6->addWidget(label6);
    layoutBatt6->addWidget(battBar6);
    label6->setVisible(false);
    battBar6->setVisible(false);

    label7 = new QLabel("");
    battBar7 = new QProgressBar();
    battBar7->setFixedSize(300,20);
    QHBoxLayout *layoutBatt7 = new QHBoxLayout;
    layoutBatt7->addWidget(label7);
    layoutBatt7->addWidget(battBar7);
    label7->setVisible(false);
    battBar7->setVisible(false);

    label8 = new QLabel("");
    battBar8 = new QProgressBar();
    battBar8->setFixedSize(300,20);
    QHBoxLayout *layoutBatt8 = new QHBoxLayout;
    layoutBatt8->addWidget(label8);
    layoutBatt8->addWidget(battBar8);
    label8->setVisible(false);
    battBar8->setVisible(false);

    label9 = new QLabel("");
    battBar9 = new QProgressBar();
    battBar9->setFixedSize(300,20);
    QHBoxLayout *layoutBatt9 = new QHBoxLayout;
    layoutBatt9->addWidget(label9);
    layoutBatt9->addWidget(battBar9);
    label9->setVisible(false);
    battBar9->setVisible(false);

    refresh = new QPushButton("Refresh");
    quit = new QPushButton("Quit");
    QHBoxLayout *boutonsLayout = new QHBoxLayout;
    boutonsLayout->setAlignment(Qt::AlignCenter);
    boutonsLayout->addWidget(refresh);
    boutonsLayout->addWidget(quit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layoutBatt0);
    mainLayout->addLayout(layoutBatt1);
    mainLayout->addLayout(layoutBatt2);
    mainLayout->addLayout(layoutBatt3);
    mainLayout->addLayout(layoutBatt4);
    mainLayout->addLayout(layoutBatt5);
    mainLayout->addLayout(layoutBatt6);
    mainLayout->addLayout(layoutBatt7);
    mainLayout->addLayout(layoutBatt8);
    mainLayout->addLayout(layoutBatt9);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(boutonsLayout);

    setLayout(mainLayout);

    connect(quit, SIGNAL(clicked()), this, SLOT(accept()));
    connect(refresh, SIGNAL(clicked()), this, SLOT(on_refresh_trigerred()));

}

void BatteryView::on_refresh_trigerred(){
    label0->setVisible(false);
    battBar0->setVisible(false);
    label1->setVisible(false);
    battBar1->setVisible(false);
    label2->setVisible(false);
    battBar2->setVisible(false);
    label3->setVisible(false);
    battBar3->setVisible(false);
    label4->setVisible(false);
    battBar4->setVisible(false);
    label5->setVisible(false);
    battBar5->setVisible(false);
    label6->setVisible(false);
    battBar6->setVisible(false);
    label7->setVisible(false);
    battBar7->setVisible(false);
    label8->setVisible(false);
    battBar8->setVisible(false);
    label9->setVisible(false);
    battBar9->setVisible(false);
    emit refresh_clicked(true);
}

void BatteryView::displayAll(int battInd, QString battID, int battLevel) {

    int temp = 100 * (battLevel-0x33A) / (0x352-0x33A);
    if (temp>100) temp = 100;

    if(battInd==0) {
        label0->setText(battID + " :");
        battBar0->setValue(temp);
        label0->setVisible(true);
        battBar0->setVisible(true);
    }
    else if(battInd==1) {
        label1->setText(battID + " :");
        battBar1->setValue(temp);
         label1->setVisible(true);
        battBar1->setVisible(true);
    }
    else if(battInd==2) {
        label2->setText(battID + " :");
         battBar2->setValue(temp);
        label2->setVisible(true);
        battBar2->setVisible(true);
    }
    else if(battInd==3) {
        label3->setText(battID + " :");
        battBar3->setValue(temp);
        label3->setVisible(true);
        battBar3->setVisible(true);
    }
    else if(battInd==4) {
        label4->setText(battID + " :");
        battBar4->setValue(temp);
        label4->setVisible(true);
        battBar4->setVisible(true);
    }
    else if(battInd==5) {
        label5->setText(battID + " :");
        battBar5->setValue(temp);
        label5->setVisible(true);
        battBar5->setVisible(true);
    }
    else if(battInd==6) {
        label6->setText(battID + " :");
        battBar6->setValue(temp);
        label6->setVisible(true);
        battBar6->setVisible(true);
    }
    else if(battInd==7) {
        label7->setText(battID + " :");
         battBar7->setValue(temp);
        label7->setVisible(true);
        battBar7->setVisible(true);
    }
    else if(battInd==8) {
        label8->setText(battID + " :");
         battBar8->setValue(temp);
       label8->setVisible(true);
        battBar8->setVisible(true);
    }
    else if(battInd==9) {
        label9->setText(battID + " :");
        battBar9->setValue(temp);
        label9->setVisible(true);
        battBar9->setVisible(true);
    }
}
