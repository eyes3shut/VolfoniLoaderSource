#-------------------------------------------------
#
# Project created by QtCreator 2014-03-24T16:23:18
#
#-------------------------------------------------

QT       += core gui

TARGET = Optical_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../Bootloader/product.cpp \
    ../Bootloader/Comm.cpp \
    ../Bootloader/hexfile.cpp \
    ../Bootloader/DeviceData.cpp \
    ../Bootloader/Device.cpp \
    ../Bootloader/ImportExportHex.cpp \
    ../Encrypter/encrypt.cpp \
    ../Encrypter/decrypt.cpp \

HEADERS  += mainwindow.h \
    ../Bootloader/product.h \
    ../Bootloader/Comm.h \
    ../Bootloader/hexfile.h \
    ../Bootloader/DeviceData.h \
    ../Bootloader/Device.h \
    ../Bootloader/ImportExportHex.h \
    ../Encrypter/encrypt.h \
    ../Encrypter/decrypt.h \

QT += network

#-------------------------------------------------
# Add the correct HIDAPI library according to what
# OS is being used
#-------------------------------------------------
win32: LIBS += -L../HIDAPI/windows
macx: LIBS += -L../HIDAPI/mac
unix: !macx: LIBS += -L../HIDAPI/linux
LIBS += -lHIDAPI

#-------------------------------------------------
# Make sure to add the required libraries or
# frameoworks for the hidapi to work depending on
# what OS is being used
#-------------------------------------------------
macx: LIBS += -framework CoreFoundation -framework IOkit
win32: LIBS += -lSetupAPI
unix: !macx: LIBS += -lusb-1.0

RESOURCES += \
    ressources.qrc
