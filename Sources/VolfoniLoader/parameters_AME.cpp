#include "parameters_AME.h"

Parameters_AME::Parameters_AME(QWidget *parent, QString firmRev) : QDialog(parent)
{
    m_swRel = firmRev;

    QGridLayout *layoutSync = new QGridLayout;
 //   QLabel *label3D = new QLabel("3D Synchronization:");
 //   layoutSync->addWidget(label3D, 0, 0);
 //   combo3D = new QComboBox(this);
 //   combo3D->addItem("Inverted");
 //   combo3D->addItem("Normal");
 //   combo3D->addItem("2D Left");
 //   combo3D->addItem("2D Right");
 //   layoutSync->addWidget(combo3D, 0, 1, 1, 12);
    QLabel *labelChannel = new QLabel("RF Channels :");
    layoutSync->addWidget(labelChannel, 2, 0, 2, 1);
    checkCh3 = new QCheckBox(this);
    layoutSync->addWidget(checkCh3, 2, 1);
    QLabel *labelCh3 = new QLabel("3");
    layoutSync->addWidget(labelCh3, 3, 1);
    checkCh4 = new QCheckBox(this);
    layoutSync->addWidget(checkCh4, 2, 2);
    QLabel *labelCh4 = new QLabel("4");
    layoutSync->addWidget(labelCh4, 3, 2);
    checkCh5 = new QCheckBox(this);
    layoutSync->addWidget(checkCh5, 2, 3);
    QLabel *labelCh5 = new QLabel("5");
    layoutSync->addWidget(labelCh5, 3, 3);
    checkCh6 = new QCheckBox(this);
    layoutSync->addWidget(checkCh6, 2, 4);
    QLabel *labelCh6 = new QLabel("6");
    layoutSync->addWidget(labelCh6, 3, 4);
    checkCh7 = new QCheckBox(this);
    layoutSync->addWidget(checkCh7, 2, 5);
    QLabel *labelCh7 = new QLabel("7");
    layoutSync->addWidget(labelCh7, 3, 5);
    checkCh8 = new QCheckBox(this);
    layoutSync->addWidget(checkCh8, 2, 6);
    QLabel *labelCh8 = new QLabel("8");
    layoutSync->addWidget(labelCh8, 3, 6);
    checkCh9 = new QCheckBox(this);
    layoutSync->addWidget(checkCh9, 2, 7);
    QLabel *labelCh9 = new QLabel("9");
    layoutSync->addWidget(labelCh9, 3, 7);
    checkChA = new QCheckBox(this);
    layoutSync->addWidget(checkChA, 2, 8);
    QLabel *labelChA = new QLabel("A");
    layoutSync->addWidget(labelChA, 3, 8);
    checkChB = new QCheckBox(this);
    layoutSync->addWidget(checkChB, 2, 9);
    QLabel *labelChB = new QLabel("B");
    layoutSync->addWidget(labelChB, 3, 9);
    checkChC = new QCheckBox(this);
    layoutSync->addWidget(checkChC, 2, 10);
    QLabel *labelChC = new QLabel("C");
    layoutSync->addWidget(labelChC, 3, 10);
    checkChD = new QCheckBox(this);
    layoutSync->addWidget(checkChD, 2, 11);
    QLabel *labelChD = new QLabel("D");
    layoutSync->addWidget(labelChD, 3, 11);
    checkChE = new QCheckBox(this);
    layoutSync->addWidget(checkChE, 2, 12);
    QLabel *labelChE = new QLabel("E");
    layoutSync->addWidget(labelChE, 3, 12);
    //Slider DarkTime
    QGroupBox *groupeDT = new QGroupBox("DarkTime", this);
    QLabel *contraste = new QLabel("Contrast");
    QLabel *luminosite = new QLabel("Brightness");
    sliderDT = new QSlider(Qt::Horizontal, this);
    sliderDT->setMinimum(-4000);
    sliderDT->setMaximum(4000);
    sliderDT->setSingleStep(100);
    sliderDT->setMinimumWidth(150);
    sliderDT->setMaximumWidth(150);
    QHBoxLayout *layoutSlider = new QHBoxLayout;
    layoutSlider->addWidget(luminosite);
    layoutSlider->addWidget(sliderDT);
    layoutSlider->addWidget(contraste);
    sliderLabel = new QLabel("0");
    QHBoxLayout *layoutSliderLabel = new QHBoxLayout;
    layoutSliderLabel->addWidget(sliderLabel,0,Qt::AlignCenter);
    QVBoxLayout *layoutDT = new QVBoxLayout;
    layoutDT->addLayout(layoutSliderLabel);
    layoutDT->addLayout(layoutSlider);
    groupeDT->setLayout(layoutDT);
    //Delta Sync
    QLabel *deltaLabel = new QLabel("Delta Sync (us) :");
    delta = new QSpinBox(this);
    delta->setMaximum(2000);
    delta->setMinimum(-2000);
    delta->setSingleStep(100);
    QHBoxLayout *layoutDelta = new QHBoxLayout;
    layoutDelta->addWidget(deltaLabel);
    layoutDelta->addWidget(delta);
    //LCD Voltage
//    QLabel *voltageLabel = new QLabel("LCD Voltage (V) :");
//    voltage = new QSpinBox(this);
//    voltage->setMaximum(8);
//    voltage->setMinimum(17);
//    voltage->setSingleStep(1);
//    QHBoxLayout *layoutVoltage = new QHBoxLayout;
//    layoutVoltage->addWidget(voltageLabel);
//    layoutVoltage->addWidget(voltage);

    //Layout : boutons du bas
    appliquer = new QPushButton("Apply");
    quitter = new QPushButton("Quit");
    QHBoxLayout *boutonsLayout = new QHBoxLayout;
    boutonsLayout->setAlignment(Qt::AlignCenter);
    boutonsLayout->addWidget(appliquer);
    boutonsLayout->addWidget(quitter);

    //Fenetre complete
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layoutSync);
    layoutPrincipal->addSpacing(10);
    layoutPrincipal->addWidget(groupeDT);
    layoutPrincipal->addSpacing(10);
    layoutPrincipal->addLayout(layoutDelta);
//    layoutPrincipal->addLayout(layoutVoltage);
    layoutPrincipal->addSpacing(30);
    layoutPrincipal->addLayout(boutonsLayout);
    layoutPrincipal->setSizeConstraint(QLayout::SetMinimumSize);

    setLayout(layoutPrincipal);
    setWindowTitle("AME Parameters");

    //Connections SLOT
    connect(quitter, SIGNAL(clicked()), this, SLOT(accept()));
    connect(appliquer, SIGNAL(clicked()), this, SLOT(on_Apply_triggered()));
    connect(sliderDT, SIGNAL(valueChanged(int)), this, SLOT(displayDT(int)));
 }

Parameters_AME::~Parameters_AME()
{
}

void Parameters_AME::displayDT(int value)
{
    int temp;

    temp = value - (value%100);
    sliderDT->setValue(temp);
    sliderLabel->setText(QString("%1").arg(temp));
}

void Parameters_AME::displayAll(unsigned char *data)
{
    int DarkTime;

    //Lecture des données sur le device
    m_volt = data[0];
    m_up = data[1];
    m_down = data[2];
    m_delta = data[3];
    m_rfPosaLsb = data[19];
    m_rfPosaMsb = data[20];
    m_rfPosbLsb = data[21];
    m_rfPosbMsb = data[22];
    m_3Dmode = data[26];

    // Switch Pos A
    if( (m_rfPosaMsb == 0xFF) && (m_rfPosaLsb == 0xFF) )
    {   // Conf par défaut de l'AME : seul le canal 8 est actif
           checkCh3->setChecked(false);
           checkCh4->setChecked(false);
           checkCh5->setChecked(false);
           checkCh6->setChecked(false);
           checkCh7->setChecked(false);
           checkCh8->setChecked(true);
           checkCh9->setChecked(false);
           checkChA->setChecked(false);
           checkChB->setChecked(false);
           checkChC->setChecked(false);
           checkChD->setChecked(false);
           checkChE->setChecked(false);
    }
    else
    { // RF
        if(m_rfPosaLsb&0x01) checkCh3->setChecked(true);
        if(m_rfPosaLsb&0x02) checkCh4->setChecked(true);
        if(m_rfPosaLsb&0x04) checkCh5->setChecked(true);
        if(m_rfPosaLsb&0x08) checkCh6->setChecked(true);
        if(m_rfPosaLsb&0x10) checkCh7->setChecked(true);
        if(m_rfPosaLsb&0x20) checkCh8->setChecked(true);
        if(m_rfPosaLsb&0x40) checkCh9->setChecked(true);
        if(m_rfPosaLsb&0x80) checkChA->setChecked(true);
        if(m_rfPosaMsb&0x01) checkChB->setChecked(true);
        if(m_rfPosaMsb&0x02) checkChC->setChecked(true);
        if(m_rfPosaMsb&0x04) checkChD->setChecked(true);
        if(m_rfPosaMsb&0x08) checkChE->setChecked(true);
    }

     // Slider DarkTime
    if(m_up == -1) DarkTime = 0;
    else DarkTime = - QString::number((char)m_up*16).toInt();
    if(m_down != -1) DarkTime -= QString::number((char)m_down*16).toInt();
    sliderDT->setValue(DarkTime);
    sliderLabel->setText(QString("%1").arg(DarkTime));

    // Delta Sync
    if(m_delta == -1) m_delta = 0;
    delta->setValue(m_delta*16);

}

void Parameters_AME::on_Apply_triggered()
{
    unsigned char data[64];

    data[0] = m_volt;

    //Slider DarkTime
    data[1] = -sliderDT->value()/2/16;
    data[2] = -sliderDT->value()/16 - data[1];

    //Delta Sync
    m_delta = (delta->value())/16;
    data[3] = m_delta;

    // 3D Switch Position
    //data[26] = combo3D->currentIndex();
    data[26] = 0;
    data[27] = 0;
    data[28] = 0;

    // EDGE VR 3D Switch Position
    m_rfPosaLsb = 0;
    if(checkCh3->isChecked()) m_rfPosaLsb += 0x01;
    if(checkCh4->isChecked()) m_rfPosaLsb += 0x02;
    if(checkCh5->isChecked()) m_rfPosaLsb += 0x04;
    if(checkCh6->isChecked()) m_rfPosaLsb += 0x08;
    if(checkCh7->isChecked()) m_rfPosaLsb += 0x10;
    if(checkCh8->isChecked()) m_rfPosaLsb += 0x20;
    if(checkCh9->isChecked()) m_rfPosaLsb += 0x40;
    if(checkChA->isChecked()) m_rfPosaLsb += 0x80;
     m_rfPosaMsb = 0x80;
    if(checkChB->isChecked()) m_rfPosaMsb += 0x01;
    if(checkChC->isChecked()) m_rfPosaMsb += 0x02;
    if(checkChD->isChecked()) m_rfPosaMsb += 0x04;
    if(checkChE->isChecked()) m_rfPosaMsb += 0x08;
    data[19] = m_rfPosaLsb;
    data[20] = m_rfPosaMsb;

    m_rfPosbLsb = 0x20;
    m_rfPosbMsb = 0x80;
    data[21] = m_rfPosbLsb;
    data[22] = m_rfPosbMsb;

    data[23] = 0;
    data[24] = 0x80;
    data[25] = 0;
    data[29] = 0;
    data[32] = 0;
    data[30] = 0;
    data[31] = 0;
    data[33] = 0;

    emit apply_clicked(data);
}
