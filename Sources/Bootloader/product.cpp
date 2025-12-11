#include <QFileDialog>
#include "product.h"

#define MAXIMUM_PROGRAMMABLE_MEMORY_SEGMENT_SIZE 0x0FFFFFFF

/*! ***********************************************
 *  \brief Constructeur
 *
 **************************************************
 *  \param -
 *  \return -
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Product::Product()
{
    comm = new Comm();
    m_updating = false;
    deviceData = new DeviceData();
    device = new Device(deviceData);
    hexFile = new HexFile(device);
    QObject::connect(hexFile, SIGNAL(httpTransferFinished()), this, SLOT(httpFinishedSlot()));
    QObject::connect(hexFile, SIGNAL(httpFirmwareTransferFinished()), this, SLOT(httpFirmwareFinishedSlot()));
    QObject::connect(comm, SIGNAL(SetProgressBar(int)), this, SLOT(updateProgress(int)));
    QObject::connect(comm, SIGNAL(SetReadedDebug(unsigned char *)), this, SLOT(readedDebug(unsigned char *)));
    QObject::connect(this, SIGNAL(getDebugMsgSignal(int, unsigned char*)), comm, SLOT(Get_Msg_Debug(int, unsigned char*)));

    //initialisation des membres si necessaire
    m_lengthRF = 0;
    for(unsigned int i=0; i<sizeof(m_messageRF); i++) m_messageRF[i] = 0;
}

/*! ***********************************************
 *  \brief Destructeur
 *
 **************************************************
 *  \param -
 *  \return -
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Product::~Product()
{
    qDebug("~Product");
    comm->close();
    delete comm;
    delete deviceData;
    delete device;
    delete hexFile;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_machineEtat
 *
 **************************************************
 *  \param -
 *  \return m_machineEtat
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
int Product::getProductMachineEtat()
{
    return m_machineEtat;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_paternIR
 *
 **************************************************
 *  \param -
 *  \return m_paternIR
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
int Product::getProductPaternIR()
{
    return m_paternIR;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_gainIR
 *
 **************************************************
 *  \param -
 *  \return m_gainIR
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
int Product::getProductGainIR()
{
    return m_gainIR;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_typeSynchro
 *
 **************************************************
 *  \param -
 *  \return m_typeSynchro
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
int Product::getProductTypeSynchro()
{
    return m_typeSynchro;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_autotest
 *
 **************************************************
 *  \param -
 *  \return m_autotest
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
bool Product::getProductAutotest()
{
    return m_autotest;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_absenceIRDLP
 *
 **************************************************
 *  \param -
 *  \return m_absenceIRDLP
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
bool Product::getProductAbsenceIRDLP()
{
    return m_absenceIRDLP;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_typeFULLHD3D
 *
 **************************************************
 *  \param -
 *  \return m_typeFULLHD3D
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
int Product::getProductTypeFULLHD3D()
{
    return m_typeFULLHD3D;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_searchingIR
 *
 **************************************************
 *  \param -
 *  \return m_searchingIR
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
bool Product::getProductSearchingIR()
{
    return m_searchingIR;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_searchingRF
 *
 **************************************************
 *  \param -
 *  \return m_searchingRF
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
bool Product::getProductSearchingRF()
{
    return m_searchingRF;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_messageRF
 *
 **************************************************
 *  \param -
 *  \return m_messageRF
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
unsigned char * Product::getProductMessageRF()
{
    return m_messageRF;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_zoneInformation
 *
 **************************************************
 *  \param -
 *  \return m_zoneInformation
 **************************************************
 *  Release :
 *      DDA, 31.01.2014, creation
 **************************************************/
unsigned char * Product::getProductZoneInfo()
{
    return m_zoneInformation;
}

/*! ***********************************************
 *  \brief  Prepare le message pour envoi à la
 *          EDGE RF
 *
 **************************************************
 *  \param length = longueur du message
 *          buffer = data à envoyer
 *  \return -
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
void Product::setDebugMsg(int length, unsigned char *buffer)
{
    qWarning("setDebugMsg");
    m_lengthRF = length;
    if(length!=0)
        for (int i=0; i<length; i++) m_messageRF[i] = buffer[i];
    else
        for (int i=0; i<23; i++) m_messageRF[i] = 0x00;
}

/*! ***********************************************
 *  \brief SLOT d'envoi du message RF
 *
 **************************************************
 *  \param -
 *  \return -
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
void Product::getDebugMsg()
{
    emit getDebugMsgSignal(m_lengthRF, m_messageRF);
}

/*! ***********************************************
 *  \brief Decode le message recu par l'AHU
 *          et si un nouveau msg RF est present
 *          emet le SIGNAL newMsgRFAvailable
 *
 **************************************************
 *  \param pData = buffer contenant le msg reçu
 *                  par l'AHU
 *  \return -
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
void Product::readedDebug(unsigned char *pData)
{
    //pData contient le msg de debug reçu
    m_machineEtat = pData[3];
    m_paternIR =    pData[4];
    m_gainIR =      pData[5];
    m_typeSynchro = pData[6];
    m_autotest =    pData[7];
    m_absenceIRDLP =pData[8];
    m_typeFULLHD3D =pData[9];
    m_searchingIR = pData[10];
    m_searchingRF = pData[11];
    emit newDataAvailable();

    if(pData[14] == 1)
    { //Si nouveau msg RF
        for(int i=0; i<23; i++) m_messageRF[i] = pData[15+i];
        QString temp = (const char*)&pData[16];
        pData[35] = 0x00; //termine la string
        qWarning("msg =" + temp.toLatin1());
        emit newMsgRFAvailable(m_messageRF);
        pData[14] = 0;  // pour indiquer msg lu.
    }
}

/*! ***********************************************
 *  \brief SLOT permettant de faire remonter de
 *          la classe COMM l'etat d'avancement
 *          d'une action
 *
 **************************************************
 *  \param value = % de la tache en cours
 *  \return -
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
void Product::updateProgress(int value)
{
    emit setProgress(value);
}

/*! ***********************************************
 *  \brief Efface le device
 *
 **************************************************
 *  \param -
 *  \return Success si OK, sinon Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::eraseDevice(void)
{
    Comm::ErrorCode result;
    Comm::BootInfo bootInfo;

    qWarning("Erasing Device... (no status update until complete, may take several seconds)");

    result = comm->Erase();
    if(result != Comm::Success)
    {
        qWarning("Erase failed");
        m_updating = false;
        return result;
    }

    result = comm->ReadBootloaderInfo(&bootInfo);
    if(result != Comm::Success)
    {
        qWarning("Bootloader Info after erase failed");
        m_updating = false;
        return result;
    }

    qWarning("Erase done");
    return result;
}

/*! ***********************************************
 *  \brief Effectue une connection en mode bootloader
 *
 **************************************************
 *  \param -
 *  \return TBD
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
int Product::pollUSB(void)
{
    return comm->PollUSB();
}

/*! ***********************************************
 *  \brief Effectue une connection en mode applicatif
 *
 **************************************************
 *  \param -
 *  \return TBD
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
int Product::pollUSBApp(void)
{
    return comm->PollUSBApp();
}

/*! ***********************************************
 *  \brief Ferme la liaison avec le device
 *
 **************************************************
 *  \param -
 *  \return TBD
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
void Product::close(void)
{
    comm->close();
}

/*! ***********************************************
 *  \brief Verifit si un device est connecte en
 *          mode bootloader
 *
 **************************************************
 *  \param -
 *  \return TBD
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
bool Product::isConnected(void)
{
    return comm->isConnected();
}

/*! ***********************************************
 *  \brief Verifit si un device est connecte en
 *          mode applicatif
 *
 **************************************************
 *  \param -
 *  \return TBD
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
bool Product::isConnectedApp(void)
{
    return comm->isConnectedApp();
}

/* Private Function */
void Product::setConnected(bool value)
{
    comm->setConnected(value);
}

/* Private Function */
void Product::setConnectedApp(bool value)
{
    comm->setConnectedApp(value);
}

/*! ***********************************************
 *  \brief Ouvre un device connecte en mode
 *          bootloader
 *
 **************************************************
 *  \param -
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::open(void)
{
    return comm->open();
}

/*! ***********************************************
 *  \brief Ouvre un device connecte en mode
 *          applicatif
 *
 **************************************************
 *  \param -
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::openApp(void)
{
    return comm->openApp();
}

/*! ***********************************************
 *  \brief Lock / Unlock les bits de configuration
 *          du PIC18F
 *
 **************************************************
 *  \param lock = empeche l'ecriture si true
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::lockUnlockConfig(bool lock)
{
    return comm->LockUnlockConfig(lock);
}

/*! ***********************************************
 *  \brief Lecture des parametres du device
 *
 **************************************************
 *  \param pData = Buffer de donnees
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::readParam(unsigned char *pData)
{
    return comm->Read_Param(pData);
}

/*! ***********************************************
 *  \brief Ecriture des parametres du device
 *
 **************************************************
 *  \param pData = Buffer de donnees
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::programParam(unsigned char *pData)
{
    return comm->Program_Param(pData);
}

/*! ***********************************************
 *  \brief Lecture du niveau batterie du device
 *
 **************************************************
 *  \param pData = Buffer de donnees
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 12.02.2014, creation
 **************************************************/
Comm::ErrorCode Product::GetBattLevel(unsigned char *pData)
{
    if( (comm->isConnected()) && (m_serialNumber.contains("EDG")) && (!m_updating) && (m_bootloaderRev.contains("V01.1")))
        return comm->GetBattLevel(pData);
    else
        return Comm::Fail;
}

/*! ***********************************************
 *  \brief Commande d'inversion Gauche / Droite
 *
 **************************************************
 *  \param -
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::inversionGD()
{
    return comm->Inversion_GD();
}

/*! **********************************************
 *  \brief Charge en RAM le fichier HEX a ecrire
 *
 *************************************************
 *  \param newFileName = chemin et nom du fichier
 *  \return 1=OK, 0 sinon
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
int Product::LoadFile(QString newFileName)
{
    return hexFile->LoadFile(newFileName);
}

/*! **********************************************
 *  \brief Lecture des parametres inscrits en FLASH
 *          du device
 *
 *************************************************
 *  \param pData = buffer de donnees 0x1011->0x1014
 *          pData2 = buffer de donnees 0x1024->0x1025
 *  \return see Comm::ErrorCode
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
Comm::ErrorCode Product::readParamFlash(unsigned char *pData, unsigned char *pData2)
{
    Comm::ErrorCode result;
    // read data 0x1011 0x1012 0x1013 0x1014
    result = comm->GetData(MEMORY_ADDRESS_MANAGER_INFO,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           MEMORY_ADDRESS_MANAGER_INFO+MEMORY_SIZE_MANAGER_INFO,
                           pData);
    if(result != Comm::Success)
        return result;

    // read data 0x1024 0x1025 0x1026 0x1027 0x1028
    return comm->GetData(MEMORY_ADDRESS_MANAGER_RF,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           MEMORY_ADDRESS_MANAGER_RF+MEMORY_SIZE_MANAGER_RF,
                           pData2);
}

/*! **********************************************
 *  \brief Ecriture des parametres en FLASH du device
 *
 *************************************************
 *  \param pData = buffer de donnees 0x1011->0x1033
 *  \return see Comm::ErrorCode
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
Comm::ErrorCode Product::programParamFlash(unsigned char *pData)
{
    Comm::ErrorCode result;

    // first : read block 0x1000-0x1400
    DeviceData::MemoryRange testMemory;
    deviceData->ranges.clear();
    testMemory.start = 0x1000;
    testMemory.end = 0x1400;
    testMemory.type = 0x01;

    QString startHexa = "0x1000";
    QString endHexa = "0x1400";
    int ecart = 1024; // 0x1400 - 0x1000
    int loop = ecart/48;

    unsigned char DataToRead[64];
    int values[64];

    QString texte = "";
    for(int i = 0; i<loop;i++){
        comm->GetData(strtoul(startHexa.toStdString().c_str(),NULL,16) + i*48,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               strtoul(startHexa.toStdString().c_str(),NULL,16) + i*48 + 49,
                               (unsigned char *)&DataToRead[0]);

         DataToRead[49]=0x00;

         for(int k = 0; k<48;k++){values[k] = (int)DataToRead[k];texte.append(QString::number(values[k])+"*");}
    }

    comm->GetData(strtoul(startHexa.toStdString().c_str(),NULL,16) + loop*48,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           strtoul(endHexa.toStdString().c_str(),NULL,16) + 1,
                           (unsigned char *)&DataToRead[0]);

    int stop = strtoul(endHexa.toStdString().c_str(),NULL,16) -  strtoul(startHexa.toStdString().c_str(),NULL,16) - loop*0x30;
    DataToRead[strtoul(endHexa.toStdString().c_str(),NULL,16) -  strtoul(startHexa.toStdString().c_str(),NULL,16) - loop*0x30 + 0x01]=0x00;
    for(int k = 0; k<=stop;k++){values[k] = (int)DataToRead[k];texte.append(QString::number(values[k])+"*");}

    QStringList list = QString(texte).split("*");

    bool ok;
    unsigned int wordByte;
    unsigned char* pHexFileBinaryDataBufferStart = NULL;
    pHexFileBinaryDataBufferStart = (unsigned char*)malloc(1024);

    for(int i =0; i<list.size()-2;i++){

        char buf[12];
        if( (i>=17 && i<=20) || (i>=36 && i<=59) ){
            *(pHexFileBinaryDataBufferStart + i) = pData[i-17]; //Save the byte
        }
         else {
            sprintf(buf, "%x", list.at(i).toInt());
            QString tt(buf);
            tt = correctSyntax(tt);
            wordByte = tt.toInt(&ok, 16);
            *(pHexFileBinaryDataBufferStart + i) = (unsigned char)wordByte; //Save the byte
        }
    }
    testMemory.data = new unsigned char[1024];
    memcpy(&testMemory.data[0], pHexFileBinaryDataBufferStart, 1024);
    deviceData->ranges.append(testMemory);

    qDebug("start trying writing device data");

    comm->PollUSB();
    if(!comm->isConnected())
    {
        return Comm::NotConnected;
    }

    //First erase the device.
    result = comm->EraseData();

    foreach(testMemory, deviceData->ranges)
    {
        result = comm->Program(testMemory.start,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               device->family,
                               testMemory.end,
                               testMemory.data);


        if(result != Comm::Success)
        {
            qWarning("Programming data failed");
        }
    }

    return result;

}

/*! **********************************************
 *  \brief Ecriture des parametres en FLASH du device
 *
 *************************************************
 *  \param pData = buffer de donnees 0x1011->0x1033
 *  \return see Comm::ErrorCode
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
Comm::ErrorCode Product::programParamFlash(int start, int size, unsigned char *pData)
{
    Comm::ErrorCode result;

    // first : read block 0x1000-0x1400
    DeviceData::MemoryRange testMemory;
    deviceData->ranges.clear();
    testMemory.start = 0x1000;
    testMemory.end = 0x1400;
    testMemory.type = 0x01;

    QString startHexa = "0x1000";
    QString endHexa = "0x1400";
    int ecart = 1024; // 0x1400 - 0x1000
    int loop = ecart/48;

    unsigned char DataToRead[64];
    int values[64];

    QString texte = "";
    for(int i = 0; i<loop;i++){
        comm->GetData(strtoul(startHexa.toStdString().c_str(),NULL,16) + i*48,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               strtoul(startHexa.toStdString().c_str(),NULL,16) + i*48 + 49,
                               (unsigned char *)&DataToRead[0]);

         DataToRead[49]=0x00;

         for(int k = 0; k<48;k++){values[k] = (int)DataToRead[k];texte.append(QString::number(values[k])+"*");}
    }

    comm->GetData(strtoul(startHexa.toStdString().c_str(),NULL,16) + loop*48,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           strtoul(endHexa.toStdString().c_str(),NULL,16) + 1,
                           (unsigned char *)&DataToRead[0]);

    int stop = strtoul(endHexa.toStdString().c_str(),NULL,16) -  strtoul(startHexa.toStdString().c_str(),NULL,16) - loop*0x30;
    DataToRead[strtoul(endHexa.toStdString().c_str(),NULL,16) -  strtoul(startHexa.toStdString().c_str(),NULL,16) - loop*0x30 + 0x01]=0x00;
    for(int k = 0; k<=stop;k++){values[k] = (int)DataToRead[k];texte.append(QString::number(values[k])+"*");}

    QStringList list = QString(texte).split("*");

    bool ok;
    unsigned int wordByte;
    unsigned char* pHexFileBinaryDataBufferStart = NULL;
    pHexFileBinaryDataBufferStart = (unsigned char*)malloc(1024);

    for(int i =0; i<list.size()-2;i++){

        char buf[12];
        if (i>=start && i<start+size) {
            *(pHexFileBinaryDataBufferStart + i) = pData[i-start]; //Save the byte
        }
         else {
            sprintf(buf, "%x", list.at(i).toInt());
            QString tt(buf);
            tt = correctSyntax(tt);
            wordByte = tt.toInt(&ok, 16);
            *(pHexFileBinaryDataBufferStart + i) = (unsigned char)wordByte; //Save the byte
        }
    }
    testMemory.data = new unsigned char[1024];
    memcpy(&testMemory.data[0], pHexFileBinaryDataBufferStart, 1024);
    deviceData->ranges.append(testMemory);

    qDebug("start trying writing device data");

    comm->PollUSB();
    if(!comm->isConnected())
    {
        return Comm::NotConnected;
    }

    //First erase the device.
    result = comm->EraseData();

    foreach(testMemory, deviceData->ranges)
    {
        result = comm->Program(testMemory.start,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               device->family,
                               testMemory.end,
                               testMemory.data);


        if(result != Comm::Success)
        {
            qWarning("Programming data failed");
        }
    }

    return result;

}

/*! **********************************************
 *  \brief Verifit si une version plus recente du
 *          firmware est presente sur le serveur
 *          Volfoni
 *
 *************************************************
 *  \param -
 *  \return -
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
void Product::checkUpdate(QString filename)
{
    hexFile->CheckUpdate(filename);
}

/*! **********************************************
 *  \brief Telecharge un fichier depuis le serveur
 *          Volfoni
 *
 *************************************************
 *  \param url = adresse internet du fichier
 *  \return -
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
void Product::startRequestFirmware(QUrl url)
{
    hexFile->httpFirmwareUpdate(url);
}

/*! **********************************************
 *  \brief Charge en RAM le fichier de mise à jour
 *          qui vient d'etre telecharge depuis le
 *          serveur Volfoni
 *
 *************************************************
 *  \param -
 *  \return TBD
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
int Product::FirmwareFinished()
{
    return hexFile->FirmwareFinished();
}

/*! **********************************************
 *  \brief Ouvre une liaison avec le device connecte
 *          au PC
 *
 *************************************************
 *  \param -
 *  \return TBD
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
int Product::connection(void)
{
    bool currStatus = comm->isConnected();
    bool currStatusApp = comm->isConnectedApp();
    int result = 0;

    comm->PollUSB();
    comm->PollUSBApp();

    if(currStatus != comm->isConnected() || currStatusApp != comm->isConnectedApp())
    {
        if(comm->isConnected())
        {
            qWarning("Attempting to open device...");
            comm->open();
            if(!m_updating)
            {
                result = 9;
            }

        }
        else if(comm->isConnectedApp())
        {
            qWarning("Attempting to open device...");
            comm->openApp();
            if(!m_updating)
            {
                result = 9;
            }

        }
        else
        {
            qWarning("Closing device.");
            comm->close();
            if(!m_updating)
            {
                result = 1;
            }
        }
    }

    return result;
}

/*! **********************************************
 *  \brief Demande ses informations au device
 *
 *************************************************
 *  \param -
 *  \return -
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
void Product::getQuery()
{
    Comm::BootInfo bootInfo;
    DeviceData::MemoryRange range;

    if(!comm->isConnected() && !comm->isConnectedApp())
    {
        emit setMsgToIHM("Query not sent, device not connected");
        return;
    }

    //Send the Query command to the device over USB, and check the result status.
    switch(comm->ReadBootloaderInfo(&bootInfo))
    {
        case Comm::Fail:
        case Comm::IncorrectCommand:
            emit setMsgToIHM("Unable to communicate with device");
            return;
        case Comm::Timeout:
            emit setMsgToIHM("Operation timed out");
            break;
        case Comm::Success:
            emit setMsgToIHM("Success to communicate with device");
            break;
        default:
            return;
    }

    deviceData->ranges.clear();

    //Now start parsing the bootInfo packet to learn more about the device.  The bootInfo packet contains
    //contains the query response data from the USB device.  We will save these values into globabl variables
    //so other parts of the application can use the info when deciding how to do things.
    device->family = (Device::Families) bootInfo.deviceFamily;
    device->bytesPerPacket = bootInfo.bytesPerPacket;

    //Set some processor family specific global variables that will be used elsewhere (ex: during program/verify operations).
    switch(device->family)
    {
        case Device::PIC18:
            device->bytesPerWordFLASH = 2;
            device->bytesPerAddressFLASH = 1;
            break;
        case Device::PIC24:
            device->bytesPerWordFLASH = 4;
            device->bytesPerAddressFLASH = 2;
            device->bytesPerWordConfig = 4;
            device->bytesPerAddressConfig = 2;
            break;
        case Device::PIC32:
            device->bytesPerWordFLASH = 4;
            device->bytesPerAddressFLASH = 1;
            break;
    default:
            break;
    }

    //Initialize the deviceData buffers and length variables, with the regions that the firmware claims are
    //reprogrammable.  We will need this information later, to decide what part(s) of the .hex file we
    //should look at/try to program into the device.  Data sections in the .hex file that are not included
    //in these regions should be ignored.
    for(int i = 0; i < MAX_DATA_REGIONS; i++)
    {
        if(bootInfo.memoryRegions[i].type == END_OF_TYPES_LIST)
        {
            break;
        }

        //Error check: Check the firmware's reported size to make sure it is sensible.  This ensures
        //we don't try to allocate ourselves a massive amount of RAM (capable of crashing this PC app)
        //if the firmware claimed an improper value.
        if(bootInfo.memoryRegions[i].size > MAXIMUM_PROGRAMMABLE_MEMORY_SEGMENT_SIZE)
        {
            bootInfo.memoryRegions[i].size = MAXIMUM_PROGRAMMABLE_MEMORY_SEGMENT_SIZE;
        }

        //Parse the bootInfo response packet and allocate ourselves some RAM to hold the eventual data to program.
        if(bootInfo.memoryRegions[i].type == PROGRAM_MEMORY)
        {
            range.type = PROGRAM_MEMORY;
            //range.data = new unsigned char[bootInfo.memoryRegions[i].size * device->bytesPerWordFLASH];
            range.data = new unsigned char[bootInfo.memoryRegions[i].size * device->bytesPerAddressFLASH];
            memset(&range.data[0], 0xFF, bootInfo.memoryRegions[i].size * device->bytesPerAddressFLASH);
        }
        else if(bootInfo.memoryRegions[i].type == EEPROM_MEMORY)
        {
            range.type = EEPROM_MEMORY;
            range.data = new unsigned char[bootInfo.memoryRegions[i].size * device->bytesPerAddressEEPROM];
            memset(&range.data[0], 0xFF, bootInfo.memoryRegions[i].size * device->bytesPerAddressEEPROM);
        }
        else if(bootInfo.memoryRegions[i].type == CONFIG_MEMORY)
        {
            range.type = CONFIG_MEMORY;
            range.data = new unsigned char[bootInfo.memoryRegions[i].size * device->bytesPerAddressConfig];
            memset(&range.data[0], 0xFF, bootInfo.memoryRegions[i].size * device->bytesPerAddressConfig);
        }

        //Notes regarding range.start and range.end: The range.start is defined as the starting address inside
        //the USB device that will get programmed.  For example, if the bootloader occupies 0x000-0xFFF flash
        //memory addresses (ex: on a PIC18), then the starting bootloader programmable address would typically
        //be = 0x1000 (ex: range.start = 0x1000).
        //The range.end is defined as the last address that actually gets programmed, plus one, in this programmable
        //region.  For example, for a 64kB PIC18 microcontroller, the last implemented flash memory address
        //is 0xFFFF.  If the last 1024 bytes are reserved by the bootloader (since that last page contains the config
        //bits for instance), then the bootloader firmware may only allow the last address to be programmed to
        //be = 0xFBFF.  In this scenario, the range.end value would be = 0xFBFF + 1 = 0xFC00.
        //When this application uses the range.end value, it should be aware that the actual address limit of
        //range.end does not actually get programmed into the device, but the address just below it does.
        //In this example, the programmed region would end up being 0x1000-0xFBFF (even though range.end = 0xFC00).
        //The proper code to program this would basically be something like this:
        //for(i = range.start; i < range.end; i++)
        //{
        //    //Insert code here that progams one device address.  Note: for PIC18 this will be one byte for flash memory.
        //    //For PIC24 this is actually 2 bytes, since the flash memory is addressed as a 16-bit word array.
        //}
        //In the above example, the for() loop exits just before the actual range.end value itself is programmed.

        range.start = bootInfo.memoryRegions[i].address;
        range.end = bootInfo.memoryRegions[i].address + bootInfo.memoryRegions[i].size;
        //Add the new structure+buffer to the list
        deviceData->ranges.append(range);
    }

    if(!m_updating)
    {
        readFirmwareData();
    }

    return;
}

/*! **********************************************
 *  \brief Lit les informations firmware du device
 *
 *************************************************
 *  \param -
 *  \return see Comm::ErrorCode
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
Comm::ErrorCode Product::readFirmwareData(){//Get the device firmware revision
    unsigned char DataToRead[64];
    Comm::ErrorCode result;

//    emit setMsgToIHM("Checking Device's Program Memory...");

    //----- FIRMWARE REVISION
    result = comm->GetData(MEMORY_ADDRESS_FIRMWAREREV,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           MEMORY_ADDRESS_FIRMWAREREV + MEMORY_SIZE_FIRMWAREREV,
                           (unsigned char *)&DataToRead[0]);

    if(result != Comm::Success)
    {
        comm->setConnected(true);
        qWarning("Error reading firmware data");
 //       emit setMsgToIHM("Error reading firmware data");
        return result;
    }

    DataToRead[MEMORY_SIZE_FIRMWAREREV]=0x00;
    m_firmwareRev = (const char*)&DataToRead[0];
    m_previousRev = m_firmwareRev;

    //----- BOOTLOADER REVISION
    result = comm->GetData(MEMORY_ADDRESS_BOOTLOADERREV,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           MEMORY_ADDRESS_BOOTLOADERREV + MEMORY_SIZE_BOOTLOADERREV,
                           (unsigned char *)&DataToRead[0]);

    if(result != Comm::Success)
    {
        comm->setConnected(true);
        qWarning("Error reading bootloader data.");
        return result;
    }

    DataToRead[MEMORY_SIZE_BOOTLOADERREV]=0x00;
    m_bootloaderRev = (const char*)&DataToRead[0];

    //----- PRODUCT INFORMATIONS
    result = comm->GetData(MEMORY_ADDRESS_INFORMATIONS,
                            device->bytesPerPacket,
                            device->bytesPerAddressFLASH,
                            device->bytesPerWordFLASH,
                            MEMORY_ADDRESS_INFORMATIONS + MEMORY_SIZE_INFORMATIONS,
                            (unsigned char *)&m_zoneInformation[0]);

    if(result != Comm::Success)
    {
        comm->setConnected(true);
        qWarning("Error reading product information data");
        return result;
    }

 /*   //----- PART NUMBER
    result = comm->GetData(MEMORY_ADDRESS_PARTNUMBER,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           MEMORY_ADDRESS_PARTNUMBER + MEMORY_SIZE_PARTNUMBER,
                           (unsigned char *)&DataToRead[0]);

    if(result != Comm::Success)
    {
        comm->setConnected(true);
        qWarning("Error reading firmware data");
//        emit setMsgToIHM("Error reading firmware data");
        return result;
    }

    DataToRead[MEMORY_SIZE_PARTNUMBER]=0x00;
    m_serialNumber = (const char*)&DataToRead[0];*/

    m_serialNumber = QString((const char*)&m_zoneInformation[0]).left(17);

/*    //----- Customer NUMBER
    result = comm->GetData(MEMORY_ADDRESS_CUSTOMERNUMBER,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           MEMORY_ADDRESS_CUSTOMERNUMBER + MEMORY_SIZE_CUSTOMERNUMBER,
                           (unsigned char *)&DataToRead[0]);

    if(result != Comm::Success)
    {
        comm->setConnected(true);
        qWarning("Error reading firmware data");
 //       emit setMsgToIHM("Error reading firmware data");
        return result;
    }

    DataToRead[MEMORY_SIZE_CUSTOMERNUMBER]=0x00;
    m_customerNumber = (const char*)&DataToRead[0];*/
    m_customerNumber = QString((const char*)&m_zoneInformation[0x15]).left(15);

//    emit setMsgToIHM("Checking completed");
    emit setProgress(100);
    return result;
}

/*! **********************************************
 *  \brief Reset le device
 *
 *************************************************
 *  \param -
 *  \return -
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
void Product::resetDevice()
{
    if(!comm->isConnected() && !comm->isConnectedApp())
    {
        qWarning("Reset not sent, device not connected");
        emit setMsgToIHM("Reset not sent, device not connected");
        return;
    }

    qWarning("Resetting...");
    emit setMsgToIHM("Resetting...");
    comm->Reset();
}

/*! **********************************************
 *  \brief Ecrit le fichier charge en RAM dans la
 *          FLASH du device
 *
 *************************************************
 *  \param -
 *  \return see Comm::ErrorCode
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
Comm::ErrorCode Product::writeDevice(){

    Comm::ErrorCode result = Comm::Fail;
    DeviceData::MemoryRange hexRange;
    DeviceData *hexData;

    m_updating = true;

    hexData = hexFile->getHexFileHexData();

    emit setProgress(3);

    comm->PollUSB();
    if(!comm->isConnected())
    {
        qWarning("writeDevice aborted, device not connected");
//        emit setMsgToIHM("writeDevice aborted, device not connected");
        m_updating = false;
        comm->setConnected(true);
        result = Comm::NotConnected;
        return result;
    }


    qWarning("start trying writing device program");

    //First erase the entire device.
    eraseDevice();

    //Now being re-programming each section based on the info we obtained when
    //we parsed the user's .hex file.

    if(m_updating==true)
    {
        foreach(hexRange, hexData->ranges)
        {
            if(hexRange.type == PROGRAM_MEMORY)
            {
                qWarning("Writing Device Program Memory...");
                emit setMsgToIHM("Writing Device Program Memory...");

                result = comm->Program(hexRange.start,
                                       device->bytesPerPacket,
                                       device->bytesPerAddressFLASH,
                                       device->bytesPerWordFLASH,
                                       device->family,
                                       hexRange.end,
                                       hexRange.data);
            }
            else
            {
                continue;
            }

            qWarning("Writing done");
            emit setMsgToIHM("Writing done");

            if(result != Comm::Success)
            {
                m_updating = false;
                qWarning("Programming failed");
                emit setMsgToIHM("Programming failed");
                break;
            }
        }
        if(m_updating==true)
        {
            bool test = verifyDevice(hexData);
            if(test== true)
            {
                m_updating = false;
            }
            else
            {
                m_updating = false;
                comm->setConnected(true);
            }
        }
        else
        {
            m_updating = false;
            comm->setConnected(true);
        }
    }
    else
    {
        m_updating = false;
        comm->setConnected(true);
        result = Comm::Fail;
    }

    emit setProgress(100);
    emit httpUpdateFinished();
    return result;
}

/*! **********************************************
 *  \brief Vérifit que le fichier charge en RAM
 *          est bien programme en FLASH du device
 *
 *************************************************
 *  \param -
 *  \return true = OK, false sinon
 *************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 *************************************************/
bool Product::verifyDevice(DeviceData *hexData)
{
    Comm::ErrorCode result;
    DeviceData::MemoryRange deviceRange, hexRange;
    unsigned int i, j;

    foreach(deviceRange, deviceData->ranges)
    {
        if(deviceRange.type == PROGRAM_MEMORY)
        {
            qWarning("Verifying Device's Program Memory...");
            emit setMsgToIHM("Verifying Device's Program Memory...");

            result = comm->GetData(deviceRange.start,
                                   device->bytesPerPacket,
                                   device->bytesPerAddressFLASH,
                                   device->bytesPerWordFLASH,
                                   deviceRange.end,
                                   deviceRange.data);

            if(result != Comm::Success)
            {
                qWarning("Error reading device.");
                emit setMsgToIHM("Error reading device.");
            }

            //Search through all of the programmable memory regions from the parsed .hex file data.
            //For each of the programmable memory regions found, if the region also overlaps a region
            //that was included in the device programmed area (which just got read back with GetData()),
            //then verify both the parsed hex contents and read back data match.
            foreach(hexRange, hexData->ranges)
            {
                if(deviceRange.start == hexRange.start)
                {
                    //For this entire programmable memory address range, check to see if the data read from the device exactly
                    //matches what was in the hex file.
                    for(i = deviceRange.start; i < deviceRange.end; i++)
                    {
                        //For each byte of each device address (1 on PIC18, 2 on PIC24, since flash memory is 16-bit WORD array)
                        for(j = 0; j < device->bytesPerAddressFLASH; j++)
                        {
                            //Check if the device response data matches the data we parsed from the original input .hex file.
                            if(deviceRange.data[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j] != hexRange.data[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j])
                            {
                                //A mismatch was detected.

                                //Check if this is a PIC24 device and we are looking at the "phantom byte"
                                //(upper byte [j = 1] of odd address [i%2 == 1] 16-bit flash words).  If the hex data doesn't match
                                //the device (which should be = 0x00 for these locations), this isn't a real verify
                                //failure, since value is a don't care anyway.  This could occur if the hex file imported
                                //doesn't contain all locations, and we "filled" the region with pure 0xFFFFFFFF, instead of 0x00FFFFFF
                                //when parsing the hex file.
                                if((device->family == Device::PIC24) && ((i % 2) == 1) && (j == 1))
                                {
                                    //Not a real verify failure, phantom byte is unimplemented and is a don't care.
                                }
                                else
                                {
                                    //If the data wasn't a match, and this wasn't a PIC24 phantom byte, then if we get
                                    //here this means we found a true verify failure.
                                    if(device->family == Device::PIC24)
                                    {
                                        qWarning("Device: 0x%x Hex: 0x%x", *(unsigned short int*)&deviceRange.data[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j], *(unsigned short int*)&hexRange.data[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j]);
                                    }
                                    else
                                    {
                                        qWarning("Device: 0x%x Hex: 0x%x", deviceRange.data[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j], hexRange.data[((i - deviceRange.start) * device->bytesPerAddressFLASH)+j]);
                                    }
                                    qWarning("Failed verify at address 0x%x", i);
                                    return false;
                                }
                            }
                        }
                    }
                }
            }//foreach(hexRange, hexData->ranges)
            qWarning("Verifying success");
        }//if(writeFlash && (deviceRange.type == PROGRAM_MEMORY))
        else
        {
            continue;
        }
    }//foreach(deviceRange, deviceData->ranges)
    qWarning("Erase/Program/Verify Completed Successfully.");
    emit setMsgToIHM("Erase/Program/Verify Completed Successfully.");
    emit setProgress(100);
    return true;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_customerNumber
 *
 **************************************************
 *  \param -
 *  \return m_customerNumber
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
QString Product::getProductCustomerNumber()
{
    return m_customerNumber;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_firmwareRev
 *
 **************************************************
 *  \param -
 *  \return m_firmwareRev
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
QString Product::getProductFirmwareRev()
{
    return m_firmwareRev;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_bootloaderRev
 *
 **************************************************
 *  \param -
 *  \return m_bootloaderRev
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
QString Product::getProductBootloaderRev()
{
    return m_bootloaderRev;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_serialNumber
 *
 **************************************************
 *  \param -
 *  \return m_serialNumber
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
QString Product::getProductSerialNumber()
{
    return m_serialNumber;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_updating
 *
 **************************************************
 *  \param -
 *  \return m_updating
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
bool Product::getProductUpdating()
{
    return m_updating;
}

/*! ***********************************************
 *  \brief Fonction d'acces au membre m_updating
 *
 **************************************************
 *  \param value = valeur a ecrire dans m_updating
 *  \return -
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
void Product::setProductUpdating(bool value)
{
    m_updating = value;
}

void Product::httpFinishedSlot()
{
    emit httpFinished();
}

void Product::httpFirmwareFinishedSlot()
{
//    writeDevice();
    if( comm->isConnectedApp() )
     {
         resetDevice();
         QTimer *timer = new QTimer(this);
         connect(timer, SIGNAL(timeout()), this, SLOT(writeDevice()));
         timer->setSingleShot(true);
         timer->start(5000);
     }
     else
     {
         writeDevice();
     }

}

/*! ***********************************************
 *  \brief Fonction qui change le numero de série
 *          du device
 *
 **************************************************
 *  \param newOne = string contenant le numero a ecrire
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::changePartNumber(QString newOne){

    DeviceData::MemoryRange testMemory;
    deviceData->ranges.clear();
    testMemory.start = 0x1000;
    testMemory.end = 0x1400;
    testMemory.type = 0x01;

    QString newHexa = "";
    QString toCopy[100];
    for(int i = 0; i<newOne.size(); i++){
        int n = newOne.at(i).toLatin1();

        char buf[12];
        sprintf(buf, "%x", n);
        QString tt(buf);

        tt = correctSyntax(tt);
        tt.replace("00","FF");
        newHexa.append(tt);
        toCopy[i]=tt;
    }

    QString startHexa = "0x1000";
    QString endHexa = "0x1400";
    int ecart = 1024; // 0x1400 - 0x1000
    int loop = ecart/48;

    unsigned char DataToRead[64];
    int values[64];
    Comm::ErrorCode result;

    QString texte = "";
    for(int i = 0; i<loop;i++){
        result = comm->GetData(strtoul(startHexa.toStdString().c_str(),NULL,16) + i*48,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               strtoul(startHexa.toStdString().c_str(),NULL,16) + i*48 + 49,
                               (unsigned char *)&DataToRead[0]);

        if(result != Comm::Success)
        {
            return result;
        }

        DataToRead[49]=0x00;

        for(int k = 0; k<48;k++){values[k] = (int)DataToRead[k];texte.append(QString::number(values[k])+"*");}
    }

    result = comm->GetData(strtoul(startHexa.toStdString().c_str(),NULL,16) + loop*48,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           strtoul(endHexa.toStdString().c_str(),NULL,16) + 1,
                           (unsigned char *)&DataToRead[0]);

    if(result != Comm::Success)
    {
        return result;
    }

    int stop = strtoul(endHexa.toStdString().c_str(),NULL,16) -  strtoul(startHexa.toStdString().c_str(),NULL,16) - loop*0x30;
    DataToRead[strtoul(endHexa.toStdString().c_str(),NULL,16) -  strtoul(startHexa.toStdString().c_str(),NULL,16) - loop*0x30 + 0x01]=0x00;
    for(int k = 0; k<=stop;k++){values[k] = (int)DataToRead[k];texte.append(QString::number(values[k])+"*");}

    QStringList list = QString(texte).split("*");

    bool ok;
    unsigned int wordByte;
    unsigned char* pHexFileBinaryDataBufferStart = NULL;
    pHexFileBinaryDataBufferStart = (unsigned char*)malloc(1024);

    for(int i =0; i<list.size()-2;i++){

        char buf[12];
        if(i<17){
            QString hexByte = toCopy[i];  //Fetch two ASCII data payload bytes from the .hex file
            wordByte = hexByte.toInt(&ok, 16);
            *(pHexFileBinaryDataBufferStart + i) = (unsigned char)wordByte; //Save the byte
        }
        else {
            sprintf(buf, "%x", list.at(i).toInt());
            QString tt(buf);
            tt = correctSyntax(tt);
            wordByte = tt.toInt(&ok, 16);
            *(pHexFileBinaryDataBufferStart + i) = (unsigned char)wordByte; //Save the byte
        }
    }
    testMemory.data = new unsigned char[1024];
    memcpy(&testMemory.data[0], pHexFileBinaryDataBufferStart, 1024);
    deviceData->ranges.append(testMemory);

    qDebug("start trying writing device program");

    comm->PollUSB();
    if(!comm->isConnected())
    {
        emit setMsgToIHM("WriteData aborted, device not connected");
        return  Comm::NotConnected;
    }
    //First erase the entire device.
    emit setMsgToIHM("Erasing Device... (no status update until complete, may take several seconds)");
    comm->EraseData();

    foreach(testMemory, deviceData->ranges)
    {
        result = comm->Program(testMemory.start,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               device->family,
                               testMemory.end,
                               testMemory.data);


        if(result != Comm::Success)
        {
            qWarning("Programming data failed");
            emit setMsgToIHM("Programming data failed");
            return result;
        }
    }

    emit setProgress(100);
    return result;
}

/*! ***********************************************
 *  \brief Fonction qui change le customer number
 *          du device
 *
 **************************************************
 *  \param newOne = string contenant le numero a ecrire
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::changeCustomerNumber(QString newOne){

    DeviceData::MemoryRange testMemory;
    deviceData->ranges.clear();
    testMemory.start = 0x1000;
    testMemory.end = 0x1400;
    testMemory.type = 0x01;

    QString newHexa = "";
    QString toCopy[100];
    for(int i = 0; i<newOne.size(); i++){
        int n = newOne.at(i).toLatin1();

        char buf[12];
        sprintf(buf, "%x", n);
        QString tt(buf);

        tt = correctSyntax(tt);
        tt.replace("00","FF");
        newHexa.append(tt);
        toCopy[i]=tt;
    }

    QString startHexa = "0x1000";
    QString endHexa = "0x1400";
    int ecart = 1024; // 0x1400 - 0x1000
    int loop = ecart/48;

    unsigned char DataToRead[64];
    int values[64];
    Comm::ErrorCode result;

    QString texte = "";
    for(int i = 0; i<loop;i++){
        result = comm->GetData(strtoul(startHexa.toStdString().c_str(),NULL,16) + i*48,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               strtoul(startHexa.toStdString().c_str(),NULL,16) + i*48 + 49,
                               (unsigned char *)&DataToRead[0]);

        if(result != Comm::Success)
        {
            return result;
        }

        DataToRead[49]=0x00;

        for(int k = 0; k<48;k++){values[k] = (int)DataToRead[k];texte.append(QString::number(values[k])+"*");}
    }

    result = comm->GetData(strtoul(startHexa.toStdString().c_str(),NULL,16) + loop*48,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           strtoul(endHexa.toStdString().c_str(),NULL,16) + 1,
                           (unsigned char *)&DataToRead[0]);

    if(result != Comm::Success)
    {
        return result;
    }

    int stop = strtoul(endHexa.toStdString().c_str(),NULL,16) -  strtoul(startHexa.toStdString().c_str(),NULL,16) - loop*0x30;
    DataToRead[strtoul(endHexa.toStdString().c_str(),NULL,16) -  strtoul(startHexa.toStdString().c_str(),NULL,16) - loop*0x30 + 0x01]=0x00;
    for(int k = 0; k<=stop;k++){values[k] = (int)DataToRead[k];texte.append(QString::number(values[k])+"*");}

    QStringList list = QString(texte).split("*");

    bool ok;
    unsigned int wordByte;
    unsigned char* pHexFileBinaryDataBufferStart = NULL;
    pHexFileBinaryDataBufferStart = (unsigned char*)malloc(1024);

    int z = 0;
    for(int i =0; i<list.size()-2;i++){

        char buf[12];
        if(i>20 && i<36){
            QString hexByte = toCopy[z];  //Fetch two ASCII data payload bytes from the .hex file
            wordByte = hexByte.toInt(&ok, 16);
            *(pHexFileBinaryDataBufferStart + i) = (unsigned char)wordByte; //Save the byte
            z++;
        }
        else {
            sprintf(buf, "%x", list.at(i).toInt());
            QString tt(buf);
            tt = correctSyntax(tt);
            wordByte = tt.toInt(&ok, 16);
            *(pHexFileBinaryDataBufferStart + i) = (unsigned char)wordByte; //Save the byte
        }
    }
    testMemory.data = new unsigned char[1024];
    memcpy(&testMemory.data[0], pHexFileBinaryDataBufferStart, 1024);
    deviceData->ranges.append(testMemory);

    qDebug("start trying writing device program");

    comm->PollUSB();
    if(!comm->isConnected())
    {
        emit setMsgToIHM("WriteData aborted, device not connected");
        return Comm::NotConnected;
    }

    //First erase the entire device.
    emit setMsgToIHM("Erasing Device... (no status update until complete, may take several seconds)");
    comm->EraseData();

    foreach(testMemory, deviceData->ranges)
    {
        result = comm->Program(testMemory.start,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               device->family,
                               testMemory.end,
                               testMemory.data);

        if(result != Comm::Success)
        {
            emit setMsgToIHM("Programming data failed");
            return result;
        }
    }

    emit setProgress(100);
    return result;
}

/*! ***********************************************
 *  \brief Lecture en memoire du device de l'adresse
 *          0x1000 à 0x7BFF (taille = 0x6C00)
 *
 **************************************************
 *  \param -
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::readMemory()
{
    /* Lecture en memoire du device de l'adresse 0x1000 a 0x7BFF : taille = 0x6C00  */
    /* La lecture s'effectue par paquet de 48 octets, il en faut donc 576.          */
    int loop = 576;

    unsigned char DataToRead[64];
    Comm::ErrorCode result;

    QString filename = QFileDialog::getSaveFileName(NULL, tr("Save File"), QDir::rootPath(), "Binary (*.hex)"  );

    QString texte = "";

    emit setMsgToIHM("Dumping Device's Program Memory...");
    emit setProgress(3);

    for(int i = 0; i<loop;i++){

        result = comm->GetData(MEMORY_START + i*48,
                               device->bytesPerPacket,
                               device->bytesPerAddressFLASH,
                               device->bytesPerWordFLASH,
                               MEMORY_START + i*48 + 49,
                               (unsigned char *)&DataToRead[0]);

        if(result != Comm::Success)
        {
            emit setMsgToIHM("Error during dumping.");
            return result;
        }

        for(int k = 0; k<48;k++) texte.append(DataToRead[k]);
     }

    QFile fichier(filename);
    fichier.open(QIODevice::WriteOnly);
    QTextStream flux(&fichier);
    flux.setCodec("ISO 8859-1");  //necessaire pour data binaire
    flux <<texte;

    emit setMsgToIHM("Done.");
    emit setProgress(100);
    return result;
}

/*! ***********************************************
 *  \brief Ecriture en memoire du device de l'adresse
 *          0x1000 à 0x7BFF (taille = 0x6C00)
 *
 **************************************************
 *  \param -
 *  \return see Comm::ErrorCode
 **************************************************
 *  Release :
 *      DDA, 23.01.2013, creation
 **************************************************/
Comm::ErrorCode Product::writeMemory(QString SerialNumber)
{
    Comm::ErrorCode result;
    QString filename = QFileDialog::getOpenFileName(NULL, tr("Open File"), QDir::rootPath(), "Binary (*.hex)"  );
    QFile fichier(filename);
    fichier.open(QIODevice::ReadOnly);

    char* pHexBinaryData = NULL;
    pHexBinaryData = (char*)malloc(fichier.size());

    QDataStream flux(&fichier);
    flux.readRawData(pHexBinaryData,fichier.size());

    fichier.close();

    //Recopie du numero de serie
    for(int i=0; i<17; i++) pHexBinaryData[i] = SerialNumber.at(i).toLatin1();

    emit setMsgToIHM("Erasing Device... (no status update until complete, may take several seconds)");
    emit setProgress(3);
    comm->EraseData();
    eraseDevice();

    emit setMsgToIHM("Writing Device's Program Memory...");
    emit setProgress(50);

    result = comm->Program(MEMORY_START,
                           device->bytesPerPacket,
                           device->bytesPerAddressFLASH,
                           device->bytesPerWordFLASH,
                           device->family,
                           0x7C00,
                           (unsigned char*) pHexBinaryData);

    if(result != Comm::Success)
    {
        emit setMsgToIHM("Error during writing.");
        return result;
    }

    emit setMsgToIHM("Done.");
    emit setProgress(100);
    return result;
}

QString Product::correctSyntax(QString text){
    QString tempo = text;
    tempo = tempo.toUpper();
    if(tempo.size()==1)tempo = "0"+tempo;
    if(tempo.startsWith("8") ||
            tempo.startsWith("9") ||
            tempo.startsWith("A") ||
            tempo.startsWith("B") ||
            tempo.startsWith("C") ||
            tempo.startsWith("D") ||
            tempo.startsWith("E") ||
            tempo.startsWith("F") )tempo="FF";

    return tempo;
}

