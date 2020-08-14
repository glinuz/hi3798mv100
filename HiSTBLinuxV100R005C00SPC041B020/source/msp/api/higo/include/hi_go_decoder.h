/**
 \file
 \brief Describes the header file of the decoder.CNcomment:Decoder模块头文件 CNend
 \author Shenzhen Hisilicon Co., Ltd.
 \version draft
 \author 1.0
 \date 2008-5-16
 */

#ifndef __HI_GO_DECODE_H__
#define __HI_GO_DECODE_H__

#include "hi_type.h"
#include "hi_go_comm.h"
#include "hi_go_surface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/*************************** Structure Definition ****************************/
/** \addtogroup      HIGO_DEC */
/** @{ */  /** <!-- [HIGO_DEC] */

/** Decoding type of the decoding instance*//** CNcomment: 解码实例的解码类型 */
typedef enum 
{
    HIGO_DEC_IMGTYPE_JPEG = HIGO_IMGTYPE_JPEG, /**<.jpeg picture*//**<CNcomment: JPEG格式图片*/
    HIGO_DEC_IMGTYPE_GIF  = HIGO_IMGTYPE_GIF,  /**<.gif picture*//**<CNcomment: GIF格式图片*/
    HIGO_DEC_IMGTYPE_BMP  = HIGO_IMGTYPE_BMP, /**<.bmp picture*//**<CNcomment: BMP格式图片 */
    HIGO_DEC_IMGTYPE_PNG  = HIGO_IMGTYPE_PNG,  /**<.png picture*//**<CNcomment: PNG格式图片 */
    HIGO_DEC_IMGTYPE_RLE  = HIGO_IMGTYPE_RLE,  /**<.rle picture*//**<CNcomment: RLE格式图片 */
    HIGO_DEC_IMGTPYE_BUTT
} HIGO_DEC_IMGTYPE_E;

/**Stream source*//** CNcomment: 码流的来源 */
typedef enum 
{
    HIGO_DEC_SRCTYPE_FILE = 0, /**<Streams are derived from files.*//**<CNcomment: 码流来源于文件*/
    HIGO_DEC_SRCTYPE_MEM,      /**<Streams are derived from the memory.*//**<CNcomment: 码流来源于内存*/
    HIGO_DEC_SRCTYPE_STREAM,   /**<Streams are derived from stream data.*/  /**<CNcomment: 码流来源于流式数据*/    
    HIGO_DEC_SRCTYPE_BUTT
} HIGO_DEC_SRCTYPE_E;

typedef enum 
{
    HIGO_DEC_POSITION_SET = 0,   /*Start position*//** CNcomment:起始位置*/
    HIGO_DEC_POSITION_CUR,       /*From the current position*//** CNcomment:从当前位置*/
    HIGO_DEC_POSITION_END,       /*From the end position*//** CNcomment:从结束位置*/
    HIGO_DEC_POSITION_BUTT
}HIGO_DEC_POSITION_E;

typedef struct
{
	HIGO_MEMTYPE_E MemType;
	HI_BOOL IsPubPalette;  /*Decoder uses common Palette or not*//** CNcomment:解码图片是否使用公共调色板,只对输出格式有效*/
}HIGO_DEC_SURINFO_S;


typedef HI_S32 (*PTR_Dec_WriteStreamCB_FN)(HIGO_DEC_POSITION_E Position,  HI_S32 Offset);


/**Detailed information about the stream source*//** CNcomment:码流来源的详细信息*/
typedef union 
{
    /*The following information is required if the input source is the memory.*//** CNcomment:输入源为内存块时需要的信息*/
    struct
    {
        HI_CHAR* pAddr;      /**<Memory pointer address*//**<CNcomment: 内存指针地址*/
        HI_U32 Length;       /**<Length*//**<CNcomment: 长度*/
    } MemInfo;

    /*Picture name*//** CNcomment:图片文件名*/
    const HI_CHAR *pFileName;
    PTR_Dec_WriteStreamCB_FN pWriteCB;
} HIGO_DEC_SRCINFO_U;

/**Parameters for creating a decoding instance.*//** CNcomment: 创建解码实例需要的参数 */
typedef struct 
{
    HIGO_DEC_SRCTYPE_E SrcType; /**<Type of the input stream.*//**<CNcomment: 输入码流的类型*/
    HIGO_DEC_SRCINFO_U SrcInfo; /**<Information about the input source.*//**<CNcomment: 输入源详细信息*/
} HIGO_DEC_ATTR_S;

/** Major information after decoding*//** CNcomment: 解码后主信息 */
typedef struct 
{
    HI_U32             Count;           /**<Number of pictures*//**<CNcomment: 图片的个数 */
    HIGO_DEC_IMGTYPE_E ImgType;         /**<Type of the input source.*//**<CNcomment: 输入源类型 */
    HI_U32             ScrWidth;        /**<Screen width. The screen refers to that of the picture. The value 0 indicates "invalid".*//**<CNcomment: 屏幕宽度，此处屏幕表示图片文件屏幕，0表示无效 */
    HI_U32             ScrHeight;       /**<Screen height. The screen refers to that of the picture. The value 0 indicates "invalid".*//**<CNcomment: 屏幕高度，此处屏幕表示图片文件屏幕，0表示无效 */
    HI_BOOL            IsHaveBGColor;   /**<Whether there is any background color.*//**<CNcomment: 是否有背景色 */
    HI_COLOR           BGColor;         /**<Background color*//**<CNcomment: 背景颜色 */
} HIGO_DEC_PRIMARYINFO_S;

/** Index picture format*//** CNcomment: 索引图片格式 */
typedef struct 
{
    HI_U32    OffSetX;                /**<X offset value on the screen. The screen mentioned in this document refers to that of the picture.*//**<CNcomment: 在屏幕上的X偏移量，此处屏幕表示图片文件屏幕，下同 */
    HI_U32    OffSetY;                /**<Y offset value on the screen.*//**<CNcomment: 在屏幕上的Y偏移量*/
    HI_U32    Width;                  /**<Width of the original picture*//**<CNcomment: 原始图片的宽度 */
    HI_U32    Height;                  /**<Height of the original picture*//**<CNcomment: 原始图片的高度 */
    HI_U8     Alpha;                  /**<Information about channel alpha used for overlaying pictures. The alpha value ranges from 0 to 255.*//**<CNcomment: 用于图片进行叠加的通道alpha信息，范围是0-255 */
    HI_BOOL   IsHaveKey;              /**<Key enable. HI_FALSE: disabled; HI_TRUE: enabled*//**<CNcomment: 是否有KEY。 HI_FALSE: KEY不使能；HI_TRUE: KEY使能 */
    HI_COLOR  Key;                    /**<Information about colorkey used for overlaying pictures*//**<CNcomment: 用于图片进行叠加的colorkey信息 */
    HIGO_PF_E Format;                 /**<Pixel format of the picture source*//**<CNcomment: 图片源像素格式 */
    HI_U32    DelayTime;              /**<Time interval between this picture and the previous one (10 ms). The value 0 indicates that the parameter is invalid.*//**<CNcomment: 与上一幅图片的时间间隔(10ms)，0表示该参数无效 */
    HI_U32    DisposalMethod;         /**<Processing method of the current frame. This application programming interface (API) is valid for only GIF pictures. 0: customized processing method; 1: no processing method; 2: use the background color.*/    /**<CNcomment: 当前帧处理方法，此接口只对GIF有效，0:用户决定处理方式，1:不做处理，2:使用背景色，*/    
} HIGO_DEC_IMGINFO_S;

/* Picture attributes after decoding*//** CNcomment: 解码后图片属性 */
typedef struct 
{
    HI_U32    Width;                  /**<Expected width after decoding*//**<CNcomment: 期望解码后宽度 */
    HI_U32    Height;                 /**<Expected height after decoding*//**<CNcomment: 期望解码后高度*/
    HIGO_PF_E Format;                 /**<Expected pixel format after decoding*//**<CNcomment: 期望解码后像素格式*/
} HIGO_DEC_IMGATTR_S;

/** Extended data format type*//** CNcomment: 扩展数据格式类型 */
typedef enum 
{
    HIGO_DEC_EXTEND_EXIF = 0,          /**<Extended-data in EXIF format*//**<CNcomment: EXIF格式的扩展数据 */
    HIGO_DEC_EXTEND_EXIF_BUTT
} HIGO_DEC_EXTENDTYPE_E;

/** Basic picture information*//** CNcomment: 图片基本信息 */
typedef struct 
{
    HI_U32    Width;                /**<Picture width*//**<CNcomment: 图片的宽度 */
    HI_U32    Height;               /**<Picture height*//**<CNcomment: 图片的高度 */
    HIGO_PF_E PixelFormat;          /**<Pixel format*//**<CNcomment: 像素格式 */
    HI_U32    VirAddr[3];           /**<Virtual address. If the address is in RGB format, only the first VirAddr[0] is the start address,
                                                and VirAddr[1] and VirAddr[2] are invalid. If the address is in YC format,
                                                VirAddr[0] indicates the start address of the Y component, VirAddr[1] indicates the start address of the C component, 
                                                and VirAddr[2] is invalid.*/
                                                /**<CNcomment: 虚拟地址。如果是RGB格式，只有第一个VirAddr[0]为起始地址，
                                                VirAddr[1]和VirAddr[2]无效；如果是YC格式，
                                                VirAddr[0]表示Y分量起始地址，VirAddr[1]表示C分量起始地址，
                                                VirAddr[2]无效 */
    HI_U32    PhyAddr[3];           /**<Physical address. The usage principles are the same as previous ones.*//**<CNcomment: 物理地址，使用原则同上 */
    HI_U32    Pitch[3];             /**<Pitch. The usage principles are the same as previous ones.*//**<CNcomment: 行间距，使用原则同上 */
    HI_U32    Palate[256];         /**<Palette*//**<CNcomment: 调色板 */
} HIGO_DEC_IMGDATA_S;

/** Information about input streams*//** CNcomment: 输入流信息*/
typedef struct
{
    HI_VOID *pStreamAddr;    /**<Virtual start address of streams*//**<CNcomment: 流虚拟起始地址*/
    HI_U32   StreamLen;      /**<Stream length*//**<CNcomment: 流长度*/
    HI_BOOL  EndFlag;        /**<Stream end flag*//**<CNcomment: 码流结束标记*/
    HI_U32   CopyLen;         /**<Actual length of the copied data*//**<CNcomment: 实际拷贝的数据长度*/
}HIGO_DEC_WRITESTREAM_S;


/** @} */  /*! <!-- Structure Definition end */
/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** \addtogroup      HIGO_DEC */
/** @{ */  /** <!-- [HIGO_DEC] */


 /**
\brief Initializes the decoder.CNcomment: 解码器初始化 CNend
\attention \n
::HI_GO_Init already calls the interfaceCNcomment: ::HI_GO_Init中已包含对该接口的调用 CNend
\param  N/ACNcomment: 无 CNend

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE
\retval ::HIGO_ERR_DEPEND_TDE

\see \n
::HI_GO_Init \n
::HI_GO_DeinitDecoder
*/

HI_S32 HI_GO_InitDecoder(HI_VOID);

/**
\brief Deinitializes the decoder.CNcomment: 解码器去初始化 CNend
\attention \n
When ::HI_GO_Deinit is called, this API is also called.CNcomment: ::HI_GO_Deinit中已包含对该接口的调用 CNend
\param  N/A.CNcomment: 无 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_Deinit \n
::HI_GO_InitDecoder
*/

HI_S32 HI_GO_DeinitDecoder(HI_VOID);

 /**
 \brief Creates a decoding handle.CNcomment: 创建解码句柄 CNend
 \attention \n
 Currently, the formats of .bmp, .png, .jpeg, and .gif are supported. The picture input source can be the memory, files
, and streams. The picture input source of streams is only applicable to the .jpeg format.
CNcomment: 目前支持BMP、PNG、JPEG和GIF格式，图片输入源只可以为内存、文件以及流式，流式目前只对JPEG格式有效。CNend 

 \param[in] pSrcDesc Information about the input source. This parameter cannot be empty. 
                The type of the input stream must match the information about the input source. For example, 
corresponding file names must be entered for the file stream, and the memory address and memory size must be entered 
for the memory stream.CNcomment: 输入源信息，不可为空\n
                输入码流的类型要与输入源详细信息一一匹配，如FILE码流则需要输入相应的文件名\n
                如Memory码流则需要输入memory的地址以及长度 CNend
 \param[out] pDecoder Pointer of the decoding instance handle. This parameter cannot be empty.CNcomment: 解码实例句柄指针，不可为空 CNend
 
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_NOTINIT
 \retval ::HIGO_ERR_NOMEM
 \retval ::HIGO_ERR_INVHANDLE
 \retval ::HIGO_ERR_INVFILE
 \retval ::HIGO_ERR_INVSRCTYPE
 \retval ::HIGO_ERR_INVIMAGETYPE
 \retval ::HIGO_ERR_INVIMGDATA

\see \n
::HI_GO_DestroyDecoder
*/
HI_S32 HI_GO_CreateDecoder(const HIGO_DEC_ATTR_S* pSrcDesc, HI_HANDLE* pDecoder);

 /** 
 \brief Destroys a decoding handle.CNcomment:销毁解码句柄 CNend
 \attention \n
 N/A.CNcomment:无 CNend
 \param[in] Decoder Handle of the decoding instance.CNcomment:解码实例句柄 CNend
 
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateDecoder
*/
HI_S32 HI_GO_DestroyDecoder(HI_HANDLE Decoder);

/**
 \brief Stream APICNcomment: 流式码流接口 CNend
 \attention \n
 N/A.CNcomment:无 CNend
 \param[in] Decoder Handle of the decoding instance.CNcomment: 解码实例句柄 CNend
 \param[in] pStream Pointer to the stream information. This parameter cannot be empty.CNcomment: 码流信息指针 ，不可为空 CNend
 
 \retval ::HI_SUCCESS
 \retval ::HI_FAILURE
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_INVHANDLE
 \retval ::HIGO_ERR_UNSUPPORTED

\see \n
::HI_GO_CreateDecoder
*/
HI_S32 HI_GO_DecWriteStream(HI_HANDLE Decoder, HIGO_DEC_WRITESTREAM_S *pStream);

/** 
 \brief Resets the decoder when you need to terminate the decoding.CNcomment:复位解码器，当需要终止解码的时候就进行解码器复位 CNend
 \attention \n
  N/A.CNcomment:无 CNend
 \param[in] Decoder Handle of the decoding instance.CNcomment:解码实例句柄 CNend
 \retval ::HI_SUCCESS
 \retval ::HI_FAILURE
 \retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateDecoder \n
::HI_GO_DestroyDecoder
*/
HI_S32 HI_GO_ResetDecoder(HI_HANDLE Decoder);

 /** 
 \brief Obtains the major information about pictures.CNcomment:获取图片文件的主要信息 CNend
 \attention
 N/A.CNcomment:无 CNend
\param[in] Decoder Handle of the decoding instance.CNcomment:解码实例句柄 CNend
\param[out] pPrimaryInfo  Pointer to the major information about the decoded picture. This parameter cannot be empty.CNcomment:解码图片主信息指针，不可为空 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\retval ::HI_NULL_PTR
\retval ::HIGO_ERR_INVIMGDATA

\see \n
::HI_GO_DecImgInfo
*/
HI_S32 HI_GO_DecCommInfo(HI_HANDLE Decoder, HIGO_DEC_PRIMARYINFO_S *pPrimaryInfo);

/**
 \brief Obtains the information about the specified picture. You can customize the picture format.CNcomment: 获取指定图片的信息，但同时用户可以设置期望的图象格式 CNend
 \attention \n
  N/A.CNcomment: 无 CNend
 \param[in] Decoder Handle of the decoding instance.CNcomment: 解码实例句柄 CNend
 \param[in] Index  
 Picture index number. For .jpeg pictures, the index number of the main picture is 0, and index 
numbers of thumbnails start from 1.
                   The index numbers of GIF pictures start from 0 and increase by 1 according to the sequence of the 
pictures stored in a file.
CNcomment: 图片索引号。JPEG格式图片，主图的索引号是0，缩略图索引号由1开始；\n
                   GIF格式图片按照在文件中的存储顺序，索引号由0开始递加 CNend
 \param[out] pImgInfo Picture information. This parameter cannot be empty.CNcomment: 图片信息，不可为空 CNend
 
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_INVHANDLE
 \retval ::HIGO_ERR_INVINDEX
 \retval ::HI_FAILURE
 \retval ::HI_NULL_PTR
 \retval ::HIGO_ERR_INVIMGDATA

\see \n
::HI_GO_DecCommInfo
*/
HI_S32 HI_GO_DecImgInfo(HI_HANDLE Decoder, HI_U32 Index, HIGO_DEC_IMGINFO_S *pImgInfo);

 /** 
 \brief Obtains the data of a picture.CNcomment:获取单个图片的数据 CNend
 \attention \n
 The format in the destination picture attributes does not support the macro block format.CNcomment:目标图片属性中的图像格式不支持宏块格式 CNend
 
\param[in] Decoder Handle of the decoding instance.CNcomment:解码实例句柄 CNend
\param[in] Index Index number of a picture, starting from 0.CNcomment:图片索引号，索引号从0开始 CNend
\param[in] pImgAttr Attribute of the destination picture. If the value is empty, it indicates that the original 
picture attributes are retained.CNcomment:目标图片属性，为空表示保持原图片属性 CNend
\param[out] pSurface Handle of the surface where the decoded data is stored.CNcomment:解码后数据存放的surface的句柄 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVINDEX
\retval ::HI_FAILURE
\retval ::HI_NULL_PTR
\retval ::HIGO_ERR_INVIMGDATA
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_UNSUPPORTED

\see \n
::HI_GO_DecCommInfo
*/
HI_S32 HI_GO_DecImgData(HI_HANDLE Decoder, HI_U32 Index, const HIGO_DEC_IMGATTR_S *pImgAttr, HI_HANDLE *pSurface);

/**
 \brief Obtains the extended data.CNcomment: 获取扩展数据 CNend
 \attention \n
 Hi3110E does not support the API.CNcomment: Hi3110E不支持此函数。 CNend
 \param[in] Decoder Handle of the decoding instance.CNcomment: 解码实例句柄 CNend
 \param[in] DecExtendType 
 Type of the obtained extended data. Currently, only the EXIF information in .jpeg format is available.
 CNcomment: 获取扩展数据的类型，目前主要只有JPEG的EXIF信息 CNend
 \param[out] ppData Pointer to the extended data pointer.CNcomment: 指向扩展数据指针的指针 CNend
 \param[out] pLength ppData Length.CNcomment: ppData长度 CNend
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_INVHANDLE
 \retval ::HIGO_ERR_UNSUPPORTED

\see \n
::HI_GO_ReleaseDecExtendData
*/
HI_S32 HI_GO_DecExtendData(HI_HANDLE Decoder, HIGO_DEC_EXTENDTYPE_E DecExtendType, HI_VOID **ppData, HI_U32 *pLength);

 /** 
 \brief Releases extended data.CNcomment:释放扩展数据 CNend
 \attention \n
 Hi3110E does not support the API.CNcomment:Hi3110E不支持此函数。CNend
 \param[in] Decoder Decoder handle.CNcomment:Decoder句柄 CNend
 \param[in] DecExtendType Type of the obtained extended data. Currently, only the EXIF information in .jpeg format is 
available.CNcomment:获取扩展数据的类型，目前主要只有JPEG的EXIF信息 CNend
 \param[in] pData Pointer to the extended data.CNcomment:指向扩展数据指针 CNend
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_UNSUPPORTED

\see \n
::HI_GO_DecExtendData
*/
HI_S32 HI_GO_ReleaseDecExtendData(HI_HANDLE Decoder, HIGO_DEC_EXTENDTYPE_E DecExtendType, HI_VOID *pData);

/**
\brief Decodes a picture to a specified surface.CNcomment:将图像解码到指定的surface CNend
\attention \n
The decoded surface is scaled automatically based on the size of the target surface. In addition, the decoded surface 
modifies the attributes of the target surface, such as colorkey, alpha value, and palette.
CNcomment:解码后的Surface会自动按照目标surface的大小进行缩放，并且会改变目标surface的COLORKEY、ALPHA以及调色板属性 CNend
\param[in] Decoder Handle of the decoding instance.CNcomment: 解码实例句柄 CNend
\param[in] Index   Index number of a decoded picture.CNcomment:解码图片索引 CNend
\param[out] Surface Decoded target surface.CNcomment:解码的目标surface CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVINDEX
\retval ::HIGO_ERR_NULLPTR
\retval ::HI_FAILURE
\retval ::HI_NULL_PTR
\retval ::HIGO_ERR_INVIMGDATA
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_UNSUPPORTED

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_DecImgToSurface(HI_HANDLE Decoder, HI_U32 Index, HI_HANDLE Surface);

/**
\brief Decodes a specified file, and generates a new surface.CNcomment:解码指定的文件，并生成一个新的surface CNend
\attention \n
    If there are multiple pictures in the file (such as .gif animation, .jpeg main picture, and .jpeg thumbnail),
    only the .jpeg main picture and the first picture of the .gif animation are decoded.
    In addition, the surface size is the same as the picture size.
    CNcomment:如果文件中有多张图片，比如:gif动画，JPEG带主图和缩略图，
    JPEG只解主图，GIF解码解码第一张图片surface的大小与图片的大小一致 CNend
\param[in] pszFile Picture name. The picture can be in the format of .gif, .png, .jpg, or .bmp.CNcomment:图片文件名，可以是GIF, PNG, JPG, BMP CNend
\param[out] pSurface, Surface for storing decoded pictures.CNcomment:保存解码后图像 CNend 

\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVINDEX
\retval ::HIGO_ERR_NULLPTR
\retval ::HI_NULL_PTR
\retval ::HIGO_ERR_INVIMGDATA
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_INVFILE
\retval ::HIGO_ERR_INVSRCTYPE
\retval ::HIGO_ERR_INVIMAGETYPE

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_DecodeFile(const HI_CHAR* pszFile, HI_HANDLE* pSurface);

/**
\brief Set the attribute of Decoder out Surface memrory.  CNcomment:设置解码输出Surface的内存属性 CNend
\attention \n
N/A.CNcomment: 无 CNend
\param[in] Decoder handle. CNcomment:解码实例句柄 CNend
\param[in] pSurfInfo.  CNcomment:Surface的内存属性 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_SetDecSurfInfo(HI_HANDLE Decoder, HIGO_DEC_SURINFO_S  *pSurfInfo);
 
/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif /* __HI_GO_DECODE_H__ */
