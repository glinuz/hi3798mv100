#ifndef __DRV_COMMON_H__
#define __DRV_COMMON_H__

#ifdef __cplusplus
 #if __cplusplus
        extern "C" {
 #endif
#endif /* __cplusplus */

#define     MOTOR_STORE_POSITION_MAX_NUM            64

//ADC COLLECT
#define TUNER_FILE_NAME_LEN   (256)

#define HI_QTC_BASE                     (0xF8A60000)
#define QTC_WRITE_REG(addr, value)    (*(volatile HI_U32*)(addr) = (value))
#define QTC_READ_REG(addr)            (*(volatile HI_U32*)(addr))


typedef union
{
    struct
    {
        HI_U32 Position1:   1;  /*bit[0]*/
        HI_U32 Position2:   1;  /*bit[1]*/
        HI_U32 Position3:   1;  /*bit[2]*/
        HI_U32 Position4:   1;
        HI_U32 Position5:   1;
        HI_U32 Position6:   1;
        HI_U32 Position7:   1;
        HI_U32 Position8:   1;
        HI_U32 Position9:   1;
        HI_U32 Position10:  1;
        HI_U32 Position11:  1;
        HI_U32 Position12:  1;
        HI_U32 Position13:  1;
        HI_U32 Position14:  1;
        HI_U32 Position15:  1;
        HI_U32 Position16:  1;
        HI_U32 Position17:  1;
        HI_U32 Position18:  1;
        HI_U32 Position19:  1;
        HI_U32 Position20:  1;
        HI_U32 Position21:  1;
        HI_U32 Position22:  1;
        HI_U32 Position23:  1;
        HI_U32 Position24:  1;
        HI_U32 Position25:  1;
        HI_U32 Position26:  1;
        HI_U32 Position27:  1;
        HI_U32 Position28:  1;
        HI_U32 Position29:  1;
        HI_U32 Position30:  1;
        HI_U32 Position31:  1;
        HI_U32 Position32:  1;
        HI_U32 Position33:  1;
        HI_U32 Position34:  1;
        HI_U32 Position35:  1;
        HI_U32 Position36:  1;
        HI_U32 Position37:  1;
        HI_U32 Position38:  1;
        HI_U32 Position39:  1;
        HI_U32 Position40:  1;
        HI_U32 Position41:  1;
        HI_U32 Position42:  1;
        HI_U32 Position43:  1;
        HI_U32 Position44:  1;
        HI_U32 Position45:  1;
        HI_U32 Position46:  1;
        HI_U32 Position47:  1;
        HI_U32 Position48:  1;
        HI_U32 Position49:  1;
        HI_U32 Position50:  1;
        HI_U32 Position51:  1;
        HI_U32 Position52:  1;
        HI_U32 Position53:  1;
        HI_U32 Position54:  1;
        HI_U32 Position55:  1;
        HI_U32 Position56:  1;
        HI_U32 Position57:  1;
        HI_U32 Position58:  1;
        HI_U32 Position59:  1;
        HI_U32 Position60:  1;
        HI_U32 Position61:  1;
        HI_U32 Position62:  1;
        HI_U32 Position63:  1;
        HI_U32 Position64:  1;
        }bits;

    HI_U32 u32Flags[2];
}U32_MOTOR_FLAG;

typedef struct
{
    HI_UNF_TUNER_DISEQC_SWITCH4PORT_S stPort1_0;
    HI_UNF_TUNER_DISEQC_SWITCH16PORT_S stPort1_1;
    U32_MOTOR_FLAG                      stMotorFlag;
    HI_S16                              s16MotorPos;
    HI_S16                              s16EastLimit;
    HI_S16                              s16WestLimit;
    HI_S16                              s16StorePos[MOTOR_STORE_POSITION_MAX_NUM];
} DISEQC_DRV_STATUS_S;



/*100 * log10(u32InValue)*/
HI_U32 tuner_get_100lg_value(HI_U32 u32InValue);

/*a ^ b*/
HI_U32 tuner_get_pow(HI_U32 a, HI_U32 b);



HI_S32 tuner_get_adc_data(HI_U32 u32TunerPort, HI_U32 u32DmxPort, HI_U32 u32TimeS);


#ifdef __cplusplus
 #if __cplusplus
    }
 #endif
#endif /* __cplusplus */

#endif  /* __DRV_COMMON_H__ */
