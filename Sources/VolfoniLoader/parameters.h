#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QtGui>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QDialog>
#include <QSpinBox>
#include <QDialog>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>



const int firstParam = 17;
const int sizeParam  = 34;

class Parameters : public QDialog
{
    Q_OBJECT

public:
    Parameters(QWidget *parent = 0, QString firmRev = "");
    ~Parameters();

private:
    QPushButton *quitter;
    QPushButton *appliquer;
    QPushButton *more;
    QRadioButton *full3D;
    QRadioButton *full3DReverse;
    QRadioButton *sharedScreen;
    QRadioButton *movesEnabled;
    QRadioButton *movesDisabled;
    QSlider *sliderFW;
    QSlider *sliderDT;
    QLabel *sliderLabel;
    QSpinBox *delta;
    QComboBox *sensiMove;
    QComboBox *delaiMove;
    QComboBox *delaiIr;
    QComboBox *seuilBatt;
    QGroupBox *groupAdv;
    QComboBox *combo3D;
    QComboBox *comboSync;
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
    QComboBox *combo3D_B;
    QComboBox *comboSync_B;
    QCheckBox *checkCh3_B;
    QCheckBox *checkCh4_B;
    QCheckBox *checkCh5_B;
    QCheckBox *checkCh6_B;
    QCheckBox *checkCh7_B;
    QCheckBox *checkCh8_B;
    QCheckBox *checkCh9_B;
    QCheckBox *checkChA_B;
    QCheckBox *checkChB_B;
    QCheckBox *checkChC_B;
    QCheckBox *checkChD_B;
    QCheckBox *checkChE_B;
    QComboBox *combo3D_C;
    QComboBox *comboSync_C;
    QCheckBox *checkCh3_C;
    QCheckBox *checkCh4_C;
    QCheckBox *checkCh5_C;
    QCheckBox *checkCh6_C;
    QCheckBox *checkCh7_C;
    QCheckBox *checkCh8_C;
    QCheckBox *checkCh9_C;
    QCheckBox *checkChA_C;
    QCheckBox *checkChB_C;
    QCheckBox *checkChC_C;
    QCheckBox *checkChD_C;
    QCheckBox *checkChE_C;
    QString m_swRel;
    char m_volt;
    char m_up;
    char m_down;
    char m_delta;
    char m_customNbre[15];
    char m_rfPosaLsb;
    char m_rfPosaMsb;
    char m_rfPosbLsb;
    char m_rfPosbMsb;
    char m_rfPoscLsb;
    char m_rfPoscMsb;
    char m_ledMode;
    char m_opticPosa;
    char m_opticPosb;
    char m_opticPosc;
    char m_activMov;
    char m_activSync;
    char m_maxRoueLibre;
    char m_sensiMove;
    char m_lowBatt;
    bool advancedVisible;

signals:
    void apply_clicked(int, int, unsigned char *);

public slots:
    void displayDT(int);
    void displayAll(unsigned char *);
    void on_Apply_triggered();
    void on_More_trigerred();
    void delaiMoveChanged(int);
    void movesDisabledChanged(bool);
    void movesEnabledChanged(bool);
};

#endif // PARAMETERS_H
