#include "i2ctester.h"

i2cTester::i2cTester(unsigned char address)
{
    i2cAddress = address;
}

void i2cTester::testRead(int count)
{
    qDebug() << readBytes(i2cAddress, count).toHex();
}

void i2cTester::testWrite(unsigned char data)
{
    char buf[1];

    buf[0] = data;

    writeBytes(i2cAddress, buf, 1);
}
