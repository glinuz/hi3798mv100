/*=================================================

Open MAX   Component: OMX Tunnel Implementation
This module contains the class definition for openMAX tunnel mode.
File:    omx_tunnel.h
Author:  yangyichang 00226912
Date:    23, 08, 2016.

=================================================*/

#ifndef __OMX_TUNNEL_H__
#define __OMX_TUNNEL_H__

#include "OMX_Types.h"
#include "OMX_VideoExt.h"
#include "gralloc_priv.h"

typedef struct
{
    HI_HANDLE play_handle;
    HI_HANDLE win_handle;
    ConfigureVideoTunnelModeParams config_param;
    native_handle_t native_handle;

}OMX_TUNNEL_INFO;


OMX_ERRORTYPE omx_tunnel_init   (OMX_COMPONENTTYPE *pcomp, OMX_PTR pInitParam);
OMX_ERRORTYPE omx_tunnel_deinit (OMX_COMPONENTTYPE *pcomp);
OMX_ERRORTYPE omx_tunnel_enable (OMX_COMPONENTTYPE *pcomp);
OMX_ERRORTYPE omx_tunnel_disable(OMX_COMPONENTTYPE *pcomp);
OMX_ERRORTYPE omx_tunnel_start  (OMX_COMPONENTTYPE *pcomp);
OMX_ERRORTYPE omx_tunnel_stop   (OMX_COMPONENTTYPE *pcomp);
OMX_ERRORTYPE omx_tunnel_flush  (OMX_COMPONENTTYPE *pcomp);

#endif
