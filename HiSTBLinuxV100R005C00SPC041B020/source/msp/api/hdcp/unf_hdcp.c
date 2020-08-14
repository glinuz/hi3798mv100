#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include "hi_drv_struct.h"
#include "hi_common.h"
#include "hi_mpi_cipher.h"

/***********************************************************************************
*  Function:       HI_UNF_HDCP_SetHDCPKey
*  Description:    Burn HDCP Key
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:      ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32  HI_UNF_HDCP_EncryptHDCPKey(HI_UNF_HDCP_HDCPKEY_S stHdcpKey, HI_BOOL bIsUseOTPRootKey, HI_U8 u8OutEncryptKey[332])
{
    return HI_MPI_CIPHER_EncryptHDCPKey(&stHdcpKey, bIsUseOTPRootKey, u8OutEncryptKey);
}


