
/*******************************************************************************
*                       Include files
*******************************************************************************/
#include <string.h>

#include "hi_type.h"
#include "aribcc_obj.h"
#include "cc_debug.h"

#define MIN_BUFFER_SIZE         (0x10000) /*64 Kbyte*/
#define ARIBCC_BUFFER_SIZE      (0x80000) /*512 Kbyte*/

static ARIBCC_OBJECT_S s_stAribccObject;

/*******************************************************************************
*                       Export Function Definition
*******************************************************************************/
HI_S32 ARIBCC_OBJ_Create(HI_UNF_CC_ARIB_CONFIGPARAM_S *pstAribConfig)
{
    memset(&s_stAribccObject, 0, sizeof(ARIBCC_OBJECT_S));
    strncpy(s_stAribccObject.acISO639LanguageCode,"jpn",3);
    s_stAribccObject.acISO639LanguageCode[3] = '\0';
    s_stAribccObject.u32PesBufSize = ARIBCC_BUFFER_SIZE;

    if (pstAribConfig)
    {
        if (pstAribConfig->u32BufferSize < MIN_BUFFER_SIZE)
        {
            s_stAribccObject.u32PesBufSize = MIN_BUFFER_SIZE;
        }
        else if (pstAribConfig->u32BufferSize > ARIBCC_BUFFER_SIZE)
        {
            s_stAribccObject.u32PesBufSize = ARIBCC_BUFFER_SIZE;
        }
        else
        {
            s_stAribccObject.u32PesBufSize = pstAribConfig->u32BufferSize;
        }
    }

    return HI_SUCCESS;
}

ARIBCC_OBJECT_S *ARIBCC_OBJ_Get(void)
{
    return &(s_stAribccObject);
}
