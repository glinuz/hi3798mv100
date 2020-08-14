#ifndef  __SAMPLE_TELETEXT_FONT_H__
#define __SAMPLE_TELETEXT_FONT_H__

#include  "hi_unf_ttx.h"
#ifdef __cplusplus
extern "C" {
#endif

#define TTX_CHARWIDTH (12)   /** The width of a  char in picture(font0.png) */
#define TTX_CHARHEIGHT (10)   /** The height of a char iin picture(font0.png) */

#define TTX_CHARWIDTH1 (6) /*The width of a  char in small size*/
#define TTX_CHARHEIGHT1 (5)/*The height of a  char in small size*/

#define  TTX_ROWOFFONT (16)   /** Row number of the font */
#define   TTX_COLOFFONT (85)   /**  Column number of the font *//*64*/

HI_U32   FindIndex(HI_UNF_TTX_CHARATTR_S stCharAttr);

#ifdef __cplusplus
}
#endif
#endif
