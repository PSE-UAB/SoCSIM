## License

This project is licensed by GPLv3. See LICENSE file for more details.

## Virtual peripherals

### GPIO

. Button 1 is connected to PORTA, pin #0
. Button 2 is connected to PORTb, pin #1
. LED 1 is connected to PORTC, pin #7
. LED 2 is connected to PORTD, pin #12

## Memory map

All registers are 32 bit width.

| Address | Register | Comment |
| ---- | ---- | ---- |
| 0x1000 | GPIO A CTRL | 1 - out , 0 - in |
| 0x1004 | GPIO A INT | 1 - Interrupt enable, 0 - Disabled | 
| 0x1008 | GPIO_A_OUT | '1' or '0' set to output pin |
| 0x100C | GPIO_A_IN | '1' or '0' set by the input pin |
| 0x2000 | GPIO B CTRL | idem |
| 0x2004 | GPIO B INT | idem |
| 0x2008 | GPIO_B_OUT | idem |
| 0x200C | GPIO_B_IN | idem |
| 0x3000 | GPIO C CTRL | idem |
| 0x3004 | GPIO C INT | idem |
| 0x3008 | GPIO_C_OUT | idem |
| 0x300C | GPIO_C_IN | idem |
| 0x4000 | GPIO D CTRL | idem |
| 0x4004 | GPIO D INT | idem |
| 0x4008 | GPIO_D_OUT | idem |
| 0x400C | GPIO_D_IN | idem |
| 0xA000 | INTERRUPT CTRL | - |
| 0xA004 | INTERRUPT IRQ | - |
