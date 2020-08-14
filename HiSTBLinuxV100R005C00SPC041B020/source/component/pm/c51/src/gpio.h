#ifndef __GPIO_H__
#define __GPIO_H__

/* 1 register addr */
#define  GPIO5_BASE_ADDR   (0xf8004000) //3798C GPIO_STB0
//#define  GPIO5_BASE_ADDR   (0xf8009000) //3798C GPIO_STB1

#define  GPIO5_REG_DIR     (GPIO5_BASE_ADDR + 0x400)
#define  GPIO5_REG_IS      (GPIO5_BASE_ADDR + 0x404)
#define  GPIO5_REG_IBE     (GPIO5_BASE_ADDR + 0x408)
#define  GPIO5_REG_IEV     (GPIO5_BASE_ADDR + 0x40c)
#define  GPIO5_REG_IE      (GPIO5_BASE_ADDR + 0x410)
#define  GPIO5_REG_RIS     (GPIO5_BASE_ADDR + 0x414)
#define  GPIO5_REG_MIS     (GPIO5_BASE_ADDR + 0x418)
#define  GPIO5_REG_IC      (GPIO5_BASE_ADDR + 0x41c)
#define  GPIO5_REG_RSV     (GPIO5_BASE_ADDR + 0x420)


HI_VOID gpio5_dirset_bit(HI_U8 idx, HI_U8 val) reentrant;  
HI_VOID gpio5_write_bit(HI_U8 idx, HI_U8 val) reentrant;
HI_VOID gpio5_read_bit(HI_U8 idx, HI_U8* val);
HI_VOID gpio_SetIntType(HI_U8 u8GpioBitNum);
HI_VOID gpio_IntEnable(HI_U8 u8GpioBitNum, HI_BOOL IsEnable);
HI_VOID gpio_isr(HI_U8 u8GpioBitNum);
#endif