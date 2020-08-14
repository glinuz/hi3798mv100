/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon. Co., Ltd.

 ******************************************************************************
 * File Name     : drv_abs_debug.c
 * Version       : 1.0
 * Author        : Hisilicon multimedia software group
 * Created       : 2014/4/1
 * Description   : HDMI layer of debuging message printing Driver
 * History       :
 * Date          : 2014/4/1
 * Author        : l00271847
 * Modification  :
 *******************************************************************************
 */
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "hi_drv_mem.h"
#include "hi_drv_file.h"
#include "drv_hdmi_debug.h"
#include "hi_osal.h"


HI_S32  ABS_ClearMem(ABS_DBG_S_PTR pABSdbg)
{
    if (!pABSdbg)
    {
        ABS_PTK("ABS_ClearMem failed, pABSdbg(0x%p) is error\n", pABSdbg);
        return HI_FAILURE;
    }

    if (!(pABSdbg->pdbg_addr))
    {
        ABS_PTK("ABS_ClearMem failed, pABSdbg->pdbg_addr(0x%p) is error\n", pABSdbg->pdbg_addr);
        return HI_FAILURE;
    }

    ABS_MEMSET(pABSdbg->pdbg_addr, 0, pABSdbg->dbg_mem_size);
    pABSdbg->pdbg_pos = pABSdbg->pdbg_addr;

    return HI_SUCCESS;
}

HI_S32  ABS_SetDbgLevel(ABS_DBG_S_PTR pABSdbg, ABS_DBG_LEVEL_T level)
{
    if (!pABSdbg)
    {
        ABS_PTK("ABS_set_debug_level failed, pABSdbg(0x%p) is error\n", pABSdbg);
        return HI_FAILURE;
    }

    //if(level >= ABS_DBG_INFO && level < ABS_DBG_END)
    if(level < ABS_DBG_END)
    {
        pABSdbg->dbg_level = level;
    }
    else
    {
        level = ABS_DBG_INFO;
        ABS_ERR(pABSdbg->module_id, "ABS set debug level(%d) failed, use default 0.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* set the logtype, file or serial */
HI_S32  ABS_SetType(ABS_DBG_S_PTR pABSdbg, PRINT_TO_DEV_E logtype, struct file *pFile)
{
    if (!pABSdbg)
    {
        ABS_PTK("ABS_set_type_failed, pABSdbg(0x%p) is error\n", pABSdbg);
        return HI_FAILURE;
    }

    pABSdbg->dbg_type = logtype;

    if(pFile)
    {
        pABSdbg->pdbg_fp  = pFile;
    }

    return HI_SUCCESS;
}

/* set the function mask */
HI_S32  ABS_SetMask(ABS_DBG_S_PTR pABSdbg, HI_U32 mask)
{
    if (!pABSdbg)
    {
        ABS_PTK("ABS_set_mask_failed, pABSdbg(0x%p) is error\n", pABSdbg);
        return HI_FAILURE;
    }

    pABSdbg->dbg_mask = mask;

    return HI_SUCCESS;
}

/**
 * ABS_PtkToMem - get strings from memroy and print it out to file or serial.
 * @pABSdbg: pointer to the struct abs_dbg_struct
 * u8DbgLevel: debug level (info, warning, error, fatal),should be decide
 *             when user call this function.
 * u32FuncNum: There are 15 functions you can chose.
 * @u8Buf: include string which will be write into the memory. Max len (512 - 13) Bytes.
 *
 * If we write the string into the memory will cause a overflow error, we should cut off
 * a piece of the string from its head to be left content, and the others be the rigth
 * content. Then wirte the left content to the end of the memory to ensure will not
 * overflow it, and write the right content from the start address of the memory to
 * cover the old content.
 *
 * NOTE: If we found there is a read operation is not finished, we should not trying to
 * write anyting into the memory.
 */
HI_VOID ABS_PtkToMem(ABS_DBG_S_PTR pABSdbg, HI_U8 u8DbgLevel, HI_U32 u32FuncNum,  HI_U8 *u8Buf)
{
    HI_U32 len, left, right;
    //HI_U8  temp[ABS_DBG_MEM_IN_BUF_SIZE  + ABS_DBG_INF_HEAD_SIZE] = {0};
    HI_U8  temp[ABS_DBG_MEM_IN_BUF_SIZE  + ABS_DBG_INF_HEAD_SIZE + 1] = {0};

    if (!pABSdbg || !u8Buf)
    {
        ABS_PTK("ABS_ptk_to_mem : pABSdbg(0x%p) or u8Buf(0x%p) is error.\n", pABSdbg, u8Buf);
        return ;
    }

    if(!pABSdbg->pdbg_addr || !pABSdbg->pdbg_pos)
    {
        ABS_ERR(pABSdbg->module_id, "error : addr(0x%p) or pos(0x%p) is error.\n",
                                                        pABSdbg->pdbg_addr, pABSdbg->pdbg_pos);
        return ;
    }

    len = strlen(u8Buf);   //do not include '\0'
    if(len > ABS_DBG_MEM_IN_BUF_SIZE)
    {
        ABS_ERR(pABSdbg->module_id, "mem input len = %d is error.\n", len);
    }

    left = len + ABS_DBG_INF_HEAD_SIZE - 1; //reduce ABS_DBG_INF_HEAD's '\0'
    right = 0;

    //HI_OSAL_Snprintf(temp, ABS_BUF_DEBUG_LEN, "%s%04d%d%04d%s%s",
    //                            "##<", u32FuncNum, u8DbgLevel, len, ">##", u8Buf);

    HI_OSAL_Snprintf(temp, ABS_DBG_MEM_IN_BUF_SIZE  + ABS_DBG_INF_HEAD_SIZE,    
                "%s%04d%d%04d%s%s", "##<", u32FuncNum, u8DbgLevel, len, ">##", u8Buf);

    if(pABSdbg->pdbg_pos + left >= pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size)
    {
        left = pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - pABSdbg->pdbg_pos;
        right = len + ABS_DBG_INF_HEAD_SIZE - 1 - left;
    }

    if(HI_TRUE == pABSdbg->dbg_rflag)
    {
        return ;
    }

    ABS_DBG_LOCK(pABSdbg->dbg_lock);

    memcpy(pABSdbg->pdbg_pos, temp, left);

    if(right)
    {
        memcpy(pABSdbg->pdbg_addr, temp + left, right);
        pABSdbg->pdbg_pos = pABSdbg->pdbg_addr + right;
    }
    else
    {
        pABSdbg->pdbg_pos = pABSdbg->pdbg_pos + left;
    }

    *pABSdbg->pdbg_pos = '\0';
   
    ABS_DBG_UNLOCK(pABSdbg->dbg_lock);

    return ;
}

/**
 * ABS_MemOut - get strings from memroy and print it out to file or serial.
 * @pABSdbg: pointer to the struct abs_dbg_struct
 * @@pFuncName: pointer to the functions name's array.
 *
 * This happens when user space call echo logcatch > /proc/msp/hdmi0. pFuncName
 * should be intilized at first of all and it has [ABS_FUNC_15 + 1] strings to
 * save the function's name.
 * We first set pABSdbg->dbg_rflag to tell ABS_ptk_to_mem function we are reading
 * and do not write anything to the mem. When we finished reading, we should clear
 * this flag.
 * Next, we find ">##" to ensure the we can read useful massages from the information
 * head, and decide whether or not to printing the strings in memory.
 */
HI_S32 ABS_MemOut(ABS_DBG_S_PTR pABSdbg, const HI_CHAR **pFuncName)
{
    HI_U8 *pos = HI_NULL, *pmem = HI_NULL;
    HI_U8  u8FuncNumBuf[ABS_DBG_INF_DATA_WIDTH + 2] = {0}, u8StrLenBuf[ABS_DBG_INF_DATA_WIDTH + 1] = {0};
    HI_U32 u32FuncNum, u32StrLen = 0;
    HI_U8  u8DbgLevel;
    HI_S32 right, left, ret = 0;

    if(!pABSdbg)
    {
        ABS_PTK("ABS_mem_out failed, pABSdbg(0x%p) is error\n", pABSdbg);
        return -EPERM;
    }

    if(!pABSdbg->pdbg_addr || !pABSdbg->pdbg_pos)
    {
        ABS_ERR(pABSdbg->module_id, "error : addr(0x%p) or pos(0x%p) is error.\n",
                                                        pABSdbg->pdbg_addr, pABSdbg->pdbg_pos);
        return -EPERM;
    }

    if(!pFuncName)
    {
        ABS_ERR(pABSdbg->module_id, "error : **FuncName(0x%p) is error\n", pFuncName);
        return -EPERM;
    }

    //ABS_PTK("mem_L(=%d) : \n%s\n\n", strlen(pABSdbg->pdbg_addr), pABSdbg->pdbg_addr);
    //ABS_PTK("mem_R(=%d) : \n%s\n\n", strlen(pABSdbg->pdbg_addr + strlen(pABSdbg->pdbg_addr) + 1), pABSdbg->pdbg_addr + strlen(pABSdbg->pdbg_addr) + 1);

#if 1
    ABS_DBG_LOCK(pABSdbg->dbg_lock);
    pABSdbg->dbg_rflag = HI_TRUE;
    ABS_DBG_UNLOCK(pABSdbg->dbg_lock);

    pmem = pABSdbg->pdbg_addr;
    left = pABSdbg->pdbg_pos - pABSdbg->pdbg_addr;

    /* NULL ASCII code is 000, and if the last byte of mem isn't 000, means the mem is full */
    if(*(pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - 1))
    {
        right = pABSdbg->dbg_mem_size - left;
        pmem = pABSdbg->pdbg_pos + 1;
    }
    else
    {
        right = 0;
    }
    //ABS_PTK("** left = %d, right = %d, stlen addr = %d\n", left, right, strlen(pABSdbg->pdbg_addr));
    //ABS_PTK("addr = 0x%p, size = 0x%08X\n", pABSdbg->pdbg_addr, pABSdbg->dbg_mem_size);

    while(left >= 0)
    {
        //ABS_PTK("-- left = %d, right = %d, pmem = 0x%p\n", left, right, pmem);
        /* process the information head */
        /* mem is full */
        if(right > 0)
        {
            //ABS_PTK("R\n");
            pos = strstr(pmem, ">##");
            //ABS_PTK("R_p = 0x%p\n", pos);
            if(!pos)
            {
                //ABS_PTK("R not >##\n");
                if(*(pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - 1) == '>' &&
                   *pABSdbg->pdbg_addr == '#' && *(pABSdbg->pdbg_addr + 1) == '#')
                {
                    //ABS_PTK("> ##\n");
                    pos = pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - 1;
                }
                else if (*(pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - 2) == '>' &&
                    *(pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - 1) == '#' &&
                   *pABSdbg->pdbg_addr == '#')
                {
                    //ABS_PTK("># #\n");
                    pos = pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - 2;
                }
                else
                {
                    right = -1;
                    pmem = pABSdbg->pdbg_addr;
                    continue;
                }
            }
            else
            {
                pmem = pos;
            }

            HI_OSAL_Strncpy(u8FuncNumBuf, pos - ABS_DBG_INF_DATA_LEN, ABS_DBG_INF_DATA_WIDTH);
            u8DbgLevel = *(pos - ABS_DBG_INF_DATA_LEN + ABS_DBG_INF_DATA_WIDTH);
            HI_OSAL_Strncpy(u8StrLenBuf, pos - ABS_DBG_INF_DATA_LEN + ABS_DBG_INF_DATA_WIDTH + 1,
                                    ABS_DBG_INF_DATA_WIDTH);
        }
        else
        {
            //ABS_PTK("L\n");
            pos = strstr(pmem, ">##");
            if(!pos)
            {
                //ABS_PTK("Find Over.\n");
                HI_WARN_PRINT(pABSdbg->module_id, "Find Over.\n");
                ABS_DBG_LOCK(pABSdbg->dbg_lock);
                pABSdbg->dbg_rflag = HI_FALSE;
                ABS_DBG_UNLOCK(pABSdbg->dbg_lock);

                return -EPERM;
            }

            if((pos - pABSdbg->pdbg_addr >= 0) && (pos - pABSdbg->pdbg_addr < ABS_DBG_INF_DATA_LEN))
            {
                HI_U8 temp[ABS_DBG_INF_DATA_LEN + 1] = {0};

                HI_OSAL_Strncpy(temp, pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size -
                                    (ABS_DBG_INF_DATA_LEN - (pos - pABSdbg->pdbg_addr)),
                                    ABS_DBG_INF_DATA_LEN - (pos - pABSdbg->pdbg_addr));

                temp[ABS_DBG_INF_DATA_LEN - (pos - pABSdbg->pdbg_addr)] = '\0';
                HI_OSAL_Strncat(temp, pABSdbg->pdbg_addr, pos - pABSdbg->pdbg_addr);

                //temp[ABS_DBG_INF_DATA_LEN] = '\0';

                HI_OSAL_Strncpy(u8FuncNumBuf, temp, ABS_DBG_INF_DATA_WIDTH);
                u8DbgLevel = *(temp + ABS_DBG_INF_DATA_WIDTH);
                HI_OSAL_Strncpy(u8StrLenBuf, temp + ABS_DBG_INF_DATA_WIDTH + 1, ABS_DBG_INF_DATA_WIDTH);
                //ABS_PTK("temp :%s\n", temp);
            }
            else if(pos - pABSdbg->pdbg_addr >= 0)
            {
                HI_OSAL_Strncpy(u8FuncNumBuf, pos - ABS_DBG_INF_DATA_LEN, ABS_DBG_INF_DATA_WIDTH);
                u8DbgLevel = *(pos - ABS_DBG_INF_DATA_LEN + ABS_DBG_INF_DATA_WIDTH);
                HI_OSAL_Strncpy(u8StrLenBuf, pos - ABS_DBG_INF_DATA_LEN + ABS_DBG_INF_DATA_WIDTH + 1,
                                    ABS_DBG_INF_DATA_WIDTH);
                //ABS_PTK("pos :%s\n", pos);
            }
            else
            {
                HI_ERR_PRINT(pABSdbg->module_id, "wrong pos value 0x%08X, start 0x%08X\n", pos, pABSdbg->pdbg_addr);
                return -EPERM;
            }

            pmem = pos;
        }

        //ABS_PTK("\nno = %s \nlevel = %c \nlen = %s\n", u8FuncNumBuf, u8DbgLevel, u8StrLenBuf);

        u32FuncNum = simple_strtoul(u8FuncNumBuf, NULL, 10);
        u8DbgLevel = u8DbgLevel - '0';
        u32StrLen = simple_strtoul(u8StrLenBuf, NULL, 10);

        //ABS_PTK("no = %d, level = %d, len = %d\n\n", u32FuncNum, u8DbgLevel, len);

        if((u32FuncNum <= ABS_FUNC_15)
            && (u32StrLen > 0 && u32StrLen < ABS_DBG_MEM_IN_BUF_SIZE) 
            && (u8DbgLevel < ABS_DBG_END))
        {
            if( ((1 << u32FuncNum) & pABSdbg->dbg_mask) && (u8DbgLevel >= pABSdbg->dbg_level))
            {
                HI_U8 buf_out[ABS_DBG_MEM_IN_BUF_SIZE + ABS_DBG_INF_HEAD_SIZE + 8] = {0};

                switch (u8DbgLevel)
                {
                    case ABS_DBG_INFO:
                        HI_OSAL_Strncpy(buf_out, "INFO : ", 7);
                        break;
                    case ABS_DBG_WARN:
                        HI_OSAL_Strncpy(buf_out, "WARN : ", 7);
                        break;
                    case ABS_DBG_ERROR:
                        HI_OSAL_Strncpy(buf_out, "ERR  : ", 7);
                        break;
                    case ABS_DBG_FATAL:
                        HI_OSAL_Strncpy(buf_out, "FATAL: ", 7);
                        break;
            /* annotation for CodeCC, Because cannot reach the default case.*/                        
//                    default :
//                        ABS_ERR(pABSdbg->module_id, "error : Please set correct u8DbgLevel first.\n");
//                        ABS_DBG_LOCK(pABSdbg->dbg_lock);
//                        pABSdbg->dbg_rflag = HI_FALSE;
//                        ABS_DBG_UNLOCK(pABSdbg->dbg_lock);
//
//                        return -EPERM;
                }

                HI_OSAL_Strncat(buf_out, pFuncName[u32FuncNum], ABS_DBG_INF_DATA_LEN);

                if(pmem + ABS_DBG_INF_FUN_NO >= pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size)
                {                    
                    HI_OSAL_Strncat(buf_out, pmem + ABS_DBG_INF_FUN_NO - pABSdbg->dbg_mem_size, u32StrLen);
                }
                else if(pmem + ABS_DBG_INF_FUN_NO + u32StrLen >= pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size)
                {                    
                    HI_OSAL_Strncat(buf_out, pmem + ABS_DBG_INF_FUN_NO, pABSdbg->pdbg_addr +
                                        pABSdbg->dbg_mem_size - (pmem + ABS_DBG_INF_FUN_NO));

                    HI_OSAL_Strncat(buf_out, pABSdbg->pdbg_addr, u32StrLen + pmem + ABS_DBG_INF_FUN_NO -
                                                (pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size));
                }
                else 
                {                    
                    HI_OSAL_Strncat(buf_out, pmem + ABS_DBG_INF_FUN_NO, u32StrLen);
                }

                switch (pABSdbg->dbg_type)
                {

                    case DBG_NONE:
                        break;

                    case DBG_ALL:
                        ABS_PTK("%s", buf_out);
                        if(pABSdbg->pdbg_fp)
                        {
                            ret = HI_DRV_FILE_Write(pABSdbg->pdbg_fp, buf_out, strlen(buf_out));
                        }
                        break;

                    case DBG_SCREEN:
                        ABS_PTK("%s", buf_out);
                        break;

                    case DBG_FILE:
                        if(pABSdbg->pdbg_fp)
                        {
                            ret = HI_DRV_FILE_Write(pABSdbg->pdbg_fp, buf_out, strlen(buf_out));
                        }
                        break;

                    default :
                        ABS_ERR(pABSdbg->module_id, "error : Please set correct logtype first.\n");
                        ABS_DBG_LOCK(pABSdbg->dbg_lock);
                        pABSdbg->dbg_rflag = HI_FALSE;
                        ABS_DBG_UNLOCK(pABSdbg->dbg_lock);

                        return -EPERM;
                }
            }

            if(*(pmem += ABS_DBG_INF_FUN_NO + u32StrLen) == '\0')        /* jump over ">##......" */
            {
                //ABS_PTK("Come to the End ...\n");
                HI_WARN_PRINT(pABSdbg->module_id, "Come to the End ...\n");
                break;
            }

            pmem += ABS_DBG_INF_FUN_NO + ABS_DBG_INF_DATA_LEN;      /* jump over "##<000000000" */
            if(pmem > pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size)
            {
                pmem = pmem - pABSdbg->dbg_mem_size;
                left -= pmem - pABSdbg->pdbg_addr;
                right = -1;
            }
            else
            {
                if(right > 0)
                {
                    right = pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - pmem;
                }
                else
                {
                    left = pABSdbg->pdbg_pos - pmem;
                }
            }
        }
        else
        {
            pmem +=  ABS_DBG_INF_FUN_NO;    /* jump over ">##" */
            if(right > 0)
            {
                right -= ABS_DBG_INF_FUN_NO;
            }
            else
            {
                left -= ABS_DBG_INF_FUN_NO;
            }
        }
    }

    ABS_DBG_LOCK(pABSdbg->dbg_lock);
    pABSdbg->dbg_rflag = HI_FALSE;
    ABS_DBG_UNLOCK(pABSdbg->dbg_lock);

#else
    ABS_DBG_LOCK(pABSdbg->dbg_lock);

    //ABS_PTK("mem : \n%s", pABSdbg->pdbg_addr);
    /* NULL ASCII code is 000, and if the last byte of mem is 000, means the mem has not full */
    if(*(pABSdbg->pdbg_addr + pABSdbg->dbg_mem_size - 1))
    {
        memcpy(temp, pABSdbg->pdbg_pos + 1,
                pABSdbg->dbg_mem_size - (pABSdbg->pdbg_pos - pABSdbg->pdbg_addr + 1));

        memcpy(temp + pABSdbg->dbg_mem_size - (pABSdbg->pdbg_pos - pABSdbg->pdbg_addr + 1),
                            pABSdbg->pdbg_addr, pABSdbg->pdbg_pos - pABSdbg->pdbg_addr + 1);
    }
    else
    {
        memcpy(temp, pABSdbg->pdbg_addr, pABSdbg->pdbg_pos - pABSdbg->pdbg_addr);
        //ABS_PTK("pos(%p) - addr(%p) + 1= %d\n",pABSdbg->pdbg_pos, pABSdbg->pdbg_addr, pABSdbg->pdbg_pos - pABSdbg->pdbg_addr + 1);
    }


    ABS_DBG_UNLOCK(pABSdbg->dbg_lock);
    //ABS_PTK("temp : \n%s", temp);


    if(!(pos = strstr(temp, ">##")))
    {
        ABS_ERR(pABSdbg->module_id, "error : not find string : >##.\n");
        return -EPERM;
    }

    //ABS_PTK("temp = 0x%p, pos = 0x%p\n", temp, pos);

    while(pos < temp + ABS_BUF_DEBUG_LEN)
    {
        HI_OSAL_Strncat(u8FuncNumBuf, pos - 9, 4);
        HI_OSAL_Strncat(u8StrLenBuf, pos - 4, 4);
        u32FuncNum = simple_strtoul(u8FuncNumBuf, NULL, 10);
        u32StrLen = simple_strtoul(u8StrLenBuf, NULL, 10);
        memset(u8FuncNumBuf, 0, 5);
        memset(u8StrLenBuf, 0, 5);

        //ABS_PTK("** : %s\n", pos);

        //ABS_PTK("u32FuncNum = %d, u32StrLen = %d\n", u32FuncNum, u32StrLen);

        if(u32FuncNum >= 0 && u32FuncNum <= ABS_FUNC_15)
        {
            //ABS_PTK("u32FuncNum %d, mask 0x%x\n", u32FuncNum, pABSdbg->dbg_mask);
            if( (1 << u32FuncNum) & pABSdbg->dbg_mask)
            {
                HI_U8 buf_out[ABS_DBG_MEM_IN_BUF_SIZE + ABS_DBG_INF_HEAD_SIZE] = {0};

                memcpy(buf_out, pFuncName[u32FuncNum], ABS_DBG_INF_DATA_LEN);

                if(u32StrLen >= 0 && u32StrLen <= ABS_DBG_MEM_IN_BUF_SIZE)
                {
                    memcpy(buf_out + ABS_DBG_INF_DATA_LEN, pos + ABS_DBG_INF_FUN_NO, u32StrLen);

                    switch (pABSdbg->dbg_type)
                    {
                        case DBG_NONE:
                            break;

                        case DBG_ALL:
                            ABS_PTK("%s", buf_out);
                            if(pABSdbg->pdbg_fp)
                            {
                                ret = HI_DRV_FILE_Write(pABSdbg->pdbg_fp, buf_out, strlen(buf_out));
                            }
                            break;

                        case DBG_SCREEN:
                            ABS_PTK("%s", buf_out);
                            break;

                        case DBG_FILE:
                            if(pABSdbg->pdbg_fp)
                            {
                                ret = HI_DRV_FILE_Write(pABSdbg->pdbg_fp, buf_out, strlen(buf_out));
                            }
                            break;

                        default :
                            ABS_ERR(pABSdbg->module_id, "error : Please set logtype first.\n");
                            return -EPERM;
                    }
                }
            }
        }

        if(u32StrLen >= 0 && u32StrLen <= ABS_DBG_MEM_IN_BUF_SIZE)
        {
            pos += u32StrLen + ABS_DBG_INF_FUN_NO                             /* jump ">##" */
                        + ABS_DBG_INF_FUN_NO + ABS_DBG_INF_DATA_LEN;    /* jump "##<0000,0000" */
            if(*pos == '\0')
            {
                //ABS_PTK("end ...\n");
                break;
            }
        }
        else
        {
            pos = strstr(pos + 3, ">##");
            if(!pos)
                break;
        }
    }
#endif

    return ret;
}

HI_S32 ABS_DbgInit(ABS_DBG_S_PTR pABSdbg, HI_MOD_ID_E module_id)
{
    if(!pABSdbg)
    {
        ABS_PTK("ABS_debug_Iinit failed, ptr(0x%p) is error.\n", pABSdbg);
        return HI_FAILURE;
    }

    pABSdbg->dbg_mem_size = ABS_BUF_DEBUG_LEN;
    pABSdbg->pdbg_addr = (HI_U8 *)ABS_KMALLOC(module_id, pABSdbg->dbg_mem_size + 1);
    if (!pABSdbg->pdbg_addr)
    {
        ABS_ERR(pABSdbg->module_id, "ABS malloc memory space failed! addr(0x%p) is error\n", pABSdbg->pdbg_addr);
        pABSdbg->pdbg_addr = HI_NULL;
        return HI_FAILURE;
    }
    ABS_MEMSET(pABSdbg->pdbg_addr, 0, pABSdbg->dbg_mem_size);

    /* to prevent strstr() can't find '\0' to result kernel panic */
    pABSdbg->pdbg_addr[pABSdbg->dbg_mem_size] = '\0';

    ABS_INIT_LOCK(pABSdbg->dbg_lock);
    pABSdbg->pdbg_pos = pABSdbg->pdbg_addr;
    pABSdbg->module_id = module_id;
    pABSdbg->dbg_rflag = HI_FALSE;
    pABSdbg->pdbg_fp = HI_NULL;
    pABSdbg->dbg_level = ABS_DBG_INFO;

    return HI_SUCCESS;
}

/* free memory and resource*/
HI_VOID ABS_DbgDeInit(ABS_DBG_S_PTR pABSdbg)
{
    if(!pABSdbg)
    {
        ABS_PTK("ABS_debug_DeIinit failed, ptr(0x%p) is error.\n", pABSdbg);
        return ;
    }

    if(pABSdbg->pdbg_addr)
    {
        ABS_KFREE(pABSdbg->module_id, pABSdbg->pdbg_addr);
        pABSdbg->pdbg_addr = HI_NULL;
    }
}

