#ifndef PROGRESS_H
#define PROGRESS_H

#include <QtGui>
#include <iostream>

#include "generalTab.h"
class GeneralTab;

#include "SendServer.h"
class SendServer;

#include "mainwindow.h"
class MainWindow;

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;



class Progress : public QDialog
{
    Q_OBJECT

public:
    Progress(QWidget* parent, int caseProgress);
    //caseProgress = 0, downloading
    //caseProgress = 1, uploading
    //caseProgress = 2, zipping
    ~Progress();
    void setValueProgress(int value);
    void setClosable(bool accepted);

    void setGT(GeneralTab *gT);
    void setMW(MainWindow *mW);
    void setSS(SendServer *sS);

    void setValueLabel(QString value);
    void setValueTitle(QString value);


private:

    int _caseProgress;
    GeneralTab *_gT;
    SendServer *_sS;
    MainWindow *_mW;
    QProgressBar *progressbar;
    QLabel *label;


    QVBoxLayout *layout;

    bool closeAccepted;


protected:
    void closeEvent(QCloseEvent *event);

public slots:


private slots:


};

#endif // PROGRESS_H
