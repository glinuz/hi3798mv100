#ifndef __HI_BOOT_COMMON_H__
#define __HI_BOOT_COMMON_H__

#include "hi_type.h"
#include "hi_module.h"
#include "hi_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HI_MMZ_INFOZONE_SIZE    (8 * 1024)
#define BOOT_ARGS_SIZE          (512)
#define MEM_BASE                (0x80000000)
#define NAND_DEF_PAGESIZE       (8 * 1024)

#define READ_REG(base, offset) \
    (*(volatile HI_U32   *)((HI_U32)(base) + (offset)))

typedef struct hiMMZ_INFO_S
{
    HI_CHAR         name[32];
    HI_U32          gfp;
    HI_U32          startAddr;
    HI_U32          size;
}MMZ_INFO_S;

typedef struct hiMMZ_BUFFER_S
{
    HI_U32 u32StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
}MMZ_BUFFER_S;

/** Define the chip type. */
typedef enum hiCHIP_TYPE_E
{
    HI_CHIP_TYPE_HI3716M,
    HI_CHIP_TYPE_HI3716H,
    HI_CHIP_TYPE_HI3716C,
    HI_CHIP_TYPE_HI3716D,

    HI_CHIP_TYPE_HI3720,
    HI_CHIP_TYPE_HI3712,
    HI_CHIP_TYPE_HI3715,

    HI_CHIP_TYPE_HI3718M,
    HI_CHIP_TYPE_HI3718C,
    HI_CHIP_TYPE_HI3719M,
    HI_CHIP_TYPE_HI3719C,
    HI_CHIP_TYPE_HI3719M_A,

    HI_CHIP_TYPE_HI3796C    = 0x20,
    HI_CHIP_TYPE_HI3798C,
    HI_CHIP_TYPE_HI3796M,
    HI_CHIP_TYPE_HI3798M,

    HI_CHIP_TYPE_HI3796C_A  = 0x40,
    HI_CHIP_TYPE_HI3798C_A,
    HI_CHIP_TYPE_HI3798C_B,

    HI_CHIP_TYPE_HI3798M_A,

    HI_CHIP_TYPE_BUTT
}HI_CHIP_TYPE_E;

/** Define the chip version. */
typedef enum hiCHIP_VERSION_E
{
    HI_CHIP_VERSION_V100 = 0x100,
    HI_CHIP_VERSION_V101 = 0x101,
    HI_CHIP_VERSION_V200 = 0x200,
    HI_CHIP_VERSION_V300 = 0x300,
    HI_CHIP_VERSION_V310 = 0x310,
    HI_CHIP_VERSION_V400 = 0x400,
    HI_CHIP_VERSION_V410 = 0x410,
    HI_CHIP_VERSION_V420 = 0x420,
    HI_CHIP_VERSION_BUTT
}HI_CHIP_VERSION_E;

typedef struct hiRECT_S
{
    HI_S32 s32X;
    HI_S32 s32Y;
    HI_S32 s32Width;
    HI_S32 s32Height;
} HI_RECT_S;

typedef enum
{
    HI_CHIP_PACKAGE_TYPE_BGA_15_15 = 0,
    HI_CHIP_PACKAGE_TYPE_BGA_16_16,
    HI_CHIP_PACKAGE_TYPE_BGA_19_19,
    HI_CHIP_PACKAGE_TYPE_BGA_23_23,
    HI_CHIP_PACKAGE_TYPE_BGA_31_31,
    HI_CHIP_PACKAGE_TYPE_QFP_216,
    HI_CHIP_PACKAGE_TYPE_BGA_21_21,
    HI_CHIP_PACKAGE_TYPE_BGA_14_14,
    HI_CHIP_PACKAGE_TYPE_BUTT
} HI_CHIP_PACKAGE_TYPE_E;

/********* common: string ************/
char * strtok_2(char * s,const char * ct);
char * strstr_2(const char * s1,const char * s2);
//char * strcpy_2(char * dest,const char *src);
char * strncpy_2(char * dest,const char *src, unsigned int count);
unsigned long simple_strtoul_2(const char *cp,char **endp,unsigned int base);

/********* common: memory ************/
HI_S32 HI_MEM_Alloc(HI_U32 *pu32PhyAddr, HI_U32 Len);
HI_VOID HI_MEM_Free(HI_VOID *pPhyAddr);

/********* common: log ************/
HI_VOID HI_LOG_Init(HI_VOID);
const HI_CHAR* HI_LOG_GetModuleName(HI_MOD_ID_E enModule);
HI_VOID HI_LOG_SetLevel(HI_MOD_ID_E enModule, HI_U32 u32LogLevel);
HI_VOID HI_LogOut(HI_U32 u32Level, HI_MOD_ID_E enModId, HI_U8 *pFuncName, HI_U32 u32LineNum, const char *format, ...);

/********* common: sys ************/
HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType,HI_CHIP_VERSION_E *penChipVersion);
HI_S32 HI_DRV_SYS_GetChipPackageType(HI_CHIP_PACKAGE_TYPE_E *penPackageType);

/********* common: osal ************/
#include <stdarg.h>
typedef va_list HI_VA_LIST_S;
HI_CHAR* HI_OSAL_Strncpy(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen);
HI_S32 HI_OSAL_Strncmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen);
HI_CHAR* HI_OSAL_Strncat(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen);
HI_S32 HI_OSAL_Snprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, ...);
HI_S32 HI_OSAL_Vsnprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, HI_VA_LIST_S stVAList);

#ifdef __cplusplus
}
#endif

#endif

