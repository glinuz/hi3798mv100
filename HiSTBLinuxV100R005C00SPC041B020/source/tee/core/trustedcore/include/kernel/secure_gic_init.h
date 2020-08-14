/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : secure_gic_init.c
* Description :
* Platform    :
* Author      : z00206978
* Version     : 1.0
* Date        : 2012.8.23
* Notes       :
*
*------------------------------------------------------------------------------
* Modifications:
*   Date        Author          Modifications
*******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
*******************************************************************************/
#ifndef SECURE_GIC_INIT_H
#define SECURE_GIC_INIT_H

#include "sre_base.h"
#include "secure_gic_common.h"


extern void secure_ext_GIC_init(void);
extern void GIC_normal_cpuInterface_ctrl_init(void);
extern void normal_binary_point_init(void);
extern void GIC_PRIMASK_init(void);
extern void GIC_secure_cpuInterface_ctrl_init(void);
extern void GIC_normal_cpuInterface_ctrl_init(void);
extern void secure_distributor_enable(void);
extern void secure_distributor_disable(void);
UINT32 osGetCoreGICInterface();

#endif
