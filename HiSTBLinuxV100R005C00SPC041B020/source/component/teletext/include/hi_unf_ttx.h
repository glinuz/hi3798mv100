/******************************************************************************

*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*

******************************************************************************
  File Name     : hi_unf_ttx.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/10/20
  Description   : Teletext UNF header file
  History       :
  1.Date        : 2011/10/20
    Author      : l00185424
    Modification: Created file

******************************************************************************/
#ifndef __HI_UNF_TTX_H__
#define __HI_UNF_TTX_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif  /* End of #ifdef __cplusplus */

/********************************Structure Definition********************************/
/** \addtogroup      Teletext */
/** @{ */  /** <!-- [Teletext] */

/** Teletext max line in page *//** CNcomment:teletext页的最大行数 */
#define HI_UNF_TTX_MAX_LINES (32)
/** Teletext line size */ /** CNcomment:teletext行的长度 */
#define HI_UNF_TTX_LINE_SIZE (46)

/** Teletext key support list *//** CNcomment:默认支持的交互方式 */
typedef enum hiUNF_TTX_KEY_E
{
    HI_UNF_TTX_KEY_0,
    HI_UNF_TTX_KEY_1,
    HI_UNF_TTX_KEY_2,
    HI_UNF_TTX_KEY_3,
    HI_UNF_TTX_KEY_4,
    HI_UNF_TTX_KEY_5,
    HI_UNF_TTX_KEY_6,
    HI_UNF_TTX_KEY_7,
    HI_UNF_TTX_KEY_8,
    HI_UNF_TTX_KEY_9, /**<Three number key to open a specified page *//**<CNcomment:三个数字键打开指定页 */
    HI_UNF_TTX_KEY_PREVIOUS_PAGE, /**<Previous page *//**<CNcomment:上一页 */
    HI_UNF_TTX_KEY_NEXT_PAGE, /**<Next page *//**<CNcomment:下一页 */
    HI_UNF_TTX_KEY_PREVIOUS_SUBPAGE, /**<Previous subpage *//**<CNcomment:上一 子页 */
    HI_UNF_TTX_KEY_NEXT_SUBPAGE, /**<Next subpage *//**<CNcomment:下一 子页 */
    HI_UNF_TTX_KEY_PREVIOUS_MAGAZINE, /**<Previous magazine *//**<CNcomment:上一杂志 */
    HI_UNF_TTX_KEY_NEXT_MAGAZINE, /**<Next magazine *//**<CNcomment:下一杂志 */
    HI_UNF_TTX_KEY_RED,    /**<First link in packet X/27, if inexistence, Show first valid page*//**<CNcomment:X/27 包中的第一个链接，无X/27则显示最近有效页 */
    HI_UNF_TTX_KEY_GREEN,  /**<Second  link in packet X/27, if inexistence, Show second valid page*//**<CNcomment:X/27 包中的第二个链接，无X/27则显示第二有效页 */
    HI_UNF_TTX_KEY_YELLOW, /**<Third  link in packet X/27, if inexistence, Show third valid page*//**<CNcomment:X/27 包中的第三个链接，无X/27则显示第三有效页 */
    HI_UNF_TTX_KEY_CYAN,   /**<Fourth  link in packet X/27, if inexistence, Show fourth valid page. you can replace it with blue key if no cyan key on the user's control unit*/
                           /**<CNcomment:X/27 包中的第四个链接，无X/27则显示第四有效页。如果遥控器上没有CYAN键，你可以用BLUE键来替代。*/
    HI_UNF_TTX_KEY_INDEX,  /**<Sixth  link in packet X/27, if inexistence, Show index  page*//**<CNcomment:X/27 包中的第六个链接，无X/27则指向起始页 */
    HI_UNF_TTX_KEY_REVEAL, /**<Reveal or hide concealed  information *//**<CNcomment:显示/隐藏conceal 信息 */
    HI_UNF_TTX_KEY_HOLD,   /**<Switch between hold and resume ttx play  *//**<CNcomment:暂停图文播放/恢复图文播放切换 */
    HI_UNF_TTX_KEY_MIX,    /**<Switch  between  transparent and nontransparent  background *//**<CNcomment:Teletext背景透明/不透明切换 */
    HI_UNF_TTX_KEY_UPDATE, /**<Update current page*//**<CNcomment:更新当前页 */
    HI_UNF_TTX_KEY_ZOOM,   /**<Send this cmd sevral times to display the upper,then the lower part of the screen and then return to the normal size teletext page*/
                           /**<CNcomment:通过连续发送此命令依次显示teletext页面的上半部分、下半部分、全部 */
    HI_UNF_TTX_KEY_SUBPAGE,/**<switch from page num ipunt mode to subpage input mode*//**<CNcomment:输入页号输入模式转到subpage输入模式 */
    HI_UNF_TTX_KEY_CANCEL,/**<Hide or display current page except page number, currently not support*//**<CNcomment:隐藏或者显示除了页号外的当前页内容 ,当前暂不支持*/
    HI_UNF_TTX_KEY_BUTT    /**<Invalid key*//**<CNcomment:无效的按键 */
} HI_UNF_TTX_KEY_E;

/** Teletext output type *//** CNcomment:Teletext输出的类型 */
typedef enum hiUNF_TTX_OUTPUT_E
{
    HI_UNF_TTX_VBI_OUTPUT,  /**<Only VBI output *//**<CNcomment:VBI 输出 */
    HI_UNF_TTX_OSD_OUTPUT,  /**<Only OSD output *//**<CNcomment:OSD 输出 */
    HI_UNF_TTX_DUAL_OUTPUT, /**<VBI OSD dual output *//**<CNcomment:VBI和OSD同时输出 */
    HI_UNF_TTX_BUTT         /**<Invalid output type *//**<CNcomment:无效输出类型 */
} HI_UNF_TTX_OUTPUT_E;

/** Teletext type *//** CNcomment:Teletext的类型 */
typedef enum hiUNF_TTX_TYPE_E
{
    HI_UNF_TTX_INITTTX = 1, /**<Initial Teletext page *//**<CNcomment:Teletext 图文 */
    HI_UNF_TTX_TTXSUBT = 2, /**<Teletext subtitle page *//**<CNcomment:Teletext 字幕 */
    HI_UNF_TTX_ADDINFO = 3, /**<Nonsupport for the moment <Additional information page *//**<CNcomment:暂不支持附加信息页 */
    HI_UNF_TTX_PROGSCD = 4, /**<Nonsupport for the moment <Programme schedule page *//**<CNcomment:暂不支持节目指南页 */
    HI_UNF_TTX_TTXSUBT_HIP = 5, /**<Nonsupport for the moment <Teletext subtitle page for hearing impaired people *//**<CNcomment:暂不支持为听力有障碍的人设置的Teletext字幕页 */
    HI_UNF_TTX_TTXSUBT_BUTT /**<Invalid teletext type *//**<CNcomment:无效的teletext 类型 */
} HI_UNF_TTX_TYPE_E;

/** Teletext page type *//** CNcomment:Teletext页的类型 */
typedef enum hiUNF_TTX_PAGE_TYPE_E
{
    HI_UNF_TTX_PAGE_CUR,   /**<Current reveal page *//**<CNcomment:当前显示页 */
    HI_UNF_TTX_PAGE_INDEX, /**<Initial Teletext page , if  packet X/30 exist, return index page in X/30, otherwise return default index page 100*/
                           /**<CNcomment:初始页，如果有X/30包，返回X/30包指定初始页，否则返回默认首页100 */
    HI_UNF_TTX_PAGE_LINK1, /**<First link  in packet  X/27, if  inexistence, return first valid page*//**<CNcomment:X/27包中第1个链接，如果没有，返回当前页最近有效页 */
    HI_UNF_TTX_PAGE_LINK2, /**<Second link  in packet  X/27, if inexistence, return second valid page*//**<CNcomment:X/27包中第2个链接，如果没有，返回当前页第二有效页 */
    HI_UNF_TTX_PAGE_LINK3, /**<Third link  in packet  X/27, if  inexistence, return third valid page*//**<CNcomment:X/27包中第3个链接，如果没有，返回当前页第三有效页 */
    HI_UNF_TTX_PAGE_LINK4, /**<Fourth  link  in packet  X/27, if  inexistence, return fourth valid page*//**<CNcomment:X/27包中第4个链接，如果没有，返回当前页第四有效页 */
    HI_UNF_TTX_PAGE_LINK5, /**<Fifth link  in packet  X/27, if inexistence, return fifth valid page*//**<CNcomment:X/27包中第5个链接，如果没有，返回0ff:3f7f */
    HI_UNF_TTX_PAGE_LINK6, /**<Sixth link  in packet  X/27, if inexistence, return Sixth valid page*//**<CNcomment:X/27包中第6个链接，如果没有，返回0ff:3f7f */
    HI_UNF_TTX_PAGE_BUTT   /**<Invalid  page type *//**<CNcomment:无效页类型 */
} HI_UNF_TTX_PAGE_TYPE_E;

/** Teletext user command type *//** CNcomment:Teletext命令类型 */
typedef enum hiUNF_TTX_CMD_E
{
    HI_UNF_TTX_CMD_KEY,          /**<(HI_UNF_TTX_KEY_E *)Default alternation type, key *//**<CNcomment:按键，默认的交互方式 */
    HI_UNF_TTX_CMD_OPENPAGE,     /**<(HI_UNF_TTX_PAGE_ADDR_S *) Open specified page*//**<CNcomment:(HI_UNF_TTX_PAGE_ADDR_S *)打开指定页 */
    HI_UNF_TTX_CMD_GETPAGEADDR,  /**<(HI_UNF_TTX_GETPAGEADDR_S *)Get current page , index page and  link page  address*//**<CNcomment:(HI_UNF_TTX_GETPAGEADDR_S *) 获取当前、首页、链接页地址 */
    HI_UNF_TTX_CMD_CHECKPAGE,    /**<(HI_UNF_TTX_CHECK_PARAM_S *) Check the specified page be received or not*//**<CNcomment:(HI_UNF_TTX_CHECK_PARAM_S *) 检查是否收到参数中指定的页 */
    HI_UNF_TTX_CMD_SETREQUEST,   /**<(HI_UNF_TTX_REQUEST_RAWDATA_S *) Sets up a request for teletext raw data*//**<CNcomment:(HI_UNF_TTX_REQUEST_RAWDATA_S *) 请求获取teletext中的原始数据 */
    HI_UNF_TTX_CMD_CLEARREQUEST, /**<(HI_UNF_TTX_REQUEST_RAWDATA_S *) Clears a request set up by the HI_UNF_TTX_CMD_SETREQUEST*//**<CNcomment:(HI_UNF_TTX_REQUEST_RAWDATA_S *) 释放由HI_UNF_TTX_CMD_SETREQUEST创建的数据请求 */
    HI_UNF_TTX_CMD_BUTT          /**<Invalid command type *//**<CNcomment:无效命令类型 */
} HI_UNF_TTX_CMD_E;

/** G0 char set *//** CNcomment:G0字符集 */
typedef  enum   hiUNF_TTX_G0SET_E
{
    HI_UNF_TTX_G0SET_LATIN,      /**<LATIN G0 Primary Set  *//**<CNcomment:LATIN G0主字符集 */
    HI_UNF_TTX_G0SET_CYRILLIC_1, /**<CYRILLIC_1 G0 Primary Set *//**<CNcomment:CYRILLIC_1  G0主字符集 */
    HI_UNF_TTX_G0SET_CYRILLIC_2, /**<CYRILLIC_2 G0 Primary Set*//**<CNcomment:CYRILLIC_2 G0主字符集 */
    HI_UNF_TTX_G0SET_CYRILLIC_3, /**<CYRILLIC_3 G0 Primary Set*//**<CNcomment:CYRILLIC_3 G0主字符集 */
    HI_UNF_TTX_G0SET_GREEK,      /**<GREEK G0 Primary Set*//**<CNcomment:GREEK G0主字符集 */
    HI_UNF_TTX_G0SET_HEBREW,     /**<HEBREW G0 Primary Set*//**<CNcomment:HEBREW  G0主字符集 */
    HI_UNF_TTX_G0SET_ARABIC,     /**<ARABIC G0 Primary Set*//**<CNcomment:ARABIC G0主字符集 */
    HI_UNF_TTX_G0SET_BUTT        /**<Invalid G0 Primary Set *//**<CNcomment:无效G0主字符集 */
} HI_UNF_TTX_G0SET_E;

/** G2 char set *//** CNcomment:G2字符集 */
typedef enum hiUNF_TTX_G2SET_E
{
    HI_UNF_TTX_G2SET_LATIN,    /**<LATIN G2 Set *//**<CNcomment:LATIN G2字符集 */
    HI_UNF_TTX_G2SET_CYRILLIC, /**<CYRILLIC G2 Set *//**<CNcomment:CYRILLIC G2字符集 */
    HI_UNF_TTX_G2SET_GREEK,    /**<GREEK G2 Set *//**<CNcomment:GREEK G2字符集 */
    HI_UNF_TTX_G2SET_ARABIC,   /**<ARABIC G2 Set *//**<CNcomment:ARABIC G2字符集 */
    HI_UNF_TTX_G2SET_BUTT      /**<Invalid G2 Set *//**<CNcomment:无效G2字符集 */
}HI_UNF_TTX_G2SET_E;

/** Latin G0 National Option Sub-sets *//** CNcomment:Latin G0国家字符子集 */
typedef enum hiUNF_TTX_NATION_SET_E
{
    HI_UNF_TTX_NATION_SET_PRIMARY,      /**<Latin G0 Primary nation sub set*//**<CNcomment:LATIN主国家子集*/
    HI_UNF_TTX_NATION_SET_CZECH,        /**<Latin czech slovak nation sub set*//**<CNcomment:LATIN czech/slovak国家子集*/
    HI_UNF_TTX_NATION_SET_ENGLISH,      /**<Latin english nation sub set*//**<CNcomment:LATIN english国家子集*/
    HI_UNF_TTX_NATION_SET_ESTONIAN,     /**<Latin estonian nation sub set*//**<CNcomment:LATIN estonian国家子集*/
    HI_UNF_TTX_NATION_SET_FRENCH,       /**<Latin french nation sub set*//**<CNcomment:LATIN french国家子集*/
    HI_UNF_TTX_NATION_SET_GERMAN,       /**<Latin german nation sub set*//**<CNcomment:LATIN german国家子集*/
    HI_UNF_TTX_NATION_SET_ITALIAN,      /**<Latin italish nation sub set*//**<CNcomment:LATIN italish国家子集*/
    HI_UNF_TTX_NATION_SET_LETTISH,      /**<Latin lettish lithuanian nation sub set*//**<CNcomment:LATIN lithuanian国家子集*/
    HI_UNF_TTX_NATION_SET_POLISH,       /**<Latin polish nation sub set*//**<CNcomment:LATIN polish国家子集*/
    HI_UNF_TTX_NATION_SET_PORTUGUESE,   /**<Latin portutuese spanish nation sub set*//**<CNcomment:LATIN portutuese/spanish国家子集*/
    HI_UNF_TTX_NATION_SET_RUMANIAN,     /**<Latin rumanian nation sub set*//**<CNcomment:LATIN rumanian国家子集*/
    HI_UNF_TTX_NATION_SET_SERBIAN,      /**<Latin serbian croatian slovenian nation sub set*//**<CNcomment:LATIN serbian/croatian/slovenian国家子集*/
    HI_UNF_TTX_NATION_SET_SWEDISH,      /**<Latin swedish finnish nation sub set*//**<CNcomment:LATIN finnish国家子集*/
    HI_UNF_TTX_NATION_SET_TURKISH,      /**<Latin turkish nation sub set*//**<CNcomment:LATIN turkish国家子集*/
    HI_UNF_TTX_NATION_SET_BUTT          /**<Invalid nation sub Set *//**<CNcomment:无效国家字符子集*/

}HI_UNF_TTX_NATION_SET_E;

/** Teletext char set *//** CNcomment:Teletext字符集 */
typedef  enum   hiUNF_TTX_CHARSET_E
{
    HI_UNF_TTX_CHARSET_G0,  /**<G0  character set *//**<CNcomment:G0 字符集 */
    HI_UNF_TTX_CHARSET_G1,  /**<G1  character set*//**<CNcomment:G1 字符集 */
    HI_UNF_TTX_CHARSET_G2,  /**<G2  character set *//**<CNcomment:G2 字符集 */
    HI_UNF_TTX_CHARSET_G3,  /**<G3  character set *//**<CNcomment:G3 字符集 */
    HI_UNF_TTX_CHARSET_BUTT /**<Invalid  character set *//**<CNcomment:无效字符集 */
} HI_UNF_TTX_CHARSET_E;

/** DRCS character size *//** CNcomment:DRCS字符大小 */
typedef enum hiUNF_TTX_DRCS_SIZE_E
{
    HI_UNF_TTX_DRCS_SIZE_NORMAL = 0,  /**<char size is 12*10*//**<CNcomment:字符大小12*10*/
    HI_UNF_TTX_DRCS_SIZE_SMALL = 1,   /**<char size is 6*5*//**<CNcomment:字符大小6*5*/
    HI_UNF_TTX_DRCS_SIZE_BUTT         /**<Invalid char size*//**<CNcomment:无效字符大小*/
}HI_UNF_TTX_DRCS_SIZE_E;

typedef HI_U32 HI_UNF_TTX_COLOR;

/** Teletext page info *//** CNcomment:Teletext页区域信息 */
typedef struct hiUNF_TTX_PAGEAREA_S
{
    HI_U32 u32Row         : 8; /**<The origination  row  number of the area  *//**<CNcomment:区域起始行号 */
    HI_U32 u32Column      : 8; /**<The origination  column  number of the area *//**<CNcomment:区域起始列号 */
    HI_U32 u32RowCount    : 8; /**<The count of row the area covers *//**<CNcomment:区域覆盖的列数 */
    HI_U32 u32ColumnCount : 8; /**<The count of column  the area covers *//**<CNcomment:区域覆盖的行数 */
} HI_UNF_TTX_PAGEAREA_S;

/** Teletext char attribute *//** CNcomment:Teletext字符属性 */
typedef  struct  hiUNF_TTX_CHARATTR_S
{
    HI_U32               u32Index    : 8; /**<Index of a char in a character set *//**<CNcomment:字符在字符集中的索引 */
    HI_BOOL              bContiguous : 1; /**<Contiguous mosaic char or not *//**<CNcomment:是否为连续马赛克标志 */
    HI_UNF_TTX_G0SET_E   enG0Set     : 3; /**<G0 Primary Set  , latin , arabic .... *//**<CNcomment:G0主字符集 */
    HI_UNF_TTX_G2SET_E   enG2Set     : 3;  /**<G2 set ,latin, cyrillic, greek, arabic*//**<CNcomment:G2主字符集*/
    HI_UNF_TTX_CHARSET_E enCharSet   : 3; /**<Character set  , G0 ,  G1  ....*//**<CNcomment:字符集 */
    HI_U32               u8NationSet : 6; /**<Latin National  subset,  English ,French,  German .... *//**<CNcomment:国家字符子集 */
    HI_U32               u8Reserved  : 8; /**<Reserved *//**<CNcomment:预留 */
} HI_UNF_TTX_CHARATTR_S;

/** The info of draw char *//** CNcomment:绘制字符的信息 */
typedef struct hiUNF_TTX_DRAWCAHR_S
{
    HI_UNF_TTX_CHARATTR_S  * pstCharAttr;   /**<Character attribution, it can decide the position of a char in  a  font */
                                            /**<CNcomment:字符属性，决定了一个字符在特定字库中的位置 */
    HI_UNF_TTX_PAGEAREA_S * pstPageArea;    /**<Area of character in page *//**<CNcomment:字符在页面上的位置 */
    HI_UNF_TTX_COLOR        u32Foreground;  /**<Foreground color *//**<CNcomment:前景色 */
    HI_UNF_TTX_COLOR        u32Background;  /**<Background color *//**<CNcomment:背景色 */
} HI_UNF_TTX_DRAWCAHR_S;

/** The info of draw DRCS character *//** CNcomment:绘制DRCS字符的信息 */
typedef struct hiUNF_TTX_DRAWDRCSCHAR_S
{
    HI_UNF_TTX_PAGEAREA_S  *pstPageArea;      /**<Area of character in page *//**<CNcomment:字符在页面上的位置 */
    HI_UNF_TTX_COLOR       u32Background;     /**<Background color *//**<CNcomment:背景色 */
    HI_UNF_TTX_COLOR*      pu32DRCSColorInfo; /**<color info of DRCS char,which define the color value of  every pixel in a DRCS char*/
                                              /**<CNcomment:DRCS字符颜色信息，指定了DRCS字符每个像素的颜色值*/
    HI_UNF_TTX_DRCS_SIZE_E enDRCSSize;        /**<size of DRCS char,normal is 12*10,and small is 6*5*//**<CNcomment:DRCS字符大小，正常是12*10,小字符是6*5 */
}HI_UNF_TTX_DRAWDRCSCHAR_S;


/** The filled area *//** CNcomment:填充区域 */
typedef struct hiUNF_TTX_FILLRECT_S
{
    HI_UNF_TTX_PAGEAREA_S * pstPageArea; /**<Destination rectangle  *//**<CNcomment:目的矩形 */
    HI_UNF_TTX_COLOR        u32Color;    /**<Color *//**<CNcomment:颜色值 */
} HI_UNF_TTX_FILLRECT_S;

/** Refreshed layer *//** CNcomment:刷新图层 */
typedef struct hiUNF_TTX_REFRESHLAYER_S
{
    HI_UNF_TTX_PAGEAREA_S * pstPageArea; /**<Destination rectangle  *//**<CNcomment:(HI_UNF_TTX_BUFFER_PARAM_S *) 创建缓存 */
} HI_UNF_TTX_REFRESHLAYER_S;

/** Teletext Buffer info *//** CNcomment:Teletext缓存信息 */
typedef struct hiUNF_TTX_BUFFER_PARAM_S
{
    HI_U32 u32Row     : 8; /**<The row number of buffer page*//**<CNcomment:缓存页面的行数 */
    HI_U32 u32Column  : 8; /**<The column  number of buffer page*//**<CNcomment:缓存页面的列数 */
    HI_U32 u8Reserved : 16;/**<Reserved *//**<CNcomment:预留 */
} HI_UNF_TTX_BUFFER_PARAM_S;

/** The set of callback cmd *//** CNcomment:回调命令集合 */
typedef enum hiUNF_TTX_CB_E
{
    HI_UNF_TTX_CB_TTX_TO_APP_MSG, /**<Send message to GUI pthread *//**<CNcomment:发送绘制消息到GUI线程 */
    HI_UNF_TTX_CB_APP_FILLRECT,   /**<(HI_UNF_TTX_FILLRECT_S *) Fill rectangle *//**<CNcomment:(HI_UNF_TTX_FILLRECT_S *)矩形填充 */
    HI_UNF_TTX_CB_APP_DRAWCHAR,   /**<(HI_UNF_TTX_DRAWCAHR_S*)Select a char from a specified font and draw it  in specified rectangle of OSD by specified foreground and background */
                                  /**<CNcomment:(HI_UNF_TTX_DRAWCAHR_S*) 绘制函数，将指定字符以指定的前、背景色显示在OSD的指定区域 */
    HI_UNF_TTX_CB_APP_DRAWDRCSCHAR,/**<(HI_UNF_TTX_DRAWDRCSCHAR_S*)draw a DRCS char which specified by the color of every pixel*//**<CNcomment:绘制DRCS字符，字符由每个像素的颜色值确定*/
    HI_UNF_TTX_CB_APP_REFRESH,    /**<(HI_UNF_TTX_REFRESHLAYER_S*) Refresh layer *//**<CNcomment:(HI_UNF_TTX_REFRESHLAYER_S*) 图层刷新函数 */
    HI_UNF_TTX_CB_CREATE_BUFF,    /**<(HI_UNF_TTX_BUFFER_PARAM_S *) Create buffer *//**<CNcomment:(HI_UNF_TTX_BUFFER_PARAM_S *) 创建缓存 */
    HI_UNF_TTX_CB_DESTROY_BUFF,   /**<Destroy buffer *//**<CNcomment:销毁缓存 */
    HI_UNF_TTX_CB_GETPTS,         /**<(HI_S64 *) Get the PTS of the stream *//**<CNcomment:(HI_S64 *) 获取当前播放码流的PTS */
    HI_UNF_TTX_CB_BUTT            /**<Invalid callback type*//**<CNcomment:无效回调类型 */
} HI_UNF_TTX_CB_E;

typedef enum hiUNF_TTX_PACKET_TYPE_E
{
    HI_UNF_TTX_PACKET_TYPE_PES,  /**<ETSI EN DVB 300472 teletext syntax data packets, including PES header information*//**<CNcomment:ETSI EN 300472 DVB teletext语法数据包,包括PES头部信息*/
    HI_UNF_TTX_PACKET_TYPE_RAW,   /**<ETSI EN DVB 300706 teletext syntax data packets, not including PES header information*//**<CNcomment:ETSI EN 300706 DVB teletext语法数据包,不包括PES头部信息*/
    HI_UNF_TTX_PACKET_TYPE_BUTT,
}HI_UNF_TTX_PACKET_TYPE_E;

/** Callback function *//** CNcomment:回调函数 */
typedef HI_S32 (*HI_UNF_TTX_CB_FN)(HI_HANDLE hTTX, HI_UNF_TTX_CB_E enCB, HI_VOID *pvCBParam);

/** Teletext Init param *//** CNcomment:Teletext初始化参数 */
typedef struct hiUNF_TTX_INIT_PARA_S
{
    HI_U8 *pu8MemAddr;       /**<The address of memory, If  0, malloc the memory in the module, Otherwise malloced outside the module */
                             /**<CNcomment:数据分配的起始地址，如果为0则内部分配 ，否则由外部分配内存 */
    HI_U32 u32MemSize;       /**<The size of memory,  If  0, the size decided in the module, Otherwise decided outside the module */
                             /**<CNcomment:数据分配的大小，如果为0 ，由内部决定，否则大小由外部决定 */
    HI_UNF_TTX_CB_FN pfnCB;  /**<Callback function *//**<CNcomment:回调函数 */
    HI_BOOL          bFlash; /**<Permit flash or not *//**<CNcomment:是否开启闪烁功能 */
    HI_BOOL          bNavigation; /**<Permit navigation bar *//**<CNcomment:是否提供导航条 */
} HI_UNF_TTX_INIT_PARA_S;

/** Teletext page address *//** CNcomment:Teletext页属性 */
typedef struct hiUNF_TTX_PAGE_ADDR_S
{
    HI_U8  u8MagazineNum;  /**<Magazine number *//**<CNcomment:杂志号 */
    HI_U8  u8PageNum;      /**<Page number *//**<CNcomment:页号 */
    HI_U16 u16PageSubCode; /**<Page sub-code *//**<CNcomment:子页号 */
} HI_UNF_TTX_PAGE_ADDR_S;

/** Teletext content param *//** CNcomment:Teletext内容参数 */
typedef struct hiUNF_TTX_CONTENT_PARA_S
{
    HI_UNF_TTX_TYPE_E      enType; /**<Teletext content type *//**<CNcomment:Teletext内容类型 */
    HI_U32                         u32ISO639LanCode;/**<teletext iso639  language code*//**<CNcomment:Teletext  iso639语言*/
    HI_UNF_TTX_PAGE_ADDR_S stInitPgAddr; /**<Init page address, if Magazine number or Page number be equal to 0xFF,set to 100th page.sub-page numbet default  0*/
                                         /**<CNcomment:初始页地址，如果杂志号或页号为0xff，则设置为第100页。子页号默认为0 */
} HI_UNF_TTX_CONTENT_PARA_S;

/** Teletext checked param *//** CNcomment:Teletext检测参数 */
typedef struct hiUNF_TTX_CHECK_PARAM_S
{
    HI_UNF_TTX_PAGE_ADDR_S stPageAddr; /**<page address*//**<CNcomment:页地址  */
    HI_BOOL                bSucceed;   /**<success or failure*//**<CNcomment:是否成功 */
} HI_UNF_TTX_CHECK_PARAM_S;

/** Get page address*//** CNcomment:获取Teletext页 */
typedef struct hiUNF_TTX_GETPAGEADDR_S
{
    HI_UNF_TTX_PAGE_TYPE_E enPageType; /**<page type *//**<CNcomment:页类型 */
    HI_UNF_TTX_PAGE_ADDR_S stPageAddr; /**<page address *//**<CNcomment:页地址*/
} HI_UNF_TTX_GETPAGEADDR_S;

/** Teletext raw data*//** CNcomment:ttx原始数据 */
typedef struct hiUNF_TTX_RAWDATA_S
{
    HI_U32 u32ValidLines; /**<bit-field lines  0..31 *//**<CNcomment:有效位标识 */
    HI_U8  au8Lines[HI_UNF_TTX_MAX_LINES][HI_UNF_TTX_LINE_SIZE]; /**<line data *//**<CNcomment:ttx行数据 */
} HI_UNF_TTX_RAWDATA_S;

/** Callback function in which notified raw data to consumer *//** CNcomment:用于回传ttx原始数据的回调函数 */
typedef HI_S32 (*HI_UNF_TTX_REQUEST_CALLBACK_FN)(HI_UNF_TTX_RAWDATA_S *pstRawData);

/** Request teletext raw data*//** CNcomment:请求ttx原始数据 */
typedef struct hiUNF_TTX_REQUEST_RAWDATA_S
{
    HI_UNF_TTX_RAWDATA_S *pstRawData;  /**<raw data address*//**<CNcomment:原始数据地址 */
    HI_UNF_TTX_REQUEST_CALLBACK_FN pfnRequestCallback; /**<Callback function *//**<CNcomment:回调函数 */
} HI_UNF_TTX_REQUEST_RAWDATA_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/********************************API declaration********************************/
/** \addtogroup      Teletext  */
/** @{ */  /** <!-- [Teletext] */

/**
\brief     Initializes  TTX  module. CNcomment:初始化TTX模块。CNend
\attention \n
none.
\retval ::HI_SUCCESS     success. CNcomment:成功。CNend
\retval ::HI_FAILURE     failure. CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_Init(HI_VOID);

/**
\brief  Deinitializes TTX module. CNcomment:去初始化TTX模块。CNend
\attention \n
none.
\retval ::HI_SUCCESS     success. CNcomment:成功。CNend
\retval ::HI_FAILURE     failure. CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_DeInit(HI_VOID);

/**
\brief  Create a TTX instance, just support for a single  instance for the moment.  CNcomment:创建TTX实例，目前只支持单实例。CNend
\attention \n
After creating a instance  successfully, the instance  default  to be enable , decode and be
prepared to display teletext. Call  correspond interface to display teletext.
CNcomment:创建成功后这个实例默认enable，解析并准备显示teletext内容。调用相应的输出控制接口后输出。CNend
\param[in]  pstInitParam  Initialized  parameter. CNcomment:初始化参数。CNend
\param[out] phTTX         Teletext instance. CNcomment:Teletext句柄。CNend
\retval ::HI_SUCCESS      success.  CNcomment:成功。CNend
\retval ::HI_FAILURE      failure.  CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_Create(HI_UNF_TTX_INIT_PARA_S* pstInitParam, HI_HANDLE* phTTX);

/**
\brief    Destory  a  teletext instance.  CNcomment:销毁TTX实例。CNend
\attention \n
none.
\param[in] hTTX        Teletext instance. CNcomment:Teletext句柄。CNend
\retval ::HI_SUCCESS   success.  CNcomment:成功。CNend
\retval ::HI_FAILURE   failure.  CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_Destroy(HI_HANDLE hTTX);

/**
\brief   Inject the teletext PES data . CNcomment:注入Teletext PES 数据。CNend
\attention \n
none.
\param[in] hTTX    Teletext instance. CNcomment:Teletext句柄。CNend
\param[in] pu8Data   Address of data. CNcomment:数据地址。CNend
\param[in] u32DataSize  the length of data. CNcomment:数据长度。CNend
\retval ::HI_SUCCESS    success.  CNcomment:成功。CNend
\retval ::HI_FAILURE    failure.  CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_InjectData(HI_HANDLE hTTX, HI_U8 *pu8Data, HI_U32 u32DataSize);

/**
\brief   Reset data. CNcomment:清除收到的数据。CNend
\attention \n
none.
\param[in] hTTX   Teletext instance. CNcomment:Teletext句柄。CNend
\retval ::HI_SUCCESS    success.  CNcomment:成功。CNend
\retval ::HI_FAILURE    failure.  CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_ResetData(HI_HANDLE hTTX);

/**
\brief  Set the initial page address of teletext. CNcomment:设置Teletext的初始页。CNend
\attention \n
none.
\param[in] hTTX   Teletext instance. CNcomment:Teletext句柄。CNend
\param[in] pstContentParam  context  parameter.  CNcomment:内容参数。CNend
\retval ::HI_SUCCESS    success. CNcomment:成功。CNend
\retval ::HI_FAILURE    failure. CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_SwitchContent (HI_HANDLE hTTX, HI_UNF_TTX_CONTENT_PARA_S* pstContentParam);

/**
\brief  All operation related to OSD. CNcomment:和OSD相关的所有操作。CNend
\attention \n
none.
\param[in] hTTX    Teletext instance.   CNcomment:Teletext句柄。CNend
\param[in] enMsgAction  Action of the message.  CNcomment:消息指定的动作。CNend
\retval ::HI_SUCCESS    success.   CNcomment:成功。CNend
\retval ::HI_FAILURE    failure.   CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_Display(HI_HANDLE hTTX, HI_HANDLE hDispalyHandle);

/**
\brief   Enable  or disable teletext output, and set the type of output.
CNcomment:使能或关闭teletext输出、设置输出类型。CNend
\attention \n
 The parameter can be reset  time after time.   CNcomment: 可重复进行输出设置。CNend
\param[in] hTTX        Teletext instance.     CNcomment:Teletext句柄。CNend
\param[in]enOutput     Output  type: OSD,VBI or OSD  VBI dual output.   CNcomment:输出类型:OSD / VBI /OSD-VBI同时输出。CNend
\param[in] bEnable       HI_TRUE: enable,  HI_FALSE: disable.   CNcomment:HI_TRUE: 使能，HI_FALSE: 关闭。CNend
\retval ::HI_SUCCESS      success.  CNcomment:成功。CNend
\retval ::HI_FAILURE      failure.  CNcomment:失败。CNend
\see \n
none.
*/
HI_S32 HI_UNF_TTX_Output (HI_HANDLE hTTX, HI_UNF_TTX_OUTPUT_E enOutput, HI_BOOL bEnable);

/**
\brief     The function  of TTX instance to handle user's operation.  CNcomment:TTX实例用户操作处理函数。CNend
\attention \n
none.
\param[in] hTTX     Teletext instance.   CNcomment:Teletext句柄。CNend
\param[in] enCMD      Type of command.   CNcomment:命令类型。CNend
\param[in] pvCMDParam     Parameter of  command(The parameter must be  conveted to appropriate  type at every
 specifical application), when the command is UPDATE or EXIT, the command can be NULL.
 CNcomment:命令参数(具体应用需要强制转换)，UPDATE/EXIT时可为NULL。CNend
\param[out] pvCMDParam    Parameter of command , when the command is  GETPAGEADDR, it points to the address of specifical  pages.
 CNcomment:命令参数，GETPAGEADDR时输出页地址。CNend
\retval ::HI_SUCCESS    success.  CNcomment:成功。CNend
\retval ::HI_FAILURE    failure.  CNcomment:失败。CNend
\see \n
 Please  consult  the definition of  HI_UNF_TTX_CMD_E.  CNcomment:请参考HI_UNF_TTX_CMD_E定义。CNend
*/
HI_S32 HI_UNF_TTX_ExecCmd(HI_HANDLE hTTX,
                          HI_UNF_TTX_CMD_E enCMD, HI_VOID *pvCMDParam);


/**
\brief  Setting max interval time of the teletext.
CNcomment:设置图文最大的时间偏差。CNend
\attention \n
None
\param[in] handle Handle of teletext instance. CNcomment:TTX实例句柄。CNend
\param[in] u32IntervalMs max interval of teletext, unit is Millisecondes.
CNcomment:图文最大的时间偏差值，单位ms。CNend

\retval ::HI_SUCCESS  Operation success. CNcomment:成功。CNend
\retval ::HI_FAILURE  Operation fail. CNcomment:失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_TTX_SetMaxInterval(HI_HANDLE hTTX, HI_U32 u32IntervalMs );

/**
\brief  Setting teletext packet type.
CNcomment:设置图文解析的数据包类型。CNend
\attention \n
None
\param[in] handle Handle of teletext instance. CNcomment:TTX实例句柄。CNend
\param[in] enPacketType the type of teletext packet
CNcomment:图文数据包类型。CNend

\retval ::HI_SUCCESS  Operation success. CNcomment:成功。CNend
\retval ::HI_FAILURE  Operation fail. CNcomment:失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_TTX_SetPacketType (HI_HANDLE hTTX, HI_UNF_TTX_PACKET_TYPE_E enPacketType);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
#endif
