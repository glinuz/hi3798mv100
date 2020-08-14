/******************************************************************************
*             Copyright 2009 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: hi_type.h
* Description: STB Step-2 Common data types of the system
*
* History:
* Version  Date        Author                               DefectNum  Description
* 1.0      2009/03/19  Hisilicon STB Step-2 software group  NULL       Create this file.
******************************************************************************/

#ifndef __HI_TYPE_H__
#define __HI_TYPE_H__


/*----------------------------------------------*
/*--------------------------------------------------------------------------------------------------------------*
 * Defintion of basic data types. The data types are applicable to both the application layer and kernel codes. *
 * CNcomment: 基本数据类型定义，应用层和内核代码均使用                                                             *
 *--------------------------------------------------------------------------------------------------------------*/

typedef unsigned char       HI_U8;
typedef unsigned char       HI_UCHAR;
typedef unsigned int         HI_U16;
typedef unsigned long        HI_U32;

typedef char                HI_S8;
typedef int                  HI_S16;
typedef long                 HI_S32;

typedef void                 HI_VOID;

typedef char                HI_CHAR;
typedef char*               HI_PCHAR;

typedef unsigned long       HI_SIZE_T;
typedef unsigned long       HI_LENGTH_T;

typedef int                 STATUS;

typedef enum 
{
    HI_FALSE    = 0,
    HI_TRUE     = 1
} HI_BOOL;

/* struct */
typedef union hiDATA_U32_S
{
	HI_U32  val32;
	HI_U16  val16[2];
	HI_U8    val8[4];
}DATA_U32_S;

#ifndef NULL
#define NULL                (0U)
#endif

#define HI_NULL             (0U)
#define HI_NULL_PTR         (0U)

#define HI_SUCCESS          (0)
#define HI_FAILURE          (-1)

#endif /* __HI_TYPE_H__ */

