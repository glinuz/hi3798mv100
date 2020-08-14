/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCG6A3KHwW/DEcLM2NcwtJs7QdH8sM1n3S/1wSu
i69XAQKt7IknkkYgV2tKWh0AbsuTw6j/aWmxeTKDrzNTXReYsZDKxbeWtdXO93eOSk7+b9MF
tbFzPNbwnQL3tFV54I/eksux/XLfW8K59JGGD0IPW21j4/hLpmn/c3gajdPZCg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/***********************************************************************
*
* Copyright (c) 2006 HUAWEI - All Rights Reserved
*
* File: $public.h$
* Date: $2006/11/30$
* Revision: $v1.0$
* Purpose: source file for interface & storage management module
*
*
* Change History:
*
* Date             Author            Change
* ====             ======            ======
* 2006/11/30       z56361            Original
*
*
* Dependencies:
*
************************************************************************/

#include "basedef.h"
#include "mem_manage.h"
#include "public.h"

//#include "vfmw_osal.h"

#include "vfmw_osal_ext.h"

extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;

/******************************************************
    常数&数据结构&枚举
*******************************************************/
#ifdef ENV_ARMLINUX_KERNEL
#define  TRACE_MEM_SIZE   (16*1024*2)
#define  MAX_FUNC_NAME_LEN  15

/* 内存跟踪单元 */
typedef struct hiTRACE_OBJ_S
{
    SINT8    s8FuncName[MAX_FUNC_NAME_LEN + 1];
    SINT32   s32LineNum;
    SINT32   s32Data;
    UINT32   u32Time;
} TRACE_OBJ_S;

/* 内存跟踪器 */
typedef struct  hiTRACE_DESC_S
{
    /* 标识串 */
    UINT32   u32ValidFlag[4];

    /* 基准地址 */
    TRACE_OBJ_S  *pstTraceBaseObj;
    UADDR       s32TraceBasePhyAddr;

    /* 内部索引 */
    SINT32   s32MaxTraceNumMinus4;
    SINT32   s32CurrTraceNum;
} TRACE_DESC_S;

/******************************************************
   共享数据
*******************************************************/
TRACE_DESC_S  *s_pstTracer = 0;
UADDR        TracerPhyAddr = 0;

SINT32   g_LogEnable = 0;
SINT8    g_LogFileName[500];
#endif

/* 受控打印的控制变量 */
UINT32   g_PrintEnable = DEFAULT_PRINT_ENABLE;    /* 打印使能开关 */
UINT32   g_PrintDevice = DEFAULT_PRINT_DEVICE;    /* 打印设备选择开关 */

SINT32   g_TraceCtrl = 0xffffffff;   /* vfmw代码运行状态控制参数 */
SINT32   g_TraceBsPeriod = 200;    /* 码流相关打印和调试信息的周期控制参数 */
SINT32   g_TraceFramePeriod = 0;  /* 帧级相关打印和调试信息的周期控制参数 */
SINT32   g_TraceImgPeriod = 500;    /* 图像相关打印和调试信息的周期控制参数 */
SINT32   g_TunnelLineNumber = 10; /*模块间低延时行号水线控制参数*/
#ifdef VFMW_SCD_LOWDLY_SUPPORT
LOWDLY_INFO_S  g_stLowdlyInfo[MAX_CHAN_NUM];
#endif

char *g_strDefine[] = 
{
    "hi_vdec_vdh_irq",
    "hi_vdec_vdh1_irq",
    "hi_vdec_vdh2_irq",
    "hi_vdec_scd_irq",
    "hi_vdec_scd1_irq",
    "VFMW_Chan_Ctx",
    "VFMW_Chan_Vdh",
    "VFMW_Chan_Scd",
    "VFMW_DspCtxMem",
    "hi_vdec_dnr_irq",
    "hi_vdec_btl_irq",
    "VFMW_Hal_%d",
    "VFMW_ScdMsg_%d",
    "%s/vfmw_raw_save_%d.dat",
    "%s/vfmw_seg_save_%d.dat",
    "%s/vfmw_yuv_save_%d.yuv",
    "%s/2d_0x%x.yuv",
    "%s/1d_0x%x.yuv",
    "VFMW_Tracer",
    "VFMW_BigTile1d_YUV"
};

char *VFMW_GetString(VFMW_STRING_E enStringName)
{
    if (enStringName >= VFMW_STRING_BUTT)
    {
        dprint(PRN_FATAL, "______invalid parm= %d ______\n", enStringName);
        
        return "";
    }
    
    return g_strDefine[enStringName];
}


#ifdef ENV_ARMLINUX_KERNEL
/*################################################################################################
    内存trace
#################################################################################################*/
/* 创建tracer */
#define VF0  0x1a2a3a4a
#define VF1  0x55657585
#define VF2  0x0f152f35
#define VF3  0x4a5f6a7f

SINT32   DBG_CreateTracer(VOID)
{
    SINT32 ret = DBG_ERR;
    MEM_RECORD_S MemRecord;

    if ( s_pstTracer )
    {
        return DBG_OK;
    }

    if (MEM_AllocMemBlock(VFMW_GetString(VFMW_STRING_TRACER), 0, TRACE_MEM_SIZE, &MemRecord, 0) == MEM_MAN_OK)
    {
        MEM_AddMemRecord(MemRecord.PhyAddr, MemRecord.VirAddr, MemRecord.Length);

        /* 上下文 */
        s_pstTracer = (TRACE_DESC_S *)MemRecord.VirAddr;
        TracerPhyAddr = MemRecord.PhyAddr;

        /* 清除tracer的内部数据 */
        //memset( MemRecord.VirAddr, 0, TRACE_MEM_SIZE );
        vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(MemRecord.VirAddr, 0, TRACE_MEM_SIZE);

        /* 基准地址 */
        s_pstTracer->pstTraceBaseObj = (TRACE_OBJ_S *)(MemRecord.VirAddr + sizeof(TRACE_DESC_S));
        s_pstTracer->s32TraceBasePhyAddr = (MemRecord.PhyAddr + sizeof(TRACE_DESC_S));

        /* 索引 */
        s_pstTracer->s32MaxTraceNumMinus4 = (MemRecord.Length - sizeof(TRACE_DESC_S)) / sizeof(TRACE_OBJ_S) - 4;
        s_pstTracer->s32CurrTraceNum = 0;

        /* 标识tracer有效 */
        s_pstTracer->u32ValidFlag[0] = VF0;
        s_pstTracer->u32ValidFlag[1] = VF1;
        s_pstTracer->u32ValidFlag[2] = VF2;
        s_pstTracer->u32ValidFlag[3] = VF3;

        dprint(PRN_DBG, "________________ mem_phy, trace_phy,trace_max = 0x%x, 0x%x,%d _________________\n", MemRecord.PhyAddr,
               (MemRecord.PhyAddr + sizeof(TRACE_DESC_S)), s_pstTracer->s32MaxTraceNumMinus4 );

        ret = DBG_OK;
    }


    return ret;
}

/* 销毁tracer */
VOID DBG_DestroyTracer(VOID)
{
    if ( NULL != s_pstTracer )
    {
        UADDR  PhyAddr;
        PhyAddr = MEM_Vir2Phy((SINT8 *)s_pstTracer);
        MEM_ReleaseMemBlock(PhyAddr, (SINT8 *)s_pstTracer);
        dprint(PRN_DBG, "=============== destroy tracer: phy,vir = 0x%x,%p ===================\n", PhyAddr, s_pstTracer);
        s_pstTracer = NULL;
    }

    return;
}

/* 增加一条trace 记录 */
VOID DBG_AddTrace( SINT8 *pFuncName, SINT32 nLine, SINT32 Data )
{
    if ( NULL != s_pstTracer )
    {
        SINT32  s32CurrTraceNum = s_pstTracer->s32CurrTraceNum;
        TRACE_OBJ_S *pstTraceObj;

        /* 尽快更新s_pstTracer->CurrTraceNum, 减少任务之间冲突的可能性，但依然是不严密的??? */
        s_pstTracer->s32CurrTraceNum = (s32CurrTraceNum < s_pstTracer->s32MaxTraceNumMinus4) ? (s32CurrTraceNum + 1) : 0;
        pstTraceObj = s_pstTracer->pstTraceBaseObj + s32CurrTraceNum;

        /* 记录这条trace信息 */
        pstTraceObj->s32LineNum = nLine;
        pstTraceObj->s32Data = Data;

        //pstTraceObj->u32Time = OSAL_GetTimeInUs();
        pstTraceObj->u32Time = vfmw_Osal_Func_Ptr_S.pfun_Osal_GetTimeInUs();

        //strncpy( pstTraceObj->s8FuncName, pFuncName, MAX_FUNC_NAME_LEN );
        vfmw_Osal_Func_Ptr_S.pfun_Osal_strlcpy( pstTraceObj->s8FuncName, pFuncName, MAX_FUNC_NAME_LEN );

        //pstTraceObj->s8FuncName[MAX_FUNC_NAME_LEN] = 0;
    }

    return;
}

VOID DBG_GetTracerInfo( SINT32 *pMemPhyAddr, SINT32 *pMemLength, SINT32 *pTracePhyAddr, SINT32 *pMaxNum, SINT32 *pCurIdx )
{
    if ( s_pstTracer )
    {
        *pMemPhyAddr = MEM_Vir2Phy((SINT8 *)s_pstTracer);
        *pMemLength  = TRACE_MEM_SIZE;
        *pTracePhyAddr = (SINT32)s_pstTracer->pstTraceBaseObj;
        *pMaxNum  = s_pstTracer->s32MaxTraceNumMinus4 + 1;
        *pCurIdx  = s_pstTracer->s32CurrTraceNum;
    }
    else
    {
        *pMemPhyAddr = 0;
        *pMemLength  = 0;
        *pTracePhyAddr = 0;
        *pMaxNum  = 0;
        *pCurIdx  = 0;
    }

    return;
}

VOID DBG_PrintTracer( SINT32 TracerMemroy )
{
    TRACE_DESC_S *pstThisTracer;
    UINT32 *pv;
    SINT32  s32CurrTraceNum;
    SINT32  i;
    TRACE_OBJ_S *pstTraceObj;

    if ( NULL == s_pstTracer )
    {
        //pstThisTracer = (TRACE_DESC_S*)ioremap( TracerMemroy, 1024 * 1024 );
        pstThisTracer = (TRACE_DESC_S *)vfmw_Osal_Func_Ptr_S.pfun_Osal_ioremap( TracerMemroy, 1024 * 1024 );

        if (NULL == pstThisTracer)
        {
            dprint(PRN_ALWS, "map vir address for '%x' failed!!!\n", TracerMemroy);
            return;
        }

        pstThisTracer->pstTraceBaseObj = (TRACE_OBJ_S *)((UINT32)pstThisTracer + sizeof(TRACE_DESC_S));

        dprint(PRN_ALWS, "tracer memory '0x%x' is mapped to '%p'\n", TracerMemroy, pstThisTracer);
    }
    else
    {
        pstThisTracer = s_pstTracer;
    }

    if (NULL == pstThisTracer)
    {
        dprint(PRN_ALWS, "map vir address for '%x' failed!!!\n", TracerMemroy);
        return;
    }

    pv = &pstThisTracer->u32ValidFlag[0];

    if (pv[0] != VF0 || pv[1] != VF1 || pv[2] != VF2	 || pv[3] != VF3)
    {
        dprint(PRN_ALWS, "no valid trace info stored in '%x'!!!\n", TracerMemroy);
        return;
    }

    dprint(PRN_ALWS, "====================== tracer context =====================\n");
    dprint(PRN_ALWS, "%-25s :%x\n", "pstTraceBaseObj", pstThisTracer->pstTraceBaseObj);
    dprint(PRN_ALWS, "%-25s :%p\n", "s32TraceBasePhyAddr", pstThisTracer->s32TraceBasePhyAddr);
    dprint(PRN_ALWS, "%-25s :%d\n", "s32MaxTraceNumMinus4", pstThisTracer->s32MaxTraceNumMinus4);
    dprint(PRN_ALWS, "%-25s :%d\n", "s32CurrTraceNum", pstThisTracer->s32CurrTraceNum);

    /* 内部索引 */
    s32CurrTraceNum = pstThisTracer->s32CurrTraceNum;

    for (i = 0; i < pstThisTracer->s32MaxTraceNumMinus4; i++)
    {
        s32CurrTraceNum = (s32CurrTraceNum == 0) ? pstThisTracer->s32MaxTraceNumMinus4 : (s32CurrTraceNum - 1);
        pstTraceObj = pstThisTracer->pstTraceBaseObj + s32CurrTraceNum;

        dprint(PRN_ALWS, "-%05d-", i);
        dprint(PRN_ALWS, "func:%-15s;", pstTraceObj->s8FuncName);
        dprint(PRN_ALWS, " line:%5d;", pstTraceObj->s32LineNum);
        dprint(PRN_ALWS, " data:0x%08x(%-10d);", pstTraceObj->s32Data, pstTraceObj->s32Data);
        dprint(PRN_ALWS, " time:%u\n", pstTraceObj->u32Time);
    }

    if (NULL == s_pstTracer)
    {
        //iounmap(pstThisTracer);
        vfmw_Osal_Func_Ptr_S.pfun_Osal_iounmap(pstThisTracer);
    }

    return;
}

//EXPORT_SYMBOL(DBG_AddTrace);
#endif

#ifdef VFMW_SCD_LOWDLY_SUPPORT
VOID DBG_CountTimeInfo(SINT32 ChanID, VFMW_STATE_TYPE_E SateType, SINT32 LowdlyFlag)
{
    SINT32 CurTime;
    LOWDLY_INFO_S *pLowdlyInfo = NULL;

    pLowdlyInfo = &(g_stLowdlyInfo[ChanID]);

    //CurTime = OSAL_GetTimeInMs();
    CurTime = vfmw_Osal_Func_Ptr_S.pfun_Osal_GetTimeInMs();

    pLowdlyInfo->chan_id = ChanID;

    switch (SateType)
    {
        case STATE_RCV_RAW:
            if (0 == pLowdlyInfo->receive_raw)
            {
                pLowdlyInfo->receive_raw = CurTime;
            }

            break;

        case STATE_SCD_START:
            if (0 != pLowdlyInfo->receive_raw)
            {
                pLowdlyInfo->scd_start = CurTime;
                pLowdlyInfo->lowdly_enable = LowdlyFlag;
            }

            break;

        case STATE_SCD_INTERRUPT:
            if (0 != pLowdlyInfo->receive_raw)
            {
                pLowdlyInfo->scd_return = CurTime;
                pLowdlyInfo->scd_count = CurTime - pLowdlyInfo->scd_start;
                pLowdlyInfo->raw_to_scd_return = CurTime - pLowdlyInfo->receive_raw;
            }

            break;

        case STATE_VDH_START:
            if (0 != pLowdlyInfo->receive_raw)
            {
                pLowdlyInfo->vdh_start = CurTime;
            }

            break;

        case STATE_VDH_INTERRUPT:
            if (0 != pLowdlyInfo->receive_raw)
            {
                pLowdlyInfo->vdh_return = CurTime;
                pLowdlyInfo->vdh_count = CurTime - pLowdlyInfo->vdh_start;
                pLowdlyInfo->raw_to_img = CurTime - pLowdlyInfo->receive_raw;
                pLowdlyInfo->receive_raw = 0;
            }

            break;

        case STATE_VO_RCV_IMG:
            pLowdlyInfo->vo_read_img = CurTime;
            pLowdlyInfo->raw_to_vo = CurTime - pLowdlyInfo->receive_raw;
            break;

        default:
            break;

    }

    return;
}
#endif

/*################################################################################################
    可控调试信息打印
    SINT32 dprint( UINT32 type, const char *format [, argument]... );
    SINT32 InitMemPrint( UINT8* pBuf, UINT32 BufLen );
#################################################################################################*/
SINT32 IsDprintTypeEnable(UINT32 type)
{
    if ((PRN_ALWS != type) && (0 == (g_PrintEnable & (1 << type)))) /* 该类别打印没有打开 */
    {
        return 0;
    }

    return 1;
}

VOID vfmw_dprint_nothing(VOID)
{
    return;
}

SINT32 CHECK_REC_POS_ENABLE(SINT32 type)
{
    if ((g_TraceCtrl & (1 << type)) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*************************************************************************/

/*************************************************************************/
/* 函数原型*/
static void shortsort(char *lo, char *hi, UINT32 width,
                      int ( *comp)(const void *, const void *));
static void swap_kn(char *a, char *b, UINT32 width);
/* this parameter defines the cutoff between using quick sort and
   insertion sort for arrays; arrays with lengths shorter or equal to the
   below value use insertion sort */

/* 这个参数定义的作用是，当快速排序的循环中遇到大小小于CUTOFF的数组时，就使用插入

排序来进行排序，这样就避免了对小数组继续拆分而带来的额外开销。这里的取值8，是

经过测试以后能够时快速排序算法达到最快的CUTOFF的值。*/

#define CUTOFF 8            /* testing shows that this is good value */



/* 源代码中这里是qsort的代码，但是我觉得先解释了qsort要调用的函数的功能比较

好。

    shortsort函数：

    这个函数的作用，上面已经有提到。就是当对快速排序递归调用的时候，如果遇到

大小小于CUTOFF的数组，就调用这个函数来进行排序，而不是继续拆分数组进入下一层

递归。因为虽然这里用的是基本排序方法，它的运行时间和O(n^2)成比例，但是如果是

只有8个元素，它的速度比需要递归的快速排序要快得多。另外，在源代码的注释中，说

这是一个插入排序(insertion sort)，但是我觉得这个应该是一个选择排序才对

(selection sort)。至于为什么用选择排序而不用插入排序，应该是和选择排序的元素

交换次数少有关系，只需要N-1次交换，而插入排序平均需要(N^2)/2次。之所以要选择

交换次数少的算法，是因为有可能数组里面的单个元素的大小很大，使得交换成为最主

要的性能瓶颈。

    参数说明：

       char *lo;    指向要排序的子数组的第一个元素的指针

       char *hi;    指向要排序的子数组的最后一个元素的指针

       UINT32 width;  数组中单个元素的大小

       int (__cdecl *comp)(const void *,const void *);   用来比较两个元素大

小的函数指针，这个函数是你在调用qsort的时候传入的参数，当前一个指针指向的元素

小于后一个时，返回负数；当相等时，返回0；当大于时，返回正数。*/

static void shortsort (
    char *lo,
    char *hi,
    UINT32 width,
    int (*comp)(const void *, const void *)
)
{
    char *p, *max;

    /* Note: in assertions below, i and j are alway inside original bound of
       array to sort. */

    while (hi > lo)
    {
        /* A[i] <= A[j] for i <= j, j > hi */
        max = lo;

        /*下面这个for循环作用是从lo到hi的元素中，选出最大的一个，max指针指向这

        个最大项*/
        for (p = lo + width; p <= hi; p += width)
        {
            /* A[i] <= A[max] for lo <= i < p */
            if (comp(p, max) > 0)
            {
                max = p;
            }

            /* A[i] <= A[max] for lo <= i <= p */
        }

        /* A[i] <= A[max] for lo <= i <= hi */

        /*这里把最大项和hi指向的项向交换*/
        swap_kn(max, hi, width);

        /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

        /*hi向前移动一个指针。经过这一步，在hi后面的是已经排好序的比未排序部分

        所有的数要大的数。*/

        hi -= width;

        /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
    }

    /* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
       so array is sorted */
}



/*下面分析swap函数：

      这个函数比较简单，就是交换两个项的操作，不过是用指针来实现的。

*/

static void swap_kn (
    char *a,
    char *b,
    UINT32 width
)
{
    char tmp;

    if ( a != b )

        /* Do the swap one character at a time to avoid potential alignment
           problems. */
        while ( width-- )
        {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
}



/*下面是最重要的部分，qsort函数：*/
#ifdef VFMW_BVT_SUPPORT

 void qsort ( void *base, UINT32 num, UINT32 width, SINT32 (*comp)(const void *, const void *) )
{
    char *lo, *hi; 			 /* 数组的两端项指针，用来指明数组的上界和下界 */

    /*如果只有一个或以下的元素，则退出*/
    if (num < 2 || width == 0)
    {
        return;
    }

    lo = base;
    hi = (char *)base + width * (num - 1);		 /* initialize limits */
    shortsort(lo, hi, width, comp);
}
#else

/*使用的是非递归方式，所以这里有一个自定义的栈式结构，下面这个定义是栈的大小

*/

#define STKSIZ (8*sizeof(void*) - 2)

void qsort (
    void *base,
    UINT32 num,
    UINT32 width,
    int (*comp)(const void *, const void *)
)
{
    /* Note: the number of stack entries required is no more than
       1 + log2(num), so 30 is sufficient for any array */

    /*由于使用了某些技巧（下面会讲到），使得栈大小的需求不会大于1+log2(num)，

    因此30的栈大小应该是足够了。为什么说是30呢？其实在上面STKSIZ的定义中可以计算出sizeof(void*)＝4，所以8*4-2＝30*/
    char *lo, *hi;              /* ends of sub-array currently sorting   数组

的两端项指针，用来指明数组的上界和下界*/
    char *mid;                  /* points to middle of subarray  数组的中间项指

针*/
    char *loguy, *higuy;        /* traveling pointers for partition step  循

环中的游动指针*/
    UINT32 size;                /* size of the sub-array  数组的大小*/
    char *lostk[STKSIZ], *histk[STKSIZ];
    int stkptr;                 /* stack for saving sub-array to be processed  栈顶指针

*/



    /*如果只有一个或以下的元素，则退出*/

    if (num < 2 || width == 0)
    { return; }                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = base;
    hi = (char *)base + width * (num - 1);      /* initialize limits */

    /* this entry point is for pseudo-recursion calling: setting
       lo and hi and jumping to here is like recursion, but stkptr is
       preserved, locals aren't, so we preserve stuff on the stack */

    /*这个标签是伪递归的开始*/
recurse:

    size = (hi - lo) / width + 1;        /* number of el's to sort */

    /* below a certain size, it is faster to use a O(n^2) sorting method */

    /*当size小于CUTOFF时，使用O(n2)的排序算法更快*/
    if (size <= CUTOFF)
    {
        shortsort(lo, hi, width, comp);
    }
    else
    {
        /* First we pick a partitioning element.  The efficiency of the
           algorithm demands that we find one that is approximately the

        median
           of the values, but also that we select one fast.  We choose the
           median of the first, middle, and last elements, to avoid bad
           performance in the face of already sorted data, or data that is

        made
           up of multiple sorted runs appended together.  Testing shows that

        a
           median-of-three algorithm provides better performance than simply
           picking the middle element for the latter case. */

        /*首先我们要选择一个分区项。算法的高效性要求我们找到一个近似数组中间值

        的项，但我们要保证能够很快找到它。我们选择数组的第一项、中间项和最后一项的中

        间值，来避免最坏情况下的低效率。测试表明，选择三个数的中间值，比单纯选择数组

        的中间项的效率要高。

         我们解释一下为什么要避免最坏情况和怎样避免。在最坏情况下，快速排序算法

        的运行时间复杂度是O(n^2)。这种情况的一个例子是已经排序的文件。如果我们选择最

        后一个项作为划分项，也就是已排序数组中的最大项，我们分区的结果是分成了一个大

        小为N-1的数组和一个大小为1的数组，这样的话，我们需要的比较次数是N + N-1 + N-2

        + N-3 +...+2+1=(N+1)N/2=O(n^2)。而如果选择前 中 后三个数的中间值，这种最坏情况的

        数组也能够得到很好的处理。*/

        mid = lo + (size / 2) * width;      /* find middle element */

        /*第一项 中间项 和最后项三个元素排序*/

        /* Sort the first, middle, last elements into order */
        if (comp(lo, mid) > 0)
        {
            swap_kn(lo, mid, width);
        }

        if (comp(lo, hi) > 0)
        {
            swap_kn(lo, hi, width);
        }

        if (comp(mid, hi) > 0)
        {
            swap_kn(mid, hi, width);
        }

        /* We now wish to partition the array into three pieces, one

        consisting
           of elements <= partition element, one of elements equal to the
           partition element, and one of elements > than it.  This is done
           below; comments indicate conditions established at every step. */

        /*下面要把数组分区成三块，一块是小于分区项的，一块是等于分区项的，而

        另一块是大于分区项的。*/

        /*这里初始化的loguy 和 higuy两个指针，是在循环中用于移动来指示需要交换的两个元素的。higuy递减，loguy递增，所以下面的for循环总是可以终止。*/

        loguy = lo;
        higuy = hi;

        /* Note that higuy decreases and loguy increases on every iteration,
           so loop must terminate. */
        for (;;)
        {
            /* lo <= loguy < hi, lo < higuy <= hi,
               A[i] <= A[mid] for lo <= i <= loguy,
               A[i] > A[mid] for higuy <= i < hi,
               A[hi] >= A[mid] */

            /* The doubled loop is to avoid calling comp(mid,mid), since some
               existing comparison funcs don't work when passed the same
               value for both pointers. */

            /*开始移动loguy指针，直到A[loguy]>A[mid]*/

            if (mid > loguy)
            {
                do
                {
                    loguy += width;
                }
                while (loguy < mid && comp(loguy, mid) <= 0);
            }

            /*如果移动到loguy>=mid的时候，就继续向后移动，使得A[loguy]>a

            [mid]。这一步实际上作用就是使得移动完loguy之后，loguy指针之前的元素都是不大于划分值的元素。*/
            if (mid <= loguy)
            {
                do
                {
                    loguy += width;
                }
                while (loguy <= hi && comp(loguy, mid) <= 0);
            }

            /* lo < loguy <= hi+1, A[i] <= A[mid] for lo <= i < loguy,
               either loguy > hi or A[loguy] > A[mid] */

            /*执行到这里的时候，lo<loguy<=hi+1，

              对所有lo<=i<loguy，有A[i]<=A[mid]，

              或者loguy>hi成立，或者A[loguy]>A[mid]成立

             也就是说，loguy指针之前的项都比A[mid]要小或者等于它*/



            /*下面移动higuy指针，直到A[higuy]<＝A[mid]*/

            do
            {
                higuy -= width;
            }
            while (higuy > mid && comp(higuy, mid) > 0);

            /* lo <= higuy < hi, A[i] > A[mid] for higuy < i < hi,
               either higuy == lo or A[higuy] <= A[mid] */



            /*如果两个指针交叉了，则退出循环。*/

            if (higuy < loguy)
            { break; }

            /* if loguy > hi or higuy == lo, then we would have exited, so
               A[loguy] > A[mid], A[higuy] <= A[mid],
               loguy <= hi, higuy > lo */

            /*如果loguy>hi 或者higuy==lo，则上面一条break语句已经成立，我们已

            经跳出。

              因此，此时A[loguy]>A[mid],A[higuy]<=A[mid],

             loguy<=hi，higuy>lo。*/

            /*交换两个指针指向的元素*/

            swap_kn(loguy, higuy, width);

            /* If the partition element was moved, follow it.  Only need
               to check for mid == higuy, since before the swap,
               A[loguy] > A[mid] implies loguy != mid. */

            /*如果划分元素的位置移动了，我们要跟踪它。

               因为在前面对loguy处理的两个循环中的第二个循环已经保证了loguy>mid,

              即loguy指针不和mid指针相等。

              所以我们只需要看一下higuy指针是否等于mid指针，

             如果原来是mid==higuy成立了，那么经过刚才的交换，中间值项已经到了

            loguy指向的位置（注意：刚才是值交换了，但是并没有交换指针。当higuy和mid相等，交换higuy和loguy指向的内容，higuy依然等于mid），所以让mid＝loguy，重新跟踪中间值。*/

            if (mid == higuy)
            { mid = loguy; }

            /* A[loguy] <= A[mid], A[higuy] > A[mid]; so condition at top
               of loop is re-established */

            /*这个循环一直进行到两个指针交叉为止*/
        }

        /*     A[i] <= A[mid] for lo <= i < loguy,
               A[i] > A[mid] for higuy < i < hi,
               A[hi] >= A[mid]
               higuy < loguy
           implying:
               higuy == loguy-1
               or higuy == hi - 1, loguy == hi + 1, A[hi] == A[mid] */

        /*上一个循环结束之后，因为还没有执行loguy指针和higuy指针内容的交换，所以loguy指针的前面的数组元素都不大于划分值，而higuy指针之后的数组元素都大于划分值，所以此时有两种情况：

        1)  higuy＝loguy－1

        2)  higuy＝hi－1，loguy＝hi+1

        其中第二种情况发生在一开始选择三个元素的时候，hi指向的元素和mid指向的元素值相等，而hi前面的元素全部都不大于划分值，使得移动loguy指针的时候，一直移动到了hi+1才停止，再移动higuy指针的时候，higuy指针移动一步就停止了，停在hi－1处。

        */

        /* Find adjacent elements equal to the partition element.  The
           doubled loop is to avoid calling comp(mid,mid), since some
           existing comparison funcs don't work when passed the same value
           for both pointers. */

        higuy += width;

        if (mid < higuy)
        {
            do
            {
                higuy -= width;
            }
            while (higuy > mid && comp(higuy, mid) == 0);
        }

        if (mid >= higuy)
        {
            do
            {
                higuy -= width;
            }
            while (higuy > lo && comp(higuy, mid) == 0);
        }

        /* OK, now we have the following:
              higuy < loguy
              lo <= higuy <= hi
              A[i]  <= A[mid] for lo <= i <= higuy
              A[i]  == A[mid] for higuy < i < loguy
              A[i]  >  A[mid] for loguy <= i < hi
              A[hi] >= A[mid] */

        /*经过上面的处理，higuy指针和之前的都是小于等于A[mid]的数，而higuy指针

        和loguy指针之间的是等于A[mid]的数，而loguy指针和之后的是大于A[mid]的数。实际上我们可以看到，higuy指针前面仍然可能有等于A[mid]的数，但是这样的三路划分之后，确实能够在一定程度上面减少子数组的大小。优化了程序的效率。*/

        /* We've finished the partition, now we want to sort the subarrays
           [lo, higuy] and [loguy, hi].
           We do the smaller one first to minimize stack usage.
           We only sort arrays of length 2 or more.*/

        /*我们现在已经完成了分区，可以开始对子数列[lo,higuy]和[loguy,hi]的排序

        。

          我们先处理小的那个数列，这样可以避免最坏情况下栈大小和N成比例的情况

        。

          我们可以想像一下，对于一个已经排序的数组，如果每次分成N-1和1的数组，

         而我们又每次都先处理N-1那一半，

         那么我们的递归深度就是和N成比例，这样对于大N，栈空间的开销是很大的。

         如果先处理1的那一半，栈里面最多只有2项。

         当划分元素刚好在数组中间时，栈的长度是logN。

          对于栈的操作，就是先把大的数组信息入栈。

        */

        if ( higuy - lo >= hi - loguy )
        {
            if (lo < higuy)
            {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }                           /* save big recursion for later */

            if (loguy < hi)
            {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else
        {
            if (loguy < hi)
            {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }

            if (lo < higuy)
            {
                hi = higuy;
                goto recurse;           /* do small recursion */
            }
        }
    }

    /* We have sorted the array, except for any pending sorts on the stack.
       Check if there are any, and do them. */

    /*出栈操作，直到栈为空，退出循环*/

    --stkptr;

    if (stkptr >= 0)
    {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
    {
        return;
    }                 /* all subarrays done */
}
#endif



