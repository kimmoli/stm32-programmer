#include "i2ctester.h"

i2cTester::i2cTester(unsigned char address)
{
    i2cAddress = address;
}

void i2cTester::testRead(int count)
{
    qDebug() << readBytes(i2cAddress, count).toHex();
}

void i2cTester::testWrite(QByteArray data)
{
    char buf[255];

    for (int i=0 ; i<data.length(); i++)
    {
        buf[i] = data.at(i);
    }

    writeBytes(i2cAddress, buf, data.length());
}
