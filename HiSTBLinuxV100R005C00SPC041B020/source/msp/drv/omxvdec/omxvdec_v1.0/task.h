/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    task.h
 *
 * Purpose: omxvdec task interface
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#ifndef __TASK_H__
#define __TASK_H__

#include "omxvdec.h"


HI_S32 task_create_thread(OMXVDEC_ENTRY* vdec);

HI_S32 task_destroy_thread(OMXVDEC_ENTRY* vdec);

HI_VOID task_proc_entry(struct seq_file *p, OMXVDEC_TASK *ptask);


#endif

