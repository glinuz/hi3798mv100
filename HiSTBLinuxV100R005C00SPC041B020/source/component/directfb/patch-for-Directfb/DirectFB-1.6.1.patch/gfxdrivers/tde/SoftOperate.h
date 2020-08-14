
#ifndef SGO_SOFTOPERATE_H
#define SGO_SOFTOPERATE_H


#include "hi_type.h"
#include "hi_tde_type.h"
#include "hi_tde_api.h"

typedef enum _SGO_RETVAL_TYPE_E
{

	SGO_SUCCESS = 0,                                     
	SGO_NULLADDR_SURFACE,                         
	SGO_NOTSUPPORT_COLORFORMAT,       
	SGO_NULLADDR_SURBUFFER ,                
	SGO_TOOLARGE_REGION,
	SGO_ERR_ALUCMD,                 
	SGO_ERR_COLORKEYMODE	,
	SGO_ERR_REGION,
	SGO_NOTSUPPORT_OPERATION
}SGO_RETVAL_TYPE_E;




/** 
\brief Fill rectangle CNcomment:矩形填充
\attention \n
Support colorkey,alpha,ROP,Colorkey+ROP operation
CNcomment:可以支持操作如下colorkey、alpha、ROP、colorkey+ROP等组合操作\

\param[in] pDstSurface  Dst surface     CNcomment:目标surface
\param[in] pRect  Fill region   CNcomment:填充区域
\param[in] u32FillColor  Fill color value   CNcomment:填充颜色
\param[in] pBlitOpt operation config    CNcomment:混合方式

\retval ::SGO_SUCCESS 
\retval ::
\see \n
none
CNcomment:无
*/
HI_S32 Hi_Soft_FillRect(TDE2_SURFACE_S* pDstSurface,  TDE2_RECT_S* pRect, HI_U32  u32FillColor, TDE2_OPT_S* pBlitOpt);



/** 
\brief 
Surface blit,support CSC(Color space change)
CNcomment:位块搬移，在搬移过程中，可以实现色彩空间转换、
\attention \n
CSC only supports YUV to RGB
Support colorkey,alpha,ROP,colorkey+ROP opreation
CNcomment:色彩空间转换仅支持YUV到RGB转换 \n
可以支持操作如下colorkey、alpha、ROP、colorkey+ROP等组合操作\


\param[in] pSrcSurface  Src surface CNcomment:源surface
\param[in] pSrcRect Dst region,NULL specify surface size
                            CNcomment:搬移的源区域，为空表示整个源surface区域
\param[in] pDstSurface  Dst surface     CNcomment:目标surface
\param[in] pDstRect Dst region,NULL specify surface size
                            CNcomment:搬移的目的区域，为空表示整个目标surface区域
\param[in]  pBlitOpt Operation config       CNcomment:混合方式

\retval ::SGO_SUCCESS 
\retval ::
\see \n
none
CNcomment:无
*/
HI_S32 Hi_Soft_Blit( TDE2_SURFACE_S* pSrcSurface,  TDE2_RECT_S *pSrcRect,  TDE2_SURFACE_S* pDstSurface,  TDE2_RECT_S *pDstRect,  TDE2_OPT_S* pBlitOpt);


/** 
\brief 
Surface blit,support CSC(Color space change) and scale
CNcomment:位块搬移，在搬移过程中，可以实现色彩空间转换,缩放
\attention \n
CSC only supports YUV to RGB
Support colorkey,alpha,ROP,colorkey+ROP opreation
CNcomment:色彩空间转换仅支持YUV到RGB转换 \n
可以支持操作如下colorkey、alpha、ROP、colorkey+ROP等组合操作\


\param[in] pSrcSurface  Src surface CNcomment:源surface
\param[in] pSrcRect Dst region,NULL specify surface size
                            CNcomment:搬移的源区域，为空表示整个源surface区域
\param[in] pDstSurface  Dst surface     CNcomment:目标surface
\param[in] pDstRect Dst region,NULL specify surface size
                            CNcomment:搬移的目的区域，为空表示整个目标surface区域
\param[in]  pBlitOpt Operation config       CNcomment:混合方式

\retval ::SGO_SUCCESS 
\retval ::
\see \n
none
CNcomment:无
*/
HI_S32 Hi_Soft_StretchBlit( TDE2_SURFACE_S* pSrcSurface,  TDE2_RECT_S *pSrcRect,  TDE2_SURFACE_S* pDstSurface,  TDE2_RECT_S *pDstRect,   TDE2_OPT_S* pBlitOpt);


#endif
