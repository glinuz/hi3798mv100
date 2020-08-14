#ifndef AVL6381_API_H
#define AVL6381_API_H


#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>

#include "drv_tuner_ext.h"
#include "hi_drv_i2c.h"
#include "hi_type.h"
#include "drv_demod.h"
#include "hi_kernel_adapt.h"

#include "AVL6381_iic_rw.h"
#include "AVL6381.h"
#include "AVL6381_Internal.h"
#include "AVL6381_data_type.h"

#define AVL6381_API_VER_MAJOR   1
#define AVL6381_API_VER_MINOR   16
#define AVL6381_API_VER_BUILD   30

// The Availink version structure.
typedef struct AVL63xx_VerInfo {
    AVL6381_uchar m_Major;                  // The major version number
    AVL6381_uchar m_Minor;                  // The minor version number
    AVL6381_uint16 m_Build;                 // The build version number
}AVL63xx_VerInfo;
    
// Stores AVL6381 device version information.
typedef struct AVL6381_VerInfo
{
    struct AVL63xx_VerInfo m_Chip;          // Hardware version information. Should be X.X.X. 
    struct AVL63xx_VerInfo m_API;           // SDK version information.
    struct AVL63xx_VerInfo m_Patch;         // The version of the internal patch.
} AVL6381_VerInfo;


//extern AVL6381_ErrorCode AVL6381_I2CBypassOn(HI_U8 IICchannel, HI_U32 IICaddr);
//extern AVL6381_ErrorCode AVL6381_I2CBypassOff(HI_U8 IICchannel, HI_U32 IICaddr);
extern AVL6381_ErrorCode AVL6381_I2CBypassOn(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_I2CBypassOff(struct AVL6381_Chip * pAVLChip);
extern AVL6381_uint16    AVL6381_GetChipID(struct AVL6381_Chip * pAVLChip, AVL6381_puint32 puiChipID);
extern AVL6381_ErrorCode AVL6381_GetVersion(AVL6381_VerInfo * pVerInfo, struct AVL6381_Chip * pAVLChip);

extern HI_S32 AVL6381_Mutex_init(struct semaphore * stS);
extern void   AVL6381_Mutex_Lock(struct semaphore * stS);
extern void   AVL6381_Mutex_Unlock(struct semaphore * stS);

#endif

