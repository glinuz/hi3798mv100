#ifndef __MXL203_H__
#define __MXL203_H__

extern HI_S32 mxl203_set_tuner(HI_U32 u32TunerPort, HI_U8 i2c_channel, HI_U32 puRF);

extern HI_VOID mxl203_set_qam_reg( HI_U32 u32TunerPort );
extern HI_VOID mxl203_manage_after_qamreset( HI_U32 u32TunerPort );

#endif /* __MXL203_H__ */
