/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv/JzwTgKEHe/9N4yU5rqhEDaRH/mYdIijFrTqr/UaRBfB2ylm29ZtemC0p1r
w4HAPYRiuKi4GvKfi8iu84r1JEaaq3tNwB6X1M35tlzg0RhYXjjkOGKalHVXNqj9DZSjBg5k
dAvamuVCN+WzNx/CC5UzQZWVuW7HG8ZYVtdjOPuBv8HmXBUvSpW7iLN2TwByBw==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
#ifndef __VFMW_SYSCONFIG_HEADER__
#define __VFMW_SYSCONFIG_HEADER__

//#define TWO_VDH

/* 最大可支持的通道数目 */
#ifdef CFG_MAX_CHAN_NUM
#define MAX_CHAN_NUM CFG_MAX_CHAN_NUM
#else
#define MAX_CHAN_NUM 32
#endif

/*可支持的VDH个数*/
#if defined(TWO_VDH)
#define MAX_VDH_NUM 2
#else
#define MAX_VDH_NUM 1
#endif

/* 系统控制寄存器地址和位域 */
#define SCD_RESET_REG_PHY_ADDR   0x0   //scd
/* 硬件加速IP接口寄存器分址 */
#define VDM_REG_PHY_ADDR       0xf8c30000    // VDM0 寄存器映射到ARM空间的地址
#define VDM_REG_PHY_ADDR_1     0xf8c30000    // VDM1寄存器映射到ARM空间的地址
#define DNR_REG_PHY_ADDR       0x10450000    //DNR
#define SCD_REG_PHY_ADDR       0xf8c3c000    // scd 寄存器映射到ARM空间的地址
#define SCD_REG_PHY_ADDR_1     SCD_REG_PHY_ADDR    // scd 寄存器映射到ARM空间的地址
#define FOD_REG_PHY_ADDR       0x10150000    // fod 寄存器映射到ARM空间的地址
#define BPD_REG_PHY_ADDR       0xf8c60000
#define BTL_REG_PHY_ADDR       0xf8d00000   //0x10450000 // 0x101b0000   //BTL寄存器映射到ARM空间的地址，起始地址待定
#define SYSTEM_REG_RANGE       (1024*1024)
/* 中断号 */
#define VDM_INT_NUM            (95+32)
#define VDM_INT_NUM_1          (95+32)
#define SCD_INT_NUM            (104+32)

#ifdef ENV_SOS_KERNEL
/*sec interrupt*/
#define SEC_VDM_INT_NUM        (105+32)
#define SEC_VDM_INT_NUM_1      (105+32)
#define SEC_SCD_INT_NUM        (106+32)
#endif

#define BPD_INT_NUM            (99+32)  
#define DNR_INT_NUM            (61+32)
#define BTL_INT_NUM            (103+32)

#ifdef ENV_SOS_KERNEL
#define SYSTEM_REG_PHY_ADDR        0xf8a22000
#define BOARD_MEM_BASE_ADDR        0x3E200000
#define BOARD_MEM_TOTAL_SIZE       (10*1024*1024) //非安的要删掉，暂时保留10M做初始化用
#define BOARD_MEM_BASE_ADDR_Sec    0x3B000000     //安全内存基地址
#define BOARD_MEM_TOTAL_SIZE_Sec   (50*1024*1024) //安全内存总大小
#else
/* 测试单板上可被视频解码使用的memory预算 */
//#define BOARD_MEM_BASE_ADDR    0x98000000
#define BOARD_MEM_BASE_ADDR    0x08000000
#define BOARD_MEM_TOTAL_SIZE   (72*1024*1024)
#endif

#endif
