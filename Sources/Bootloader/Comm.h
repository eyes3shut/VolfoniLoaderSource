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
*
* Author        Date        Comment
*************************************************************************
* T. Lawrence  2011/01/24  Initial code ported from AN1310.
************************************************************************/

#ifndef COMM_H
#define COMM_H

#include <QThread>
#include <QTimer>

#include "../HIDAPI/hidapi.h"
#include "Device.h"

// Device Vendor and Product IDs
#define VID 0x04d8
#define PID 0x003C // mode bootloader
#define PID2 0x0033 // mode applicatif

// Packet commands
/*#define QUERY_DEVICE        0x02
#define UNLOCK_CONFIG       0x03
#define ERASE_DEVICE        0x04
#define PROGRAM_DEVICE      0x05
#define PROGRAM_COMPLETE    0x06
#define GET_DATA            0x07
#define RESET_DEVICE        0x08
#define GET_MSG             0x0B
#define PROGRAM_PARAM       0x11
#define READ_PARAM          0x12
#define INVERS_GD           0x13
*/
// Maximum number of memory regions that can be bootloaded
#define MAX_DATA_REGIONS    0x06

/*!
 * Provides low level HID bootloader communication.
 */
class Comm : public QObject
{
    Q_OBJECT

signals:
    void SetProgressBar(int newValue);
    void SetReadedDebug(unsigned char *dataDebug);
public slots:
    void Get_Msg_Debug(int length, unsigned char* buffer);
private:


protected:
    hid_device *boot_device;
    hid_device *boot_deviceApp;
    bool connected;
    bool connectedApp;

public:

    explicit Comm();
    ~Comm();

    static const int SyncWaitTime;

    enum ErrorCode
    {
        Success = 0, NotConnected, Fail, IncorrectCommand, Timeout, NoData, Other = 0xFF
    };

    enum PacketCmd
    {
        QUERY_DEVICE = 0x02,
        UNLOCK_CONFIG = 0x03,
        ERASE_DEVICE = 0x04,
        PROGRAM_DEVICE = 0x05,
        PROGRAM_COMPLETE = 0x06,
        GET_DATA = 0x07,
        RESET_DEVICE = 0x08,
        GET_BATT_LEVEL = 0x0A,
        GET_MSG = 0x0B,
        ERASE_DATA = 0x10,
        PROGRAM_PARAM = 0x11,
        READ_PARAM = 0x12,
        INVERS_GD = 0x13,
        READ_IP = 0x14,
        WRITE_IP = 0x15,
        RESTART_DEVICE = 0x16,
        GET_EEPROM = 0x17,
        GET_DATE = 0x18,
        SET_DATE = 0x19,
        ERASE_EEPROM = 0x1A,
        TEST_1 = 0x1B
    };

    QString ErrorString(ErrorCode errorCode) const;

    #pragma pack(1)
#ifdef Q_OS_MAC
    struct MemoryRegion
    {
        unsigned char type;
        unsigned int address;
        unsigned int size;
    };
#else
    struct MemoryRegion
    {
        unsigned char type;
        unsigned long int address;
        unsigned long int size;
    };
#endif

    struct BootInfo
    {
        unsigned char command;
        unsigned char bytesPerPacket;
        unsigned char deviceFamily;
        MemoryRegion memoryRegions[MAX_DATA_REGIONS];
        unsigned char pad[8];
    };
#ifdef Q_OS_MAC
    struct WritePacket
    {
        unsigned char report;
        unsigned char command;
        union {
            unsigned int address;
            unsigned char LockedValue;
        };
        unsigned char bytesPerPacket;
        unsigned char data[58];
    };
    struct ReadPacket
    {
        unsigned char command;
        unsigned int address;
        unsigned char bytesPerPacket;
        unsigned char data[59];
    };
#else
    struct WritePacket
    {
        unsigned char report;
        unsigned char command;
        union {
            unsigned long int address;
            unsigned char LockedValue;
        };
        unsigned char bytesPerPacket;
        unsigned char data[58];
    };
    struct ReadPacket
    {
        unsigned char command;
        unsigned long int address;
        unsigned char bytesPerPacket;
        unsigned char data[59];
    };
#endif

    #pragma pack()

    int PollUSB(void);
    int PollUSBApp(void);

    ErrorCode open(void);
    ErrorCode openApp(void);

    void close(void);
    void closeApp(void);
    bool isConnected(void);
    bool isConnectedApp(void);
    void setConnected(bool value);
    void setConnectedApp(bool value);
    void Reset(void);

    ErrorCode GetData(unsigned long address, unsigned char bytesPerPacket, unsigned char bytesPerAddress,
                      unsigned char bytesPerWord, unsigned long endAddress, unsigned char *data);
    ErrorCode Program(unsigned long address, unsigned char bytesPerPacket, unsigned char bytesPerAddress,
                      unsigned char bytesPerWord, unsigned char deviceFamily, unsigned long endAddress, unsigned char *data);
    ErrorCode Erase(void);
    ErrorCode EraseData(void);
    ErrorCode LockUnlockConfig(bool lock);
    ErrorCode ReadBootloaderInfo(BootInfo* bootInfo);
    ErrorCode SendPacket(unsigned char *data, int size);
    ErrorCode ReceivePacket(unsigned char *data, int size);
    ErrorCode SendPacketApp(unsigned char *data, int size);
    ErrorCode ReceivePacketApp(unsigned char *data, int size);
    ErrorCode Get_Msg(unsigned char *data);
    ErrorCode GetDate(unsigned char *pData);
    ErrorCode SetDate(unsigned char *pData);
    ErrorCode GetBattLevel(unsigned char *pData);
    ErrorCode GetEeprom(unsigned char page, unsigned char *pData);

    ErrorCode Program_Param(unsigned char *data);
    ErrorCode Read_Param(unsigned char *data);
    ErrorCode Inversion_GD(void);
    ErrorCode Program_IP(unsigned char *data);
    ErrorCode Read_IP(unsigned char *data);
    ErrorCode EraseEeprom(void);

};

#endif // COMM_H
