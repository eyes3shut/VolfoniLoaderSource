#include "parameters.h"

Parameters::Parameters(QWidget *parent, QString firmRev) : QDialog(parent)
{
    m_swRel = firmRev;

    //GroupBox mode Optique
    QGroupBox *group3D = new QGroupBox("3D switch", this);
    QVBoxLayout *layout3D = new QVBoxLayout;
    full3D = new QRadioButton("3D on all positions");
    full3DReverse = new QRadioButton("3D Reverse on all positions");
    sharedScreen = new QRadioButton("Shared screen");
    layout3D->addWidget(full3D);
    layout3D->addWidget(full3DReverse);
    layout3D->addWidget(sharedScreen);
    group3D->setLayout(layout3D);
    if(m_swRel.startsWith("EDGVR") || m_swRel.startsWith("EDGHS") || m_swRel.startsWith("__AHU")) group3D->setVisible(false);

    //GoupBox switch pos A
    QGroupBox *groupPosA = new QGroupBox("Switch Pos A", this);
    QGridLayout *layoutPosA = new QGridLayout;
    QLabel *label3D = new QLabel("3D :");
    layoutPosA->addWidget(label3D, 0, 0);
    combo3D = new QComboBox(this);
    combo3D->addItem("Inverted");
    combo3D->addItem("Normal");
    combo3D->addItem("2D Left");
    combo3D->addItem("2D Right");
    layoutPosA->addWidget(combo3D, 0, 1, 1, 12);
    QLabel *labelSync = new QLabel("Synchronization :");
    layoutPosA->addWidget(labelSync, 1, 0);
    comboSync = new QComboBox(this);
    comboSync->addItem("IR");
    comboSync->addItem("RF");
    layoutPosA->addWidget(comboSync, 1, 1, 1, 12);
    QLabel *labelChannel = new QLabel("RF Channels :");
    layoutPosA->addWidget(labelChannel, 2, 0, 2, 1);
    checkCh3 = new QCheckBox(this);
    layoutPosA->addWidget(checkCh3, 2, 1);
    QLabel *labelCh3 = new QLabel("3");
    layoutPosA->addWidget(labelCh3, 3, 1);
    checkCh4 = new QCheckBox(this);
    layoutPosA->addWidget(checkCh4, 2, 2);
    QLabel *labelCh4 = new QLabel("4");
    layoutPosA->addWidget(labelCh4, 3, 2);
    checkCh5 = new QCheckBox(this);
    layoutPosA->addWidget(checkCh5, 2, 3);
    QLabel *labelCh5 = new QLabel("5");
    layoutPosA->addWidget(labelCh5, 3, 3);
    checkCh6 = new QCheckBox(this);
    layoutPosA->addWidget(checkCh6, 2, 4);
    QLabel *labelCh6 = new QLabel("6");
    layoutPosA->addWidget(labelCh6, 3, 4);
    checkCh7 = new QCheckBox(this);
    layoutPosA->addWidget(checkCh7, 2, 5);
    QLabel *labelCh7 = new QLabel("7");
    layoutPosA->addWidget(labelCh7, 3, 5);
    checkCh8 = new QCheckBox(this);
    layoutPosA->addWidget(checkCh8, 2, 6);
    QLabel *labelCh8 = new QLabel("8");
    layoutPosA->addWidget(labelCh8, 3, 6);
    checkCh9 = new QCheckBox(this);
    layoutPosA->addWidget(checkCh9, 2, 7);
    QLabel *labelCh9 = new QLabel("9");
    layoutPosA->addWidget(labelCh9, 3, 7);
    checkChA = new QCheckBox(this);
    layoutPosA->addWidget(checkChA, 2, 8);
    QLabel *labelChA = new QLabel("A");
    layoutPosA->addWidget(labelChA, 3, 8);
    checkChB = new QCheckBox(this);
    layoutPosA->addWidget(checkChB, 2, 9);
    QLabel *labelChB = new QLabel("B");
    layoutPosA->addWidget(labelChB, 3, 9);
    checkChC = new QCheckBox(this);
    layoutPosA->addWidget(checkChC, 2, 10);
    QLabel *labelChC = new QLabel("C");
    layoutPosA->addWidget(labelChC, 3, 10);
    checkChD = new QCheckBox(this);
    layoutPosA->addWidget(checkChD, 2, 11);
    QLabel *labelChD = new QLabel("D");
    layoutPosA->addWidget(labelChD, 3, 11);
    checkChE = new QCheckBox(this);
    layoutPosA->addWidget(checkChE, 2, 12);
    QLabel *labelChE = new QLabel("E");
    layoutPosA->addWidget(labelChE, 3, 12);
    groupPosA->setLayout(layoutPosA);
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS") && !m_swRel.startsWith("__AHU")) groupPosA->setVisible(false);

    //GoupBox switch pos B
    QGroupBox *groupPosB = new QGroupBox("Switch Pos B", this);
    QGridLayout *layoutPosB = new QGridLayout;
    QLabel *label3D_B = new QLabel("3D :");
    layoutPosB->addWidget(label3D_B, 0, 0);
    combo3D_B = new QComboBox(this);
    combo3D_B->addItem("Inverted");
    combo3D_B->addItem("Normal");
    combo3D_B->addItem("2D Left");
    combo3D_B->addItem("2D Right");
    layoutPosB->addWidget(combo3D_B, 0, 1, 1, 12);
    QLabel *labelSync_B = new QLabel("Synchronization :");
    layoutPosB->addWidget(labelSync_B, 1, 0);
    comboSync_B = new QComboBox(this);
    comboSync_B->addItem("IR");
    comboSync_B->addItem("RF");
    layoutPosB->addWidget(comboSync_B, 1, 1, 1, 12);
    QLabel *labelChannel_B = new QLabel("RF Channels :");
    layoutPosB->addWidget(labelChannel_B, 2, 0, 2, 1);
    checkCh3_B = new QCheckBox(this);
    layoutPosB->addWidget(checkCh3_B, 2, 1);
    QLabel *labelCh3_B = new QLabel("3");
    layoutPosB->addWidget(labelCh3_B, 3, 1);
    checkCh4_B = new QCheckBox(this);
    layoutPosB->addWidget(checkCh4_B, 2, 2);
    QLabel *labelCh4_B = new QLabel("4");
    layoutPosB->addWidget(labelCh4_B, 3, 2);
    checkCh5_B = new QCheckBox(this);
    layoutPosB->addWidget(checkCh5_B, 2, 3);
    QLabel *labelCh5_B = new QLabel("5");
    layoutPosB->addWidget(labelCh5_B, 3, 3);
    checkCh6_B = new QCheckBox(this);
    layoutPosB->addWidget(checkCh6_B, 2, 4);
    QLabel *labelCh6_B = new QLabel("6");
    layoutPosB->addWidget(labelCh6_B, 3, 4);
    checkCh7_B = new QCheckBox(this);
    layoutPosB->addWidget(checkCh7_B, 2, 5);
    QLabel *labelCh7_B = new QLabel("7");
    layoutPosB->addWidget(labelCh7_B, 3, 5);
    checkCh8_B = new QCheckBox(this);
    layoutPosB->addWidget(checkCh8_B, 2, 6);
    QLabel *labelCh8_B = new QLabel("8");
    layoutPosB->addWidget(labelCh8_B, 3, 6);
    checkCh9_B = new QCheckBox(this);
    layoutPosB->addWidget(checkCh9_B, 2, 7);
    QLabel *labelCh9_B = new QLabel("9");
    layoutPosB->addWidget(labelCh9_B, 3, 7);
    checkChA_B = new QCheckBox(this);
    layoutPosB->addWidget(checkChA_B, 2, 8);
    QLabel *labelChA_B = new QLabel("A");
    layoutPosB->addWidget(labelChA_B, 3, 8);
    checkChB_B = new QCheckBox(this);
    layoutPosB->addWidget(checkChB_B, 2, 9);
    QLabel *labelChB_B = new QLabel("B");
    layoutPosB->addWidget(labelChB_B, 3, 9);
    checkChC_B = new QCheckBox(this);
    layoutPosB->addWidget(checkChC_B, 2, 10);
    QLabel *labelChC_B = new QLabel("C");
    layoutPosB->addWidget(labelChC_B, 3, 10);
    checkChD_B = new QCheckBox(this);
    layoutPosB->addWidget(checkChD_B, 2, 11);
    QLabel *labelChD_B = new QLabel("D");
    layoutPosB->addWidget(labelChD_B, 3, 11);
    checkChE_B = new QCheckBox(this);
    layoutPosB->addWidget(checkChE_B, 2, 12);
    QLabel *labelChE_B = new QLabel("E");
    layoutPosB->addWidget(labelChE_B, 3, 12);
    groupPosB->setLayout(layoutPosB);
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS") && !m_swRel.startsWith("__AHU")) groupPosB->setVisible(false);

    //GoupBox switch pos C
    QGroupBox *groupPosC = new QGroupBox("Switch Pos C", this);
    QGridLayout *layoutPosC = new QGridLayout;
    QLabel *label3D_C = new QLabel("3D :");
    layoutPosC->addWidget(label3D_C, 0, 0);
    combo3D_C = new QComboBox(this);
    combo3D_C->addItem("Inverted");
    combo3D_C->addItem("Normal");
    combo3D_C->addItem("2D Left");
    combo3D_C->addItem("2D Right");
    layoutPosC->addWidget(combo3D_C, 0, 1, 1, 12);
    QLabel *labelSync_C = new QLabel("Synchronization :");
    layoutPosC->addWidget(labelSync_C, 1, 0);
    comboSync_C = new QComboBox(this);
    comboSync_C->addItem("IR");
    comboSync_C->addItem("RF");
    layoutPosC->addWidget(comboSync_C, 1, 1, 1, 12);
    QLabel *labelChannel_C = new QLabel("RF Channels :");
    layoutPosC->addWidget(labelChannel_C, 2, 0, 2, 1);
    checkCh3_C = new QCheckBox(this);
    layoutPosC->addWidget(checkCh3_C, 2, 1);
    QLabel *labelCh3_C = new QLabel("3");
    layoutPosC->addWidget(labelCh3_C, 3, 1);
    checkCh4_C = new QCheckBox(this);
    layoutPosC->addWidget(checkCh4_C, 2, 2);
    QLabel *labelCh4_C = new QLabel("4");
    layoutPosC->addWidget(labelCh4_C, 3, 2);
    checkCh5_C = new QCheckBox(this);
    layoutPosC->addWidget(checkCh5_C, 2, 3);
    QLabel *labelCh5_C = new QLabel("5");
    layoutPosC->addWidget(labelCh5_C, 3, 3);
    checkCh6_C = new QCheckBox(this);
    layoutPosC->addWidget(checkCh6_C, 2, 4);
    QLabel *labelCh6_C = new QLabel("6");
    layoutPosC->addWidget(labelCh6_C, 3, 4);
    checkCh7_C = new QCheckBox(this);
    layoutPosC->addWidget(checkCh7_C, 2, 5);
    QLabel *labelCh7_C = new QLabel("7");
    layoutPosC->addWidget(labelCh7_C, 3, 5);
    checkCh8_C = new QCheckBox(this);
    layoutPosC->addWidget(checkCh8_C, 2, 6);
    QLabel *labelCh8_C = new QLabel("8");
    layoutPosC->addWidget(labelCh8_C, 3, 6);
    checkCh9_C = new QCheckBox(this);
    layoutPosC->addWidget(checkCh9_C, 2, 7);
    QLabel *labelCh9_C = new QLabel("9");
    layoutPosC->addWidget(labelCh9_C, 3, 7);
    checkChA_C = new QCheckBox(this);
    layoutPosC->addWidget(checkChA_C, 2, 8);
    QLabel *labelChA_C = new QLabel("A");
    layoutPosC->addWidget(labelChA_C, 3, 8);
    checkChB_C = new QCheckBox(this);
    layoutPosC->addWidget(checkChB_C, 2, 9);
    QLabel *labelChB_C = new QLabel("B");
    layoutPosC->addWidget(labelChB_C, 3, 9);
    checkChC_C = new QCheckBox(this);
    layoutPosC->addWidget(checkChC_C, 2, 10);
    QLabel *labelChC_C = new QLabel("C");
    layoutPosC->addWidget(labelChC_C, 3, 10);
    checkChD_C = new QCheckBox(this);
    layoutPosC->addWidget(checkChD_C, 2, 11);
    QLabel *labelChD_C = new QLabel("D");
    layoutPosC->addWidget(labelChD_C, 3, 11);
    checkChE_C = new QCheckBox(this);
    layoutPosC->addWidget(checkChE_C, 2, 12);
    QLabel *labelChE_C = new QLabel("E");
    layoutPosC->addWidget(labelChE_C, 3, 12);
    groupPosC->setLayout(layoutPosC);
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS") && !m_swRel.startsWith("__AHU")) groupPosC->setVisible(false);

    //Slider DarkTime
    QGroupBox *groupeDT = new QGroupBox("DarkTime", this);
    QLabel *contraste = new QLabel("Contrast");
    QLabel *luminosite = new QLabel("Brightness");
    sliderDT = new QSlider(Qt::Horizontal, this);
    sliderDT->setMinimum(-7000);
    sliderDT->setMaximum(7000);
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
    if(m_swRel.startsWith("EDGHS")) groupeDT->setVisible(false);

    //GroupBox detection de mouvement
    QGroupBox *groupMoves = new QGroupBox("Moves Detection", this);
    movesEnabled = new QRadioButton("Enabled");
    movesDisabled = new QRadioButton("Disabled");
    QVBoxLayout *layoutMoves = new QVBoxLayout;
    layoutMoves->addWidget(movesEnabled);
    layoutMoves->addWidget(movesDisabled);
    groupMoves->setLayout(layoutMoves);

    //GroupBox Advanced
    groupAdv = new QGroupBox("Advanced", this);
    //----------/
    QLabel *deltaLabel = new QLabel("Delta Sync (us) :");
    delta = new QSpinBox(this);
    delta->setMaximum(4064);
    delta->setMinimum(-4064);
    delta->setSingleStep(32);
    QHBoxLayout *layoutDelta = new QHBoxLayout;
    layoutDelta->addWidget(deltaLabel);
    layoutDelta->addWidget(delta);
    //----------/
    QGroupBox *groupeFW = new QGroupBox("3D Synchronization", this);
    QLabel *perf = new QLabel("Accuracy");
    QLabel *eco = new QLabel("Energy Saving");
    sliderFW = new QSlider(Qt::Horizontal, this);
    sliderFW->setMinimum(0);
    sliderFW->setMaximum(10);
    sliderFW->setSingleStep(1);
    sliderFW->setMinimumWidth(150);
    sliderFW->setMaximumWidth(150);
    QHBoxLayout *layoutSliderFW = new QHBoxLayout;
    layoutSliderFW->addWidget(perf);
    layoutSliderFW->addWidget(sliderFW);
    layoutSliderFW->addWidget(eco);
    groupeFW->setLayout(layoutSliderFW);
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS")) groupeFW->setEnabled(false);
    //----------/
    QLabel *sensiLabel = new QLabel("Moves sensibility :");
    sensiMove = new QComboBox;
    sensiMove->addItem("High");
    sensiMove->addItem("Medium");
    sensiMove->addItem("Low");
    QHBoxLayout *layoutSensi = new QHBoxLayout;
    layoutSensi->addWidget(sensiLabel);
    layoutSensi->addWidget(sensiMove);
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS") && !m_swRel.startsWith("__AHU")) {sensiLabel->setEnabled(false);sensiMove->setEnabled(false);}
    //----------/
    QLabel *delaiMoveLabel = new QLabel("auto-OFF on moves :");
    delaiMove = new QComboBox;
    delaiMove->addItem("Never");
    delaiMove->addItem("1 minutes");
    delaiMove->addItem("5 minutes");
    delaiMove->addItem("10 minutes");
    delaiMove->addItem("15 minutes");
    QHBoxLayout *layoutdelaiMove = new QHBoxLayout;
    layoutdelaiMove->addWidget(delaiMoveLabel);
    layoutdelaiMove->addWidget(delaiMove);
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS") && !m_swRel.startsWith("__AHU")) {delaiMoveLabel->setEnabled(false);delaiMove->setEnabled(false);}
    //----------/
    QLabel *delaiIrLabel = new QLabel("auto-OFF on Sync :");
    delaiIr = new QComboBox;
    delaiIr->addItem("Never");
    delaiIr->addItem("1 minutes");
    delaiIr->addItem("5 minutes");
    delaiIr->addItem("10 minutes");
    delaiIr->addItem("15 minutes");
    QHBoxLayout *layoutdelaiIr = new QHBoxLayout;
    layoutdelaiIr->addWidget(delaiIrLabel);
    layoutdelaiIr->addWidget(delaiIr);
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS") && !m_swRel.startsWith("__AHU")) {delaiIrLabel->setEnabled(false);delaiIr->setEnabled(false);}
    //----------/
    QLabel *seuilBattLabel = new QLabel("Low level battery :");
    seuilBatt = new QComboBox;
    seuilBatt->addItem("30 minutes");
    seuilBatt->addItem("3 hours");
    seuilBatt->addItem("12 hours");
    QHBoxLayout *layoutseuilBatt = new QHBoxLayout;
    layoutseuilBatt->addWidget(seuilBattLabel);
    layoutseuilBatt->addWidget(seuilBatt);
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS") && !m_swRel.startsWith("__AHU")) {seuilBattLabel->setEnabled(false);seuilBatt->setEnabled(false);}
    //----------/
    QVBoxLayout *layoutAdv = new QVBoxLayout;
    if(m_swRel.startsWith("EDGVR") || m_swRel.startsWith("EDGHS")) layoutAdv->addSpacing(50);
    layoutAdv->addLayout(layoutDelta);
    if(m_swRel.startsWith("EDGVR") || m_swRel.startsWith("EDGHS")) layoutAdv->addSpacing(50);
    layoutAdv->addWidget(groupeFW);
    if(m_swRel.startsWith("EDGVR") || m_swRel.startsWith("EDGHS")) layoutAdv->addSpacing(50);
    layoutAdv->addLayout(layoutSensi);
    layoutAdv->addLayout(layoutdelaiMove);
    layoutAdv->addLayout(layoutdelaiIr);
    layoutAdv->addLayout(layoutseuilBatt);
    if(m_swRel.startsWith("EDGVR") || m_swRel.startsWith("EDGHS")) layoutAdv->addSpacing(150);
    groupAdv->setLayout(layoutAdv);
    groupAdv->setVisible(false);

    //Layout : boutons du bas
    appliquer = new QPushButton("Apply");
    quitter = new QPushButton("Quit");
    more = new QPushButton("More>>");
    QHBoxLayout *boutonsLayout = new QHBoxLayout;
    boutonsLayout->setAlignment(Qt::AlignLeft);
    boutonsLayout->addWidget(appliquer);
    boutonsLayout->addWidget(quitter);
    boutonsLayout->addSpacing(50);
    boutonsLayout->addWidget(more);

    //Fenetre complete
    QVBoxLayout *layoutGauche = new QVBoxLayout;
    if(!m_swRel.startsWith("EDGVR") && !m_swRel.startsWith("EDGHS") && !m_swRel.startsWith("__AHU")) layoutGauche->addWidget(group3D);
    else{
        layoutGauche->addWidget(groupPosA);
        layoutGauche->addWidget(groupPosB);
        layoutGauche->addWidget(groupPosC);
    }
    layoutGauche->addWidget(groupeDT);
    layoutGauche->addWidget(groupMoves);
    QHBoxLayout *layoutGroup = new QHBoxLayout;
    layoutGroup->addLayout(layoutGauche);
    layoutGroup->addWidget(groupAdv);
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layoutGroup);
    layoutPrincipal->addLayout(boutonsLayout);
    layoutPrincipal->setSizeConstraint(QLayout::SetMinimumSize);

    setLayout(layoutPrincipal);
    setWindowTitle("Parameters");

    //Initialisation parametres
    advancedVisible = false;

    //Connections SLOT
    connect(quitter, SIGNAL(clicked()), this, SLOT(accept()));
    connect(appliquer, SIGNAL(clicked()), this, SLOT(on_Apply_triggered()));
    connect(sliderDT, SIGNAL(valueChanged(int)), this, SLOT(displayDT(int)));
    connect(more, SIGNAL(clicked()), this, SLOT(on_More_trigerred()));
    connect(delaiMove, SIGNAL(currentIndexChanged(int)), this, SLOT(delaiMoveChanged(int)));
    connect(movesEnabled, SIGNAL(clicked(bool)), this, SLOT(movesEnabledChanged(bool)));
    connect(movesDisabled, SIGNAL(clicked(bool)), this, SLOT(movesDisabledChanged(bool)));
 }

Parameters::~Parameters()
{
}

void Parameters::movesDisabledChanged(bool active)
{
    if(active) delaiMove->setCurrentIndex(0);
}

void Parameters::movesEnabledChanged(bool active)
{
    if(active) delaiMove->setCurrentIndex(2);
}

void Parameters::delaiMoveChanged(int newValue)
{
    if(newValue) movesEnabled->setChecked(true);
    else movesDisabled->setChecked(true);
}

void Parameters::displayDT(int value)
{
    int temp;

    temp = value - (value%100);
    sliderDT->setValue(temp);
    sliderLabel->setText(QString("%1").arg(temp));
}

void Parameters::displayAll(unsigned char *data)
{
    int DarkTime;

    //Lecture des données sur le device
    m_volt = data[0];
    m_up = data[1];
    m_down = data[2];
    m_delta = data[3];
    for (int i=0;i<15;i++) m_customNbre[i] = data[i+4];
    m_rfPosaLsb = data[19];
    m_rfPosaMsb = data[20];
    m_rfPosbLsb = data[21];
    m_rfPosbMsb = data[22];
    m_rfPoscLsb = data[23];
    m_rfPoscMsb = data[24];
    m_ledMode = data[25];
    m_opticPosa = data[26];
    m_opticPosb = data[27];
    m_opticPosc = data[28];
    m_activMov = data[29];
    m_activSync = data[30];
    m_maxRoueLibre = data[31];
    m_sensiMove = data[32];
    m_lowBatt = data[33];

    // 3D Switch Position
    if(m_opticPosa!=0 && m_opticPosa!=1 && m_opticPosa!=2 && m_opticPosa!=3)
        full3D->setChecked(true); // mode par défaut
    else if(m_opticPosa==1 && m_opticPosb==1 && m_opticPosc==1 )
        full3D->setChecked(true);
    else if(m_opticPosa==0 && m_opticPosb==0 && m_opticPosc==0 )
        full3DReverse->setChecked(true);
    else if(m_opticPosa==1 && m_opticPosb==2 && m_opticPosc==3 )
        sharedScreen->setChecked(true);
    else
    {
        full3D->setChecked(false);
        sharedScreen->setChecked(false);
    }

    if(m_opticPosa!=0 && m_opticPosa!=1 && m_opticPosa!=2 && m_opticPosa!=3)
        combo3D->setCurrentIndex(1);
    else combo3D->setCurrentIndex(m_opticPosa);
    if(m_opticPosb!=0 && m_opticPosb!=1 && m_opticPosb!=2 && m_opticPosb!=3)
        combo3D_B->setCurrentIndex(1);
    else combo3D_B->setCurrentIndex(m_opticPosb);
    if(m_opticPosc!=0 && m_opticPosc!=1 && m_opticPosc!=2 && m_opticPosc!=3)
        combo3D_C->setCurrentIndex(1);
    else combo3D_C->setCurrentIndex(m_opticPosc);

    // Switch Pos A
    if( (m_rfPosaMsb&0x80) == 0)
    {   // IR
           comboSync->setCurrentIndex(0);
           checkCh3->setChecked(false);
           checkCh4->setChecked(false);
           checkCh5->setChecked(false);
           checkCh6->setChecked(false);
           checkCh7->setChecked(false);
           checkCh8->setChecked(false);
           checkCh9->setChecked(false);
           checkChA->setChecked(false);
           checkChB->setChecked(false);
           checkChC->setChecked(false);
           checkChD->setChecked(false);
           checkChE->setChecked(false);
    }
    else
    { // RF
        comboSync->setCurrentIndex(1);
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

    // Switch Pos B
    if( (m_rfPosbMsb&0x80) == 0)
    {   // IR
           comboSync_B->setCurrentIndex(0);
           checkCh3_B->setChecked(false);
           checkCh4_B->setChecked(false);
           checkCh5_B->setChecked(false);
           checkCh6_B->setChecked(false);
           checkCh7_B->setChecked(false);
           checkCh8_B->setChecked(false);
           checkCh9_B->setChecked(false);
           checkChA_B->setChecked(false);
           checkChB_B->setChecked(false);
           checkChC_B->setChecked(false);
           checkChD_B->setChecked(false);
           checkChE_B->setChecked(false);
    }
    else
    { // RF
        comboSync_B->setCurrentIndex(1);
        if(m_rfPosbLsb&0x01) checkCh3_B->setChecked(true);
        if(m_rfPosbLsb&0x02) checkCh4_B->setChecked(true);
        if(m_rfPosbLsb&0x04) checkCh5_B->setChecked(true);
        if(m_rfPosbLsb&0x08) checkCh6_B->setChecked(true);
        if(m_rfPosbLsb&0x10) checkCh7_B->setChecked(true);
        if(m_rfPosbLsb&0x20) checkCh8_B->setChecked(true);
        if(m_rfPosbLsb&0x40) checkCh9_B->setChecked(true);
        if(m_rfPosbLsb&0x80) checkChA_B->setChecked(true);
        if(m_rfPosbMsb&0x01) checkChB_B->setChecked(true);
        if(m_rfPosbMsb&0x02) checkChC_B->setChecked(true);
        if(m_rfPosbMsb&0x04) checkChD_B->setChecked(true);
        if(m_rfPosbMsb&0x08) checkChE_B->setChecked(true);
    }

    // Switch Pos C
    if( (m_rfPoscMsb&0x80) == 0)
    {   // IR
           comboSync_C->setCurrentIndex(0);
           checkCh3_C->setChecked(false);
           checkCh4_C->setChecked(false);
           checkCh5_C->setChecked(false);
           checkCh6_C->setChecked(false);
           checkCh7_C->setChecked(false);
           checkCh8_C->setChecked(false);
           checkCh9_C->setChecked(false);
           checkChA_C->setChecked(false);
           checkChB_C->setChecked(false);
           checkChC_C->setChecked(false);
           checkChD_C->setChecked(false);
           checkChE_C->setChecked(false);
    }
    else
    { // RF
        comboSync_C->setCurrentIndex(1);
        if(m_rfPoscLsb&0x01) checkCh3_C->setChecked(true);
        if(m_rfPoscLsb&0x02) checkCh4_C->setChecked(true);
        if(m_rfPoscLsb&0x04) checkCh5_C->setChecked(true);
        if(m_rfPoscLsb&0x08) checkCh6_C->setChecked(true);
        if(m_rfPoscLsb&0x10) checkCh7_C->setChecked(true);
        if(m_rfPoscLsb&0x20) checkCh8_C->setChecked(true);
        if(m_rfPoscLsb&0x40) checkCh9_C->setChecked(true);
        if(m_rfPoscLsb&0x80) checkChA_C->setChecked(true);
        if(m_rfPoscMsb&0x01) checkChB_C->setChecked(true);
        if(m_rfPoscMsb&0x02) checkChC_C->setChecked(true);
        if(m_rfPoscMsb&0x04) checkChD_C->setChecked(true);
        if(m_rfPoscMsb&0x08) checkChE_C->setChecked(true);
    }

    // Slider DarkTime
    if(m_up == -1) DarkTime = 0;
//Modif AT    else DarkTime = - QString::number((char)m_up*16).toInt();
        else DarkTime = - QString::number((char)m_up*32).toInt();
//Modif AT    if(m_down != -1) DarkTime -= QString::number((char)m_down*16).toInt();
        if(m_down != -1) DarkTime -= QString::number((char)m_down*32).toInt();
    sliderDT->setValue(DarkTime);
    sliderLabel->setText(QString("%1").arg(DarkTime));

    // Detection de mouvement
    if( (m_activMov >= 12) && (m_activMov <= 15) ) {movesEnabled->setChecked(true); delaiMove->setCurrentIndex(m_activMov-11);}
    else { movesDisabled->setChecked(true); delaiMove->setCurrentIndex(0);}

    // Delta Sync
    if(m_delta == -1) m_delta = 0;
    delta->setValue(m_delta*32);

    // Roue Libre
    if( (m_maxRoueLibre>=0) && (m_maxRoueLibre<=9) ) sliderFW->setValue(m_maxRoueLibre);
    else sliderFW->setValue(5);

    // Moves sensibility
    if (m_sensiMove==0x04) sensiMove->setCurrentIndex(1);
    else if (m_sensiMove==0x02) sensiMove->setCurrentIndex(0);
    else sensiMove->setCurrentIndex(2);

    // Auto-OFF on Sync
    if( (m_activSync>=12) && (m_activSync<=15) ) delaiIr->setCurrentIndex(m_activSync-11);
    else if (m_activSync==0) delaiIr->setCurrentIndex(0);
    else delaiIr->setCurrentIndex(2);

    // Seuil low batt
    if(m_lowBatt == 0x38)
        seuilBatt->setCurrentIndex(0);
    else if(m_lowBatt == 0x3F)
        seuilBatt->setCurrentIndex(2);
    else
        seuilBatt->setCurrentIndex(1);
}

void Parameters::on_Apply_triggered()
{
    unsigned char data[64];

    data[0] = m_volt;

    //Slider DarkTime
//Modif AT    data[1] = -sliderDT->value()/2/16;
    data[1] = -sliderDT->value()/2/32;
//Modif AT    data[2] = -sliderDT->value()/16 - data[1];
    data[2] = -sliderDT->value()/32 - data[1];

    //Delta Sync
    m_delta = (delta->value())/32;
    data[3] = m_delta;

    //Customer Number
    for(int i=0; i<15; i++) data[i+4] = m_customNbre[i];

    // 3D Switch Position
    if(!m_swRel.startsWith("EDGVR")){
        if(sharedScreen->isChecked())
        {
            data[26] = 1;
            data[27] = 2;
            data[28] = 3;
        }
        else if(full3D->isChecked())
        {
            data[26] = 1;
            data[27] = 1;
            data[28] = 1;
        }
        else
        {
            data[26] = 0;
            data[27] = 0;
            data[28] = 0;
        }
    }
    else {
        data[26] = combo3D->currentIndex();
        data[27] = combo3D_B->currentIndex();
        data[28] = combo3D_C->currentIndex();
    }
    // EDGE VR 3D Switch Position
    if(comboSync->currentIndex()==0){ //IR
        m_rfPosaMsb &= 0x7F;
    }
    else {
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
    }
    data[19] = m_rfPosaLsb;
    data[20] = m_rfPosaMsb;
    if(comboSync_B->currentIndex()==0){ //IR
        m_rfPosbMsb &= 0x7F;
    }
    else {
        m_rfPosbLsb = 0;
        if(checkCh3_B->isChecked()) m_rfPosbLsb += 0x01;
        if(checkCh4_B->isChecked()) m_rfPosbLsb += 0x02;
        if(checkCh5_B->isChecked()) m_rfPosbLsb += 0x04;
        if(checkCh6_B->isChecked()) m_rfPosbLsb += 0x08;
        if(checkCh7_B->isChecked()) m_rfPosbLsb += 0x10;
        if(checkCh8_B->isChecked()) m_rfPosbLsb += 0x20;
        if(checkCh9_B->isChecked()) m_rfPosbLsb += 0x40;
        if(checkChA_B->isChecked()) m_rfPosbLsb += 0x80;
         m_rfPosbMsb = 0x80;
        if(checkChB_B->isChecked()) m_rfPosbMsb += 0x01;
        if(checkChC_B->isChecked()) m_rfPosbMsb += 0x02;
        if(checkChD_B->isChecked()) m_rfPosbMsb += 0x04;
        if(checkChE_B->isChecked()) m_rfPosbMsb += 0x08;
    }
    data[21] = m_rfPosbLsb;
    data[22] = m_rfPosbMsb;
    if(comboSync_C->currentIndex()==0){ //IR
        m_rfPoscMsb &= 0x7F;
    }
    else {
        m_rfPoscLsb = 0;
        if(checkCh3_C->isChecked()) m_rfPoscLsb += 0x01;
        if(checkCh4_C->isChecked()) m_rfPoscLsb += 0x02;
        if(checkCh5_C->isChecked()) m_rfPoscLsb += 0x04;
        if(checkCh6_C->isChecked()) m_rfPoscLsb += 0x08;
        if(checkCh7_C->isChecked()) m_rfPoscLsb += 0x10;
        if(checkCh8_C->isChecked()) m_rfPoscLsb += 0x20;
        if(checkCh9_C->isChecked()) m_rfPoscLsb += 0x40;
        if(checkChA_C->isChecked()) m_rfPoscLsb += 0x80;
        m_rfPoscMsb = 0x80;
        if(checkChB_C->isChecked()) m_rfPoscMsb += 0x01;
        if(checkChC_C->isChecked()) m_rfPoscMsb += 0x02;
        if(checkChD_C->isChecked()) m_rfPoscMsb += 0x04;
        if(checkChE_C->isChecked()) m_rfPoscMsb += 0x08;
    }
    data[23] = m_rfPoscLsb;
    data[24] = m_rfPoscMsb;
    data[25] = m_ledMode;

    //Detection de mouvement
    if(movesEnabled->isChecked()) {
        if(delaiMove->currentIndex() != 0) data[29] = delaiMove->currentIndex() + 11;
        else data[29] = 13;  //5 minutes par défaut si Enable
    }
    else if( (delaiMove->currentIndex() != 0) && (delaiMove->isEnabled()==true) ) data[29] = delaiMove->currentIndex() + 11;
    else data[29] = 0;

    //Sensibilité de la détection de mouvement
    if(sensiMove->currentIndex()==0) data[32] = 0x02;
    else if(sensiMove->currentIndex()==1) data[32] = 0x04;
    else data[32] = 0x08;

    //Delai avant mise off sur absence de signal
    if(delaiIr->currentIndex() != 0) data[30] = delaiIr->currentIndex() + 11;
    else data[30] = 0;

    data[31] = sliderFW->value();

    if(seuilBatt->currentIndex() == 0)
        data[33] = 0x38;
    else if(seuilBatt->currentIndex() == 2)
        data[33] = 0x3F;
    else
        data[33] = 0x3A;

    emit apply_clicked(firstParam, sizeParam, data);
}

void Parameters::on_More_trigerred()
{
    groupAdv->setVisible(true);
    more->setVisible(false);
}
