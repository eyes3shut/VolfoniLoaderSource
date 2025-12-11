/************************************************************************
* Copyright (c) 2009-2010,  Microchip Technology Inc.
*
* Microchip licenses this software to you solely for use with Microchip
* products.  The software is owned by Microchip and its licensors, and
* is protected under applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
* MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
* EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
* OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
* TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
* OR OTHER SIMILAR COSTS.
*
* To the fullest extent allowed by law, Microchip and its licensors
* liability shall not exceed the amount of fees, if any, that you
* have paid directly to Microchip to use this software.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QtCore/QProcess>
#include <QMenu>
#include <QFuture>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QtXml>

#include "Comm.h"
#include "DeviceData.h"
#include "Device.h"
#include "ImportExportHex.h"
#include "../Encrypter/decrypt.h"
#include "../Encrypter/encrypt.h"
#include "product.h"

namespace Ui
{
    class MainWindowClass;
}

#define MAX_RECENT_FILES 3

/*!
 * The main Serial Bootloader GUI window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, QString pathResource ="");
    ~MainWindow();

    void GetQuery(void);
    void LoadFile(QString fileName);

    void EraseDevice(void);
//    void EraseData(void);
    void BlankCheckDevice(void);
    void VerifyDevice(void);
    void ReadFirmwareData();

    void setBootloadBusy(bool busy);

    void startRequest(QUrl url);
    void startRequestFirmware(QUrl url);

    QString adaptTextQStatusBar(QString text);
    void printSettings();
    void changePartNumber(QString newOne);
    void changeCustomerNumber(QString oldOne, QString newOne);
    QList<DeviceData::MemoryRange> rawimport;
    bool hasEndOfFileRecord;    // hex file does have an end of file record
    bool hasConfigBits;         // hex file has config bit settings
    bool fileExceedsFlash;      // hex file records exceed device memory constraints

    void setTrigramm(QString trigram);
    void setAcceptManager(bool valid);
    void EnableEdit_Part_Number(bool enable);
    void ReadMemory();
    void WriteMemory();

signals:
    void IoWithDeviceCompleted(QString msg, Comm::ErrorCode, double time);
    void IoWithDeviceStarted(QString msg);
    void AppendString(QString msg);
    void SetProgressBar(int newValue);
    void SetReadedDebug(unsigned char *dataDebug);
    void send_data_for_manager(unsigned char *data);
    void send_Channel_for_manager(unsigned char *data);
    void getDebugMsg();
    void getDebugMsg(int, unsigned char *);

public slots:
    void Connection(void);
    void openRecentFile(void);
    void GetMessages(void);
    void IoWithDeviceComplete(QString msg, Comm::ErrorCode, double time);
    void IoWithDeviceStart(QString msg);
    void displayMsgFromProduct(QString msg);
    void AppendStringToTextbox(QString msg);
    void readedDebug(unsigned char *dataDebug);
    void readedDebug();
    void UpdateProgressBar(int newValue);
    void Set_version_p(QString msg);
    void WriteDevice(void);

protected:
    Product* product;
//    Comm* comm;
    DeviceData* deviceData;
    DeviceData* hexData;
    Device* device;
    Decrypt *decrypt;
    Encrypt *encrypt;

    QFuture<void> future;

    QString fileName;
    QFileSystemWatcher* fileWatcher;
    QTimer *timer;
    QTimer *DebugTimer;

    struct Messages{
        QString code;
        QString text_value;
    };
    QList < Messages > listeMessages;

    bool writeFlash;
    bool writeEeprom;
    bool writeConfig;
    bool eraseDuringWrite;
    bool hexOpen;
    bool ActivatedDebug;

    void setBootloadEnabled(bool enable);

    void UpdateRecentFileList(void);

    Comm::ErrorCode RemapInterruptVectors(Device* device, DeviceData* deviceData);

    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent * event)
    {
        if(statusMsg==1)_msg = adaptTextQStatusBar(tr("Disconnected"));
        else if(statusMsg==2)_msg = adaptTextQStatusBar(tr("Activ Me connected"));
        else if(statusMsg==3)_msg = adaptTextQStatusBar(tr("Activ Hub connected"));
        else if(statusMsg==4)_msg = adaptTextQStatusBar(tr("Smart crystal cinema connected"));
        else if(statusMsg==5)_msg = adaptTextQStatusBar(tr("Smart crystal pro connected"));
        else if(statusMsg==6)_msg = adaptTextQStatusBar(tr("Unknown device connected"));
        else if(statusMsg==7)_msg = adaptTextQStatusBar(tr("Connected"));
    }

private:
    Ui::MainWindowClass *ui;

    int failed;
    QAction *recentFiles[MAX_RECENT_FILES];

    bool wasBootloaderMode;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;

    QString FirmwareRev;
    QString BootloaderRev;
    QString SerialNumber;
    QString trigrammeProduct;
    QString trigrammeProductTempo;
    QString KEY_Decrypt;
    QString debutVersion;
    QString customerNumber;
    QString customerNumberRead;

    QString File_to_download();
    QTimer *wait;
    bool firmwareDownloaded;
    QTimer *_timer;
    QString _msg;
    int statusMsg;
    bool AcceptManager;
    bool opened;

    bool busy;
    QString _pathResource;

    int NOC_Time;
    int NOC_Move;
    int NOC_Temp;
    int NOC_Def_mot;
    int NOC_Def_vent;


private slots:
    void on_actionReset_Device_triggered();
    void on_action_Settings_triggered();
    void on_action_Manager_triggered();
    void on_action_Verify_Device_triggered();
    void on_action_About_triggered();
    void on_actionWrite_Device_triggered();
    void on_actionOpen_triggered();
    void on_actionErase_Device_triggered();
    void on_actionExit_triggered();
    void on_actionGet_Msg_triggered();
    void on_actionDownload_triggered();
    void on_actionGetFile_triggered();
    void on_mng_apply_triggered(unsigned char *);
    void on_mng_sync_changes();
    void httpFinished();
    void httpFirmwareFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);

    void StopWait();
    void updateStatusBar();
    void on_actionEdit_Part_Number_triggered();
    void on_actionEdit_Customer_Number_First_Firmware_Revision_triggered();
    void on_checkBoxDebug_toggled(bool checked);
    void on_actionRead_Memory_triggered();
    void on_actionWrite_Memory_triggered();
    void on_actionSniffer_triggered();
};

#endif // MAINWINDOW_H
