#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMenu>
#include <iostream>
#include <QStatusBar>
#include <QComboBox>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QSignalMapper>
#include <QScrollArea>
#include <QTabWidget>
#include <QMainWindow>

#include "editTab.h"
class EditTab;

#include "SendServer.h"
class SendServer;

#include "generalTab.h"
class GeneralTab;

#include "viewTab.h"
class ViewTab;

#include "tools.h"
class CRC32;
class EncryptFunction;
class DecryptFunction;

class MainWindow : public QMainWindow//QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, QString pathResource ="");

    void setTabs();
    void resetTabs();
    int CurrentTabIndex();
    EditTab *edittab[100];
    GeneralTab *generaltab;
    ViewTab *viewtab[100];
    QString trigramDevices[100];
    QString nameDevices[100];
    CRC32 *crc32;
    EncryptFunction *encryptfunction;
    DecryptFunction *decryptfunction;

    void logUser(QString m_url, QString m_login, QString m_pwd);
    void sendToServer();



private:


    int nDevices;
    QTabWidget *tabWidget;
    bool init;

    //////////////////////////////////////////

    QPushButton *m_save;
    QPushButton *m_saveQuit;
    QPushButton *m_cancel;
    QHBoxLayout *layout_buttons;

    QVBoxLayout *layout_principal;

    bool closeAccepted;

    SendServer *sendserver;

    QMenu * toolsMenu;
    QMenu * helpMenu;

    QAction * _CRC32Action;
    QAction * _EncryptAction;
    QAction * _DecryptAction;
    QAction * _AboutAction;

    QString _pathResource;



protected:
    void closeEvent(QCloseEvent *event);


public slots:


private slots:
    void save();
    void saveQuit();
    void currentTabChanged(int);

    void Encrypt_Function();
    void Decrypt_Function();
    void CRC32_Function();
    void aboutManager();


};


class Features : public QDialog
{
    Q_OBJECT

public:
    Features(QWidget* parent, MainWindow *mW, QString rootFolder, QString trigrams[100], int nDevices);
    ~Features();


private:

    QString _trigrams[100];
    int _nDevices;
    MainWindow *_mW;
    QString _rootFolder;

    QPushButton *m_cancel;
    QPushButton *m_apply;
    QHBoxLayout *layout_buttons;

    QCheckBox *zipFolder;
    QLineEdit *zipName;
    QCheckBox *sendFolder;
    QLineEdit *workingDirectory;
    QPushButton *browse;

    QHBoxLayout *layout_checkboxes;


    QVBoxLayout *layout_principal;



protected:

public slots:


private slots:

    void save();
    void browseFolder();


};
#endif // MAINWINDOW_H
