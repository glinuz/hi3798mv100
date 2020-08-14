
#include "drv_disp_alg_service.h"
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
#include <linux/smp_lock.h>
#endif
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>

#define  MAX_ALG_SUPPORT_NUM  20

HI_BOOL CheckWindowOverlap(WindowInfor_S stWindowBase,WindowInfor_S stWindowIn)
{   
    //判断上下左右，不重叠的条件，其他条件下都是重叠的
    if((stWindowIn.y_end <= stWindowBase.y_start) || (stWindowIn.y_start >= stWindowBase.y_end)
        || (stWindowIn.x_end <= stWindowBase.x_start) || (stWindowIn.x_start >= stWindowBase.x_end)) 
    {
        return HI_FALSE;        
    }
    else
    {
        return HI_TRUE;
    }
}

HI_VOID RegisterWindowOverlap(WindowMappingResult_S *winMappingResult,
                             HI_U32 *calcWindowNum,
                             HI_U32 numofWindow)
{
    HI_U32 i = 0;

    for (i = 0; i < numofWindow; i++)
    {        
        winMappingResult[i].u32OverlapCnt = calcWindowNum[i];
    }

    return;
}

HI_S32 judgeWhetherZorderMatch(WindowMappingResult_S *winMappingResult,
                             HI_U32 numofWindow,
                             WindowInfor_S *winCoordinate)
{
    HI_U32 i = 0, v0_zorder = 0;
    HI_BOOL blarger = HI_FALSE;
    HI_BOOL bless = HI_FALSE;

    for (i = 0; i < numofWindow; i++)
    {
        if (winMappingResult[i].videoLayer == V0)
        {
            v0_zorder = winCoordinate[i].zorder;
            break;
        }
    }
    
    for (i = 0; i < numofWindow; i++)
    {
        if (winMappingResult[i].u32OverlapCnt != 0)
        {
            if (winCoordinate[i].zorder > v0_zorder)
                blarger = HI_TRUE;
            else if (winCoordinate[i].zorder < v0_zorder)
                bless = HI_TRUE;
        }
    }
    
    if ((blarger) && (bless))
        return HI_FAILURE;
    
    return HI_SUCCESS;
}




HI_VOID CalcWindowOverlapNum(WindowInfor_S *winCoordinate,HI_U32 *calcWindowNum,HI_U32  numofWindow)
{
    HI_U32 u32X;
    HI_U32 u32Y;
    
    for(u32X = 0; u32X < numofWindow; u32X++)
    {
        for(u32Y = 0; u32Y < numofWindow; u32Y++)
        {
            if(u32X != u32Y)
            {
                calcWindowNum[u32X] += CheckWindowOverlap(winCoordinate[u32X],winCoordinate[u32Y]);    
            }
        }    
    }
}

HI_VOID GetMaxWindowOverlapPos(HI_U32 *calcWindowNum,HI_U32 numofWindow,HI_U32 *maxOverlapValue,HI_U32 *maxOverlapWindow)
{
    HI_U32 u32X;
    
    *maxOverlapValue = calcWindowNum[0];
    *maxOverlapWindow = 0;
    for(u32X = 1; u32X < numofWindow; u32X++)
    {
        if(calcWindowNum[u32X] > *maxOverlapValue)
        {
            *maxOverlapValue = calcWindowNum[u32X];
            *maxOverlapWindow = u32X;  
        }             
    }
}

HI_U32 GetWindowArea(WindowInfor_S stWindowInfo)
{
    return (stWindowInfo.x_end - stWindowInfo.x_start)*(stWindowInfo.y_end - stWindowInfo.y_start);

}
HI_VOID GetMaxWindowAreaPos(WindowInfor_S *winCoordinate,HI_U32 numofWindow,HI_U32 *maxAreaWindow)
{
    HI_U32 u32X;
    HI_U32 u32MaxArea;
    HI_U32 u32MaxZorder;    
    HI_U32 i = 0, j = 0;
    HI_U32 zorder[MAX_ALG_SUPPORT_NUM][2] = {{0}};
    
    *maxAreaWindow = 0;
    u32MaxArea= GetWindowArea(winCoordinate[0]);
    for(u32X = 1; u32X < numofWindow; u32X++)
    {
        if(GetWindowArea(winCoordinate[u32X]) > u32MaxArea)
        {
            u32MaxArea = GetWindowArea(winCoordinate[u32X]);
            *maxAreaWindow = u32X;  
        }
    }


    j = 0;
    for (i = 0; i < numofWindow; i++)
    {
        if (GetWindowArea(winCoordinate[i]) == u32MaxArea)
        {
            zorder[j][0] = i;
            zorder[j][1] = winCoordinate[i].zorder;
            j ++;
        }
    } 

    *maxAreaWindow = zorder[0][0];
    u32MaxZorder = zorder[0][1];
    
    for (i = 1; i < j; i++)
    {
        if (zorder[i][1] > u32MaxZorder)
        {
            *maxAreaWindow = zorder[i][0];
            u32MaxZorder = zorder[i][1];
        }
    }
    
    return ;
}

HI_S32 GetMaxZorderByOverlapWindow(WindowInfor_S *winCoordinate,
                                    HI_U32  numofWindow,
                                    HI_U32 FindOverlapWindow,
                                    HI_U32 * GetMaxZorderWindow,
                                    WindowMappingResult_S *winMappingResult_tmp)
{
    HI_U32 u32X;
    HI_S32  s32Ret = 0, s32Ret1 = 0;
    *GetMaxZorderWindow = 0;
    
    for(u32X = 0; u32X < numofWindow; u32X++)
    {
        if((FindOverlapWindow != u32X) && CheckWindowOverlap(winCoordinate[FindOverlapWindow],winCoordinate[u32X]))
        {
            if (GetWindowArea(winCoordinate[FindOverlapWindow]) > GetWindowArea(winCoordinate[u32X]))
                *GetMaxZorderWindow = FindOverlapWindow;
            else if (GetWindowArea(winCoordinate[FindOverlapWindow]) < GetWindowArea(winCoordinate[u32X]))
                *GetMaxZorderWindow = u32X;
            else 
                *GetMaxZorderWindow = (winCoordinate[FindOverlapWindow].zorder > winCoordinate[u32X].zorder ?  FindOverlapWindow:u32X);           
            break;
        }
    }

    
    ClacLayerMapping(winCoordinate,winMappingResult_tmp,numofWindow,FindOverlapWindow);
    s32Ret = judgeWhetherZorderMatch(winMappingResult_tmp, numofWindow, winCoordinate);
    
    ClacLayerMapping(winCoordinate,winMappingResult_tmp,numofWindow,u32X);        
    s32Ret1 = judgeWhetherZorderMatch(winMappingResult_tmp, numofWindow, winCoordinate);

    if (s32Ret && s32Ret1)
    {
        return HI_FAILURE;
    }
    else if (!s32Ret && s32Ret1)
    {
        *GetMaxZorderWindow = FindOverlapWindow;
    }
    else if (s32Ret && !s32Ret1)
    {
        *GetMaxZorderWindow = u32X;
    }
    
    return HI_SUCCESS;    
}
HI_VOID ClacLayerMapping(WindowInfor_S *winCoordinate,WindowMappingResult_S *winMappingResult,HI_U32  numofWindow,HI_U32 u32VoWinPos)
{
    //已经知道哪个window应该放置到v0,现在将剩余的window排序
    HI_U32 u32X; 
    HI_U32 u32Y; 
    HI_U32 u32ArrCount; 
    AlgInfor_s *winOrderArr;
    AlgInfor_s tmpAlgInfo;
    AlgInfor_s    stAlgInfor_s[MAX_ALG_SUPPORT_NUM] = {{0}};
    
    winOrderArr = stAlgInfor_s; 
    u32Y = 0;
    
    for(u32X = 0; u32X < numofWindow; u32X++)
    {
        if(u32X == u32VoWinPos)
        {
            continue;
        }
        winOrderArr[u32Y].x_start = winCoordinate[u32X].x_start;
        winOrderArr[u32Y].u32OrigPos = u32X;
        u32Y++;
    }

    //X坐标排序,冒泡排序，大在后
    u32ArrCount = (numofWindow-1);
    for(u32X = 0; u32X < u32ArrCount; u32X++)
    {
        for(u32Y = u32X + 1; u32Y < u32ArrCount; u32Y++)
        {
            if(winOrderArr[u32Y].x_start <  winOrderArr[u32X].x_start)
            {
                tmpAlgInfo = winOrderArr[u32X];
                winOrderArr[u32X] = winOrderArr[u32Y];
                winOrderArr[u32Y] = tmpAlgInfo;                
            }
        }
    }

    //开始计算输出结果
    for(u32X = 0; u32X < u32ArrCount; u32X++)
    {
        winMappingResult[winOrderArr[u32X].u32OrigPos].regionNo = u32X;      
        winMappingResult[winOrderArr[u32X].u32OrigPos].videoLayer = V1;
    }
    
    winMappingResult[u32VoWinPos].regionNo = 0; 
    winMappingResult[u32VoWinPos].videoLayer = V0;

    return ;
}


/*
说明： 实现一个n个窗口到V0,V1 2个通道的映射算法，其中：V0上只能分配一个窗口；
       V1上：可以分配n-1个窗口。 窗口到通道映射完毕后，分配到V1上的N-1个窗口，
       还需要根据x值从小到大排序， 返回排序后的编号。比如x值最小的编号为0，
       x值最大的为n-2；              

使用及约束；
   1） 这n个窗口， 只允许1个窗口位置不受限制：可与其它窗口重叠，剩余N-1个窗口彼此之间不允许重叠；              
        当不满足这条限制时，即可报错；
   2） V0/V1能力集不同： V0上的窗口可以与其它任意窗口重叠， V1上的窗口，彼此之间不能重叠；
   
   3） V0不能出现空缺；即无窗口的情况；
   
   4） 如果窗口互不重叠：
       a） 窗口面积最大者，优先分配到V0通道上；
       b)  面积最大者有2个，则窗口的zorder优先级高者，分配到V0上；
       
   5） 如果出现窗口重叠，V0上的分配优先级按如下优先级来：
          a）如果将某个窗口分配到V0上，其余不再出现彼此重叠，这样的窗口有且只有1个；
             即需要把此窗口分配到V0上；
          b) 如果将某个窗口分配到V0上，其余不再出现彼此重叠，这样的窗口有多余一个，
             则按照“4）如果窗口互不重叠”的优先级来；
          c) 如果任何一个窗口分配到V0上，剩余的窗口仍然彼此重叠，则需要报错；
   6） 如果分配到Ｖ０上的窗口winv0，与多于一个的窗口重叠，则winv0的zorder需要大于
       或者小于其它与其有重叠的窗口的 zorder， 不能介于这些重叠窗口zorder之间；
          
   7） 此算法只是做窗口到通道的映射，以及V1上窗口x值由小到大的排序（输出编号）
       不能改变窗口的位置；
*/

HI_S32 WindowAndLayerMapping( WindowInfor_S *winCoordinate,
			      WindowMappingResult_S *winMappingResult,
			      HI_U32  numofWindow)
{
    HI_U32 u32FirstMaxOverlapValue;  
    HI_U32 u32SecondMaxOverlapValue;
    HI_U32 u32FirstMaxOverlapWindow;
    HI_U32 u32SecondMaxOverlapWindow;    
    HI_U32 u32VoWinPos;    
    WindowInfor_S *winTmpCoordinate;
    HI_U32 *calcWindowNumArr;
    WindowInfor_S stWinInfor[MAX_ALG_SUPPORT_NUM] = {{0}};
    HI_U32        u32calcWindowNumArr[MAX_ALG_SUPPORT_NUM] = {0};
    WindowMappingResult_S winMappingResult_tmp[MAX_ALG_SUPPORT_NUM];
    HI_S32 s32Ret = HI_SUCCESS;
    
    memset((void*)winMappingResult_tmp, 0, sizeof(WindowMappingResult_S) * MAX_ALG_SUPPORT_NUM);
    if (1 == numofWindow)
    {
        winMappingResult[0].videoLayer = V0;
        winMappingResult[0].regionNo = 0;
        return HI_SUCCESS;
    } 
    else if (0 == numofWindow)
    {
       return HI_FAILURE;
    }
    else if (numofWindow > MAX_ALG_SUPPORT_NUM)
    {
        return HI_FAILURE;
    }
    
    winTmpCoordinate = stWinInfor;
    memcpy((HI_VOID*)winTmpCoordinate,(HI_VOID*)winCoordinate,numofWindow*sizeof(WindowInfor_S));
    
    calcWindowNumArr = u32calcWindowNumArr;
    memset((HI_VOID*)calcWindowNumArr,0,numofWindow * sizeof(HI_U32));
    
    /*calculate the overlap count for every window.*/
    CalcWindowOverlapNum(winTmpCoordinate,calcWindowNumArr,numofWindow);
    /*register every window's overlap cnt to final winMappingResult.*/
    RegisterWindowOverlap(winMappingResult,calcWindowNumArr,numofWindow);
    
    GetMaxWindowOverlapPos(calcWindowNumArr,
                           numofWindow,
                           &u32FirstMaxOverlapValue,
                           &u32FirstMaxOverlapWindow);
    
    /*no overlap*/
    if(0 == u32FirstMaxOverlapValue)
    {
        GetMaxWindowAreaPos(winTmpCoordinate,numofWindow,&u32VoWinPos);         
        ClacLayerMapping(winTmpCoordinate,winMappingResult,numofWindow,u32VoWinPos); 
    }
    else
    {
        //去掉重叠次数最多的窗口，再次判断是否还有重叠
        winTmpCoordinate[u32FirstMaxOverlapWindow].x_start = 0;
        winTmpCoordinate[u32FirstMaxOverlapWindow].x_end = 0;
        winTmpCoordinate[u32FirstMaxOverlapWindow].y_start = 0;
        winTmpCoordinate[u32FirstMaxOverlapWindow].y_end = 0;
        
        memset((HI_VOID*)calcWindowNumArr,0,numofWindow * sizeof(HI_U32));
        CalcWindowOverlapNum(winTmpCoordinate,calcWindowNumArr,numofWindow);
        GetMaxWindowOverlapPos(calcWindowNumArr,numofWindow,&u32SecondMaxOverlapValue,&u32SecondMaxOverlapWindow);       

        //还是有重叠，无法修正，返回错误    
        if(0 < u32SecondMaxOverlapValue)
        {
            s32Ret =  HI_FAILURE;            
            if (s32Ret)
                goto ALG_EXIT;
        }

        memcpy((HI_VOID*)winTmpCoordinate,(HI_VOID*)winCoordinate,numofWindow*sizeof(WindowInfor_S));
        
        //只一个窗口与多个窗口重叠
        if(2 <= u32FirstMaxOverlapValue)
        {
            u32VoWinPos = u32FirstMaxOverlapWindow;
            ClacLayerMapping(winTmpCoordinate,winMappingResult,numofWindow,u32VoWinPos);
        }
        else//两个窗口相互重叠
        {
            //判断下两个窗体的order顺序            
            memcpy((void*)winMappingResult_tmp, (void*)winMappingResult, numofWindow*sizeof(WindowMappingResult_S));            
            s32Ret = GetMaxZorderByOverlapWindow(winTmpCoordinate,
                                        numofWindow,
                                        u32FirstMaxOverlapWindow,
                                        &u32VoWinPos,
                                        winMappingResult_tmp);
             if (s32Ret)             
                goto ALG_EXIT;
             
            ClacLayerMapping(winTmpCoordinate,winMappingResult,numofWindow,u32VoWinPos);
        }

        /*if some window overlaps, the zorder in the window  which located in v0,  should
          be less than all the overlaps window ,or be larger than all the overlaps window.*/
        s32Ret = judgeWhetherZorderMatch(winMappingResult, numofWindow, winTmpCoordinate);
        if (s32Ret)
            goto ALG_EXIT;
    }
    
ALG_EXIT:    
	return s32Ret;
}


HI_S32 layerZorderMatch( WindowZorderInfor_S *winZorderInfor,			    
		                 HI_U32  numofWindow)
{
    HI_U32 i = 0, win_zorder = 0, k = 0;
    HI_U32 layerV0Zorder = 0;
    HI_U32 layerV1Zorder = 0;
    
    HI_BOOL blarger = HI_FALSE;
    HI_BOOL bless = HI_FALSE;

    for (i = 0; i < numofWindow; i++)
    {
        if ((winZorderInfor[i].layerNumber == V0)
            || (winZorderInfor[i].layerNumber == (V0 +3)))
        {
            win_zorder = winZorderInfor[i].winZorder;
            layerV0Zorder = winZorderInfor[i].layerZorder;
            k = i;
        }
        else
        {
            layerV1Zorder = winZorderInfor[i].layerZorder;
        }  
        
        winZorderInfor[i].matchOperation = HI_DRV_DISP_ZORDER_BUTT;  
    }
    
    for (i = 0; i < numofWindow; i++)
    {
        if (winZorderInfor[i].winZorder > win_zorder)
            bless = HI_TRUE;
        else if (winZorderInfor[i].winZorder < win_zorder)
            blarger = HI_TRUE;
    }
    
    if ((blarger) && (bless))
        return HI_FAILURE;

    if (blarger)
    {
        if (layerV0Zorder > layerV1Zorder)
            winZorderInfor[k].matchOperation = HI_DRV_DISP_ZORDER_BUTT;
        else if (layerV0Zorder < layerV1Zorder)
            winZorderInfor[k].matchOperation = HI_DRV_DISP_ZORDER_MOVETOP;
    }
    else if (bless)
    {
        if (layerV0Zorder <  layerV1Zorder)
            winZorderInfor[k].matchOperation = HI_DRV_DISP_ZORDER_BUTT;
        else if (layerV0Zorder > layerV1Zorder)
            winZorderInfor[k].matchOperation = HI_DRV_DISP_ZORDER_MOVEBOTTOM;
    }    
    
	return HI_SUCCESS;	
}

