#ifndef PARAMETERS_SCP_H
#define PARAMETERS_SCP_H

#include <QtWidgets>
#include <QSpinBox>
#include <QDialog>
#include <QPushButton>
#include <QComboBox>

class Parameters_SCP : public QDialog
{
    Q_OBJECT

public:
    Parameters_SCP(QWidget *parent = 0, QString firmRev = "");
    ~Parameters_SCP();

private:
    QPushButton *quitter;
    QPushButton *appliquer;
    QSpinBox *upTime;
    QSpinBox *downTime;
    QSpinBox *delta;
    QSpinBox *voltage;
    QComboBox *combo3D;
    QComboBox *comboAuto;
    QComboBox *comboSafe;
    QString m_swRel;
    int m_volt;
    int m_up;
    int m_down;
    int m_delta;
    int m_3Dmode;
    int m_openSh;
    int m_deltaSh;
    int m_offPeriod;
    int m_automode;
    int m_safelock;

signals:
    void apply_clicked(unsigned char *);

public slots:
    void displayAll(unsigned char *);
    void on_Apply_triggered();
};

#endif // PARAMETERS_SCP_H
