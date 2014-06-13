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
