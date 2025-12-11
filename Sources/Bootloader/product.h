#ifndef PROGRAM_H
#define PROGRAM_H

//#include <qobject.h>
#include "../Bootloader/Comm.h"
#include "../Bootloader/DeviceData.h"
#include "../Bootloader/DeviceData.h"
#include "../Bootloader/hexfile.h"

#define MEMORY_ADDRESS_PARTNUMBER       0x1000
#define MEMORY_SIZE_PARTNUMBER          0x11
#define MEMORY_ADDRESS_INFORMATIONS     0x1000
#define MEMORY_SIZE_INFORMATIONS        0x33
#define MEMORY_ADDRESS_FIRMWAREREV      0x2000
#define MEMORY_SIZE_FIRMWAREREV         0x11
#define MEMORY_ADDRESS_BOOTLOADERREV    0x0100
#define MEMORY_SIZE_BOOTLOADERREV       0x10
#define MEMORY_ADDRESS_CUSTOMERNUMBER   0x1015
#define MEMORY_SIZE_CUSTOMERNUMBER      0x0F
#define MEMORY_ADDRESS_MANAGER_INFO     0x1011
#define MEMORY_SIZE_MANAGER_INFO        0x04
#define MEMORY_ADDRESS_MANAGER_RF       0x1024
#define MEMORY_SIZE_MANAGER_RF          0x05
#define MEMORY_START                    0x1000


class Product : public QObject
{
    Q_OBJECT

public:
    explicit Product();
    ~Product();

    // surcharge des fonctions de la classe Comm
    int pollUSB(void);
    int pollUSBApp(void);
    void close(void);
    bool isConnected(void);
    bool isConnectedApp(void);
    Comm::ErrorCode open(void);
    Comm::ErrorCode openApp(void);
    Comm::ErrorCode lockUnlockConfig(bool);
    Comm::ErrorCode programParam(unsigned char *pData);
    Comm::ErrorCode readParam(unsigned char *pData);
    Comm::ErrorCode GetBattLevel(unsigned char *pData);
    Comm::ErrorCode inversionGD();

    int connection(void);
    Comm::ErrorCode changePartNumber(QString newOne);
    Comm::ErrorCode changeCustomerNumber(QString newOne);
    Comm::ErrorCode readParamFlash(unsigned char *pData, unsigned char *pData2);
    Comm::ErrorCode programParamFlash(unsigned char *pData);
    Comm::ErrorCode programParamFlash(int, int, unsigned char *pData);
    void resetDevice();
    bool verifyDevice(DeviceData *);
    Comm::ErrorCode readFirmwareData();
    void getQuery();
    void checkUpdate(QString);
    void startRequestFirmware(QUrl);
    int FirmwareFinished();
    Comm::ErrorCode readMemory();
    Comm::ErrorCode writeMemory(QString SerialNumber);
    Comm::ErrorCode eraseDevice(void);
    int LoadFile(QString newFileName);

    QString getProductCustomerNumber();
    QString getProductFirmwareRev();
    QString getProductSerialNumber();
    QString getProductBootloaderRev();
    bool getProductUpdating();
    void setProductUpdating(bool);
    int getProductMachineEtat();
    int getProductPaternIR();
    int getProductGainIR();
    int getProductTypeSynchro();
    bool getProductAutotest();
    bool getProductAbsenceIRDLP();
    int getProductTypeFULLHD3D();
    bool getProductSearchingIR();
    bool getProductSearchingRF();
    unsigned char *getProductMessageRF();
    unsigned char *getProductZoneInfo();


protected:
    Comm* comm;
    DeviceData* deviceData;
    Device* device;
    HexFile* hexFile;
    /*! \brief numero de version actuel du firmware */
    QString m_firmwareRev;

    /*! \brief numero de version bootloader */
    QString m_bootloaderRev;

    /*! \brief numero de version du firmware lors de la derniere connection du device */
    QString m_previousRev;

    /*! \brief numero de serie du device */
    QString m_serialNumber;

    /*! \brief numero de version firmware en sortie d'usine */
    QString m_customerNumber;

    /*! \brief indique une mise a jour soft en cours */
    bool m_updating;

    /*! \brief contenu de la zone info (50 octets utiles) */
    unsigned char m_zoneInformation[64];

    /*! \brief etat de la machine d'etat du device (si applicable) */
    int m_machineEtat;

    /*! \brief patern IR detectee par le device (si applicable) */
    int m_paternIR;

    /*! \brief valeur du gain IR applique par le device (si applicable) */
    int m_gainIR;

    /*! \brief type de synchro detectee par le device (si aplicable) */
    int m_typeSynchro;

    /*! \brief device en mode autotest (si applicable) */
    bool m_autotest;

    /*! \brief Pas d'IR ni de DLP detecte (si applicable) */
    bool m_absenceIRDLP;

    /*! \brief type de la commande FULL HD 3D detecte par le device (si applicable) */
    int m_typeFULLHD3D;

    /*! \brief recherche signal IR en cours (si applicable) */
    bool m_searchingIR;

    /*! \brief recherche signal RF en cours (si applicable) */
    bool m_searchingRF;

    /*! \brief buffer d'echange via RF (si applicable) */
    unsigned char m_messageRF[23];

    /*! \brief longueur utile du buffer RF (si applicable) */
    int m_lengthRF;

private:
    QString correctSyntax(QString text);
    void setConnected(bool value);
    void setConnectedApp(bool value);

signals:
    void httpFinished();
    void httpUpdateFinished();
    void setProgress(int newValue);
    void setMsgToIHM(QString message);
    void newDataAvailable();
    void getDebugMsgSignal(int, unsigned char *);
    void newMsgRFAvailable(unsigned char *);

public slots:
    void httpFinishedSlot();
    void httpFirmwareFinishedSlot();
    void updateProgress(int value);
    void readedDebug(unsigned char *pData);
    void setDebugMsg(int length, unsigned char *buffer);
    void getDebugMsg();
    Comm::ErrorCode writeDevice();

};

#endif // PROGRAM_H
