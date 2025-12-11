TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = \
    HIDAPI \
    Bootloader \
    Encrypter \
    VolfoniLoader \
##    AHU_Monitor \
##    Light_VolfoniLoader \
    Manager_for_Firmware_Versioning \
##    MemoryReader \
##    Optical_Manager \
##    SCC_Manager
    Optical \
#	ProdAssist
    Optical_Client \
    Register
