#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMenu>
#include <iostream>
#include <QtXml>
#include <QStatusBar>
#include <QComboBox>
#include <QMouseEvent>

#include "../Bootloader/Comm.h"
#include "../Bootloader/Device.h"
#include "../Bootloader/DeviceData.h"
#include "../Bootloader/ImportExportHex.h"
#include "../Bootloader/product.h"
#include "../Bootloader/hexfile.h"
#include "../Encrypter/decrypt.h"
#include "../Encrypter/encrypt.h"
#include "batteryview.h"

#define MEMORY_ADDRESS_PARTNUMBER       0x1000
#define MEMORY_SIZE_PARTNUMBER          0x11
#define MEMORY_ADDRESS_FIRMWAREREV      0x2000
#define MEMORY_SIZE_FIRMWAREREV         0x11
#define MEMORY_ADDRESS_BOOTLOADERREV    0x0100
#define MEMORY_SIZE_BOOTLOADERREV       0x10
#define MEMORY_ADDRESS_CUSTOMERNUMBER   0x1015
#define MEMORY_SIZE_CUSTOMERNUMBER      0x0F


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, QString fileLang ="");
    ~MainWindow();
    void DisplayFirmwareData();
    void GetPreviousVersion();
    void logUser();// envoie le login et mot de passe de l'utilisateur entrés par l'utilisateur
    void requestUpdateFirmware(QString fileName, QString updateName, bool localFile);
    void saveConfigXML();
    QString changeWithPrevious(QString current, QString lastOne);
    QString adaptTextQStatusBar(QString text);
    QString getDateLastUpdate(QString serial_number);
    QString getVersionLastUpdate(QString serial_number);
    void UpdateFirmware();

    void retranslateUi();
    void retranslateApp(QString langSelected, QString oldLanguage);

    void changeEvent ( QEvent * event )
        {
            if (event->type() == QEvent::LanguageChange) {
                retranslateUi();
            }
            QMainWindow::changeEvent(event);
        }
    void resizeEvent(QResizeEvent * event)
    {
        if(statutmsg==1)_msg = adaptTextQStatusBar(tr("Volfoni Loader, updater for Volfoni products"));
        else if(statutmsg==2)_msg = adaptTextQStatusBar(tr("Activ Me detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        else if(statutmsg==3)_msg = adaptTextQStatusBar(tr("Activ Hub detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        else if(statutmsg==4)_msg = adaptTextQStatusBar(tr("Smart Crystal Cinema detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        else if(statutmsg==5)_msg = adaptTextQStatusBar(tr("Smart Crystal Pro detected") + getDateLastUpdate(SerialNumber)+ getVersionLastUpdate(SerialNumber));
        else if(statutmsg==6)_msg = adaptTextQStatusBar(tr("Impossible to update this product using Volfoni Loader"));
    }


private:

    QTranslator *translatorFR;
    QTranslator *translatorEN;
    QTranslator *translatorSP;
    QTranslator *translatorGR;
    int nLang;

    bool init;
    QWidget *zonecentrale;
    QLabel *label_maj;
    int statutLabelmaj;
    QWidget *widget;
    QLabel *label;
    QMovie *animatedgif;
    QPushButton *button;
    QPushButton *button_maj;
    QPushButton *button_checkupdate;
    QFrame *frame;
    QLabel *label_pic;
    QLabel *label_picture;
    QLabel *label_desc;
    QLabel *label_help;
    int statutLabelDesc;
    QProgressBar *progressbar;
    QProgressBar *batterieBar;
    QMovie *animatedUSB;

    QMenu * editMenu;
    QMenu * logMenu;
    QMenu * helpMenu;

    QAction * _EditAdmin;

    QAction * _HelpOnline;
    QAction * _HelpOffline;
    QAction * _HelpAbout;
    QAction * _HelpBootloader;
    QAction * _backup;
    QAction * _reset;
    QAction * _updatePCLOader;
    QAction * _language;
    QAction * _parameters;
    QAction * _battery;
    QAction * _progLCD;
    QAction * _dump_log;
    QAction * _erase_log;

    QString log_advancedUser;
    QString pwd_advancedUser;
    QString log_adminUser;
    QString pwd_adminUser;

    QString trigrammeProduct;
    QString customerNumber;

    QStatusBar *status;
    QTimer *_timer;
    QString _msg;
    int statutmsg;

    int widthWindow;

    bool enableCheckUpdate;
    bool _returntoLastVersion;

    BatteryView *battery;
    QString glassesID[10];
    QString memoGlassesID[10];
    int batterieLevel[10];
    int batterieIndex;
    int memoBatterieIndex;
    int batterieBuffer[10];
    int battIndex;
    int batterieNbValide;

    void resetState();

    void showMenu();

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;

    QString FirmwareRev;
    QString previousRev;
    QString selectedRev;
    QString BootloaderRev;
    QString SerialNumber;
    QString strLastVersion;

    QString tmpFilenameV[1000];
    QString tmpNameV[1000];
    int tmpV;

    QTimer *waitConnection;
    QTimer *timerDebugMsg;

    int nConnected;
    bool _localFile;

    int _pendingMouseButtonEvent;

    QString versionSoftWare;
    QString _fileLang;

    unsigned char informations[64];
    int battFullCounter;
    int numeroTrameRF;


protected:
    Product* product;
    QTimer *timer_usb;
    QTimer *timerBatt;

    QString m_updateFilename;

    void closeEvent(QCloseEvent *event);

    // To receive mouse button events
    virtual void mousePressEvent(QMouseEvent * event);

public slots:
    void Connection(void);

private slots:
    void GetLastVersion();
    void UpdateFirmwareClicked();
    void WriteDeviceFinished();

    void aboutVolfoni();
    void aboutPCLoader();
    void helpBootloader();
    void openHelp();
    void log_onUser();
    void helpUser();
    void returnLastVersion();
    void resetDevice();
    void setParameters();
    void setProgLCD();
    void getBattery();
//    void dumpLog();
  //  void EraseEeprom();
    void checkupdaterequest();
    void updateStatusBar();
    void updatePCLoader();
    void StopConnection();
    void selectLanguage();
    void applyChanges(int, int, unsigned char *);
    void applyChanges(int, unsigned char *);
    void applyChanges(unsigned char *);
    void acceptParameters();
    void CheckConnected();
    void askDebugMsg();
    void checkMsgRF(unsigned char*);
    void checkAllGlasses(bool);
    void checkOneBattery();

signals:
    void display_data(unsigned char *);
    void display_data(int);
    void displayBattData(int, QString, int);

};

#endif // MAINWINDOW_H
