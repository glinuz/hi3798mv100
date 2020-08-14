#ifndef __SERIAL_H__
#define __SERIAL_H__

#define SERIAL_BASE		REG_BASE_UART0

#define UART_PL01x_DR		0x00
#define UART_PL01x_RSR		0x04
#define UART_PL01x_ECR		0x04
#define UART_PL01x_FR		0x18

#define UART_PL01x_FR_TXFE	0x80
#define UART_PL01x_FR_RXFF	0x40
#define UART_PL01x_FR_TXFF	0x20
#define UART_PL01x_FR_RXFE	0x10
#define UART_PL01x_FR_BUSY	0x08
#define UART_PL01x_FR_TMSK	(UART_PL01x_FR_TXFF + UART_PL01x_FR_BUSY)

#endif
