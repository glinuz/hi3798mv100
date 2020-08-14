/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name       	    :   avplay_frc.c
  Version        		: 	Initial Draft
  Author         		: 	Hisilicon multimedia software group
  Created       		:
  Description  	        :   Algorithm Module 
  History       		:
  1.Date        		:
  Author      		    :
  Modification   	    :	Created file

******************************************************************************/
#include <linux/time.h>
#include <linux/memory.h>
#include "drv_mce_frc.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

HI_VOID MCE_FrcCreate(AVPLAY_ALG_FRC_S *FrcCalAlg, AVPLAY_FRC_CFG_S *FrcParamCfg, AVPLAY_FRC_CTRL_S *FrcCtrlInfo)
{
    memset(FrcCalAlg, 0, sizeof(AVPLAY_ALG_FRC_S));

    FrcCtrlInfo->s32FrmState = 0;

    FrcParamCfg->u32InRate   = 25;
    FrcParamCfg->u32OutRate  = 25;
    FrcParamCfg->u32PlayRate = AVPLAY_ALG_FRC_BASE_PLAY_RATIO;
}

HI_VOID MCE_FrcDestroy(AVPLAY_ALG_FRC_S *FrcCalAlg)
{
    memset(FrcCalAlg, 0, sizeof(AVPLAY_ALG_FRC_S));
}

HI_VOID MCE_FrcReset(AVPLAY_ALG_FRC_S *hFrc)
{
    memset(hFrc, 0, sizeof(AVPLAY_ALG_FRC_S));
}

HI_VOID MCE_FrcCalculate(AVPLAY_ALG_FRC_S *hFrc, AVPLAY_FRC_CFG_S *pstFrcCfg, AVPLAY_FRC_CTRL_S *pstFrcCtrl)
{
	HI_U32 quot, remder, u32InRate, u32OutRate, u32tmpFrameRate;
	HI_S32 flag = 0;
	HI_U32 tmp = 0;
	HI_U32 remder_cyc;
	HI_U32 u32PlayRate;
	HI_U32  u32CurID;     /* current insert or drop position in a FRC cycle, add by b00182026*/

    u32tmpFrameRate = pstFrcCfg->u32InRate;

    //if (u32tmpFrameRate <= 60) 	/* to support stream which rate is less tha 6 frame per second/ */
    if (u32tmpFrameRate < 1)
    {
    	pstFrcCtrl->s32FrmState = 0;
    	return;
    }

    if ( (hFrc->u32OutRate != pstFrcCfg->u32OutRate)
	    ||(hFrc->u32PlayRate != pstFrcCfg->u32PlayRate)			/*add by b00182026*/
	    ||(hFrc->u32InRate != u32tmpFrameRate)
	    )
    {
	    //RESET
	    MCE_FrcReset(hFrc);
    	hFrc->u32InRate = u32tmpFrameRate;
    	hFrc->u32OutRate = pstFrcCfg->u32OutRate;
    	hFrc->u32PlayRate = pstFrcCfg->u32PlayRate;
    	hFrc->u32CurID = 1;		/*this should be 0 or 1 ?*/
    }

    u32PlayRate = pstFrcCfg->u32PlayRate; //8bit fraction 

    u32OutRate = (hFrc->u32OutRate)*AVPLAY_ALG_FRC_PRECISION*AVPLAY_ALG_FRC_BASE_PLAY_RATIO/u32PlayRate; 	//support  256X playrate 
	u32InRate = (hFrc->u32InRate)*AVPLAY_ALG_FRC_PRECISION;

    u32CurID = hFrc->u32CurID;

	if((u32InRate) < (u32OutRate))  ////Lower framerate to Higher framerate ---need repeat
	{
	    quot = (u32OutRate)/(u32InRate);
	    remder = (u32OutRate)%(u32InRate);
	    remder_cyc = (hFrc->u32InputCount%u32InRate + 1);

	    flag = 0; //if remder==0
	    if(remder==0)
	    {
		    flag = 0;
	    }
	    else
	    {
		tmp = ((u32InRate*u32CurID + remder/2) / remder);
		if (tmp == remder_cyc)
		{
		    flag = 1;
		    hFrc->u32CurID++;
		    hFrc->u32CurID = (hFrc->u32CurID%remder == 0)? remder: (hFrc->u32CurID%remder);
		}
		else
		{
		    flag = 0;
		}
	    }

	    if(flag==1)
	    {
    		//repeat time: (quot-1)+1;
    		pstFrcCtrl->s32FrmState = quot;
	    }
	    else
	    {
    		//repeat time: (quot-1);
    		pstFrcCtrl->s32FrmState = quot - 1;
	    }

	}
	else if(u32InRate > u32OutRate) //Higher framerate to Lower framerate ---need drop
	{
	    quot = (u32InRate)/(u32OutRate);
	    remder = (u32InRate)-(u32OutRate);
	    remder_cyc = (hFrc->u32InputCount%u32InRate + 1);

	    flag = 0; //if remder==0
	    if(remder==0)
	    {
    		flag =0;
	    }
	    else
	    {
    		tmp = ((u32InRate*u32CurID + remder/2) / remder);
    		if(tmp == remder_cyc)
    		{
    		    flag = 1;
    		    hFrc->u32CurID++;
    		    hFrc->u32CurID = (hFrc->u32CurID%remder == 0)? remder: (hFrc->u32CurID%remder);
    		}
    		else
    		{
    		    flag = 0;
    		}
	    }

	    //if flag==1, need drop this frame.
	    if(flag==1)
	    {
    		pstFrcCtrl->s32FrmState = -1;
	    }
	    else
	    {
    		pstFrcCtrl->s32FrmState = 0;
	    }
	}
	else  //don't need frame rate conversion
	{
	    pstFrcCtrl->s32FrmState = 0;
	}

    hFrc->u32InputCount++;

    return;    
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


