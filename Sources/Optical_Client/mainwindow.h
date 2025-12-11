#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
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

    char mode;
    char oeil;
    char freq1;
    char freq2;
    char freq3;
    char freq4;
    char freq5;
    char freq6;
    char freq7;
    char freq8;
    char timing;
    char sequences;
    char mode_2;
    char oeil_2;
    char freq1_2;
    char freq2_2;
    char freq3_2;
    char freq4_2;
    char freq5_2;
    char freq6_2;
    char freq7_2;
    char freq8_2;
    char timing_2;
    char sequences_2;
    double setFreq1;
    double setFreq2;
    double setFreq3;
    double setFreq4;
    double setFreq5;
    double setFreq6;
    double setFreq7;
    double setFreq8;
    char dutyCycle;

private:
    Product* product;
    QLabel* logo;
    QLabel* isConnected;
    QTextEdit* summary;
    QLineEdit* filePath;
    QPushButton* open;
    QPushButton* program;
    QPushButton* quit;
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

private slots:
    void programProduct();
    void programGlasses();
    void openPreset();
    void summarize();

};

#endif // MAINWINDOW_H
