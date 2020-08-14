#ifndef     _COMMON_HIGO_H
#define     _COMMON_HIGO_H

extern HI_HANDLE hLayer, hLayer_sd,hLayerSurface;

HI_S32 Higo_Initial(HIGO_LAYER_FLUSHTYPE_E type);
HI_VOID Higo_Deinitial();
HI_S32 Higo_Dec(HI_CHAR *pszFileName, HI_U32 index, HI_HANDLE *pSurface,
	HIGO_DEC_PRIMARYINFO_S *pPrimaryInfo,HIGO_DEC_IMGINFO_S *pImgInfo);
HI_S32 Higo_DecWithAttr(HI_CHAR *pszFileName, HI_U32 index, HI_HANDLE *pSurface,
	HIGO_DEC_IMGATTR_S *pImgAttr);

#endif
