#ifndef __SRE_RWROOT_H
#define __SRE_RWROOT_H

UINT32 SRE_ReadRootStatus(VOID);
UINT32 SRE_WriteRootStatus(UINT32 status);

/*
 *  mask the enabled BIT.
 *  need to change with operation_configs simultaneously.
 */
#define WRITE_MASK 0x387DF


/*
 * READ_MASK don't take OEMINFO_BIT | ROOTPROCBIT in count.
 *	OEMINFO_BIT : not status on last poweron.
 *	ROOTPROCBIT : not accurate enough to take account.
 *
 *	on  :	do scan and verify
 *	off :	not support, not scan or verify
 */
#define READ_MASK 0xFFFFFEFC

typedef enum {
	ROOTSTATE_BIT   = 0,		/* 0	on */
	/* read from fastboot */
	OEMINFO_BIT,			/* 1 	on */
	FBLOCK_YELLOW_BIT,		/* 2	on */
	FBLOCK_RED_BIT,			/* 3	on */
	FBLOCK_ORANGE_BIT,		/* 4	on */
					/* 5	off */
	/* dy scan result */
	KERNELCODEBIT   = 6,		/* 6    on */
	SYSTEMCALLBIT,			/* 7    on */
	ROOTPROCBIT,			/* 8    on */
	SESTATUSBIT,			/* 9    on */
	SEHOOKBIT       = 10,		/* 10   on */
	SEPOLICYBIT,			/* 11   off */
	PROCINTERBIT,			/* 12	off */
	FRAMINTERBIT,			/* 13	off */
	INAPPINTERBIT,			/* 14	off */
	NOOPBIT        = 15,		/* 15	on */
	ITIMEOUTBIT,			/* 16	on */
	CHECKFAILBIT,                   /* 17   on */
	TOTALBIT
} ROOTSTATUSBIT;

#define	RWROOT_RET_SUCCESS		(0)
#define	RWBOOT_RET_FAILURE		(0xFFFFFFFF)


#endif
