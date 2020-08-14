/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAZ8tsS4we+PBWC6a/UNlzCWIaw+Ujkv9NAY+as0
fg7WZIRvw27AjvRqJbkRJvqFUORSa6KPQaSBMxCxJTGTTf//sQbjPOyYldN0OVR9ut4HFO4U
ZguGQVqcOAJQbE96v6175DqhuprKgQB8R+2fu7VD3qtX+ZJh/t0512oqv+e8YA==*/
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

/*可支持的SCD个数,总是与VDH成对交付*/
#define MAX_SCD_NUM  MAX_VDH_NUM
#define MAX_DSP_NUM  MAX_SCD_NUM

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

#define BPD_INT_NUM            (99+32)
#define DNR_INT_NUM            (61+32)
#define BTL_INT_NUM            (103+32)
/* 测试单板上可被视频解码使用的memory预算 */
//#define BOARD_MEM_BASE_ADDR    0x98000000
#define BOARD_MEM_BASE_ADDR    0x08000000
#define BOARD_MEM_TOTAL_SIZE   (72*1024*1024)

#endif
