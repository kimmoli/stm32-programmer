/*
Copyright (c) 2014 kimmoli kimmo.lindholm@gmail.com @likimmo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stm32p.h"
#include "stm32driver.h"
#include <QSettings>
#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QFile>

#define GPIO_INT "67"
#define GPIO_OUT true
#define GPIO_IN false

Stm32p::Stm32p(QObject *parent) :
    QObject(parent)
{
    STM32 = new stm32Driver(STM32F401_ADDRESS);

    gpioExport();
}

Stm32p::~Stm32p()
{
    qDebug() << "bye...";

    delete STM32;

    gpioRelease();
    vddStateSet(false);
}

bool Stm32p::filenameSet(QString name)
{
    qDebug() << "setting file name to" << name;

    hexFile.setFileName(name);

    return hexFile.exists();
}

/*
 * Function to control TOH Vdd state
 */
void Stm32p::vddStateSet(bool state)
{
    qDebug() << "changing vdd state to" << state;

    int fd = open("/sys/devices/platform/reg-userspace-consumer.0/state", O_WRONLY);

    if (!(fd < 0))
    {
        if (write (fd, state ? "1" : "0", 1) != 1)
            qCritical() << "Failed to control VDD.";

        close(fd);
        return;
    }

    QThread::msleep(100);
}

/*
 * Function to control state of GPIO pin
 */

void Stm32p::gpioExport()
{
    qDebug() << "exporting gpio";

    int fd = open("/sys/class/gpio/export", O_WRONLY);

    if (!(fd < 0))
    {
        if (write (fd, GPIO_INT, strlen(GPIO_INT)) != strlen(GPIO_INT))
            qCritical() << "Failed to export GPIO";

        close(fd);
        return;
    }
}

void Stm32p::gpioRelease()
{
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);

    if (!(fd < 0))
    {
        if (write (fd, GPIO_INT, strlen(GPIO_INT)) != strlen(GPIO_INT))
            qCritical() << "Failed to unexport GPIO";

        close(fd);
        return;
    }

}

void Stm32p::gpioDirection(bool output)
{

    int fd = open("/sys/class/gpio/gpio" GPIO_INT "/direction", O_WRONLY);

    if (!(fd < 0))
    {
        if (write (fd, output ? "out" : "in", (output ? 3 : 2)) != (output ? 3 : 2))
            qCritical() << "Failed to change direction of GPIO to " << (output ? "output" : "input");

        close(fd);
        return;
    }
}

void Stm32p::gpioStateSet(bool state)
{
    qDebug() << "changing GPIO state to" << state;

    int fd = open("/sys/class/gpio/gpio" GPIO_INT "/value", O_WRONLY);

    if (!(fd < 0))
    {
        if (write (fd, state ? "1" : "0", 1) != 1)
            qCritical() << "Failed to set GPIO state";

        close(fd);
    }
}

/*
 * Frontend of STM programmer
 */
void Stm32p::startProgram()
{
    vddStateSet(false);
    gpioDirection(GPIO_OUT);

    /* GPIO low when reset released enters bootloader */
    gpioStateSet(false);
    vddStateSet(true);

    /* The voltagesupervisor keeps reset active for max 300ms */
    QThread::msleep(400);

    QByteArray bootloaderVersion = STM32->cmdGetBootloaderVersion();
    if (bootloaderVersion == QByteArray())
        qCritical() << "Get Version Failed";
    else
        printf("Bootloader version v%s.%s\n", qPrintable(bootloaderVersion.toHex().at(0)), qPrintable(bootloaderVersion.toHex().at(1)));


    QByteArray chipId = STM32->cmdGetId();
    if (chipId == QByteArray())
        qCritical() << "Get ID Failed";
    else
        printf("Device id is 0x%s\n", qPrintable(chipId.toHex()));

    printf("erasing sector 0 %s\n", qPrintable(STM32->cmdEraseMemory(0)));

    printf("read 10 bytes from 0x08000000 %s\n", qPrintable(STM32->cmdReadMemory(0x08000000, 10).toHex()));

    printf("write some data to flash; %s\n", qPrintable(STM32->cmdWriteMemory(0x08000000, QByteArray::fromHex("deadbeef"))));

    printf("read 10 bytes from 0x08000000 %s\n", qPrintable(STM32->cmdReadMemory(0x08000000, 10).toHex()));

}

