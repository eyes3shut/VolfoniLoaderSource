#ifndef EDITTAB_H
#define EDITTAB_H

#include <QtGui>
#include <QMenu>
#include <iostream>
#include <QStatusBar>
#include <QComboBox>
#include <QtXml>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QSignalMapper>
#include <QScrollArea>

#include "mainwindow.h"

#include "../Encrypter/decrypt.h"
#include "../Encrypter/encrypt.h"

class MainWindow;


class EditTab : public QDialog
{
    Q_OBJECT

public:
    EditTab(QString trigram, MainWindow *mW, QString fileNameVersioning, QString folder, QString pathResource);
    QDomElement save();

    QString getFileName();
    QString getName();
    QString getDate();
    QString getDescription();
    QString getCRC();
    QString getModifs();
    int getNUpdate();
    int getNVersionDiff();
    QString getNVersionDiffValue();
    bool getSucces();
    void setSucces();


private:

    bool noError;
    void addUpdateFunction(bool init);
    QString getCRC32(QString filePath);

    QLineEdit *fileName[1000];
    QLineEdit *newfileName[1000];
    QLineEdit *name[1000];
    QLineEdit *description[1000];
    QLineEdit *date[1000];
    QLineEdit *CRC[1000];
    QLineEdit *modif[1000][100];
    int       nModifFound[1000];
    QPushButton *delModif[1000][100];
    QPushButton *addModif[1000][100];
    QLabel    *l_modif[1000][100];
    QLabel *l_version[1000];
    QColor colorLabel[1000];
    QSignalMapper *signalMapperDelModif;
    QSignalMapper *signalMapperAddModif;
    QGridLayout *GridModif[1000];
    QGridLayout *GridUpdate[1000];
    QVBoxLayout *grids[1000];
    QGroupBox *GroupBoxUpdate[1000];

    QString fileNameValue[1000];
    QString newfileNameValue[1000];
    QString nameValue[1000];
    QString descriptionValue[1000];
    QString dateValue[1000];
    QString CRCValue[1000];
    QString modifValue[1000][100];

    QPushButton *deleteThisUpdate[1000];
    QPushButton *browseUpdate[1000];
    QPushButton *encryptUpdate[1000];
    QPushButton *decryptUpdate[1000];
    QPushButton *CRC_Update[1000];
    QSignalMapper *signalMapperCRC_Update;
    QSignalMapper *signalMapperBrowseUpdate;
    QPushButton *addUpdateButton;
    QSignalMapper *signalMapperDeleteUpdate;
    QSignalMapper *signalMapperEncryptUpdate;
    QSignalMapper *signalMapperDecryptUpdate;

    int nUpdate;

    QVBoxLayout *layout_updates;

    QScrollArea *Scroll;
    QWidget *areaScroll;

    QComboBox *comboFilter;
    QString diffVersions[120];
    int nVersionDiff;
    //////////////////////////////////////////


    QVBoxLayout *layout_principal;
    QString _trigram;
    MainWindow *_mW;
    QString _fileNameVersioning;
    QString _folder;
    QString KEY_Decrypt;
    QString debutVersion;
    QString _pathResource;

    void focusBottom();

    void updateFilter();

    Encrypt *encrypt;
    Decrypt *decrypt;



protected:


public slots:


private slots:

    void addUpdate();
    void deleteUpdate(int);
    void delModification(QString values);
    void addModification(QString values);
    void browseUpdt(int a);
    void CRC_Updt(int a);
    void encryptUpdt(int a);
    void decryptUpdt(int a);

    void validFilter(int a);
    void NametextChanged(const QString & texte);




};

#endif // EDITTAB_H
