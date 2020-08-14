
#ifndef IMEDIA_CONFIG_H
#define IMEDIA_CONFIG_H

// 以下编译配置项为iMedia添加
#undef  restrict
#define restrict

#undef CONFIG_GPL
#define CONFIG_GPL 1

// decoder配置，所有编译平台通用
#undef CONFIG_MPEG1VIDEO_DECODER
#undef CONFIG_MPEG2VIDEO_DECODER
#undef CONFIG_MPEG4_DECODER
#undef CONFIG_MPEGVIDEO_DECODER
#undef CONFIG_H261_DECODER
#undef CONFIG_H263_DECODER
#undef CONFIG_H263I_DECODER
#undef CONFIG_H264_DECODER
#undef CONFIG_WMV1_DECODER
#undef CONFIG_WMV2_DECODER
#undef CONFIG_WMV3_DECODER
#undef CONFIG_VC1_DECODER
#undef CONFIG_VP3_DECODER
#undef CONFIG_VP5_DECODER
#undef CONFIG_VP6_DECODER
#undef CONFIG_VP6A_DECODER
#undef CONFIG_VP6F_DECODER

#define CONFIG_MPEG1VIDEO_DECODER 0
#define CONFIG_MPEG2VIDEO_DECODER 0
#define CONFIG_MPEG4_DECODER 0
#define CONFIG_MPEGVIDEO_DECODER 0
#define CONFIG_H261_DECODER 1
#define CONFIG_H263_DECODER 1
#define CONFIG_H263I_DECODER 1
#define CONFIG_H264_DECODER 1
#define CONFIG_WMV1_DECODER 0
#define CONFIG_WMV2_DECODER 0
#define CONFIG_WMV3_DECODER 0
#define CONFIG_VC1_DECODER 0
#define CONFIG_VP3_DECODER 0
#define CONFIG_VP5_DECODER 0
#define CONFIG_VP6_DECODER 0
#define CONFIG_VP6A_DECODER 0
#define CONFIG_VP6F_DECODER 0

#ifndef __KERNEL__
#undef HAVE_LLRINT
#undef HAVE_LRINT
#undef HAVE_LRINTF

#define HAVE_LLRINT 1
#define HAVE_LRINT 1
#define HAVE_LRINTF 1
#endif

#if defined(_MSC_VER)
#undef ARCH_X86
#undef ARCH_X86_32
#undef ARCH_X86_64

/* Define this if your system has pthreads */
#undef HAVE_THREADS

#define ARCH_X86 0
#define ARCH_X86_32 1
#define ARCH_X86_64 0

/* Define this if you enabled thread support for libavcodec */
#define HAVE_THREADS 1


#define lrintf(x) (int)(x)

// linux编译配置通用部分
#elif defined(__GNUC__)

#if defined(__KERNEL__)
//linux内核态定义
#include <linux/init.h>
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/limits.h>
#include <linux/string.h>
#include <linux/fcntl.h>
//#include <linux/math.h>
//#include <linux/delay.h>
//#include <linux/in.h>
//#include <linux/ioport.h>
//#include <linux/version.h>
#include "inttypes.h"
#include "math.h"
#include "limits2.h"

#define stdin  0
#define stdout 1
#define stderr 2
#define int_fast32_t int32_t
#define __STDC_VERSION__ 199901L
#undef  PRId64
#define PRId64 "lld"
/*
#include <asm/div64.h>

#define imedia_highprec_div(a1,a2)   do_div((a1),(a2))
#define imedia_highprec_mod(a1,a2)   do_mod((a1), (a2))

static inline int64_t do_mod(int64_t a1, int64_t a2)
{
	return do_div(a1,a2);
}*/

#else

//linux用户态定义
#define _alloca alloca
#undef HAVE_MEMALIGN
#undef HAVE_MKSTEMP
#undef HAVE_POSIX_MEMALIGN
#undef HAVE_MALLOC_H

#define HAVE_MEMALIGN 1
#define HAVE_MKSTEMP 1
#define HAVE_POSIX_MEMALIGN 1
#define HAVE_MALLOC_H 1
#endif

// linux编译配置 - X86平台差异部分
#ifdef BUILD_X86
#undef ARCH_X86
#undef ARCH_X86_32
#undef ARCH_X86_64
#undef HAVE_MMX
#undef HAVE_MMX2
#undef HAVE_SSE
#undef HAVE_SSSE3
#undef HAVE_AMD3DNOW
#undef HAVE_AMD3DNOWEXT
#undef HAVE_BSWAP
#undef HAVE_EBP_AVAILABLE
#undef HAVE_EBX_AVAILABLE
#undef HAVE_FAST_UNALIGNED
#undef HAVE_TEN_OPERANDS
#undef CONFIG_BZLIB
#undef HAVE_TEN_OPERANDS
#undef HAVE_YASM

#define ARCH_X86 1
#define ARCH_X86_32 1
#define ARCH_X86_64 0
#define HAVE_MMX 1
#define HAVE_MMX2 1
#define HAVE_SSE 1
#define HAVE_SSSE3 1
#define HAVE_AMD3DNOW 1
#define HAVE_AMD3DNOWEXT 1
#define HAVE_BSWAP 1
#define HAVE_EBP_AVAILABLE 1
#define HAVE_EBX_AVAILABLE 1
#define HAVE_FAST_UNALIGNED 1
#define HAVE_TEN_OPERANDS 1
#define CONFIG_BZLIB 1
#define HAVE_TEN_OPERANDS 1
#define HAVE_YASM 1
#endif

// linux编译配置 - ARM平台差异部分
#ifdef BUILD_ARM
#undef ARCH_ARM
#undef HAVE_ARMV5TE
#undef HAVE_ARMV6
#undef HAVE_ARMV6T2
#undef HAVE_ARMVFP
#undef HAVE_PLD
#undef CONFIG_MEMALIGN_HACK

#define ARCH_ARM 1
#define HAVE_ARMV5TE 0
#define HAVE_ARMV6 0
#define HAVE_ARMV6T2 0
#define HAVE_ARMVFP 1
#define HAVE_PLD 1
#define CONFIG_MEMALIGN_HACK 1

#define HAVE_IPP 0
#define ALT_BITSTREAM_READER
#endif

#endif //__GNUC__


#endif /* IMEDIA_CONFIG_H */
