#ifndef __DRV_SCI_REG_H__
#define __DRV_SCI_REG_H__

#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef union
{
	struct
	{
		HI_U32	scidata	   : 8;
		HI_U32  parity	   : 1;
		HI_U32	Reserved   : 23;
	}Bits;

	HI_U32 Value;
}SCI_DATA_REG;

typedef union
{
	struct
	{
		HI_U32	sense	   : 1;
		HI_U32  order	   : 1;
		HI_U32	txparity   : 1;
        HI_U32	txnak      : 1;
        HI_U32	rxparity   : 1;
        HI_U32	rxnak      : 1;
        HI_U32	clkdis     : 1;
        HI_U32	clkval     : 1;

        HI_U32	paritybit  : 1;
        HI_U32	vccen_inv  : 1;
        HI_U32	detect_inv : 1;
        HI_U32	Reserved   : 21;
	}Bits;

	HI_U32 Value;
}SCI_SCR0_REG;

#if defined(CHIP_TYPE_hi3798cv200)||defined (CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)||defined(CHIP_TYPE_hi3796mv200)
typedef union
{
	struct
	{
		HI_U32	atrden	   : 1;
		HI_U32  blken	   : 1;
        HI_U32  mode	   : 1;
        HI_U32  clkzl	   : 1;
        HI_U32  bgten	   : 1;
        HI_U32  exdbnce	   : 1;
		HI_U32  synccard   : 1;
		HI_U32  rstoutctl  : 1;
		HI_U32  vccenoutctl: 1;

        HI_U32	Reserved   : 23;
	}Bits;

	HI_U32 Value;
}SCI_SCR1_REG;

#else

typedef union
{
	struct
	{
		HI_U32	atrden	   : 1;
		HI_U32  blken	   : 1;
        HI_U32  mode	   : 1;
        HI_U32  clkzl	   : 1;
        HI_U32  bgten	   : 1;
        HI_U32  exdbnce	   : 1;
		HI_U32  synccard   : 1;

        HI_U32	Reserved   : 25;
	}Bits;

	HI_U32 Value;
}SCI_SCR1_REG;
#endif


typedef union
{
	struct
	{
		HI_U32	startup	   : 1;
		HI_U32  finish	   : 1;
        HI_U32  wreset	   : 1;

        HI_U32	Reserved   : 29;
	}Bits;

	HI_U32 Value;
}SCI_SCR2_REG;

typedef union
{
	struct
	{
		HI_U32	clkicc	   : 8;

        HI_U32	Reserved   : 24;
	}Bits;

	HI_U32 Value;
}SCI_CLKICC_REG;

typedef union
{
	struct
	{
		HI_U32	value	   : 8;

        HI_U32	Reserved   : 24;
	}Bits;

	HI_U32 Value;
}SCI_VALUE_REG;

typedef union
{
	struct
	{
		HI_U32	baud	   : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_BAUD_REG;

typedef union
{
	struct
	{
		HI_U32	txtide	   : 6;
        HI_U32	rxtide	   : 6;

        HI_U32	Reserved   : 20;
	}Bits;

	HI_U32 Value;
}SCI_TIDE_REG;

typedef union
{
	struct
	{
		HI_U32	rxdmae	   : 1;
        HI_U32	txdmae	   : 1;

        HI_U32	Reserved   : 30;
	}Bits;

	HI_U32 Value;
}SCI_DMACR_REG;

typedef union
{
	struct
	{
		HI_U32	stable	   : 10;

        HI_U32	Reserved   : 22;
	}Bits;

	HI_U32 Value;
}SCI_STABLE_REG;

typedef union
{
	struct
	{
		HI_U32	atime	   : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_ATIME_REG;

typedef union
{
	struct
	{
		HI_U32	dtime	   : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_DTIME_REG;

typedef union
{
	struct
	{
		HI_U32	atrstime   : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_ATRSTIME_REG;

typedef union
{
	struct
	{
		HI_U32	atrdtime   : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_ATRDTIME_REG;

typedef union
{
	struct
	{
		HI_U32	stoptime   : 12;

        HI_U32	Reserved   : 20;
	}Bits;

	HI_U32 Value;
}SCI_STOPTIME_REG;

typedef union
{
	struct
	{
		HI_U32	starttime  : 12;

        HI_U32	Reserved   : 20;
	}Bits;

	HI_U32 Value;
}SCI_STARTTIME_REG;

typedef union
{
	struct
	{
		HI_U32	txretry    : 3;
        HI_U32	rxretry    : 3;

        HI_U32	Reserved   : 26;
	}Bits;

	HI_U32 Value;
}SCI_RETRY_REG;


typedef union
{
	struct
	{
		HI_U32	chtimels   : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_CHTIMELS_REG;

typedef union
{
	struct
	{
		HI_U32	chtimems   : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_CHTIMEMS_REG;

typedef union
{
	struct
	{
		HI_U32	blktimels  : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_BLKTIMELS_REG;

typedef union
{
	struct
	{
		HI_U32	blktimems  : 16;

        HI_U32	Reserved   : 16;
	}Bits;

	HI_U32 Value;
}SCI_BLKTIMEMS_REG;

typedef union
{
	struct
	{
		HI_U32	scichguard   : 8;

        HI_U32	Reserved     : 24;
	}Bits;

	HI_U32 Value;
}SCI_CHGUARD_REG;

typedef union
{
	struct
	{
		HI_U32	sciblkguard   : 8;

        HI_U32	Reserved      : 24;
	}Bits;

	HI_U32 Value;
}SCI_BLKGUARD_REG;

typedef union
{
	struct
	{
		HI_U32	rxtime    : 16;

        HI_U32	Reserved  : 16;
	}Bits;

	HI_U32 Value;
}SCI_RXTIME_REG;

typedef union
{
	struct
	{
		HI_U32	txff       : 1;
        HI_U32	txfe       : 1;
        HI_U32	rxff       : 1;
        HI_U32	rxfe       : 1;

        HI_U32	Reserved   : 28;
	}Bits;

	HI_U32 Value;
}SCI_FIFOSTATUS_REG;

typedef union
{
	struct
	{
        HI_U32	txcount    : 6;

        HI_U32	Reserved   : 26;
	}Bits;

	HI_U32 Value;
}SCI_TXCOUNT_REG;

typedef union
{
	struct
	{
        HI_U32	rxcount    : 6;

        HI_U32	Reserved   : 26;
	}Bits;

	HI_U32 Value;
}SCI_RXCOUNT_REG;

typedef union
{
	struct
	{
        HI_U32	cardinim    : 1;
        HI_U32	cardoutim   : 1;
        HI_U32	cardupim    : 1;
        HI_U32	carddnim    : 1;
        HI_U32	txerrim     : 1;
        HI_U32	atrstoutim  : 1;
        HI_U32	atrdtoutim  : 1;
        HI_U32	blktoutim   : 1;

        HI_U32	chtoutim    : 1;
        HI_U32	rtoutim     : 1;
        HI_U32	rorim       : 1;
        HI_U32	clkstpim    : 1;
        HI_U32	clkactim    : 1;
        HI_U32	rxtideim    : 1;
        HI_U32	txtideim    : 1;

        HI_U32	Reserved    : 17;
	}Bits;

	HI_U32 Value;
}SCI_IMSC_REG;

typedef union
{
	struct
	{
        HI_U32	cardinris    : 1;
        HI_U32	cardoutris   : 1;
        HI_U32	cardupris    : 1;
        HI_U32	carddnris    : 1;
        HI_U32	txerrris     : 1;
        HI_U32	atrstoutris  : 1;
        HI_U32	atrdtoutiris : 1;
        HI_U32	blktoutris   : 1;

        HI_U32	chtoutris    : 1;
        HI_U32	rtoutris     : 1;
        HI_U32	rorris       : 1;
        HI_U32	clkstpris    : 1;
        HI_U32	clkactris    : 1;
        HI_U32	rxtideris    : 1;
        HI_U32	txtideris    : 1;

        HI_U32	Reserved     : 17;
	}Bits;

	HI_U32 Value;
}SCI_RIS_REG;

typedef union
{
	struct
	{
        HI_U32	cardinmis    : 1;
        HI_U32	cardoutmis   : 1;
        HI_U32	cardupmis    : 1;
        HI_U32	carddnmis    : 1;
        HI_U32	txerrmis     : 1;
        HI_U32	atrstoutmis  : 1;
        HI_U32	atrdtoutimis : 1;
        HI_U32	blktoutmis   : 1;

        HI_U32	chtoutmis    : 1;
        HI_U32	rtoutmis     : 1;
        HI_U32	rormis       : 1;
        HI_U32	clkstpmis    : 1;
        HI_U32	clkactmis    : 1;
        HI_U32	rxtidemis    : 1;
        HI_U32	txtidemis    : 1;

        HI_U32	Reserved     : 17;
	}Bits;

	HI_U32 Value;
}SCI_MIS_REG;

typedef union
{
	struct
	{
        HI_U32	cardinic    : 1;
        HI_U32	cardoutic   : 1;
        HI_U32	cardupic    : 1;
        HI_U32	carddnic    : 1;
        HI_U32	txerric     : 1;
        HI_U32	atrstoutic  : 1;
        HI_U32	atrdtoutiic : 1;
        HI_U32	blktoutic   : 1;

        HI_U32	chtoutic    : 1;
        HI_U32	rtoutic     : 1;
        HI_U32	roric       : 1;
        HI_U32	clkstpic    : 1;
        HI_U32	clkactic    : 1;

        HI_U32	Reserved    : 19;
	}Bits;

	HI_U32 Value;
}SCI_ICR_REG;

typedef union
{
	struct
	{
        HI_U32	power          : 1;
        HI_U32	creset         : 1;
        HI_U32	clken          : 1;
        HI_U32	dataen         : 1;
        HI_U32	fcb            : 1;
        HI_U32	nsciclkouten   : 1;
        HI_U32	nsciclken      : 1;
        HI_U32	sciclkout      : 1;

        HI_U32	nscidataouten  : 1;
        HI_U32	nscidataen     : 1;
        HI_U32	cardpresent    : 1;

        HI_U32	Reserved       : 21;
	}Bits;

	HI_U32 Value;
}SCI_SYNCACT_REG;

typedef union
{
	struct
	{
        HI_U32	wdata        : 1;
        HI_U32	wclk         : 1;
        HI_U32	wdataen      : 1;
        HI_U32	wclken       : 1;
        HI_U32	wrst         : 1;
        HI_U32	wfcb         : 1;

        HI_U32	Reserved     : 22;
	}Bits;

	HI_U32 Value;
}SCI_SYNCTX_REG;

typedef union
{
	struct
	{
        HI_U32	rdata        : 1;
        HI_U32	rclk         : 1;

        HI_U32	Reserved     : 30;
	}Bits;

	HI_U32 Value;
}SCI_SYNCRX_REG;

typedef struct
{
    SCI_DATA_REG          SciData;
    SCI_SCR0_REG          SciCr0;
    SCI_SCR1_REG          SciCr1;
    SCI_SCR2_REG          SciCr2;
    SCI_CLKICC_REG        SciClkIcc;
    SCI_VALUE_REG         SciValue;
    SCI_BAUD_REG          SciBaud;
    SCI_TIDE_REG          SciTide;
    SCI_DMACR_REG         SciDmaCr;
    SCI_STABLE_REG        SciStable;
    SCI_ATIME_REG         SciATime;
    SCI_DTIME_REG         SciDTime;
    SCI_ATRSTIME_REG      SciAtrsTime;
    SCI_ATRDTIME_REG      SciAtrdTime;
    SCI_STOPTIME_REG      SciStopTime;
    SCI_STARTTIME_REG     SciStartTime;
    SCI_RETRY_REG         SciRetry;
    SCI_CHTIMELS_REG      SciChTimels;
    SCI_CHTIMEMS_REG      SciChTimems;
    SCI_BLKTIMELS_REG     SciBlkTimels;
    SCI_BLKTIMEMS_REG     SciBlkTimems;
    SCI_CHGUARD_REG       SciChGuard;
    SCI_BLKGUARD_REG      SciBlkGuard;
    SCI_RXTIME_REG        SciRxTime;
    SCI_FIFOSTATUS_REG    SciFifoStatus;
    SCI_TXCOUNT_REG       SciTxCount;
    SCI_RXCOUNT_REG       SciRxCount;
    SCI_IMSC_REG          SciImsc;
    SCI_RIS_REG           SciRis;
    SCI_MIS_REG           SciMis;
    SCI_ICR_REG           SciIcr;
    SCI_SYNCACT_REG       SciSyncAct;
    SCI_SYNCTX_REG        SciSyncTx;
    SCI_SYNCRX_REG	      SciSyncRx;
}SCI_REG_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
