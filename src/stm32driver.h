#ifndef STM32DRIVER_H
#define STM32DRIVER_H
#include "driverBase.h"

/*
 * According to AN2606, STM32F401 I2C Bootloader address is 0x72
 */
#define STM32F401_ADDRESS 0x39

/*
 * I2C Bootloader constants
 */
#define CMD_GETVERSION  0x01
#define CMD_GETID       0x02
#define CMD_READMEMORY  0x11
#define CMD_WRITEMEMORY 0x31
#define CMD_ERASEMEMORY 0x44
#define ACK             0x79
#define NACK            0x1F
#define BUSY            0x76


class stm32Driver : public DriverBase
{
    Q_OBJECT
public:
    explicit stm32Driver(unsigned char address);
    ~stm32Driver();

    void init();
    QByteArray cmdGetId();
    QByteArray cmdGetBootloaderVersion();
    QByteArray cmdReadMemory(unsigned long address, unsigned char count);
    QByteArray cmdWriteMemory(unsigned long address, QByteArray data);
    QByteArray cmdEraseMemory(int sector);


private:
    unsigned char driverAddress;

};

#endif // STM32DRIVER_H
