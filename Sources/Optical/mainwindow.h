#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

#include "../Bootloader/product.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    unsigned char m_modeGauche;
    unsigned char m_modeDroit;
    unsigned char m_frequence;
    unsigned char m_timing;
    unsigned char m_sequences;
    unsigned char m_modeGauche_2;
    unsigned char m_modeDroit_2;
    unsigned char m_frequence_2;
    unsigned char m_timing_2;
    unsigned char m_sequences_2;
    unsigned char m_dutyCycle;
    unsigned char m_dutyCycle_2;

private slots:
    void changerMode(int);
    void changerMode_2(int);
    void programGlasses();
    void savePreset();
    void openPreset();
    void summarize(int);
    void modifyFreq();
    void modifyFreq_clicked();

private:
    Product* product;
    QTabWidget *onglets;
    QWidget *pageButton1;
    QWidget *pageButton2;
    QWidget *pageFrequencies;
    QWidget *pageProgram;
    //page 1
    QComboBox *mode;
    QWidget *oeilWidget;
    QComboBox *oeil;
    QGroupBox *groupFreq;
    QCheckBox *freq1;
    QLabel *freq1_label;
    QCheckBox *freq2;
    QLabel *freq2_label;
    QCheckBox *freq3;
    QLabel *freq3_label;
    QCheckBox *freq4;
    QLabel *freq4_label;
    QCheckBox *freq5;
    QLabel *freq5_label;
    QCheckBox *freq6;
    QLabel *freq6_label;
    QCheckBox *freq7;
    QLabel *freq7_label;
    QCheckBox *freq8;
    QLabel *freq8_label;
    QSpinBox *dutyCycle;
    QLabel *dutyCycle_label;
    QWidget *timingWidget;
    QComboBox *timing;
    QWidget *sequencesWidget;
    QComboBox *sequences;
    //page 2
    QComboBox *mode_2;
    QWidget *oeilWidget_2;
    QComboBox *oeil_2;
    QGroupBox *groupFreq_2;
    QCheckBox *freq1_2;
    QLabel *freq1_label_2;
    QCheckBox *freq2_2;
    QLabel *freq2_label_2;
    QCheckBox *freq3_2;
    QLabel *freq3_label_2;
    QCheckBox *freq4_2;
    QLabel *freq4_label_2;
    QCheckBox *freq5_2;
    QLabel *freq5_label_2;
    QCheckBox *freq6_2;
    QLabel *freq6_label_2;
    QCheckBox *freq7_2;
    QLabel *freq7_label_2;
    QCheckBox *freq8_2;
    QLabel *freq8_label_2;
    QWidget *timingWidget_2;
    QComboBox *timing_2;
    QWidget *sequencesWidget_2;
    QComboBox *sequences_2;
    //page frequences
    QGroupBox *groupSetFreq;
    QDoubleSpinBox *setFreq1;
    QLabel *setFreq1_label;
    QDoubleSpinBox *setFreq2;
    QLabel *setFreq2_label;
    QDoubleSpinBox *setFreq3;
    QLabel *setFreq3_label;
    QDoubleSpinBox *setFreq4;
    QLabel *setFreq4_label;
    QDoubleSpinBox *setFreq5;
    QLabel *setFreq5_label;
    QDoubleSpinBox *setFreq6;
    QLabel *setFreq6_label;
    QDoubleSpinBox *setFreq7;
    QLabel *setFreq7_label;
    QDoubleSpinBox *setFreq8;
    QLabel *setFreq8_label;
    QPushButton *freqModif;
    //page programmation
    QGroupBox *groupSummary;
    QTextEdit *summary;

    //boutons
    QPushButton *open;
    QPushButton *save;
    QPushButton *program;
    QPushButton *quit;
    enum
    {
        C_MODE_FREQ=0, C_MODE_DECAL, C_MODE_BLOQUE, C_MODE_PASSE
    };
    char getFrequences();
    char getTiming();
    char getSequences();
    char getFrequences_2();
    char getTiming_2();
    char getSequences_2();
    void programProduct();
};

#endif // MAINWINDOW_H
