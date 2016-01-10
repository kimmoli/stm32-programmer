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
#include <unistd.h>

#define GPIO_INT "67"
#define GPIO_OUT true
#define GPIO_IN false

Stm32p::Stm32p(QObject *parent) :
    QObject(parent)
{
    STM32 = new stm32Driver(STM32F401_ADDRESS);

    sectorStartAddress << 0x08000000 << 0x08004000 << 0x08008000 << 0x0800C000 << 0x08010000 << 0x08020000 << 0x08040000 << 0x08060000;
    sectorEndAddress   << 0x08003FFF << 0x08007FFF << 0x0800BFFF << 0x0800FFFF << 0x0801FFFF << 0x0803FFFF << 0x0805FFFF << 0x0807FFFF;
    sectorErased << false << false << false << false << false << false << false << false;

    startAfterProgramming = false;
    doneProgramming = false;

    gpioExport();
}

Stm32p::~Stm32p()
{
    delete STM32;

    gpioRelease();

    if (doneProgramming)
        vddStateSet(false);

    if (doneProgramming && startAfterProgramming)
    {
        printf("Starting...\n");
        QThread::msleep(300);
    }

    if (startAfterProgramming)
        vddStateSet(true);
}

bool Stm32p::filenameSet(QString name)
{
    hexFile.setFileName(name);

    return hexFile.exists();
}

/*
 * Function to control TOH Vdd state
 */
void Stm32p::vddStateSet(bool state)
{
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
    QThread::msleep(300);

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

    if (!hexFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Error opening file";
        return;
    }

    QByteArray data;
    unsigned long address = 0;
    QTextStream infile(&hexFile);

    while (parseHex(&infile, &address, &data))
    {
        for (int i=0; i<sectorErased.size(); ++i)
            if (address >= sectorStartAddress.at(i) && address <= sectorEndAddress.at(i) && !sectorErased.at(i))
            {
                printf("Erasing sector %d (0x%08lx...%08lx)\n", i, sectorStartAddress.at(i), sectorEndAddress.at(i));
                STM32->cmdEraseMemory(i);
                sectorErased.replace(i, true);
            }
        printf("Programming 0x%08lx\r", address);
        STM32->cmdWriteMemory(address, data);
    }
    printf("\n");

    doneProgramming = true;

}

bool Stm32p::parseHex(QTextStream* infile, unsigned long* address, QByteArray* data)
{
    bool ready = false;
    QByteArray row;
    unsigned long addr = *address;

    while (!ready && !infile->atEnd())
    {
        row = infile->readLine().toUtf8();
        if (row.at(0) == ':')
        {
            bool b = false;

            int type = row.mid(7, 2).toInt(&b, 16);
            int numOfBytes = row.mid(1, 2).toInt(&b, 16);

            if (type == 4) /* this is Extended Linear Address Record */
            {
                addr = row.mid(9, 4).toLong(&b, 16) << 16;
                ready = false;
            }
            else if (type == 0) /* this is data record */
            {
                addr = (addr & 0xFFFF0000) | row.mid(3, 4).toLong(&b, 16);

                QByteArray tmp;

                for (int i=0; i<numOfBytes; i++)
                    tmp.append(row.mid(9+2*i, 2).toInt(&b, 16));

                *data = tmp;
                *address = addr;

                ready = true;
            }
            else
                ready = false; /* continue to next row */
        }
        else
        {
            qCritical() << "Error in hex file";
            return false;
        }
    }

    return !infile->atEnd();
}




