#ifndef __GST_OMX_1_2_0_H__
#define __GST_OMX_1_2_0_H__

#include <OMX_Core.h>
#include <OMX_Component.h>

#define OMX_VERSION_1_2_0 ((0<<24) | (0<<16) | (2<<8) | 1)

/**< Currently gst-omx only supports openmax IL 1.1.2, need to define some
 *   MACROs to match the definitions of two versions.
 */
#if (OMX_VERSION == OMX_VERSION_1_2_0)
#define OMX_StateInvalid OMX_StateReserved_0x00000000
#define OMX_ErrorInvalidComponent OMX_ErrorReserved_0x80001004
#define OMX_ErrorInvalidState OMX_ErrorReserved_0x8000100A
#define OMX_ErrorPortUnresponsiveDuringAllocation OMX_ErrorReserved_0x80001014
#define OMX_ErrorPortUnresponsiveDuringDeallocation OMX_ErrorReserved_0x80001015
#define OMX_ErrorPortUnresponsiveDuringStop OMX_ErrorReserved_0x80001016
#define OMX_ErrorContentPipeOpenFailed OMX_ErrorReserved_0x80001021
#define OMX_ErrorContentPipeCreationFailed OMX_ErrorReserved_0x80001022

#endif

#endif
