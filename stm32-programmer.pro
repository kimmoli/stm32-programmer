#
# Project stm32-programmer, STM32 Programmer
#

TARGET = stm32-programmer

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
QT -= gui

target.path = /usr/bin/

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

message($${DEFINES})

INSTALLS += target

SOURCES += src/stm32-programmer.cpp \
	src/stm32p.cpp \
    src/driverBase.cpp \
    src/stm32driver.cpp
	
HEADERS += src/stm32p.h \
    src/driverBase.h \
    src/stm32driver.h

OTHER_FILES += \
    rpm/stm32-programmer.spec

