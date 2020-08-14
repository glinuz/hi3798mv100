#ifndef __IR_REGISTER_H__
#define __IR_REGISTER_H__

#define IR_EN		(0x00)
#define IR_CFG		(0x04)
#define IR_LEADS	(0x08)
#define IR_LEADE	(0x0c)
#define IR_SLEADE	(0x10)
#define IR_B0		(0x14)
#define IR_B1		(0X18)
#define IR_BUSY		(0x1c)
#define IR_DATAH	(0x20)
#define IR_DATAL	(0x24)

#define IR_INT_MASK	(0x28)
# define IR_INT_MASK_KEY	((unsigned int)~0xf)
# define IR_INT_MASK_SYMBOL	((unsigned int)~(0x7 << 16))

#define IR_INT_STATUS	(0x2c)
# define IR_INTMS_OVERRUN	(1 << 26)
# define IR_INTMS_TIME_OUT	(1 << 25)
# define IR_INTMS_SYMB_RCV	(1 << 24)
# define IR_INTMS_RELEASE	(1 << 19)
# define IR_INTMS_OVERFLOW	(1 << 18)
# define IR_INTMS_FRAMERR	(1 << 17)
# define IR_INTMS_RCV		(1 << 16)
# define IR_INTRS_OVERRUN	(1 << 10)
# define IR_INTRS_TIME_OUT	(1 << 9)
# define IR_INTRS_SYMB_RCV	(1 << 8)
# define IR_INTRS_RELEASE	(1 << 3)
# define IR_INTRS_OVERFLOW	(1 << 2)
# define IR_INTRS_FRAMERR	(1 << 1)
# define IR_INTRS_RCV		(1 << 0)
# define IR_KEY_STAT_MASK	(0xf << 16)
# define IR_SYMBOL_STAT_MASK	(0x7 << 24)

#define IR_INT_CLR	(0x30)
# define IR_INTC_RCV		(1 << 0)
# define IR_INTC_FRAMERR	(1 << 1)
# define IR_INTC_OVERFLOW	(1 << 2)
# define IR_INTC_RELEASE	(1 << 3)
# define IR_INTC_SYMB_RCV	(1 << 16)
# define IR_INTC_TIME_OUT	(1 << 17)
# define IR_INTC_OVERRUN	(1 << 18)

#define IR_START	(0x34)

#endif
