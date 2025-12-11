#ifndef PROGLCD_H
#define PROGLCD_H

#include <QtWidgets>
#include "highlighter.h"
#include <QTextList>
#include <QTextEdit>
#include <QDialog>
#include <QProgressBar>
#include <QPushButton>

const int firstParamEDGEHS = 51;
const int sizeParamEDGEHS  = 32;
//HAx : Oeil Droit
/*const short HAx_Prepa     = 0b00111100;
const short HAx_Open_Plus   = 0b00111000;
const short HAx_Open_Minus  = 0b00110100;
const short HAx_Close_Plus  = 0b00101100;
const short HAx_Close_Minus = 0b00011100;
//HBx : Oeil Gauche
const short HBx_Prepa       = 0b11000011;
const short HBx_Open_Plus   = 0b10000011;
const short HBx_Open_Minus  = 0b01000011;
const short HBx_Close_Plus  = 0b11000010;
const short HBx_Close_Minus = 0b11000001;*/
//HAx : Oeil Droit
const short HAx_Prepa       = 0b11110000;
const short HAx_Open_Plus   = 0b10110000;
const short HAx_Open_Minus  = 0b01110000;
const short HAx_Close_Plus  = 0b11100000;
const short HAx_Close_Minus = 0b11010000;
//HBx : Oeil Gauche
const short HBx_Prepa       = 0b00001111;
const short HBx_Open_Plus   = 0b00001011;
const short HBx_Open_Minus  = 0b00000111;
const short HBx_Close_Plus  = 0b00001110;
const short HBx_Close_Minus = 0b00001101;

class ProgLCD : public QDialog
{
    Q_OBJECT

public:
    ProgLCD(QWidget *parent = 0);
    ~ProgLCD();

private:
    QProgressBar *comBar;
    QPushButton *ouvrir;
    QPushButton *sauver;
    QPushButton *appliquer;
    QPushButton *quitter;
    QTextEdit   *contenu;
    QTextEdit   *nomFichier;
    Highlighter *highlighter;
    int         varTiming[8];
    int         varCdeLcd[16];
    int         varInter;
    int         varUP;
    int         varDOWN;
    int         varDelay;
    int         varDiode;
    int         varEEPROM;
    int         varPERIOD;
    QTimer      *waitAck;


signals:
    void apply_clicked(int, int, unsigned char *);
    void apply_clicked(int, unsigned char *);

private slots:
    void compil();
    void colorer();
    void sauvegarder();
    void ouvrirFichier();
    void finished(int);
    void StopWaitAck();

};

#endif // PROGLCD_H
