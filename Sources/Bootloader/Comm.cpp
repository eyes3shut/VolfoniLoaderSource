/************************************************************************
* Copyright (c) 2010-2011,  Microchip Technology Inc.
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
* T. Lawrence   2011/01/14  Initial Version.
************************************************************************/

#include "Comm.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QTime>
#include <iostream>

const int Comm::SyncWaitTime = 2000;

/**
 *
 */
Comm::Comm()
{
    connected = false;
    connectedApp = false;
    boot_device = NULL;
    boot_deviceApp = NULL;
}

/**
 *
 */
Comm::~Comm()
{
    boot_device = NULL;
    boot_deviceApp = NULL;
}

/**
 *
 */
int Comm::PollUSB()
{
    hid_device_info *dev;

    dev = hid_enumerate(VID, PID);

    connected = (dev != NULL);

    ///////////////////////////////////////////////////
    //compte combien sont branchés
    int count = 0;
    struct hid_device_info *d = dev;
    while (d) {
        struct hid_device_info *next = d->next;
        d = next;
        count++;
    }
    ////////////////////////////////////////////////////
    hid_free_enumeration(dev);
    /*////////////////////
    // mode applicatif

    dev = hid_enumerate(VID, PID2);

    connectedApp = (dev != NULL);

    struct hid_device_info *d2 = dev;
    while (d2) {
        struct hid_device_info *next = d2->next;
        d2 = next;
        count++;
    }

    hid_free_enumeration(dev);*/
    ////////////////////////////////////////////////////

    return count;
}

/**
 *
 */
int Comm::PollUSBApp()
{
    ////////////////////
    // mode applicatif

    hid_device_info *dev;

    dev = hid_enumerate(VID, PID2);

    connectedApp = (dev != NULL);

    int count = 0;
    struct hid_device_info *d2 = dev;
    while (d2) {
        struct hid_device_info *next = d2->next;
        d2 = next;
        count++;
    }

    hid_free_enumeration(dev);
    ////////////////////////////////////////////////////

    return count;
}
/**
 *
 */
bool Comm::isConnected(void)
{
    return connected;
}

bool Comm::isConnectedApp(void)
{
    return connectedApp;
}

void Comm::setConnected(bool value){
    connected = value;
}

void Comm::setConnectedApp(bool value){
    connectedApp = value;
}

/**
 *
 */
Comm::ErrorCode Comm::open(void)
{
    boot_device = hid_open(VID, PID, NULL);
    if(boot_device)
    {
        connected = true;
        hid_set_nonblocking(boot_device, true);
        qWarning("Device successfully connected to.");
        return Success;
    }

    qWarning("Unable to open device.");
    return NotConnected;
}

Comm::ErrorCode Comm::openApp(void)
{
    boot_deviceApp = hid_open(VID, PID2, NULL);
    if(boot_deviceApp)
    {
        connectedApp = true;
        hid_set_nonblocking(boot_deviceApp, true);
        qWarning("Device successfully connected to.");
        return Success;
    }
    qWarning("Unable to open device.");
    return NotConnected;
}

/**
 *
 */
void Comm::close(void)
{
    hid_close(boot_device);
    hid_close(boot_deviceApp);
    boot_device = NULL;
    boot_deviceApp = NULL;
    connected = false;
    connectedApp = false;
}

/**
 *
 */
void Comm::Reset(void)
{
    unsigned char sendPacket[65];
    QTime elapsed;
    ErrorCode status = Comm::Fail;

    if(connected || connectedApp) {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket[1] = RESET_DEVICE;


        qDebug("Sending Reset Command...");
        elapsed.start();

        if(connected)status = SendPacket(sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp(sendPacket, sizeof(sendPacket));

        if(status == Comm::Success)
            qDebug("Successfully sent reset command (%fs)", (double)elapsed.elapsed() / 1000);
        else
            qWarning("Sending reset command failed.");
    }
}

/**
 *
 */
Comm::ErrorCode Comm::Program(unsigned long address, unsigned char bytesPerPacket,
                              unsigned char bytesPerAddress, unsigned char bytesPerWord, unsigned char deviceFamily,
                              unsigned long endAddress, unsigned char *pData)
{
    WritePacket writePacket;
    ErrorCode result = Success;
    unsigned int i;
    bool allPayloadBytesFF;
    bool firstAllFFPacketFound = false;
    unsigned int bytesToSend;
    unsigned char lastCommandSent = PROGRAM_DEVICE;
    unsigned int percentCompletion;
    unsigned int addressesToProgram;
    unsigned int startOfDataPayloadIndex;

    //Error check input parameters before using them
    if((pData == NULL) || (bytesPerAddress == 0) || (address > endAddress) || (bytesPerWord == 0))
    {
        qWarning("Bad parameters specified when calling Program() function.");
        return Fail;
    }

    //Error check to make sure the requested maximum data payload size is an exact multiple of the bytesPerAddress.
    //If not, shrink the number of bytes we actually send, so that it is always an exact multiple of the
    //programmable media bytesPerAddress.  This ensures that we don't "half" program any memory address (ex: if each
    //flash address is a 16-bit word address, we don't want to only program one byte of the address, we want to program
    //both bytes.
    while((bytesPerPacket % bytesPerWord) != 0)
    {
        bytesPerPacket--;
    }

    //Setup variable, used for progress bar updating computations.
    addressesToProgram = endAddress - address;
    if(addressesToProgram == 0) //Check to avoid potential divide by zero later.
        addressesToProgram++;


    //Make sure the device is still connected before we start trying to communicate with it.
    if(connected || connectedApp)
    {
        //Loop through the entire data set/region, but break it into individual packets before sending it
        //to the device.
        while(address < endAddress)
        {
            //Update the progress bar so the user knows things are happening.
            percentCompletion = 100*((float)1 - (float)((float)(endAddress - address)/(float)addressesToProgram));
            if(percentCompletion > 100)
            {
                percentCompletion = 100;
            }
            //Reformat the percent completion so it "fits" in the 33% to 66% region (since erase
            //"completes" 0%-32% of the total erase/program/verify cycle, and verify completes 67%-100%).
            percentCompletion /= 3;
            percentCompletion += 33;
            emit SetProgressBar(percentCompletion);

            //Prepare the packet to send to the device.
            memset((void*)&writePacket, 0x00, sizeof(writePacket)); //initialize all bytes clear, so unused pad bytes are = 0x00.
            writePacket.command = PROGRAM_DEVICE;
            writePacket.address = address;

            //Check if we are near the end of the programmable region, and need to send a "short packet" (with less than the maximum
            //allowed program data payload bytes).  In this case we need to notify the device by using the PROGRAM_COMPLETE command instead
            //of the normal PROGRAM_DEVICE command.  This lets the bootloader firmware in the device know it should flush any internal
            //buffers it may be using, by programming all of the bufferred data to NVM memory.
            if(((endAddress - address) * bytesPerAddress) < bytesPerPacket)
            {
                writePacket.bytesPerPacket = (endAddress - address) * bytesPerAddress;
                //Copy the packet data to the actual outgoing buffer and then send it over USB to the device.
                memcpy((unsigned char*)&writePacket.data[0] + 58 - writePacket.bytesPerPacket, pData, writePacket.bytesPerPacket);
                //Check to make sure we are completely programming all bytes of the destination address.  If not,
                //increase the data size and set the extra byte(s) to 0xFF (the default/blank value).
                while((writePacket.bytesPerPacket % bytesPerWord) != 0)
                {
                    if(writePacket.bytesPerPacket >= bytesPerPacket)
                    {
                        break; //should never hit this break, due to while((bytesPerPacket % bytesPerWord) != 0) check at start of function
                    }

                    //Shift all the data payload bytes in the packet to the left one (lower address),
                    //so we can insert a new 0xFF byte.
                    for(i = 0; i < (unsigned char)(bytesPerPacket - 1); i++)
                    {
                        writePacket.data[i] = writePacket.data[i+1];
                    }
                    writePacket.data[writePacket.bytesPerPacket] = 0xFF;
                    writePacket.bytesPerPacket++;

                }
                bytesToSend = writePacket.bytesPerPacket;
                qDebug("Preparing short packet of final program data with payload: 0x%x", (unsigned int)writePacket.bytesPerPacket);
            }
            else
            {
                //Else we are planning on sending a full length packet with the full size payload.
                writePacket.bytesPerPacket = bytesPerPacket;
                bytesToSend = bytesPerPacket;
                //Copy the packet data to the actual outgoing buffer and then prepare to send it.
                memcpy((unsigned char*)&writePacket.data[0] + 58 - writePacket.bytesPerPacket, pData, writePacket.bytesPerPacket);
            }


            //Check if all bytes of the data payload section of the packet are == 0xFF.  If so, we can save programming
            //time by skipping the packet by not sending it to the device.  The default/erased value is already = 0xFF, so
            //the contents of the flash memory will be correct (although if we want to be certain we should make sure
            //the 0xFF regions are still getting checked during the verify step, in case the erase procedure failed to set
            //all bytes = 0xFF).
            allPayloadBytesFF = true;   //assume true until we do the actual check below
            //Loop for all of the bytes in the data payload portion of the writePacket.  The data payload is little endian but is stored
            //"right justified" in the packet.  Therefore, writePacket.data[0] isn't necessarily the LSB data byte in the packet.
            startOfDataPayloadIndex = 58 - writePacket.bytesPerPacket;
            for(i = startOfDataPayloadIndex; i < (startOfDataPayloadIndex + writePacket.bytesPerPacket); i++)
            {
                if(writePacket.data[i] != 0xFF)
                {
                    //Special check for PIC24, where every 4th byte from the .hex file is == 0x00,
                    //which is the "phantom byte" (the upper byte of each odd address 16-bit word
                    //is unimplemented, and is probably 0x00 in the .hex file).
                    if((((i - startOfDataPayloadIndex) % bytesPerWord) == 3) && (deviceFamily == Device::PIC24))
                    {
                        //We can ignore this "phantom byte", since it is unimplemented and effectively a "don't care" byte.
                    }
                    else
                    {
                        //We found a non 0xFF (or blank value) byte.  We need to send and program the
                        //packet of useful data into the device.
                        allPayloadBytesFF = false;
                        break;
                    }
                }
            }

            //Check if we need to send a normal packet of data to the device, if the packet was all 0xFF and
            //we need to send a PROGRAM_COMPLETE packet, or if it was all 0xFF and we can simply skip it without
            //doing anything.
            if(allPayloadBytesFF == false)
            {
                qDebug("Sending program data packet with address: 0x%x", (unsigned int)writePacket.address);

                //We need to send a normal PROGRAM_DEVICE packet worth of data to program.
                if(connected)result = SendPacket((unsigned char*)&writePacket, sizeof(writePacket));
                else if(connectedApp)result = SendPacketApp((unsigned char*)&writePacket, sizeof(writePacket));
                //Verify the data was successfully received by the USB device.
                if(result != Success)
                {
                    qWarning("Error during program sending packet with address: 0x%x", (unsigned int)writePacket.address);
                    return result;
                }
                firstAllFFPacketFound = true; //reset flag so it will be true the next time a pure 0xFF packet is found
                lastCommandSent = PROGRAM_DEVICE;

            }
            else if((allPayloadBytesFF == true) && (firstAllFFPacketFound == true))
            {
                //In this case we need to send a PROGRAM_COMPLETE command to let the firmware know it should flush
                //its buffer by programming all of it to flash, since we are about to skip to a new address range.
                writePacket.command = PROGRAM_COMPLETE;
                writePacket.bytesPerPacket = 0;
                firstAllFFPacketFound = false;
                qDebug("Sending program complete data packet to skip a packet with address: 0x%x", (unsigned int)writePacket.address);
                if(connected)result = SendPacket((unsigned char*)&writePacket, sizeof(writePacket));
                else if(connectedApp)result = SendPacketApp((unsigned char*)&writePacket, sizeof(writePacket));
                //Verify the data was successfully received by the USB device.
                if(result != Success)
                {
                    qWarning("Error during program sending packet with address: 0x%x", (unsigned int)writePacket.address);
                    return result;
                }
                lastCommandSent = PROGRAM_COMPLETE;
            }
            else
            {
                //If we get to here, this means that (allPayloadBytesFF == true) && (firstAllFFPacketFound == false).
                //In this case, the last packet that we processed was all 0xFF, and all bytes of this packet are
                //also all 0xFF.  In this case, we don't need to send any packet data to the device.  All we need
                //to do is advance our pointers and keep checking for a new non-0xFF section.
                qDebug("Skipping data packet with all 0xFF with address: 0x%x", (unsigned int)writePacket.address);
            }

            //Increment pointers now that we successfully programmed (or deliberately skipped) a packet worth of data
            address += bytesPerPacket / bytesPerAddress;
            pData += bytesToSend;

            //Check if we just now exactly finished programming the memory region (in which case address will be exactly == endAddress)
            //region. (ex: we sent a PROGRAM_DEVICE instead of PROGRAM_COMPLETE for the last packet sent).
            //In this case, we still need to send the PROGRAM_COMPLETE command to let the firmware know that it is done,
            //and will not be receiving any subsequent program packets for this memory region.
            if((unsigned int)address >= (unsigned int)endAddress)
            {
                //Check if we still need to send a PROGRAM_COMPLETE command (we don't need to send one if
                //the last command we sent was a PRORAM_COMPLETE already).
                if(lastCommandSent == PROGRAM_COMPLETE)
                {
                    break;
                }

                memset((void*)&writePacket, 0x00, sizeof(writePacket));
                writePacket.command = PROGRAM_COMPLETE;
                writePacket.bytesPerPacket = 0;
                qDebug("Sending final program complete command for this region.");

                if(connected)result = SendPacket((unsigned char*)&writePacket, sizeof(writePacket));
                else if(connectedApp)result = SendPacketApp((unsigned char*)&writePacket, sizeof(writePacket));
                break;
            }
        }//while(address < endAddress)

        return result;

    }//if(connected)
    else
    {
        return NotConnected;
    }
}

/**
 *
 */
Comm::ErrorCode Comm::GetData(unsigned long address, unsigned char bytesPerPacket,
                              unsigned char bytesPerAddress, unsigned char bytesPerWord,
                              unsigned long endAddress, unsigned char *pData)
{
    ReadPacket readPacket;
    WritePacket writePacket;
    ErrorCode result = Comm::Fail;
    unsigned int percentCompletion,i;
    unsigned int addressesToFetch = endAddress - address;

    //Check to avoid possible division by zero when computing the percentage completion status.
    if(addressesToFetch == 0)
        addressesToFetch++;


    if(connected || connectedApp) {
        //First error check the input parameters before using them
        if((pData == NULL) || (endAddress < address) || (bytesPerPacket == 0))
        {
            qWarning("Error, bad parameters provided to call of GetData()");
            return Fail;
        }

        // Continue reading from device until the entire programmable region has been read
        while(address < endAddress)
        {
            //Update the progress bar so the user knows things are happening.
            percentCompletion = 100*((float)1 - (float)((float)(endAddress - address)/(float)addressesToFetch));
            if(percentCompletion > 100)
            {
                percentCompletion = 100;
            }
            //Reformat the percent completion so it "fits" in the 33% to 66% region (since erase
            //"completes" 0%-32% of the total erase/program/verify cycle, and verify completes 67%-100%).
            percentCompletion /= 3;
            percentCompletion += 67;
            emit SetProgressBar(percentCompletion);


            // Set up the buffer packet with the appropriate address and with the get data command
            memset((void*)&writePacket, 0x00, sizeof(writePacket));
            writePacket.command = GET_DATA;
            writePacket.address = address;

            //Debug output info.
            qWarning("Fetching packet with address: 0x%x", (unsigned int)writePacket.address);

            // Calculate to see if the entire buffer can be filled with data, or just partially
            if(((endAddress - address) * bytesPerAddress) < bytesPerPacket)
                // If the amount of bytes left over between current address and end address is less than
                //  the max amount of bytes per packet, then make sure the bytesPerPacket info is updated
                writePacket.bytesPerPacket = (endAddress - address) * bytesPerAddress;
            else
                // Otherwise keep it at its maximum
                writePacket.bytesPerPacket = bytesPerPacket;


            // Send the packet
            if(connected)result = SendPacket((unsigned char*)&writePacket, sizeof(writePacket));
            else if(connectedApp)result = SendPacketApp((unsigned char*)&writePacket, sizeof(writePacket));

            // If it wasn't successful, then return with error
            if(result != Success)
            {
                qWarning("Error during verify sending packet with address: 0x%x", (unsigned int)writePacket.address);
                return result;
            }

            // Otherwise, read back the packet from the device
            memset((void*)&readPacket, 0x00, sizeof(readPacket));
            if(connected)result = ReceivePacket((unsigned char*)&readPacket, sizeof(readPacket));
            else if(connectedApp)result = ReceivePacketApp((unsigned char*)&readPacket, sizeof(readPacket));

            // If it wasn't successful, then return with error
            if(result != Success)
            {
                qWarning("Error reading packet with address: 0x%x", (unsigned int)readPacket.address);
                return result;
            }

            // Copy contents from packet to data pointer
            //memcpy(pData, readPacket.data + 58 - readPacket.bytesPerPacket, readPacket.bytesPerPacket);
            for(i=0; i<readPacket.bytesPerPacket; i++)
                pData[i] = readPacket.data[i + 58 - readPacket.bytesPerPacket];

            // Increment data pointer
            pData += readPacket.bytesPerPacket;

            // Increment address by however many bytes were received divided by how many bytes per address
            address += readPacket.bytesPerPacket / bytesPerAddress;
        }

        // if successfully received entire region, return success
        return Success;
    }

    // If not connected, return not connected
    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::GetEeprom(unsigned char page, unsigned char *pData)
{
    ReadPacket readPacket;
    WritePacket writePacket;
    ErrorCode result;
    unsigned int i;

    if(connectedApp) {
        //First error check the input parameters before using them
        if(pData == NULL)
        {
            qWarning("Error, bad parameters provided to call of GetEeprom()");
            return Fail;
        }

        // Set up the buffer packet with the appropriate address and with the get data command
        memset((void*)&writePacket, 0x00, sizeof(writePacket));
        writePacket.command = GET_EEPROM;
        writePacket.address = page;
        writePacket.bytesPerPacket = 0x20;  //1 page = 32 octets

        //Debug output info.
        qWarning("Fetching page : 0x%x", (unsigned int)writePacket.address);

        // Send the packet
        result = SendPacketApp((unsigned char*)&writePacket, sizeof(writePacket));

        // If it wasn't successful, then return with error
        if(result != Success)
        {
            qWarning("Error during verify sending packet with address: 0x%x", (unsigned int)writePacket.address);
            return result;
        }

        // Otherwise, read back the packet from the device
        memset((void*)&readPacket, 0x00, sizeof(readPacket));
        result = ReceivePacketApp((unsigned char*)&readPacket, sizeof(readPacket));

        // If it wasn't successful, then return with error
        if(result != Success)
        {
            qWarning("Error reading packet with address: 0x%x", (unsigned int)readPacket.address);
            return result;
        }

        // Copy contents from packet to data pointer
        //memcpy(pData, readPacket.data + 58 - readPacket.bytesPerPacket, readPacket.bytesPerPacket);
        for(i=0; i<readPacket.bytesPerPacket; i++)
            pData[i] = readPacket.data[i];

        // Increment data pointer
        pData += readPacket.bytesPerPacket;


        // if successfully received entire region, return success
        return Success;
    }

    // If not connected, return not connected
    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::SetDate(unsigned char *pData)
{
    ReadPacket readPacket;
    WritePacket writePacket;
    ErrorCode result;

    if(connectedApp) {
        //First error check the input parameters before using them
        if(pData == NULL)
        {
            qWarning("Error, bad parameters provided to call of GetEeprom()");
            return Fail;
        }

        // Set up the buffer packet with the appropriate address and with the get data command
        memset((void*)&writePacket, 0x00, sizeof(writePacket));
        writePacket.command = SET_DATE;
        writePacket.bytesPerPacket = 6;
        memcpy((void*)&writePacket.data, pData, 6);

        //Debug output info.
        qWarning("Setting date...");

        // Send the packet
        result = SendPacketApp((unsigned char*)&writePacket, sizeof(writePacket));

        // If it wasn't successful, then return with error
        if(result != Success)
        {
            qWarning("Error during setting date");
            return result;
        }

        // Otherwise, read back the packet from the device
        memset((void*)&readPacket, 0x00, sizeof(readPacket));
        result = ReceivePacketApp((unsigned char*)&readPacket, sizeof(readPacket));

        // If it wasn't successful, then return with error
        if(result != Success)
        {
            qWarning("Error during setting date");
            return result;
        }

        // if successfully received entire region, return success
        return Success;
    }

    // If not connected, return not connected
    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::GetDate(unsigned char *pData)
{
    ReadPacket readPacket;
    WritePacket writePacket;
    ErrorCode result;
    unsigned int i;

    if(connectedApp) {
        //First error check the input parameters before using them
        if(pData == NULL)
        {
            qWarning("Error, bad parameters provided to call of GetEeprom()");
            return Fail;
        }

        // Set up the buffer packet with the appropriate address and with the get data command
        memset((void*)&writePacket, 0x00, sizeof(writePacket));
        writePacket.command = GET_DATE;

        //Debug output info.
        qWarning("Getting date...");

        // Send the packet
        result = SendPacketApp((unsigned char*)&writePacket, sizeof(writePacket));

        // If it wasn't successful, then return with error
        if(result != Success)
        {
            qWarning("Error during getting date");
            return result;
        }

        // Otherwise, read back the packet from the device
        memset((void*)&readPacket, 0x00, sizeof(readPacket));
        result = ReceivePacketApp((unsigned char*)&readPacket, sizeof(readPacket));

        // If it wasn't successful, then return with error
        if(result != Success)
        {
            qWarning("Error during getting date");
            return result;
        }

        // Copy contents from packet to data pointer
        for(i=0; i<readPacket.bytesPerPacket; i++)
        {
            pData[i] = readPacket.data[i];
        }

        // if successfully received entire region, return success
        return Success;
    }

    // If not connected, return not connected
    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::GetBattLevel(unsigned char *pData)
{
    ReadPacket readPacket;
    WritePacket writePacket;
    ErrorCode result;
    unsigned int i;

    if(connected) {
        //First error check the input parameters before using them
        if(pData == NULL)
        {
            qWarning("Error, bad parameters provided to call of GetEeprom()");
            return Fail;
        }

        // Set up the buffer packet with the appropriate address and with the get data command
        memset((void*)&writePacket, 0x00, sizeof(writePacket));
        writePacket.command = GET_BATT_LEVEL;
        writePacket.bytesPerPacket = 2;

        //Debug output info.
        qWarning("Getting battery level...");

        // Send the packet
        result = SendPacket((unsigned char*)&writePacket, sizeof(writePacket));

        // If it wasn't successful, then return with error
        if(result != Success)
        {
            qWarning("Error during getting date");
            return result;
        }

        // Otherwise, read back the packet from the device
        memset((void*)&readPacket, 0x00, sizeof(readPacket));
        result = ReceivePacket((unsigned char*)&readPacket, sizeof(readPacket));

        // If it wasn't successful, then return with error
        if(result != Success)
        {
            qWarning("Error during getting date");
            return result;
        }

        // Copy contents from packet to data pointer
        for(i=0; i<readPacket.bytesPerPacket; i++)
        {
            pData[i] = readPacket.data[i];
        }

        // if successfully received entire region, return success
        return Success;
    }

    // If not connected, return not connected
    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::Erase(void) {
    WritePacket sendPacket;
    QTime elapsed;
    ErrorCode status = Comm::Fail;

    if(connected || connectedApp) {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = ERASE_DEVICE;

        qDebug("Sending Erase Command...");

        elapsed.start();

        if(connected)status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        if(status == Comm::Success)
            qDebug("Successfully sent erase command (%fs)", (double)elapsed.elapsed() / 1000);
        else
            qDebug("Erasing Device Failed");

        return status;
    }

    qDebug("Device not connected");
    return NotConnected;
}

// Pour la partie part number, numero de client,...
Comm::ErrorCode Comm::EraseData(void) {
    WritePacket sendPacket;
    QTime elapsed;
    ErrorCode status = Comm::Fail;

    if(connected || connectedApp) {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = ERASE_DATA;

        qDebug("Sending Erase Command...");

        elapsed.start();

        if(connected)status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        if(status == Comm::Success)
            qDebug("Successfully sent erase command (%fs)", (double)elapsed.elapsed() / 1000);
        else
            qDebug("Erasing Device Failed");

        return status;
    }

    qDebug("Device not connected");
    return NotConnected;
}

Comm::ErrorCode Comm::EraseEeprom(void) {
    WritePacket sendPacket;
    QTime elapsed;
    ErrorCode status=Comm::Other;

    if(connected || connectedApp) {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = ERASE_EEPROM;

        qDebug("Sending Erase EEPROM Command...");

        elapsed.start();

        if(connected)status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        if(status == Comm::Success)
            qDebug("Successfully sent erase eeprom command (%fs)", (double)elapsed.elapsed() / 1000);
        else
            qDebug("Erasing Device EEPROM Failed");

        return status;
    }

    qDebug("Device not connected");
    return NotConnected;
}

//Sends command to USB device to lock or unlock the config bit region.  If the config bits overlapped an
//erase page with standard program memory, this will also affect the status of the erase page.  (for example
//locking the config bits on a PIC18FxxJ or PIC24FJ device, which stores config bits at the end of the last page
//of flash memory, will also prevent reprogramming of the last page of flash memory).
//The bool lock value input parameter is either true (lock/protect the config bits), or false (unlock/allow
//reprogramming of config bits and any overlapping flash erase page, if relevant).
Comm::ErrorCode Comm::LockUnlockConfig(bool lock)
{
    WritePacket sendPacket;
    QTime elapsed;
    ErrorCode status = Comm::Fail;

    if(connected || connectedApp) {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = UNLOCK_CONFIG;
        if(lock == false)
        {
            sendPacket.LockedValue = 0x00;  //0x00 is used in the bootloader firmware to mean unlock the config bits ("UNLOCKCONFIG")
        }
        else
        {
            //All other values should mean we are locking the config bits
            sendPacket.LockedValue = 0x01;  //lock the config bits
        }

        if(lock)
            qDebug("Locking Config Bits...");
        else
            qDebug("Unlocking Config Bits...");

        elapsed.start();

        if(connected)status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        if(status == Comm::Success)
            qDebug("Successfully sent un/lock command (%fs)", (double)elapsed.elapsed() / 1000);
        else
            qDebug("Unsuccessfully sent un/lock command");

        return status;
    }

    qDebug("Device not connected");
    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::ReadBootloaderInfo(BootInfo* bootInfo)
{
    QTime elapsed;
    WritePacket sendPacket;
    ErrorCode status = Comm::Fail;

    qDebug("Getting Query packet...");

    if(connected || connectedApp)
    {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = QUERY_DEVICE;

        elapsed.start();
        qDebug("Send packet..");
        if(connected)status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));
        switch(status)
        {
            case Fail:
            qDebug("Fail...");
                close();
            case Timeout:
            qDebug("Timeout...");
                return status;
            default:
                break;
        }

        qDebug("Successfully sent querying command (%fs)", (double)elapsed.elapsed() / 1000);
        memset((void*)bootInfo, 0x00, sizeof(BootInfo));

        elapsed.start();

        if(connected)status = ReceivePacket((unsigned char*)bootInfo, sizeof(BootInfo));
        else if (connectedApp)status = ReceivePacketApp((unsigned char*)bootInfo, sizeof(BootInfo));

        if(bootInfo->command != 0x02) {
            qWarning("Received incorrect command.");
            return IncorrectCommand;
        }

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        qDebug("Successfully received query packet (%fs)", (double)elapsed.elapsed() / 1000);
        return Success;
    }

    return NotConnected;
}

void Comm::Get_Msg_Debug(int length, unsigned char* buffer){
    unsigned char pData[64];
    ReadPacket receiveMsg;
    WritePacket sendPacket;
    ErrorCode status;
    int i;

    qDebug("Getting Debug Msg...");

    if(connectedApp)
    {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = GET_MSG;
        if(length !=0)
        {
            sendPacket.bytesPerPacket = length;
            for (i=0; i<length; i++) sendPacket.data[i] = buffer[i];
        }

        status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return ;
            default:
                break;
        }

        memset((void*)&receiveMsg, 0x00, sizeof(receiveMsg));

        status = ReceivePacketApp((unsigned char*)&receiveMsg, sizeof(receiveMsg));

        if(receiveMsg.command != GET_MSG) {
            qWarning("Received incorrect command:%i", receiveMsg.command);
            return ;
        }

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return ;
            default:
                break;
        }

        // Copy contents from packet to data pointer
        if(receiveMsg.bytesPerPacket>0)
        {
            for(i=0;i<receiveMsg.bytesPerPacket;i++){
                pData[i] = receiveMsg.data[i];
            }
             emit SetReadedDebug(pData);
        }
        else
        {
            qDebug("NO DATA in received debug msg");
            return ;
        }
    }
    return ;
}


/**
 *
 */
Comm::ErrorCode Comm::Get_Msg(unsigned char *pData)
{
    QTime elapsed;
    ReadPacket receiveMsg;
    WritePacket sendPacket;
    ErrorCode status;
    unsigned int i;

    qDebug("Getting Debug Msg...");

    if(connectedApp)
    {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = GET_MSG;

        elapsed.start();

        status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        qDebug("Successfully sent GET_MSG command (%fs)", (double)elapsed.elapsed() / 1000);
        memset((void*)&receiveMsg, 0x00, sizeof(receiveMsg));

        elapsed.start();

        status = ReceivePacket((unsigned char*)&receiveMsg, sizeof(receiveMsg));

        if(receiveMsg.command != GET_MSG) {
            qWarning("Received incorrect command:%i", receiveMsg.command);
            return IncorrectCommand;
        }

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        // Copy contents from packet to data pointer
        if(receiveMsg.bytesPerPacket>0)
        {
            for(i=0;i<receiveMsg.bytesPerPacket;i++){
                pData[i] = receiveMsg.data[i];
            }
        }
        else
        {
            qDebug("NO DATA in received debug msg (%fs)", (double)elapsed.elapsed() / 1000);
            return NoData;
        }

        qDebug("Successfully received debug msg (%fs)", (double)elapsed.elapsed() / 1000);
        return Success;
    }

    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::Program_Param(unsigned char *pData)
{
    QTime elapsed;
    ReadPacket receiveMsg;
    WritePacket sendPacket;
    ErrorCode status = Comm::Fail;

    qDebug("Programming parameters...");

    if(connected || connectedApp)
    {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = PROGRAM_PARAM;
        sendPacket.data[17] = pData[0];
        sendPacket.data[18] = pData[1];
        sendPacket.data[19] = pData[2];
        sendPacket.data[20] = pData[3];
        sendPacket.data[21] = pData[4];
        sendPacket.data[22] = pData[5];
        sendPacket.data[23] = pData[6];
        sendPacket.data[24] = pData[7];
        sendPacket.data[25] = pData[8];
        sendPacket.data[26] = pData[9];
        sendPacket.data[27] = pData[10];
        sendPacket.data[28] = pData[11];
        sendPacket.data[29] = pData[12];
        sendPacket.data[30] = pData[13];
        sendPacket.data[31] = pData[14];
        sendPacket.data[32] = pData[15];

        elapsed.start();

        if(connected)status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        qDebug("Successfully sent PROGRAM_PARAM command (%fs)", (double)elapsed.elapsed() / 1000);
        memset((void*)&receiveMsg, 0x00, sizeof(receiveMsg));

        elapsed.start();

        if(connected)status = ReceivePacket((unsigned char*)&receiveMsg, sizeof(receiveMsg));
        else if(connectedApp)status = ReceivePacketApp((unsigned char*)&receiveMsg, sizeof(receiveMsg));

        if(receiveMsg.command != PROGRAM_PARAM) {
            qWarning("Received incorrect command:%i", receiveMsg.command);
            return IncorrectCommand;
        }

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        qDebug("Successfully received program acknowledgement msg (%fs)", (double)elapsed.elapsed() / 1000);
        return Success;
    }

    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::Program_IP(unsigned char *pData)
{
    QTime elapsed;
    ReadPacket receiveMsg;
    WritePacket sendPacket;
    ErrorCode status;

    qDebug("Programming IP Settings...");

    if(connectedApp)
    {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = WRITE_IP;
        sendPacket.data[1] = pData[0];
        sendPacket.data[2] = pData[1];
        sendPacket.data[3] = pData[2];
        sendPacket.data[4] = pData[3];
        sendPacket.data[5] = pData[4];
        sendPacket.data[6] = pData[5];
        sendPacket.data[7] = pData[6];
        sendPacket.data[8] = pData[7];
        sendPacket.data[9] = pData[8];
        sendPacket.data[10] = pData[9];
        sendPacket.data[11] = pData[10];
        sendPacket.data[12] = pData[11];
        sendPacket.data[13] = pData[12];
        sendPacket.data[14] = pData[13];
        sendPacket.data[15] = pData[14];
        sendPacket.data[16] = pData[15];

        elapsed.start();

        status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        qDebug("Successfully sent WRITE_IP command (%fs)", (double)elapsed.elapsed() / 1000);
        memset((void*)&receiveMsg, 0x00, sizeof(receiveMsg));

        elapsed.start();

        status = ReceivePacketApp((unsigned char*)&receiveMsg, sizeof(receiveMsg));

        if(receiveMsg.command != WRITE_IP) {
            qWarning("Received incorrect command:%i", receiveMsg.command);
            return IncorrectCommand;
        }

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        qDebug("Successfully received WRITE_IP acknowledgement msg (%fs)", (double)elapsed.elapsed() / 1000);
        return Success;
    }

    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::Read_Param(unsigned char *pData)
{
    QTime elapsed;
    ReadPacket receiveMsg;
    WritePacket sendPacket;
    ErrorCode status = Comm::Fail;
    unsigned int i;

    qDebug("Getting parameters...");

    if(connected || connectedApp)
    {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = READ_PARAM;

        elapsed.start();

        if(connected)status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        qDebug("Successfully sent READ_PARAM command (%fs)", (double)elapsed.elapsed() / 1000);
        memset((void*)&receiveMsg, 0x00, sizeof(receiveMsg));

        elapsed.start();

        if(connected)status = ReceivePacket((unsigned char*)&receiveMsg, sizeof(receiveMsg));
        else if(connectedApp)status = ReceivePacketApp((unsigned char*)&receiveMsg, sizeof(receiveMsg));

        if(receiveMsg.command != READ_PARAM) {
            qWarning("Received incorrect command:%i", receiveMsg.command);
            return IncorrectCommand;
        }

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        // Copy contents from packet to data pointer
        if(receiveMsg.bytesPerPacket>0)
        {
            for(i=0;i<receiveMsg.bytesPerPacket;i++)
                pData[i] = receiveMsg.data[i];
        }
        else
        {
            qDebug("NO DATA in received debug msg (%fs)", (double)elapsed.elapsed() / 1000);
            return NoData;
        }

        qDebug("Successfully received debug msg (%fs)", (double)elapsed.elapsed() / 1000);
        return Success;
    }

    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::Read_IP(unsigned char *pData)
{
    QTime elapsed;
    ReadPacket receiveMsg;
    WritePacket sendPacket;
    ErrorCode status;
    unsigned int i;

    qDebug("Getting IP settings...");

    if(connectedApp)
    {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = READ_IP;

        elapsed.start();

        status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        qDebug("Successfully sent READ_IP command (%fs)", (double)elapsed.elapsed() / 1000);
        memset((void*)&receiveMsg, 0x00, sizeof(receiveMsg));

        elapsed.start();

        status = ReceivePacketApp((unsigned char*)&receiveMsg, sizeof(receiveMsg));

        if(receiveMsg.command != READ_IP) {
            qWarning("Received incorrect command:%i", receiveMsg.command);
            return IncorrectCommand;
        }

        switch(status)
        {
            case Fail:
                close();
            case Timeout:
                return status;
            default:
                break;
        }

        // Copy contents from packet to data pointer
        if(receiveMsg.bytesPerPacket>0)
        {
            for(i=0;i<receiveMsg.bytesPerPacket;i++)
                pData[i] = receiveMsg.data[i];
        }
        else
        {
            qDebug("NO DATA in received debug msg (%fs)", (double)elapsed.elapsed() / 1000);
            return NoData;
        }

        qDebug("Successfully received debug msg (%fs)", (double)elapsed.elapsed() / 1000);
        return Success;
    }

    return NotConnected;
}

/**
 *
 */
Comm::ErrorCode Comm::Inversion_GD(void) {
    WritePacket sendPacket;
    QTime elapsed;
    ErrorCode status = Comm::Fail;

    if(connected || connectedApp) {
        memset((void*)&sendPacket, 0x00, sizeof(sendPacket));
        sendPacket.command = INVERS_GD;

        qDebug("Sending Inversion G/D Command...");

        elapsed.start();

        if(connected)status = SendPacket((unsigned char*)&sendPacket, sizeof(sendPacket));
        else if(connectedApp)status = SendPacketApp((unsigned char*)&sendPacket, sizeof(sendPacket));

        if(status == Comm::Success)
            qDebug("Successfully sent command (%fs)", (double)elapsed.elapsed() / 1000);
        else
            qDebug("Sending Command Failed");

        return status;
    }

    qDebug("Device not connected");
    return NotConnected;
}

Comm::ErrorCode Comm::SendPacket(unsigned char *pData, int size)
{
    QTime timeoutTimer;
    int res = 0, timeout = 5;

    timeoutTimer.start();
    while(res < 1)
    {
        if(connected==true && boot_device!=NULL){res = hid_write(boot_device, pData, size);}
        else if(connectedApp==true && boot_deviceApp!=NULL){res = hid_write(boot_deviceApp, pData, size);}
        else {
            return Fail;
        }
        if(timeoutTimer.elapsed() > SyncWaitTime)
        {
            timeoutTimer.start();
            timeout--;
        }

        // If timed out several times, or return error then close device and return failure
        if(timeout == 0)
        {
            qWarning("Timed out waiting for query command acknowledgement.");
            return Timeout;
        }

        if(res == -1)
        {
            qWarning("Write failed.");
            close();
            return Fail;
        }
    }
    return Success;
}


Comm::ErrorCode Comm::ReceivePacket(unsigned char *data, int size)
{
    QTime timeoutTimer;
    int res = 0, timeout = 3;

    timeoutTimer.start();

    while(res < 1)
    {
        if(connected==true)res = hid_read(boot_device, data, size);
        else if(connectedApp==true)res = hid_read(boot_deviceApp, data, size);

        if(timeoutTimer.elapsed() > SyncWaitTime)
        {
            timeoutTimer.start();
            timeout--;
        }

        // If timed out twice, or return error then close device and return failure
        if(timeout == 0)
        {
            qWarning("Timeout.");
            return Timeout;
        }

        if(res == -1)
        {
            qWarning("Read failed.");
            close();
            return Fail;
        }
    }
    return Success;
}

Comm::ErrorCode Comm::SendPacketApp(unsigned char *pData, int size)
{
    QTime timeoutTimer;
    int res = 0, timeout = 5;

    timeoutTimer.start();

    if(boot_deviceApp == NULL){
        qWarning("Boot device is empty, try to open again the connection.");
        openApp();
        //return Fail;
    }
    if(boot_deviceApp == NULL)return Fail;
    while(res < 1)
    {
        res = hid_write(boot_deviceApp, pData, size);

        if(timeoutTimer.elapsed() > SyncWaitTime)
        {
            timeoutTimer.start();
            timeout--;
        }

        // If timed out several times, or return error then close device and return failure
        if(timeout == 0)
        {
            qWarning("Timed out waiting for query command acknowledgement.");
            return Timeout;
        }

        if(res == -1)
        {
            qWarning("Write failed.");
            close();
            return Fail;
        }
    }
    return Success;
}


Comm::ErrorCode Comm::ReceivePacketApp(unsigned char *data, int size)
{
    QTime timeoutTimer;
    int res = 0, timeout = 3;

    timeoutTimer.start();

    if(boot_deviceApp == NULL){
        qWarning("Boot device is empty, try to open again the connection.");
        openApp();
        //return Fail;
    }
    if(boot_deviceApp == NULL)return Fail;

    while(res < 1)
    {
        res = hid_read(boot_deviceApp, data, size);

        if(timeoutTimer.elapsed() > SyncWaitTime)
        {
            timeoutTimer.start();
            timeout--;
        }

        // If timed out twice, or return error then close device and return failure
        if(timeout == 0)
        {
            qWarning("Timeout.");
            return Timeout;
        }

        if(res == -1)
        {
            qWarning("Read failed.");
            close();
            return Fail;
        }
    }
    return Success;
}
