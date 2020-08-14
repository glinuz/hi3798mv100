#include <stdlib.h>
#include <string.h>

#include "teletext_debug.h"
#include "teletext_data.h"
#include "teletext_parse.h"
#include "teletext_showpage.h"
#include "hi_unf_ttx.h"
#include "teletext_utils.h"

#define TTX_COLOR_NUM (8)    /** The number  of color */

#define P_CHARACTER(x) ((x) & 0x7F)
#define P_ADDITION(x) ((x) & 0x80)
#define P_GCODE(x) (((x) & 0x300) >> 8)
#define P_MARKS(x) (((x) & 0x3C00) >> 10)

#define TTX_GET_COLOR(entry) (g_u32RedefColorMap[entry])

#define TTX_SHOWPAGE_LOCK()  \
    do{\
        int ret = pthread_mutex_lock(&pstCurrentPoint->Mutexlock);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_lock(SHOWPAGE) failure ret = 0x%x\n",ret);\
        }\
    }while(0)

#define TTX_SHOWPAGE_UNLOCK()  \
    do{\
        int ret = pthread_mutex_unlock(&pstCurrentPoint->Mutexlock);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_unlock(SHOWPAGE) failure ret = 0x%x\n",ret);\
        }\
    }while(0)


/**  Color set */

#define BLACK_COLOR (0xff000000)
#define RED_COLOR (0xffff0000)
#define GREEN_COLOR (0xff00ff00)
#define YELLOW_COLOR (0xffffff00)
#define BLUE_COLOR (0xff0000ff)
#define MAGENTA_COLOR (0xffff00ff)
#define CYAN_COLOR (0xff00ffff)
#define WHITE_COLOR (0xffffffff)

static HI_U32 s_ColorSet[TTX_COLOR_NUM] =
{
    0xff000000,   /**black */
    0xffff0000,  /**red */
    0xff00ff00, /**green*/
    0xffffff00, /**yellow*/
    0xff0000ff,/**blue*/
    0xffff00ff,/**magenta */
    0xff00ffff,/**cyan */
    0xffffffff /**white */
};

TTX_TRIPLET_PARSER g_stTripParser[TTX_MAX_OPEN_OBJECTS];

/* character set maps */
static const TTX_DEF_G0G2_S g_stDefG0G2Map0000[8] =
{
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ENGLISH},/*C12 C13 C14: 000*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_GERMAN},/*C12 C13 C14: 001*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_SWEDISH},/*C12 C13 C14: 010*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ITALIAN},/*C12 C13 C14: 011*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_FRENCH},/*C12 C13 C14: 100*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_PORTUGUESE},/*C12 C13 C14: 101*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_CZECH},/*C12 C13 C14: 110*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_PRIMARY}/*C12 C13 C14: 111*/

};
static const TTX_DEF_G0G2_S g_stDefG0G2Map0001[8] =
{
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_POLISH},/*C12 C13 C14: 000*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_GERMAN},/*C12 C13 C14: 001*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_SWEDISH},/*C12 C13 C14: 010*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ITALIAN},/*C12 C13 C14: 011*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_FRENCH},/*C12 C13 C14: 100*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 101*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_CZECH},/*C12 C13 C14: 110*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_PRIMARY}/*C12 C13 C14: 111*/

};
static const TTX_DEF_G0G2_S g_stDefG0G2Map0010[8] =
{
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ENGLISH},/*C12 C13 C14: 000*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_GERMAN},/*C12 C13 C14: 001*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_SWEDISH},/*C12 C13 C14: 010*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ITALIAN},/*C12 C13 C14: 011*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_FRENCH},/*C12 C13 C14: 100*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_PORTUGUESE},/*C12 C13 C14: 101*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_TURKISH},/*C12 C13 C14: 110*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_PRIMARY}/*C12 C13 C14: 111*/

};
static const TTX_DEF_G0G2_S g_stDefG0G2Map0011[8] =
{
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 000*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 001*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 010*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 011*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 1001*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_SERBIAN},/*C12 C13 C14: 101*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 110*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_RUMANIAN}/*C12 C13 C14: 111*/

};
static const TTX_DEF_G0G2_S g_stDefG0G2Map0100[8] =
{
    {HI_UNF_TTX_G0SET_CYRILLIC_1, HI_UNF_TTX_G2SET_CYRILLIC, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 000*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_GERMAN},/*C12 C13 C14: 001*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ESTONIAN},/*C12 C13 C14: 010*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_LETTISH},/*C12 C13 C14: 011*/
    {HI_UNF_TTX_G0SET_CYRILLIC_2, HI_UNF_TTX_G2SET_CYRILLIC, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 100*/
    {HI_UNF_TTX_G0SET_CYRILLIC_3, HI_UNF_TTX_G2SET_CYRILLIC, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 101*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_CZECH},/*C12 C13 C14: 110*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT}/*C12 C13 C14: 111*/
};
static const TTX_DEF_G0G2_S g_stDefG0G2Map0110[8] =
{
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 000*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 001*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 010*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 011*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 100*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 101*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_TURKISH},/*C12 C13 C14: 110*/
    {HI_UNF_TTX_G0SET_GREEK, HI_UNF_TTX_G2SET_GREEK, HI_UNF_TTX_NATION_SET_BUTT}/*C12 C13 C14: 111*/

};
static const TTX_DEF_G0G2_S g_stDefG0G2Map1000[8] =
{
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_ARABIC, HI_UNF_TTX_NATION_SET_ENGLISH},/*C12 C13 C14: 000*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 001*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 010*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 011*/
    {HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_ARABIC, HI_UNF_TTX_NATION_SET_FRENCH},/*C12 C13 C14: 100*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 101*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 110*/
    {HI_UNF_TTX_G0SET_ARABIC, HI_UNF_TTX_G2SET_ARABIC, HI_UNF_TTX_NATION_SET_BUTT}/*C12 C13 C14: 111*/

};
static const TTX_DEF_G0G2_S g_stDefG0G2Map1010[8] =
{
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 000*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 001*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 010*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 011*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 100*/
    {HI_UNF_TTX_G0SET_HEBREW, HI_UNF_TTX_G2SET_ARABIC, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 101*/
    {HI_UNF_TTX_G0SET_BUTT, HI_UNF_TTX_G2SET_BUTT, HI_UNF_TTX_NATION_SET_BUTT},/*C12 C13 C14: 110*/
    {HI_UNF_TTX_G0SET_ARABIC, HI_UNF_TTX_G2SET_ARABIC, HI_UNF_TTX_NATION_SET_BUTT}/*C12 C13 C14: 111*/

};
static const TTX_SEC_G0_S g_stSecG0Map0000[8] =
{
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_ENGLISH},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_GERMAN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_SWEDISH},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_ITALIAN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_FRENCH},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_PORTUGUESE},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_CZECH},/**/
    {HI_UNF_TTX_G0SET_BUTT,   HI_UNF_TTX_NATION_SET_BUTT}/**/
};

static const TTX_SEC_G0_S g_stSecG0Map0001[8] =
{
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_POLISH},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_GERMAN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_SWEDISH},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_ITALIAN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_FRENCH},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_CZECH},/**/
    {HI_UNF_TTX_G0SET_BUTT,   HI_UNF_TTX_NATION_SET_BUTT}/**/
};

static const TTX_SEC_G0_S g_stSecG0Map0010[8] =
{
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_POLISH},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_GERMAN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_SWEDISH},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_ITALIAN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_FRENCH},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_PORTUGUESE},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_TURKISH},/**/
    {HI_UNF_TTX_G0SET_BUTT,   HI_UNF_TTX_NATION_SET_BUTT}/**/
};
static const TTX_SEC_G0_S g_stSecG0Map0011[8] =
{
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_SERBIAN},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,   HI_UNF_TTX_NATION_SET_RUMANIAN}/**/
};

static const TTX_SEC_G0_S g_stSecG0Map0100[8] =
{
    {HI_UNF_TTX_G0SET_CYRILLIC_1,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_GERMAN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_ESTONIAN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_LETTISH},/**/
    {HI_UNF_TTX_G0SET_CYRILLIC_2,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_CYRILLIC_3,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_CZECH},/**/
    {HI_UNF_TTX_G0SET_BUTT,   HI_UNF_TTX_NATION_SET_BUTT}/**/
};

static const TTX_SEC_G0_S g_stSecG0Map0110[8] =
{
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_TURKISH},/**/
    {HI_UNF_TTX_G0SET_GREEK,   HI_UNF_TTX_NATION_SET_BUTT}/**/
};

static const TTX_SEC_G0_S g_stSecG0Map1000[8] =
{
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_ENGLISH},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_NATION_SET_FRENCH},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_ARABIC,   HI_UNF_TTX_NATION_SET_BUTT}/**/
};

static const TTX_SEC_G0_S g_stSecG0Map1010[8] =
{
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_HEBREW,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_NATION_SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_ARABIC,   HI_UNF_TTX_NATION_SET_BUTT}/**/
};


static const TTX_MODIFIED_G0G2_S g_stModifiedG0G2Map00XX[8] =
{
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,   HI_UNF_TTX_G2SET_LATIN}/**/
};
static const TTX_MODIFIED_G0G2_S g_stModifiedG0G2Map0100[8] =
{
    {HI_UNF_TTX_G0SET_CYRILLIC_1,  HI_UNF_TTX_G2SET_CYRILLIC},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_CYRILLIC_2,  HI_UNF_TTX_G2SET_CYRILLIC},/**/
    {HI_UNF_TTX_G0SET_CYRILLIC_3,  HI_UNF_TTX_G2SET_CYRILLIC},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_BUTT,   HI_UNF_TTX_G2SET_BUTT}/**/
};
static const TTX_MODIFIED_G0G2_S g_stModifiedG0G2Map0110[8] =
{
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_LATIN},/**/
    {HI_UNF_TTX_G0SET_GREEK,   HI_UNF_TTX_G2SET_GREEK}/**/
};
static const TTX_MODIFIED_G0G2_S g_stModifiedG0G2Map1000[8] =
{
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_ARABIC},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_LATIN,  HI_UNF_TTX_G2SET_ARABIC},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_ARABIC,   HI_UNF_TTX_G2SET_ARABIC}/**/
};
static const TTX_MODIFIED_G0G2_S g_stModifiedG0G2Map1010[8] =
{
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_HEBREW,  HI_UNF_TTX_G2SET_ARABIC},/**/
    {HI_UNF_TTX_G0SET_BUTT,  HI_UNF_TTX_G2SET_BUTT},/**/
    {HI_UNF_TTX_G0SET_ARABIC, HI_UNF_TTX_G2SET_ARABIC} /**/
};


#define TTX_SHOW_FONTMAP_SIZE (11)

static const TTX_DEF_G0G2_S* g_astDefG0G2Map[TTX_SHOW_FONTMAP_SIZE] =
{
    g_stDefG0G2Map0000,
    g_stDefG0G2Map0001,
    g_stDefG0G2Map0010,
    g_stDefG0G2Map0011,
    g_stDefG0G2Map0100,
    HI_NULL,
    g_stDefG0G2Map0110,
    HI_NULL,
    g_stDefG0G2Map1000,
    HI_NULL,
    g_stDefG0G2Map1010
};

static const TTX_SEC_G0_S* g_astSecG0Map[TTX_SHOW_FONTMAP_SIZE] =
{
    g_stSecG0Map0000,
    g_stSecG0Map0001,
    g_stSecG0Map0010,
    g_stSecG0Map0011,
    g_stSecG0Map0100,
    HI_NULL,
    g_stSecG0Map0110,
    HI_NULL,
    g_stSecG0Map1000,
    HI_NULL,
    g_stSecG0Map1010
};


typedef struct tagTTX_LANGUAGE_S
{
    HI_U8                       u8LanguageCode[4];
    HI_UNF_TTX_G0SET_E          enG0Set;
    HI_UNF_TTX_G2SET_E          enG2Set;
    HI_UNF_TTX_NATION_SET_E enNationSet;
} TTX_LANGUAGE_S;

TTX_LANGUAGE_S g_stLanguageCode[] =
{
#if 0
    {"cze", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_CZECH},
    {"slo", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_CZECH},
    {"eng", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ENGLISH},
    {"est", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ESTONIAN},
    {"fre", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_FRENCH},
    {"ger", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_GERMAN},
    {"ita", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_ITALIAN},
    {"lit", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_LETTISH},
    {"pol", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_POLISH},
    {"por", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_PORTUGUESE},
    {"spa", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_PORTUGUESE},
    {"rum", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_RUMANIAN},
    {"slv", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_SERBIAN},
    {"swe", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_SWEDISH},
    {"fin", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_SWEDISH},
    {"tur", HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_NATION_SET_TURKISH},
#endif
    {"per", HI_UNF_TTX_G0SET_ARABIC, HI_UNF_TTX_G2SET_ARABIC, HI_UNF_TTX_NATION_SET_BUTT},
    {"ara", HI_UNF_TTX_G0SET_ARABIC, HI_UNF_TTX_G2SET_ARABIC, HI_UNF_TTX_NATION_SET_BUTT}
};


typedef struct tagTTX_SHOW_FONT_INFO_S
{
    HI_UNF_TTX_NATION_SET_E enNationSet;

    HI_UNF_TTX_G0SET_E enG0Set;
    HI_UNF_TTX_G0SET_E enLOPG0Set;
    HI_UNF_TTX_G0SET_E enModifiedG0Set;

    HI_UNF_TTX_G2SET_E enG2Set;
    HI_UNF_TTX_G2SET_E enLOPG2Set;
    HI_UNF_TTX_G2SET_E enModifiedG2Set;

    HI_UNF_TTX_CHARSET_E enCharSet;
} TTX_SHOW_FONT_INFO_S;

typedef struct tagTTX_SHOW_CURPACKET_INFO_S
{
    HI_U32 u32Band;
    HI_U32 u32LineNum;
    HI_U32 u32ColumnNum;
    HI_U32 u32OpenCount;
    HI_BOOL bLarged;
    HI_BOOL bFlag;
    HI_BOOL bBoxPage;

    HI_BOOL bMosaic;
    HI_BOOL bReleaseMos;
    HI_BOOL bUseDefaultCharSet;
    HI_BOOL bModifiedG0G2;
    HI_BOOL bEnhanceChar;/*char defined by X26 or object*/
    HI_BOOL bBlackBgSubstitute;




    /* background color for current row and screen */
    HI_U8 u8DefRowColor;
    HI_U8 u8FullRowColor;
    HI_U32 u32Fillback;
    HI_U32 u32MosaicCh;
    HI_U32 u32DesignCode;
    HI_UNF_TTX_COLOR au32BackgroundRec[72];/*40+16+16*/

    //Font info
    TTX_SHOW_FONT_INFO_S stFontInfo;

    HI_U8  u8CMBDRCSChar;        /* current DRCS char */
    HI_U8  u8CMBDRCSSubPage;
    HI_U32 u32X;
    TTX_DISP_ATTR stCMBAttr;       /* current attribute */

    HI_U8* au8DataLine;
} TTX_SHOW_CURPACKET_INFO_S;


typedef struct tagTTX_SHOW_SPACEATTR_S
{
    HI_BOOL bSpacingAttr;
    HI_BOOL bDoubleSize;
    HI_BOOL bSetAForeground;
    HI_BOOL bSetMForeground;
    HI_BOOL bSetFlash;
    HI_BOOL bHoldMosaic;
    HI_BOOL bStartBox;
    HI_BOOL bContiguous;
    HI_BOOL bEsc;

    HI_U8 u8LOPFlash;
    HI_UNF_TTX_COLOR u32TmpForeground;

    //Lop attr
    TTX_DISP_ATTR stLOPAttr;
} TTX_SHOW_SPACEATTR_S;

typedef struct tagTTX_SHOW_PRIORITY_S
{
    HI_U8 u8CharPriority;
    HI_U8 u8ForePriority;
    HI_U8 u8BackPriority;
    HI_U8 u8AttrPriority;
    HI_U8 u8FlashPriority;
} TTX_SHOW_PRIORITY_S;


typedef struct tagTTX_SHOW_DECHAM24_INFO_S
{
    HI_U8 u8Addr;
    HI_U8 u8Mode;
    HI_U8 u8Data;
} TTX_SHOW_DECHAM24_INFO_S;

typedef struct tagTTX_SHOW_PARSER_INFO_S
{
    HI_U32 u32Parser;
    TTX_TRIPLET_PARSER* pstParser;
    TTX_SHOW_PRIORITY_S stShowPriority;
    TTX_SHOW_DECHAM24_INFO_S stHam24Info;
} TTX_SHOW_PARSER_INFO_S;


/** Debug , print the name of color */
static  HI_VOID  TTX_Show_PutColor(HI_U32 u32Color)
{
    switch (u32Color)
    {
        case 0:
            HI_INFO_TTX("ABlack");
            break;

        case 1:
            HI_INFO_TTX("ARed");
            break;

        case 2:
            HI_INFO_TTX("AGreen");
            break;

        case 3:
            HI_INFO_TTX("AYellow");
            break;

        case 4:
            HI_INFO_TTX("ABlue");
            break;

        case 5:
            HI_INFO_TTX("AMagenta");
            break;

        case 6:
            HI_INFO_TTX("ACyan");
            break;

        case 7:
            HI_INFO_TTX("AWhite");
            break;

        case 0x10:
            HI_INFO_TTX("MBlack");
            break;

        case 0x11:
            HI_INFO_TTX("MRed");
            break;

        case 0x12:
            HI_INFO_TTX("MGreen");
            break;

        case 0x13:
            HI_INFO_TTX("MYellow");
            break;

        case 0x14:
            HI_INFO_TTX("MBlue");
            break;

        case 0x15:
            HI_INFO_TTX("MMagenta");
            break;

        case 0x16:
            HI_INFO_TTX("MCyan");
            break;

        case 0x17:
            HI_INFO_TTX("MWhite");
            break;

        case 0x1c:
            HI_INFO_TTX("BBACK");
            break;

        default:
            HI_INFO_TTX(" ");
            break;
    }
}

/** A char  is a space char or not */
static HI_BOOL TTX_Show_IsSpaceChar(HI_U32 u32Index)
{
    HI_BOOL bIsSpace = HI_FALSE;
    HI_U32 u32Char = u32Index;

    if (P_CHARACTER(u32Char) <= 0x20)
    {
        bIsSpace = HI_TRUE;
    }

    if (3 == P_GCODE(u32Char))
    {
        bIsSpace = HI_FALSE;
    }

    return bIsSpace;
}

/** A char  is a mosaic char or not */
static  HI_BOOL TTX_Show_IsMosaicChar(HI_BOOL bMosMode, HI_U32 u32Index)
{
    HI_BOOL bIsMosChar = HI_FALSE;
    HI_U32 u32Char = 0;

    if (u32Index < 0x20)
    {
        bIsMosChar = HI_FALSE;
        return bIsMosChar;
    }

    u32Char = u32Index;

    if (P_ADDITION(u32Char))   /** The char specify  by packet 26 */
    {
        switch (P_GCODE(u32Char))
        {
            case 0:                              /** G0 char  */
                bIsMosChar = HI_FALSE;
                break;

            case 1:                         /** G1 char  */
                u32Char = P_CHARACTER(u32Char) - 32;

                if ((u32Char < 32) || (u32Char >= 64))
                {
                    bIsMosChar = HI_TRUE;
                }
                else
                {
                    bIsMosChar = HI_FALSE;
                }

                break;

            case 2:                      /** G2 char */
                bIsMosChar = HI_FALSE;
                break;

            case 3:                    /** G3 char */
                bIsMosChar = HI_FALSE;
                break;

            default:
                bIsMosChar = HI_FALSE;
                break;
        }
    }
    else
    {
        u32Char = P_CHARACTER(u32Char) - 32;  /** Visible char  start from 0x20*/

        if (bMosMode)     /** Mosaic char  */
        {
            if ((u32Char < 32) || (u32Char >= 64))
            {
                bIsMosChar = HI_TRUE;
            }
            else
            {
                bIsMosChar = HI_FALSE;
            }
        }
        else
        {
            bIsMosChar = HI_FALSE;
        }
    }

    return bIsMosChar;
}

/** Record  a flash area */
static HI_VOID TTX_Show_ADD_FlashArea(TTX_FLAAREA_S** ppstFlaAreaHead, TTX_FLAAREA_S*   pstInitFlashArea)
{
    if (((HI_NULL) == pstInitFlashArea) || (HI_NULL == ppstFlaAreaHead))  /** Invalid param, just return */
    {
        return;
    }

    if (HI_NULL == *ppstFlaAreaHead)
    {
        (*ppstFlaAreaHead) = pstInitFlashArea;
        (*ppstFlaAreaHead)->pstNext = HI_NULL;
    }
    else
    {
        pstInitFlashArea->pstNext = (*ppstFlaAreaHead);
        *ppstFlaAreaHead = pstInitFlashArea;
    }
}

/** Release  all flash area */
static HI_VOID TTX_Show_Release_FlashArea(TTX_FLAAREA_S* pstFlashAreaHead)
{
    TTX_FLAAREA_S* pstFlashPoint = HI_NULL;
    TTX_FLAAREA_S* pstFlashLastPoint = HI_NULL;

    if (HI_NULL == pstFlashAreaHead)    /** Invalid param, just return */
    {
        return;
    }

    pstFlashPoint = pstFlashAreaHead;

    while (HI_NULL != pstFlashPoint)
    {
        pstFlashLastPoint = pstFlashPoint;
        pstFlashPoint = pstFlashPoint->pstNext;

        free(pstFlashLastPoint);
        pstFlashLastPoint = HI_NULL;
    }

    return;
}

static HI_VOID TTX_Show_Parse_MOTPage(TTX_PAGE_CONTEXT_S* pstCurrentPoint)
{
    HI_BOOL bHasP27 = HI_FALSE;

    if ((HI_TRUE == pstCurrentPoint->bHasP27_4) || (HI_TRUE == pstCurrentPoint->bHasP27_5))
    {
        bHasP27 = HI_TRUE;
    }

    (HI_VOID)TTX_DecodeMOTPage(&pstCurrentPoint->stCurPgAddr, &pstCurrentPoint->stMOTInfo, bHasP27);

    return;
}

static HI_VOID TTX_Show_GetColorMap(TTX_PAGE_CONTEXT_S* pstCurrentPoint)
{
    HI_U32 i = 0;
    HI_U8 u8MagNum = 0;
    HI_BOOL bColorConvert = HI_FALSE;

    /**set colormap to default*/
    memcpy(g_u32RedefColorMap, g_u32DefaultColorMap, 128);

    /**convert the color from step 16 to 256 */
    for (i = 0; i < 32; i++)
    {
        g_u32RedefColorMap[i] = TTX_COLOR_CONVERT(g_u32RedefColorMap[i] );
    }

    g_u32RedefColorMap[8] = 0;/*the entry 8 of default color map is transparent*/
    u8MagNum = pstCurrentPoint->stCurPgAddr.u8MagazineNum & 0x7;

    bColorConvert = HI_FALSE;

    if (HI_TRUE == g_bHasM29_0[u8MagNum])
    {
        memcpy(g_u32RedefColorMap + 16, g_stM29_0_Info[u8MagNum].u32CLUT2_3, 64);
        bColorConvert = HI_TRUE;
    }

    if (HI_TRUE == pstCurrentPoint->bHasP28_0)
    {
        memcpy(g_u32RedefColorMap + 16, pstCurrentPoint->stP28_0_Info.u32CLUT2_3, 64);
        bColorConvert = HI_TRUE;
    }

    /**convert the color from step 16 to 256 */
    if (HI_TRUE == bColorConvert)
    {
        for (i = 16; i < 32; i++)
        {
            g_u32RedefColorMap[i] = TTX_COLOR_CONVERT(g_u32RedefColorMap[i] );
        }

        bColorConvert = HI_FALSE;
    }


    if (HI_TRUE == g_bHasM29_4[u8MagNum])
    {
        memcpy(g_u32RedefColorMap, g_stM29_4_Info[u8MagNum].u32CLUT0_1, 64);
        bColorConvert = HI_TRUE;
    }

    if (HI_TRUE == pstCurrentPoint->bHasP28_4)
    {
        memcpy(g_u32RedefColorMap, pstCurrentPoint->stP28_4_Info.u32CLUT0_1, 64);
        bColorConvert = HI_TRUE;
    }

    /**convert the color from step 16 to 256 */
    if (HI_TRUE == bColorConvert)
    {
        for (i = 0; i < 16; i++)
        {
            g_u32RedefColorMap[i] = TTX_COLOR_CONVERT(g_u32RedefColorMap[i] );
        }
    }

    return;
}


static HI_VOID TTX_Show_GetP28_0_Info(TTX_PAGE_CONTEXT_S* pstCurrentPoint)
{
    HI_U8 u8MagNum = pstCurrentPoint->stCurPgAddr.u8MagazineNum & 0x7;

    if (HI_FALSE == pstCurrentPoint->bHasP28_0)
    {
        if (HI_FALSE == pstCurrentPoint->bHasP28_4)
        {
            if (HI_FALSE == g_bHasM29_0[u8MagNum])
            {
                if (HI_TRUE == g_bHasM29_4[u8MagNum])
                {
                    memcpy(&pstCurrentPoint->stP28_0_Info, g_stM29_4_Info + u8MagNum, sizeof(TTX_P28_0_M29_0_INFO_S) - 64);
                }
                else
                {
                    pstCurrentPoint->stP28_0_Info.u82ndG0 = pstCurrentPoint->stP28_0_Info.u8DefG0G2;
                }
            }
            else
            {
                memcpy(&pstCurrentPoint->stP28_0_Info, g_stM29_0_Info + u8MagNum, sizeof(TTX_P28_0_M29_0_INFO_S) - 64);
            }
        }
        else
        {
            memcpy(&pstCurrentPoint->stP28_0_Info, &pstCurrentPoint->stP28_4_Info, sizeof(TTX_P28_0_M29_0_INFO_S) - 64);
        }
    }

    if (HI_FALSE == pstCurrentPoint->bHasP28_1)
    {
        if (HI_TRUE == g_bHasM29_1[u8MagNum])
        {
            memcpy(&pstCurrentPoint->stP28_1_Info, g_stM29_1_Info + u8MagNum, sizeof(TTX_P28_1_M29_1_INFO_S));
        }
    }

    return;
}


static HI_VOID TTX_Show_GetBandInfo(TTX_PAGE_CONTEXT_S* pstCurrentPoint)
{
    HI_U32 i = 0;
    HI_U8 u8MagNum = pstCurrentPoint->stCurPgAddr.u8MagazineNum & 0x7;

    /** black background substitute */
    pstCurrentPoint->bBlackBgSubstitute = HI_FALSE;

    pstCurrentPoint->u8BandStart[0] = 0;
    pstCurrentPoint->u8BandEnd[0] = 40;
    pstCurrentPoint->u8BandStart[1] = 40;
    pstCurrentPoint->u8BandEnd[1] = 40;
    pstCurrentPoint->u8BandStart[2] = 72;
    pstCurrentPoint->u8BandEnd[2] = 72;

    if ((HI_TRUE == pstCurrentPoint->bHasP28_0) ||
        (HI_TRUE == pstCurrentPoint->bHasP28_4) ||
        (HI_TRUE == g_bHasM29_0[u8MagNum]) ||
        (HI_TRUE == g_bHasM29_4[u8MagNum]))
    {
        pstCurrentPoint->bBlackBgSubstitute = pstCurrentPoint->stP28_0_Info.bBlackBgSubstitute ? HI_TRUE : HI_FALSE;
        //if(pstCurrentPoint->stP28_0_Info.bSidePanelStatus)
        {
            i = pstCurrentPoint->stP28_0_Info.u8SidePanelCols;

            if (i)
            {
                if (pstCurrentPoint->stP28_0_Info.bSidePanelRight)
                {
                    pstCurrentPoint->u8BandEnd[1] = pstCurrentPoint->u8BandStart[1] + i;
                }

                if (pstCurrentPoint->stP28_0_Info.bSidePanelLeft)
                {
                    pstCurrentPoint->u8BandStart[2] = pstCurrentPoint->u8BandEnd[2] - (16 - i);
                }
            }
            else
            {
                if (pstCurrentPoint->stP28_0_Info.bSidePanelRight)
                {
                    pstCurrentPoint->u8BandEnd[1] = pstCurrentPoint->u8BandStart[1] + 16;
                }
                else if (pstCurrentPoint->stP28_0_Info.bSidePanelLeft)
                {
                    pstCurrentPoint->u8BandStart[2] = pstCurrentPoint->u8BandEnd[2] - 16;
                }
            }
        }
    }
    else if (HI_TRUE == pstCurrentPoint->stMOTInfo.bFallback)
    {
        pstCurrentPoint->bBlackBgSubstitute = pstCurrentPoint->stMOTInfo.bBlackBgSubstitute;

        if (pstCurrentPoint->stMOTInfo.bSidePanelLeft && pstCurrentPoint->stMOTInfo.bSidePanelRight)
        {
            pstCurrentPoint->stP28_0_Info.bSidePanelLeft  = HI_TRUE;
            pstCurrentPoint->stP28_0_Info.bSidePanelRight = HI_TRUE;
            pstCurrentPoint->stP28_0_Info.u8SidePanelCols = 8;
            pstCurrentPoint->u8BandEnd[1]       = 40 + 8;
            pstCurrentPoint->u8BandStart[2]      = 40 + 16 + 8;
        }
        else if (pstCurrentPoint->stMOTInfo.bSidePanelRight)
        {
            pstCurrentPoint->stP28_0_Info.bSidePanelRight = HI_TRUE;
            pstCurrentPoint->stP28_0_Info.u8SidePanelCols = 16;
            pstCurrentPoint->u8BandEnd[1]       = 40 + 16;
        }
        else if (pstCurrentPoint->stMOTInfo.bSidePanelLeft)
        {
            pstCurrentPoint->stP28_0_Info.bSidePanelLeft = HI_TRUE;
            pstCurrentPoint->stP28_0_Info.u8SidePanelCols = 16;
            pstCurrentPoint->u8BandStart[2]      = 40 + 16;
        }
    }

    pstCurrentPoint->u8BandLeft[0] = pstCurrentPoint->u8BandEnd[2] -  pstCurrentPoint->u8BandStart[2];/**normal area*/
    pstCurrentPoint->u8BandLeft[1] = pstCurrentPoint->u8BandLeft[0] + 40;/**right side panel*/
    pstCurrentPoint->u8BandLeft[2] = 0;/**left side panel*/

    return;
}



static HI_S32 TTX_Show_CalculatePageArea(TTX_PAGE_CONTEXT_S* pstCurrentPoint)
{
    HI_U32 u32MaxColNum = TTX_LOP_COL_NUM;/*max column num, if has sidepanel, this value is 56*/

    HI_UNF_TTX_BUFFER_PARAM_S stBuffParam;

    memset(&stBuffParam, 0x0, sizeof(HI_UNF_TTX_BUFFER_PARAM_S));

    if (pstCurrentPoint->stP28_0_Info.bSidePanelRight || pstCurrentPoint->stP28_0_Info.bSidePanelLeft)
    {
        pstCurrentPoint->u32ColumnNum = TTX_CMB_COL_NUM;
    }
    else
    {
        pstCurrentPoint->u32ColumnNum = TTX_LOP_COL_NUM;
    }

    if (u32MaxColNum != pstCurrentPoint->u32ColumnNum)
    {
        u32MaxColNum = pstCurrentPoint->u32ColumnNum;

        if (HI_TRUE == pstCurrentPoint->bNavigation)
        {
            stBuffParam.u32Row = TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR;
        }
        else
        {
            stBuffParam.u32Row = TTX_ROW_NUM + TTX_SUBPAGE_BAR;
        }

        stBuffParam.u32Column = u32MaxColNum;

        if (HI_SUCCESS != TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_DESTROY_BUFF, (HI_VOID*)1))
        {
            HI_ERR_TTX("TTX_Show_CallBack:HI_UNF_TTX_CB_DESTROY_BUFF error!\n");
            return HI_FAILURE;
        }

        if (HI_SUCCESS != TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_CREATE_BUFF, &stBuffParam))
        {
            HI_ERR_TTX("TTX_Show_CallBack: HI_UNF_TTX_CB_CREATE_BUFF error!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


static HI_S32 TTX_Show_ProcControlBits(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_BOOL* pbBoxPage, HI_U8* pu8NumOfSubPg)
{
    HI_U32 C10 = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bBoxPage  = HI_FALSE;
    HI_U8  u8NumOfSubPg = TTX_MAX_SUBPAGENO;
    HI_U32 u32MaxColNum = pstCurrentPoint->u32ColumnNum;

    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_FILLRECT_S stFillRectParam;
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;
    HI_U8  au8SubPage[TTX_MAX_SUBPAGENO] = {0};


    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));

    memset(au8SubPage, 0x0, sizeof(au8SubPage));

    if ((pstCurrentPoint->u32Flag >> 1) & 0x1)/** flag C5 */
    {
        HI_INFO_TTX("C5,NewFlash and boxed\n");
        bBoxPage = HI_TRUE;
    }

    if ((pstCurrentPoint->u32Flag >> 2) & 0x1)   /** flag C6 */
    {
        HI_INFO_TTX("C6,Subpage boxed\n");
        bBoxPage = HI_TRUE;
    }

    if ((pstCurrentPoint->u32Flag >> 3) & 0x1)  /** flag C7  */
    {
        HI_INFO_TTX("C7, row 0 not display\n");
    }

    C10 = (pstCurrentPoint->u32Flag >> 6) & 0x1;   /** flag C10 */

    if (1 == C10)
    {
        HI_INFO_TTX("C10,row 1-24 not display");
    }

    if (bBoxPage || (1 == C10))
    {
        pstCurrentPoint->bUpdateTime = HI_FALSE;
    }
    else
    {
        pstCurrentPoint->bUpdateTime = HI_TRUE;
    }


    if (1 == C10)      /** don't display the page */
    {
        if (pstCurrentPoint->bNavigation)
        {
            s32Ret = TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR,
                                      u32MaxColNum);
        }
        else
        {
            s32Ret = TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, u32MaxColNum);
        }

        stFillRectParam.pstPageArea = &stPageArea;
        stFillRectParam.u32Color = 0x00000000;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
        stRefrshLayerParam.pstPageArea = &stPageArea;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);

        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_TTX("in 1 == C10 condition,TTX_Show_DecShowPage, Errors occur \n");
        }

        return HI_FAILURE;
    }

    if (bBoxPage)   /** Subtitle , set the background  transparent */
    {
        if (pstCurrentPoint->bNavigation)
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR,
                                       u32MaxColNum);
        }
        else
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, u32MaxColNum);
        }

        stFillRectParam.pstPageArea = &stPageArea;
        stFillRectParam.u32Color = 0x00000000;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
    }
    else
    {
        if (pstCurrentPoint->bTtxMix)
        {
            stFillRectParam.u32Color = 0x00000000;
        }
        else
        {
            stFillRectParam.u32Color = 0xff000000;/**black*/
        }

        stFillRectParam.pstPageArea = &stPageArea;

        if (pstCurrentPoint->bNavigation)
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR, u32MaxColNum);
        }
        else
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM, u32MaxColNum);
        }

        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);

        u8NumOfSubPg = TTX_MAX_SUBPAGENO;
        s32Ret |= TTX_Data_GetSubpageNum(pstCurrentPoint->stCurPgAddr.u8MagazineNum,
                                         pstCurrentPoint->stCurPgAddr.u8PageNum,
                                         au8SubPage, &u8NumOfSubPg);

        if (u8NumOfSubPg == 0)      /** no subpage */
        {
            stFillRectParam.u32Color = 0x00000000;

            if (pstCurrentPoint->bNavigation)
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM + TTX_NAVIGATION_BAR, 0, TTX_SUBPAGE_BAR,
                                           u32MaxColNum);
            }
            else
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, 0, TTX_SUBPAGE_BAR, u32MaxColNum);
            }

            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
        }
    }


    *pbBoxPage = bBoxPage;
    *pu8NumOfSubPg = u8NumOfSubPg;

    return HI_SUCCESS;
}

static HI_VOID TTX_Show_ParsePOPPage(HI_U8 u8ObjIndex, TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U32* pu32OpenCount, HI_U8* au8DataLine)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_TRIPLET_PARSER* pstParser = HI_NULL;

    if (pstCurrentPoint->stMOTInfo.stDefaultObject[u8ObjIndex].u8ObjectType)
    {
        HI_U32 u32Pos = 0xff;
        HI_UNF_TTX_PAGE_ADDR_S* pstPgAddr = HI_NULL;

        pstPgAddr = u8ObjIndex < 2 ? &pstCurrentPoint->stMOTInfo.stGPOP : &pstCurrentPoint->stMOTInfo.stNPOP;

        s32Ret = TTX_DecodePOPPage(pstPgAddr, pstCurrentPoint->stMOTInfo.stDefaultObject + u8ObjIndex, &u32Pos, au8DataLine);

        if (HI_SUCCESS == s32Ret)
        {
            /*find the object*/
            if (u32Pos != 0xff)
            {
                pstParser = g_stTripParser + u8ObjIndex;
                pstParser->bOpen = HI_TRUE;
                pstParser->bRowOpen = HI_FALSE;
                pstParser->bStartDefine = HI_TRUE;
                pstParser->u8ObjectType = pstCurrentPoint->stMOTInfo.stDefaultObject[u8ObjIndex].u8ObjectType;

                pstParser->u8Priority       = OBJECT_PRIORITY(pstParser->u8ObjectType);
                pstParser->u8ActiveRow      = 0;
                pstParser->u8ActiveCol      = 0;
                pstParser->u8OffsetRow      = 0;
                pstParser->u8OffsetCol      = 0;
                pstParser->u8OriginRow      = 0;
                pstParser->u8OriginCol      = 0;
                pstParser->stAttr.u8ForeColor = pstParser->u8ObjectType == PASSIVE_OBJECT ? TTX_COLOR_WHITE : TTX_COLOR_UNDEFINED;
                pstParser->stAttr.u8BackColor = pstParser->u8ObjectType == PASSIVE_OBJECT ? TTX_COLOR_BLACK : TTX_COLOR_UNDEFINED;
                pstParser->u8Flash          = 0xff;
                pstParser->u8GDRCSSubPage   = 0;
                pstParser->u8NDRCSSubPage   = 0;
                memcpy(pstParser->u8Data,  au8DataLine, TTX_LINE_MEM_SIZE);
                pstParser->pu8Triplet = pstParser->u8Data + 3 + u32Pos * 3;
                pstParser->u8TripletPos = (HI_U8)u32Pos;

                *pu32OpenCount = *pu32OpenCount + 1;
            }
        }
    }

    return;
}

static HI_VOID TTX_Show_ParsePacket26(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U32* pu32OpenCount, HI_U32* pu32DesignCode, HI_U8* au8DataLine)
{
    HI_U8  i = 0;

    HI_S32 s32Ret = HI_SUCCESS;
    TTX_TRIPLET_PARSER* pstParser = HI_NULL;

    s32Ret = TTX_Data_FindPacket(pstCurrentPoint->pstCurPage, 26, 0, au8DataLine);

    if (HI_SUCCESS == s32Ret) /** if packet x/26 is found... */
    {

        pstParser = g_stTripParser;
        pstParser->bOpen = HI_TRUE;
        pstParser->bRowOpen = HI_FALSE;
        pstParser->bStartDefine = HI_FALSE;
        pstParser->u8ObjectType = ACTIVE_OBJECT;/* just like active objects */

        pstParser->u8Priority       = OBJECT_PRIORITY(ACTIVE_OBJECT) + 1;
        pstParser->u8ActiveRow      = 0;
        pstParser->u8ActiveCol      = 0;
        pstParser->u8OffsetRow      = 0;
        pstParser->u8OffsetCol      = 0;
        pstParser->u8OriginRow      = 0;
        pstParser->u8OriginCol      = 0;
        pstParser->stAttr.u8ForeColor = TTX_COLOR_UNDEFINED;
        pstParser->stAttr.u8BackColor =  TTX_COLOR_UNDEFINED;
        pstParser->u8Flash          = 0xff;
        pstParser->u8GDRCSSubPage   = 0;
        pstParser->u8NDRCSSubPage   = 0;
        memcpy(pstParser->u8Data,  au8DataLine, TTX_LINE_MEM_SIZE);
        pstParser->pu8Triplet = pstParser->u8Data + 3;
        pstParser->u8TripletPos     = 0;

        *pu32OpenCount  = 1;
        *pu32DesignCode = 1;
    }
    else /** if packet x/26 is not found... */
    {
        *pu32OpenCount = 0;

        for (i = 0; i < 4; i++)
        {
            TTX_Show_ParsePOPPage(i, pstCurrentPoint, pu32OpenCount, au8DataLine);
        }
    }

    return;
}


static HI_VOID TTX_Show_ProcSpaceAttributes(HI_U32 u32CurChar,
        TTX_PAGE_CONTEXT_S* pstCurrentPoint,
        TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{

    TTX_DISP_ATTR* pstLOPAttr = &(pstSpaceAttr->stLOPAttr);

    switch (u32CurChar)
    {
        case 0x0:              /** Set foreground color */
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        {
            pstSpaceAttr->bSetAForeground = HI_TRUE;
            pstSpaceAttr->u32TmpForeground = u32CurChar | pstCurrentPoint->stP28_0_Info.u8ForeCLUT;//s_ColorSet[u32TempCh];

            if (pstSpaceAttr->u32TmpForeground != pstLOPAttr->u8BackColor)
            {
                pstLOPAttr->bConceal = HI_FALSE;    /** The Background color be same to foreground color  , conceal */
            }
            else
            {
                pstLOPAttr->bConceal = HI_TRUE;
            }

            TTX_Show_PutColor(u32CurChar);
        }
        break;

        case 0x08:          /** Flash will begin from the next char */
            //HI_INFO_TTX("Flash1:(%d,%d)", u32LineNum, u32ColumnNum);
            pstSpaceAttr->bSetFlash = HI_TRUE;
            //u8LOPFlash = 1;
            break;

        case 0x09:        /** Flash end */
            //HI_INFO_TTX("Flash0:(%d,%d)", u32LineNum, u32ColumnNum);
            pstSpaceAttr->u8LOPFlash = 0;
            break;

        case 0x0A:
            HI_INFO_TTX("Box0");
            pstSpaceAttr->bStartBox = HI_FALSE;
            break;

        case 0x0B:
            HI_INFO_TTX("Box1");
            pstSpaceAttr->bStartBox = HI_TRUE;
            break;

        case 0x0C:                    /** Normal size  */
            pstLOPAttr->bDHeight = HI_FALSE;
            pstLOPAttr->bDWidth = HI_FALSE;
            pstSpaceAttr->bDoubleSize   = HI_FALSE;
            pstSpaceAttr->bHoldMosaic = HI_FALSE;
            HI_INFO_TTX("NS1");
            break;

        case 0x0D:                /** Double height */
            pstLOPAttr->bDHeight = HI_TRUE;
            pstCurPacketInfo->bLarged = HI_TRUE;
            pstSpaceAttr->bDoubleSize  = HI_FALSE;
            pstSpaceAttr->bHoldMosaic  = HI_FALSE;
            pstCurPacketInfo->u32Fillback += 1;
            HI_INFO_TTX("DH");
            break;

        case 0x0E:          /** Double width  nonsupport for the moment*/
            pstLOPAttr->bDWidth = HI_FALSE;
            pstSpaceAttr->bDoubleSize   = HI_FALSE;
            pstLOPAttr->bDHeight = HI_FALSE;
            pstSpaceAttr->bHoldMosaic = HI_FALSE;
            HI_INFO_TTX("DW");
            break;

        case 0x0F:        /** Double size , treated as double Height*/
            pstSpaceAttr->bDoubleSize = HI_FALSE;
            pstCurPacketInfo->bLarged = HI_TRUE;
            pstLOPAttr->bDWidth  = HI_FALSE;
            pstLOPAttr->bDHeight = HI_TRUE;
            pstSpaceAttr->bHoldMosaic  = HI_FALSE;
            pstCurPacketInfo->u32Fillback += 1;
            HI_INFO_TTX("NS0");
            break;

        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        {
            pstSpaceAttr->bSetMForeground = HI_TRUE;            /** Set foreground color and mosaic mode */
            pstSpaceAttr->u32TmpForeground = (u32CurChar - 0x10) | pstCurrentPoint->stP28_0_Info.u8ForeCLUT;

            if (pstSpaceAttr->u32TmpForeground != pstLOPAttr->u8BackColor)
            {
                pstLOPAttr->bConceal = HI_FALSE;
            }
            else
            {
                pstLOPAttr->bConceal = HI_TRUE;
            }

            TTX_Show_PutColor(u32CurChar);
        }
        break;

        case 0x18:
            pstLOPAttr->bConceal = HI_TRUE;               /** Conceal */
            HI_INFO_TTX("Con1");
            break;

        case 0x19:
            pstSpaceAttr->bContiguous = HI_TRUE;       /** Contiguous mosaic */
            HI_INFO_TTX("Cont1");
            break;

        case 0x1A:
            pstSpaceAttr->bContiguous = HI_FALSE;         /** Seperated  mosaic */
            HI_INFO_TTX("Cont0");
            break;

        case 0x1B:
            pstSpaceAttr->bEsc = HI_TRUE;
            HI_INFO_TTX("Switch G0 Set");
            break;

        case 0x1C:
            if (pstCurPacketInfo->bBlackBgSubstitute)
            {
                pstLOPAttr->u8BackColor = pstCurPacketInfo->u8FullRowColor;
            }
            else
            {
                pstLOPAttr->u8BackColor = pstCurrentPoint->stP28_0_Info.u8BackCLUT; //s_ColorSet[0];      /** Black  background */

            }

            if (pstLOPAttr->u8ForeColor != pstLOPAttr->u8BackColor)
            {
                pstLOPAttr->bConceal = HI_FALSE;
            }
            else
            {
                pstLOPAttr->bConceal = HI_TRUE;
            }

            TTX_Show_PutColor(u32CurChar);
            break;

        case 0x1D:
            pstLOPAttr->u8BackColor = (pstLOPAttr->u8ForeColor & 0x7) | pstCurrentPoint->stP28_0_Info.u8BackCLUT;
            pstLOPAttr->bConceal = HI_TRUE;
            HI_INFO_TTX("SetB");
            break;

        case 0x1E:              /** Hold mosaic */
            pstSpaceAttr->bHoldMosaic = HI_TRUE;
            HI_INFO_TTX("Hold1");
            break;

        case 0x1F:                /** Release mosaic */
            pstCurPacketInfo->bReleaseMos = HI_TRUE;
            HI_INFO_TTX("Hold0");
            break;

        default:
            HI_INFO_TTX("Unkown char");
            break;
    }

    return;
}


static HI_VOID TTX_Show_ParseActiveTriplets(TTX_TRIPLET_PARSER* pstParser, TTX_SHOW_DECHAM24_INFO_S* pstHam24Info)
{
    if (pstHam24Info->u8Addr < 40)
    {
        switch (pstHam24Info->u8Mode)
        {
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x0a: /* triplets for PDC or reserved triplets */
                break;

            default: /* other triplets all effect active position */
                pstParser->u8ActiveCol = pstParser->u8OriginCol + pstHam24Info->u8Addr;
                break;
        }
    }
    else if (pstHam24Info->u8Addr < 64)
    {
        switch (pstHam24Info->u8Mode)
        {
            case 0x00: /* 00000 full screen color */

                /* fall through */
            case 0x01: /* 00001 full row color */
                if (pstParser->u8ObjectType == ACTIVE_OBJECT)
                {
                    pstParser->u8ActiveCol = pstParser->u8OriginCol;
                    pstParser->u8ActiveRow = pstHam24Info->u8Addr - 40;

                    if (pstParser->u8ActiveRow == 0)
                    {
                        pstParser->u8ActiveRow = 24;
                    }

                    pstParser->u8ActiveRow += pstParser->u8OriginRow;
                }

                break;

            case 0x04: /* 00100 set active position */
                pstParser->u8ActiveCol = pstParser->u8OriginCol + pstHam24Info->u8Data;
                pstParser->u8ActiveRow = pstHam24Info->u8Addr - 40;

                if (pstParser->u8ActiveRow == 0)
                {
                    pstParser->u8ActiveRow = 24;
                }

                pstParser->u8ActiveRow += pstParser->u8OriginRow;
                break;

            case 0x07: /* 00111 address display row 0 */
                if (pstParser->u8ObjectType == ACTIVE_OBJECT && pstHam24Info->u8Addr == 0x3f)
                {
                    pstParser->u8ActiveCol = pstParser->u8OriginCol;
                    pstParser->u8ActiveRow = pstParser->u8OriginRow;
                }

                break;
        }
    }

    return;
}


static HI_VOID TTX_Show_PutChar(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                TTX_SHOW_PARSER_INFO_S* pstParserInfo,
                                HI_U8 u8Charset,
                                HI_U8 u8Accent)
{
    HI_U8 u8Data = pstParserInfo->stHam24Info.u8Data;
    HI_U32 u32LineNum = pstCurPacketInfo->u32LineNum;
    HI_U32 u32ColumnNum = pstCurPacketInfo->u32ColumnNum;

    if ((pstParserInfo->stHam24Info.u8Data >= 0x20) &&
        (pstParserInfo->pstParser->u8Priority >= pstParserInfo->stShowPriority.u8CharPriority))
    {
        pstCurPacketInfo->bEnhanceChar = HI_TRUE;

        TTX_SetPgActPos(pstCurrentPoint, u32LineNum, u32ColumnNum, u8Charset, u8Data, u8Accent);

        if (pstParserInfo->pstParser->u8ObjectType == PASSIVE_OBJECT)
        {
            pstCurPacketInfo->stCMBAttr.u8ForeColor = pstParserInfo->pstParser->stAttr.u8ForeColor;
            pstCurPacketInfo->stCMBAttr.u8BackColor = pstParserInfo->pstParser->stAttr.u8BackColor;

            pstParserInfo->stShowPriority.u8ForePriority  = OBJECT_PRIORITY(PASSIVE_OBJECT);
            pstParserInfo->stShowPriority.u8BackPriority  = OBJECT_PRIORITY(PASSIVE_OBJECT);
            pstParserInfo->stShowPriority.u8AttrPriority  = OBJECT_PRIORITY(PASSIVE_OBJECT);
            pstParserInfo->stShowPriority.u8FlashPriority = OBJECT_PRIORITY(PASSIVE_OBJECT);
        }

        pstParserInfo->stShowPriority.u8CharPriority = pstParserInfo->pstParser->u8Priority;
    }

}


static HI_VOID TTX_Show_ProcColumnMode00(TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_SHOW_PARSER_INFO_S* pstParserInfo,
        TTX_DISP_ATTR* pstLOPAttr)
{
    if (!(pstParserInfo->stHam24Info.u8Data & 0x60))
    {
        if (ACTIVE_OBJECT == pstParserInfo->pstParser->u8ObjectType)
        {
            pstLOPAttr->u8ForeColor = pstParserInfo->stHam24Info.u8Data & 0x1f;

            if (pstParserInfo->stShowPriority.u8ForePriority <= pstParserInfo->pstParser->u8Priority)
            {
                pstCurPacketInfo->stCMBAttr.u8ForeColor  = pstLOPAttr->u8ForeColor;
                pstParserInfo->stShowPriority.u8ForePriority = pstParserInfo->pstParser->u8Priority;
            }
        }
        else
        {
            pstParserInfo->pstParser->stAttr.u8ForeColor = pstParserInfo->stHam24Info.u8Data & 0x1f;
        }
    }

    return;
}


static HI_VOID TTX_Show_ProcColumnMode03(TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_SHOW_PARSER_INFO_S* pstParserInfo,
        TTX_DISP_ATTR* pstLOPAttr)
{
    if (pstParserInfo->pstParser->u8ObjectType == ACTIVE_OBJECT)
    {
        pstLOPAttr->u8BackColor = pstParserInfo->stHam24Info.u8Data & 0x1f;

        if (pstParserInfo->stShowPriority.u8BackPriority <= pstParserInfo->pstParser->u8Priority)
        {
            pstCurPacketInfo->stCMBAttr.u8BackColor = pstLOPAttr->u8BackColor;
            pstParserInfo->stShowPriority.u8BackPriority = pstParserInfo->pstParser->u8Priority;
        }
    }
    else
    {
        pstParserInfo->pstParser->stAttr.u8BackColor = pstParserInfo->stHam24Info.u8Data & 0x1f;
    }

    return;
}


static HI_VOID TTX_Show_ProcColumnMode07(TTX_SHOW_PARSER_INFO_S* pstParserInfo, HI_U8* pu8LOPFlash)
{
    if (pstParserInfo->pstParser->u8ObjectType == ACTIVE_OBJECT)
    {
        if (pstParserInfo->stShowPriority.u8FlashPriority <= pstParserInfo->pstParser->u8Priority)
        {
            *pu8LOPFlash = pstParserInfo->stHam24Info.u8Data & 0x1f;
            pstParserInfo->stShowPriority.u8FlashPriority = pstParserInfo->pstParser->u8Priority;
        }
    }
    else
    {
        pstParserInfo->pstParser->u8Flash = pstParserInfo->stHam24Info.u8Data & 0x1f;
    }

    return;
}

static HI_VOID TTX_Show_ProcColumnMode0C(TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_SHOW_PARSER_INFO_S* pstParserInfo,
        TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    HI_U8 u8Data = pstParserInfo->stHam24Info.u8Data;
    TTX_TRIPLET_PARSER* pstParser = pstParserInfo->pstParser;
    TTX_DISP_ATTR* pstLOPAttr = &(pstSpaceAttr->stLOPAttr);


    if (pstParserInfo->pstParser->u8ObjectType == ACTIVE_OBJECT)
    {

        pstLOPAttr->bDHeight = (u8Data & 0x1) ? HI_TRUE : HI_FALSE;
        pstLOPAttr->bBox = (u8Data & 0x2) ? HI_TRUE : HI_FALSE;
        pstLOPAttr->bConceal = (u8Data & 0x4) ? HI_TRUE : HI_FALSE;
        pstLOPAttr->bUnderline = (u8Data & 0x20) ? HI_TRUE : HI_FALSE;
        pstLOPAttr->bInvert = (u8Data & 0x10) ? HI_TRUE : HI_FALSE;
        pstLOPAttr->bSeparated = (u8Data & 0x20) ? HI_TRUE : HI_FALSE;
        pstLOPAttr->bDWidth = (u8Data & 0x40) ? HI_TRUE : HI_FALSE;

        if (!pstLOPAttr->bSeparated)
        {
            pstSpaceAttr->bContiguous = HI_TRUE; /* level one separated-mosaic can be canceled by local enhancement data or active objects */
        }

        if (pstParserInfo->stShowPriority.u8AttrPriority <= pstParser->u8Priority)
        {
            pstCurPacketInfo->stCMBAttr.bDHeight = pstLOPAttr->bDHeight ;
            pstCurPacketInfo->stCMBAttr.bBox = pstLOPAttr->bBox;
            pstCurPacketInfo->stCMBAttr.bConceal = pstLOPAttr->bConceal;
            pstCurPacketInfo->stCMBAttr.bUnderline = pstLOPAttr->bUnderline;
            pstCurPacketInfo->stCMBAttr.bInvert = pstLOPAttr->bInvert;
            pstCurPacketInfo->stCMBAttr.bSeparated = pstLOPAttr->bSeparated;
            pstCurPacketInfo->stCMBAttr.bDWidth =  pstLOPAttr->bDWidth;
            pstParserInfo->stShowPriority.u8AttrPriority = pstParserInfo->pstParser->u8Priority;
        }
    }
    else
    {
        pstParser->stAttr.bDHeight = (u8Data & 0x1) ? HI_TRUE : HI_FALSE;
        pstParser->stAttr.bBox = (u8Data & 0x2) ? HI_TRUE : HI_FALSE;
        pstParser->stAttr.bConceal = (u8Data & 0x4) ? HI_TRUE : HI_FALSE;
        pstParser->stAttr.bUnderline = (u8Data & 0x20) ? HI_TRUE : HI_FALSE;
        pstParser->stAttr.bInvert = (u8Data & 0x10) ? HI_TRUE : HI_FALSE;
        pstParser->stAttr.bSeparated = (u8Data & 0x20) ? HI_TRUE : HI_FALSE;
        pstParser->stAttr.bDWidth = (u8Data & 0x40) ? HI_TRUE : HI_FALSE;


        pstParser->stAttr.bChanged = HI_TRUE;
    }

    return;
}


static HI_VOID TTX_Show_ProcColumnMode0D(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
        TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_SHOW_PARSER_INFO_S* pstParserInfo)
{
    HI_U8 u8Data = pstParserInfo->stHam24Info.u8Data;
    TTX_TRIPLET_PARSER* pstParser = pstParserInfo->pstParser;

    if (pstParser->u8Priority >= pstParserInfo->stShowPriority.u8CharPriority)
    {
        if (u8Data & 0x40)
        {
            if (pstCurrentPoint->stMOTInfo.stNDRCS.u8PageNum != 0xff)
            {
                pstCurPacketInfo->u8CMBDRCSChar    = u8Data;/*u8CMBDRCSChar:is the PTU num*/
                pstCurPacketInfo->u8CMBDRCSSubPage = pstParser->u8NDRCSSubPage;/*CMBDRCSSubPage is the subpage num, which the PTU be defined*/
            }
        }
        else
        {
            if (pstCurrentPoint->stMOTInfo.stGDRCS.u8PageNum != 0xff)
            {
                pstCurPacketInfo->u8CMBDRCSChar    = u8Data;
                pstCurPacketInfo->u8CMBDRCSSubPage = pstParser->u8GDRCSSubPage;
            }
        }

        if (pstCurPacketInfo->u8CMBDRCSChar != 0xff)
        {
            if (pstParser->u8ObjectType == PASSIVE_OBJECT)
            {
                pstCurPacketInfo->stCMBAttr         = pstParser->stAttr;
                pstParserInfo->stShowPriority.u8ForePriority  = OBJECT_PRIORITY(PASSIVE_OBJECT);
                pstParserInfo->stShowPriority.u8BackPriority  = OBJECT_PRIORITY(PASSIVE_OBJECT);
                pstParserInfo->stShowPriority.u8AttrPriority  = OBJECT_PRIORITY(PASSIVE_OBJECT);
                pstParserInfo->stShowPriority.u8FlashPriority = OBJECT_PRIORITY(PASSIVE_OBJECT);
            }

            pstParserInfo->stShowPriority.u8CharPriority = pstParser->u8Priority;
        }
    }

    return;
}


static HI_VOID TTX_Show_ProcColumn(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                   TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                   TTX_SHOW_PARSER_INFO_S* pstParserInfo,
                                   TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    HI_U8 u8Charset = 0;
    HI_U8 u8Accent = 2;


    if (pstParserInfo->stHam24Info.u8Mode > 0x10) /* 10001 ~ 11111 G0 character with diacritical mark */
    {
        u8Charset = 0;
        u8Accent = pstParserInfo->stHam24Info.u8Mode & 0x0f;
        TTX_Show_PutChar(pstCurrentPoint, pstCurPacketInfo, pstParserInfo, u8Charset, u8Accent);
    }
    else
    {
        switch (pstParserInfo->stHam24Info.u8Mode)
        {
            case 0x09: /* 01001 character from the G0 set ( level 2.5 & 3.5 ) */
            case 0x10: /* 10000 G0 character without diacritical mark */
            {
                if (pstParserInfo->stHam24Info.u8Data == 0x2A)
                {
                    /* replace packet x/26 character '*' by '@' in case of no diacritical mark */

                    pstParserInfo->stHam24Info.u8Data = '@';
                }

                u8Charset = 0;
                u8Accent = 0;
                TTX_Show_PutChar(pstCurrentPoint, pstCurPacketInfo, pstParserInfo, u8Charset, u8Accent);
            }
            break;

            case 0x01: /* 00001 block mosaic character from the G1 set */
            {
                u8Charset = 1;
                /* patch: see ETSI EN 300 706 15.7.1 NOTE 3 */
                //if (u8Charset < 14) u8Charset = 0;
                u8Accent = 0;
                TTX_Show_PutChar(pstCurrentPoint, pstCurPacketInfo, pstParserInfo, u8Charset, u8Accent);
            }
            break;

            case 0x0f: /* 01111 character from the G2 set */
            {
                u8Charset = 2;
                u8Accent = 0;
                TTX_Show_PutChar(pstCurrentPoint, pstCurPacketInfo, pstParserInfo, u8Charset, u8Accent);
            }
            break;

            case 0x02: /* 00010 line drawing or smoothed mosaic character form the G3 set ( level 1.5 ) */
            case 0x0b: /* 01011 line drawing or smoothed mosaic character from the G3 set ( level 2.5 & 3.5 ) */
            {
                u8Charset = 3;
                u8Accent = 0;
                TTX_Show_PutChar(pstCurrentPoint, pstCurPacketInfo, pstParserInfo, u8Charset, u8Accent);
            }
            break;

            case 0x00: /* 00000 foreground color */
            {
                TTX_Show_ProcColumnMode00(pstCurPacketInfo, pstParserInfo, &(pstSpaceAttr->stLOPAttr));
            }
            break;

            case 0x03: /* 00011 background color */
            {
                TTX_Show_ProcColumnMode03(pstCurPacketInfo, pstParserInfo, &(pstSpaceAttr->stLOPAttr));
            }
            break;

            case 0x0c: /* 01100 display attributes */
            {
                TTX_Show_ProcColumnMode0C(pstCurPacketInfo, pstParserInfo, pstSpaceAttr);
            }
            break;

            case 0x07: /* 00111 additional flash functions */
            {
                TTX_Show_ProcColumnMode07(pstParserInfo, &(pstSpaceAttr->u8LOPFlash));
            }
            break;

            case 0x08: /* 01000 modified G0 and G2 character design */
            {
                HI_UNF_TTX_G0SET_E* penModifiedG0Set = &(pstCurPacketInfo->stFontInfo.enModifiedG0Set);
                HI_UNF_TTX_G2SET_E* penModifiedG2Set = &(pstCurPacketInfo->stFontInfo.enModifiedG2Set);
                TTX_Show_GetModifiedG0G2((pstParserInfo->stHam24Info.u8Data & 0x7f), penModifiedG0Set, penModifiedG2Set);
                pstCurPacketInfo->bModifiedG0G2 = HI_TRUE;
            }
            break;

            case 0x0d: /* 01101 DRCS character invokation */
            {
                TTX_Show_ProcColumnMode0D(pstCurrentPoint, pstCurPacketInfo, pstParserInfo);
            }
            break;

            case 0x0e: /* 01110 font style */
            {
                /* only for level 3.5 */
            }
            break;
        }
    }

    return;
}


static HI_VOID TTX_Show_ProcRow00(TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                  TTX_SHOW_PARSER_INFO_S* pstParserInfo,
                                  TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    HI_U8 u8Data = pstParserInfo->stHam24Info.u8Data;
    TTX_TRIPLET_PARSER* pstParser = pstParserInfo->pstParser;

    if (pstParser->u8ObjectType == ACTIVE_OBJECT)
    {
        if (!(u8Data & 0xe0))
        {
            pstCurPacketInfo->u8DefRowColor = u8Data;
            pstCurPacketInfo->u8FullRowColor = pstCurPacketInfo->u8DefRowColor;

            if (pstCurPacketInfo->bBlackBgSubstitute)
            {
                pstSpaceAttr->stLOPAttr.u8BackColor = pstCurPacketInfo->u8FullRowColor;
                pstCurPacketInfo->stCMBAttr.u8BackColor = pstCurPacketInfo->u8FullRowColor;
            }
        }
    }

    return;
}

static HI_VOID TTX_Show_ProcRow01(TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                  TTX_SHOW_PARSER_INFO_S* pstParserInfo,
                                  TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    HI_U8 u8Data = pstParserInfo->stHam24Info.u8Data;
    TTX_TRIPLET_PARSER* pstParser = pstParserInfo->pstParser;

    if (pstParser->u8ObjectType == ACTIVE_OBJECT)
    {
        switch (u8Data & 0x60) /* s1 s0, bit6 bit5 */
        {
            case 0x60:
                pstCurPacketInfo->u8DefRowColor = u8Data & 0x1f;

                /* fall through */
            case 0x00:
                pstCurPacketInfo->u8FullRowColor = u8Data & 0x1f;

                if (pstCurPacketInfo->bBlackBgSubstitute)
                {
                    pstSpaceAttr->stLOPAttr.u8BackColor = pstCurPacketInfo->u8FullRowColor;
                    pstCurPacketInfo->stCMBAttr.u8BackColor = pstCurPacketInfo->u8FullRowColor;
                }

                break;
        }
    }

    return;
}

static HI_VOID TTX_Show_ProcRow11(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                  TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                  TTX_SHOW_PARSER_INFO_S* pstParserInfo)
{
    HI_U8 u8Data = pstParserInfo->stHam24Info.u8Data;
    HI_U8 u8Addr = pstParserInfo->stHam24Info.u8Addr;
    HI_U8 u8Mode = pstParserInfo->stHam24Info.u8Mode;

    TTX_TRIPLET_PARSER* pstParser = pstParserInfo->pstParser;

    if (pstCurPacketInfo->u32OpenCount >= TTX_MAX_OPEN_OBJECTS)
    {
        /* object buffer is full, try to clean up */
        TTX_TRIPLET_PARSER* p;
        TTX_TRIPLET_PARSER* q = g_stTripParser;
        HI_U32 m, n = 0;

        for (m = 0; m < pstCurPacketInfo->u32OpenCount; m++)
        {
            p = g_stTripParser + m;

            if (p->bOpen)
            {
                if (p != q)
                {
                    memcpy(q, p, sizeof(TTX_TRIPLET_PARSER));

                    if (m == pstParserInfo->u32Parser)
                    {
                        pstParserInfo->u32Parser = n;
                        pstParser = q;
                    }
                }

                n++;
                q = g_stTripParser + n;
            }
        }

        pstCurPacketInfo->u32OpenCount = n;
    }

    if (pstCurPacketInfo->u32OpenCount < TTX_MAX_OPEN_OBJECTS)
    {
        TTX_TRIPLET_PARSER* p = g_stTripParser + pstCurPacketInfo->u32OpenCount;
        p->stAttr.bDHeight = HI_FALSE;
        p->stAttr.bBox = HI_FALSE;
        p->stAttr.bConceal = HI_FALSE;
        p->stAttr.bUnderline = HI_FALSE;
        p->stAttr.bInvert = HI_FALSE;
        p->stAttr.bSeparated = HI_FALSE;
        p->stAttr.bDWidth = HI_FALSE;
        p->stAttr.bChanged = HI_FALSE;

        p->bRowOpen         = HI_FALSE;
        p->bStartDefine     = HI_TRUE;
        p->u8ObjectType     = u8Mode & 0x0f;
        p->u8Priority       = OBJECT_PRIORITY(p->u8ObjectType);
        p->u8ActiveRow      = pstParser->u8OffsetRow + pstCurPacketInfo->u32LineNum;
        p->u8ActiveCol      = pstParser->u8OffsetCol + pstCurPacketInfo->u32ColumnNum;
        p->u8OffsetRow      = 0;
        p->u8OffsetCol      = 0;
        p->u8OriginRow      = pstParser->u8OffsetRow + pstCurPacketInfo->u32LineNum;
        p->u8OriginCol      = pstParser->u8OffsetCol + pstCurPacketInfo->u32ColumnNum;
        p->stAttr.u8ForeColor = p->u8ObjectType == PASSIVE_OBJECT ? TTX_COLOR_WHITE : 0xff;
        p->stAttr.u8BackColor = p->u8ObjectType == PASSIVE_OBJECT ? TTX_COLOR_BLACK : 0xff;
        p->u8Flash          = p->u8ObjectType == PASSIVE_OBJECT ? 0x00 : 0xff;
        p->u8GDRCSSubPage   = 0;
        p->u8NDRCSSubPage   = 0;

        if (u8Addr & 0x10) /* POP or GPOP object */
        {
            TTX_OBJECT_POINTER_S  ObjPtr;
            HI_U32            u32Pos;

            ObjPtr.u8ObjectType = u8Mode & 0x0f;
            ObjPtr.u8SubPageNum = u8Data & 0x0f;
            ObjPtr.u8PointerTab = u8Addr & 0x03;
            u8Data >>= 4;
            ObjPtr.u8PointerPos = u8Data & 0x1;
            u8Data >>= 1;
            ObjPtr.u8TripletNum = u8Data + (u8Data << 1) + (u8Mode & 0x0f);

            if (HI_SUCCESS == TTX_DecodePOPPage(u8Addr & 0x8 ? &pstCurrentPoint->stMOTInfo.stGPOP : &pstCurrentPoint->stMOTInfo.stNPOP, &ObjPtr, &u32Pos, pstCurPacketInfo->au8DataLine))
            {
                if (0xff != u32Pos)
                {
                    memcpy(p->u8Data,  pstCurPacketInfo->au8DataLine, TTX_LINE_MEM_SIZE);
                    p->pu8Triplet = p->u8Data + 3 + u32Pos * 3;
                    p->u8TripletPos     = (HI_U8)u32Pos;
                    p->bOpen        = HI_TRUE;
                    pstCurPacketInfo->u32OpenCount++;
                }
            }
        }
        else /* local object */
        {

            if (HI_SUCCESS == TTX_Data_FindPacket(pstCurrentPoint->pstCurPage, 26, ((u8Data >> 4) & 0x7) + ((u8Addr & 0x1 ) << 3), pstCurPacketInfo->au8DataLine))
            {
                p->u8TripletPos = u8Data & 0x0f;

                if (p->u8TripletPos < 13)
                {
                    memcpy(p->u8Data,  pstCurPacketInfo->au8DataLine, TTX_LINE_MEM_SIZE);
                    p->pu8Triplet = p->u8Data + 3 + p->u8TripletPos * 3;
                    p->bOpen        = HI_TRUE;
                    pstCurPacketInfo->u32OpenCount++;
                }
            }

        }

        pstParser->u8OffsetRow = pstParser->u8OffsetCol = 0;
    }


    return;
}


static HI_VOID TTX_Show_ProcRow18(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                  TTX_SHOW_PARSER_INFO_S* pstParserInfo)
{
    HI_U8 u8Data = pstParserInfo->stHam24Info.u8Data;

    TTX_TRIPLET_PARSER* pstParser = pstParserInfo->pstParser;

    if (u8Data & 0x40) /* normal DRCS */
    {
        pstParser->u8NDRCSSubPage = u8Data & 0x0f;
        pstCurrentPoint->stMOTInfo.stNDRCS.u16PageSubCode = pstParser->u8NDRCSSubPage;
    }
    else /* global DRCS */
    {
        pstParser->u8GDRCSSubPage = u8Data & 0x0f;
        pstCurrentPoint->stMOTInfo.stGDRCS.u16PageSubCode = pstParser->u8GDRCSSubPage;

    }

    return;
}


static HI_VOID TTX_Show_ProcRow(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                TTX_SHOW_PARSER_INFO_S* pstParserInfo,
                                TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    TTX_TRIPLET_PARSER* pstParser = pstParserInfo->pstParser;

    switch (pstParserInfo->stHam24Info.u8Mode)
    {
        case 0x00: /* 00000 full screen color */
            TTX_Show_ProcRow00(pstCurPacketInfo, pstParserInfo, pstSpaceAttr);
            break;

        case 0x07: /* 00111 address display row 0 */
            if (pstParserInfo->stHam24Info.u8Addr != 0x3f) { break; }

            /* fall through */

        case 0x01: /* 00001 full row color */
            TTX_Show_ProcRow01(pstCurPacketInfo, pstParserInfo, pstSpaceAttr);
            break;

        case 0x10: /* 10000 origin modifier */
            pstParser->u8OffsetRow = pstParserInfo->stHam24Info.u8Addr - 40;
            pstParser->u8OffsetCol = pstParserInfo->stHam24Info.u8Data;
            break;

        case 0x11: /* 10001 active object invokation */
        case 0x12: /* 10001 adaptive object invokation */
        case 0x13: /* 10001 passive object invokation */
            TTX_Show_ProcRow11(pstCurrentPoint, pstCurPacketInfo, pstParserInfo);
            break;

        case 0x18: /* 11000 DRCS mode */
            TTX_Show_ProcRow18(pstCurrentPoint, pstParserInfo);
            break;

        case 0x15: /* 10101 active object definition */
        case 0x16: /* 10110 adaptive object definition */
        case 0x17: /* 10111 passive object definition */
            if (pstParser->bStartDefine)
            { pstParser->bStartDefine = HI_FALSE; }
            else
            { pstParser->bOpen = HI_FALSE; }

            break;

        case 0x1f: /* 11111 termination marker */
            pstParser->bOpen = HI_FALSE;
            break;
    }


    return;
}

static HI_S32 TTX_Show_ProcCurPosition(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                       TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                       TTX_SHOW_PARSER_INFO_S* pstParserInfo,
                                       TTX_SHOW_SPACEATTR_S* pstSpaceAttr,
                                       HI_BOOL* pbRowOpen)
{
    *pbRowOpen = HI_TRUE;

    pstParserInfo->pstParser->bRowOpen = HI_TRUE;

    if (pstParserInfo->stHam24Info.u8Addr < 40) /* column address */
    {
        TTX_Show_ProcColumn(pstCurrentPoint, pstCurPacketInfo, pstParserInfo, pstSpaceAttr);
    }
    else if (pstParserInfo->stHam24Info.u8Addr < 64) /* row address */
    {
        TTX_Show_ProcRow(pstCurrentPoint, pstCurPacketInfo, pstParserInfo, pstSpaceAttr);
    }
    else /* invalid row address... */
    {
        /* error */
        pstParserInfo->pstParser->bOpen = HI_FALSE;
        return HI_FAILURE;
    }


    return HI_SUCCESS;
}


static HI_S32 TTX_Show_ProcActiveTriplets26(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
        TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_TRIPLET_PARSER* pstParser)
{
    if ((pstParser->u8ActiveCol >= 0x48) && (pstParser->u8ActiveRow <= pstCurPacketInfo->u32LineNum))
    {
        /* read the next triplet */
        pstParser->u8TripletPos++;

        if (pstParser->u8TripletPos >= 13)
        {
            /* goto the next packet */
            if (HI_SUCCESS == TTX_Data_FindPacket(pstCurrentPoint->pstCurPage, 26, pstCurPacketInfo->u32DesignCode, pstCurPacketInfo->au8DataLine))
            {
                pstParser->u8TripletPos = 0;
                memcpy(pstParser->u8Data,  pstCurPacketInfo->au8DataLine, TTX_LINE_MEM_SIZE);
                pstParser->pu8Triplet = pstParser->u8Data + 3;
                pstCurPacketInfo->u32DesignCode++;
            }
            else
            {
                /* error */
                pstParser->bOpen = HI_FALSE;
                return HI_FAILURE;
            }
        }
        else
        {
            pstParser->pu8Triplet += 3;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 TTX_Show_ProcPrePosition(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                       TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                       TTX_TRIPLET_PARSER* pstParser,
                                       HI_U32 u32Cell)
{
    if ((((HI_U32)pstParser->u8ActiveRow << 8) + pstParser->u8ActiveCol) <= u32Cell)
    {
        /* read the next triplet */
        pstParser->u8TripletPos++;

        if (pstParser->u8TripletPos >= 13)
        {
            /* goto the next packet */
            if (HI_SUCCESS == TTX_Data_FindPacket(pstCurrentPoint->pstCurPage, 26, pstCurPacketInfo->u32DesignCode, pstCurPacketInfo->au8DataLine))
            {
                pstParser->u8TripletPos = 0;
                memcpy(pstParser->u8Data,  pstCurPacketInfo->au8DataLine, TTX_LINE_MEM_SIZE);
                pstParser->pu8Triplet = pstParser->u8Data + 3;
                pstCurPacketInfo->u32DesignCode++;
            }
            else
            {
                /* error */
                pstParser->bOpen = HI_FALSE;
                return HI_FAILURE;
            }
        }
        else
        {
            pstParser->pu8Triplet += 3;
        }
    }

    return HI_SUCCESS;
}

static HI_VOID TTX_Show_SetLOPAttributes(TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_SHOW_PARSER_INFO_S* pstParserInfo,
        HI_BOOL bRowOpen)
{
    HI_U8 u8ForePriority = pstParserInfo->stShowPriority.u8AttrPriority;
    HI_U8 u8BackPriority = pstParserInfo->stShowPriority.u8BackPriority;
    HI_U8 u8AttrPriority = pstParserInfo->stShowPriority.u8AttrPriority;
    HI_U8 u8FlashPriority = pstParserInfo->stShowPriority.u8FlashPriority;

    TTX_DISP_ATTR* pstCMBAttr = &(pstCurPacketInfo->stCMBAttr);
    TTX_TRIPLET_PARSER* pstParser = pstParserInfo->pstParser;

    if (pstParser->u8ObjectType == ADAPTIVE_OBJECT && bRowOpen)
    {
        if (pstParser->stAttr.u8ForeColor != 0xff && u8ForePriority <= OBJECT_PRIORITY(ADAPTIVE_OBJECT))
        {
            pstCMBAttr->u8ForeColor = pstParser->stAttr.u8ForeColor;
            u8ForePriority = OBJECT_PRIORITY(ADAPTIVE_OBJECT);
        }

        if (pstParser->stAttr.u8BackColor != 0xff && u8BackPriority <= OBJECT_PRIORITY(ADAPTIVE_OBJECT))
        {
            pstCMBAttr->u8BackColor = pstParser->stAttr.u8BackColor;
            u8BackPriority = OBJECT_PRIORITY(ADAPTIVE_OBJECT);
        }

        if (pstParser->stAttr.bChanged && u8AttrPriority <= OBJECT_PRIORITY(ADAPTIVE_OBJECT))
        {
            pstCMBAttr->bDHeight = pstParser->stAttr.bDHeight ;
            pstCMBAttr->bBox = pstParser->stAttr.bBox;
            pstCMBAttr->bConceal = pstParser->stAttr.bConceal;
            pstCMBAttr->bUnderline = pstParser->stAttr.bUnderline;
            pstCMBAttr->bInvert = pstParser->stAttr.bInvert;
            pstCMBAttr->bSeparated = pstParser->stAttr.bSeparated;
            pstCMBAttr->bDWidth =  pstParser->stAttr.bDWidth;

            u8AttrPriority = OBJECT_PRIORITY(ADAPTIVE_OBJECT);
        }

        if (pstParser->u8Flash != 0xff && u8FlashPriority <= OBJECT_PRIORITY(ADAPTIVE_OBJECT))
        {
            u8FlashPriority = OBJECT_PRIORITY(ADAPTIVE_OBJECT);
        }
    }

    return;
}


static HI_S32 TTX_Show_ProcSidePanel(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                     TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                     TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    HI_U32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_FLAAREA_S*  pstFlashArea = NULL;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_FILLRECT_S stFillRectParam;


    HI_U32 u32X = pstCurPacketInfo->u32X;
    HI_U32 u32LineNum = pstCurPacketInfo->u32LineNum;
    HI_U32 u32ColumnNum = pstCurPacketInfo->u32ColumnNum;
    HI_BOOL bDoubleSize = pstSpaceAttr->bDoubleSize;

    TTX_DISP_ATTR stCMBAttr = pstCurPacketInfo->stCMBAttr;
    TTX_SHOW_FONT_INFO_S* pstFontInfo = &(pstCurPacketInfo->stFontInfo);

    HI_U32 u32CurrentCh = pstCurrentPoint->u32aPageText[u32LineNum][u32ColumnNum];

    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));

    if (u32CurrentCh == 0xff)     /**  invalid  char */
    {
        s32Ret = TTX_Show_SetArea(&stPageArea, u32LineNum, u32X, 1, 1);
        stFillRectParam.pstPageArea = &stPageArea;
        stFillRectParam.u32Color = TTX_GET_COLOR(stCMBAttr.u8BackColor);

        s32Ret |= TTT_Show_FillRect(pstCurrentPoint, &stFillRectParam);

        return HI_FAILURE;
    }

    if (TTX_Show_IsMosaicChar(pstCurPacketInfo->bMosaic, u32CurrentCh))
    {
        pstCurPacketInfo->u32MosaicCh = u32CurrentCh;
    }

    pstCurPacketInfo->au32BackgroundRec[u32ColumnNum] =  TTX_GET_COLOR(stCMBAttr.u8BackColor);

    if (pstCurrentPoint->bFlash && pstSpaceAttr->u8LOPFlash && (!TTX_Show_IsSpaceChar(u32CurrentCh)))
    {
        pstFlashArea = (TTX_FLAAREA_S*)malloc(sizeof(TTX_FLAAREA_S));   /**  Record a flash area except space */

        if (HI_NULL != pstFlashArea)
        {
            pstFlashArea->u8LineNum = (HI_U8) u32LineNum;
            pstFlashArea->u8ColNum = u32X;
            pstFlashArea->u8DoubleHeight = stCMBAttr.bDHeight;
            pstFlashArea->u8DoubleWidth = stCMBAttr.bDWidth;
            pstFlashArea->u8DoubleSize = bDoubleSize;
            pstFontInfo->enCharSet = TTX_Show_GetCharSet(u32CurrentCh, pstCurPacketInfo->bMosaic);

            if (pstCurPacketInfo->bModifiedG0G2 && pstCurPacketInfo->bEnhanceChar)
            {
                pstFontInfo->enG0Set = pstFontInfo->enModifiedG0Set;
                pstFontInfo->enG2Set = pstFontInfo->enModifiedG2Set;
                pstCurPacketInfo->bEnhanceChar = HI_FALSE;
            }
            else
            {
                pstFontInfo->enG0Set = pstFontInfo->enLOPG0Set;
                pstFontInfo->enG2Set = pstFontInfo->enLOPG2Set;
            }


            TTX_Show_SetCharAttr(&(pstFlashArea->stCharAttr), P_CHARACTER(u32CurrentCh), pstSpaceAttr->bContiguous,
                                 pstFontInfo->enG0Set, pstFontInfo->enG2Set, pstFontInfo->enCharSet, pstFontInfo->enNationSet);

            pstFlashArea->ForegdCol = TTX_GET_COLOR(stCMBAttr.u8ForeColor);
            pstFlashArea->BackgdCol = TTX_GET_COLOR(stCMBAttr.u8BackColor);
            pstFlashArea->pstNext = HI_NULL;
            TTX_Show_ADD_FlashArea(&(pstCurrentPoint->pFlaAreaHead), pstFlashArea);
        }
    }

    if ((pstCurPacketInfo->u32Fillback == 1) && (stCMBAttr.bDHeight || bDoubleSize)) /** Double height or double size , fill  the space of lower line*/
    {
        if (!pstCurrentPoint->bTtxMix)
        {
            for (i = 0; i <= u32X; i++)
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, u32LineNum + 1, i, 1, 1);
                stFillRectParam.pstPageArea = &stPageArea;
                stFillRectParam.u32Color = pstCurPacketInfo->au32BackgroundRec[i];
                s32Ret |= TTT_Show_FillRect(pstCurrentPoint, &stFillRectParam);
            }
        }

        pstCurPacketInfo->u32Fillback += 1;
    }

    if ((pstCurPacketInfo->u32Fillback > 1) && (!stCMBAttr.bDHeight) && (!bDoubleSize))  /** Restore to normal size ,fill the space in the next line  */
    {
        if (!pstCurrentPoint->bTtxMix)   /** Not mix*/
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, u32LineNum + 1, u32X, 1, 1);
            stFillRectParam.pstPageArea = &stPageArea;
            stFillRectParam.u32Color = TTX_GET_COLOR(stCMBAttr.u8BackColor);
            s32Ret |= TTT_Show_FillRect(pstCurrentPoint, &stFillRectParam);
        }
    }

    return HI_SUCCESS;
}


static HI_VOID TTX_Show_DrawDRCSChar(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                     TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                     TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    HI_U32 i = 0;
    HI_U32 k = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    HI_U32 u32ColCount = 0;
    HI_U32 u32RowCount = 0;
    HI_U32 au32DRCSColorInfo[120];
    TTX_DRCS_MODE_E enDRCSMode = TTX_DRCS_MODE_BUTT;

    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_DRAWDRCSCHAR_S stDRCSCharParam;
    HI_UNF_TTX_FILLRECT_S stFillRectParam;
    HI_UNF_TTX_PAGE_ADDR_S* pstPage = HI_NULL;

    HI_U32 u32X = pstCurPacketInfo->u32X;
    HI_U8 u8CMBDRCSChar = pstCurPacketInfo->u8CMBDRCSChar;
    HI_U8 u8CMBDRCSSubPage = pstCurPacketInfo->u8CMBDRCSSubPage;
    TTX_DISP_ATTR stCMBAttr = pstCurPacketInfo->stCMBAttr;


    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDRCSCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWDRCSCHAR_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));

    if (u8CMBDRCSChar & 0x40)/*normal DRCS*/
    {
        pstCurrentPoint->stMOTInfo.stNDRCS.u16PageSubCode = u8CMBDRCSSubPage;
        pstPage = &pstCurrentPoint->stMOTInfo.stNDRCS;
    }
    else
    {
        pstCurrentPoint->stMOTInfo.stGDRCS.u16PageSubCode = u8CMBDRCSSubPage;
        pstPage = &pstCurrentPoint->stMOTInfo.stGDRCS;
    }

    if (HI_SUCCESS == TTX_DecodeDRCSPage(pstCurrentPoint, pstPage, u8CMBDRCSChar, &enDRCSMode, au32DRCSColorInfo))
    {
        if (TTX_DRCS_MODE_0 == enDRCSMode)
        {

            for (i = 0; i < 120; i++)
            {
                k = (!(*(au32DRCSColorInfo + i))) ? stCMBAttr.u8ForeColor : stCMBAttr.u8BackColor;
                *(au32DRCSColorInfo + i) = TTX_GET_COLOR(k);
            }
        }


        /*draw DRCS char*/
        if (stCMBAttr.bConceal && pstCurrentPoint->bConceal)
        {
            if (!pstCurrentPoint->bTtxMix)   /**  Not mix */
            {
                u32ColCount = ((stCMBAttr.bDWidth || pstSpaceAttr->bDoubleSize) ? 2 : 1);
                u32RowCount = ((stCMBAttr.bDHeight || pstSpaceAttr->bDoubleSize) ? 2 : 1);
                s32Ret |= TTX_Show_SetArea(&stPageArea, pstCurPacketInfo->u32LineNum, u32X, u32RowCount, u32ColCount);
                stFillRectParam.pstPageArea = &stPageArea;
                stFillRectParam.u32Color = TTX_GET_COLOR(stCMBAttr.u8BackColor);
                s32Ret |= TTT_Show_FillRect(pstCurrentPoint, &stFillRectParam);
            }
        }
        else
        {
            u32ColCount = ((stCMBAttr.bDWidth || pstSpaceAttr->bDoubleSize) ? 2 : 1);
            u32RowCount = ((stCMBAttr.bDHeight || pstSpaceAttr->bDoubleSize) ? 2 : 1);

            s32Ret |= TTX_Show_SetArea(&stPageArea, pstCurPacketInfo->u32LineNum, u32X, u32RowCount, u32ColCount);
            stDRCSCharParam.pstPageArea   = &stPageArea;

            if (pstCurrentPoint->bTtxMix)
            {
                stDRCSCharParam.u32Background = 0x00000000;
            }
            else
            {
                stDRCSCharParam.u32Background = TTX_GET_COLOR(stCMBAttr.u8BackColor);
            }

            stDRCSCharParam.pu32DRCSColorInfo = au32DRCSColorInfo;
            stDRCSCharParam.enDRCSSize = (enDRCSMode < TTX_DRCS_MODE_3) ? HI_UNF_TTX_DRCS_SIZE_NORMAL : HI_UNF_TTX_DRCS_SIZE_SMALL;
            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWDRCSCHAR, (HI_VOID*)&stDRCSCharParam);
        }

    }

    return;
}



static HI_VOID TTX_Show_DrawNoneDRCSChar(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
        TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ColCount = 0;
    HI_U32 u32RowCount = 0;
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;
    HI_UNF_TTX_FILLRECT_S stFillRectParam;

    HI_U32 u32X = pstCurPacketInfo->u32X;
    HI_BOOL bDoubleSize = pstSpaceAttr->bDoubleSize;
    HI_U32 u32LineNum = pstCurPacketInfo->u32LineNum;
    HI_U32 u32ColumnNum = pstCurPacketInfo->u32ColumnNum;
    HI_U32 u32MosaicCh = pstCurPacketInfo->u32MosaicCh;
    TTX_DISP_ATTR stCMBAttr = pstCurPacketInfo->stCMBAttr;

    HI_U32 u32CurrentCh = pstCurrentPoint->u32aPageText[u32LineNum][u32ColumnNum];

    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));


    if (TTX_Show_IsSpaceChar(u32CurrentCh))
    {
        u32ColCount = ((stCMBAttr.bDWidth || bDoubleSize) ? 2 : 1);
        u32RowCount = ((stCMBAttr.bDHeight || bDoubleSize) ? 2 : 1);

        /** Holdmosaic  , mosaic mode  ,space attribute */
        if (pstCurPacketInfo->bMosaic && pstSpaceAttr->bHoldMosaic && pstSpaceAttr->bSpacingAttr)
        {
            TTX_Show_SetCharAttr(&stCharAttr, P_CHARACTER(u32MosaicCh), pstSpaceAttr->bContiguous,
                                 pstCurPacketInfo->stFontInfo.enG0Set, pstCurPacketInfo->stFontInfo.enG2Set, HI_UNF_TTX_CHARSET_G1, 0);

            stDrawCharParam.pstCharAttr = &stCharAttr;
            s32Ret = TTX_Show_SetArea(&stPageArea, u32LineNum, u32X, u32RowCount, u32ColCount);
            stDrawCharParam.pstPageArea   = &stPageArea;
            stDrawCharParam.u32Foreground = TTX_GET_COLOR(stCMBAttr.u8ForeColor);

            if (pstCurrentPoint->bTtxMix)
            {
                stDrawCharParam.u32Background = 0x00000000;
            }
            else
            {
                stDrawCharParam.u32Background = TTX_GET_COLOR(stCMBAttr.u8BackColor);
            }

            s32Ret |= TTT_Show_DrawChar(pstCurrentPoint, &stDrawCharParam);
        }
        else
        {
            if (!pstCurrentPoint->bTtxMix)
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, u32LineNum, u32X, u32RowCount, u32ColCount);
                stFillRectParam.pstPageArea = &stPageArea;
                stFillRectParam.u32Color = TTX_GET_COLOR(stCMBAttr.u8BackColor);
                s32Ret |= TTT_Show_FillRect(pstCurrentPoint, &stFillRectParam);
            }
        }
    }
    else           /** None  space char */
    {
        if (stCMBAttr.bConceal && pstCurrentPoint->bConceal)
        {
            if (!pstCurrentPoint->bTtxMix)   /**  Not mix */
            {
                u32ColCount = ((stCMBAttr.bDWidth || bDoubleSize) ? 2 : 1);
                u32RowCount = ((stCMBAttr.bDHeight || bDoubleSize) ? 2 : 1);
                s32Ret |= TTX_Show_SetArea(&stPageArea, u32LineNum, u32X, u32RowCount, u32ColCount);
                stFillRectParam.pstPageArea = &stPageArea;
                stFillRectParam.u32Color = TTX_GET_COLOR(stCMBAttr.u8BackColor);
                s32Ret |= TTT_Show_FillRect(pstCurrentPoint, &stFillRectParam);
            }
        }
        else
        {
            u32ColCount = ((stCMBAttr.bDWidth || bDoubleSize) ? 2 : 1);
            u32RowCount = ((stCMBAttr.bDHeight || bDoubleSize) ? 2 : 1);

            pstCurPacketInfo->stFontInfo.enCharSet = TTX_Show_GetCharSet(u32CurrentCh, pstCurPacketInfo->bMosaic);

            if (pstCurPacketInfo->bModifiedG0G2 && pstCurPacketInfo->bEnhanceChar)
            {
                pstCurPacketInfo->stFontInfo.enG0Set = pstCurPacketInfo->stFontInfo.enModifiedG0Set;
                pstCurPacketInfo->stFontInfo.enG2Set = pstCurPacketInfo->stFontInfo.enModifiedG2Set;
                pstCurPacketInfo->bEnhanceChar = HI_FALSE;
            }
            else
            {
                pstCurPacketInfo->stFontInfo.enG0Set = pstCurPacketInfo->stFontInfo.enLOPG0Set;
                pstCurPacketInfo->stFontInfo.enG2Set = pstCurPacketInfo->stFontInfo.enLOPG2Set;
            }

            TTX_Show_SetCharAttr(&stCharAttr, P_CHARACTER(u32CurrentCh),
                                 pstSpaceAttr->bContiguous,
                                 pstCurPacketInfo->stFontInfo.enG0Set,
                                 pstCurPacketInfo->stFontInfo.enG2Set,
                                 pstCurPacketInfo->stFontInfo.enCharSet,
                                 pstCurPacketInfo->stFontInfo.enNationSet);

            stDrawCharParam.pstCharAttr = &stCharAttr;
            s32Ret |= TTX_Show_SetArea(&stPageArea, u32LineNum, u32X, u32RowCount, u32ColCount);
            stDrawCharParam.pstPageArea   = &stPageArea;
            stDrawCharParam.u32Foreground = TTX_GET_COLOR(stCMBAttr.u8ForeColor);

            if (pstCurrentPoint->bTtxMix)
            {
                stDrawCharParam.u32Background = 0x00000000;
            }
            else
            {
                stDrawCharParam.u32Background = TTX_GET_COLOR(stCMBAttr.u8BackColor);
            }

            s32Ret |= TTT_Show_DrawChar(pstCurrentPoint, &stDrawCharParam);
        }
    }

    if (P_MARKS(u32CurrentCh))                   /** Diacritical mark */
    {
        u32ColCount = ((stCMBAttr.bDWidth || bDoubleSize) ? 2 : 1);
        u32RowCount = ((stCMBAttr.bDHeight || bDoubleSize) ? 2 : 1);

        TTX_Show_SetCharAttr(&stCharAttr, P_MARKS(u32CurrentCh) + 0x40, HI_FALSE,
                             pstCurPacketInfo->stFontInfo.enG0Set, pstCurPacketInfo->stFontInfo.enG2Set,
                             HI_UNF_TTX_CHARSET_G2, 0);
        stDrawCharParam.pstCharAttr = &stCharAttr;
        s32Ret |= TTX_Show_SetArea(&stPageArea, u32LineNum, u32X, u32RowCount, u32ColCount);
        stDrawCharParam.pstPageArea   = &stPageArea;
        stDrawCharParam.u32Foreground = TTX_GET_COLOR(stCMBAttr.u8ForeColor);
        stDrawCharParam.u32Background = TTX_GET_COLOR(stCMBAttr.u8ForeColor);


        s32Ret |= TTT_Show_DrawChar(pstCurrentPoint, &stDrawCharParam);
    }


    return;
}

//PacketsX26 X28 X29
static HI_VOID TTX_Show_ParseEnhanceDataPacket(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
        TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
        TTX_SHOW_SPACEATTR_S* pstSpaceAttr)
{
    HI_U32 u32Cell = 0;
    HI_U32 u32Parser = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bRowOpen = HI_FALSE;
    HI_U32 u32LineNum = pstCurPacketInfo->u32LineNum;
    HI_U32 u32ColumnNum = pstCurPacketInfo->u32ColumnNum;

    TTX_SHOW_PARSER_INFO_S stParserInfo;
    TTX_TRIPLET_PARSER* pstParser = HI_NULL;
    TTX_SHOW_DECHAM24_INFO_S* pstHam24Info = &(stParserInfo.stHam24Info);
    TTX_SHOW_PRIORITY_S* pstShowPriority = &(stParserInfo.stShowPriority);


    memset(&stParserInfo, 0x0, sizeof(TTX_SHOW_PARSER_INFO_S));

    pstShowPriority->u8CharPriority  = OBJECT_PRIORITY(ACTIVE_OBJECT);
    pstShowPriority->u8ForePriority  = OBJECT_PRIORITY(ACTIVE_OBJECT);
    pstShowPriority->u8BackPriority  = OBJECT_PRIORITY(ACTIVE_OBJECT);
    pstShowPriority->u8AttrPriority  = OBJECT_PRIORITY(ACTIVE_OBJECT);
    pstShowPriority->u8FlashPriority = OBJECT_PRIORITY(ACTIVE_OBJECT);


    /* take LOP attributes as default before parsing enhancement data */
    pstCurPacketInfo->stCMBAttr = pstSpaceAttr->stLOPAttr;
    pstCurPacketInfo->u8CMBDRCSChar = 0xff;

    u32Cell = (u32LineNum << 8) + u32ColumnNum;

    for (u32Parser = 0; u32Parser < pstCurPacketInfo->u32OpenCount; u32Parser++)
    {
        pstParser = g_stTripParser + u32Parser;

        stParserInfo.pstParser = pstParser;
        stParserInfo.u32Parser = u32Parser;

        if (pstParser->bOpen)
        {
            bRowOpen = pstParser->bRowOpen;

            while ((pstParser->bOpen && ((((HI_U32)pstParser->u8ActiveRow << 8) + pstParser->u8ActiveCol) <= u32Cell)) ||
                   (pstParser->u8ActiveCol >= 0x48 && pstParser->u8ActiveRow <= u32LineNum))
            {
                if (!TTX_DecodeHam24_18(pstParser->pu8Triplet, &(pstHam24Info->u8Addr), &(pstHam24Info->u8Mode), &(pstHam24Info->u8Data)))
                {
                    pstParser->bOpen = HI_FALSE;
                    break;
                }


                /* parse those triplets that will change active position */
                TTX_Show_ParseActiveTriplets(pstParser, pstHam24Info);


                /* if the element is at the current position... */
                if ((pstParser->u8ActiveRow == u32LineNum) && (pstParser->u8ActiveCol == u32ColumnNum))
                {
                    s32Ret = TTX_Show_ProcCurPosition(pstCurrentPoint, pstCurPacketInfo, &stParserInfo, pstSpaceAttr, &bRowOpen);

                    if (HI_SUCCESS != s32Ret)
                    {
                        break;
                    }
                }
                else /* if active position changed... */
                {
                    /* if it's the last element on this row... */
                    if (pstParser->u8ActiveRow != u32LineNum)
                    {
                        pstParser->bRowOpen = HI_FALSE;
                    }
                }


                s32Ret = TTX_Show_ProcActiveTriplets26(pstCurrentPoint, pstCurPacketInfo, stParserInfo.pstParser);

                if (HI_SUCCESS != s32Ret)
                {
                    break;
                }


                /* if we are before the current position... */
                s32Ret = TTX_Show_ProcPrePosition(pstCurrentPoint, pstCurPacketInfo, stParserInfo.pstParser, u32Cell);

                if (HI_SUCCESS != s32Ret)
                {
                    break;
                }

            }

            /* overwrite LOP attributes with adaptive object attributes */
            TTX_Show_SetLOPAttributes(pstCurPacketInfo, &stParserInfo, bRowOpen);
        }

    }

    return;
}


static HI_VOID TTX_Show_ParseCurPacket(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                       TTX_SHOW_CURPACKET_INFO_S* pstCurPacketInfo,
                                       TTX_SHOW_SPACEATTR_S* pstSpaceAttr)

{
    HI_U32 u32X = 0;
    HI_U32 u32TempCh = 0;
    HI_U32 u32CurrentCh = 0;
    HI_U32 u32ColumnNum = 0;
    HI_S32 s32Ret = HI_SUCCESS;


    HI_U32 u32Band = pstCurPacketInfo->u32Band;
    HI_U32 u32LineNum = pstCurPacketInfo->u32LineNum;
    TTX_DISP_ATTR* pstLOPAttr = &(pstSpaceAttr->stLOPAttr);
    HI_BOOL bUseDefaultCharSet = pstCurPacketInfo->bUseDefaultCharSet;

    TTX_SHOW_FONT_INFO_S* pstFontInfo = &(pstCurPacketInfo->stFontInfo);



    pstFontInfo->enNationSet = HI_UNF_TTX_NATION_SET_PRIMARY;
    pstFontInfo->enG0Set = HI_UNF_TTX_G0SET_LATIN;   /** G0  Char set */
    pstFontInfo->enLOPG0Set = HI_UNF_TTX_G0SET_LATIN;   /** LOP G0  Char set */
    pstFontInfo->enModifiedG0Set = HI_UNF_TTX_G0SET_LATIN;   /** modified G0  Char set */
    pstFontInfo->enG2Set = HI_UNF_TTX_G2SET_LATIN;   /** G2  Char set */
    pstFontInfo->enLOPG2Set = HI_UNF_TTX_G2SET_LATIN;   /** LOP G2  Char set */
    pstFontInfo->enModifiedG2Set = HI_UNF_TTX_G2SET_LATIN;   /** modified G2  Char set */
    pstFontInfo->enCharSet = HI_UNF_TTX_CHARSET_G0;


    TTX_Show_GetLOPG0G2AndNationSet(pstCurrentPoint, bUseDefaultCharSet, \
                                    & (pstFontInfo->enLOPG0Set), &(pstFontInfo->enLOPG2Set), &(pstFontInfo->enNationSet));

    u32ColumnNum = pstCurrentPoint->u8BandStart[u32Band];
    u32X = pstCurrentPoint->u8BandLeft[u32Band];

    for ( ; u32ColumnNum < pstCurrentPoint->u8BandEnd[u32Band]; u32ColumnNum++, u32X++)
    {
        pstCurPacketInfo->u32X = u32X;
        pstCurPacketInfo->u32ColumnNum = u32ColumnNum;

        if (pstSpaceAttr->bSetAForeground)       /** Set alpha foreground  color */
        {
            pstCurPacketInfo->bMosaic = HI_FALSE;
            pstLOPAttr->u8ForeColor = pstSpaceAttr->u32TmpForeground;
            pstSpaceAttr->bSetAForeground = HI_FALSE;
        }

        if (pstSpaceAttr->bSetMForeground)   /** Set mosaic foreground  color */
        {
            pstCurPacketInfo->bMosaic = HI_TRUE;
            pstLOPAttr->u8ForeColor = pstSpaceAttr->u32TmpForeground;
            pstSpaceAttr->bSetMForeground = HI_FALSE;
        }

        if (pstSpaceAttr->bSetFlash)     /** Start Flash */
        {
            pstSpaceAttr->u8LOPFlash = 1;
            pstSpaceAttr->bSetFlash = HI_FALSE;
        }

        if (pstSpaceAttr->bStartBox)
        {
            pstLOPAttr->bBox = HI_TRUE;
        }
        else
        {
            pstLOPAttr->bBox = HI_FALSE;
        }

        if (pstCurPacketInfo->bReleaseMos)
        {
            pstSpaceAttr->bHoldMosaic = HI_FALSE;
            pstCurPacketInfo->bReleaseMos = HI_FALSE;
        }

        if (pstSpaceAttr->bEsc)
        {
            pstSpaceAttr->bEsc = HI_FALSE;

            pstCurPacketInfo->bUseDefaultCharSet = (pstCurPacketInfo->bUseDefaultCharSet == HI_TRUE) ? HI_FALSE : HI_TRUE;

            bUseDefaultCharSet = pstCurPacketInfo->bUseDefaultCharSet;

            TTX_Show_GetLOPG0G2AndNationSet(pstCurrentPoint, bUseDefaultCharSet, \
                                            & (pstFontInfo->enLOPG0Set), &(pstFontInfo->enLOPG2Set), &(pstFontInfo->enNationSet));
        }

        if (!u32Band) /*normal area*/
        {
            u32CurrentCh = pstCurrentPoint->u32aPageText[u32LineNum][u32ColumnNum];

            if (u32CurrentCh != 0xff)/**  not invalid  char */
            {
                u32TempCh = P_CHARACTER(u32CurrentCh);

                /* process space attributes */
                if (u32TempCh < 0x20)
                {
                    TTX_Show_ProcSpaceAttributes(u32TempCh, pstCurrentPoint, pstCurPacketInfo, pstSpaceAttr);

                    pstSpaceAttr->bSpacingAttr = HI_TRUE;
                }
                else
                {
                    pstSpaceAttr->bSpacingAttr = HI_FALSE;
                }
            }

        }

        if (pstCurPacketInfo->bBoxPage && !pstLOPAttr->bBox) //Box flag
        {
            pstCurPacketInfo->au32BackgroundRec[u32ColumnNum] = 0;/*transparent*/
            continue;
        }

        /* PROCESS ENHANCEMENT DATA */
        TTX_Show_ParseEnhanceDataPacket(pstCurrentPoint, pstCurPacketInfo, pstSpaceAttr);


        /*the display data may be rewrited by object or the data of side panels may be defined*/
        s32Ret = TTX_Show_ProcSidePanel(pstCurrentPoint, pstCurPacketInfo, pstSpaceAttr);

        if (HI_SUCCESS != s32Ret)
        {
            continue;
        }

        /*DRCS char*/
        if ((0xff != pstCurPacketInfo->u8CMBDRCSChar) && (0xff != pstCurPacketInfo->u8CMBDRCSSubPage))
        {
            TTX_Show_DrawDRCSChar(pstCurrentPoint, pstCurPacketInfo, pstSpaceAttr);
        }
        else
        {
            TTX_Show_DrawNoneDRCSChar(pstCurrentPoint, pstCurPacketInfo, pstSpaceAttr);
        }

    }


    return;
}



static HI_VOID TTX_Show_ParseShowPacket(TTX_PAGE_CONTEXT_S* pstCurrentPoint,
                                        HI_U32 u32OpenCount,
                                        HI_U32 u32DesignCode,
                                        HI_BOOL bBoxPage,
                                        HI_U8* au8DataLine)
{
    HI_U32 u32StartLine = 0;
    HI_U32 u32EndLine = TTX_ROW_NUM;

    TTX_DISP_ATTR stLOPAttr;
    TTX_SHOW_SPACEATTR_S stSpaceAttr;
    TTX_SHOW_CURPACKET_INFO_S stCurPacketInfo;

    if (pstCurrentPoint->u32ZoomFlag == 1)
    {
        u32EndLine = TTX_ROW_NUM >> 1;
    }
    else if (pstCurrentPoint->u32ZoomFlag == 2)
    {
        u32StartLine = TTX_ROW_NUM >> 1;
    }

    memset(&stLOPAttr, 0x0, sizeof(TTX_DISP_ATTR));
    memset(&stSpaceAttr, 0x0, sizeof(TTX_SHOW_SPACEATTR_S));
    memset(&stCurPacketInfo, 0x0, sizeof(TTX_SHOW_CURPACKET_INFO_S));

    stCurPacketInfo.u32DesignCode = u32DesignCode;
    stCurPacketInfo.u32OpenCount = u32OpenCount;
    stCurPacketInfo.au8DataLine = au8DataLine;
    stCurPacketInfo.bBoxPage = bBoxPage;
    stCurPacketInfo.u32Fillback = 0;
    stCurPacketInfo.u32ColumnNum = 0;
    stCurPacketInfo.bReleaseMos = HI_FALSE;
    stCurPacketInfo.bMosaic = HI_FALSE;
    stCurPacketInfo.bLarged   = HI_FALSE;
    stCurPacketInfo.bUseDefaultCharSet = HI_TRUE;
    stCurPacketInfo.bModifiedG0G2 = HI_FALSE;
    stCurPacketInfo.bEnhanceChar = HI_FALSE;/*char defined by X26 or object*/
    stCurPacketInfo.bFlag = HI_FALSE;
    stCurPacketInfo.u32MosaicCh = 0;

    stCurPacketInfo.u32Band = 0;
    stCurPacketInfo.u8CMBDRCSChar = 0;        /* current DRCS char */
    stCurPacketInfo.u8CMBDRCSSubPage = 0;     /* current DRCS sub-page number */
    stCurPacketInfo.u8FullRowColor = 0;
    stCurPacketInfo.u8DefRowColor = pstCurrentPoint->stP28_0_Info.u8DefRowColor;
    stCurPacketInfo.bBlackBgSubstitute = pstCurrentPoint->bBlackBgSubstitute;
    stCurPacketInfo.u32LineNum = u32StartLine;



    memset(&stSpaceAttr, 0x0, sizeof(TTX_SHOW_SPACEATTR_S));
    stSpaceAttr.u32TmpForeground = 0xffffffff;

    stSpaceAttr.bSetAForeground = HI_FALSE;
    stSpaceAttr.bSetFlash = HI_FALSE;
    stSpaceAttr.bSpacingAttr = HI_FALSE;
    stSpaceAttr.bSetMForeground = HI_FALSE;

    stSpaceAttr.bDoubleSize = HI_FALSE;
    stSpaceAttr.bHoldMosaic = HI_FALSE;

    stSpaceAttr.bContiguous = HI_TRUE;
    stSpaceAttr.bStartBox = HI_FALSE;
    stSpaceAttr.bEsc = HI_FALSE;
    stSpaceAttr.u8LOPFlash = 0;




    for ( ; stCurPacketInfo.u32LineNum < u32EndLine; stCurPacketInfo.u32LineNum++)
    {
        if (stCurPacketInfo.bLarged)   /** Double height or double size ,ingore  the next line */
        {
            stCurPacketInfo.bLarged = HI_FALSE;
            stSpaceAttr.bDoubleSize = HI_FALSE;
            stLOPAttr.bDHeight = HI_FALSE;
            stLOPAttr.bDWidth = HI_FALSE;
            continue;
        }

        stCurPacketInfo.bLarged = HI_FALSE;

        if (stCurPacketInfo.bBoxPage)
        {
            stCurPacketInfo.bFlag = HI_TRUE;/*if this line has not valid char ,skip it */
            stCurPacketInfo.u32Band = 0;
            stCurPacketInfo.u32ColumnNum = pstCurrentPoint->u8BandStart[stCurPacketInfo.u32Band];

            for (; stCurPacketInfo.u32ColumnNum < pstCurrentPoint->u8BandEnd[stCurPacketInfo.u32Band]; stCurPacketInfo.u32ColumnNum++)
            {
                if (pstCurrentPoint->u32aPageText[stCurPacketInfo.u32LineNum][stCurPacketInfo.u32ColumnNum] > 0x20)
                {
                    stCurPacketInfo.bFlag = HI_FALSE;
                    break;
                }
            }

            if (HI_TRUE == stCurPacketInfo.bFlag)
            {
                continue;
            }
        }


        stCurPacketInfo.u8FullRowColor = stCurPacketInfo.u8DefRowColor;

        for (stCurPacketInfo.u32Band = 0; stCurPacketInfo.u32Band < 3; stCurPacketInfo.u32Band++)/*0:Normal area 1:Right sidepanel 2:Left sidepanel*/
        {

            stCurPacketInfo.bMosaic = HI_FALSE;
            stCurPacketInfo.bReleaseMos = HI_FALSE;
            stCurPacketInfo.bModifiedG0G2 = HI_FALSE;
            stCurPacketInfo.bUseDefaultCharSet = HI_TRUE;
            stCurPacketInfo.bEnhanceChar = HI_FALSE;
            stCurPacketInfo.u32MosaicCh = 0x20;
            stCurPacketInfo.u32Fillback = 0;




            //space attr
            stSpaceAttr.bSetFlash = HI_FALSE;
            stSpaceAttr.bSetAForeground = HI_FALSE;
            stSpaceAttr.bSetMForeground = HI_FALSE;
            stSpaceAttr.u32TmpForeground = pstCurrentPoint->stP28_0_Info.u8ForeCLUT + TTX_COLOR_WHITE;;

            stSpaceAttr.bStartBox = HI_FALSE;
            stSpaceAttr.bDoubleSize = HI_FALSE;
            stSpaceAttr.bHoldMosaic = HI_FALSE;
            stSpaceAttr.bContiguous = HI_TRUE;
            stSpaceAttr.bEsc = HI_FALSE;
            stSpaceAttr.u8LOPFlash = 0;


            /* reset LOP attributes at the row beginning */
            stLOPAttr.bDHeight = HI_FALSE;
            stLOPAttr.bBox = HI_FALSE;
            stLOPAttr.bConceal = HI_FALSE;
            stLOPAttr.bUnderline = HI_FALSE;
            stLOPAttr.bInvert = HI_FALSE;
            stLOPAttr.bSeparated = HI_FALSE;
            stLOPAttr.bDWidth = HI_FALSE;
            stLOPAttr.bChanged = HI_FALSE;


            if (stCurPacketInfo.bBlackBgSubstitute)
            {
                stLOPAttr.u8BackColor = stCurPacketInfo.u8FullRowColor;
            }
            else
            {
                stLOPAttr.u8BackColor = pstCurrentPoint->stP28_0_Info.u8BackCLUT + TTX_COLOR_BLACK;
            }

            stLOPAttr.u8ForeColor = pstCurrentPoint->stP28_0_Info.u8ForeCLUT + TTX_COLOR_WHITE;

            stSpaceAttr.stLOPAttr = stLOPAttr;

            TTX_Show_ParseCurPacket(pstCurrentPoint, &stCurPacketInfo, &stSpaceAttr);
        }

    }

    return;
}


static HI_VOID TTX_Show_ShowNoneBoxPage(TTX_PAGE_CONTEXT_S* pstCurrentPoint)
{
    HI_U8  j = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;
    HI_U32 u32aPageNum[6] = {0, 0, 0, 0, 0, 0};
    HI_U32 u32MaxColNum = pstCurrentPoint->u32ColumnNum;


    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));


    if (!TTX_InputNumIsOnGoing(&pstCurrentPoint->stInputNum))    /**  0x2d, the ASCII of ' -'.  Show page and subpage number */
    {
        if (pstCurrentPoint->stCurPgAddr.u8MagazineNum == 0)
        {
            u32aPageNum[0] = 8 + 0x30;
        }
        else
        {
            u32aPageNum[0] = pstCurrentPoint->stCurPgAddr.u8MagazineNum + 0x30;
        }

        u32aPageNum[1] = pstCurrentPoint->stCurPgAddr.u8PageNum / 16 + 0x30;
        u32aPageNum[2] = pstCurrentPoint->stCurPgAddr.u8PageNum % 16 + 0x30;
        u32aPageNum[3] = 0x2e;
        u32aPageNum[4] = pstCurrentPoint->stCurPgAddr.u16PageSubCode / 10 + 0x30;
        u32aPageNum[5] = pstCurrentPoint->stCurPgAddr.u16PageSubCode % 10 + 0x30;

        /*display the magazine num ,page num and subpage in the start of row 0*/
        for (j = 0; j < 6; j++)
        {
            TTX_Show_SetCharAttr(&stCharAttr, u32aPageNum[j], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                                 HI_UNF_TTX_CHARSET_G0, 0);
            stDrawCharParam.pstCharAttr = &stCharAttr;

            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, j + 1, 1, 1);
            stDrawCharParam.pstPageArea   = &stPageArea;
            stDrawCharParam.u32Foreground = 0xff00ff00;

            if (pstCurrentPoint->bTtxMix)
            {
                stDrawCharParam.u32Background = 0x00000000;
            }
            else
            {
                stDrawCharParam.u32Background = 0xff000000;
            }


            if (2 != pstCurrentPoint->u32ZoomFlag)
            {
                s32Ret |= TTT_Show_DrawChar(pstCurrentPoint, &stDrawCharParam);
            }

        }
    }

    if (pstCurrentPoint->bNavigation)
    {
        if (pstCurrentPoint->bSubpageMode)
        {
            TTX_Show_DrawNavigationSubpageMode(pstCurrentPoint, u32MaxColNum);
        }
        else
        {
            TTX_Show_DrawNavigation(pstCurrentPoint, u32MaxColNum);
        }
    }

    return;
}



static HI_S32 TTX_Show_RefreshShowArea(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_BOOL bBoxPage, HI_U8 u8NumOfSubPg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;
    HI_U32 u32MaxColNum = pstCurrentPoint->u32ColumnNum;

    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));


    if (bBoxPage)
    {
        if (pstCurrentPoint->bNavigation)
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR,
                                       u32MaxColNum);
        }
        else
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, u32MaxColNum);
        }

        stRefrshLayerParam.pstPageArea = &stPageArea;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
    }
    else
    {
        if (0 == pstCurrentPoint->u32ZoomFlag)
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, 1, 32);
            stRefrshLayerParam.pstPageArea = &stPageArea;
            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
        }
        else
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, 1, TTX_COL_NUM);
            stRefrshLayerParam.pstPageArea = &stPageArea;
            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
        }

        if (pstCurrentPoint->bNavigation)
        {
            if (u8NumOfSubPg > 0)/** Have subpage,don't refresh and just refresh by updatetask  */
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, 1, 0, TTX_ROW_NUM, u32MaxColNum);
            }
            else
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, 1, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, u32MaxColNum);
            }
        }
        else
        {
            if (u8NumOfSubPg > 0)
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, 1, 0, TTX_ROW_NUM - 1, u32MaxColNum);
            }
            else
            {
                s32Ret |= TTX_Show_SetArea(&stPageArea, 1, 0, TTX_ROW_NUM, u32MaxColNum);
            }
        }

        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
    }

    return s32Ret;
}


static HI_VOID TTX_Show_DecShowPage(TTX_PAGE_CONTEXT_S*  pstContextHead)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bBoxPage  = HI_FALSE;
    HI_U8 u8NumOfSubPg = TTX_MAX_SUBPAGENO;
    HI_U32 u32OpenCount = 0;
    HI_U32 u32DesignCode = 0;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;


    /**  Invalid  page context ,  just  return  */
    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("In TTX_ShowPage, Invalid  page context\n");
        return;
    }

    pstCurrentPoint->bInvalidReq = HI_FALSE;



    /**parse MOT page, at level 1.0 and 1.5,this function will return failure ,so we not judge the return value*/
    TTX_Show_Parse_MOTPage(pstCurrentPoint);

    /**Get Reference color map*/
    TTX_Show_GetColorMap(pstCurrentPoint);

    /**get p28_0 info*/
    TTX_Show_GetP28_0_Info(pstCurrentPoint);

    /**get band info*/
    TTX_Show_GetBandInfo(pstCurrentPoint);

    s32Ret = TTX_Show_CalculatePageArea(pstCurrentPoint);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TTX("TTX_Show_CalculatePageArea, Errors occur while call functions\n");
        return;
    }


    if (HI_NULL != pstCurrentPoint->pFlaAreaHead)   /** Before  show a page ,release the flash area of prve page */
    {
        HI_INFO_TTX("Release   ");
        TTX_Show_Release_FlashArea(pstCurrentPoint->pFlaAreaHead);
        pstCurrentPoint->pFlaAreaHead = HI_NULL;
    }

    /**control bits proc*/
    s32Ret = TTX_Show_ProcControlBits(pstCurrentPoint, &bBoxPage, &u8NumOfSubPg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TTX("TTX_Show_ProcControlBits, Errors occur while call functions\n");
        return;
    }

    HI_U8 au8DataLine[TTX_LINE_MEM_SIZE];
    memset(au8DataLine, 0 , TTX_LINE_MEM_SIZE);

    /** find packet 26*/
    TTX_Show_ParsePacket26(pstCurrentPoint, &u32OpenCount, &u32DesignCode, au8DataLine);

    /**parse all packet to show*/
    TTX_Show_ParseShowPacket(pstCurrentPoint, u32OpenCount, u32DesignCode, bBoxPage, au8DataLine);

    if (!bBoxPage)   /**  Not subtitle */
    {
        TTX_Show_ShowNoneBoxPage(pstCurrentPoint);
    }

    /**refresh show area*/
    s32Ret = TTX_Show_RefreshShowArea(pstCurrentPoint, bBoxPage, u8NumOfSubPg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_Show_DecShowPage, Errors occur while call functions\n");
    }


    return;
}

HI_VOID TTX_Show_DrawNavigation(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U32 u32MaxColNum)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 u32aPageNum[6] = {0, 0, 0, 0, 0, 0};   /** Page num and subpage nums  */

    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;
    HI_UNF_TTX_FILLRECT_S stFillRectParam;

    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));

    /** Add  four navigation key at the bottom of teletext pages */
    if (pstCurrentPoint->bNavigation)
    {
        if (!pstCurrentPoint->bTtxMix)
        {
            s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, 1, 1, u32MaxColNum / 4);
            stFillRectParam.pstPageArea = &stPageArea;
            stFillRectParam.u32Color = s_ColorSet[1];    /** Red key */
            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
            stPageArea.u32Column = 1 + 1 * u32MaxColNum / 4;
            stFillRectParam.u32Color = s_ColorSet[2];   /** Green  key */
            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
            stPageArea.u32Column = 1 + 2 * u32MaxColNum / 4;
            stFillRectParam.u32Color = s_ColorSet[3];   /** Yellow key */
            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
            stPageArea.u32Column = 1 + 3 * u32MaxColNum / 4;
            stPageArea.u32ColumnCount = (u32MaxColNum / 4 - 1);
            stFillRectParam.u32Color = s_ColorSet[6];   /** Cyan key */
            s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
        }

        for (i = 0; i < 4; i++)      /** Add  the page address of navigation key */
        {
            if (pstCurrentPoint->astLink27[i].u8MagazineNum == 0)
            {
                u32aPageNum[0] = 8 + 0x30;
            }
            else
            {
                u32aPageNum[0] = pstCurrentPoint->astLink27[i].u8MagazineNum + 0x30;
            }

            u32aPageNum[1] = pstCurrentPoint->astLink27[i].u8PageNum / 16 + 0x30;
            u32aPageNum[2] = pstCurrentPoint->astLink27[i].u8PageNum % 16 + 0x30;

            for (j = 0; j < 3; j++)
            {
                TTX_Show_SetCharAttr(&stCharAttr, u32aPageNum[j], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                                     HI_UNF_TTX_CHARSET_G0, 0);
                stDrawCharParam.pstCharAttr = &stCharAttr;
                s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, 4 + i * u32MaxColNum / 4 + j, 1, 1);
                stDrawCharParam.pstPageArea   = &stPageArea;
                stDrawCharParam.u32Foreground = s_ColorSet[4];
                stDrawCharParam.u32Background = s_ColorSet[7];
                s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR,
                                            (HI_VOID*)&stDrawCharParam);
            }
        }
    }


    return;
}

static HI_VOID TTX_Show_PageNumToAscii(HI_UNF_TTX_PAGE_ADDR_S* pstPageNum, HI_U8* pu8Ascii)
{
    if (pstPageNum->u8MagazineNum == 0)
    {
        pu8Ascii[0] = 8 + '0';
    }
    else
    {
        pu8Ascii[0] = pstPageNum->u8MagazineNum + '0';
    }

    pu8Ascii[1] = pstPageNum->u8PageNum / 16 + '0';
    pu8Ascii[2] = pstPageNum->u8PageNum % 16 + '0';

    return;
}

static HI_VOID TTX_Show_PageSubcodeToAscii(HI_UNF_TTX_PAGE_ADDR_S* pstPageNum, HI_U8* pu8Ascii)
{
    pu8Ascii[0] = '0';
    pu8Ascii[1] = '0';
    pu8Ascii[2] = TTX_INT_TO_CHAR(pstPageNum->u16PageSubCode / 10);
    pu8Ascii[3] = TTX_INT_TO_CHAR(pstPageNum->u16PageSubCode % 10);

    return;
}

HI_VOID TTX_Show_DrawNavigationSubpageMode(TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_U32 u32MaxColNum)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i = 0;
    HI_U8 au8PageNum[TTX_DIGIT_NUM_OF_PAGE_NUM];
    HI_U8 au8Subcode[TTX_DIGIT_NUM_OF_SUBCODE];
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_U32 u32ColPos = u32MaxColNum >> 3;   /* position of '-' */
    TTX_INPUT_NUM_S* pstInputNum = HI_NULL;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;
    HI_UNF_TTX_FILLRECT_S stFillRectParam;
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;

    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));
    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));


    if (!pstCurrentPoint->bTtxMix)
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, 1, 1, u32MaxColNum / 4);
        stFillRectParam.pstPageArea = &stPageArea;
        stFillRectParam.u32Color = RED_COLOR;    /** Red key */
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
        stPageArea.u32Column = 1 + 1 * u32MaxColNum / 4;
        stFillRectParam.u32Color = GREEN_COLOR;   /** Green  key */
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
        stPageArea.u32Column = 1 + 2 * u32MaxColNum / 4;
        stFillRectParam.u32Color = YELLOW_COLOR;   /** Yellow key */
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
        stPageArea.u32Column = 1 + 3 * u32MaxColNum / 4;
        stPageArea.u32ColumnCount = (u32MaxColNum / 4 - 1);
        stFillRectParam.u32Color = YELLOW_COLOR;   /** Cyan key */
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);
    }


    /* display as: <    -      +      165/0001    > , (if page = 165, subpage = 0001)*/
    TTX_Show_SetCharAttr(&stCharAttr, '-', HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                         HI_UNF_TTX_CHARSET_G0, 0);
    stDrawCharParam.pstCharAttr = &stCharAttr;
    s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, u32ColPos, 1, 1);  /*  col position: 1/8 */
    stDrawCharParam.pstPageArea   = &stPageArea;
    stDrawCharParam.u32Foreground = BLACK_COLOR;
    stDrawCharParam.u32Background = RED_COLOR;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID*)&stDrawCharParam);

    u32ColPos += u32MaxColNum >> 2;
    TTX_Show_SetCharAttr(&stCharAttr, '+', HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                         HI_UNF_TTX_CHARSET_G0, 0);
    s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, u32ColPos, 1, 1);
    stDrawCharParam.u32Background = GREEN_COLOR;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID*)&stDrawCharParam);

    /* draw page number */
    memset(au8PageNum, 0, sizeof(au8PageNum));
    TTX_Show_PageNumToAscii(&pstCurrentPoint->stCurPgAddr, au8PageNum);
    u32ColPos += u32MaxColNum >> 2;

    for (i = 0; i < TTX_DIGIT_NUM_OF_PAGE_NUM; i++)
    {
        TTX_Show_SetCharAttr(&stCharAttr, au8PageNum[i], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                             HI_UNF_TTX_CHARSET_G0, 0);
        s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, u32ColPos++, 1, 1);
        stDrawCharParam.u32Background = YELLOW_COLOR;
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID*)&stDrawCharParam);
    }

    TTX_Show_SetCharAttr(&stCharAttr, '/', HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                         HI_UNF_TTX_CHARSET_G0, 0);
    s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, u32ColPos++, 1, 1);
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID*)&stDrawCharParam);

    /* inputting subcode now */
    pstInputNum = &pstCurrentPoint->stInputNum;
    memset(au8Subcode, 0, sizeof(au8Subcode));

    if (TTX_InputNumIsOnGoing(pstInputNum))
    {
        memcpy(au8Subcode, pstInputNum->au8Buf, sizeof(au8Subcode));

        if (TTX_IsLastFigureInputted(pstInputNum, TTX_DIGIT_NUM_OF_SUBCODE))  /** Input three number */
        {
            TTX_ResetInputNum(pstInputNum);
        }
    }
    else
    {
        TTX_Show_PageSubcodeToAscii(&pstCurrentPoint->stCurPgAddr, au8Subcode);
    }

    for (i = 0; i < TTX_DIGIT_NUM_OF_SUBCODE; i++)
    {
        TTX_Show_SetCharAttr(&stCharAttr, au8Subcode[i], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN,
                             HI_UNF_TTX_CHARSET_G0, 0);
        s32Ret |= TTX_Show_SetArea(&stPageArea, TTX_ROW_NUM, u32ColPos++, 1, 1);
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID*)&stDrawCharParam);
    }

    stRefrshLayerParam.pstPageArea = &stPageArea;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);
}

HI_VOID TTX_Show_ShowPage(TTX_PAGE_CONTEXT_S*    pstContextHead)
{
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    if (HI_NULL == pstCurrentPoint)    /** Invalid param, just return */
    {
        return;
    }

    TTX_SHOWPAGE_LOCK();

    if (HI_TRUE == pstCurrentPoint->bTTXSearching)
    {
        pstCurrentPoint->bTTXSearching = HI_FALSE;
    }

    if (!pstCurrentPoint->bTtxHiden)
    {
        TTX_Show_DecShowPage(pstCurrentPoint);
    }

    pstCurrentPoint->bProcessed = HI_TRUE;
    TTX_SHOWPAGE_UNLOCK();
}

HI_S32  TTX_Show_SetArea(HI_UNF_TTX_PAGEAREA_S* pstPageArea, HI_U32 u32Row, HI_U32 u32Colunm, HI_U32 u32RowCount,
                         HI_U32 u32ColCount)
{
    if (HI_NULL == pstPageArea)
    {
        return HI_FAILURE;
    }

    pstPageArea->u32Row = u32Row;
    pstPageArea->u32Column   = u32Colunm;
    pstPageArea->u32RowCount = u32RowCount;
    pstPageArea->u32ColumnCount = u32ColCount;

    return HI_SUCCESS;
}

HI_VOID   TTX_Show_SetCharAttr(HI_UNF_TTX_CHARATTR_S* pstCharAttr, HI_U32 u32Index,
                               HI_BOOL bContiguous, HI_UNF_TTX_G0SET_E enG0Set, HI_UNF_TTX_G2SET_E enG2Set,
                               HI_UNF_TTX_CHARSET_E enCharSet, HI_U8 u8NationSet)

{
    if (HI_NULL == pstCharAttr)
    {
        return;
    }

    pstCharAttr->u32Index = u32Index;
    pstCharAttr->bContiguous = bContiguous;
    pstCharAttr->enG0Set     = enG0Set;
    pstCharAttr->enG2Set     = enG2Set;
    pstCharAttr->enCharSet   = enCharSet;
    pstCharAttr->u8NationSet = u8NationSet;

    return;
}

HI_UNF_TTX_CHARSET_E  TTX_Show_GetCharSet(HI_U32 u32Index, HI_BOOL bMosMod)
{
    HI_UNF_TTX_CHARSET_E enCharSet = HI_UNF_TTX_CHARSET_G0;
    HI_U32 u32Char = u32Index;

    if (u32Index < 0x20)
    {
        return enCharSet;
    }

    if (P_ADDITION(u32Char))   /** The char specify  by packet 26 */
    {
        switch (P_GCODE(u32Char))
        {
            case 0:                              /** G0 char  */
                enCharSet = HI_UNF_TTX_CHARSET_G0;
                break;

            case 1:                         /** G1 char  */
                u32Char = P_CHARACTER(u32Char) - 32;

                if ((u32Char < 32) || (u32Char >= 64))
                {
                    enCharSet = HI_UNF_TTX_CHARSET_G1;
                }
                else
                {
                    enCharSet = HI_UNF_TTX_CHARSET_G0;
                }

                break;

            case 2:                      /** G2 char */
                enCharSet = HI_UNF_TTX_CHARSET_G2;
                break;

            case 3:                    /** G3 char */
                enCharSet = HI_UNF_TTX_CHARSET_G3;
                break;

            default:
                enCharSet = HI_UNF_TTX_CHARSET_G0;
                break;
        }
    }
    else
    {
        u32Char = P_CHARACTER(u32Char) - 32;   /** Visible char  begin with 0x20*/

        if (bMosMod)     /** Mosaic char  */
        {
            if ((u32Char < 32) || (u32Char >= 64))
            {
                enCharSet = HI_UNF_TTX_CHARSET_G1;
            }
            else
            {
                enCharSet = HI_UNF_TTX_CHARSET_G0;
            }
        }
        else
        {
            enCharSet = HI_UNF_TTX_CHARSET_G0;
        }
    }

    return enCharSet;
}


HI_VOID TTX_Show_GetLOPG0G2AndNationSet(TTX_PAGE_CONTEXT_S*  pstContextHead, HI_BOOL bUseDefaultCharSet, HI_UNF_TTX_G0SET_E* penG0Set, HI_UNF_TTX_G2SET_E* penG2Set, HI_UNF_TTX_NATION_SET_E* penNationSet)
{
    HI_U8 i = 0;
    HI_U8 u8MagNum = 0;
    HI_U8 u8DefG0G2 = 0;
    HI_U8 u8SecG0 = 0;
    HI_U8 u8FontMapIndex = 0;
    HI_U32 u32MaxLanNum = 0;
    HI_UNF_TTX_G0SET_E enDefG0Set = HI_UNF_TTX_G0SET_BUTT;
    HI_UNF_TTX_G0SET_E enSecG0Set = HI_UNF_TTX_G0SET_BUTT;
    HI_UNF_TTX_G2SET_E enG2Set = HI_UNF_TTX_G2SET_BUTT;
    HI_UNF_TTX_NATION_SET_E enDefNationSet = HI_UNF_TTX_NATION_SET_BUTT;
    HI_UNF_TTX_NATION_SET_E enSecNationSet = HI_UNF_TTX_NATION_SET_BUTT;
    TTX_DEF_G0G2_S* pstDefG0G2 = HI_NULL;
    TTX_SEC_G0_S* pstSecG0 = HI_NULL;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = pstContextHead;
    u8MagNum = pstCurrentPoint->stCurPgAddr.u8MagazineNum & 0x7;


    /*if has defined the G0,G2,nation set by enhancement packet*/
    if ((HI_TRUE == pstCurrentPoint->bHasP28_0) || (HI_TRUE == pstCurrentPoint->bHasP28_4) || (HI_TRUE == g_bHasM29_0[u8MagNum]) || (HI_TRUE == g_bHasM29_4[u8MagNum]))
    {
        u8DefG0G2 = pstCurrentPoint->stP28_0_Info.u8DefG0G2;
        u8SecG0 = pstCurrentPoint->stP28_0_Info.u82ndG0;


        u8FontMapIndex = (u8DefG0G2 >> 3) & 0xf;

        if (u8FontMapIndex < TTX_SHOW_FONTMAP_SIZE)
        {
            pstDefG0G2 = (TTX_DEF_G0G2_S*)g_astDefG0G2Map[u8FontMapIndex];
        }

        if (HI_NULL != pstDefG0G2)
        {
            i = u8DefG0G2 & 0x7;
            pstDefG0G2 += i;
            enDefG0Set = pstDefG0G2->enG0Set;
            enG2Set = pstDefG0G2->enG2Set;
            enDefNationSet = pstDefG0G2->enNationSet;
        }



        u8FontMapIndex = (u8SecG0 >> 3) & 0xf;

        if (u8FontMapIndex < TTX_SHOW_FONTMAP_SIZE)
        {
            pstSecG0 = (TTX_SEC_G0_S*)g_astSecG0Map[u8FontMapIndex];
        }

        if (HI_NULL != pstSecG0)
        {
            i = u8SecG0 & 0x7;
            pstSecG0 += i;
            enSecG0Set = pstSecG0->enG0Set;
            enSecNationSet = pstSecG0->enNationSet;
        }

        if (HI_TRUE == bUseDefaultCharSet)
        {
            *penG0Set = enDefG0Set;
            *penNationSet = enDefNationSet;
        }
        else
        {
            *penG0Set = enSecG0Set;
            *penNationSet = enSecNationSet;
        }

        *penG2Set = enG2Set;
    }
    else
    {
        u32MaxLanNum = sizeof(g_stLanguageCode) / sizeof(TTX_LANGUAGE_S);

        for (i = 0; i < u32MaxLanNum; i++)
        {
            if ((pstCurrentPoint->u8Language[0] == g_stLanguageCode[i].u8LanguageCode[0])\
                && (pstCurrentPoint->u8Language[1] == g_stLanguageCode[i].u8LanguageCode[1])\
                && (pstCurrentPoint->u8Language[2] == g_stLanguageCode[i].u8LanguageCode[2]))
            {
                *penNationSet = g_stLanguageCode[i].enNationSet;
                *penG0Set = g_stLanguageCode[i].enG0Set;
                *penG2Set = g_stLanguageCode[i].enG2Set;
                break;
            }
        }

        if (i >= u32MaxLanNum)
        {
            i = (pstCurrentPoint->u32Flag >> 8) & 0x07; /** National char set */
            *penNationSet = g_stDefG0G2Map0010[i].enNationSet;
            /*set char set to default value*/
            *penG0Set = HI_UNF_TTX_G0SET_LATIN;
            *penG2Set = HI_UNF_TTX_G2SET_LATIN;
        }
        else
        {
            /*no found  X/28/0, X/28/4,M/29/0,M/29/4 conditon,need change charset,use default charset*/
            if (HI_FALSE == bUseDefaultCharSet)
            {
                *penG0Set = HI_UNF_TTX_G0SET_LATIN;
                *penG2Set = HI_UNF_TTX_G2SET_LATIN;
                *penNationSet = HI_UNF_TTX_NATION_SET_PRIMARY;
            }
        }
    }

    *penG0Set  = (HI_UNF_TTX_G0SET_BUTT == *penG0Set) ? HI_UNF_TTX_G0SET_LATIN : *penG0Set;
    *penG2Set  = (HI_UNF_TTX_G2SET_BUTT == *penG2Set) ? HI_UNF_TTX_G2SET_LATIN : *penG2Set;
    *penNationSet = (HI_UNF_TTX_NATION_SET_BUTT == *penNationSet) ? HI_UNF_TTX_NATION_SET_PRIMARY : *penNationSet;

    return;
}

HI_VOID TTX_Show_GetModifiedG0G2(HI_U8 u8ModifiedValue, HI_UNF_TTX_G0SET_E* penG0Set, HI_UNF_TTX_G2SET_E* penG2Set)
{
    HI_U32 i = (u8ModifiedValue >> 3) & 0xf;
    TTX_MODIFIED_G0G2_S* pstModifiedG0G2 = HI_NULL;

    switch (i)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            pstModifiedG0G2 = (TTX_MODIFIED_G0G2_S*)g_stModifiedG0G2Map00XX;
            break;

        case 4:
            pstModifiedG0G2 = (TTX_MODIFIED_G0G2_S*)g_stModifiedG0G2Map0100;
            break;

        case 6:
            pstModifiedG0G2 = (TTX_MODIFIED_G0G2_S*)g_stModifiedG0G2Map0110;
            break;

        case 8:
            pstModifiedG0G2 = (TTX_MODIFIED_G0G2_S*)g_stModifiedG0G2Map1000;
            break;

        case 0xa:
            pstModifiedG0G2 = (TTX_MODIFIED_G0G2_S*)g_stModifiedG0G2Map1010;
            break;

        default:
            break;
    }

    if (HI_NULL != pstModifiedG0G2)
    {
        i = u8ModifiedValue & 0x7;
        *penG0Set = pstModifiedG0G2[i].enG0Set;
        *penG2Set = pstModifiedG0G2[i].enG2Set;
    }
    else
    {
        *penG0Set = HI_UNF_TTX_G0SET_LATIN;
        *penG2Set = HI_UNF_TTX_G2SET_LATIN;
    }

    return;
}

HI_S32 TTX_Show_SetOSDOutput_ON(TTX_PAGE_CONTEXT_S* pstContextHead)
{
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    if (HI_NULL == pstCurrentPoint)        /** Invalid param, just return */
    {
        return HI_FAILURE;
    }

    TTX_SHOWPAGE_LOCK();

    pstCurrentPoint->bTtxHiden = HI_FALSE;

    TTX_SHOWPAGE_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Show_SetOSDOutput_OFF(TTX_PAGE_CONTEXT_S* pstContextHead)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_FILLRECT_S stFillRectParam ;
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    if (HI_NULL == pstCurrentPoint)    /** Invalid param, just return */
    {
        return HI_FAILURE;
    }

    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));


    TTX_SHOWPAGE_LOCK();

    pstCurrentPoint->bTtxHiden   = HI_TRUE;
    pstCurrentPoint->bUpdateTime = HI_FALSE;

    if (pstCurrentPoint->bNavigation)
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR,
                                   pstCurrentPoint->u32ColumnNum);
    }
    else
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, pstCurrentPoint->u32ColumnNum);
    }

    stFillRectParam.pstPageArea = &stPageArea;
    stFillRectParam.u32Color = 0x00000000;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);

    stRefrshLayerParam.pstPageArea = &stPageArea;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);

    TTX_SHOWPAGE_UNLOCK();

    return s32Ret;
}

/**  Enable  or disalbe the output */
HI_S32 TTX_Show_SetOSDOutputMsg(TTX_PAGE_CONTEXT_S* pstContextHead, HI_BOOL bEnable)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_MSG_ACTION_E enMsgAction = TTX_MSG_ACTION_BUTT;
    HI_HANDLE hDispalyHandle = HI_INVALID_HANDLE;

    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    if (HI_NULL == pstCurrentPoint)    /** Invalid param, just return */
    {
        return HI_FAILURE;
    }

    if (bEnable)
    {
        enMsgAction = TTX_MSG_ACTION_OSDOUTPUT_ON;
    }
    else
    {
        enMsgAction = TTX_MSG_ACTION_OSDOUTPUT_OFF;
    }

    hDispalyHandle = 0x7f7f0000 | enMsgAction;
    s32Ret = TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG, (HI_VOID*) &hDispalyHandle);

    return s32Ret;
}

HI_VOID    TTX_Show_ShowPrompt(TTX_PAGE_CONTEXT_S* pstContextHead)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 i = 0;
    HI_U8 j = 0;
    HI_U32 u32ColNum = 0;
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_UNF_TTX_FILLRECT_S stFillRectParam;
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = HI_NULL;
    HI_U8 u8szPrompt[22] = {0x54, 0x65, 0x6c, 0x65, 0x74, 0x65, 0x78, 0x74, 0x20,
                            0x53, 0x65, 0x61, 0x72, 0x63, 0x68, 0x69, 0x6e, 0x67, 0x2e, 0x2e, 0x2e, 0
                           };/** Teletext Searching... */

    pstCurrentPoint = pstContextHead;

    if (HI_NULL == pstCurrentPoint)    /** Invalid param, just return */
    {
        return;
    }

    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));


    TTX_SHOWPAGE_LOCK();

    if (pstCurrentPoint->bTtxHiden)
    {
        TTX_SHOWPAGE_UNLOCK();

        return;
    }

    u32ColNum = pstCurrentPoint->u32ColumnNum;

    if (pstCurrentPoint->bNavigation)
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR, u32ColNum);
    }
    else
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, u32ColNum);
    }

    stFillRectParam.pstPageArea = &stPageArea;
    stFillRectParam.u32Color = 0xff000000;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);

    for (i = 0, j = 9; i < 21; i++, j++)
    {
        TTX_Show_SetCharAttr(&stCharAttr, u8szPrompt[i], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_CHARSET_G0, 0);
        stDrawCharParam.pstCharAttr = &stCharAttr;
        s32Ret |= TTX_Show_SetArea(&stPageArea, 13, j, 1, 1);
        stDrawCharParam.pstPageArea   = &stPageArea;
        stDrawCharParam.u32Foreground = s_ColorSet[3];
        stDrawCharParam.u32Background = s_ColorSet[0];
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID*)&stDrawCharParam);
    }

    if (pstCurrentPoint->bNavigation)
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR, u32ColNum);
    }
    else
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, u32ColNum);
    }

    stRefrshLayerParam.pstPageArea = &stPageArea;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_Show_ShowPrompt, Errors occur while call functions\n");
    }

    pstCurrentPoint->bTTXSearching = HI_TRUE;
    TTX_SHOWPAGE_UNLOCK();
}

HI_VOID  TTX_ShowPromptMsg(TTX_PAGE_CONTEXT_S* pstContextHead)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_MSG_ACTION_E enMsgAction = TTX_MSG_ACTION_BUTT;
    HI_HANDLE hDispalyHandle = HI_INVALID_HANDLE;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    if (HI_NULL == pstCurrentPoint)    /** Invalid param, just return */
    {
        return;
    }

    enMsgAction = TTX_MSG_ACTION_SHOWPROMPT_SEARCH;
    hDispalyHandle = 0x7f7f0000 | enMsgAction;
    s32Ret = TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG, (HI_VOID*) &hDispalyHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_ShowPromptMsg, Errors occur while call functions\n");
    }
}

HI_VOID    TTX_Show_ShowPromptTimeOut(TTX_PAGE_CONTEXT_S* pstContextHead)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 i = 0;
    HI_U8 j = 0;
    HI_U32 u32ColNum = 0;
    HI_UNF_TTX_CHARATTR_S stCharAttr;
    HI_UNF_TTX_DRAWCAHR_S stDrawCharParam;
    HI_UNF_TTX_FILLRECT_S stFillRectParam;
    HI_UNF_TTX_REFRESHLAYER_S stRefrshLayerParam;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = HI_NULL;
    HI_UNF_TTX_PAGEAREA_S stPageArea;
    HI_U8 u8szPrompt[26] = {0x54, 0x65, 0x6c, 0x65, 0x74, 0x65, 0x78, 0x74, 0x20,
                            0x49, 0x73, 0x20, 0x4e, 0x6f, 0x74, 0x20, 0x41, 0x76, 0x61, 0x69, 0x6c, 0x61, 0x62, 0x6c, 0x65, 0
                           };/** Teletext Is Not Available */

    pstCurrentPoint = pstContextHead;

    if (HI_NULL == pstCurrentPoint)    /** Invalid param, just return */
    {
        return;
    }



    memset(&stCharAttr, 0x0, sizeof(HI_UNF_TTX_CHARATTR_S));
    memset(&stDrawCharParam, 0x0, sizeof(HI_UNF_TTX_DRAWCAHR_S));
    memset(&stPageArea, 0x0, sizeof(HI_UNF_TTX_PAGEAREA_S));
    memset(&stFillRectParam, 0x0, sizeof(HI_UNF_TTX_FILLRECT_S));
    memset(&stRefrshLayerParam, 0x0, sizeof(HI_UNF_TTX_REFRESHLAYER_S));


    TTX_SHOWPAGE_LOCK();

    if (pstCurrentPoint->bTtxHiden)
    {
        TTX_SHOWPAGE_UNLOCK();
        return;
    }

    u32ColNum = pstCurrentPoint->u32ColumnNum;

    if (pstCurrentPoint->bNavigation)
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR, u32ColNum);
    }
    else
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, u32ColNum);
    }

    stFillRectParam.pstPageArea = &stPageArea;
    stFillRectParam.u32Color = 0xff000000;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_FILLRECT, (HI_VOID*)&stFillRectParam);

    for (i = 0, j = 9; i < 25; i++, j++)
    {
        TTX_Show_SetCharAttr(&stCharAttr, u8szPrompt[i], HI_FALSE, HI_UNF_TTX_G0SET_LATIN, HI_UNF_TTX_G2SET_LATIN, HI_UNF_TTX_CHARSET_G0, 0);
        stDrawCharParam.pstCharAttr = &stCharAttr;
        s32Ret |= TTX_Show_SetArea(&stPageArea, 13, j, 1, 1);
        stDrawCharParam.pstPageArea   = &stPageArea;
        stDrawCharParam.u32Foreground = s_ColorSet[3];
        stDrawCharParam.u32Background = s_ColorSet[0];
        s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_DRAWCHAR, (HI_VOID*)&stDrawCharParam);
    }

    if (pstCurrentPoint->bNavigation)
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR, u32ColNum);
    }
    else
    {
        s32Ret |= TTX_Show_SetArea(&stPageArea, 0, 0, TTX_ROW_NUM + TTX_SUBPAGE_BAR, u32ColNum);
    }

    stRefrshLayerParam.pstPageArea = &stPageArea;
    s32Ret |= TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_APP_REFRESH, (HI_VOID*)&stRefrshLayerParam);

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_Show_ShowPromptTimeOut, Errors occur while call functions\n");
    }

    pstCurrentPoint->bTTXSearching = HI_FALSE;
    TTX_SHOWPAGE_UNLOCK();
}

HI_VOID  TTX_ShowPromptTimeOutMsg(TTX_PAGE_CONTEXT_S* pstContextHead)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_MSG_ACTION_E enMsgAction = TTX_MSG_ACTION_BUTT;
    HI_HANDLE hDispalyHandle = HI_INVALID_HANDLE;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = pstContextHead;

    if (HI_NULL == pstCurrentPoint)    /** Invalid param, just return */
    {
        return;
    }

    enMsgAction = TTX_MSG_ACTION_SHOWPROMPT_SEARCH_TIMEOUT;
    hDispalyHandle = 0x7f7f0000 | enMsgAction;
    s32Ret = TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_TTX_TO_APP_MSG, (HI_VOID*) &hDispalyHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_TTX("TTX_ShowPromptTimeOutMsg, Errors occur while call functions\n");
    }
}
HI_S32 TTX_Show_CallBack(TTX_PAGE_CONTEXT_S* pstContextHead, HI_UNF_TTX_CB_E enCBType, HI_VOID* pvParam)
{
    TTX_PAGE_CONTEXT_S*  pstCurrentPoint = pstContextHead;

    if ((HI_NULL == pstCurrentPoint) || (HI_NULL == pstCurrentPoint->pfnCallBackFunction))
    {
        return HI_FAILURE;
    }

    return pstCurrentPoint->pfnCallBackFunction( pstCurrentPoint->hTTX, enCBType, pvParam);
}

