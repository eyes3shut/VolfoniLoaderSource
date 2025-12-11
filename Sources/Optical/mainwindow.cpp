#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "password.h"

MainWindow::MainWindow()
{
    setWindowTitle("ActivEyes Optical Training V00.02.01");

    // Creation boite a onglets
    onglets = new QTabWidget(this);
    onglets->setGeometry(5,5,500,350);
    pageButton1 = new QWidget;
    pageButton2 = new QWidget;
    pageFrequencies = new QWidget;
    pageProgram = new QWidget;

    /************* PAGE FREQUENCES ***********************/
     // Groupe : Frequences
    setFreq1 = new QDoubleSpinBox();
//CEP DIJON    setFreq1->setMinimum(0.1); setFreq1->setMaximum(20); setFreq1->setValue(0.5);setFreq1->setEnabled(false);setFreq1->setDecimals(1);
    setFreq1->setMinimum(1); setFreq1->setMaximum(200); setFreq1->setValue(1);setFreq1->setEnabled(false);setFreq1->setDecimals(0);
    setFreq1_label = new QLabel("Frequency 1 (Hz) :");
    setFreq2 = new QDoubleSpinBox();
    setFreq2->setMinimum(1); setFreq2->setMaximum(200); setFreq2->setValue(2);setFreq2->setEnabled(false);setFreq2->setDecimals(0);
    setFreq2_label = new QLabel("Frequency 2 (Hz) :");
    setFreq3 = new QDoubleSpinBox();
    setFreq3->setMinimum(1); setFreq3->setMaximum(200); setFreq3->setValue(3);setFreq3->setEnabled(false);setFreq3->setDecimals(0);
    setFreq3_label = new QLabel("Frequency 3 (Hz) :");
    setFreq4 = new QDoubleSpinBox();
    setFreq4->setMinimum(1); setFreq4->setMaximum(200); setFreq4->setValue(4);setFreq4->setEnabled(false);setFreq4->setDecimals(0);
    setFreq4_label = new QLabel("Frequency 4 (Hz) :");
    setFreq5 = new QDoubleSpinBox();
    setFreq5->setMinimum(1); setFreq5->setMaximum(200); setFreq5->setValue(6);setFreq5->setEnabled(false);setFreq5->setDecimals(0);
    setFreq5_label = new QLabel("Frequency 5 (Hz) :");
    setFreq6 = new QDoubleSpinBox();
    setFreq6->setMinimum(1); setFreq6->setMaximum(200); setFreq6->setValue(8);setFreq6->setEnabled(false);setFreq6->setDecimals(0);
    setFreq6_label = new QLabel("Frequency 6 (Hz) :");
    setFreq7 = new QDoubleSpinBox();
    setFreq7->setMinimum(1); setFreq7->setMaximum(200); setFreq7->setValue(10);setFreq7->setEnabled(false);setFreq7->setDecimals(0);
    setFreq7_label = new QLabel("Frequency 7 (Hz) :");
    setFreq8 = new QDoubleSpinBox();
    setFreq8->setMinimum(1); setFreq8->setMaximum(200); setFreq8->setValue(15);setFreq8->setEnabled(false);setFreq8->setDecimals(0);
    setFreq8_label = new QLabel("Frequency 8 (Hz) :");
    dutyCycle = new QSpinBox();
    dutyCycle->setMinimum(10); dutyCycle->setMaximum(90); dutyCycle->setValue(50);dutyCycle->setEnabled(false);
    dutyCycle_label = new QLabel("Duty Cycle (%) :");

    QGridLayout *setFreqLayout = new QGridLayout;
    setFreqLayout->setAlignment(Qt::AlignCenter);
    setFreqLayout->addWidget(setFreq1_label, 0, 0);
    setFreqLayout->addWidget(setFreq1, 0, 1);
    setFreqLayout->addWidget(setFreq2_label, 1, 0);
    setFreqLayout->addWidget(setFreq2, 1, 1);
    setFreqLayout->addWidget(setFreq3_label, 2, 0);
    setFreqLayout->addWidget(setFreq3, 2, 1);
    setFreqLayout->addWidget(setFreq4_label, 3, 0);
    setFreqLayout->addWidget(setFreq4, 3, 1);
    setFreqLayout->addWidget(setFreq5_label, 4, 0);
    setFreqLayout->addWidget(setFreq5, 4, 1);
    setFreqLayout->addWidget(setFreq6_label, 5, 0);
    setFreqLayout->addWidget(setFreq6, 5, 1);
    setFreqLayout->addWidget(setFreq7_label, 6, 0);
    setFreqLayout->addWidget(setFreq7, 6, 1);
    setFreqLayout->addWidget(setFreq8_label, 7, 0);
    setFreqLayout->addWidget(setFreq8, 7, 1);
    setFreqLayout->addWidget(dutyCycle_label, 8, 0);
    setFreqLayout->addWidget(dutyCycle, 8, 1);

    groupSetFreq = new QGroupBox("Operating frequencies :");
    groupSetFreq->setLayout(setFreqLayout);

    freqModif = new QPushButton("Modify");
    //Fenetre principale
    QVBoxLayout *layoutPageSet = new QVBoxLayout;
    layoutPageSet->addWidget(groupSetFreq);
    layoutPageSet->addWidget(freqModif);

    /************* PAGE 1 ***********************/
    // Groupe : Mode de fonctionnement
    mode = new QComboBox;
    mode->addItem("Please select...");
    mode->addItem("Opening/Closing at the same time");
    mode->addItem("Opening/Closing alternately");
    mode->addItem("One closed, other opening/closing");
    mode->addItem("Different sequences");

    QHBoxLayout *modeLayout = new QHBoxLayout;
    modeLayout->setAlignment(Qt::AlignCenter);
    QLabel *labelope = new QLabel("Operating Mode :");
    modeLayout->addWidget(labelope);
    modeLayout->addWidget(mode);

    // Groupe : Oeil Ferme
    oeil = new QComboBox;
    oeil->addItem("Please select...");
    oeil->addItem("Left");
    oeil->addItem("Right");

    oeilWidget = new QWidget;
    QHBoxLayout *oeilLayout = new QHBoxLayout;
    oeilLayout->setAlignment(Qt::AlignCenter);
    QLabel *labeloeil = new QLabel("Select which eye should stay closed :");
    oeilLayout->addWidget(labeloeil);
    oeilLayout->addWidget(oeil);
    oeilWidget->setLayout(oeilLayout);
    oeilWidget->setVisible(false);

    // Groupe : Frequences
    freq1 = new QCheckBox();
    freq1_label = new QLabel(setFreq1->cleanText() + " Hz");
    freq2 = new QCheckBox();
    freq2_label = new QLabel(setFreq2->cleanText() + " Hz");
    freq3 = new QCheckBox();
    freq3_label = new QLabel(setFreq3->cleanText() + " Hz");
    freq4 = new QCheckBox();
    freq4_label = new QLabel(setFreq4->cleanText() + " Hz");
    freq5 = new QCheckBox();
    freq5_label = new QLabel(setFreq5->cleanText() + " Hz");
    freq6 = new QCheckBox();
    freq6_label = new QLabel(setFreq6->cleanText() + " Hz");
    freq7 = new QCheckBox();
    freq7_label = new QLabel(setFreq7->cleanText() + " Hz");
    freq8 = new QCheckBox();
    freq8_label = new QLabel(setFreq8->cleanText() + " Hz");

    QGridLayout *freqLayout = new QGridLayout;
    freqLayout->setAlignment(Qt::AlignCenter);
    freqLayout->addWidget(freq1, 0, 0);
    freqLayout->addWidget(freq1_label, 0, 1);
    freqLayout->addWidget(freq2, 1, 0);
    freqLayout->addWidget(freq2_label, 1, 1);
    freqLayout->addWidget(freq3, 2, 0);
    freqLayout->addWidget(freq3_label, 2, 1);
    freqLayout->addWidget(freq4, 3, 0);
    freqLayout->addWidget(freq4_label, 3, 1);
    freqLayout->addWidget(freq5, 4, 0);
    freqLayout->addWidget(freq5_label, 4, 1);
    freqLayout->addWidget(freq6, 5, 0);
    freqLayout->addWidget(freq6_label, 5, 1);
    freqLayout->addWidget(freq7, 6, 0);
    freqLayout->addWidget(freq7_label, 6, 1);
    freqLayout->addWidget(freq8, 7, 0);
    freqLayout->addWidget(freq8_label, 7, 1);

    groupFreq = new QGroupBox("Operating frequencies :");
    groupFreq->setLayout(freqLayout);
    groupFreq->setVisible(false);

    // Groupe : Darkening Timing
    timing = new QComboBox;
    timing->addItem("Please select...");
    timing->addItem("10 seconds");
    timing->addItem("20 seconds");
    timing->addItem("30 seconds");
    timing->addItem("40 seconds");
    timing->addItem("50 seconds");
    timing->addItem("1 minute");
    timing->addItem("2 minutes");

    timingWidget = new QWidget;
    QHBoxLayout *timingLayout = new QHBoxLayout;
    timingLayout->setAlignment(Qt::AlignCenter);
    QLabel *labelTim = new QLabel("Glasses get from \"open\" to \"closed\" in :");
    timingLayout->addWidget(labelTim);
    timingLayout->addWidget(timing);
    timingWidget->setLayout(timingLayout);
    timingWidget->setVisible(false);

    // Groupe : Nombre de sequence
    sequences = new QComboBox;
    sequences->addItem("Please select...");
    sequences->addItem("1 step");
    sequences->addItem("2 steps");
    sequences->addItem("3 steps");
    sequences->addItem("4 steps");
    sequences->addItem("5 steps");
    sequences->addItem("6 steps");

    sequencesWidget = new QWidget;
    QHBoxLayout *sequencesLayout = new QHBoxLayout;
    sequencesLayout->setAlignment(Qt::AlignCenter);
    QLabel *labelSeq = new QLabel("Glasses get from \"open\" to \"closed\" in :");
    sequencesLayout->addWidget(labelSeq);
    sequencesLayout->addWidget(sequences);
    sequencesWidget->setLayout(sequencesLayout);
    sequencesWidget->setVisible(false);

    //Fenetre principale
    QVBoxLayout *layoutPage1 = new QVBoxLayout;
    layoutPage1->setAlignment(Qt::AlignTop);
    layoutPage1->addLayout(modeLayout);
    layoutPage1->addWidget(oeilWidget);
    layoutPage1->addWidget(groupFreq);
    layoutPage1->addWidget(timingWidget);
    layoutPage1->addWidget(sequencesWidget);

    /************* PAGE 2 ***********************/
    // Groupe : Mode de fonctionnement
    mode_2 = new QComboBox;
    mode_2->addItem("Same as Button 1");
    mode_2->addItem("Opening/Closing at the same time");
    mode_2->addItem("Opening/Closing alternately");
    mode_2->addItem("One closed, other opening/closing");
    mode_2->addItem("Different sequences");

    QHBoxLayout *modeLayout_2 = new QHBoxLayout;
    modeLayout_2->setAlignment(Qt::AlignCenter);
    QLabel *labelope_2 = new QLabel("Operating Mode :");
    modeLayout_2->addWidget(labelope_2);
    modeLayout_2->addWidget(mode_2);

    // Groupe : Oeil Ferme
    oeil_2 = new QComboBox;
    oeil_2->addItem("Please select...");
    oeil_2->addItem("Left");
    oeil_2->addItem("Right");

    oeilWidget_2 = new QWidget;
    QHBoxLayout *oeilLayout_2 = new QHBoxLayout;
    oeilLayout_2->setAlignment(Qt::AlignCenter);
    QLabel *labelOeil_2 = new QLabel("Select which eye should stay closed :");
    oeilLayout_2->addWidget(labelOeil_2);
    oeilLayout_2->addWidget(oeil_2);
    oeilWidget_2->setLayout(oeilLayout_2);
    oeilWidget_2->setVisible(false);

    // Groupe : Frequences
    freq1_2 = new QCheckBox();
    freq1_label_2 = new QLabel(setFreq1->cleanText() + " Hz");
    freq2_2 = new QCheckBox();
    freq2_label_2 = new QLabel(setFreq2->cleanText() + " Hz");
    freq3_2 = new QCheckBox();
    freq3_label_2 = new QLabel(setFreq3->cleanText() + " Hz");
    freq4_2 = new QCheckBox();
    freq4_label_2 = new QLabel(setFreq4->cleanText() + " Hz");
    freq5_2 = new QCheckBox();
    freq5_label_2 = new QLabel(setFreq5->cleanText() + " Hz");
    freq6_2 = new QCheckBox();
    freq6_label_2 = new QLabel(setFreq6->cleanText() + " Hz");
    freq7_2 = new QCheckBox();
    freq7_label_2 = new QLabel(setFreq7->cleanText() + " Hz");
    freq8_2 = new QCheckBox();
    freq8_label_2 = new QLabel(setFreq8->cleanText() + " Hz");

    QGridLayout *freqLayout_2 = new QGridLayout;
    freqLayout_2->setAlignment(Qt::AlignCenter);
    freqLayout_2->addWidget(freq1_2, 0, 0);
    freqLayout_2->addWidget(freq1_label_2, 0, 1);
    freqLayout_2->addWidget(freq2_2, 1, 0);
    freqLayout_2->addWidget(freq2_label_2, 1, 1);
    freqLayout_2->addWidget(freq3_2, 2, 0);
    freqLayout_2->addWidget(freq3_label_2, 2, 1);
    freqLayout_2->addWidget(freq4_2, 3, 0);
    freqLayout_2->addWidget(freq4_label_2, 3, 1);
    freqLayout_2->addWidget(freq5_2, 4, 0);
    freqLayout_2->addWidget(freq5_label_2, 4, 1);
    freqLayout_2->addWidget(freq6_2, 5, 0);
    freqLayout_2->addWidget(freq6_label_2, 5, 1);
    freqLayout_2->addWidget(freq7_2, 6, 0);
    freqLayout_2->addWidget(freq7_label_2, 6, 1);
    freqLayout_2->addWidget(freq8_2, 7, 0);
    freqLayout_2->addWidget(freq8_label_2, 7, 1);

    groupFreq_2 = new QGroupBox("Operating frequencies :");
    groupFreq_2->setLayout(freqLayout_2);
    groupFreq_2->setVisible(false);

    // Groupe : Darkening Timing
    timing_2 = new QComboBox;
    timing_2->addItem("Please select...");
    timing_2->addItem("10 seconds");
    timing_2->addItem("20 seconds");
    timing_2->addItem("30 seconds");
    timing_2->addItem("40 seconds");
    timing_2->addItem("50 seconds");
    timing_2->addItem("1 minute");
    timing_2->addItem("2 minutes");

    timingWidget_2 = new QWidget;
    QHBoxLayout *timingLayout_2 = new QHBoxLayout;
    timingLayout_2->setAlignment(Qt::AlignCenter);
    QLabel *labelTim2 = new QLabel("Glasses get from \"open\" to \"closed\" in :");
    timingLayout_2->addWidget(labelTim2);
    timingLayout_2->addWidget(timing_2);
    timingWidget_2->setLayout(timingLayout_2);
    timingWidget_2->setVisible(false);

    // Groupe : Nombre de sequence
    sequences_2 = new QComboBox;
    sequences_2->addItem("Please select...");
    sequences_2->addItem("1 step");
    sequences_2->addItem("2 steps");
    sequences_2->addItem("3 steps");
    sequences_2->addItem("4 steps");
    sequences_2->addItem("5 steps");
    sequences_2->addItem("6 steps");

    sequencesWidget_2 = new QWidget;
    QHBoxLayout *sequencesLayout_2 = new QHBoxLayout;
    sequencesLayout_2->setAlignment(Qt::AlignCenter);
    QLabel *labelSeq2 = new QLabel("Glasses get from \"open\" to \"closed\" in :");
    sequencesLayout_2->addWidget(labelSeq2);
    sequencesLayout_2->addWidget(sequences_2);
    sequencesWidget_2->setLayout(sequencesLayout_2);
    sequencesWidget_2->setVisible(false);

    //Fenetre principale
    QVBoxLayout *layoutPage2 = new QVBoxLayout;
    layoutPage2->setAlignment(Qt::AlignTop);
    layoutPage2->addLayout(modeLayout_2);
    layoutPage2->addWidget(oeilWidget_2);
    layoutPage2->addWidget(groupFreq_2);
    layoutPage2->addWidget(timingWidget_2);
    layoutPage2->addWidget(sequencesWidget_2);

    /***************** PAGE PROGRAMMATION **********************/
    summary = new QTextEdit();
    summary->setFrameShape(QFrame::NoFrame);
    QVBoxLayout *ProgLayout = new QVBoxLayout;
    ProgLayout->setAlignment(Qt::AlignCenter);
    ProgLayout->addWidget(summary);

    groupSummary = new QGroupBox("Summary");
    groupSummary->setLayout(ProgLayout);

    //Layout : bouton du bas
    open = new QPushButton("Open");
    save = new QPushButton("Save");
    program = new QPushButton("Program");
    quit = new QPushButton("Quit");
    QHBoxLayout *boutonsLayout = new QHBoxLayout;
    boutonsLayout->setAlignment(Qt::AlignRight);
    boutonsLayout->addWidget(open);
    boutonsLayout->addWidget(save);
    boutonsLayout->addSpacing(150);
    boutonsLayout->addWidget(program);
    boutonsLayout->addWidget(quit);

    //Fenetre principale
    QVBoxLayout *layoutPageProg = new QVBoxLayout;
    layoutPageProg->addWidget(groupSummary);
    layoutPageProg->addSpacing(20);
    layoutPageProg->addLayout(boutonsLayout);

    /***************** FENETRE COMPLETE ***********************/
    pageButton1->setLayout(layoutPage1);
    pageButton2->setLayout(layoutPage2);
    pageFrequencies->setLayout(layoutPageSet);
    pageProgram->setLayout(layoutPageProg);
    onglets->addTab(pageButton1, "Button 1");
    onglets->addTab(pageButton2, "Button 2");
    onglets->addTab(pageFrequencies, "Frequencies");
    onglets->addTab(pageProgram, "Program");

    //CONNECTIONS
    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(changerMode(int)));
    connect(mode_2, SIGNAL(currentIndexChanged(int)), this, SLOT(changerMode_2(int)));
    connect(program, SIGNAL(clicked()), this, SLOT(programGlasses()));
    connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(summarize(int)));
    connect(save, SIGNAL(clicked()), this, SLOT(savePreset()));
    connect(open, SIGNAL(clicked()), this, SLOT(openPreset()));
    connect(freqModif, SIGNAL(clicked()), this, SLOT(modifyFreq_clicked()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::summarize(int index)
{
    QString duree, steps, freq, duty;

    if( (index == 0) || (index == 1) )
    {
        freq1_label->setText(setFreq1->cleanText() + " Hz");
        freq2_label->setText(setFreq2->cleanText() + " Hz");
        freq3_label->setText(setFreq3->cleanText() + " Hz");
        freq4_label->setText(setFreq4->cleanText() + " Hz");
        freq5_label->setText(setFreq5->cleanText() + " Hz");
        freq6_label->setText(setFreq6->cleanText() + " Hz");
        freq7_label->setText(setFreq7->cleanText() + " Hz");
        freq8_label->setText(setFreq8->cleanText() + " Hz");
        freq1_label_2->setText(setFreq1->cleanText() + " Hz");
        freq2_label_2->setText(setFreq2->cleanText() + " Hz");
        freq3_label_2->setText(setFreq3->cleanText() + " Hz");
        freq4_label_2->setText(setFreq4->cleanText() + " Hz");
        freq5_label_2->setText(setFreq5->cleanText() + " Hz");
        freq6_label_2->setText(setFreq6->cleanText() + " Hz");
        freq7_label_2->setText(setFreq7->cleanText() + " Hz");
        freq8_label_2->setText(setFreq8->cleanText() + " Hz");
    }

    if (index == 3)
    {
        //TO DO : faire le résumé de la prog
        summary->clear();
        summary->append("Button 1 :");
        switch (mode->currentIndex())
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
            if(oeil->currentIndex()==1)
                summary->append("Keep the left eye closed while the right one is opening/closing");
            else if(oeil->currentIndex()==2)
                summary->append("Keep the right eye closed while the left one is opening/closing");
            else
                summary->append("Incoherent operating mode");
            break;
        case 4:
            if( (timing->currentIndex()==0) || (sequences->currentIndex()==0) )
            {
                summary->append("Incoherent operating mode");
                break;
            }
            if(timing->currentIndex()==1) duree="10 seconds";
            else if(timing->currentIndex()==2) duree="20 seconds";
            else if(timing->currentIndex()==3) duree="30 seconds";
            else if(timing->currentIndex()==4) duree="40 seconds";
            else if(timing->currentIndex()==5) duree="50 seconds";
            else if(timing->currentIndex()==6) duree="1 minute";
            else if(timing->currentIndex()==7) duree="2 minutes";
            if(sequences->currentIndex()==1) steps = "1 step";
            else if(sequences->currentIndex()==2) steps = "2 steps";
            else if(sequences->currentIndex()==3) steps = "3 steps";
            else if(sequences->currentIndex()==4) steps = "4 steps";
            else if(sequences->currentIndex()==5) steps = "5 steps";
            else if(sequences->currentIndex()==6) steps = "6 steps";
            summary->append("Glasses get from OPEN to CLOSED in " + duree + " and in " + steps);
            break;
        }
        if( (mode->currentIndex()==1) || (mode->currentIndex()==2) || (mode->currentIndex()==3) )
        {
            freq = "";
            if(freq1->isChecked()) freq += setFreq1->cleanText() + " Hz; ";
            if(freq2->isChecked()) freq += setFreq2->cleanText() + " Hz; ";
            if(freq3->isChecked()) freq += setFreq3->cleanText() + " Hz; ";
            if(freq4->isChecked()) freq += setFreq4->cleanText() + " Hz; ";
            if(freq5->isChecked()) freq += setFreq5->cleanText() + " Hz; ";
            if(freq6->isChecked()) freq += setFreq6->cleanText() + " Hz; ";
            if(freq7->isChecked()) freq += setFreq7->cleanText() + " Hz; ";
            if(freq8->isChecked()) freq += setFreq8->cleanText() + " Hz; ";
            summary->append("Used frequencies : " + freq);
            duty = dutyCycle->cleanText() + " %";
            summary->append("Duty Cycle = " + duty);
        }

        summary->append("\nButton 2 :");
        switch (mode_2->currentIndex())
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
            if(oeil_2->currentIndex()==1)
                summary->append("Keep the left eye closed while the right one is opening/closing");
            else if(oeil_2->currentIndex()==2)
                summary->append("Keep the right eye closed while the left one is opening/closing");
            else
                summary->append("Incoherent operating mode");
            break;
        case 4:
            if( (timing_2->currentIndex()==0) || (sequences_2->currentIndex()==0) )
            {
                summary->append("Incoherent operating mode");
                break;
            }
            if(timing_2->currentIndex()==1) duree="10 seconds";
            else if(timing_2->currentIndex()==2) duree="20 seconds";
            else if(timing_2->currentIndex()==3) duree="30 seconds";
            else if(timing_2->currentIndex()==4) duree="40 seconds";
            else if(timing_2->currentIndex()==5) duree="50 seconds";
            else if(timing_2->currentIndex()==6) duree="1 minute";
            else if(timing_2->currentIndex()==7) duree="2 minutes";
            if(sequences_2->currentIndex()==1) steps = "1 step";
            else if(sequences_2->currentIndex()==2) steps = "2 steps";
            else if(sequences_2->currentIndex()==3) steps = "3 steps";
            else if(sequences_2->currentIndex()==4) steps = "4 steps";
            else if(sequences_2->currentIndex()==5) steps = "5 steps";
            else if(sequences_2->currentIndex()==6) steps = "6 steps";
            summary->append("Glasses get from OPEN to CLOSED in " + duree + " and in " + steps);
            break;
        }
        if( (mode_2->currentIndex()==1) || (mode_2->currentIndex()==2) || (mode_2->currentIndex()==3) )
        {
            freq = "";
            if(freq1_2->isChecked()) freq += setFreq1->cleanText() + " Hz; ";
            if(freq2_2->isChecked()) freq += setFreq2->cleanText() + " Hz; ";
            if(freq3_2->isChecked()) freq += setFreq3->cleanText() + " Hz; ";
            if(freq4_2->isChecked()) freq += setFreq4->cleanText() + " Hz; ";
            if(freq5_2->isChecked()) freq += setFreq5->cleanText() + " Hz; ";
            if(freq6_2->isChecked()) freq += setFreq6->cleanText() + " Hz; ";
            if(freq7_2->isChecked()) freq += setFreq7->cleanText() + " Hz; ";
            if(freq8_2->isChecked()) freq += setFreq8->cleanText() + " Hz; ";
            summary->append("Used frequencies : " + freq);
            duty = dutyCycle->cleanText() + " %";
            summary->append("Duty Cycle = " + duty);
        }

        summary->append("\n\nClick the \"Program\" button to validate !");
    }
}

void MainWindow::changerMode(int newMode)
{
    switch(mode->currentIndex())
    {
    case 1:
    case 2:
        oeilWidget->setVisible(false);
        groupFreq->setVisible(true);
        timingWidget->setVisible(false);
        sequencesWidget->setVisible(false);
        break;
    case 3:
        oeilWidget->setVisible(true);
        groupFreq->setVisible(true);
        timingWidget->setVisible(false);
        sequencesWidget->setVisible(false);
        break;
    case 4:
        oeilWidget->setVisible(false);
        groupFreq->setVisible(false);
        timingWidget->setVisible(true);
        sequencesWidget->setVisible(true);
        break;
    case 0:
    default:
        oeilWidget->setVisible(false);
        groupFreq->setVisible(false);
        timingWidget->setVisible(false);
        sequencesWidget->setVisible(false);
    }
    this->adjustSize();
}

void MainWindow::changerMode_2(int newMode)
{
    switch(mode_2->currentIndex())
    {
    case 1:
    case 2:
        oeilWidget_2->setVisible(false);
        groupFreq_2->setVisible(true);
        timingWidget_2->setVisible(false);
        sequencesWidget_2->setVisible(false);
        break;
    case 3:
        oeilWidget_2->setVisible(true);
        groupFreq_2->setVisible(true);
        timingWidget_2->setVisible(false);
        sequencesWidget_2->setVisible(false);
        break;
    case 4:
        oeilWidget_2->setVisible(false);
        groupFreq_2->setVisible(false);
        timingWidget_2->setVisible(true);
        sequencesWidget_2->setVisible(true);
        break;
    case 0:
    default:
        oeilWidget_2->setVisible(false);
        groupFreq_2->setVisible(false);
        timingWidget_2->setVisible(false);
        sequencesWidget_2->setVisible(false);
    }
    this->adjustSize();
}

void MainWindow::programGlasses()
{
    switch(mode->currentIndex())
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
        if (oeil->currentIndex() == 1)
        {
            m_modeGauche = C_MODE_BLOQUE;
            m_modeDroit = C_MODE_FREQ;
        }
        else if (oeil->currentIndex() == 2)
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

    switch(mode_2->currentIndex())
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
        if (oeil_2->currentIndex() == 1)
        {
            m_modeGauche_2 = C_MODE_BLOQUE;
            m_modeDroit_2 = C_MODE_FREQ;
        }
        else if (oeil_2->currentIndex() == 2)
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

    m_dutyCycle = dutyCycle->value();
    m_dutyCycle_2 = dutyCycle->value();

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

    if(freq1->isChecked()) result += 0x01;
    if(freq2->isChecked()) result += 0x02;
    if(freq3->isChecked()) result += 0x04;
    if(freq4->isChecked()) result += 0x08;
    if(freq5->isChecked()) result += 0x10;
    if(freq6->isChecked()) result += 0x20;
    if(freq7->isChecked()) result += 0x40;
    if(freq8->isChecked()) result += 0x80;

    return result;
}

char MainWindow::getFrequences_2()
{
    char result=0x00;

    if(freq1_2->isChecked()) result += 0x01;
    if(freq2_2->isChecked()) result += 0x02;
    if(freq3_2->isChecked()) result += 0x04;
    if(freq4_2->isChecked()) result += 0x08;
    if(freq5_2->isChecked()) result += 0x10;
    if(freq6_2->isChecked()) result += 0x20;
    if(freq7_2->isChecked()) result += 0x40;
    if(freq8_2->isChecked()) result += 0x80;

    return result;
}

char MainWindow::getTiming()
{
    char result = 0x00;

    switch(timing->currentIndex())
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

    if(sequences->currentIndex() == 0) return 0;

    result /= sequences->currentIndex();
    return result;
}

char MainWindow::getTiming_2()
{
    char result = 0x00;

    switch(timing_2->currentIndex())
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

    if(sequences_2->currentIndex() == 0) return 0;

    result /= sequences_2->currentIndex();
    return result;
}

char MainWindow::getSequences()
{
    char result=0x00;

    switch(sequences->currentIndex())
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

    switch(sequences_2->currentIndex())
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

void MainWindow::savePreset()
{
    char* pdata = NULL;
    int crcValue=0;
    pdata = (char*)malloc(49);

    QString nomfichier = QFileDialog::getSaveFileName(this, "Save As...", QDir::rootPath(), "Optical Presets(*.opt)");

    QFile file(nomfichier);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Error", "Invalid save file !");
        return ;
    }

    pdata[0] = 'v';
    pdata[1] = 'o';
    pdata[2] = 'l';
    pdata[3] = 'f';
    pdata[4] = 'o';
    pdata[5] = 'n';
    pdata[6] = 'i';
    pdata[7] = mode->currentIndex();
    pdata[8] = oeil->currentIndex();
    pdata[9] = freq1->isChecked();
    pdata[10] = freq2->isChecked();
    pdata[11] = freq3->isChecked();
    pdata[12] = freq4->isChecked();
    pdata[13] = freq5->isChecked();
    pdata[14] = freq6->isChecked();
    pdata[15] = freq7->isChecked();
    pdata[16] = freq8->isChecked();
    pdata[17] = timing->currentIndex();
    pdata[18] = sequences->currentIndex();

    pdata[19] = mode_2->currentIndex();
    pdata[20] = oeil_2->currentIndex();
    pdata[21] = freq1_2->isChecked();
    pdata[22] = freq2_2->isChecked();
    pdata[23] = freq3_2->isChecked();
    pdata[24] = freq4_2->isChecked();
    pdata[25] = freq5_2->isChecked();
    pdata[26] = freq6_2->isChecked();
    pdata[27] = freq7_2->isChecked();
    pdata[28] = freq8_2->isChecked();
    pdata[29] = timing_2->currentIndex();
    pdata[30] = sequences_2->currentIndex();

    pdata[31] = ((int) (10 * setFreq1->value()))>>8;
    pdata[32] = 10 * setFreq1->value();
    pdata[33] = ((int) (10 * setFreq2->value()))>>8;
    pdata[34] = 10 * setFreq2->value();
    pdata[35] = ((int) (10 * setFreq3->value()))>>8;
    pdata[36] = 10 * setFreq3->value();
    pdata[37] = ((int) (10 * setFreq4->value()))>>8;
    pdata[38] = 10 * setFreq4->value();
    pdata[39] = ((int) (10 * setFreq5->value()))>>8;
    pdata[40] = 10 * setFreq5->value();
    pdata[41] = ((int) (10 * setFreq6->value()))>>8;
    pdata[42] = 10 * setFreq6->value();
    pdata[43] = ((int) (10 * setFreq7->value()))>>8;
    pdata[44] = 10 * setFreq7->value();
    pdata[45] = ((int) (10 * setFreq8->value()))>>8;
    pdata[46] = 10 * setFreq8->value();

    pdata[47] = dutyCycle->value();

    //calcul du CRC
    for(int i=0; i<47; i++) crcValue += pdata[i];
    pdata[48] = (char) crcValue;

    QDataStream flux(&file);
    flux.writeRawData(pdata,49);
    file.close();

    QMessageBox::information(this, "Save", "Presets successfully saved !");
}

void MainWindow::openPreset()
{
    double dTemp;
    int crcValue=0;
    QString nomfichier = QFileDialog::getOpenFileName(this, "Open", QDir::rootPath(), "Optical Presets(*.opt)");

    QFile file(nomfichier);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Invalid presets file !");
        return ;
    }

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

    mode->setCurrentIndex(pdata[7]);
    oeil->setCurrentIndex(pdata[8]);
    freq1->setChecked(pdata[9]);
    freq2->setChecked(pdata[10]);
    freq3->setChecked(pdata[11]);
    freq4->setChecked(pdata[12]);
    freq5->setChecked(pdata[13]);
    freq6->setChecked(pdata[14]);
    freq7->setChecked(pdata[15]);
    freq8->setChecked(pdata[16]);
    timing->setCurrentIndex(pdata[17]);
    sequences->setCurrentIndex(pdata[18]);

    mode_2->setCurrentIndex(pdata[19]);
    oeil_2->setCurrentIndex(pdata[20]);
    freq1_2->setChecked(pdata[21]);
    freq2_2->setChecked(pdata[22]);
    freq3_2->setChecked(pdata[23]);
    freq4_2->setChecked(pdata[24]);
    freq5_2->setChecked(pdata[25]);
    freq6_2->setChecked(pdata[26]);
    freq7_2->setChecked(pdata[27]);
    freq8_2->setChecked(pdata[28]);
    timing_2->setCurrentIndex(pdata[29]);
    sequences_2->setCurrentIndex(pdata[30]);

    dTemp = pdata[31]<<8;
    dTemp += (unsigned char) pdata[32];
    setFreq1->setValue(dTemp/10);
    dTemp = pdata[33]<<8;
    dTemp += (unsigned char) pdata[34];
    setFreq2->setValue(dTemp/10);
    dTemp = pdata[35]<<8;
    dTemp += (unsigned char) pdata[36];
    setFreq3->setValue(dTemp/10);
    dTemp = pdata[37]<<8;
    dTemp += (unsigned char) pdata[38];
    setFreq4->setValue(dTemp/10);
    dTemp = pdata[39]<<8;
    dTemp += (unsigned char) pdata[40];
    setFreq5->setValue(dTemp/10);
    dTemp = pdata[41]<<8;
    dTemp += (unsigned char) pdata[42];
    setFreq6->setValue(dTemp/10);
    dTemp = pdata[43]<<8;
    dTemp += (unsigned char) pdata[44];
    setFreq7->setValue(dTemp/10);
    dTemp = pdata[45]<<8;
    dTemp += (unsigned char) pdata[46];
    setFreq8->setValue(dTemp/10);

    dutyCycle->setValue(pdata[47]);

    summarize(onglets->currentIndex());
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
    param[19] = ((int) (0x3D00 / setFreq1->value()))>>8;
    param[20] = 0x3D00 / setFreq1->value();
    param[21] = ((int) (0x3D00 / setFreq2->value()))>>8;
    param[22] = 0x3D00 / setFreq2->value();
    param[23] = ((int) (0x3D00 / setFreq3->value()))>>8;
    param[24] = 0x3D00 / setFreq3->value();
    param[25] = ((int) (0x3D00 / setFreq4->value()))>>8;
    param[26] = 0x3D00 / setFreq4->value();
    param[27] = ((int) (0x3D00 / setFreq5->value()))>>8;
    param[28] = 0x3D00 / setFreq5->value();
    param[29] = ((int) (0x3D00 / setFreq6->value()))>>8;
    param[30] = 0x3D00 / setFreq6->value();
    param[31] = ((int) (0x3D00 / setFreq7->value()))>>8;
    param[32] = 0x3D00 / setFreq7->value();
    param[33] = ((int) (0x3D00 / setFreq8->value()))>>8;
    param[34] = 0x3D00 / setFreq8->value();
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

void MainWindow::modifyFreq_clicked()
{
    password *fenetre = new password(this);
    connect(fenetre, SIGNAL(pwdValid()), this, SLOT(modifyFreq()));
    fenetre->show();
}

void MainWindow::modifyFreq()
{
    setFreq1->setEnabled(true);
    setFreq2->setEnabled(true);
    setFreq3->setEnabled(true);
    setFreq4->setEnabled(true);
    setFreq5->setEnabled(true);
    setFreq6->setEnabled(true);
    setFreq7->setEnabled(true);
    setFreq8->setEnabled(true);
    dutyCycle->setEnabled(true);
    freqModif->setEnabled(false);
}
