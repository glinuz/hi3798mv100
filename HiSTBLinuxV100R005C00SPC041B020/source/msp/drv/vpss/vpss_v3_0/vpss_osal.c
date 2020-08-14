#include "vpss_osal.h"
#include <linux/wait.h>

#ifdef HI_TEE_SUPPORT
#include "sec_mmz.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/************************************************************************/
/* file operation                                                       */
/************************************************************************/

struct file *VPSS_OSAL_fopen(const char *filename, int flags, int mode)
{
        struct file *filp = filp_open(filename, flags, mode);
        return (IS_ERR(filp)) ? NULL : filp;
}

void VPSS_OSAL_fclose(struct file *filp)
{
        if (filp)
            filp_close(filp, NULL);
}

int VPSS_OSAL_fread(char *buf, unsigned int len, struct file *filp)
{
        int readlen;
        mm_segment_t oldfs;

        if (filp == NULL)
                return -ENOENT;
        if (filp->f_op->read == NULL)
                return -ENOSYS;
        if (((filp->f_flags & O_ACCMODE) & (O_RDONLY | O_RDWR)) == 0)
                return -EACCES;
        oldfs = get_fs();
        set_fs(KERNEL_DS);
        readlen = filp->f_op->read(filp, buf, len, &filp->f_pos);
        set_fs(oldfs);

        return readlen;
}

int VPSS_OSAL_fwrite(char *buf, int len, struct file *filp)
{
        int writelen;
        mm_segment_t oldfs;

        if (filp == NULL)
                return -ENOENT;
        if (filp->f_op->write == NULL)
                return -ENOSYS;
        if (((filp->f_flags & O_ACCMODE) & (O_WRONLY | O_RDWR)) == 0)
                return -EACCES;
        oldfs = get_fs();
        set_fs(KERNEL_DS);
        writelen = filp->f_op->write(filp, buf, len, &filp->f_pos);
        set_fs(oldfs);

        return writelen;
}


/************************************************************************/
/* event operation                                                      */
/************************************************************************/
HI_S32 VPSS_OSAL_InitEvent( OSAL_EVENT *pEvent, HI_S32 InitVal1, HI_S32 InitVal2)
{
    pEvent->flag_1 = InitVal1;
    pEvent->flag_2 = InitVal2;
    init_waitqueue_head( &(pEvent->queue_head) );	
    return OSAL_OK;
}

HI_S32 VPSS_OSAL_GiveEvent( OSAL_EVENT *pEvent, HI_S32 InitVal1, HI_S32 InitVal2)
{
    pEvent->flag_1 = InitVal1;
    pEvent->flag_2 = InitVal2;
    
    wake_up(&(pEvent->queue_head));
	return OSAL_OK;
}

HI_S32 VPSS_OSAL_WaitEvent( OSAL_EVENT *pEvent, HI_S32 s32WaitTime )
{
	int l_ret;
    long unsigned int time;
    time = jiffies;
    l_ret = wait_event_timeout( pEvent->queue_head, 
                                (pEvent->flag_1 != 0 || pEvent->flag_2 != 0), 
                                s32WaitTime );
    if(l_ret == 0 
       || pEvent->flag_2 == 1
       || l_ret < 0)
    {
        return OSAL_ERR;
    }
    else
    {   
        return OSAL_OK;
    }
}

HI_S32 VPSS_OSAL_ResetEvent( OSAL_EVENT *pEvent, HI_S32 InitVal1, HI_S32 InitVal2)
{
    pEvent->flag_1 = InitVal1;
    pEvent->flag_2 = InitVal2;
    
    return OSAL_OK;
}


/************************************************************************/
/* mutux lock operation                                                 */
/************************************************************************/
HI_S32 VPSS_OSAL_InitLOCK(VPSS_OSAL_LOCK *pLock, HI_U32 u32InitVal)
{
    sema_init(pLock,u32InitVal);
    return HI_SUCCESS;
}

HI_S32 VPSS_OSAL_DownLock(VPSS_OSAL_LOCK *pLock)
{
    HI_S32 s32Ret;
    s32Ret = down_interruptible(pLock);

    if (s32Ret < 0) 
    {
		return HI_FAILURE;
	}
    else if (s32Ret == 0)
    {
        return HI_SUCCESS;
    }
    else
    {
        VPSS_FATAL("DownLock Error! ret = %d\n",s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_OSAL_TryLock(VPSS_OSAL_LOCK *pLock)
{
    HI_S32 s32Ret;
    s32Ret = down_trylock(pLock);
    if (s32Ret == 0)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 VPSS_OSAL_UpLock(VPSS_OSAL_LOCK *pLock)
{
    up(pLock);
    return HI_SUCCESS;
}



/************************************************************************/
/* spin lock operation                                                  */
/************************************************************************/
HI_S32 VPSS_OSAL_InitSpin(VPSS_OSAL_SPIN *pLock)
{
    spin_lock_init(pLock);
	return HI_SUCCESS;
}

HI_S32 VPSS_OSAL_DownSpin(VPSS_OSAL_SPIN *pLock,unsigned long *flags)
{
    spin_lock_irqsave(pLock, *flags);
    
    return HI_SUCCESS;
}

HI_S32 VPSS_OSAL_UpSpin(VPSS_OSAL_SPIN *pLock,unsigned long *flags)
{
    spin_unlock_irqrestore(pLock, *flags);

    return HI_SUCCESS;
}

HI_S32 VPSS_OSAL_TryLockSpin(VPSS_OSAL_SPIN *pLock,unsigned long *flags)
{
   if(spin_trylock_irqsave(pLock, *flags))
   {
        return HI_SUCCESS;
   }
   else
   {
        return HI_FAILURE;
   }
}

/************************************************************************/
/* debug operation                                                      */
/************************************************************************/
HI_S32 VPSS_OSAL_GetProcArg(HI_CHAR*  chCmd,HI_CHAR*  chArg,HI_U32 u32ArgIdx)
{
    HI_U32 u32Count;
    HI_U32 u32CmdCount;
    HI_U32 u32LogCount;
    HI_U32 u32NewFlag;
    HI_CHAR chArg1[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR chArg2[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR chArg3[DEF_FILE_NAMELENGTH] = {0};
    u32CmdCount = 0;

    /*clear empty space*/
    u32Count = 0;
    u32CmdCount = 0;
    u32LogCount = 1;
    u32NewFlag = 0;
    while(chCmd[u32Count] != 0 && chCmd[u32Count] != '\n' )
    {
        if (chCmd[u32Count] != ' ')
        {
            u32NewFlag = 1;
        }
        else
        {
            if(u32NewFlag == 1)
            {
                u32LogCount++;
                u32CmdCount= 0;
                u32NewFlag = 0;
            }
        }
        
        if (u32NewFlag == 1)
        {
            switch(u32LogCount)
            {
                case 1:
                    chArg1[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                case 2:
                    chArg2[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                case 3:
                    chArg3[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                default:
                    break;
            }
            
        }
        u32Count++;
    }
    
    switch(u32ArgIdx)
    {
        case 1:
            memcpy(chArg,chArg1,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);
            break;
        case 2:
            memcpy(chArg,chArg2,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);
            break;
        case 3:
            memcpy(chArg,chArg3,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}


HI_S32 VPSS_OSAL_ParseCmd(HI_CHAR*  chArg1,HI_CHAR*  chArg2,HI_CHAR*  chArg3,HI_VOID *pstCmd)
{
    return HI_SUCCESS;
}

HI_S32 VPSS_OSAL_TransforUV10bitTobit(HI_U8 *pu10UVdata,HI_U8 *pu8Udata,HI_U8 *pu8Vdata,HI_U32 u32Stride,HI_U32 u32Width,HI_DRV_PIX_FORMAT_E eFormat)
{
    HI_U8 *pTmp;
    HI_U32 i,j,tmpU,tmpV;
    pTmp = VPSS_VMALLOC(u32Stride*8);
    if (pTmp == HI_NULL)
    {
        return HI_FAILURE;
    }
    for(i = 0; i < u32Stride; i++)
    {
        for(j=0; j < 8; j++)
        {
            pTmp[i*8 +j] = (pu10UVdata[i]>>j) & 0x1;
        }         
    }
    for(i = 0; i < u32Width/2; i = i+2) 
    {
        tmpU = 0;
        for(j=0; j < 8; j++)
        {
            tmpU |= pTmp[i*10+j+2]<<j;              
        }
        tmpV = 0;
        for(j=0; j < 8; j++)
        {
            tmpV |= pTmp[(i+1)*10+j+2]<<j;              
        }  
        if((eFormat == HI_DRV_PIX_FMT_NV21) || (eFormat == HI_DRV_PIX_FMT_NV61_2X1) )
        {
            pu8Vdata[i] = tmpU;
            pu8Udata[i] = tmpV;
        }
        else
        {
            pu8Udata[i] = tmpU;
            pu8Vdata[i] = tmpV;
        }
    }    
    VPSS_VFREE(pTmp);
    return HI_SUCCESS;
}
HI_S32 VPSS_OSAL_Transfor10bitTobit(HI_U8 *pu10Ydata,HI_U8 *pu8Ydata,HI_U32 u32Stride,HI_U32 u32Width)
{
    HI_U8 *pTmp;
    HI_U32 i,j,tmp;
    pTmp = VPSS_VMALLOC(u32Stride*8);
    if (pTmp == HI_NULL)
    {
        return HI_FAILURE;
    }    
    for(i = 0; i < u32Stride; i++)
    {
        for(j=0; j < 8; j++)
        {
            pTmp[i*8 +j] = (pu10Ydata[i]>>j) & 0x1;
        }         
    }
    for(i = 0; i < u32Width; i++) 
    {
        tmp = 0;
        for(j=0; j < 8; j++)
        {
            tmp |= pTmp[i*10+j+2]<<j;              
        } 
        pu8Ydata[i] = tmp;
    }
    VPSS_VFREE(pTmp);
    return HI_SUCCESS;
}

HI_U8  temp[4096]={0};
static inline HI_VOID VPSS_OSAL_OneLine10To8Bit(int enType, HI_CHAR *pInAddr, HI_U32 u32Width, HI_CHAR *pOutAddr)
{
    HI_U32 i, j, u32Cnt;
    HI_CHAR  *pTmpMem = NULL;

    pTmpMem = pInAddr;
    u32Cnt = HICEILING(u32Width, 4);//四个像素一循环，占5 byte,不足四个像素也占5byte处理    
    for (i = 0; i < u32Cnt; i++) 
    {   
        for(j = 0; j < 5; j++)
        {
             temp[j] = *(pTmpMem + i*5 + j);
        }

        if (0 == enType)
        {
            *pOutAddr = ((temp[1] << 6) & 0xc0) | ((temp[0] >> 2)& 0x3f);
            pOutAddr++;
            *pOutAddr = ((temp[2] << 4) & 0xf0) | ((temp[1] >> 4)& 0x0f);
            pOutAddr++;
            *pOutAddr = ((temp[3] << 2) & 0xfc) | ((temp[2] >> 6)& 0x03);
            pOutAddr++;
            *pOutAddr = temp[4] & 0xff;
            pOutAddr++;

        }
        else if (1 == enType)
        {
            *pOutAddr = ((temp[2] << 4) & 0xf0) | ((temp[1] >> 4)& 0x0f);
			pOutAddr++;
			*pOutAddr = temp[4] & 0xff;
			pOutAddr++;
        }
        else
        {
            *pOutAddr = ((temp[1] << 6) & 0xc0) | ((temp[0] >> 2)& 0x3f);
			pOutAddr++;
			*pOutAddr = ((temp[3] << 2) & 0xfc) | ((temp[2] >> 6)& 0x03);
			pOutAddr++;
        }
    }

}

HI_S32 VPSS_OSAL_StrToNumb(HI_CHAR*  chStr,HI_U32 *pu32Numb)
{
    HI_U32 u32Count = 0;
    HI_U32 u32RetNumb = 0;
    
    while(chStr[u32Count] != 0)
    {
        u32RetNumb = u32RetNumb*10 + chStr[u32Count] - '0';
        u32Count++;
    }

    *pu32Numb = u32RetNumb;

    return HI_SUCCESS;
}
HI_S32 VPSS_OSAL_WRITEYUV_10BIT(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pchFile)
{
    char str[50] = {0};
    unsigned char *ptr;
    FILE *fp;
    HI_U8 *pu8Udata;
    HI_U8 *pu8Vdata;
    HI_U8 *pu8Ydata;
    HI_U8 *pu10UVdata;
    HI_U8 *pu10Ydata;    
    HI_S8  s_VpssSavePath[DEF_FILE_NAMELENGTH];
    HI_U32 i;
    HI_DRV_LOG_GetStorePath(s_VpssSavePath, DEF_FILE_NAMELENGTH);
    HI_OSAL_Snprintf(str, 50, "%s/%s", s_VpssSavePath,pchFile);

    if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21
        || pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12)
    {
        pu8Udata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 /2);
        if (pu8Udata == HI_NULL)
        {
            return HI_FAILURE;
        }
        
        pu10UVdata = VPSS_VMALLOC(pstFrame->stBufAddr[0].u32Stride_C);        
        if (pu10UVdata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            return HI_FAILURE;
        }   
        
        pu8Vdata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 /2);
        if (pu8Vdata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            VPSS_VFREE(pu10UVdata);            
            return HI_FAILURE;
        }
        pu8Ydata = VPSS_VMALLOC(pstFrame->u32Width);
        if (pu8Ydata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            VPSS_VFREE(pu8Vdata);
            VPSS_VFREE(pu10UVdata);            
            return HI_FAILURE;
        }        
        pu10Ydata = VPSS_VMALLOC(pstFrame->stBufAddr[0].u32Stride_Y);        
        if (pu10Ydata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            VPSS_VFREE(pu8Vdata);
            VPSS_VFREE(pu10UVdata); 
            VPSS_VFREE(pu8Ydata); 
            return HI_FAILURE;
        }
        ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y);
        if (!ptr)
        {
            VPSS_FATAL("address is not valid!\n");
        }
        else
        {   
            fp = VPSS_OSAL_fopen(str, O_RDWR | O_CREAT|O_APPEND, 0);
            if (fp == HI_NULL)
            {
                VPSS_FATAL("open file '%s' fail!\n", str);
                VPSS_VFREE(pu8Udata);
                VPSS_VFREE(pu8Vdata);
                VPSS_VFREE(pu8Ydata);
                VPSS_VFREE(pu10UVdata);
                VPSS_VFREE(pu10Ydata);                
                return HI_FAILURE;
            }
            for (i=0; i<pstFrame->u32Height; i++)
            {
                memcpy(pu10Ydata,ptr,sizeof(HI_U8)*pstFrame->stBufAddr[0].u32Stride_Y); 
                VPSS_OSAL_Transfor10bitTobit(pu10Ydata,pu8Ydata,pstFrame->stBufAddr[0].u32Stride_Y,pstFrame->u32Width);
                if(pstFrame->u32Width != VPSS_OSAL_fwrite(pu8Ydata,pstFrame->u32Width, fp))
                {
                    VPSS_FATAL("line %d: fwrite fail!\n",__LINE__);
                }
                ptr += pstFrame->stBufAddr[0].u32Stride_Y;
            }
            ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_C);
            for (i=0; i<pstFrame->u32Height/2; i++)
            {
                memcpy(pu10UVdata,ptr,pstFrame->stBufAddr[0].u32Stride_C);

                VPSS_OSAL_OneLine10To8Bit(1,pu10UVdata,pstFrame->u32Width, pu8Udata+i*pstFrame->u32Width/2);
                
                VPSS_OSAL_OneLine10To8Bit(2,pu10UVdata,pstFrame->u32Width, pu8Vdata+i*pstFrame->u32Width/2);
                
                //VPSS_OSAL_TransforUV10bitTobit(pu10UVdata,&pu8Udata[i*pstFrame->u32Width/2],&pu8Vdata[i*pstFrame->u32Width/2],
                //    pstFrame->stBufAddr[0].u32Stride_C,pstFrame->u32Width,pstFrame->ePixFormat);
                ptr += pstFrame->stBufAddr[0].u32Stride_C;
            }
            VPSS_OSAL_fwrite(pu8Udata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, fp);
            VPSS_OSAL_fwrite(pu8Vdata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, fp);
            VPSS_OSAL_fclose(fp);
            VPSS_FATAL("2d image has been saved to '%s' W=%d H=%d Format=%d \n", 
                        str,pstFrame->u32Width,pstFrame->u32Height,pstFrame->ePixFormat);
        }
        VPSS_VFREE(pu8Udata);
        VPSS_VFREE(pu8Vdata);
        VPSS_VFREE(pu8Ydata);
        VPSS_VFREE(pu10UVdata);
        VPSS_VFREE(pu10Ydata);        
    }       
    else if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1
            || pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV16_2X1)
    {
        pu8Udata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 );
        if (pu8Udata == HI_NULL)
        {
            return HI_FAILURE;
        } 
        
        pu10UVdata = VPSS_VMALLOC(pstFrame->stBufAddr[0].u32Stride_C); 
        if (pu10UVdata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);            
            return HI_FAILURE;
        }

        pu8Vdata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 );
        if (pu8Vdata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            VPSS_VFREE(pu10UVdata);            
            return HI_FAILURE;
        }
        pu8Ydata = VPSS_VMALLOC(pstFrame->u32Width);
        if (pu8Ydata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            VPSS_VFREE(pu8Vdata);
            VPSS_VFREE(pu10UVdata);            
            return HI_FAILURE;
        }        
        pu10Ydata = VPSS_VMALLOC(pstFrame->stBufAddr[0].u32Stride_Y);        
        if (pu10Ydata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            VPSS_VFREE(pu8Vdata);
            VPSS_VFREE(pu10UVdata); 
            VPSS_VFREE(pu8Ydata); 
            return HI_FAILURE;
        }
        ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y);
        if (!ptr)
        {
            VPSS_FATAL("address is not valid!\n");
        }
        else
        {   
            fp = VPSS_OSAL_fopen(str, O_RDWR | O_CREAT|O_APPEND, 0);
            if (fp == HI_NULL)
            {
                VPSS_FATAL("open file '%s' fail!\n", str);
                VPSS_VFREE(pu8Udata);
                VPSS_VFREE(pu8Vdata);
                VPSS_VFREE(pu8Ydata);
                VPSS_VFREE(pu10UVdata);
                VPSS_VFREE(pu10Ydata);                  
                return HI_FAILURE;
            }
            for (i=0; i<pstFrame->u32Height; i++)
            {
                memcpy(pu10Ydata,ptr,sizeof(HI_U8)*pstFrame->stBufAddr[0].u32Stride_Y); 
                VPSS_OSAL_Transfor10bitTobit(pu10Ydata,pu8Ydata,pstFrame->stBufAddr[0].u32Stride_Y,pstFrame->u32Width);
                if(pstFrame->u32Width != VPSS_OSAL_fwrite(pu8Ydata,pstFrame->u32Width, fp))
                {
                    VPSS_FATAL("line %d: fwrite fail!\n",__LINE__);
                }
                ptr += pstFrame->stBufAddr[0].u32Stride_Y;
            }
            
            ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_C);
            for (i=0; i<pstFrame->u32Height; i++)
            {
                memcpy(pu10UVdata,ptr,pstFrame->stBufAddr[0].u32Stride_C); 

                VPSS_OSAL_OneLine10To8Bit(1,pu10UVdata,pstFrame->u32Width, pu8Udata+i*pstFrame->u32Width/2);
                
                VPSS_OSAL_OneLine10To8Bit(2,pu10UVdata,pstFrame->u32Width, pu8Vdata+i*pstFrame->u32Width/2);

//                VPSS_OSAL_TransforUV10bitTobit(pu10UVdata,&pu8Udata[i*pstFrame->u32Width/2],&pu8Vdata[i*pstFrame->u32Width/2],
   //                 pstFrame->stBufAddr[0].u32Stride_C,pstFrame->u32Width,pstFrame->ePixFormat);
                ptr += pstFrame->stBufAddr[0].u32Stride_C;
            }
            VPSS_OSAL_fwrite(pu8Udata, pstFrame->u32Width * pstFrame->u32Height  /2, fp);
            VPSS_OSAL_fwrite(pu8Vdata, pstFrame->u32Width * pstFrame->u32Height  /2, fp);
            VPSS_OSAL_fclose(fp);
            VPSS_FATAL("2d image has been saved to '%s' W=%d H=%d Format=%d \n", 
                        str,pstFrame->u32Width,pstFrame->u32Height,pstFrame->ePixFormat);
        }
        VPSS_VFREE(pu8Udata);
        VPSS_VFREE(pu8Vdata);
        VPSS_VFREE(pu8Ydata);
        VPSS_VFREE(pu10UVdata);
        VPSS_VFREE(pu10Ydata);          
    }
	else if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP
				|| pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12_CMP)
	{
		HI_U8 u8YhFileName[50] = "/mnt/cmp_y.head";
		HI_U8 u8YdFileName[50] = "/mnt/cmp_y.dat";
		HI_U8 u8ChFileName[50] = "/mnt/cmp_c.head";
		HI_U8 u8CdFileName[50] = "/mnt/cmp_c.dat";
		FILE *pfYh; 
		FILE *pfYd; 
		FILE *pfCh; 
		FILE *pfCd; 
		HI_U32 u32Y_Stride;
		HI_U32 u32C_Stride;
		HI_U32 u32W;
		HI_U32 u32H;
		HI_U8 *pu8Y_VAddr;
		HI_U8 *pu8C_VAddr;
		HI_U32 u32HeadStride;
		HI_BOOL b8BitWidth;
		HI_U32 u32HeadLenth = 0;
		HI_U8 pu8TmpData[4]={0};
		HI_U8 u8TmpZero = 0;
		HI_U32 i,j;

		u32W = pstFrame->u32Width;
		u32H = pstFrame->u32Height;

		u32Y_Stride = pstFrame->stBufAddr[0].u32Stride_Y;
		u32C_Stride = pstFrame->stBufAddr[0].u32Stride_C;

		pu8Y_VAddr = phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y);
		pu8C_VAddr = phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_C);

		b8BitWidth = (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_8BIT)?0:1;

		u32HeadStride = (u32W*((b8BitWidth == 0)?8:10))*2/128*16;	


		pfYh = VPSS_OSAL_fopen(u8YhFileName, O_RDWR | O_CREAT|O_APPEND, 0644);
		pfYd = VPSS_OSAL_fopen(u8YdFileName, O_RDWR | O_CREAT|O_APPEND, 0644);
		pfCh = VPSS_OSAL_fopen(u8ChFileName, O_RDWR | O_CREAT|O_APPEND, 0644);
		pfCd = VPSS_OSAL_fopen(u8CdFileName, O_RDWR | O_CREAT|O_APPEND, 0644);

		for(j=0; j<u32H; j++, pu8Y_VAddr += u32Y_Stride)
		{
			memcpy(pu8TmpData, pu8Y_VAddr, 2);
			memcpy(&u32HeadLenth, pu8Y_VAddr, 2);

			if(4 !=VPSS_OSAL_fwrite(pu8TmpData, 4,pfYh))
			{
				VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); 
				break;
			}

			HI_ASSERT(u32HeadLenth*16<=u32Y_Stride);

			if(u32HeadLenth*16 != VPSS_OSAL_fwrite(pu8Y_VAddr,u32HeadLenth*16, pfYd))
			{
				VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); 
				break;
			}

			if(u32HeadStride>u32HeadLenth*16)
			{
				for(i=0; i<(u32HeadStride-u32HeadLenth*16); i++)
				{
					if(1 !=VPSS_OSAL_fwrite(&u8TmpZero, 1, pfYd))
					{
						VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); break;}
				}
			}
		}

		for(j=0; j<u32H/2; j++, pu8C_VAddr+=u32C_Stride)
		{
			memcpy(pu8TmpData,pu8C_VAddr, 2);

			memcpy(&u32HeadLenth,pu8C_VAddr, 2);

			if(4 !=VPSS_OSAL_fwrite(pu8TmpData, 4,pfCh))
			{
				VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__);
				break;
			}

			HI_ASSERT(u32HeadLenth*16<=u32C_Stride);

			if(u32HeadLenth*16 !=VPSS_OSAL_fwrite(pu8C_VAddr,u32HeadLenth*16,pfCd))
			{
				VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); 
				break;
			}

			if(u32HeadStride>u32HeadLenth*16)
			{
				for(i=0; i<(u32HeadStride-u32HeadLenth*16); i++)
				{
					if(1 !=VPSS_OSAL_fwrite(&u8TmpZero, 1,pfCd))
					{
						VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); 
						break;
					}
				}
			}
		}


	}
    else
    {
        VPSS_FATAL("PixFormat %d can't saveyuv\n",pstFrame->ePixFormat);
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_OSAL_WRITEYUV_8BIT(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pchFile)
{
	char str[50] = {0};
	unsigned char *ptr;
	FILE *fp;
    HI_U8 *pu8Udata;
    HI_U8 *pu8Vdata;
    HI_U8 *pu8Ydata;
    HI_S8  s_VpssSavePath[DEF_FILE_NAMELENGTH];
    HI_U32 i,j;
    
    HI_DRV_LOG_GetStorePath(s_VpssSavePath, DEF_FILE_NAMELENGTH);

    HI_OSAL_Snprintf(str, 50, "%s/%s", s_VpssSavePath,pchFile);

    if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21
        || pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12)
    {
        pu8Udata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 /2);
        if (pu8Udata == HI_NULL)
        {
            return HI_FAILURE;
        }
        pu8Vdata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 /2);
        if (pu8Vdata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            return HI_FAILURE;
        }
        pu8Ydata = VPSS_VMALLOC(pstFrame->stBufAddr[0].u32Stride_Y);
        if (pu8Ydata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            VPSS_VFREE(pu8Vdata);
            return HI_FAILURE;
        }
        
        ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y);
        
		if (!ptr)
		{
            VPSS_FATAL("address is not valid!\n");
		}
		else
		{   
            fp = VPSS_OSAL_fopen(str, O_RDWR | O_CREAT|O_APPEND, 0644);

            if (fp == HI_NULL)
            {
                VPSS_FATAL("open file '%s' fail!\n", str);
                VPSS_VFREE(pu8Udata);
                VPSS_VFREE(pu8Vdata);
                VPSS_VFREE(pu8Ydata);
                return HI_FAILURE;
            }
            
            /*write Y data*/
            for (i=0; i<pstFrame->u32Height; i++)
            {
                memcpy(pu8Ydata,ptr,sizeof(HI_U8)*pstFrame->stBufAddr[0].u32Stride_Y);

                if(pstFrame->u32Width != VPSS_OSAL_fwrite(pu8Ydata,pstFrame->u32Width, fp))
				{
                    VPSS_FATAL("line %d: fwrite fail!\n",__LINE__);
                }
                ptr += pstFrame->stBufAddr[0].u32Stride_Y;
            }
            
            ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_C);
            /*write UV data */
            for (i=0; i<pstFrame->u32Height/2; i++)
            {
                for (j=0; j<pstFrame->u32Width/2; j++)
                {
                    if(pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
                    {
                        pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                        pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
                    }
                    else
                    {
                        pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                        pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
                    }
                }
                ptr += pstFrame->stBufAddr[0].u32Stride_C;
            }
            VPSS_OSAL_fwrite(pu8Udata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, fp);

            VPSS_OSAL_fwrite(pu8Vdata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, fp);
            

            VPSS_OSAL_fclose(fp);
            VPSS_FATAL("2d image has been saved to '%s' W=%d H=%d Format=%d \n", 
                        str,pstFrame->u32Width,pstFrame->u32Height,pstFrame->ePixFormat);

            
		}
        VPSS_VFREE(pu8Udata);
        VPSS_VFREE(pu8Vdata);
        VPSS_VFREE(pu8Ydata);
    }
    else if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE
            || pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12_TILE)
    {
        HI_U8 *s8YuvArray;
        HI_U8 *s8UArray;
        HI_U8 *s8VArray;
        HI_U8 *dst;
        HI_U8 *src;
        HI_U8 *tmp;
        HI_U32 Stride;
        HI_U8 *Caddress ;	
		
        s8YuvArray = VPSS_VMALLOC(((pstFrame->u32Width+16)&0xfffffff0) * pstFrame->u32Height);
        if (HI_NULL == s8YuvArray)
        {
			return HI_FAILURE;
        }
		
		s8UArray = VPSS_VMALLOC(((pstFrame->u32Width+16)&0xfffffff0) * pstFrame->u32Height / 2);
        if (HI_NULL == s8UArray)
        {
            VPSS_VFREE(s8YuvArray);
			return HI_FAILURE;
        }
		
		s8VArray = VPSS_VMALLOC(((pstFrame->u32Width+16)&0xfffffff0) * pstFrame->u32Height / 2);
        if (HI_NULL == s8VArray)
        {
            VPSS_VFREE(s8YuvArray);
            VPSS_VFREE(s8UArray);
			return HI_FAILURE;
        }

        fp = VPSS_OSAL_fopen(str, O_RDWR | O_CREAT|O_APPEND, 0644);
        if (fp == HI_NULL)
        {
            VPSS_FATAL("open file '%s' fail!\n", str);
            VPSS_VFREE(s8YuvArray);
            VPSS_VFREE(s8UArray);
            VPSS_VFREE(s8VArray);
            return HI_FAILURE;
        }
		
        Stride = ((pstFrame->u32Width + (64*4 -1))&(~(64*4 -1)))*16;
        for(i=0;i<pstFrame->u32Height;i++)
        {
            for(j=0;j<pstFrame->u32Width;j+=256)
            {
                dst  = (unsigned char*)(s8YuvArray+ pstFrame->u32Width*i + j);
                src =  (unsigned char*)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y) + Stride*(i/16)+(i%16)*256 + (j/256)*256*16;
				memcpy(dst,src,256);
            }
        }
		
        VPSS_OSAL_fwrite(s8YuvArray,pstFrame->u32Width*pstFrame->u32Height, fp);        
         
        Caddress = phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y) + ((pstFrame->u32Width + (64*4 -1))&(~(64*4 -1)))*((pstFrame->u32Height + 31)/32)*32
                + ((pstFrame->u32Width  + 127) / 128 + 15) / 16 * 16*((pstFrame->u32Height + 31)/32)*32;
        
        for(i=0;i<pstFrame->u32Height/2;i++)
        {
            for(j=0;j<pstFrame->u32Width;j+=256)
            {
               dst  = (unsigned char*)(s8YuvArray + pstFrame->u32Width*i + j);
               src =  (unsigned char*)Caddress + (Stride/2)*(i/8)+(i%8)*256 +  (j/256)*256*8;
			   memcpy(dst,src,256);
            }
        }
		
        tmp = s8YuvArray;
        for (i=0;i<pstFrame->u32Height/2;i++)
        {
            for (j=0;j<pstFrame->u32Width/2;j++)
            {
                s8VArray[i*pstFrame->u32Width/2+j] = tmp[2*j];
                s8UArray[i*pstFrame->u32Width/2+j] = tmp[2*j+1];
            }
            tmp+= pstFrame->u32Width;
        }
		
        VPSS_OSAL_fwrite(s8UArray,pstFrame->u32Width*pstFrame->u32Height/4, fp);     
        VPSS_OSAL_fwrite(s8VArray,pstFrame->u32Width*pstFrame->u32Height/4, fp);      
       
		VPSS_OSAL_fclose(fp);
        VPSS_FATAL("Tile image has been saved to '%s' W=%d H=%d Format=%d \n", 
                    str,pstFrame->u32Width,pstFrame->u32Height,pstFrame->ePixFormat);
        VPSS_VFREE(s8YuvArray);
        VPSS_VFREE(s8UArray);
        VPSS_VFREE(s8VArray);
	}   
	else if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1
            || pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV16_2X1)
    {
        pu8Udata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 );
        if (pu8Udata == HI_NULL)
        {
            return HI_FAILURE;
        }
        pu8Vdata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 );
        if (pu8Vdata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            return HI_FAILURE;
        }
        pu8Ydata = VPSS_VMALLOC(pstFrame->stBufAddr[0].u32Stride_Y);
        if (pu8Ydata == HI_NULL)
        {
            VPSS_VFREE(pu8Udata);
            VPSS_VFREE(pu8Vdata);
            return HI_FAILURE;
        }
        
        ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y);
        
		if (!ptr)
		{
            VPSS_FATAL("address is not valid!\n");
		}
		else
		{   

            fp = VPSS_OSAL_fopen(str, O_RDWR | O_CREAT|O_APPEND, 0644);

            if (fp == HI_NULL)
            {
                VPSS_FATAL("open file '%s' fail!\n", str);
                VPSS_VFREE(pu8Udata);
                VPSS_VFREE(pu8Vdata);
                VPSS_VFREE(pu8Ydata);
                return HI_FAILURE;
            }
            
            /*write Y data*/
            for (i=0; i<pstFrame->u32Height; i++)
            {
                memcpy(pu8Ydata,ptr,sizeof(HI_U8)*pstFrame->stBufAddr[0].u32Stride_Y);

                if(pstFrame->u32Width != VPSS_OSAL_fwrite(pu8Ydata,pstFrame->u32Width, fp))
				{
                    VPSS_FATAL("line %d: fwrite fail!\n",__LINE__);
                }
                ptr += pstFrame->stBufAddr[0].u32Stride_Y;
            }
            
            ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_C);
            /*write UV data */
            for (i=0; i<pstFrame->u32Height; i++)
            {
                for (j=0; j<pstFrame->u32Width/2; j++)
                {
                    if(pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1)
                    {
                        pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                        pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
                    }
                    else
                    {
                        pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                        pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
                    }
                }
                ptr += pstFrame->stBufAddr[0].u32Stride_C;
            }
            VPSS_OSAL_fwrite(pu8Udata, pstFrame->u32Width * pstFrame->u32Height  /2, fp);

            VPSS_OSAL_fwrite(pu8Vdata, pstFrame->u32Width * pstFrame->u32Height  /2, fp);
            

            VPSS_OSAL_fclose(fp);
            VPSS_FATAL("2d image has been saved to '%s' W=%d H=%d Format=%d \n", 
                        str,pstFrame->u32Width,pstFrame->u32Height,pstFrame->ePixFormat);
            
		}
        VPSS_VFREE(pu8Udata);
        VPSS_VFREE(pu8Vdata);
        VPSS_VFREE(pu8Ydata);
    }
	else if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP
			|| pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12_CMP)
	{
		HI_U8 u8YhFileName[50] = "/mnt/cmp_y.head";
		HI_U8 u8YdFileName[50] = "/mnt/cmp_y.dat";
		HI_U8 u8ChFileName[50] = "/mnt/cmp_c.head";
		HI_U8 u8CdFileName[50] = "/mnt/cmp_c.dat";
		FILE *pfYh; 
		FILE *pfYd; 
		FILE *pfCh; 
		FILE *pfCd; 
		HI_U32 u32Y_Stride;
		HI_U32 u32C_Stride;
		HI_U32 u32W;
		HI_U32 u32H;
		HI_U8 *pu8Y_VAddr;
		HI_U8 *pu8C_VAddr;
		HI_U32 u32HeadStride;
		HI_BOOL b8BitWidth;
		HI_U32 u32HeadLenth = 0;
		HI_U8 pu8TmpData[4]={0};
		HI_U8 u8TmpZero = 0;

		u32W = pstFrame->u32Width;
		u32H = pstFrame->u32Height;

		u32Y_Stride = pstFrame->stBufAddr[0].u32Stride_Y;
		u32C_Stride = pstFrame->stBufAddr[0].u32Stride_C;

		pu8Y_VAddr = phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y);
		pu8C_VAddr = phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_C);

		b8BitWidth = (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_8BIT)?0:1;

		u32HeadStride = (u32W*((b8BitWidth == 0)?8:10))*2/128*16;	


		pfYh = VPSS_OSAL_fopen(u8YhFileName, O_RDWR | O_CREAT|O_APPEND, 0644);
		pfYd = VPSS_OSAL_fopen(u8YdFileName, O_RDWR | O_CREAT|O_APPEND, 0644);
		pfCh = VPSS_OSAL_fopen(u8ChFileName, O_RDWR | O_CREAT|O_APPEND, 0644);
		pfCd = VPSS_OSAL_fopen(u8CdFileName, O_RDWR | O_CREAT|O_APPEND, 0644);

		for(j=0; j<u32H; j++, pu8Y_VAddr += u32Y_Stride)
		{
			memcpy(pu8TmpData, pu8Y_VAddr, 2);
			memcpy(&u32HeadLenth, pu8Y_VAddr, 2);

			if(4 !=VPSS_OSAL_fwrite(pu8TmpData, 4,pfYh))
			{
				VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); 
				break;
			}

			HI_ASSERT(u32HeadLenth*16<=u32Y_Stride);

			if(u32HeadLenth*16 != VPSS_OSAL_fwrite(pu8Y_VAddr,u32HeadLenth*16, pfYd))
			{
				VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); 
				break;
			}

			if(u32HeadStride>u32HeadLenth*16)
			{
				for(i=0; i<(u32HeadStride-u32HeadLenth*16); i++)
				{
					if(1 !=VPSS_OSAL_fwrite(&u8TmpZero, 1, pfYd))
					{
						VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); break;}
				}
			}
		}

		for(j=0; j<u32H/2; j++, pu8C_VAddr+=u32C_Stride)
		{
			memcpy(pu8TmpData,pu8C_VAddr, 2);

			memcpy(&u32HeadLenth,pu8C_VAddr, 2);

			if(4 !=VPSS_OSAL_fwrite(pu8TmpData, 4,pfCh))
			{
				VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__);
				break;
			}

			HI_ASSERT(u32HeadLenth*16<=u32C_Stride);

			if(u32HeadLenth*16 !=VPSS_OSAL_fwrite(pu8C_VAddr,u32HeadLenth*16,pfCd))
			{
				VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); 
				break;
			}

			if(u32HeadStride>u32HeadLenth*16)
			{
				for(i=0; i<(u32HeadStride-u32HeadLenth*16); i++)
				{
					if(1 !=VPSS_OSAL_fwrite(&u8TmpZero, 1,pfCd))
					{
						VPSS_ERROR("error:write %d, %s\n", __LINE__,__FILE__); 
						break;
					}
				}
			}
		}


	}
    else
    {
        VPSS_FATAL("PixFormat %d can't saveyuv\n",pstFrame->ePixFormat);
    }
    
    return HI_SUCCESS;
}
HI_S32 VPSS_OSAL_WRITEYUV(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pchFile)
{		
    if(HI_DRV_PIXEL_BITWIDTH_8BIT == pstFrame->enBitWidth)
    {
        return VPSS_OSAL_WRITEYUV_8BIT(pstFrame,pchFile);
    }
    else
    if(HI_DRV_PIXEL_BITWIDTH_10BIT == pstFrame->enBitWidth)
    {
        return VPSS_OSAL_WRITEYUV_10BIT(pstFrame,pchFile);
    }
    else
    {
        return HI_FAILURE;
    }
}
HI_S32 VPSS_OSAL_CalBufSize(HI_U32 *pSize,HI_U32 *pStride,HI_U32 u32Height,HI_U32 u32Width,HI_DRV_PIX_FORMAT_E ePixFormat,HI_DRV_PIXEL_BITWIDTH_E  enOutBitWidth)
{
    HI_U32 u32RetSize = 0;
    HI_U32 u32RetStride = 0;

    //:TODO:默认按10bit紧凑分配
    switch (ePixFormat)
    {
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21:
            if(HI_DRV_PIXEL_BITWIDTH_8BIT == enOutBitWidth)
            {
                u32RetStride = (u32Width + 0xF) & 0xFFFFFFF0;
            }
            else
            {
                u32RetStride = HI_ALIGN_10BIT_COMP_YSTRIDE(u32Width); 
            }
            u32RetSize = u32Height * u32RetStride*3/2;
            break;
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1:
            if(HI_DRV_PIXEL_BITWIDTH_8BIT == enOutBitWidth)
            {
                u32RetStride = HI_ALIGN_8BIT_YSTRIDE(u32Width);
            }
            else
            {
                u32RetStride = HI_ALIGN_10BIT_COMP_YSTRIDE(u32Width); 
            }
            u32RetSize = u32Height * u32RetStride*2;
            break;
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV21_CMP:
			{
			#if defined(CHIP_TYPE_hifone) || defined(CHIP_TYPE_hi3716mv410)|| defined(CHIP_TYPE_hi3716mv420)
				HI_U32 u32tempt = 0;
				u32tempt = u32Width * ( (HI_DRV_PIXEL_BITWIDTH_10BIT == enOutBitWidth)? 10 : 8);
				if ( u32Width <= 3584)
				{
					u32RetStride = 16 + (u32tempt+127)/128 * 16;
				}
				else if( 3584 < u32Width && u32Width <= 7680)
				{
					u32RetStride = 2*16 + (u32tempt+127)/128 * 16;
				}
				else
				{
					u32RetStride = 3*16 + (u32tempt+127)/128 * 16;
				}	

			#else
				if(HI_DRV_PIXEL_BITWIDTH_8BIT == enOutBitWidth)
				{
					u32RetStride = HI_ALIGN_8BIT_YSTRIDE(u32Width);
				}
				else
				{
					u32RetStride = HI_ALIGN_10BIT_COMP_YSTRIDE(u32Width); 
				}
			#endif
			}
            u32RetSize = u32Height * u32RetStride*3/2 + 16 * u32Height*3/2;
            break;
        case HI_DRV_PIX_FMT_NV16_CMP:
        case HI_DRV_PIX_FMT_NV61_CMP:
            if(HI_DRV_PIXEL_BITWIDTH_8BIT == enOutBitWidth)
            {
                u32RetStride = HI_ALIGN_8BIT_YSTRIDE(u32Width);
            }
            else
            {
                u32RetStride = HI_ALIGN_10BIT_COMP_YSTRIDE(u32Width); 
            }
            u32RetSize = u32Height * u32RetStride*3/2 + 16 * u32Height*2;
            break;
        case HI_DRV_PIX_FMT_ARGB8888:
        case HI_DRV_PIX_FMT_ABGR8888:
            u32RetStride = HI_ALIGN_8BIT_YSTRIDE(u32Width*4);
            u32RetSize = u32Height * u32RetStride;
            break;
        default:
            VPSS_FATAL("Unsupport PixFormat %d.\n",ePixFormat);
            return HI_FAILURE;
    }
    
    *pSize = u32RetSize;
    *pStride = u32RetStride;
    return HI_SUCCESS;
}


HI_S32 VPSS_OSAL_GetVpssVersion(VPSS_VERSION_E *penVersion, VPSS_CAPABILITY_U *penCapability)
{
    HI_CHIP_TYPE_E enChipType;
    HI_CHIP_VERSION_E enChipVersion;

    VPSS_CHECK_NULL(penVersion);

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

	penCapability->u32 = 0; //should be initialized

	if (enChipType == HI_CHIP_TYPE_HI3716M 
			&& (enChipVersion == HI_CHIP_VERSION_V410 || enChipVersion == HI_CHIP_VERSION_V420))
	{
		*penVersion = VPSS_VERSION_V3_0;
		penCapability->bits.hi3716mv410 = HI_TRUE;
	}
	else if (enChipType == HI_CHIP_TYPE_HI3798C)
    {
        *penVersion = VPSS_VERSION_V2_0;
    }
    else if (enChipType == HI_CHIP_TYPE_HI3716C)
    {
        *penVersion = VPSS_VERSION_BUTT;
    }
    else if (enChipType == HI_CHIP_TYPE_HI3798M_A 
            || enChipType == HI_CHIP_TYPE_HI3796M 
            || enChipType == HI_CHIP_TYPE_HI3798M 
            || enChipType == HI_CHIP_TYPE_HI3716D)
    {
        *penVersion = VPSS_VERSION_V1_0;
		penCapability->bits.hi3798mv100 = HI_TRUE;
    }
    else if ((enChipType == HI_CHIP_TYPE_HI3798C_A
				|| enChipType == HI_CHIP_TYPE_HI3798C_B) && enChipVersion == HI_CHIP_VERSION_V200)
    {
        *penVersion = VPSS_VERSION_V1_0;
		penCapability->bits.hi3798cv200_a = HI_TRUE;
    }
    else
    {
        *penVersion = VPSS_VERSION_BUTT;
		penCapability->u32 = 0;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_OSAL_GetTileOffsetAddr(HI_U32 u32Xoffset,HI_U32 u32Yoffset,
                                   HI_U32 *pu32Yaddr,HI_U32 *pu32Caddr,
                                   HI_DRV_VID_FRAME_ADDR_S *pstOriAddr)
{
    HI_U32 tile_pix_pos,tile_y_stride,tile_c_stride;
    HI_U32 u32Y_Stride,u32Y_Addr;
    HI_U32 u32C_Stride,u32C_Addr;
    HI_U32 u32C_StartAddr;
    HI_U32 u32Y_StartAddr;
    
    u32Y_Addr = pstOriAddr->u32PhyAddr_Y;
    u32Y_Stride = pstOriAddr->u32Stride_Y;
    
    tile_pix_pos = (((u32Xoffset)/256)==(u32Y_Stride/256));
    tile_y_stride = (tile_pix_pos==1)?(((u32Y_Stride%256)==0)?256:(u32Y_Stride%256)):256;
    u32Y_StartAddr =u32Y_Addr+((u32Yoffset)/16)*u32Y_Stride*16+((u32Xoffset)/256)*256*16+((u32Yoffset)%16)*tile_y_stride+(u32Xoffset)%256;

    u32C_Addr = pstOriAddr->u32PhyAddr_C;
    u32C_Stride = pstOriAddr->u32Stride_C;

    tile_pix_pos = (((u32Xoffset)/256)==(u32C_Stride/256));
    tile_c_stride = (tile_pix_pos==1)?(((u32C_Stride%256)==0)?256:(u32C_Stride%256)):256;
    u32C_StartAddr =u32C_Addr+((u32Yoffset/2)/8)*u32C_Stride*8+((u32Xoffset)/256)*256*8+((u32Yoffset/2)%8)*tile_c_stride+(u32Xoffset)%256;

    *pu32Yaddr = u32Y_StartAddr;
    *pu32Caddr = u32C_StartAddr;

    return HI_SUCCESS;
}                           
HI_S32 VPSS_OSAL_GetCurTime(HI_U32 *pu32Hour,HI_U32 *pu32Minute,
                                   HI_U32 *pu32Second)
{
    struct tm now;

    time_to_tm(get_seconds(), 0, &now);

    *pu32Hour = now.tm_hour;
    *pu32Minute = now.tm_min;
    *pu32Second = now.tm_sec;

    return HI_SUCCESS;
}

HI_S32 VPSS_OSAL_GetSysMemSize(HI_U32 *pu32MemSize)
{
    HI_S32 s32Ret;
    HI_U32 u32Mem = 0;
    HI_SYS_MEM_CONFIG_S stConfig;

    s32Ret = HI_DRV_SYS_GetMemConfig(&stConfig);

    if (HI_SUCCESS == s32Ret)
    {
        u32Mem = stConfig.u32TotalSize;
    }
    else
    {
        u32Mem = 0;
    }

    *pu32MemSize = u32Mem;

    return s32Ret;
}

HI_S32 VPSS_OSAL_AllocateMem(HI_U8 u8flag, 
		HI_U32  u32Size,
		HI_U8  *pu8ZoneName,
		HI_U8  *pu8MemName,
		VPSS_MEM_S *pstMem)
{
	HI_S32 s32Ret = HI_SUCCESS;

    MMZ_BUFFER_S stMMZ;

	if (u8flag == VPSS_MEM_FLAG_NORMAL)
	{
		s32Ret = HI_DRV_MMZ_AllocAndMap( pu8MemName, pu8ZoneName, 
				u32Size, 0, &stMMZ);
		if (s32Ret == HI_SUCCESS)
		{
			pstMem->u32Size = stMMZ.u32Size;
			pstMem->u32StartPhyAddr = stMMZ.u32StartPhyAddr;
	
			pstMem->pu8StartVirAddr = stMMZ.pu8StartVirAddr;

			pstMem->u8flag = u8flag;
		}
	}
	else if (u8flag == VPSS_MEM_FLAG_SECURE)
	{
#ifdef HI_TEE_SUPPORT
		HI_U32 u32SecAddr;
		
		u32SecAddr = (HI_U32)HI_SEC_MMZ_New(u32Size,"SEC-MMZ",pu8MemName);
		if (u32SecAddr != 0x0)
		{
			pstMem->u32Size = u32Size;
			pstMem->u32StartPhyAddr = u32SecAddr;
			pstMem->pu8StartVirAddr = (HI_U8*)u32SecAddr;
			pstMem->u8flag = u8flag;
		}
		else
		{
			VPSS_ERROR("alloc secure buffer failed\n");
		}
#else
		s32Ret = HI_DRV_MMZ_AllocAndMap( pu8MemName, pu8ZoneName, 
				u32Size, 0, &stMMZ);
		if (s32Ret == HI_SUCCESS)
		{
			pstMem->u32Size = stMMZ.u32Size;
			pstMem->u32StartPhyAddr = stMMZ.u32StartPhyAddr;

			pstMem->pu8StartVirAddr = stMMZ.pu8StartVirAddr;

			pstMem->u8flag = u8flag;
		}
#endif
	}
	else
	{
	
	}
	return s32Ret;
}

HI_S32 VPSS_OSAL_FreeMem(VPSS_MEM_S *pstMem)
{
    MMZ_BUFFER_S stMMZ;
	HI_S32 s32Ret = HI_SUCCESS;

	if (pstMem->u8flag == VPSS_MEM_FLAG_NORMAL)
	{
		stMMZ.u32StartPhyAddr = pstMem->u32StartPhyAddr;

		stMMZ.pu8StartVirAddr = pstMem->pu8StartVirAddr;

		stMMZ.u32Size = pstMem->u32Size;
		HI_DRV_MMZ_UnmapAndRelease(&stMMZ);
	}
	else if (pstMem->u8flag == VPSS_MEM_FLAG_SECURE)
	{
#ifdef HI_TEE_SUPPORT
		(HI_VOID)HI_SEC_MMZ_Delete(pstMem->u32StartPhyAddr);
#else
		stMMZ.u32StartPhyAddr = pstMem->u32StartPhyAddr;


		stMMZ.pu8StartVirAddr = pstMem->pu8StartVirAddr;

		stMMZ.u32Size = pstMem->u32Size;
		HI_DRV_MMZ_UnmapAndRelease(&stMMZ);
#endif
	}
	else
	{
	
	}

	return s32Ret;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
