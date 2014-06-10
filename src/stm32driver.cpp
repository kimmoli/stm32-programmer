#include "stm32driver.h"

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
