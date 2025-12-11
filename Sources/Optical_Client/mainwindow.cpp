#include "mainwindow.h"

MainWindow::MainWindow()
{
    setWindowTitle("ActivEyes Optical Client V01");

    //Premiere ligne : logo + statut connexion
    QHBoxLayout *firstLineLayout = new QHBoxLayout;
    logo = new QLabel;
    logo->setPixmap(QPixmap(":/MainWindow/ressources/logo_volfoni.png"));
//    isConnected = new QLabel("<font color=\"#FF0000\"><b>Product is not connected</b></font>");
    firstLineLayout->setAlignment(Qt::AlignCenter);
    firstLineLayout->addWidget(logo);
//    firstLineLayout->addWidget(isConnected);

    //Deuxieme ligne : zone d'info
    summary = new QTextEdit;
    summary->setEnabled(false);
    summary->setVisible(false);
    summary->setReadOnly(true);

    //Troisieme ligne : nom du fichier + bouton open
    QHBoxLayout *secondLineLayout = new QHBoxLayout;
    filePath = new QLineEdit("Select a file");
    open = new QPushButton;
    open->setIcon(QIcon(":/MainWindow/ressources/open.png"));
    secondLineLayout->addWidget(filePath);
    secondLineLayout->addWidget(open);

    //Quatrieme ligne : boutons
    QHBoxLayout *thirdLineLayout = new QHBoxLayout;
    program = new QPushButton("Program");
    quit = new QPushButton("Quit");
    thirdLineLayout->setAlignment(Qt::AlignRight);
    thirdLineLayout->addWidget(program);
    thirdLineLayout->addWidget(quit);

    //Layout global
    QVBoxLayout *fenetreLayout = new QVBoxLayout;
    fenetreLayout->addLayout(firstLineLayout);
    fenetreLayout->addWidget(summary);
    fenetreLayout->addLayout(secondLineLayout);
    fenetreLayout->addLayout(thirdLineLayout);

    setLayout(fenetreLayout);

    //CONNECTIONS
    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(program, SIGNAL(clicked()), this, SLOT(programGlasses()));
    connect(open, SIGNAL(clicked()), this, SLOT(openPreset()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::openPreset()
{
    int crcValue = 0;
    double dTemp;
    QString nomfichier = QFileDialog::getOpenFileName(this, "Open", QDir::rootPath(), "Optical Presets(*.opt)");

    QFile file(nomfichier);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Invalid presets file !");
        return ;
    }

    filePath->setText(file.fileName());

    char* pdata = NULL;
    pdata = (char*)malloc(49);

    QDataStream flux(&file);
    flux.readRawData(pdata,49);
    file.close();

    //check file
    if(pdata[0]!='v' || pdata[1]!='o' || pdata[2]!='l' || pdata[3]!='f' || pdata[4]!='o' || pdata[5]!='n' || pdata[6]!='i')
    {
        QMessageBox::critical(this, "Error", "Invalid presets file !");
        return ;
    }
    for(int i=0; i<47; i++) crcValue += pdata[i];
    if( pdata[48] != (char) crcValue )
    {
        QMessageBox::critical(this, "Error", "Invalid presets file !");
        return ;
    }

    mode=pdata[7];
    oeil=pdata[8];
    freq1=pdata[9];
    freq2=pdata[10];
    freq3=pdata[11];
    freq4=pdata[12];
    freq5=pdata[13];
    freq6=pdata[14];
    freq7=pdata[15];
    freq8=pdata[16];
    timing=pdata[17];
    sequences=pdata[18];

    mode_2=pdata[19];
    oeil_2=pdata[20];
    freq1_2=pdata[21];
    freq2_2=pdata[22];
    freq3_2=pdata[23];
    freq4_2=pdata[24];
    freq5_2=pdata[25];
    freq6_2=pdata[26];
    freq7_2=pdata[27];
    freq8_2=pdata[28];
    timing_2=pdata[29];
    sequences_2=pdata[30];

    dTemp = pdata[31]<<8;
    dTemp += (unsigned char) pdata[32];
    setFreq1=dTemp/10;
    dTemp = pdata[33]<<8;
    dTemp += (unsigned char) pdata[34];
    setFreq2=dTemp/10;
    dTemp = pdata[35]<<8;
    dTemp += (unsigned char) pdata[36];
    setFreq3=dTemp/10;
    dTemp = pdata[37]<<8;
    dTemp += (unsigned char) pdata[38];
    setFreq4=dTemp/10;
    dTemp = pdata[39]<<8;
    dTemp += (unsigned char) pdata[40];
    setFreq5=dTemp/10;
    dTemp = pdata[41]<<8;
    dTemp += (unsigned char) pdata[42];
    setFreq6=dTemp/10;
    dTemp = pdata[43]<<8;
    dTemp += (unsigned char) pdata[44];
    setFreq7=dTemp/10;
    dTemp = pdata[45]<<8;
    dTemp += (unsigned char) pdata[46];
    setFreq8=dTemp/10;

    dutyCycle = pdata[47];

    summarize();
 }

void MainWindow::programProduct()
{
    Comm::ErrorCode result;
    unsigned char *param;
    int status = 0;
    product = new Product();

    //Connexion à l'AME
    qDebug("start the programming...");
    if( !product->isConnected() )
    {
        qDebug("product not connected, try to connect...");
        status = product->connection();
    }
    if(status!=9)
    {
        QMessageBox::critical(this, "Error", "Impossible to communicate with the ActivEyes.");
        return ;
    }

    //Mise à jour des variables microcontroleur
    qDebug("product connected, try to get informations...");
    product->getQuery();

    //Preparation des donnees a transmettre
    param = (unsigned char*)malloc(43);
    param[0] = m_modeGauche;
    param[1] = m_modeDroit;
    param[2] = m_sequences;
    param[3] = m_timing;
    param[19] = ((int) (0x3D00 / setFreq1))>>8;
    param[20] = 0x3D00 / setFreq1;
    param[21] = ((int) (0x3D00 / setFreq2))>>8;
    param[22] = 0x3D00 / setFreq2;
    param[23] = ((int) (0x3D00 / setFreq3))>>8;
    param[24] = 0x3D00 / setFreq3;
    param[25] = ((int) (0x3D00 / setFreq4))>>8;
    param[26] = 0x3D00 / setFreq4;
    param[27] = ((int) (0x3D00 / setFreq5))>>8;
    param[28] = 0x3D00 / setFreq5;
    param[29] = ((int) (0x3D00 / setFreq6))>>8;
    param[30] = 0x3D00 / setFreq6;
    param[31] = ((int) (0x3D00 / setFreq7))>>8;
    param[32] = 0x3D00 / setFreq7;
    param[33] = ((int) (0x3D00 / setFreq8))>>8;
    param[34] = 0x3D00 / setFreq8;
    param[35] = m_frequence;
    param[36] = m_modeGauche_2;
    param[37] = m_modeDroit_2;
    param[38] = m_sequences_2;
    param[39] = m_timing_2;
    param[40] = m_frequence_2;
    param[41] = m_dutyCycle;
    param[42] = m_dutyCycle_2;

    //Programmation
    qDebug("programming...");
    result = product->programParamFlash(param);
    if(result!=Comm::Success)
        QMessageBox::critical(this, "Error", "Error during ActivEyes programamtion.");
    else
        QMessageBox::information(this, "ActivEyes", "Programming successful.");

    free(param);
    product->close();
    delete(product);
}

void MainWindow::summarize()
{
    QString duree, steps, freq, duty;

    //TO DO : faire le résumé de la prog
    summary->clear();
    summary->append("Button 1 :");
    switch (mode)
    {
    case 0:
        summary->append("No operating mode selected");
        break;
    case 1:
        summary->append("Opening/Closing both eyes at the same time");
        break;
    case 2:
        summary->append("Opening/Closing eyes alternatively");
        break;
    case 3:
        if(oeil==1)
            summary->append("Keep the left eye closed while the right one is opening/closing");
        else if(oeil==2)
            summary->append("Keep the right eye closed while the left one is opening/closing");
        else
            summary->append("Incoherent operating mode");
        break;
    case 4:
        if( (timing==0) || (sequences==0) )
        {
            summary->append("Incoherent operating mode");
            break;
        }
        if(timing==1) duree="10 seconds";
        else if(timing==2) duree="20 seconds";
        else if(timing==3) duree="30 seconds";
        else if(timing==4) duree="40 seconds";
        else if(timing==5) duree="50 seconds";
        else if(timing==6) duree="1 minute";
        else if(timing==7) duree="2 minutes";
        if(sequences==1) steps = "1 step";
        else if(sequences==2) steps = "2 steps";
        else if(sequences==3) steps = "3 steps";
        else if(sequences==4) steps = "4 steps";
        else if(sequences==5) steps = "5 steps";
        else if(sequences==6) steps = "6 steps";
        summary->append("Glasses get from OPEN to CLOSED in " + duree + " and in " + steps);
        break;
    }
    if( (mode==1) || (mode==2) || (mode==3) )
    {
        freq = "";
        if(freq1) freq += QString::number(setFreq1) + " Hz; ";
        if(freq2) freq += QString::number(setFreq2) + " Hz; ";
        if(freq3) freq += QString::number(setFreq3) + " Hz; ";
        if(freq4) freq += QString::number(setFreq4) + " Hz; ";
        if(freq5) freq += QString::number(setFreq5) + " Hz; ";
        if(freq6) freq += QString::number(setFreq6) + " Hz; ";
        if(freq7) freq += QString::number(setFreq7) + " Hz; ";
        if(freq8) freq += QString::number(setFreq8) + " Hz; ";
        summary->append("Used frequencies : " + freq);
        duty = QString::number(dutyCycle) + " %";
        summary->append("Duty Cycle = " + duty);
    }

    summary->append("\nButton 2 :");
    switch (mode_2)
    {
    case 0:
        summary->append("Identical to the Button 1");
        break;
    case 1:
        summary->append("Opening/Closing both eyes at the same time");
        break;
    case 2:
        summary->append("Opening/Closing both eyes alternatively");
        break;
    case 3:
        if(oeil_2==1)
            summary->append("Keep the left eye closed while the right one is opening/closing");
        else if(oeil_2==2)
            summary->append("Keep the right eye closed while the left one is opening/closing");
        else
            summary->append("Incoherent operating mode");
        break;
    case 4:
        if( (timing_2==0) || (sequences_2==0) )
        {
            summary->append("Incoherent operating mode");
            break;
        }
        if(timing_2==1) duree="10 seconds";
        else if(timing_2==2) duree="20 seconds";
        else if(timing_2==3) duree="30 seconds";
        else if(timing_2==4) duree="40 seconds";
        else if(timing_2==5) duree="50 seconds";
        else if(timing_2==6) duree="1 minute";
        else if(timing_2==7) duree="2 minutes";
        if(sequences_2==1) steps = "1 step";
        else if(sequences_2==2) steps = "2 steps";
        else if(sequences_2==3) steps = "3 steps";
        else if(sequences_2==4) steps = "4 steps";
        else if(sequences_2==5) steps = "5 steps";
        else if(sequences_2==6) steps = "6 steps";
        summary->append("Glasses get from OPEN to CLOSED in " + duree + " and in " + steps);
        break;
    }
    if( (mode_2==1) || (mode_2==2) || (mode_2==3) )
    {
        freq = "";
        if(freq1_2) freq += QString::number(setFreq1) + " Hz; ";
        if(freq2_2) freq += QString::number(setFreq2) + " Hz; ";
        if(freq3_2) freq += QString::number(setFreq3) + " Hz; ";
        if(freq4_2) freq += QString::number(setFreq4) + " Hz; ";
        if(freq5_2) freq += QString::number(setFreq5) + " Hz; ";
        if(freq6_2) freq += QString::number(setFreq6) + " Hz; ";
        if(freq7_2) freq += QString::number(setFreq7) + " Hz; ";
        if(freq8_2) freq += QString::number(setFreq8) + " Hz; ";
        summary->append("Used frequencies : " + freq);
        duty = QString::number(dutyCycle) + " %";
        summary->append("Duty Cycle = " + duty);
    }

    summary->append("\n\nClick the \"Program\" button to validate !");
    summary->setVisible(true);
    summary->setEnabled(true);
    summary->setTextBackgroundColor(QColor(255,255,255));
    this->resize(300,330);
}

void MainWindow::programGlasses()
{
    switch(mode)
    {
    case 1:     //Open close same time
        m_modeGauche = C_MODE_FREQ;
        m_modeDroit = C_MODE_FREQ;
        m_frequence = getFrequences();
        if(m_frequence == 0)
        {
            QMessageBox::critical(this, "Error", "You must select one or more frequencies for button 1");
            return ;
        }
        m_timing = 0;
        m_sequences = 0;
        break;
    case 2:     //Open close alternatively
        m_modeGauche = C_MODE_FREQ;
        m_modeDroit = C_MODE_DECAL;
        m_frequence = getFrequences();
        if(m_frequence == 0)
        {
            QMessageBox::critical(this, "Error", "You must select one or more frequencies for button 1");
            return ;
        }
        m_timing = 0;
        m_sequences = 0;
        break;
    case 3:     //one closed, other opening closing
        if (oeil == 1)
        {
            m_modeGauche = C_MODE_BLOQUE;
            m_modeDroit = C_MODE_FREQ;
        }
        else if (oeil == 2)
        {
            m_modeGauche = C_MODE_FREQ;
            m_modeDroit = C_MODE_BLOQUE;
        }
        else
        {
            QMessageBox::critical(this, "Unknown parameter", "Invalid closed eye for button 1 !");
            return;
        }
        m_frequence = getFrequences();
        if(m_frequence == 0)
        {
            QMessageBox::critical(this, "Error", "You must select one or more frequencies for button 1");
            return ;
        }
        m_timing = 0;
        m_sequences = 0;
        break;
    case 4:
        m_modeGauche = C_MODE_BLOQUE;
        m_modeDroit = C_MODE_BLOQUE;
        m_frequence = 0;
        m_timing = getTiming();
        if(m_timing == 0)
        {
            QMessageBox::critical(this, "Error", "Invalid sequence timing for button 1 !");
            return ;
        }
        m_sequences = getSequences();
        if(m_sequences == 0)
        {
            QMessageBox::critical(this, "Error", "Invalid sequence for button 1 !");
            return ;
        }
        break;
    default :
        QMessageBox::critical(this, "Error", "Invalid operating mode for button 1!");
        return;
        break;
    }

    switch(mode_2)
    {
    case 0:     //Identical to button 1
        m_modeGauche_2 = m_modeGauche;
        m_modeDroit_2 = m_modeDroit;
        m_frequence_2 = m_frequence;
        m_timing_2 = m_timing;
        m_sequences_2 = m_sequences;
        break;
    case 1:     //Open close same time
        m_modeGauche_2 = C_MODE_FREQ;
        m_modeDroit_2 = C_MODE_FREQ;
        m_frequence_2 = getFrequences_2();
        if(m_frequence_2 == 0)
        {
            QMessageBox::critical(this, "Error", "You must select one or more frequencies for button 2");
            return ;
        }
        m_timing_2 = 0;
        m_sequences_2 = 0;
        break;
    case 2:     //Open close alternatively
        m_modeGauche_2 = C_MODE_FREQ;
        m_modeDroit_2 = C_MODE_DECAL;
        m_frequence_2 = getFrequences_2();
        if(m_frequence_2 == 0)
        {
            QMessageBox::critical(this, "Error", "You must select one or more frequencies for button 2");
            return ;
        }
        m_timing_2 = 0;
        m_sequences_2 = 0;
        break;
    case 3:     //one closed, other opening closing
        if (oeil_2 == 1)
        {
            m_modeGauche_2 = C_MODE_BLOQUE;
            m_modeDroit_2 = C_MODE_FREQ;
        }
        else if (oeil_2 == 2)
        {
            m_modeGauche_2 = C_MODE_FREQ;
            m_modeDroit_2 = C_MODE_BLOQUE;
        }
        else
        {
            QMessageBox::critical(this, "Unknown parameter", "Invalid closed eye for button 2!");
            return;
        }
        m_frequence_2 = getFrequences_2();
        if(m_frequence_2 == 0)
        {
            QMessageBox::critical(this, "Error", "You must select one or more frequencies for button 2");
            return ;
        }
        m_timing_2 = 0;
        m_sequences_2 = 0;
        break;
    case 4:
        m_modeGauche_2 = C_MODE_BLOQUE;
        m_modeDroit_2 = C_MODE_BLOQUE;
        m_frequence_2 = 0;
        m_timing_2 = getTiming_2();
        if(m_timing_2 == 0)
        {
            QMessageBox::critical(this, "Error", "Invalid sequence timing for button 2!");
            return ;
        }
        m_sequences_2 = getSequences_2();
        if(m_sequences_2 == 0)
        {
            QMessageBox::critical(this, "Error", "Invalid sequence for button 2!");
            return ;
        }
        break;
    default :
        QMessageBox::critical(this, "Error", "Invalid operating mode for button 2!");
        return;
        break;
    }

    m_dutyCycle = dutyCycle;
    m_dutyCycle_2 = dutyCycle;

    qDebug("m_modeGauche = %d", m_modeGauche);
    qDebug("m_modeDroit = %d", m_modeDroit);
    qDebug("m_frequence = 0x%x", m_frequence);
    qDebug("m_timing = %d", m_timing);
    qDebug("m_sequences = 0x%x", m_sequences);
    qDebug("m_modeGauche_2 = %d", m_modeGauche_2);
    qDebug("m_modeDroit_2 = %d", m_modeDroit_2);
    qDebug("m_frequence_2 = 0x%x", m_frequence_2);
    qDebug("m_timing_2 = %d", m_timing_2);
    qDebug("m_sequences_2 = 0x%x", m_sequences_2);
    qDebug("m_dutyCycle = %d", m_dutyCycle);
    qDebug("m_dutyCycle_2 = %d", m_dutyCycle_2);

    //Sequence de programmation de l'AME
    programProduct();
}

char MainWindow::getFrequences()
{
    char result=0x00;

    if(freq1) result += 0x01;
    if(freq2) result += 0x02;
    if(freq3) result += 0x04;
    if(freq4) result += 0x08;
    if(freq5) result += 0x10;
    if(freq6) result += 0x20;
    if(freq7) result += 0x40;
    if(freq8) result += 0x80;

    return result;
}

char MainWindow::getFrequences_2()
{
    char result=0x00;

    if(freq1_2) result += 0x01;
    if(freq2_2) result += 0x02;
    if(freq3_2) result += 0x04;
    if(freq4_2) result += 0x08;
    if(freq5_2) result += 0x10;
    if(freq6_2) result += 0x20;
    if(freq7_2) result += 0x40;
    if(freq8_2) result += 0x80;

    return result;
}

char MainWindow::getTiming()
{
    char result = 0x00;

    switch(timing)
    {
    case 1 : // 10 secondes
        result = 10;
        break;
    case 2 : // 20 secondes
        result = 20;
        break;
    case 3 : // 30 secondes
        result = 30;
        break;
    case 4 : // 40 secondes
        result = 40;
        break;
    case 5 : // 50 secondes
        result = 50;
        break;
    case 6 : // 1 minute
        result = 60;
        break;
    case 7 : // 2 minutes
        result = 120;
        break;
    }

    if(sequences == 0) return 0;

    result /= sequences;
    return result;
}

char MainWindow::getTiming_2()
{
    char result = 0x00;

    switch(timing_2)
    {
    case 1 : // 10 secondes
        result = 10;
        break;
    case 2 : // 20 secondes
        result = 20;
        break;
    case 3 : // 30 secondes
        result = 30;
        break;
    case 4 : // 40 secondes
        result = 40;
        break;
    case 5 : // 50 secondes
        result = 50;
        break;
    case 6 : // 1 minute
        result = 60;
        break;
    case 7 : // 2 minutes
        result = 120;
        break;
    }

    if(sequences_2 == 0) return 0;

    result /= sequences_2;
    return result;
}

char MainWindow::getSequences()
{
    char result=0x00;

    switch(sequences)
    {
    case 1 :
        result = 0x81; // 2 valeurs : 0b10000001
        break;
    case 2 :
        result = 0x91; // 3 valeurs : 0b10010001
        break;
    case 3 :
        result = 0xA5; // 4 valeurs : 0b10100101
        break;
    case 4 :
        result = 0xE9; // 5 valeurs : 0b11101001
        break;
    case 5 :
        result = 0xED; // 6 valeurs : 0b11101101
        break;
    case 6 :
        result = 0xFD; // 7 valeurs : 0b11111101
        break;
     }

    return result;
}

char MainWindow::getSequences_2()
{
    char result=0x00;

    switch(sequences_2)
    {
    case 1 :
        result = 0x81; // 2 valeurs : 0b10000001
        break;
    case 2 :
        result = 0x91; // 3 valeurs : 0b10010001
        break;
    case 3 :
        result = 0xA5; // 4 valeurs : 0b10100101
        break;
    case 4 :
        result = 0xE9; // 5 valeurs : 0b11101001
        break;
    case 5 :
        result = 0xED; // 6 valeurs : 0b11101101
        break;
    case 6 :
        result = 0xFD; // 7 valeurs : 0b11111101
        break;
     }

    return result;
}

