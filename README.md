WIP
===

STM32 I2C Programmer for Jolla TOH
====================

Program STM32 through I2C directly from Jolla.

``stmtohprog [hexfile] {i2c address}``

Boot mode is selected by INT pin state while powering V33 up.

State | Boot from
----- | ----------
Low   | Enter bootloader (Boot from system memory)
High  | Normal boot (Boot from Flash)

For more details, see ST Documents AN2606, AN4221
