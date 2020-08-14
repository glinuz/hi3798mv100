/**
 * @file sre_err.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：错误处理头文件。 \n
 */

/**
*@defgroup  SRE_err  错误处理
*@ingroup SRE_inspect
*/

/**
*@defgroup  SRE_cda  CDA
*@ingroup SRE_inspect
*/
/**
*@defgroup  SRE_cda  CDA
*@ingroup SRE_inspect
*/

#ifndef _SRE_ERR_H
#define _SRE_ERR_H
#include "sre_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/**
 * @ingroup SRE_cda
 * CDA记录信息中任务栈或各个内存分区的起始地址和大小。
 */
typedef struct tagPtInfo {
	UINT32 uwAddr;                  /**< 内存分区起始地址   */
	UINT32 uwSize;                  /**< 内存分区的大小     */
} CDA_PT_INFO_S;

/**
*@ingroup SRE_err
*
*@brief 错误处理钩子函数。
*
*@par 描述:
*错误处理的钩子函数，用户自定义的错误处理函数必须要符合该钩子函数的参数要求。
*
*@attention 无
*
*@param pcFileName [IN] 类型#CHAR *，出错文件名。
*@param uwLineNo [IN] 类型#UINT32 ，出错的行号。
*@param uwErrorNo [IN] 类型#UINT32，用户定义的错误码。
*@param uwParaLen [IN] 类型#UINT32，入参pPara的长度。
*@param pPara [IN] 类型#VOID *，记录用户对于错误的描述或其他。
*
*@retval 无
*@par 依赖:
*<li>sre_err.h：该接口声明所在的头文件。</li>
*@since UniDSP V100R001C01
*@see 无
*/
typedef VOID (*SRE_ERRORHANDLE_FUNC)(CHAR    *pcFileName,
				     UINT32   uwLineNo,
				     UINT32   uwErrorNo,
				     UINT32   uwParaLen,
				     VOID     *pPara);

/**
*@ingroup SRE_err
*
*@brief 注册用户错误处理的钩子函数。
*
*@par 描述:
*注册pfnHook作为用户钩子函数，在错误处理时对该钩子进行调用。
*@attention 用户定义的钩子函数必须符合#SRE_ERRORHANDLE_FUNC定义的形式，而且只能定义一个钩子函数。
*<ul>
<li>若入参pfnHook为NULL,则为取消钩子。
<li>重复注册，则以最后一次注册钩子为准。
*</ul>
*
*@param pfnHook [IN] 类型#SRE_ERRORHANDLE_FUNC，用户钩子函数的宏定义。
*
*@retval 无
*@par 依赖:
*<li>sre_err.h：该接口声明所在的头文件。</li>
*@since UniDSP V100R001C01
*@see 无
*/
extern VOID SRE_ErrRegHook(SRE_ERRORHANDLE_FUNC pfnHook);

/**
*@ingroup SRE_err
*
*@brief 处理错误。
*
*@par 描述:
*用户调用该函数，则回调用户SRE_ErrRegHook中注册的钩子函数。
*@attention
* <ul>
* <li>系统不会做入参检测，用户SRE_ErrHandle会全部当做钩子入参输入。
* </ul>
*
*@param pcFileName [IN] 类型#CHAR *，错误发生的文件名，可以用__FILE__作为输入。
*@param uwLineNo [IN] 类型#UINT32，错误发生所在的行号，可以用__LINE__作为输入。
*@param uwErrorNo [IN] 类型#UINT32，用户输入的错误码。
*@param uwParaLen [IN] 类型#UINT32，入参pPara的长度。
*@param pPara [OUT] 类型#VOID *，记录用户对于错误的描述或其他。
*
*@retval #SRE_OK      0x00000000，处理错误成功。
*@par 依赖:
*<li>sre_err.h: 该接口声明所在的头文件。</li>
*@since UniDSP V100R001C01
*@see 无
*/
extern UINT32 SRE_ErrHandle(CHAR *pcFileName, UINT32 uwLineNo, UINT32 uwErrorNo, UINT32 uwParaLen, VOID *pPara);

/**
*@ingroup SRE_cda
*
*@brief 获取CDA分区信息函数。
*
*@par 描述:
*获取任务栈起始地址和长度(独立配置时有效)、以及当前核创建的所有分区起始地址以及长度。
*@attention
* <ul>
* <li>pstCdaPtInfo须是一个数组，数组类型是CDA_PT_INFO_S，为防止数组写越界，请确保数组空间足够。
* <li>此数组存放元属顺序为：用户配置的任务栈+os默认创建的分区+用户配置的分区。详见CDA使用实例。
* <li>入参pstCdaPtInfo如果是Null，接口不会返回错误码，不做处理返回成功，
* </ul>
*
*@param pstCdaPtInfo [OUT] 类型#CDA_PT_INFO_S *，保存任务栈和内存分区的首地址与长度。
*
*@retval 无
*@par 依赖:
*<li>sre_err.h: 该接口声明所在的头文件。</li>
*@since UniDSP V100R001C01
*@see 无
*/
extern VOID SRE_CdaInfoGet(CDA_PT_INFO_S *pstCdaPtInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_ERR_H */

/*
 * History: \n
 * 2009-3-13, l00133294, Create this file. \n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */
