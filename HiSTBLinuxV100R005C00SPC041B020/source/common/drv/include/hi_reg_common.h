
#ifndef __HI_REG_COMMON_H__
#define __HI_REG_COMMON_H__

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

#include "./hi3798mv100/hi_reg_sys.h"
#include "./hi3798mv100/hi_reg_peri.h"
#include "./hi3798mv100/hi_reg_io.h"
#include "./hi3798mv100/hi_reg_crg.h"
#include "./hi3798mv100/hi_reg_vpss.h"
#include "./hi3798mv100/hi_reg_vdp.h"

#elif defined(CHIP_TYPE_hi3798mv200)

#include "./hi3798mv200/hi_reg_sys.h"
#include "./hi3798mv200/hi_reg_peri.h"
#include "./hi3798mv200/hi_reg_io.h"
#include "./hi3798mv200/hi_reg_crg.h"
#include "./hi3798mv200/hi_reg_vdp.h"
#include "./hi3798mv200/hi_reg_vpss.h"
#include "./hi3798mv200/hi_reg_vpss_wbc.h"

#elif defined(CHIP_TYPE_hi3798mv200_a)

#include "./hi3798mv200_a/hi_reg_sys.h"
#include "./hi3798mv200_a/hi_reg_peri.h"
#include "./hi3798mv200_a/hi_reg_io.h"
#include "./hi3798mv200_a/hi_reg_crg.h"
#include "./hi3798mv200_a/hi_reg_vdp.h"
#include "./hi3798mv200_a/hi_reg_vpss.h"
#include "./hi3798mv200_a/hi_reg_vpss_wbc.h"

#elif defined(CHIP_TYPE_hi3798cv200)

#include "./hi3798cv200/hi_reg_sys.h"
#include "./hi3798cv200/hi_reg_peri.h"
#include "./hi3798cv200/hi_reg_io.h"
#include "./hi3798cv200/hi_reg_crg.h"
#include "./hi3798cv200/hi_reg_vdp.h"
#include "./hi3798cv200/hi_reg_vpss.h"
#include "./hi3798cv200/hi_reg_vpss_wbc.h"

#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) 

#include "./hi3716mv410/hi_reg_sys.h"
#include "./hi3716mv410/hi_reg_peri.h"
#include "./hi3716mv410/hi_reg_io.h"
#include "./hi3716mv410/hi_reg_crg.h"
#include "./hi3716mv410/hi_reg_vpss.h"
#include "./hi3716mv410/hi_reg_vdp.h"
#include "./hi3716mv410/hi_reg_vpss_wbc.h"

#elif defined(CHIP_TYPE_hi3796mv200)

#include "./hi3796mv200/hi_reg_sys.h"
#include "./hi3796mv200/hi_reg_peri.h"
#include "./hi3796mv200/hi_reg_io.h"
#include "./hi3796mv200/hi_reg_crg.h"
#include "./hi3796mv200/hi_reg_vdp.h"
#include "./hi3796mv200/hi_reg_vpss.h"
#include "./hi3796mv200/hi_reg_vpss_wbc.h"

#endif

#define HI_SYS_BASE_ADDR        0xF8000000
#define HI_PERI_BASE_ADDR       0xF8A20000
#define HI_IO_BASE_ADDR         0xF8A21000
#define HI_CRG_BASE_ADDR        0xF8A22000

typedef S_SYSCTRL_REGS_TYPE     HI_REG_SYSCTRL_S;
typedef S_PERICTRL_REGS_TYPE    HI_REG_PERI_S;
typedef S_IO_REGS_TYPE          HI_REG_IO_S;
typedef S_CRG_REGS_TYPE         HI_REG_CRG_S;

extern volatile HI_REG_SYSCTRL_S    *g_pstRegSysCtrl;
extern volatile HI_REG_CRG_S        *g_pstRegCrg;
extern volatile HI_REG_IO_S         *g_pstRegIO;
extern volatile HI_REG_PERI_S       *g_pstRegPeri;

#endif  // __HI_REG_COMMON_H__

