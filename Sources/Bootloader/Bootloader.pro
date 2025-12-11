TARGET = "USB_Tool"
TEMPLATE = app
QT += sql
QT += xml
QMAKE_CXXFLAGS_RELEASE = -Os
INCLUDEPATH += ../
SOURCES += \
    Settings.cpp \
    MainWindow.cpp \
    main.cpp \
    DeviceData.cpp \
    Device.cpp \
    Comm.cpp \
    ImportExportHex.cpp \
    aboutbox.cpp \
    manager.cpp \
    versionwindow.cpp \
    PartNumber.cpp \
    CustomerNumber.cpp \
    tools.cpp \
    ../Encrypter/encrypt.cpp \
    ../Encrypter/decrypt.cpp \
    product.cpp \
    hexfile.cpp \
    sniffer.cpp
HEADERS += \
    Settings.h \
    MainWindow.h \
    DeviceData.h \
    Device.h \
    Comm.h \
    ImportExportHex.h \
    aboutbox.h \
    manager.h \
    versionwindow.h \
    PartNumber.h \
    CustomerNumber.h \
    tools.h \
    ../Encrypter/encrypt.h \
    ../Encrypter/decrypt.h \
    product.h \
    hexfile.h \
    sniffer.h

FORMS += MainWindow.ui \
    Settings.ui \
    aboutbox.ui \
    partnumberbox.ui \
    customernumberbox.ui \
    manager.ui \
    sniffer.ui
RESOURCES += resources.qrc

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
    ICON = VolfoTool.icns

    APP_FILES2.files = messages.xml
    APP_FILES2.path = Contents/Resources
    QMAKE_BUNDLE_DATA += APP_FILES2

    APP_FILES3.files = limites.xml
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
    RC_FILE += windows.rc
}
