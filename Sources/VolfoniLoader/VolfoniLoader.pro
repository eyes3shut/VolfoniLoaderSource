#-------------------------------------------------
#
# Project created by QtCreator 2011-10-13T16:08:25
#
#-------------------------------------------------

TARGET = VolfoniLoader
TEMPLATE = app
QT += core gui widgets network sql xml

TRANSLATIONS = trad_French.ts \
                 trad_English.ts \
                 trad_German.ts \
                 trad_Spanish.ts \

QMAKE_CXXFLAGS_RELEASE = -Os
SOURCES += main.cpp\
        mainwindow.cpp \
        login.cpp \
        listUpdate.cpp \
        tools.cpp\
    ../Bootloader/Comm.cpp \
    ../Bootloader/DeviceData.cpp \
    ../Bootloader/Device.cpp \
    ../Bootloader/ImportExportHex.cpp \
    ../Encrypter/encrypt.cpp \
    ../Encrypter/decrypt.cpp \
    ../Bootloader/product.cpp \
    ../Bootloader/hexfile.cpp \
    parameters.cpp \
    batteryview.cpp \
    parameters_AME.cpp \
    parameters_SCP.cpp \
    proglcd.cpp \
    highlighter.cpp

HEADERS  += mainwindow.h \
            login.h \
            listUpdate.h \
            tools.h\
    ../Bootloader/Comm.h \
    ../Bootloader/DeviceData.h \
    ../Bootloader/Device.h \
    ../Bootloader/ImportExportHex.h\
    ../Encrypter/encrypt.h \
    ../Encrypter/decrypt.h \
    ../Bootloader/product.h \
    ../Bootloader/hexfile.h \
    parameters.h \
    batteryview.h \
    parameters_AME.h \
    parameters_SCP.h \
    proglcd.h \
    highlighter.h

#-------------------------------------------------
# Add the correct HIDAPI library according to what
# OS is being used
#-------------------------------------------------
win32: LIBS += -L../HIDAPI/windows
win64: LIBS += -L../HIDAPI/windows
macx: LIBS += -L../../../HIDAPI/mac
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
    ICON = VolfoniLoader.icns

    #Fichiers a mettre dans le bundle, fichiers modifiables par utilisateur
    #fichiers fixes dans ressources et invisible pour l'utilisateur (fichiers de lang, images, etc)
    APP_FILES1.files = Config/config.xml
    APP_FILES1.path = Contents/Resources
    QMAKE_BUNDLE_DATA += APP_FILES1

    APP_FILES2.files = Config/language.xml
    APP_FILES2.path = Contents/Resources
    QMAKE_BUNDLE_DATA += APP_FILES2

    APP_FILES3.files = Docs/help.pdf
    APP_FILES3.path = Contents/Resources
    QMAKE_BUNDLE_DATA += APP_FILES3
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
