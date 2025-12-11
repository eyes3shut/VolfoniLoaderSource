#-------------------------------------------------
#
# Project created by QtCreator 2011-10-13T16:08:25
#
#-------------------------------------------------

TARGET = Firmware_Manager
TEMPLATE = app
QT += xml
QT += network
QT += sql

QMAKE_CXXFLAGS_RELEASE = -Os
SOURCES += main.cpp\
        mainwindow.cpp \
        editTab.cpp \
        generalTab.cpp \
        viewTab.cpp\
        tools.cpp\
        login.cpp\
        progress.cpp\
        SendServer.cpp\
    ../Encrypter/encrypt.cpp \
    ../Encrypter/decrypt.cpp


HEADERS  += mainwindow.h \
        editTab.h \
        generalTab.h \
        viewTab.h\
        tools.h\
        login.h\
        progress.h\
        SendServer.h\
    ../Encrypter/encrypt.h \
    ../Encrypter/decrypt.h


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

#-------------------------------------------------
# Make sure output directory for object file and
# executable is in the correct subdirectory
#-------------------------------------------------
macx {
    DESTDIR = mac
    OBJECTS_DIR = mac
    MOC_DIR = mac
    UI_DIR = mac
    RCC_DIR = mac
    ICON = manager.icns
}
unix: !macx {
    DESTDIR = linux
    OBJECTS_DIR = linux
    MOC_DIR = linux
    UI_DIR = linux
    RCC_DIR = linux
}
win32 {
    DESTDIR = windows
    OBJECTS_DIR = windows
    MOC_DIR = windows
    UI_DIR = windows
    RCC_DIR = windows
    RC_FILE += files.rc
}

RESOURCES += \
    ressources.qrc

