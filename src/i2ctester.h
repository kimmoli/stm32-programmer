#ifndef I2CTESTER_H
#define I2CTESTER_H
#include "driverBase.h"

class i2cTester : public DriverBase
{
    Q_OBJECT
public:
    explicit i2cTester(unsigned char address);

    void testRead(int count);
    void testWrite(unsigned char data);

private:
    unsigned char i2cAddress;

};

#endif // I2CTESTER_H
