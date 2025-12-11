#include "manager.h"
#include "ui_manager.h"
#include <QtGui>

Manager::Manager(QWidget *parent, QString trigrammeProduct, QString pathResource) :
    QDialog(parent,Qt::WindowCloseButtonHint),
    ui(new Ui::Manager),
    _trigrammeProduct(trigrammeProduct),
    _pathResource(pathResource)
{

    ui->setupUi(this);

    ui->comboBox_LR->addItem("Inverted");
    ui->comboBox_LR->addItem("Normal");

    ui->comboBox_RFPower->addItem(" 14 dBm");
    ui->comboBox_RFPower->addItem(" 12 dBm");
    ui->comboBox_RFPower->addItem(" 10 dBm");
    ui->comboBox_RFPower->addItem("  8 dBm");
    ui->comboBox_RFPower->addItem("  6 dBm");
    ui->comboBox_RFPower->addItem("  4 dBm");
    ui->comboBox_RFPower->addItem("  2 dBm");
    ui->comboBox_RFPower->addItem("  0 dBm");
    ui->comboBox_RFPower->addItem(" -2 dBm");
    ui->comboBox_RFPower->addItem(" -4 dBm");
    ui->comboBox_RFPower->addItem(" -6 dBm");
    ui->comboBox_RFPower->addItem(" -8 dBm");
    ui->comboBox_RFPower->addItem("-10 dBm");
    ui->comboBox_RFPower->addItem("-12 dBm");
    ui->comboBox_RFPower->addItem("-14 dBm");
    ui->comboBox_RFPower->addItem("-16 dBm");

    if(_trigrammeProduct == "AME" || _trigrammeProduct == "EDG" || _trigrammeProduct == "AHU"){
        ui->Mng_Shutter_Delay_spinBox->setVisible(false);ui->label_18->setVisible(false);
        ui->Mng_Delta_Shutter_spinBox->setVisible(false);ui->label_19->setVisible(false);
        ui->Mng_OffPeriod_spinBox->setVisible(false);ui->label_21->setVisible(false);
        if(_trigrammeProduct == "EDG" || _trigrammeProduct == "AHU")
        {
            ui->label_17->setText("RF Channel :");
            ui->label_46->setVisible(false);
            ui->label_47->setVisible(false);
            ui->label_48->setVisible(false);
            ui->label_49->setVisible(false);
            ui->label_50->setVisible(false);
            ui->label_51->setVisible(false);
            ui->label_52->setVisible(false);
            ui->label_53->setVisible(false);
            ui->label_54->setVisible(false);
            ui->label_55->setVisible(false);
            ui->label_56->setVisible(false);
            ui->label_57->setVisible(false);
            ui->label_58->setVisible(false);
            ui->CBF1_4->setVisible(false);
            ui->CBF2_4->setVisible(false);
            ui->CBF3_4->setVisible(false);
            ui->CBF4_4->setVisible(false);
            ui->CBF5_4->setVisible(false);
            ui->CBF6_4->setVisible(false);
            ui->CBF7_4->setVisible(false);
            ui->CBF8_4->setVisible(false);
            ui->CBF9_4->setVisible(false);
            ui->CBF10_4->setVisible(false);
            ui->CBF11_4->setVisible(false);
            ui->CBF12_4->setVisible(false);
        }
        if(_trigrammeProduct == "EDG" || _trigrammeProduct == "AME")
        {
            ui->label_22->setVisible(false); ui->comboBox_RFPower->setVisible(false);
        }
        if( _trigrammeProduct == "AME" || _trigrammeProduct == "AHU")
        {
            ui->comboBox_LR->setVisible(false); ui->label_20->setVisible(false);
        }
     }else{
        if(_trigrammeProduct == "SCC")
        {
            ui->Mng_Shutter_Delay_spinBox->setVisible(false);ui->label_18->setVisible(false);
            ui->Mng_Delta_Shutter_spinBox->setVisible(false);ui->label_19->setVisible(false);
            ui->Mng_OffPeriod_spinBox->setVisible(false);ui->label_21->setVisible(false);
        }
        ui->widgetChannel->setVisible(false);
    }

    QPushButton* applybutton=ui->buttonBox->button(QDialogButtonBox::Apply);
    //    QObject::connect(applybutton, SIGNAL(clicked()), this, SIGNAL(Mng_apply_changes()));
    QObject::connect(applybutton, SIGNAL(clicked()), this, SLOT(Apply_clicked()));
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(Apply_clicked()));
   // QObject::connect(ui->Sync_pushButton, SIGNAL(clicked()), this, SIGNAL(Mng_sync_changes()));
//    QObject::connect(ui->Sync_pushButton, SIGNAL(clicked()), this, SLOT(Inversion_clicked()));
}

Manager::~Manager()
{
    delete ui;
}

void Manager::Inversion_clicked()
{
    sync_LR = !sync_LR;
    display_sync(sync_LR);
}

void Manager::Apply_clicked()
{
    unsigned char Param[20];
    int value;

    if(_trigrammeProduct == "SCC" || _trigrammeProduct == "SCP"){
        Param[0] = ui->Mng_LCDVolt_spinBox->value();
        Param[1] = (unsigned char) (ui->Mng_UP_spinBox->value() >> 8);
        Param[2] = (unsigned char) (ui->Mng_UP_spinBox->value());
        Param[3] = (unsigned char) (ui->Mng_DOWN_spinBox->value() >> 8);
        Param[4] = (unsigned char) (ui->Mng_DOWN_spinBox->value());
        Param[5] = (unsigned char) (ui->Mng_Delta_spinBox->value() >> 8);
        Param[6] = (unsigned char) (ui->Mng_Delta_spinBox->value());
        Param[7] = (unsigned char) (ui->Mng_Shutter_Delay_spinBox->value() >> 8);
        Param[8] = (unsigned char) (ui->Mng_Shutter_Delay_spinBox->value());
        Param[9] = (unsigned char) (ui->Mng_Delta_Shutter_spinBox->value() >> 8);
        Param[10] = (unsigned char) (ui->Mng_Delta_Shutter_spinBox->value());
        Param[11] = ui->comboBox_LR->currentIndex();
        value = ui->Mng_OffPeriod_spinBox->value();
        if( (value>0) && (value<12) )
            {
                value = 12;
                ui->Mng_OffPeriod_spinBox->setValue(12);
            }
        if(value!=0) value = 750000 / ui->Mng_OffPeriod_spinBox->value();
        else value = 0;
        Param[12] = (unsigned char) (value >> 8);
        Param[13] = (unsigned char) (value);
    }else{
        Param[0] = ui->Mng_LCDVolt_spinBox->value();
        Param[1] = ui->Mng_UP_spinBox->value()/16;
        Param[2] = ui->Mng_DOWN_spinBox->value()/16;
        Param[3] = ui->Mng_Delta_spinBox->value()/16;
        if(_trigrammeProduct == "EDG") Param[14] = ui->comboBox_LR->currentIndex();
    }

    //////////////////////////////////////////////////////////////////////
    //partie channel frequency

    if(_trigrammeProduct == "AME" || _trigrammeProduct == "EDG" || _trigrammeProduct == "AHU"){
        int N1=0;
        int N2=0;
        int N3=0;
        int N4=0;
        if(ui->CBF1->isChecked())N1+=1;
        if(ui->CBF2->isChecked())N1+=2;
        if(ui->CBF3->isChecked())N1+=4;
        if(ui->CBF4->isChecked())N1+=8;
        if(ui->CBF5->isChecked())N1+=16;
        if(ui->CBF6->isChecked())N1+=32;
        if(ui->CBF7->isChecked())N1+=64;
        if(ui->CBF8->isChecked())N1+=128;
        if(ui->CBF9->isChecked())N2+=1;
        if(ui->CBF10->isChecked())N2+=2;
        if(ui->CBF11->isChecked())N2+=4;
        if(ui->CBF12->isChecked())N2+=8;
        if(ui->CBF1_4->isChecked())N3+=1;
        if(ui->CBF2_4->isChecked())N3+=2;
        if(ui->CBF3_4->isChecked())N3+=4;
        if(ui->CBF4_4->isChecked())N3+=8;
        if(ui->CBF5_4->isChecked())N3+=16;
        if(ui->CBF6_4->isChecked())N3+=32;
        if(ui->CBF7_4->isChecked())N3+=64;
        if(ui->CBF8_4->isChecked())N3+=128;
        if(ui->CBF9_4->isChecked())N4+=1;
        if(ui->CBF10_4->isChecked())N4+=2;
        if(ui->CBF11_4->isChecked())N4+=4;
        if(ui->CBF12_4->isChecked())N4+=8;
        Param[10] = N1;
        Param[11] = N2;
        Param[12] = N3;
        Param[13] = N4;
        if(_trigrammeProduct == "AHU") Param[14] = ui->comboBox_RFPower->currentIndex();
    }
    // verifier que c'est dans le bon interval
    QString macChange = "limites.xml";
#ifdef Q_OS_MAC
    macChange =_pathResource;
    macChange.append("limites.xml");
#endif
    if(QFile::exists(macChange)){
        QString fileName = macChange;
        QFile file(fileName);
        QDomDocument settings(fileName);

        //handle non-existent file case (print debug message and return)
        if (!file.open(QIODevice::ReadOnly)){
            std::cout << "Error while reading odin configuration file" << std::endl;
        }
        QString errorMsg;
        if(!settings.setContent(&file, &errorMsg)){ //handle error (print error message on pane for instance)
            std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
            file.close();
        }
        file.close();

        //parse the settings tree and restore state
        QDomElement root = settings.documentElement();
        QDomElement b = root.firstChildElement("minLCDVoltage");
        int minLCDVoltage = b.text().toInt();
        b = root.firstChildElement("maxLCDVoltage");
        int maxLCDVoltage = b.text().toInt();
        b = root.firstChildElement("minUPDelay");
        int minUPDelay = b.text().toInt();
        b = root.firstChildElement("maxUPDelay");
        int maxUPDelay = b.text().toInt();
        b = root.firstChildElement("minDOWNDelay");
        int minDOWNDelay = b.text().toInt();
        b = root.firstChildElement("maxDOWNDelay");
        int maxDOWNDelay = b.text().toInt();
        b = root.firstChildElement("minDelaSync");
        int minDelaSync = b.text().toInt();
        b = root.firstChildElement("maxDelaSync");
        int maxDelaSync = b.text().toInt();
        b = root.firstChildElement("minShutterDelay");
        int minShutterDelay = b.text().toInt();
        b = root.firstChildElement("maxShutterDelay");
        int maxShutterDelay = b.text().toInt();
        b = root.firstChildElement("minDeltaShutter");
        int minDeltaShutter = b.text().toInt();
        b = root.firstChildElement("maxDeltaShutter");
        int maxDeltaShutter = b.text().toInt();

        if(ui->Mng_LCDVolt_spinBox->text().toInt()>maxLCDVoltage)ui->Mng_LCDVolt_spinBox->setMaximum(maxLCDVoltage);
        if(ui->Mng_LCDVolt_spinBox->text().toInt()<minLCDVoltage)ui->Mng_LCDVolt_spinBox->setMinimum(minLCDVoltage);
        if(ui->Mng_UP_spinBox->text().toInt()>maxUPDelay)ui->Mng_UP_spinBox->setMaximum(maxUPDelay);
        if(ui->Mng_UP_spinBox->text().toInt()<minUPDelay)ui->Mng_UP_spinBox->setMinimum(minUPDelay);
        if(ui->Mng_DOWN_spinBox->text().toInt()>maxDOWNDelay)ui->Mng_DOWN_spinBox->setMaximum(maxDOWNDelay);
        if(ui->Mng_DOWN_spinBox->text().toInt()<minDOWNDelay)ui->Mng_DOWN_spinBox->setMinimum(minDOWNDelay);
        if(ui->Mng_Delta_spinBox->text().toInt()>maxDelaSync)ui->Mng_Delta_spinBox->setMaximum(maxDelaSync);
        if(ui->Mng_Delta_spinBox->text().toInt()<minDelaSync)ui->Mng_Delta_spinBox->setMinimum(minDelaSync);
        if(ui->Mng_Shutter_Delay_spinBox->text().toInt()>maxShutterDelay)ui->Mng_Shutter_Delay_spinBox->setMaximum(maxShutterDelay);
        if(ui->Mng_Shutter_Delay_spinBox->text().toInt()<minShutterDelay)ui->Mng_Shutter_Delay_spinBox->setMinimum(minShutterDelay);
        if(ui->Mng_Delta_Shutter_spinBox->text().toInt()>maxDeltaShutter)ui->Mng_Delta_Shutter_spinBox->setMaximum(maxDeltaShutter);
        if(ui->Mng_Delta_Shutter_spinBox->text().toInt()<minDeltaShutter)ui->Mng_Delta_Shutter_spinBox->setMinimum(minDeltaShutter);

    }else{
        if(ui->Mng_LCDVolt_spinBox->text().toInt()>27)ui->Mng_LCDVolt_spinBox->setMaximum(27);
        if(ui->Mng_LCDVolt_spinBox->text().toInt()<2)ui->Mng_LCDVolt_spinBox->setMinimum(2);
        if(ui->Mng_UP_spinBox->text().toInt()>1000)ui->Mng_UP_spinBox->setMaximum(1000);
        if(ui->Mng_UP_spinBox->text().toInt()<-1000)ui->Mng_UP_spinBox->setMinimum(-1000);
        if(ui->Mng_DOWN_spinBox->text().toInt()>1000)ui->Mng_DOWN_spinBox->setMaximum(1000);
        if(ui->Mng_DOWN_spinBox->text().toInt()<-1000)ui->Mng_DOWN_spinBox->setMinimum(-1000);
        if(ui->Mng_Delta_spinBox->text().toInt()>1000)ui->Mng_Delta_spinBox->setMaximum(1000);
        if(ui->Mng_Delta_spinBox->text().toInt()<-1000)ui->Mng_Delta_spinBox->setMinimum(-1000);
        if(ui->Mng_Shutter_Delay_spinBox->text().toInt()>2000)ui->Mng_Shutter_Delay_spinBox->setMaximum(2000);
        if(ui->Mng_Shutter_Delay_spinBox->text().toInt()<0)ui->Mng_Shutter_Delay_spinBox->setMinimum(0);
        if(ui->Mng_Delta_Shutter_spinBox->text().toInt()>1000)ui->Mng_Delta_Shutter_spinBox->setMaximum(1000);
        if(ui->Mng_Delta_Shutter_spinBox->text().toInt()<-1000)ui->Mng_Delta_Shutter_spinBox->setMinimum(-1000);
    }

    emit Mng_apply_changes(Param);
}

void Manager::display_channel(unsigned char *data){
    ui->CBF1->setChecked(false);
    ui->CBF2->setChecked(false);
    ui->CBF3->setChecked(false);
    ui->CBF4->setChecked(false);
    ui->CBF5->setChecked(false);
    ui->CBF6->setChecked(false);
    ui->CBF7->setChecked(false);
    ui->CBF8->setChecked(false);
    ui->CBF9->setChecked(false);
    ui->CBF10->setChecked(false);
    ui->CBF11->setChecked(false);
    ui->CBF12->setChecked(false);
    ui->CBF1_4->setChecked(false);
    ui->CBF2_4->setChecked(false);
    ui->CBF3_4->setChecked(false);
    ui->CBF4_4->setChecked(false);
    ui->CBF5_4->setChecked(false);
    ui->CBF6_4->setChecked(false);
    ui->CBF7_4->setChecked(false);
    ui->CBF8_4->setChecked(false);
    ui->CBF9_4->setChecked(false);
    ui->CBF10_4->setChecked(false);
    ui->CBF11_4->setChecked(false);
    ui->CBF12_4->setChecked(false);

    int N1 = (int)data[0];
    if(N1&0x80)ui->CBF8->setChecked(true);
    if(N1&0x40)ui->CBF7->setChecked(true);
    if(N1&0x20)ui->CBF6->setChecked(true);
    if(N1&0x10)ui->CBF5->setChecked(true);
    if(N1&0x08)ui->CBF4->setChecked(true);
    if(N1&0x04)ui->CBF3->setChecked(true);
    if(N1&0x02)ui->CBF2->setChecked(true);
    if(N1&0x01)ui->CBF1->setChecked(true);

    int N2 = (int)data[1];
    if(N2&0x08)ui->CBF12->setChecked(true);
    if(N2&0x04)ui->CBF11->setChecked(true);
    if(N2&0x02)ui->CBF10->setChecked(true);
    if(N2&0x01)ui->CBF9->setChecked(true);

    int N3 = (int)data[2];
    if(N3&0x80)ui->CBF8_4->setChecked(true);
    if(N3&0x40)ui->CBF7_4->setChecked(true);
    if(N3&0x20)ui->CBF6_4->setChecked(true);
    if(N3&0x10)ui->CBF5_4->setChecked(true);
    if(N3&0x08)ui->CBF4_4->setChecked(true);
    if(N3&0x04)ui->CBF3_4->setChecked(true);
    if(N3&0x02)ui->CBF2_4->setChecked(true);
    if(N3&0x01)ui->CBF1_4->setChecked(true);

    int N4 = (int)data[3];
    if(N4&0x08)ui->CBF12_4->setChecked(true);
    if(N4&0x04)ui->CBF11_4->setChecked(true);
    if(N4&0x02)ui->CBF10_4->setChecked(true);
    if(N4&0x01)ui->CBF9_4->setChecked(true);

    if(_trigrammeProduct == "EDG") display_sync(data[4]);
    else
    {
        if(data[4]<0x10) ui->comboBox_RFPower->setCurrentIndex(data[4]);
        else ui->comboBox_RFPower->setCurrentIndex(0);
    }

}

void Manager::display_sync(unsigned char sync)
{
    ui->comboBox_LR->setCurrentIndex(sync);
 /*   if(sync)
    {
        ui->Sync_Text->setTextColor(Qt::blue);
        ui->Sync_Text->setPlainText("NORMAL");
    }
    else
    {
        ui->Sync_Text->setTextColor(Qt::red);
        ui->Sync_Text->setPlainText("INVERSE");
    }*/
}

void Manager::display_data(unsigned char *data)
{
    QApplication::restoreOverrideCursor();
    int value;

    if(_trigrammeProduct == "SCC" || _trigrammeProduct == "SCP"){
    ui->Mng_LCDVolt_spinBox->setValue(data[0]);
    ui->Mng_UP_spinBox->setValue((short) (data[1]*0x100 + data[2]) );
    ui->Mng_DOWN_spinBox->setValue((short) (data[3]*0x100 + data[4]) );
    ui->Mng_Delta_spinBox->setValue((short) (data[5]*0x100 + data[6]) );
    ui->Mng_Shutter_Delay_spinBox->setValue((short) (data[7]*0x100 + data[8]) );
    ui->Mng_Delta_Shutter_spinBox->setValue((short) (data[9]*0x100 + data[10]) );
    display_sync(data[11]);
    if(data[12]!=0 || data[13]!=0 ) value = 750000 / (data[12]*0x100 + data[13]);
    else value=0;
    ui->Mng_OffPeriod_spinBox->setValue((short) value );
    }else{
        ui->Mng_LCDVolt_spinBox->setValue(QString::number((char)data[0]).toInt());
        ui->Mng_UP_spinBox->setValue(QString::number((char)data[1]*16).toInt());
        ui->Mng_DOWN_spinBox->setValue(QString::number((char)data[2]*16).toInt());
        ui->Mng_Delta_spinBox->setValue(QString::number((char)data[3]*16).toInt());
    }
    // verifier que c'est dans le bon interval
    bool outOfRange = false;
    QString macChange = "limites.xml";
#ifdef Q_OS_MAC
    macChange =_pathResource;
    macChange.append("limites.xml");
#endif
    if(QFile::exists(macChange)){
        QString fileName = macChange;
        QFile file(fileName);
        QDomDocument settings(fileName);

        //handle non-existent file case (print debug message and return)
        if (!file.open(QIODevice::ReadOnly)){
            std::cout << "Error while reading odin configuration file" << std::endl;
        }
        QString errorMsg;
        if(!settings.setContent(&file, &errorMsg)){ //handle error (print error message on pane for instance)
            std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
            file.close();
        }
        file.close();

        //parse the settings tree and restore state
        QDomElement root = settings.documentElement();
        QDomElement b = root.firstChildElement("minLCDVoltage");
        int minLCDVoltage = b.text().toInt();
        b = root.firstChildElement("maxLCDVoltage");
        int maxLCDVoltage = b.text().toInt();
        b = root.firstChildElement("minUPDelay");
        int minUPDelay = b.text().toInt();
        b = root.firstChildElement("maxUPDelay");
        int maxUPDelay = b.text().toInt();
        b = root.firstChildElement("minDOWNDelay");
        int minDOWNDelay = b.text().toInt();
        b = root.firstChildElement("maxDOWNDelay");
        int maxDOWNDelay = b.text().toInt();
        b = root.firstChildElement("minDelaSync");
        int minDelaSync = b.text().toInt();
        b = root.firstChildElement("maxDelaSync");
        int maxDelaSync = b.text().toInt();
        b = root.firstChildElement("minShutterDelay");
        int minShutterDelay = b.text().toInt();
        b = root.firstChildElement("maxShutterDelay");
        int maxShutterDelay = b.text().toInt();
        b = root.firstChildElement("minDeltaShutter");
        int minDeltaShutter = b.text().toInt();
        b = root.firstChildElement("maxDeltaShutter");
        int maxDeltaShutter = b.text().toInt();

        if(ui->Mng_LCDVolt_spinBox->text().toInt()>maxLCDVoltage || ui->Mng_LCDVolt_spinBox->text().toInt()<minLCDVoltage)outOfRange=true;
        if(ui->Mng_UP_spinBox->text().toInt()>maxUPDelay || ui->Mng_UP_spinBox->text().toInt()<minUPDelay)outOfRange=true;
        if(ui->Mng_DOWN_spinBox->text().toInt()>maxDOWNDelay || ui->Mng_DOWN_spinBox->text().toInt()<minDOWNDelay)outOfRange=true;
        if(ui->Mng_Delta_spinBox->text().toInt()>maxDelaSync || ui->Mng_Delta_spinBox->text().toInt()<minDelaSync)outOfRange=true;
        if(ui->Mng_Shutter_Delay_spinBox->text().toInt()>maxShutterDelay || ui->Mng_Shutter_Delay_spinBox->text().toInt()<minShutterDelay)outOfRange=true;
        if(ui->Mng_Delta_Shutter_spinBox->text().toInt()>maxDeltaShutter || ui->Mng_Delta_Shutter_spinBox->text().toInt()<minDeltaShutter)outOfRange=true;
        ui->Mng_LCDVolt_spinBox->setMinimum(minLCDVoltage);
        ui->Mng_LCDVolt_spinBox->setMaximum(maxLCDVoltage);
        ui->Mng_UP_spinBox->setMinimum(minUPDelay);
        ui->Mng_UP_spinBox->setMaximum(maxUPDelay);
        ui->Mng_DOWN_spinBox->setMinimum(minDOWNDelay);
        ui->Mng_DOWN_spinBox->setMaximum(maxDOWNDelay);
        ui->Mng_Delta_spinBox->setMinimum(minDelaSync);
        ui->Mng_Delta_spinBox->setMaximum(maxDelaSync);
        ui->Mng_Shutter_Delay_spinBox->setMinimum(minShutterDelay);
        ui->Mng_Shutter_Delay_spinBox->setMaximum(maxShutterDelay);
        ui->Mng_Delta_Shutter_spinBox->setMinimum(minDeltaShutter);
        ui->Mng_Delta_Shutter_spinBox->setMaximum(maxDeltaShutter);
    }else{
        if(ui->Mng_LCDVolt_spinBox->text().toInt()>27 || ui->Mng_LCDVolt_spinBox->text().toInt()<2)outOfRange=true;
        if(ui->Mng_UP_spinBox->text().toInt()>1000 || ui->Mng_UP_spinBox->text().toInt()<-1000)outOfRange=true;
        if(ui->Mng_DOWN_spinBox->text().toInt()>1000 || ui->Mng_DOWN_spinBox->text().toInt()<-1000)outOfRange=true;
        if(ui->Mng_Delta_spinBox->text().toInt()>1000 || ui->Mng_Delta_spinBox->text().toInt()<-1000)outOfRange=true;
        ui->Mng_LCDVolt_spinBox->setMinimum(2);
        ui->Mng_LCDVolt_spinBox->setMaximum(27);
        ui->Mng_UP_spinBox->setMinimum(-1000);
        ui->Mng_UP_spinBox->setMaximum(1000);
        ui->Mng_DOWN_spinBox->setMinimum(-1000);
        ui->Mng_DOWN_spinBox->setMaximum(1000);
        ui->Mng_Delta_spinBox->setMinimum(-1000);
        ui->Mng_Delta_spinBox->setMaximum(1000);
        ui->Mng_Shutter_Delay_spinBox->setMinimum(0);
        ui->Mng_Shutter_Delay_spinBox->setMaximum(2000);
        ui->Mng_Delta_Shutter_spinBox->setMinimum(-1000);
        ui->Mng_Delta_Shutter_spinBox->setMaximum(1000);
    }

    if(outOfRange==true)QMessageBox::warning(this,"Range value","Some values was out of range. Min or max was applied.");

}
