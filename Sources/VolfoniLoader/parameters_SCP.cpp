#include "parameters_SCP.h"

Parameters_SCP::Parameters_SCP(QWidget *parent, QString firmRev) : QDialog(parent)
{
    m_swRel = firmRev;

    //SafeLock
    QHBoxLayout *layoutSafe = new QHBoxLayout;
    QLabel *labelSafe = new QLabel("SafeLock:");
    layoutSafe->addWidget(labelSafe);
    comboSafe = new QComboBox(this);
    comboSafe->setMaximumWidth(180);
    comboSafe->addItem("Disabled");
    comboSafe->addItem("Enabled");
    layoutSafe->addWidget(comboSafe);

    //AutoMode
    QHBoxLayout *layoutAuto = new QHBoxLayout;
    QLabel *labelAuto = new QLabel("AutoMode:");
    layoutAuto->addWidget(labelAuto);
    comboAuto = new QComboBox(this);
    comboAuto->setMaximumWidth(180);
    comboAuto->addItem("Disabled");
    comboAuto->addItem("Enabled");
    layoutAuto->addWidget(comboAuto);

    //Mode 3D
    QHBoxLayout *layoutSync = new QHBoxLayout;
    QLabel *label3D = new QLabel("3D Synchronization:");
    layoutSync->addWidget(label3D);
    combo3D = new QComboBox(this);
    combo3D->setMaximumWidth(180);
    combo3D->addItem("Inverted");
    combo3D->addItem("Normal");
    layoutSync->addWidget(combo3D);

    //Delta Sync
    QLabel *deltaLabel = new QLabel("Delta Sync (us) :");
    delta = new QSpinBox(this);
    delta->setMaximum(4000);
    delta->setMinimum(-4000);
    delta->setSingleStep(100);
    QHBoxLayout *layoutDelta = new QHBoxLayout;
    layoutDelta->addWidget(deltaLabel);
    layoutDelta->addWidget(delta);

    //UP Timing
    QLabel *upLabel = new QLabel("LEFT Eye Delay (us) :");
    upTime = new QSpinBox(this);
    upTime->setMaximum(4000);
    upTime->setMinimum(-4000);
    upTime->setSingleStep(100);
    QHBoxLayout *layoutUp = new QHBoxLayout;
    layoutUp->addWidget(upLabel);
    layoutUp->addWidget(upTime);

    //DOWN Timing
    QLabel *downLabel = new QLabel("RIGHT Eye Delay (us) :");
    downTime = new QSpinBox(this);
    downTime->setMaximum(4000);
    downTime->setMinimum(-4000);
    downTime->setSingleStep(100);
    QHBoxLayout *layoutDown = new QHBoxLayout;
    layoutDown->addWidget(downLabel);
    layoutDown->addWidget(downTime);

    //LCD Voltage
    QLabel *voltageLabel = new QLabel("LCD Voltage (V) :");
    voltage = new QSpinBox(this);
    voltage->setMaximum(25);
    voltage->setMinimum(4);
    voltage->setSingleStep(1);
    QHBoxLayout *layoutVoltage = new QHBoxLayout;
    layoutVoltage->addWidget(voltageLabel);
    layoutVoltage->addWidget(voltage);

    //Layout : boutons du bas
    appliquer = new QPushButton("Apply");
    quitter = new QPushButton("Quit");
    QHBoxLayout *boutonsLayout = new QHBoxLayout;
    boutonsLayout->setAlignment(Qt::AlignCenter);
    boutonsLayout->addWidget(appliquer);
    boutonsLayout->addWidget(quitter);

    //Fenetre complete
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layoutVoltage);
    layoutPrincipal->addLayout(layoutUp);
    layoutPrincipal->addLayout(layoutDown);
    layoutPrincipal->addLayout(layoutDelta);
    layoutPrincipal->addLayout(layoutSync);
    layoutPrincipal->addLayout(layoutAuto);
    layoutPrincipal->addLayout(layoutSafe);
    layoutPrincipal->addSpacing(30);
    layoutPrincipal->addLayout(boutonsLayout);
 //   layoutPrincipal->setSizeConstraint(QLayout::SetMaximumSize);

    setLayout(layoutPrincipal);
    setWindowTitle("SCP Parameters");
    setFixedSize(300, 300);

    //Connections SLOT
    connect(quitter, SIGNAL(clicked()), this, SLOT(accept()));
    connect(appliquer, SIGNAL(clicked()), this, SLOT(on_Apply_triggered()));
 }

Parameters_SCP::~Parameters_SCP()
{
}

void Parameters_SCP::displayAll(unsigned char *data)
{
    //Lecture des données sur le device
    m_volt = data[0];
    m_up = (short) (data[1]*0x100 + data[2]);
    m_down = (short) (data[3]*0x100 + data[4]);
    m_delta = (short) (data[5]*0x100 + data[6]);
    m_openSh = (short) (data[7]*0x100 + data[8]);
    m_deltaSh = (short) (data[9]*0x100 + data[10]);
    m_3Dmode = data[11];
    m_offPeriod = (short) (data[12]*0x100 + data[13]);
    m_automode = data[14];
    m_safelock = data[15];
    if(m_automode>1)m_automode=1;
    if(m_safelock>1)m_safelock=1;

    // LCD Voltage
    if (!m_swRel.startsWith("__SCP")) voltage->setEnabled(false);
    voltage->setValue(m_volt);

    // UP Timing
    upTime->setValue(m_up);

    // DOWN Timing
    downTime->setValue(m_down);

    // Delta Sync
    delta->setValue(m_delta);

    // Mode 3D
    combo3D->setCurrentIndex(m_3Dmode);

    // AutoMode
    if (m_swRel.startsWith("__SCP")) comboAuto->setEnabled(false);
    comboAuto->setCurrentIndex(m_automode);

    // SafeLock
    if (!m_swRel.startsWith("GESPA_V02") && !m_swRel.startsWith("GESPA_V03")) comboSafe->setEnabled(false);
    comboSafe->setCurrentIndex(m_safelock);

}

void Parameters_SCP::on_Apply_triggered()
{
    unsigned char data[64];

    data[0] = voltage->value();
    data[1] = (unsigned char) (upTime->value() >> 8);
    data[2] = (unsigned char) (upTime->value());
    data[3] = (unsigned char) (downTime->value() >> 8);
    data[4] = (unsigned char) (downTime->value());
    data[5] = (unsigned char) (delta->value() >> 8);
    data[6] = (unsigned char) (delta->value());
    data[7] = (unsigned char) (m_openSh >> 8);
    data[8] = (unsigned char) (m_openSh);
    data[9] = (unsigned char) (m_deltaSh >> 8);
    data[10] = (unsigned char) (m_deltaSh);
    data[11] = combo3D->currentIndex();
    data[12] = (unsigned char) (m_offPeriod >> 8);
    data[13] = (unsigned char) (m_offPeriod);
    data[14] = comboAuto->currentIndex();
    data[15] = comboSafe->currentIndex();

    emit apply_clicked(data);
}
