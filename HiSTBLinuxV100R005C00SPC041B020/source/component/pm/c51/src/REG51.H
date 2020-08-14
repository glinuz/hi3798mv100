
#ifndef __REG51_H__
#define __REG51_H__
#include "hi_type.h"


/*Special register define*/
/*CNcomment: 特殊功能寄存器定义 */
sfr CMD_STATUS_REG = 0x91;
sfr CMD_REG = 0x92;  /* [0]: Cmd Valid
						[1]: read/write flag
							1:write, 
							0: read
						[2]: access type
							1: access isntruction ram, 
							0: access ahb bus
						[7:3]: reserved
					 */
sfr ADDR_REG_0 = 0x93; 
sfr ADDR_REG_1 = 0x94; 
sfr ADDR_REG_2 = 0x95; 
sfr ADDR_REG_3 = 0x96; 
sfr INT_STATUS_ARM = 0xB9; /* intr status reg from arm */
sfr INT_STATUS_IP0 = 0xBA; /* intr status reg from arm */
sfr INT_STATUS_IP1 = 0xBB; /* intr status reg from arm */
sfr INT_STATUS_IP2 = 0xBC; /* intr status reg from arm */
sfr INT_STATUS_IP3 = 0xBD; /* intr status reg from arm */
sfr MCU_TO_ARM_INT = 0xB1; /* 8051 mcu intr to arm */
sfr ARM_INTR_MASK = 0xB2;
sfr INT_MASK_0 = 0xB3;
sfr INT_MASK_1 = 0xB4;
sfr INT_MASK_2 = 0xB5;
sfr INT_MASK_3 = 0xB6;

sfr ARM_INT_CLEAR = 0xC4;  /* clear intr from arm */

sfr WR_DATA_REG_0 = 0x9A;
sfr WR_DATA_REG_1 = 0x9B; 
sfr WR_DATA_REG_2 = 0x9C; 
sfr WR_DATA_REG_3 = 0x9D; 

sfr RD_DATA_REG_0 = 0xA1;
sfr RD_DATA_REG_1 = 0xA2; 
sfr RD_DATA_REG_2 = 0xA3; 
sfr RD_DATA_REG_3 = 0xA4; 


/*  BYTE Register  */
sfr P0   = 0x80;
sfr P1   = 0x90;
sfr P2   = 0xA0;
sfr P3   = 0xB0;
sfr PSW  = 0xD0;
sfr ACC  = 0xE0;
sfr B    = 0xF0;
sfr SP   = 0x81;
sfr DPL  = 0x82;
sfr DPH  = 0x83;
sfr PCON = 0x87;
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TL1  = 0x8B;
sfr TH0  = 0x8C;
sfr TH1  = 0x8D;
sfr IE   = 0xA8;
sfr IP   = 0xB8;
sfr SCON = 0x98;
sfr SBUF = 0x99;


sfr Wdog_CFG = 0xC1;
sfr Wdog_Cnt0 = 0xC2;
sfr Wdog_Cnt1 = 0xC3;


/*  BIT Register  */
/*  PSW   */
sbit CY   = 0xD7;
sbit AC   = 0xD6;
sbit F0   = 0xD5;
sbit RS1  = 0xD4;
sbit RS0  = 0xD3;
sbit OV   = 0xD2;
sbit P    = 0xD0;

/*  TCON timer */
sbit TF1  = 0x8F;
sbit TR1  = 0x8E;
sbit TF0  = 0x8D;
sbit TR0  = 0x8C;
sbit IE1  = 0x8B;
sbit IT1  = 0x8A;
sbit IE0  = 0x89;
sbit IT0  = 0x88;

/*  IE   */
sbit EA   = 0xAF;
sbit ES   = 0xAC;
sbit ET1  = 0xAB;
sbit EX1  = 0xAA;
sbit ET0  = 0xA9;
sbit EX0  = 0xA8;

/*  IP   */ 
sbit PS   = 0xBC;
sbit PT1  = 0xBB;
sbit PX1  = 0xBA;
sbit PT0  = 0xB9;
sbit PX0  = 0xB8;

/*  P3  */
sbit RD   = 0xB7;
sbit WR   = 0xB6;
sbit T1   = 0xB5;
sbit T0   = 0xB4;
sbit INT1 = 0xB3;
sbit INT0 = 0xB2;
sbit TXD  = 0xB1;
sbit RXD  = 0xB0;

/*  SCON  uart*/
sbit SM0  = 0x9F;
sbit SM1  = 0x9E;
sbit SM2  = 0x9D;
sbit REN  = 0x9C;
sbit TB8  = 0x9B;
sbit RB8  = 0x9A;
sbit TI   = 0x99;
sbit RI   = 0x98;

#endif
