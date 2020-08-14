#ifndef __HISI_LAYER_H__
#define __HISI_LAYER_H__


#ifdef CONFIG_GFX_DFB_OPTV5_SUPPORT
#include <config.h>

#include <asm/types.h>

#include <stdio.h>
#include <sys/mman.h>

#include <directfb.h>

//#include <core/screens.h>

#include <direct/debug.h>
#include <direct/messages.h>

#include <sys/ioctl.h>
#include <fbdev/fbdev.h>
#include <core/layers.h>

extern DisplayLayerFuncs hisi_layer_funcs;
#endif


#endif
