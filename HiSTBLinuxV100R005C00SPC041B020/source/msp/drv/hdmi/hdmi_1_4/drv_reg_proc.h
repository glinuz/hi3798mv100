
#ifndef __DRV_REG_PROC_H__
#define __DRV_REG_PROC_H__
#include "hi_unf_hdmi.h"
#include "hi_drv_hdmi.h"
#include "hi_drv_disp.h"
#include "si_hlviic.h"
#include "si_defstx.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

//HI3716Cv200 Series
#if    defined(CHIP_TYPE_hi3716cv200) \
    || defined(CHIP_TYPE_hi3716mv400) \
    || defined(CHIP_TYPE_hi3718cv100) \
    || defined(CHIP_TYPE_hi3719cv100) \
    || defined(CHIP_TYPE_hi3718mv100) \
    || defined(CHIP_TYPE_hi3719mv100) \
    || defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

#define HDMI_TX_BASE_ADDR     0xf8ce0000L

#define HDMI_TX_PHY_ADDR      0xf8ce1800L

    //for colorbar
#define VDP_DHD_0_CTRL        0xf8ccc000L
#define VDP_DHD_1_CTRL        0xf8ccc400L

/*--NO MODIFY : COMMENT BY CODINGPARTNER--
typedef union
{
    struct
    {
        HI_U32 hdcp_mode_en         : 1;    // [0]
        HI_U32 tx_read              : 1;    // [1]
        HI_U32 hdcp_rootkey_sel     : 2;    // [3:2]
        HI_U32 rx_read              : 1;    // [4]
        HI_U32 rx_sel               : 1;    // [5]
        HI_U32 reserved             : 26;   // [31:6]
    } bits;
    HI_U32 u32;
} CIPHER_HDCP_MODE_CTRL_U;  //Offset:0x820
*/ // bit[0]/bit[1] set to 1
#define CIPHER_HDCP_MODE_CRL  0xF9A00820L


#elif  defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

#define HDMI_TX_BASE_ADDR     0xff100000L


#define HDMI_TX_PHY_ADDR      0xff101800L

//for colorbar
#define VDP_DHD_0_CTRL        0xff12c000L
#define VDP_DHD_1_CTRL        0xff12c400L

//Is there any change?
#define CIPHER_HDCP_MODE_CRL  0xF9A00820L

#endif


// some reg
// for 3716cv200EC Version bit15~bit8 ori:0x40,ec:0x41
#define EC_3716CV200_VER      0xF8a9007c



HI_S32 DRV_HDMI_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value);
HI_S32 DRV_HDMI_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value);

HI_U8 DRV_ReadByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr);
void DRV_WriteByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U8 Data);
HI_U16 DRV_ReadWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr);
HI_U8 DRV_ReadByte_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr);
HI_U16 DRV_ReadWord_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr);
void DRV_WriteWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U16 Data);
void DRV_WriteWord_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr, HI_U16 Data);
void DRV_WriteByte_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr, HI_U8 Data);
HI_S32 DRV_BlockRead_8BAS(I2CShortCommandType * I2CCommand, HI_U8 * Data);
HI_U8 DRV_BlockWrite_8BAS( I2CShortCommandType * I2CCommand, HI_U8 * Data );
//HI_U8 DRV_HDMI_HWReset(HI_U32 u32Enable);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_REG_PROC_H__ */

