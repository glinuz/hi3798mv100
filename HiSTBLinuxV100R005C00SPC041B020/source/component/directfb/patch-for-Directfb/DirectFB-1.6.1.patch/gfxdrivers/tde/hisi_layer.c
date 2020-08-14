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
#include "hisi_layer.h"

//#include "hi_unf_disp.h"
#include "hi_mpi_disp.h"

#include "hi_unf_vo.h"
#include "hi_mpi_win.h"

#include "hi_mpi_hdmi.h"
#if 0
 /*
  * Return size of layer data (shared memory).
  */
int hisi_layer_data_size ( void )
{
	return 0;
}

 /*
  * Return size of region data (shared memory).
  */
int hisi_region_data_size ( void )
{
	return 0;
}


static int find_window_from_layer(CoreLayer *layer, HI_DRV_WIN_ATTR_S *attr)
{
	int i;
	int       num = dfb_layer_num();
	HI_HANDLE win;

	win = (HI_HANDLE)((HI_ID_VO << 16) | (HI_UNF_DISPLAY1 << 8)  |(((HI_U32)layer->shared->layer_id - VIDEO_LAY_ID)&0xff));

	if (HI_SUCCESS != HI_MPI_WIN_GetAttr(win, attr))
		return -1;
	return HI_SUCCESS;
}
#endif

#define VIDEO_LAY_ID (1UL)

 /*
  * Called once by the master to initialize layer data and reset hardware.
  * Return layer description, default configuration and color adjustment.
  */
DFBResult hisi_init_layer ( CoreLayer                  *layer,
                              void                       *driver_data,
                              void                       *layer_data,
                              DFBDisplayLayerDescription *description,
                              DFBDisplayLayerConfig      *config,
                              DFBColorAdjustment         *adjustment )
{
	//HI_HANDLE win,cwin;
	//HI_UNF_WINDOW_ATTR_S attr;

	if (HI_SUCCESS != HI_MPI_WIN_Init()) {
		D_ERROR("%s->%d, fail!\n",__func__,__LINE__);
		return DFB_FAILURE;
	}

     /* set capabilities and type */
     description->caps = DLCAPS_SCREEN_LOCATION | DLCAPS_SURFACE;
     description->type = DLTF_VIDEO | DLTF_STILL_PICTURE;

     /* set name */
     snprintf( description->name, DFB_DISPLAY_LAYER_DESC_NAME_LENGTH, "HISI Video" );

     /* fill out the default configuration */
     config->flags  = DLCONF_WIDTH | DLCONF_HEIGHT | DLCONF_BUFFERMODE; 
	 config->buffermode = DLBM_FRONTONLY;
     //config->options     = DLOP_NONE;


	/* FIXME! Is set output rect right? */
     //config->width       = attr.stOutputRect.s32Width;
     //config->height      = attr.stOutputRect.s32Height;
	
	 config->width       = 1920;
     config->height      = 1080;
     return DFB_OK;

}
/*
* Check all parameters and return if this region is supported.
*/
DFBResult hisi_test_region   ( CoreLayer                  *layer,
                         void                       *driver_data,
                         void                       *layer_data,
                         CoreLayerRegionConfig      *config,
                         CoreLayerRegionConfigFlags *failed )
{
	 /* FIXME! DO WHAT ? */
     //layer->shared->contexts.primary->primary.region->state &= ~CLRSF_FROZEN;
     if (failed)
		*failed = CLRCF_NONE;
	return DFB_OK;
}
 /*
  * Setup hardware, called once after AddRegion() or when parameters
  * have changed. Surface and palette are only set if updated or new.
  */
DFBResult hisi_set_region( CoreLayer                  *layer,
                             void                       *driver_data,
                             void                       *layer_data,
                             void                       *region_data,
                             CoreLayerRegionConfig      *config,
                             CoreLayerRegionConfigFlags  updated,
                             CoreSurface                *surface,
                             CorePalette                *palette,
                             CoreSurfaceBufferLock      *lock )
{
	HI_DRV_WIN_ATTR_S attr;
	HI_HANDLE win;

	if (!layer || !layer->shared) {
		D_ERROR("%s->%d, !layer %d, !layer->shared:%d,Cannot get window handle!\n",__func__,__LINE__,!layer, !layer->shared);
		return DFB_FAILURE;
	}

	win = (HI_HANDLE)((HI_ID_VO << 16) | (HI_UNF_DISPLAY1 << 8)  |(((HI_U32)layer->shared->layer_id - VIDEO_LAY_ID)&0xff));

	printf("%s->%d, win hanlde:0x%x\n",__func__,__LINE__,win);

	if (HI_SUCCESS != HI_MPI_WIN_GetAttr(win,  &attr)) {
		D_ERROR("hisi_set_region Fail to get window attr!\n");
		return DFB_FAILURE;
	}

	attr.stOutRect.s32X = config->dest.x;
	attr.stOutRect.s32Y = config->dest.y;
	attr.stOutRect.s32Width = config->dest.w;
	attr.stOutRect.s32Height = config->dest.h;

	if (HI_SUCCESS != HI_MPI_WIN_SetAttr(win, &attr)) {
		D_ERROR("hisi_set_region Fail to set window attr!\n");
		return DFB_FAILURE;
	}

	return DFB_OK;
}

/*
* Return the z position of the layer.
*/
DFBResult hisi_get_level ( CoreLayer              *layer,
                                 void                   *driver_data,
                                 void                   *layer_data,
                                 int                    *level )
{
	return DFB_UNSUPPORTED;
}

/*
* Move the layer below or on top of others (z position).
*/
DFBResult hisi_set_level ( CoreLayer              *layer,
                                 void                   *driver_data,
                                 void                   *layer_data,
                                 int                     level )
{
	return DFB_UNSUPPORTED;
}

/*
* Adjust brightness, contrast, saturation etc.
*/
DFBResult hisi_set_color_adjustment ( CoreLayer              *layer,
                                 void                   *driver_data,
                                 void                   *layer_data,
                                 DFBColorAdjustment     *adjustment )

{
	return DFB_UNSUPPORTED;
}

DisplayLayerFuncs hisi_layer_funcs = {
//	.LayerDataSize = hisi_layer_data_size,
//	.RegionDataSize = hisi_region_data_size,
	
	.InitLayer = hisi_init_layer,

	.TestRegion = hisi_test_region,
	.SetRegion = hisi_set_region,

    .GetLevel = hisi_get_level,
    .SetLevel = hisi_set_level,

    .SetColorAdjustment = hisi_set_color_adjustment,
};

#endif
