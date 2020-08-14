
/**
\file
\brief Describes the header file of the text module.CNcomment:text模块头文件 CNend
\author Shenzhen Hisilicon Co., Ltd.
\version 1.0
\author s37678
\date 2008-7-21
*/

#ifndef __HI_GO_TEXT_H__
#define __HI_GO_TEXT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/
/** \addtogroup      HIGO_TEXT */
/** @{ */  /** <!—[HIGO_TEXT] */

#define LAYOUT_LEFT          HIGO_LAYOUT_LEFT     /**<Horizontally left*//**<CNcomment:水平居左*/
#define LAYOUT_RIGHT         HIGO_LAYOUT_RIGHT    /**<Horizontally right*//**<CNcomment:水平居右*/
#define LAYOUT_HCENTER       HIGO_LAYOUT_HCENTER  /**<Horizontally center*//**<CNcomment:水平居中*/
#define LAYOUT_TOP           HIGO_LAYOUT_TOP      /**<Vertically top*//**<CNcomment:垂直居顶*/
#define LAYOUT_BOTTOM        HIGO_LAYOUT_BOTTOM   /**<Vertically bottom*//**<CNcomment:垂直居底*/
#define LAYOUT_VCENTER       HIGO_LAYOUT_VCENTER  /**<Vertically center*//**<CNcomment:垂直居中*/
#define LAYOUT_WRAP          HIGO_LAYOUT_WRAP     /**<Wrap*//**<CNcomment:自动换行*/
#define LAYOUT_WORDELLIPSIS  HIGO_LAYOUT_WORDELLIPSIS /**<Ellipsis format*//**<CNcomment:使用省略号风格*/
#define LAYOUT_FORCE_LEFT    HIGO_LAYOUT_FORCE_LEFT     /**<Force Horizontally left*//**<CNcomment:强制水平居左*/
#define LAYOUT_FORCE_RIGHT   HIGO_LAYOUT_FORCE_RIGHT    /**<Force Horizontally right*//**<CNcomment:强制水平居右*/

/** */
typedef enum 
{
    HIGO_LAYOUT_LEFT             = 0x0001,
    HIGO_LAYOUT_RIGHT            = 0x0002,
    HIGO_LAYOUT_HCENTER          = 0x0004,
    HIGO_LAYOUT_WRAP             = 0x0008,
    HIGO_LAYOUT_WORDELLIPSIS     = 0x0010,    
    HIGO_LAYOUT_TOP              = 0x0100,
    HIGO_LAYOUT_BOTTOM           = 0x0200,
    HIGO_LAYOUT_VCENTER          = 0x0400,
    HIGO_LAYOUT_FORCE_LEFT        = 0x8000,
    HIGO_LAYOUT_FORCE_RIGHT      = 0x10000,
    HIGO_LAYOUT_BUTT             = 0x20000,
} HIGO_LAYOUT_E;

/**<Definition of a character set*//** CNcomment: 字符集定义 */
typedef enum 
{
    HIGO_CHARSET_UNKOWN,
    HIGO_CHARSET_ASCI,
    HIGO_CHARSET_GB2312,
    HIGO_CHARSET_UTF8,
    HIGO_CHARSET_UTF16,
    HIGO_CHARSET_UCS2 = HIGO_CHARSET_UTF16,
    HIGO_CHARSET_UTF32,
    HIGO_CHARSET_BUTT,
} HIGO_CHARSET_E;

/** Definition of a text base direction*//** CNcomment: 文本输出基础方向定义*/
typedef enum 
{
    HIGO_TEXT_DIR_NEUTRAL = 0,/**<neutral deirction*//**<CNcomment:中性方向，按内容查找强字符方向*/
    HIGO_TEXT_DIR_LTR,/**<Left to right*//**<CNcomment:从左到右方向 */
    HIGO_TEXT_DIR_RTL,/**<Right to left*//**<CNcomment:从右到左方向 */
    HIGO_TEXT_DIR_BUTT
} HIGO_TEXT_DIR_E;


typedef struct
{
    HIGO_CHARSET_E Charset; /**<Character set name*//**<CNcomment:字符集名称 */
    HI_U8 Height;         /**<Font height*//**<CNcomment:字体高度 */
    HI_U8 MaxWidth;      /**<Maximum font width*//**<CNcomment:字体最大宽度 */
}HIGO_FONTATTR_S;

/** Attributes of a text output object*//** CNcomment: 文本输出对像属性 */
typedef struct
{
    HI_COLOR        BgColor;     /**<Background color*//**<CNcomment:背景色 */
    HI_COLOR        FgColor;     /**<Foreground color*/ /**<CNcomment:前景色 */ 
    HIGO_FONTATTR_S SbFontAttr;  /**<Font attributes of the single-byte character set*/ /**<CNcomment:单字节字符集字体属性 */  
    HIGO_FONTATTR_S MbFontAttr;  /**<Font attributes of the multi-byte character set*//**<CNcomment:多字节字符集字体属性 */    
} HIGO_TEXTOUTATTR_S;

/** @} */  /*! <!-- Structure Definition end */

/******************************* API declaration *****************************/
/** \addtogroup      HIGO_TEXT */
/** @{ */  /** <!—[HIGO_TEXT] */

/**
\brief Creates a text output object.CNcomment: 创建文本输出对象 CNend
\attention \n
The values of pSbFontFile and pMbFontFile cannot be empty at the same time. If the received file name is in vector 
font, the font height is set based on the 22-dot array. 
CNcomment: pSbFontFile和pMbFontFile不可同时为空,如果传入的是矢量字体文件名,则字体高度按 
22点阵创建 CNend
\param[in] pSbFontFile Font file of the single-byte character set. If the value is empty, it indicates that the single
-byte character set library is not used.CNcomment: 单字节字符集字体文件，为空表示不使用单字节字符集字库 CNend
\param[in] pMbFontFile  Font file of the multi-byte character set. If the value is empty, it indicates that the multi-
byte character set library is not used. Ensure that the font files of the single-byte and multi-byte character sets 
cannot be empty concurrently.CNcomment: 多字节字符集字体文件，为空表示不使用多字节字符集字库，但是不能与单字节字符文件同时为空。 CNend
\param[out]  pTextOut Handle of a text output object.CNcomment: 文本输出对象句柄 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INVFILE
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_NOMEM

\see \n
::HI_GO_DestroyText
*/
HI_S32 HI_GO_CreateText(const HI_CHAR* pSbFontFile, const HI_CHAR* pMbFontFile, HI_HANDLE* pTextOut);

/** 
\brief Destroys a text output object.CNcomment:销毁文本输出对象 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] TextOut Handle of a text output object.CNcomment:文本输出对象句柄 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateText
*/
HI_S32 HI_GO_DestroyText(HI_HANDLE TextOut);

/** 
\brief set the input charset. CNcomment:设置输入字符集 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] TextOut Handle of a text output object.CNcomment:文本输出对象句柄 CNend
\param[out] CharSet  charset of input text.CNcomment:输入文本字符集合 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORT_CHARSET

\see \n
::HI_GO_CreateText
*/
HI_S32 HI_GO_SetInputCharSet(HI_HANDLE TextOut, HIGO_CHARSET_E CharSet);

/** 
\brief Obtains the attributes of a text output object.CNcomment:获取文本输出对象属性 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] TextOut Handle of a text output object.CNcomment:文本输出对象句柄 CNend
\param[out] pTextOutAttr  Attributes of a text output object.CNcomment:文本输出对象属性 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_GetTextAttr(HI_HANDLE TextOut,HIGO_TEXTOUTATTR_S *pTextOutAttr);

/** 
\brief Obtains the width and height of text contents.CNcomment:获取文本内容的宽高 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] TextOut Handle of a text output object.CNcomment:文本输出对象句柄 CNend
\param[in] pText Text contents.CNcomment:文本内容 CNend
\param[out] pWidth Width of text contents. It cannot be empty.CNcomment:文本内容宽度，不可为空 CNend
\param[out] pHeight Height of text contents. It cannot be empty.CNcomment:文本内容高度，不可为空 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORT_CHARSET

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_GetTextExtent(HI_HANDLE TextOut, const HI_CHAR *pText, HI_S32 *pWidth, HI_S32 *pHeight);

/** 
\brief Sets to display the background color of a text output object.CNcomment:设置文本输出对象的背景色是否显示 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] TextOut Handle of a text output object.CNcomment:文本输出对象句柄 CNend
\param[in] bTransparent  Whether to display the background color of a text output object. If the value is set to 
HI_TRUE, the background color is not displayed. The default value is HI_TRUE.CNcomment:文本输出对象背景色是否显示，为HI_TRUE, 则不显示，默认为HI_TRUE CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/ACNcomment:无 CNend
*/
HI_S32 HI_GO_SetTextBGTransparent(HI_HANDLE TextOut, HI_BOOL bTransparent);

/** 
\brief Sets the background color of a text output object.CNcomment:设置文本输出对象的背景色 CNend
\attention \n
If the target surface is a palette, the color is the palette index.CNcomment:如果目标surface是调色板，则Color是调色板索引 CNend
\param[in] TextOut Handle of a text output object.CNcomment:文本输出对象句柄 CNend
\param[in] Color  Background color of a text output object.CNcomment:文本输出对象背景色，CNend  

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_SetTextBGColor(HI_HANDLE TextOut, HI_COLOR Color);

/**
\brief Sets the font color of a text output object.CNcomment: 设置文本输出对象的字体颜色 CNend
\attention \n
If the target surface is a palette, the color is the palette index.CNcomment: 如果目标surface是调色板，则Color是调色板索引 CNend
\param[in] TextOut Handle of a text output object.CNcomment: 文本输出对象句柄 CNend
\param[in] Color  Font color of a text output object.CNcomment: 文本输出对象字体颜色 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment: 无 CNend
*/
HI_S32 HI_GO_SetTextColor(HI_HANDLE TextOut, HI_COLOR Color);

/** 
\brief Outputs text contents to a specified surface.CNcomment:输出文本内容到指定surface CNend
\attention
N/A.CNcomment:无 CNend 
\param[in] TextOut Handle of a text output object.CNcomment:文本输出对象句柄 CNend
\param[in] Surface Surface handle.CNcomment:Surface句柄 CNend
\param[in] pText  Text contents.CNcomment:文本内容 CNend
\param[in] pRect Text output region. If the value is empty, it indicates that text contents are output from the 
surface origin.CNcomment:文本输出区域，为空表示从surface原点输出 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_TextOut(HI_HANDLE TextOut,HI_HANDLE Surface, const HI_CHAR *pText, 
                           const HI_RECT *pRect);

/** 
\brief Outputs texts. This API is an extended API. You can call this API to output contents based on customized 
styles and formatting.CNcomment:文本输出扩展接口,输出内容可以按照用户指定的排版进行输出。CNend
\param[in] hTextOut Text output handle.CNcomment:文本输出句柄 CNend
\param[in] hSurface Surface handle.CNcomment:Surface句柄 CNend
\param[in] pText    Text contents ending with /0.CNcomment:以/0结尾的文本内容 CNend
\param[in] pRect    Text output region.CNcomment:文本输出区域 CNend
\param[in] Style    Styles and formatting including LAYOUT_LEFT, LAYOUT_RIGHT, and LAYOUT_HCENTER.CNcomment:排版格式，如LAYOUT_LEFT、LAYOUT_RIGHT、LAYOUT_HCENTER等 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_TextOutEx(HI_HANDLE hTextOut, HI_HANDLE hSurface, 
                               const HI_CHAR *pText,  const HI_RECT *pRect, 
                               HIGO_LAYOUT_E Style);

/**
\brief Calculates the number of characters in a specified text output region.CNcomment: 计算指定文本输出区域字符个数 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] hTextOut Text output handle.CNcomment: 文本输出句柄 CNend
\param[in] pText    Text contents ending with /0.CNcomment: 以/0结尾的文本内容 CNend
\param[in] pRect    Text output region. The value cannot be empty.CNcomment: 文本输出区域,不可为空 CNend
\param[in] pLen      Pointer to the string length.CNcomment: 字符串长度指针 CNend

\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HI_SUCCESS

\see \n
N/A.CNcomment: 无 CNend
*/
HI_S32 HI_GO_GetTextExtentEx(HI_HANDLE TextOut, const HI_CHAR *pText, 
                                       const HI_RECT *pRect, HI_U32 *pLen);

typedef enum
{
    HIGO_TEXT_STYLE_NORMAL = 0x00,/*Normal font*//**<CNcomment:正常字体*/ 
    HIGO_TEXT_STYLE_ITALIC = 0x01, /*Italic font*//**<CNcomment:斜体字体*/ 
    HIGO_TEXT_STYLE_BOLD   = 0x02, /*Bold font*//**<CNcomment:粗体字体*/ 
    HIGO_TEXT_STYLE_BUTT   = 0x80,
} HIGO_TEXT_STYLE_E;

typedef struct{
    HI_HANDLE     hFont;           		 /* handle of font file                         */
    HI_U32        Style;     		 /* layout style                                */
    HI_S32        s32Width;    		 /* width of display arear                      */
    HI_S32        s32Height;   		 /* height of display arear                     */
    HI_S32  	  horz_dist;         /* 字符间距                                    */
    HI_S32  	  vert_dist;         /* 行间距,一行文字下线和下行文字上线之间的距离 */
    HI_BOOL       single_para_flg;   /* 是否为单段落                                */
    HI_BOOL    	  auto_dir;          /* 是否为自动方向                              */
    HIGO_TEXT_DIR_E base_dir;        /* 基本方向                                    */
    HI_U32        max_char;          /* 最大字符数量                                */   
    HI_COLOR      select_bg;         /* 选中的背景色                                */
    HI_COLOR      select_fg;         /* 选中的背景色                                */
}HIGO_LAYOUT_INFO_S;

typedef struct
{
    const HI_CHAR* pSbcFontFile;
    const HI_CHAR* pMbcFontFile;
    HI_U32   u32Size; /*This value is ignored for the dot-matrix font.*//**<CNcomment:如果是点阵字体，则该值被忽略*/
} HIGO_TEXT_INFO_S;

typedef enum{
	HIGO_LAYOUT_MOVECURSOR_UP,    /**< move cursor to left position 			 */
	HIGO_LAYOUT_MOVECURSOR_DOWN,  /**< move cursor to right position 			 */
	HIGO_LAYOUT_MOVECURSOR_PREV,  /**< move cursor to previous postion 			 */
	HIGO_LAYOUT_MOVECURSOR_NEXT,  /**< move cursor to next position 			 */
	HIGO_LAYOUT_MOVECURSOR_LEFT,  /**< move cursor to left position 			 */
	HIGO_LAYOUT_MOVECURSOR_RIGHT, /**< move cursor to right positon 			 */
	HIGO_LAYOUT_MOVECURSOR_SOT,   /**< move cursor to start position of text 	 */
	HIGO_LAYOUT_MOVECURSOR_EOT,   /**< move curosr to end position of text 		 */
	HIGO_LAYOUT_MOVECURSOR_SOL,   /**< move cursor to start of current line		 */
	HIGO_LAYOUT_MOVECURSOR_EOL,   /**< move cursor to end of current line 		 */
	HIGO_LAYOUT_MOVECURSOR_SOP,   /**< move cursor to start of current paragraph */
	HIGO_LAYOUT_MOVECURSOR_EOP,   /**< move cursor to end of current paragraph	 */
	HIGO_LAYOUT_MOVECURSOR_BUTT,   
}HIGO_TEXT_CURSOR_DIR_E;


/** 
\brief Creates a font.CNcomment:创建字体 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] phText Text output handle.CNcomment:文本输出句柄 CNend
\param[in] pInfo  Text information.CNcomment:文件信息 CNend

\retval ::HI_SUCCESS A font is created successfully.CNcomment:字体创建成功 CNend
\retval ::HI_FAILURE A font fails to be created.CNcomment:字体创建失败 CNend
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_CreateTextEx(const HIGO_TEXT_INFO_S* pInfo, HI_HANDLE* phText);

/** 
\brief Sets the style of a font, such as bold, italic, or normal.CNcomment:设置字体的风格，比如:粗体，斜体，正常体 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] hTextOut Text output handle.CNcomment:文本输出句柄 CNend
\param[in] eStyle   Font style.CNcomment:字体风格 CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_SetTextStyle(HI_HANDLE hTextOut, HIGO_TEXT_STYLE_E eStyle);



/** 
\brief Sets the the text base direction
lines.CNcomment:设置文本输出对象的基本方向 CNend
\attention \n
N/A.CNcomment:无 CNend

\param[in] hTextOut Text output handle. CNcomment:文本输出句柄 CNend
\param[in] BaseDir the base direction of the text. CNcomment: 文本输出对象基本方向 CNend

\retval ::HI_SUCCESS Success.CNcomment: 成功 CNend
\retval ::HIGO_ERR_INVHANDLE Failure.CNcomment:失败 CNend

\see \n
N/A.CNcomment:无 CNend
*/

HI_S32 HI_GO_SetBaseDir(HI_HANDLE TextOut, HIGO_TEXT_DIR_E eBaseDir);
/** 
\brief Sets the horizontal spacing between displayed characters, that is, the horizontal spacing between character 
lines.CNcomment:设置字符显示水平间距，两个字符行水平间距 CNend
\attention \n
N/A.CNcomment:无 CNend

\param[in] hTextOut Text output handle.CNcomment:文本输出句柄 CNend
\param[in] u32Distance Horizontal spacing.CNcomment:水平间距 CNend

\retval ::HI_SUCCESS Success.CNcomment: 成功 CNend
\retval ::HIGO_ERR_INVHANDLE Failure.CNcomment:失败 CNend

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_SetCharExtra(HI_HANDLE hTextOut, HI_U32 u32Distance);

/** 
\brief Sets the vertical and horizontal spacing.CNcomment:设置垂直水平间距 CNend
\attention \n
This API is available only when the font style is changed.CNcomment:只有换行风格才有效 CNend

\param[in] hTextOut Text output handle.CNcomment:文本输出句柄 CNend
\param[in] u32Distance Vertical spacing.CNcomment:垂直间距 CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HIGO_ERR_INVHANDLE Failure.CNcomment:失败 CNend

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_SetLineExtra(HI_HANDLE hText, HI_U32 u32Distance);

/**
\brief Initializes the character module.CNcomment: 初始化字符模块 CNend
\attention \n
N/A.CNcomment: 无 CNend
\param N/A.CNcomment: 无 CNend
\retval ::HI_SUCCESS Success.CNcomment: 成功 CNend
\retval ::HI_FAILURE Failure.CNcomment: 失败 CNend
\retval ::HIGO_ERR_DEPEND_TDE

\see \n
N/A.CNcomment: 无 CNend
*/
HI_S32 HI_GO_InitText();

/** 
\brief Deinitializes the character module.CNcomment:去初始化字符模块 CNend
\attention \n
N/A.CNcomment:无 CNend
\param N/A.CNcomment:无 CNend
\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HIGO_ERR_NOTINIT Failure.CNcomment:失败 CNend

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_DeinitText();


/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif /* __HI_GO_TEXT_H__ */
