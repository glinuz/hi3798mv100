/** \file vp_api_cfg.h
 * vp_api_cfg.h
 *
 * This file contains the configuration and compile time settings for
 * building appropriate VP-API library modules needed for any application.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7538 $
 * $LastChangedDate: 2010-07-22 14:36:22 -0500 (Thu, 22 Jul 2010) $
 */

#ifndef VP_API_CFG_H
#define VP_API_CFG_H

/******************************************************************************
 * COMPILE-TIME OPTIONS:: API DEBUG SPECIFICATIONS                            *
 *****************************************************************************/

/*
 * Define this to remove the I/F for unsupported functions. This reduces code
 * size with the only limitation that the application cannot call functions that
 * would return "Unsupported" for all devices used in the application. The
 * specific functions removed will depend on the device and test packages
 * compiled in.
 */
#undef  VP_REDUCED_API_IF

/******************************************************************************
 * COMPILE-TIME OPTIONS:: Conditionally-Compiled API Libraries                *
 *****************************************************************************/

/*
 * Define VP_DEBUG to include debug output that can be enabled/disabled at
 * runtime with the VP_OPTION_ID_DEBUG_SELECT option.  The various message
 * types can also be compiled in or out (see vp_debug.h).
 */
#undef  VP_DEBUG

#include "vp_debug_masks.h"

/*
 * If VP_DEBUG is #defined (debug strings compiled into the code) the VP_API-II
 * initializes to a default set of debug masks defined by the value of
 * VP_OPTION_DEFAULT_DEBUG_SELECT. Note that while the debug mask settings can
 * be modified at run-time (after VpInitDevice()), the only way to generate
 * debug output during VpInitDevice() operations is to set this mask accordingly.
 */
#define VP_OPTION_DEFAULT_DEBUG_SELECT  0

/*
 * Choose which types of messages to be compiled in.  Only these message
 * types will be selectable at runtime (with VP_OPTION_ID_DEBUG_SELECT):
 */
#define VP_CC_DEBUG_SELECT (VP_DBG_ALL)

/*
 * Define (or undefine) the appropriate compile time switches based on your
 * application needs.
 *
 * NOTE: Such library selection compile time option(s) MUST be defined before
 * including any other file from VoicePath library.
 *
 * NOTE: More than one Library modules can be built simultaneosuly (if needed).
 */

#ifndef ZARLINK_CFG_INTERNAL

  /*
   * Define the API libraries that will be included in this build
   * only if the kernel wrapper library is not supported.
   */

  #undef  VP_CC_790_SERIES    /**< define to build 790 specific API library;
                             *   undef to exclude this library. */

  #undef  VP_CC_792_SERIES    /**< define to build 792 specific API library;
                             *   undef to exclude this library. */

  #undef  VP_CC_880_SERIES   /**< define to build 880 specific API library;
                             *   undef to exclude this library. */

  #define VP_CC_890_SERIES    /**< define to build 890 specific API library;
                             *   undef to exclude this library. */

  #undef  VP_CC_580_SERIES    /**< define to build 580 specific API library;
                             *   undef to exclude this library. */

  /* VCP (VCP) library */
  #undef  VP_CC_VCP_SERIES    /**< define to build VCP specific API library;
                             *   undef to exclude this library. */
  /* VCP2 library */
  #undef  VP_CC_VCP2_SERIES    /**< define to build VCP2 specific API library;
                             *   undef to exclude this library. */

  #undef VP_CC_KWRAP

  #if defined(VP_CC_KWRAP) && defined(__KWRAP__)
    #error "VP_CC_KWWAP is only applicable to user space builds"
  #endif

#endif

/******************************************************************************
 * Include Files for the API                                                  *
 *****************************************************************************/
/* Include the API types for this architecture */
#include "vp_api_types.h"

/******************************************************************************
 * Undef the following to remove the code necessary to run the API-II sequencer
 * for CSLAC devices and to enable compile of API-II Lite. This is useful to
 * reduce code space if the host is not using API-II "advanced" features.
 *****************************************************************************/
#if defined(VP_CC_790_SERIES) || defined(VP_CC_880_SERIES) || \
    defined(VP_CC_890_SERIES) || defined(VP_CC_580_SERIES)
#define VP_CSLAC_SEQ_EN
#undef  EXTENDED_FLASH_HOOK
#endif

#if defined(VP_CC_880_SERIES) || defined(VP_CC_890_SERIES)
#define VP_CSLAC_RUNTIME_CAL_ENABLED
#endif

#if defined(VP_CC_KWRAP) || defined(VP_CC_792_SERIES)
#define EXTENDED_FLASH_HOOK
#endif
/******************************************************************************
 * Library Specific COMPILE-TIME OPTIONS and defines                          *
 *****************************************************************************/
#ifdef VP_CC_790_SERIES
/* CSLAC library can be configured to handle the interrupt in four different
 * configurations. Choose (only) one of the mechanisms by defining one of the
 * following. */
#define VP790_SIMPLE_POLLED_MODE        /* Define to use simple polled interrupt
                                         * handling mode */
#undef VP790_EFFICIENT_POLLED_MODE      /* Define to use efficient polled
                                         * interrupt handling mode */
#undef VP790_INTERRUPT_LEVTRIG_MODE     /* Define to use level triggered
                                         * interrupt handling mode */
#undef VP790_INTERRUPT_EDGETRIG_MODE    /* Define to use edge triggered
                                         * interrupt handling mode */
#endif /* VP_CC_790_SERIES */

#ifdef VP_CC_792_SERIES

#undef VP_COMMON_ADDRESS_SPACE         /* Define if all VP-API-II data
                                           structures (Line Objects, Line Contexts,
                                           Device Objects, Device Contexts, Profiles)
                                           are accessible at the same address in
                                           all processes (tasks) which call VP-API-II
                                           functions. */

#define VP_CC_792_GROUP                 /* Define to include support for sharing
                                         * an interrupt pin between multiple
                                         * VP792 devices. */

#define VP792_MAILBOX_SPINWAIT 50000    /* Number of times to poll the device's
                                           command mailbox before returning
                                           VP_STATUS_MAILBOX_BUSY. */

#define VP792_INCLUDE_TESTLINE_CODE     /* Defines whether or not to enable
                                         * 792 line test capabilities */

#define VP792_SUPPORT_792388_REV_E_SILICON /* Define this if you want to support
                                              Revision E of the Le792388 silicon. An
                                              appropriate SLAC firmware patch will
                                              be compiled in. */
#define VP792_SUPPORT_792388_REV_F_SILICON /* Define this if you want to support
                                              Revision F of the Le792388 silicon. An
                                              appropriate SLAC firmware patch will
                                              be compiled in. */
#define VP792_SUPPORT_792588_REV_A_SILICON /* Define this if you want to support
                                              Revision A of the Le792588 silicon. An
                                              appropriate SLAC firmware patch will
                                              be compiled in. */

#endif /* VP_CC_792_SERIES */


#ifdef VP_CC_880_SERIES

/*******************************************************************************
 * Applications can reduce VP-API-II driver size by changing the following for
 * only the required silicon and termination types needed
 *
 * See VP-API-II User's Guide for Details.
 ******************************************************************************/
/* Device Type Support. One of these MUST be defined if FXS Support is enabled. */
#define VP880_ABS_SUPPORT
#define VP880_TRACKER_SUPPORT

/*
 * Termination Type Support.
 *
 * Note: Both ABS and Tracker can be #undef while FXO defined. This results in
 * support for FXO only silicon, and FXO/FXS silicon support for the FXO
 * termination type only. FXS support requires either ABS or Tracker support.
 */
#define VP880_FXO_SUPPORT
#define VP880_FXS_SUPPORT

/*
 * Specific FXS termination Type. FXS termination type must be supported to
 * enable LPM support.
 */
#define VP880_LP_SUPPORT

/*
 * Specific FXO Algorithm support.
 *
 * The VP880_CLARE Ringing Detect algorithm uses features of the Clare and VE880
 * silicon to provide maximum performance of signal detection on FXO lines.
 * Alternative methods (with VP880_CLARE_RINGING_DETECT set to #undef) do not
 * require connection of I/O4 to Clare device, but have the following drawbacks:
 *
 *    1. Ringing Signals MUST contain polarity reversal to be detected AND
 *       must exceed ~70V peak.
 *    2. Line-In-Use is disabled.
 *    3. Frequency Discriminaation is poor (limited to tickrate).
 *
 * It is recommended that all designs connect the CLARE Ringing output to IO4
 * of the Zarlink silicon. In that case, this value should be #define.
 *
 * Note: If FXO Support is disabled, this setting has no affect.
 */
#define VP880_CLARE_RINGING_DETECT

/*
 * When using Clare Ringing Detection, #undef this when using CPC5621
 * otherwise set to #define.
 */
#define VP880_FXO_FULL_WAVE_RINGING

/*
 * Define this option to always use the internal test termination for relay
 * state VP_RELAY_BRIDGED_TEST, even for devices that have the test load switch
 */
#undef  VP880_ALWAYS_USE_INTERNAL_TEST_TERMINATION

/* CSLAC library can be configured to handle the interrupt in four different
 * configurations. Choose (only) one of the mechanisms by defining one of the
 * following. */
#define VP880_SIMPLE_POLLED_MODE        /* Define to use simple polled interrupt
                                         * handling mode */
#undef  VP880_EFFICIENT_POLLED_MODE     /* Define to use efficient polled
                                         * interrupt handling mode */
#undef  VP880_INTERRUPT_LEVTRIG_MODE    /* Define to use level triggered
                                         * interrupt handling mode */
#undef  VP880_INTERRUPT_EDGETRIG_MODE   /* Define to use edge triggered
                                         * interrupt handling mode */
#undef  VP880_INCLUDE_TESTLINE_CODE     /* Defines whether or not to enable
                                         * 880 line test capabilities */

/*
 * Defines all possible Vp880 test line packages
 */
#define VP880_LINE_TEST_AUDITOR 1
#define VP880_LINE_TEST_PROFESSIONAL 2

#ifdef VP880_INCLUDE_TESTLINE_CODE

/* Define supported line test pacakge */
#define VP880_INCLUDE_LINE_TEST_PACKAGE VP880_LINE_TEST_PROFESSIONAL

#define  VP880_EZ_MPI_PCM_COLLECT    /* Define to use api to collect
                                     * PCM samples via the MPI bus */

/* Define the maximum expected computation time to arrive at results for
 * VpTestLineCallback() after having collected the necessary PCM samples. */
#define VP880_PCM_CALCULATION_TIME 1000     /* time is in ms */

#endif /* VP880_INCLUDE_TESTLINE_CODE */

#endif /* VP_CC_880_SERIES */

#ifdef VP_CC_580_SERIES
/* CSLAC library can be configured to handle the interrupt in four different
 * configurations. Choose (only) one of the mechanisms by defining one of the
 * following. */
#define VP580_SIMPLE_POLLED_MODE        /* Define to use simple polled interrupt
                                         * handling mode */
#undef VP580_EFFICIENT_POLLED_MODE      /* Define to use efficient polled
                                         * interrupt handling mode */
#undef VP580_INTERRUPT_LEVTRIG_MODE     /* Define to use level triggered
                                         * interrupt handling mode */
#undef VP580_INTERRUPT_EDGETRIG_MODE    /* Define to use edge triggered
                                         * interrupt handling mode */
#endif

#ifdef VP_CC_890_SERIES

/* Generic Termination Type Support */
#define VP890_FXO_SUPPORT
#define VP890_FXS_SUPPORT

/*
 * Specific FXS termination Type. FXS termination type must be supported to
 * enable LPM support.
 */
#define VP890_LP_SUPPORT

#undef  VP890_INCLUDE_TESTLINE_CODE     /* Defines whether or not to enable
                                         * 890 line test capabilities */
#ifdef VP890_INCLUDE_TESTLINE_CODE

/* Define supported line test pacakge */
#define VP890_LINE_TEST_AUDITOR 1
#define VP890_LINE_TEST_PROFESSIONAL 2

#define VP890_INCLUDE_LINE_TEST_PACKAGE VP890_LINE_TEST_PROFESSIONAL

#define VP890_EZ_MPI_PCM_COLLECT            /* Define to use api to collect
                                             * PCM samples via the MPI bus */

/* Define the maximum expected computation time to arrive at results for
 * Vp890TestLineCallback() after having collected the necessary PCM samples. */
#define VP890_PCM_CALCULATION_TIME 1000     /* time is in ms */

#endif /* VP890_INCLUDE_TESTLINE_CODE */

#define VP890_SIMPLE_POLLED_MODE        /* Define to use simple polled interrupt
                                         * handling mode */
#undef  VP890_EFFICIENT_POLLED_MODE     /* Define to use efficient polled
                                         * interrupt handling mode */
#undef  VP890_INTERRUPT_LEVTRIG_MODE    /* Define to use Level Triggered
                                         * interrupt handling mode */
#undef  VP890_INTERRUPT_EDGETRIG_MODE   /* Define to use edge triggered
                                         * interrupt handling mode */
#endif /* VP_CC_890_SERIES*/

/*
 * The following lines include necessary compile time options/definitions that
 * are required for boot loading VCP/VCP2 devices.  By default, compression
 * is not supported for boot images.  If you would to use compressed boot
 * images, the compile time options in the following lines need to be modified.
 */

#if !defined(VP_CC_VCP_SERIES) && !defined(VP_CC_VCP2_SERIES) && !defined(VP_CC_KWRAP)
typedef uint8 VpScratchMemType;
#else
/* All the following definitions are necessary only for VCP/VCP2 devices */
/******************************************************************************
 * COMPILE-TIME OPTIONS:: Boot load compression method and size               *
 *****************************************************************************/
#define VP_CLEAR_CODE_MEM
    /*
     * After VpBootLoad(), a VP_LINE_EVID_BOOT_CMP event is generated, and
     * VpGetResults() is then used to retrieve the loadChkSum value, which is
     * a checksum calculated over the VCP's code memory.  VpCodeCheckSum()
     * can then be called at any later time to verify that VCP code memory
     * has not been corrupted.
     *
     * With VP_CLEAR_CODE_MEM defined, VpBootLoad() clears all VCP code memory
     * to zeroes before loading the boot image.
     * With VP_CLEAR_CODE_MEM disabled, unused VCP code memory is not
     * initialized after a hardware reset.  However, the loadChkSum is
     * calculated over the entire VCP code memory space, including uninitialized
     * parts.  If it is important to you that the loadChkSum value stay the
     * same after a hardware reset, then enable VP_CLEAR_CODE_MEM.
     *
     * This option adds a small amount of execution time to VpBootLoad(), when
     * state = VP_BOOT_STATE_FIRST or VP_BOOT_STATE_FIRSTLAST.
     */

/*
 * Options related to BootLoad() decompression
 */
#undef VP_NO_COMPRESS
    /* Define this option to exclude all decompression code from VpBootLoad().*/
#define VP_COMPRESS_FAST
    /*
     *  Define this to include additional speed-optimized code to increase the
     * speed of VpBootLoad().
     */

    /*
     *  On our test platform, code size is affected by the above options as
     * shown:
     *  VP_NO_COMPRESS      VP_COMPRESS_FAST        added code size
     *  --------------      ----------------        ---------------
     *  #define             (don't care)            0
     *  #undef              #undef                  9.6 kB
     *  #undef              #define                 12.2 kB
     */

    /*
     * The following options determine the size of VpScratchMemType, and the
     * speed of VpBootLoad() when using a compressed VCP image.
     */
#define VP_COMPRESS_WINDOW_BITS 14
    /*
     * There is no advantage in increasing this to a value greater than the
     * value used when the VCP image was compressed.  Setting it to less than
     * the value used in compression will cause VpBootLoad() to fail.  [The
     * number of window bits used in compression is indicated in the VCP
     * image's filename; for example, dvp.bin10z was compressed with 10 window
     * bits, etc.  If the number of bits is not indicated (*.binz), then
     * 8 bits were used.]  Higher values improve compression.  Don't
     * change this value unless you change the corresponding value used
     * in compressing the VCP image.  Adds (1 << VP_COMPRESS_WINDOW_BITS)
     * bytes to sizeof(VpScratchMemType).  This option can range from
     * 8 to 15.  */
#define VP_COMPRESS_OUTBUF_PAGES 50
    /*
     * Size of the buffer for holding data after it is decompressed, and before
     * it is sent to the HBI bus.  Must be greater than 0 and less than 512.
     * Larger values increase the speed of VpBootLoad().  If VP_COMPRESS_FAST
     * is defined, VP_COMPRESS_OUTBUF_PAGES should be at least 3.
     * Adds (128 * VP_COMPRESS_OUTBUF_PAGES) bytes to sizeof(VpScratchMemType).
     */

#ifndef VP_NO_COMPRESS
#include "zstructs.h"    /**< Components of VpScratchMemType */
typedef struct {
    struct VpZlibInflateStateType state;
    VpZlibStreamType stream;
    uint8 window[1 << VP_COMPRESS_WINDOW_BITS];
    uint8 inflated[VP_COMPRESS_OUTBUF_PAGES * 128];
} VpScratchMemType;
#else /* VP_NO_COMPRESS */
typedef uint8 VpScratchMemType;
#endif /* VP_NO_COMPRESS */
#endif  /* !VP_CC_VCP_SERIES */

/******************************************************************************
 *    VCP2 library specific other defines and conditional compile flags       *
 ******************************************************************************/
/*
 * Normally, when a VP-API function needs to write to the VCP2's command
 * mailbox, it checks to see if the mailbox is busy, and if so, returns
 * VP_STATUS_MAILBOX_BUSY.  The application can then wait for a
 * VP_DEV_EVID_DNSTR_MBOX ("mailbox free") event before retrying the VP-API
 * function call.
 *
 * Use the following option to include code which spin-waits for the VCP2 to
 * relinquish control of the command mailbox.  In this case, the VP-API
 * function will continuously poll the VCP2 device until the mailbox becomes
 * available.  The number of polling iterations is specified in the following
 * option.  If this option is 0, no spin-waiting occurs.  If this option is
 * nonzero, the VP-API functions will return VP_STATUS_MAILBOX_BUSY only if
 * unable to acquire control of the mailbox after the specified number of
 * polling iterations.
 *
 * Application programming may be simplified by defining this option to a
 * conservative value and treating VP_STATUS_MAILBOX_BUSY as an error.
 * However, this is at the expense of increased CPU utilization.
 */
#define VP_VCP2_MAILBOX_SPINWAIT 2000000UL

/* When bootloading a VCP2-792, the API uses a special bootloading process to
 * load the firmware for both the VCP2 and the 792 SLACs.  This process
 * spinwaits for the command mailbox between sending blocks of the SLAC image.
 * The spinwait limit here should be a higher number than the general limit, and
 * should not be zero because we actually expect to wait in this case, and a
 * timeout aborts the SLAC bootloading. */
#define VP_VCP2_BOOTLOAD_MAILBOX_SPINWAIT 20000000UL

/* When spinwaiting for the command mailbox using either of the above
 * parameters, the VP-API can exit and re-enter the critical section between
 * each specified number of reads of the mailbox flags.  Doing this should
 * allow other threads to interrupt while spinwaiting.  Set this value to 0
 * to disable the exit/enter critical behavior. */
#define VP_VCP2_SPINS_PER_CRITICAL_SEC 1

/* When spinwaiting for the command mailbox, it may be desirable to detect a
 * loss of communication with the VCP and exit the spinwait.  If this option
 * is enabled, the VP-API will perform a write-read HBI check along with
 * reading the mailbox status flags.  The function will return status code
 * VP_STATUS_ERR_HBI if the check fails. */
#undef VP_VCP2_MAILBOX_ACQUIRE_HBI_CHECK

/*
 * Define the following option to include some extra error checking in the
 * VCP2 boot code.  This is most useful to diagnose board problems during
 * initial board bring-up.
 */
#define VP_VCP2_BOARD_BOOT_DEBUG

/*
 * Set this option to reflect the CLKSRC pin value.  Possible values:
 *   1 => crystal oscillator
 *   0 => PCLK (worst case value)
 */
#define VP_VCP2_BOARD_CLKSRC 0

/*
 * Set these options to initialize the PCLKA_SEL and PCLKB_SEL registers.  See
 * the Le79114 data sheet for details.  A safe value is 0xC000.  These options
 * are only relevant if VCP2_BOARD_CLKSRC = 0.
 */
#define VP_VCP2_BOARD_PCLKA_SEL 0xC000
#define VP_VCP2_BOARD_PCLKB_SEL 0xC000

/******************************************************************************
 *     VCP library specific other defines and conditional compile flags       *
 ******************************************************************************/

/* Define the following to wait for certain amount of time for VCP mailbox
 * to become available. By enabling this conditional compile time option,
 * application would rarely have to handle the VP_STATUS_MAILBOX_BUSY error
 * (assuimg configured properly; more later). Basically when this conditional
 * compile option is defined, API repeats checking for mailbox ownership (if
 * it already does not own). This checking continues until either mailbox
 * ownership is acquired or maximum specified number of repetitions happen (in
 * which case API throws an error).
 * If this compile flag is undefined, API checks for mailbox only once.
 */
#define WAIT_TO_ACQUIRE_VCP_MB

/* Define the number iterations host should wait for mailbox owenership */
#define ITERATIONS_TO_WAIT_FOR_VCP_MB      (100000)

/* Include internal options required to build the VP-API-II library */
#include "vp_api_cfg_int.h"

#endif /* VP_API_CFG_H */

