/*******************************************************************************
 *
 * FILE NAME          : MxL68x_FwDownload.h
 * 
 * AUTHOR             : Dong Liu
 *  
 * DATE CREATED       : 05/08/2011
 *
 * DESCRIPTION        : This file contains MxL68x common control register 
 *                      definitions
 *
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL68X_FIRMWARE_H__
#define __MXL68X_FIRMWARE_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/
#include "MaxLinearDataTypes.h"
#include "MxL68x_PhyCfg.h"
#include "MxL68x_OEM_Drv.h"

/******************************************************************************
    Macros
******************************************************************************/

#define MBIN_FORMAT_VERSION               '1'
#define MBIN_FILE_HEADER_ID               'M'
#define MBIN_SEGMENT_HEADER_ID            'S'
#define MBIN_MAX_FILE_LENGTH              (1<<23)

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/
typedef struct
{
  UINT8 id;                 /* 0x00-0x00: file identifier ('M') */
  UINT8 fmtVersion;         /* 0x01-0x01: file format version in ascii (default to '1') */
  UINT8 headerLen;          /* 0x02-0x02: header length (<256) in bytes, should be sizeof(MBIN_FILE_HEADER_T) */
  UINT8 numSegments;        /* 0x03-0x03: number of segments in the image (<256) */
  UINT8 entryAddress[4];    /* 0x04-0x07: entry address of execution, in big endian */
  UINT8 imageSize24[3];     /* 0x08-0x0A: 24-bit image size in bytes, not including header, in big endian */
  UINT8 imageChecksum;      /* 0x0B-0x0B: 8-bit checksum of file data, not including header */
  UINT8 reserved[4];        /* 0x0C-0x0F: reserved for future use */
} MBIN_FILE_HEADER_T;

typedef struct
{
  MBIN_FILE_HEADER_T  header;    /* 0x00: file header */
  UINT8 data[1];                 /* 0x10: place holder for accessing starting of filedata */
} MBIN_FILE_T;

typedef struct
{
  UINT8 id;                      /* 0x00-0x00: segment identifier (always 'S' for now) */
  UINT8 len24[3];                /* 0x01-0x03: data length in 24-bit big endian, not including header */
                                 /* PLEASE note: */
                                 /*    len24 is the real length of following data segement, and the next memory */
                                 /*    segment (if any) will start from next 32-bit aligned address, which is   */
                                 /*    &data[(len24+3)&(~3)]                                                    */
  UINT8 address[4];              /* 0x04-0x07: loading address of this segment, in big endian                   */
} MBIN_SEGMENT_HEADER_T;

typedef struct
{
  MBIN_SEGMENT_HEADER_T header;  /* 0x00: segment header */
  UINT8 data[1];                 /* 0x08: place holder for accessing starting of data */
} MBIN_SEGMENT_T;

/******************************************************************************
    Global Variable Declarations
******************************************************************************/

/******************************************************************************
    Prototypes
******************************************************************************/
// Basic address operation functions
void Ctrl_SetBigEndian24(UINT8 buf[], UINT32 val);
void Ctrl_SetBigEndian32(UINT8 buf[], UINT32 val);
UINT32 Ctrl_GetBigEndian24(const UINT8 buf[]);
UINT32 Ctrl_GetBigEndian32(const UINT8 buf[]);

MXL_STATUS_E Ctrl_DownloadFirmware(UINT8 devId,MBIN_FILE_T* mbinPtr,UINT8 enableRun);
MXL_STATUS_E Ctrl_DownloadFirmwareSegment(UINT8 devId, UINT32 address, const UINT8* dataPtr, UINT32 dataLen);
MXL_STATUS_E Ctrl_SendDownloadCommand(UINT8 devId, MXL_CMD_ID_E commandId, void* dataPtr, UINT32 dataLen, UINT16 downloadBlockCnt);

#endif