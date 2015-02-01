STM32 I2C Programmer for Jolla TOH
====================

Program STM32 through I2C directly from Jolla.

Tested to work with STM32F401

``stm32-programmer``

```
 -p filename     program hex file
 -s              reset and start after programming
 -r addr count   test reading from i2c address (hex)
 -w addr data,.. test write data bytes (comma separated) to i2c addres (hex)
 -o              shutdown
 -x              reboot
```

For more details, see ST Documents AN2606, AN4221
