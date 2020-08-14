#ifndef __HI_REG_H__
#define __HI_REG_H__

/*************** !!! important !!! ***************/
/*
never write reg like this:

	Result = 0x300;
	HI_REG_WRITE32(SYS_PERI_CRG3_ADDR, Result); <-------wrong!

always write reg like this:

	HI_REG_READ32(SYS_PERI_CRG3_ADDR, Result);  <----read reg first.
	Result = Result | 0x300;                    <----change the bits you want to set.
	HI_REG_WRITE32(SYS_PERI_CRG3_ADDR, Result); <----write the value back to reg.
*/
#ifndef HI_REG_READ8
#define HI_REG_READ8(addr,result)  ((result) = *(volatile unsigned char *)(addr))
#endif

#ifndef HI_REG_READ16
#define HI_REG_READ16(addr,result)  ((result) = *(volatile unsigned short *)(addr))
#endif

#ifndef HI_REG_READ32
#define HI_REG_READ32(addr,result)  ((result) = *(volatile unsigned int *)(addr))
#endif

#ifndef HI_REG_WRITE8
#define HI_REG_WRITE8(addr,result)  (*(volatile unsigned char *)(addr) = (result))
#endif

#ifndef HI_REG_WRITE16
#define HI_REG_WRITE16(addr,result)  (*(volatile unsigned short *)(addr) = (result))
#endif

#ifndef HI_REG_WRITE32
#define HI_REG_WRITE32(addr,result)  (*(volatile unsigned int *)(addr) = (result))
#endif

#ifndef HI_REG_READ
#define HI_REG_READ HI_REG_READ32
#endif

#ifndef HI_REG_WRITE
#define HI_REG_WRITE HI_REG_WRITE32
#endif


/* you can also use the following macro: */
/*
								 startBit
								   |
bit31                              V                      bit0
  |                 |<-- bitsNum ->|                       |
  |--------------------------------------------------------|
  |   xxxxx         |  bits to set |            xxxx       |
  |--------------------------------------------------------|


for example:

HI_REG_WriteBits(0x101e0010, 0x400, 0x00000400):
		set reg 0x101e0010 bit10( total 1bit) to 0x1,

HI_REG_WriteBits(0x101e0010, 0x10000, 0x00078000):
		set reg 0x101e0010 bit15~bit18( total 4bit) to 0x2,

HI_REG_ReadBits(0x101e0010, 0xffffffff):
		read reg 0x101e0010 bit0~bit31(total 32bits),

HI_REG_ReadBits(0x101e0010, 0x00078000):
		read reg 0x101e0010 bit15~bit18( total 4bit ),


*/
#define HI_REG_WriteBits(addr, value, mask) do{ \
			HI_U32 __reg; \
			__reg = *(HI_U32*)(addr); \
			__reg = (__reg & ~(mask)) | ((value) & (mask)); \
			*(HI_U32*)(addr) = __reg; \
		}while(0)

#define HI_REG_ReadBits(addr, mask) ({ HI_U32 __value;\
			HI_U32 __reg; \
			__reg = *(HI_U32*)(addr); \
			__value = (__reg) & (mask);\
			__value;\
		})

#endif
