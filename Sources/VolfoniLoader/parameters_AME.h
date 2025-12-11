#ifndef PARAMETERS_AME_H
#define PARAMETERS_AME_H

#include <QtWidgets>

class Parameters_AME : public QDialog
{
    Q_OBJECT

public:
    Parameters_AME(QWidget *parent = 0, QString firmRev = "");
    ~Parameters_AME();

private:
    QPushButton *quitter;
    QPushButton *appliquer;
    QSlider *sliderDT;
    QLabel *sliderLabel;
    QSpinBox *delta;
    QSpinBox *voltage;
    QComboBox *combo3D;
    QCheckBox *checkCh3;
    QCheckBox *checkCh4;
    QCheckBox *checkCh5;
    QCheckBox *checkCh6;
    QCheckBox *checkCh7;
    QCheckBox *checkCh8;
    QCheckBox *checkCh9;
    QCheckBox *checkChA;
    QCheckBox *checkChB;
    QCheckBox *checkChC;
    QCheckBox *checkChD;
    QCheckBox *checkChE;
    QString m_swRel;
    char m_volt;
    char m_up;
    char m_down;
    char m_delta;
    char m_rfPosaLsb;
    char m_rfPosaMsb;
    char m_rfPosbLsb;
    char m_rfPosbMsb;
    char m_3Dmode;

signals:
    void apply_clicked(unsigned char *);

public slots:
    void displayDT(int);
    void displayAll(unsigned char *);
    void on_Apply_triggered();
};

#endif // PARAMETERS_H
