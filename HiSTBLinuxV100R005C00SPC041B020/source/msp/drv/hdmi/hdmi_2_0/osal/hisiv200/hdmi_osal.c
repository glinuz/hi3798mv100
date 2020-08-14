/******************************************************************************

  Copyright (C), 2010-2026, Hisilicon. Co., Ltd.

 ******************************************************************************
 * File Name     : hdmi_osal.c
 * Version       : 1.0
 * Author        : Hisilicon multimedia software group
 * Created       : 2015/6/27
 * Description   : Deferent linux version for HDMI model
 * History       :
 * Date          : 2015/6/27
 * Author        : l00232354
 * Modification  :
 *******************************************************************************
 */
#include "hdmi_osal.h"

/******************************************************************************/
/* operation functinos of kernel file                                         */
/******************************************************************************/

/** open a kernel file
**/
struct file* HDMI_OSAL_FileOpen(const HI_S8* ps8FileName, OSAL_FILE_FLAG_S eFlags)
{
    struct file *pFile = NULL;

	if (NULL == ps8FileName)
	{
		return NULL;
	}

    switch (eFlags)
    {
        case OSAL_RDWR:
            eFlags = O_RDWR | O_CREAT | O_TRUNC;
            break;
        default:
            eFlags = O_RDWR | O_CREAT | O_TRUNC;
            break;
    }

    pFile = filp_open(ps8FileName, eFlags | O_LARGEFILE, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    return (IS_ERR(pFile)) ? NULL : pFile;
}

/** close a kernel file
**/
HI_VOID HDMI_OSAL_FileClose(struct file * pFile)
{
    if (NULL != pFile)
    {
        filp_close(pFile, NULL);
    }

	return;
}

/** read a kernel file
**/
HI_S32 HDMI_OSAL_FileRead(struct file * pFile,  HI_U8* ps8Buf, HI_U32 u32Len)
{
	HI_S32 s32ReadLen = 0;
    mm_segment_t stOld_FS = {0};

    if (pFile == NULL || NULL == ps8Buf)
    {
        return -ENOENT; /* No such file or directory */
    }

    if (pFile->f_op->read == NULL)
    {
        return -ENOSYS; /* Function not implemented */
    }

    if (((pFile->f_flags & O_ACCMODE) & (O_RDONLY | O_RDWR)) != 0)
    {
        return -EACCES; /* Permission denied */
    }

	/* saved the original file space */
    stOld_FS = get_fs();

	/* extend to the kernel data space */
    set_fs(KERNEL_DS);

    s32ReadLen = pFile->f_op->read(pFile, ps8Buf, u32Len, &pFile->f_pos);

	/* Restore the original file space */
	set_fs(stOld_FS);

    return s32ReadLen;
}

/** write a kernel file
**/
HI_S32 HDMI_OSAL_FileWrite(struct file* pFile, HI_S8* ps8Buf, HI_U32 u32Len)
{
    HI_S32 s32WriteLen = 0;
    mm_segment_t stOld_FS = {0};

    if (pFile == NULL || ps8Buf == NULL)
    {
        return -ENOENT; /* No such file or directory */
    }

    if (pFile->f_op->write == NULL)
    {
        return -ENOSYS; /* Function not implemented */
    }

    if (((pFile->f_flags & O_ACCMODE) & (O_WRONLY | O_RDWR)) == 0)
    {
        return -EACCES; /* Permission denied */
    }

    stOld_FS = get_fs();
    set_fs(KERNEL_DS);

    s32WriteLen = pFile->f_op->write(pFile, ps8Buf, u32Len, &pFile->f_pos);

    set_fs(stOld_FS);

    return s32WriteLen;
}

/******************************************************************************/
/* operation functinos of stream                                              */
/******************************************************************************/

/** copy str to pszDest from pszSrc
**/
HI_CHAR* HDMI_OSAL_Strncpy(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen)
{
    return strncpy(pszDest, pszSrc, ulLen);
}

/** compare pszStr1 with pszStr2
**/
HI_S32 HDMI_OSAL_Strncmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen)
{
    return strncmp(pszStr1, pszStr2, ulLen);
}

/** casecompare pszStr1 with pszStr2
**/
HI_S32 HDMI_OSAL_Strncasecmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen)
{
    return strncasecmp(pszStr1, pszStr2, ulLen);
}

/** strncat pszSrc
**/
HI_CHAR* HDMI_OSAL_Strncat(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen)
{
    return strncat(pszDest, pszSrc, ulLen);
}

/** snprintf pszSrc
**/
HI_S32 HDMI_OSAL_Snprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, ...)
{
    HI_S32 s32Len;
    va_list stArgs = {0};

    va_start(stArgs, pszFormat);
    s32Len = vsnprintf(pszStr, ulLen, pszFormat, stArgs);
    va_end(stArgs);

    return s32Len;
}

/** vsnprintf pszSrc
**/
HI_S32 HDMI_OSAL_Vsnprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, va_list stVAList)
{
    return vsnprintf(pszStr, ulLen, pszFormat, stVAList);
}

/******************************************************************************/
/* operation functinos of get system time                                     */
/******************************************************************************/
/** get system time in Ms
**/
HI_U32 HDMI_OSAL_GetTimeInMs(HI_VOID)
{
	HI_U64   SysTime = 0;

    SysTime = sched_clock();
    do_div(SysTime, 1000000);
    return (HI_U32)SysTime;
}

/** get system time in Us
**/
HI_U32 HDMI_OSAL_GetTimeInUs(HI_VOID)
{
	HI_U32 CurrUs = 0;
    struct timeval CurrentTime;

    do_gettimeofday(&CurrentTime);
    CurrUs = (HI_U32)(CurrentTime.tv_sec*1000000 + CurrentTime.tv_usec);

	return CurrUs;
}

/******************************************************************************/
/* Gloabl Data                                                                */
/******************************************************************************/

/******************************************************************************/
/* Public Interface                                                           */
/******************************************************************************/

