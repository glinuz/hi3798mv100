#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <linux/types.h>

#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "hi_mpi_win.h"
#include "drv_win_ioctl.h"

#include "hi_mpi_avplay.h"
#include "hi_error_mpi.h"
#include "hi_drv_struct.h"

#include "drv_venc_ext.h"
#include "win_ratio.h"

#include "./dcmp_implement.h"

HI_VOID  Dcmp_Func(HI_DRV_VIDEO_FRAME_S *pstOriginalCmpFrame,					   
					   HI_U32   u32DestDcmpBufSize,
					   HI_U8    *pInCmpBuf,
					   HI_U8    *pOutputDcmpBuf)
{	
		DCMP_LC_PARM_T stDcmpInfor;	
		HI_U8  *pu8tmp_mem = HI_NULL;
		HI_U32 *pu32Head = HI_NULL;		
		HI_U32  i = 0;
		HI_U32  u32Width = 0, u32Height = 0;
		HI_U32  y_stride_after_dcmp = 0;
		
		HI_U32  u32Ystride = pstOriginalCmpFrame->stBufAddr[0].u32Stride_Y;
		HI_U32  u32Cstride = pstOriginalCmpFrame->stBufAddr[0].u32Stride_C;
		

		u32Width =  pstOriginalCmpFrame->u32Width;
		u32Height =  pstOriginalCmpFrame->u32Height;
	
		
		/*for store the output dcmped date  from the alg. */
		pu8tmp_mem = malloc(u32DestDcmpBufSize * 2);

		/*store the input  head data to the alg. */
		pu32Head =  malloc(pstOriginalCmpFrame->u32Height * 4);
		
		stDcmpInfor.is_raw_cmp   = 0;/*fix  compress*/
		stDcmpInfor.is_luma      = 1;/*first dcmp luma*/
		stDcmpInfor.is_lossless  = 1;/*fix loseless*/
		stDcmpInfor.frm_width    = u32Width;/*fix loseless*/
		stDcmpInfor.frm_height   = u32Height ;
		stDcmpInfor.bit_dep      = 8;
		stDcmpInfor.is_stride_mem = 1;
		stDcmpInfor.stride    = (u32Ystride / 16);
		
		for(i = 0; i < stDcmpInfor.frm_height; i ++)
		{
			memset((void*)&pu32Head[i], 0, 4);
			memcpy((void*)&pu32Head[i], (pInCmpBuf + i * u32Ystride), 2);
		}
		
		(HI_VOID)dcmp_low_cost(&stDcmpInfor, (void*)(pInCmpBuf), (void*)pu8tmp_mem, (void*)pu32Head);


		y_stride_after_dcmp = (u32Width + 15) & 0xfffffff0;
		//HI_PRINT("------stride after dcmp:%d!\n", y_stride_after_dcmp);

		for(i = 0; i < u32Height; i ++)
		{
			unsigned  int j = 0;
			for (j = 0; j < u32Width; j++)
			{
				*(pOutputDcmpBuf + i * y_stride_after_dcmp + j) = *(pu8tmp_mem + i * y_stride_after_dcmp * 2 + 2 * j);
			}
		}
		
		
		stDcmpInfor.is_raw_cmp   =  0;   /*fix  compress*/
		stDcmpInfor.is_luma      =  0;   /*first dcmp luma*/
		stDcmpInfor.is_lossless  =  1; /*fix loseless*/
		stDcmpInfor.frm_width    =  u32Width / 2;/*fix loseless*/
		stDcmpInfor.frm_height   =  u32Height / 2;
		stDcmpInfor.bit_dep    = 8;
		stDcmpInfor.is_stride_mem    = 1;
		stDcmpInfor.stride    = u32Cstride / 16;
		//HI_PRINT("c head:%d, %d, stDcmpInfor.frm_height:%d!\n", u32Ystride, u32Height,stDcmpInfor.frm_height);
		
		for(i = 0; i < stDcmpInfor.frm_height; i ++)
		{
			memset((void*)&pu32Head[i], 0, 4);
			memcpy((void*)&pu32Head[i], (pInCmpBuf + u32Ystride * u32Height + i * u32Ystride), 2);
		}
		
		(HI_VOID)dcmp_low_cost(&stDcmpInfor, 
							  (void*)(pInCmpBuf + u32Ystride * u32Height),
							  (void*)pu8tmp_mem,
							  (void*)pu32Head);
		
		for(i = 0; i < u32Height/2; i++)
		{
			unsigned  int j = 0;
			for (j = 0; j < u32Width; j++)
			{
				if(j%2 == 0)
				{
					*(pOutputDcmpBuf + y_stride_after_dcmp * u32Height +  i * y_stride_after_dcmp + j) = *(pu8tmp_mem + i * y_stride_after_dcmp * 2 + 2 * (j + 1));
				}
				else
				{
					*(pOutputDcmpBuf + y_stride_after_dcmp * u32Height +  i * y_stride_after_dcmp + j) = *(pu8tmp_mem + i * y_stride_after_dcmp * 2 + 2 * (j - 1));
				}
			}
		}

		free(pu8tmp_mem);
		free(pu32Head);

}
