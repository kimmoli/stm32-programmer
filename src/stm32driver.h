#ifndef STM32DRIVER_H
#define STM32DRIVER_H
#include "driverBase.h"

class stm32Driver : public DriverBase
{
    Q_OBJECT
public:
    explicit stm32Driver(unsigned char address);
    ~stm32Driver();

    void init();

private:
    unsigned char driverAddress;

};

#endif // STM32DRIVER_H
