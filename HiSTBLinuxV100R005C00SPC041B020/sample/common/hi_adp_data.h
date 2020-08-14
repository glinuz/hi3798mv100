#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "hi_type.h"

#define     MAX_PROGNAME_LENGTH 32
#define     MAX_AUDIO_LANGUAGE 5
#define     MAX_PROG_COUNT 200
#define     MAX_FRONTEND_COUNT 30

#define     SEARCHING_FRONTEND_ID	0xffff

typedef enum hiFRONTEND_TYPE_E
{
    FE_TYPE_RF   = 1,
    FE_TYPE_IP   = 2,
    FE_TYPE_FILE = 3
} FRONTEND_TYPE_E;

typedef enum hiFILE_TYPE_E
{
    FILE_TYPE_TS = 0,
    FILE_TYPE_ES = 1,
}FILE_TYPE_E;

typedef enum hiFILE_AUDIO_TYPE_E
{
    FILE_AUDIO_TYPE_NONE = 0,
    FILE_AUDIO_TYPE_AAC   = 1,
    FILE_AUDIO_TYPE_MP3   = 2,
    FILE_AUDIO_TYPE_AC3   = 3,
    FILE_AUDIO_TYPE_DTS = 4,
    FILE_AUDIO_TYPE_DRA = 5
} FILE_AUDIO_TYPE_E;

typedef enum hiFILE_VIDEO_TYPE_E
{
    FILE_VIDEO_TYPE_NONE = 0,
    FILE_VIDEO_TYPE_MPEG2   = 1,
    FILE_VIDEO_TYPE_MPEG4   = 2,
    FILE_VIDEO_TYPE_H263 = 3,
    FILE_VIDEO_TYPE_H264 = 4,
    FILE_VIDEO_TYPE_AVS = 5,
    FILE_VIDEO_TYPE_REAL = 6,
    FILE_VIDEO_TYPE_AV1 = 7
} FILE_VIDEO_TYPE_E;

typedef struct hiFE_TYPEIP_S
{
    HI_CHAR multiIPAddr[20];
    HI_U32  port;
} FE_TYPE_IP_S;

typedef struct hiFE_TYPERF_S
{
    HI_U32 u32TunerPort; /*Tuner port*/
    HI_U32 u32Frequency;  /*unit:MHZ */
    HI_U32 u32SymbolRate; /*unit:KHZ*/
    HI_U32 u32Modulation;  /* 0:16QAM 1:32QAM 2:64QAM 3:128QAM 4:256QAM*/
} FE_TYPE_RF_S;

typedef struct hiFE_TYPEFILE_S
{
    HI_CHAR path[256];
    FILE_TYPE_E filetype;
    FILE_VIDEO_TYPE_E videotype;
    FILE_AUDIO_TYPE_E audiotype;
} FE_TYPE_FILE_S;

/* channel dot struct */
typedef struct hiDB_FRONTEND_S
{
    FRONTEND_TYPE_E eFEType;
    HI_U16          u16NetworkID;
    HI_U16          u16TsID;
    union
    {
	 FE_TYPE_FILE_S  sFEParaFile;  //file type
        FE_TYPE_IP_S    sFEParaIp;  //IP type
        FE_TYPE_RF_S    sFEParaRf; //RF type
    }unFEtype;

} DB_FRONTEND_S;

/* program struct */
typedef struct hiDB_VIDEOEX_S
{
    HI_U16 u16VideoPid;
    HI_U32 u32VideoEncType;
} DB_VIDEOEX_S;

typedef struct hiDB_AUDIOEX_S
{
    HI_U16 u16audiopid;
    HI_U16 u16audiolan;
    HI_U32 u32AudioEncType;
} DB_AUDIOEX_S;

typedef struct  hiDB_PROGRAM_S
{
    HI_U16 u16FrontendID;
    HI_U16 u16NetworkID;
    HI_U16 u16TsID;
    HI_U16 u16ServiceID;

    HI_U8 enServiceType;

    HI_S8 ProgramName[MAX_PROGNAME_LENGTH];

    HI_U16 u16PmtPid;
    HI_U16 u16PcrPid;

    HI_U32 u16ProgProperty;

    HI_U16 u16AudioVol;
    HI_U8  u8AudioChannel;
    HI_U8  u8VideoChannel;

    DB_VIDEOEX_S VideoEX;
    DB_AUDIOEX_S AudioEX[MAX_AUDIO_LANGUAGE];

    HI_U16 Reserved;
} DB_PROGRAM_S;

HI_S32   DB_GetDVBProgInfoByServiceID(HI_U16 u16ServiceID, DB_PROGRAM_S *proginfo );
HI_S32	DB_GetDVBProgInfo(HI_U32 prognum, DB_PROGRAM_S *proginfo );
HI_S32	DB_SetDVBProgInfo(HI_U32 prognum, DB_PROGRAM_S *proginfo );
HI_S32	DB_AddDVBProg(DB_PROGRAM_S *proginfo );
HI_S32	DB_GetProgTotalCount(void);
HI_S32	DB_GetFEChanInfo(HI_U32 channum, DB_FRONTEND_S *chaninfo );
HI_S32	DB_SetFEChanInfo(HI_U32 channum, DB_FRONTEND_S *chaninfo );
HI_S32	DB_AddFEChan(DB_FRONTEND_S *chaninfo);
HI_S32	DB_GetFEChanTotalCount(void);
HI_S32	DB_Reset(void);
HI_S32	DB_RestoreFromFile(FILE *filestream);
HI_S32	DB_SaveToFile(FILE *filestream);

#endif
