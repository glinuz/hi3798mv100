#ifndef SECURE_GIC_COMMON_H
#define SECURE_GIC_COMMON_H

#include "sre_base.h"
#include "secure_gic_offset.h"

/*interrupt value define*/
#define INT_SECURE                  0x0
#define INT_UNSECURE              0x1
#define INT_ALL_UNSECURE              0xffffffff
#define SPI_INT_ENABLE              0xffffffff
#define SPI_INT_DISENABLE         0xffffffff

extern UINT32 g_gic_cpu_inter_base;
extern UINT32 g_gic_dis_inter_base;

#define GIC_IC_PHY_BASE             (g_gic_cpu_inter_base)
#define GIC_ID_PHY_BASE             (g_gic_dis_inter_base)
#endif /* SECURE_GIC_COMMON_H */
