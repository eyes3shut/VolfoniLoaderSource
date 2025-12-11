#-------------------------------------------------
#
# Project created by QtCreator 2013-04-15T17:39:58
#
#-------------------------------------------------

QT       += core gui
QT       += xml

TARGET = ProdAssit
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
    ../Bootloader/PartNumber.cpp

HEADERS  += mainwindow.h \
    ../Bootloader/product.h \
    ../Bootloader/Comm.h \
    ../Bootloader/hexfile.h \
    ../Bootloader/DeviceData.h \
    ../Bootloader/Device.h \
    ../Bootloader/ImportExportHex.h \
    ../Encrypter/encrypt.h \
    ../Encrypter/decrypt.h \
    ../Bootloader/PartNumber.h

FORMS    += mainwindow.ui \
    ../Bootloader/partnumberbox.ui

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
    ../Bootloader/resources.qrc
