#ifndef  __HI_DRV_PMOC_H__
#define  __HI_DRV_PMOC_H__

/* add register macro */
#define C51_BASE             0xf8400000
#define C51_SIZE             0x8000
#define C51_DATA             0x7000
#define C51_START	         0xf000

#define CFG_BASE_ADDR        0xf8000000
#define PERI_BASE_ADDR       0xf8a20000
#define PERI_USB_RESUME_INT_MASK 0xb4

#define LOW_POWER_CTRL       0x40
#define SRST_REQ_CTRL        0x48
#define SC_GEN12		     (CFG_BASE_ADDR + 0xb0)
#define SC_GEN15		     (CFG_BASE_ADDR + 0xbc) //This register is set by the fastboot, indicated the C51 code is loaded.

#define MCU_IRKEY_MAXNUM     6
#define MCU_CODE_MAXSIZE     0x7000
#define MAX_BOOTARGS_SIZE    512
#define C51_CODE_LOAD_FLAG   0x80510002 //indicate the C51 code is loaded, kernel will check this flag

#define MCU_STATUS_ADDR (C51_BASE + C51_DATA + 0x51c)
#define MCU_LOAD_CODE 0x12345678
#define KERNEL_RUNNING 0x87654321

#define MCU_BOOT_SUSPEND_ADDR (C51_BASE + C51_DATA + 0x52c)
#define MCU_BOOT_SUSPEND_FLAG 0x12345678

#endif  /*  __HI_DRV_PMOC_H__ */

