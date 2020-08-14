
#ifndef __IMG_QUE_HEADER__
#define  __IMG_QUE_HEADER__

#include "basedef.h"
#include "vfmw.h"


#define IMGQUE_MAX_IMAGE_NUM    64
#define IMGQUE_MAX_NAME_LEN     16

typedef struct hiImageQueue
{
    SINT8      queue_name[IMGQUE_MAX_NAME_LEN];    //如果生成了多个实例，用这个来标识
    SINT8      ImageValid[IMGQUE_MAX_IMAGE_NUM];   // 0(00b): 均无效,
    // 1(01b): 顶场有效,
    // 2(10b): 底场有效，
    // 3(11b): 均有效
    IMAGE      *image_ptr[IMGQUE_MAX_IMAGE_NUM];
    SINT32     max_queue_size;
    SINT32     queue_size;
    SINT32     history;
    SINT32     head;
    SINT32     tail;
    SINT32     ResetCount;
    UINT32     queue_mutex;
    UINT32     last_image_id_plus2;
    SINT32     (*DelImageProc)(struct hiImageQueue *pImageQueue, SINT32 ImageID);
} IMAGE_QUEUE;


/*********************************************************************************
    函数原型
        SINT32 IMGQUE_InitQueue( IMAGE_QUEUE *pImageQueue )
    功能描述
        初始化图像队列
    参数说明
        pImageQueue: 图象队列指针
		QueueName:   图象队列的名字，16个字符以内
    返回值
        如果初始化成功返回VF_OK，否则返回错误码
 *********************************************************************************/
SINT32 IMGQUE_InitQueue( IMAGE_QUEUE *pImageQueue, SINT8 *QueueName,
                         SINT32 (*DelImageProc)(struct hiImageQueue *pImageQueue, SINT32 ImageID) );

/*********************************************************************************
    函数原型
        SINT32 IMGQUE_ResetQueue( IMAGE_QUEUE *pImageQueue )
    功能描述
        初始化图像队列
    参数说明
        pImageQueue: 图象队列指针
    返回值
        如果初始化成功返回VF_OK，否则返回错误码
 *********************************************************************************/
SINT32 IMGQUE_ResetQueue( IMAGE_QUEUE *pImageQueue );

/*********************************************************************************
    函数原型
        SINT32 IMGQUE_InsertImage(IMAGE_QUEUE *pImageQueue, IMAGE *p_image)
    功能描述
        将指定的图像插入到图象队列中
    参数说明
        pImageQueue: 图象队列指针
        p_image：待插入队列的图像描述
    返回值
        如果插入成功则返回VF_OK，否则返回错误码
    算法描述
 *********************************************************************************/
SINT32 IMGQUE_InsertImage(IMAGE_QUEUE *pImageQueue, IMAGE *p_image);

/*********************************************************************************
    函数原型
        VOID IMGQUE_GetImageNum(IMAGE_QUEUE *pImageQueue, SINT32 *pReadImgNum, SINT32 *pNewImgNum)
    功能描述
        获取队列中的图像数目
    参数说明
        pImageQueue: 图象队列指针
        pReadImgNum: 已被读走，但尚未释放的图像个数( history ~ head )
        pNewImgNum:  已插入队列还没有被读走的图像个数( head ~ tail )
    返回值
        无
    算法描述
 *********************************************************************************/
VOID IMGQUE_GetImageNum(IMAGE_QUEUE *pImageQueue, SINT32 *pReadImgNum, SINT32 *pNewImgNum);

SINT32 IMGQUE_ReturnImage(IMAGE_QUEUE *pImageQueue);

/*********************************************************************************
    函数原型
        SINT32 IMGQUE_GetImage(IMAGE_QUEUE *pImageQueue, IMAGE *p_image)
    功能描述
        从VO队列中获取一幅图像
    参数说明
        pImageQueue: 图象队列指针
        p_image:     IMAGE类型的指针，接收获取到的图像的信息
    返回值
        如果获取成功则返回VF_OK，否则返回错误码
    算法描述
 *********************************************************************************/
SINT32 IMGQUE_GetImage(IMAGE_QUEUE *pImageQueue, IMAGE *p_image);

/*********************************************************************************
    函数原型
        SINT32 IMGQUE_DeleteImage(IMAGE_QUEUE *pImageQueue, SINT32 ImageID, SINT32 *pDelTopAddr, SINT32 *pDelBtmAddr)
    功能描述
        释放一幅图像。系统软件调用此函数表明p_image所描述的图像已经使用，不再需
        要保存，其存储空间可以被后续解码过程重新分配
    参数说明
        pImageQueue: 图象队列指针
        p_image：IMAGE类型的指针，指示要释放的图像的信息
    返回值
        如果释放成功则返回VF_OK，否则返回错误码
    算法描述
 *********************************************************************************/
SINT32 IMGQUE_DeleteImage(IMAGE_QUEUE *pImageQueue, SINT32 ImageID, SINT32 *pDelTopAddr, SINT32 *pDelBtmAddr);

VOID print_que(IMAGE_QUEUE *pImageQueue);

SINT32 IMGQUE_GetLastImageID(IMAGE_QUEUE *pImageQueue);

#endif



