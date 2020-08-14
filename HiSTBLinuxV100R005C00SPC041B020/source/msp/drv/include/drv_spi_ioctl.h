#ifndef __DRV_SPI_IOCTL_H__
#define __DRV_SPI_IOCTL_H__

#include "hi_type.h"

/********************************* Ioctl definitions ************/

typedef struct
{
    HI_U8   devId;
    HI_U8  *sData;
    HI_U32  sDataCnt;
} SPI_DATA_S;

typedef struct
{
    HI_U8   devId;
    HI_U32  sData;
    HI_U32  sDataCnt;
} SPI_DATA_COMPAT_S;

typedef struct hiSPI_DATAEX_S
{
    HI_U8     devId;
    HI_U8     *sData;
    HI_U32    sDataCnt;
    HI_U8     *rData;
    HI_U32    rDataCnt;
} SPI_DATAEX_S;

typedef struct SPI_DATAEX_COMPAT_S
{
    HI_U8     devId;
    HI_U32    sData;
    HI_U32    sDataCnt;
    HI_U32    rData;
    HI_U32    rDataCnt;
} SPI_DATAEX_COMPAT_S;

typedef struct
{
    HI_U8   devId;
    HI_U8   mode;
    HI_U8   spo;
    HI_U8   sph;
    HI_U8   dss;
	HI_U8   cscfg;
	
} SPI_FFORM_S;

typedef struct
{
    HI_U8   devId;
    HI_U8   setbend;
} SPI_BLEND_S;

typedef struct
{
    HI_U8   devId;
    HI_U8   setLoop;
} SPI_LOOP_S;

#define CMD_SPI_READ            _IOWR(HI_ID_SPI, 0x1, SPI_DATA_S)
#define CMD_SPI_WRITE           _IOWR(HI_ID_SPI, 0x2, SPI_DATA_S)
#define CMD_SPI_SET_ATTR        _IOWR(HI_ID_SPI, 0x3, SPI_FFORM_S)
#define CMD_SPI_GET_ATTR        _IOWR(HI_ID_SPI, 0x4, SPI_FFORM_S)
#define CMD_SPI_OPEN            _IOW (HI_ID_SPI, 0x5, HI_U32)
#define CMD_SPI_CLOSE           _IOW (HI_ID_SPI, 0x6, HI_U32)
#define CMD_SPI_SET_BLEND       _IOWR(HI_ID_SPI, 0x7, SPI_BLEND_S)
#define CMD_SPI_GET_BLEND       _IOWR(HI_ID_SPI, 0x8, SPI_BLEND_S)
#define CMD_SPI_SET_CLK         _IOW (HI_ID_SPI, 0x9, HI_U32)
#define CMD_SPI_READEX 			_IOWR(HI_ID_SPI, 0xa, SPI_DATAEX_S)
#define CMD_SPI_SET_LOOP        _IOW (HI_ID_SPI, 0xb, SPI_LOOP_S)
#define CMD_SPI_RW_LOOP         _IOWR(HI_ID_SPI, 0xc, SPI_DATAEX_S)

#endif

