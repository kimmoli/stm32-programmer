#
# Project stm32-programmer, STM32 Programmer
#

TARGET = stm32-programmer

CONFIG += sailfishapp

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

message($${DEFINES})

SOURCES += src/stm32-programmer.cpp \
	src/stm32p.cpp \
    src/driverBase.cpp
	
HEADERS += src/stm32p.h \
    src/driverBase.h

OTHER_FILES += qml/stm32-programmer.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Programmer.qml \
    qml/pages/AboutPage.qml \
    rpm/stm32-programmer.spec \
	stm32-programmer.png \
    stm32-programmer.desktop \
    qml/components/Messagebox.qml

