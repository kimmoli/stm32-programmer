#include "stm32driver.h"
#include <QThread>

stm32Driver::stm32Driver(unsigned char address)
{
    driverAddress = address;
    init();
}

stm32Driver::~stm32Driver()
{

}

void stm32Driver::init()
{

}

/*
 * Get ID command
 *
 * if success, returns 2-byte ID
 * if fail, returns empty array
 *
 * The STM32 device sends the bytes as follows:
 * • Byte 1: ACK
 * • Byte 2: N = the number of bytes - 1 (for STM32, N = 1), except for current byte and ACKs.
 * • Bytes 3-4: PID (product ID)
 *   – byte 3 = MSB
 *   – byte 4 = LSB
 * • Byte 5: ACK
 */
QByteArray stm32Driver::cmdGetId()
{
    char buf[2];

    buf[0] = CMD_GETID;
    buf[1] = CMD_GETID ^ 0xFF;

    if (!writeBytes(driverAddress, buf, 2))
        return QByteArray();

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray();

    QByteArray ret = readBytes(driverAddress, 3);

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray();

    return ret.mid(1, 2); /* Return just the id */
}

/*
 *
 * The STM32 sends the bytes as follows:
 * • Byte 1: ACK
 * • Byte 2: Bootloader version (0 < Version ≤255) (for example, 0x10 = Version 1.0)
 * • Byte 3: ACK
 */

QByteArray stm32Driver::cmdGetBootloaderVersion()
{
    char buf[2];

    buf[0] = CMD_GETVERSION;
    buf[1] = CMD_GETVERSION ^ 0xFF;

    if (!writeBytes(driverAddress, buf, 2))
        return QByteArray();

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray();

    QByteArray ret = readBytes(driverAddress, 1);

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray();

    return ret;
}

QByteArray stm32Driver::cmdReadMemory(unsigned long address, unsigned char count)
{
    char buf[5];

    buf[0] = CMD_READMEMORY;
    buf[1] = CMD_READMEMORY ^ 0xFF;

    if (!writeBytes(driverAddress, buf, 2))
        return QByteArray("1. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("1. ack");

    buf[0] = (address >> 24) & 0xff;
    buf[1] = (address >> 16) & 0xff;
    buf[2] = (address >> 8) & 0xff;
    buf[3] = (address) & 0xff;
    buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3]; /* chksum */

    if (!writeBytes(driverAddress, buf, 5))
        return QByteArray("2. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("2. ack");

    buf[0] = count - 1;
    buf[1] = buf[0] ^ 0xff;

    if (!writeBytes(driverAddress, buf, 2))
        return QByteArray("3. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("3. ack");

    QByteArray ret = readBytes(driverAddress, count);

    return ret;
}

/*
 * write memory
 */

QByteArray stm32Driver::cmdWriteMemory(unsigned long address, QByteArray data)
{
    char buf[258];
    char chksum;
    int count;
    int i;


    buf[0] = CMD_WRITEMEMORY;
    buf[1] = CMD_WRITEMEMORY ^ 0xFF;

    if (!writeBytes(driverAddress, buf, 2))
        return QByteArray("1. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("1. ack");

    buf[0] = (address >> 24) & 0xff;
    buf[1] = (address >> 16) & 0xff;
    buf[2] = (address >> 8) & 0xff;
    buf[3] = (address) & 0xff;
    buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3]; /* chksum */

    if (!writeBytes(driverAddress, buf, 5))
        return QByteArray("2. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("2. ack");

    count = data.length();

    qDebug() << "writing" << count << "bytes:" << data.toHex();

    buf[0] = count - 1;

    chksum = buf[0];

    for (i=0; i<count; i++)
    {
        buf[i+1] = data.at(i);
        chksum ^= data.at(i);
    }

    buf[count+1] = chksum;

    if (!writeBytes(driverAddress, buf, count + 2))
        return QByteArray("3. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("3. ack");

    return QByteArray("ok");
}

/*
 * Erase sector
 */

QByteArray stm32Driver::cmdEraseMemory(int sector)
{

    char buf[3];

    buf[0] = CMD_ERASEMEMORY;
    buf[1] = CMD_ERASEMEMORY ^ 0xFF;

    if (!writeBytes(driverAddress, buf, 2))
        return QByteArray("1. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("1. ack");

    /* Erase only one sector */
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = buf[0] ^ buf[1];

    if (!writeBytes(driverAddress, buf, 3))
        return QByteArray("2. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("2. ack");

    buf[0] = 0;
    buf[1] = sector & 0xff;
    buf[2] = buf[0] ^ buf[1];

    if (!writeBytes(driverAddress, buf, 3))
        return QByteArray("3. write");

    if (readBytes(driverAddress, 1).at(0) != ACK)
        return QByteArray("3. ack");

    return QByteArray("ok");
}











