/*==========================================================================*/
/*     (Copyright (C) 2003 Koninklijke Philips Electronics N.V.             */
/*     All rights reserved.                                                 */
/*     This source code and any compilation or derivative thereof is the    */
/*     proprietary information of Koninklijke Philips Electronics N.V.      */
/*     and is confidential in nature.                                       */
/*     Under no circumstances is this software to be exposed to or placed   */
/*     under an Open Source License of any type without the expressed       */
/*     written permission of Koninklijke Philips Electronics N.V.           */
/*==========================================================================*/
/*-----------------------------------------------------------------------------
// MoReUse - 2001-11-12  Continuus Version 16
//
// Added
//   CID_COMP_TRICK
//   CID_COMP_TODISK
//   CID_COMP_FROMDISK
// 
// Removed
// CID_COMP_RTC  Twice the same request - duplicate removed          
//
// (C) Copyright 2000-2001 Koninklijke Philips Electronics N.V.,
//     All rights reserved
//
// This source code and any compilation or derivative thereof is the sole
// property of Philips Corporation and is provided pursuant to a Software
// License Agreement.  This code is the proprietary information of Philips
// Corporation and is confidential in nature.  Its use and dissemination by
// any party other than Philips Corporation is strictly limited by the
// confidential information provisions of the Agreement referenced above.
*/
/*-----------------------------------------------------------------------------
// FILE NAME:    tmCompId.h
//
// DESCRIPTION:  This header file identifies the standard component identifiers
//               for DVP platforms.  The objective of DVP component IDs is to
//               enable unique identification of software components at all
//               classes, types, and layers.  In addition, standard status
//               values are also defined to make determination of typical error
//               cases much easier.  The component identifier bitfields are
//               four bit aligned to ease in reading the hexadecimal value.
//
//               The process to create a new component ID follows the sequence
//               of steps:
//
//               1) Select a component class:  The class is the most general
//                  component classification.  If none of the classifications
//                  match and the component can still be uniquely identified
//                  by its type/tag/layer combination, use CID_CLASS_NONE.
//                  For customers, the CID_CLASS_CUSTOMER value is defined.
//                  If that value is used in the CID_CLASS_BITMASK field,
//                  all other bits in the component ID are user defined.
//
//               2) Select a component type:   The component type is used to
//                  classify how a component processes data.  Components may
//                  have only output pins (source), only input pins (sink),
//                  input and output pins with or without data modifications
//                  (filter), control of data flow without input/output pins
//                  (control), data storage/access/retrieval (database),
//                  or component group (subsystem).  If the component does
//                  not fit into any type classification, use CID_TYPE_NONE.
//
//               3) Create a component ID:     The component ID is used to
//                  classify the specific type and/or attributes of a software
//                  component API interface.  The currently defined list should
//                  be scanned for a component match.  If no component match
//                  can be found, define a new component tag that descibes the
//                  component clearly.  Component name abbreviations/acronyms
//                  are generally used; build a name starting from left to
//                  right with the most general ('A' or 'AUD' for audio, 'V' or
//                  'VID' for video, etc.) to the most specific ('AC3' or 'MP3'
//                  as specific audio filter types) terms for the component.
//
//               NOTE: Component layer (CID_LAYER_XXX) and status (CID_ERR_XXX)
//                     values are defined within the software component APIs
//                     header files, not in this file.
//
// DOCUMENT REF: DVP Software Coding Guidelines specification
//               DVP/MoReUse Naming Conventions specification
//
// NOTES:        The 32 bit component identifier bitfields are defined in the
//               diagram below:
//
//           +-----------------------------------------  4 bit Component Class
//          /      +-----------------------------------  4 bit Component Type
//         /      /         +--------------------------  8 bit Component Tag
//        /      /         /         +-----------------  4 bit Component Layer
//       /      /         /         /            +----- 12 bit Component Status
//      /      /         /         /            /
//  |31  28|27  24|23        16|15  12|11               0| bit
//  +------+------+------------+------+------------------+
//  |Class | Type |ComponentTag|Layer |  Error/Progress  |
//  +------+------+------------+------+------------------+
//
//-----------------------------------------------------------------------------
*/
#ifndef TM_COMP_ID_H
#define TM_COMP_ID_H

#ifdef __cplusplus
extern "C"
{
#endif

/*-----------------------------------------------------------------------------
// Types and defines:
//-----------------------------------------------------------------------------
*/

/*-----------------------------------------------------------------------------
// TM_OK is the 32 bit global status value used by all DVP software components
//  to indicate successful function/operation status.  If a non-zero value is
//  returned as status, it should use the component ID formats defined.
*/
#define TM_OK               0         /* Global success return status */
#define TM_FAIL				 1

/*
// NOTE: Component ID types are defined as unsigned 32 bit integers (UInt32).
//
//-----------------------------------------------------------------------------
// Component Class definitions (bits 31:28, 4 bits)
// NOTE: A class of 0x0 must not be defined to ensure that the overall 32 bit
//       component ID/status combination is always non-0 (no TM_OK conflict).
*/
#define CID_CLASS_BITSHIFT  28                          /* Component class bit shift */
#define CID_CLASS_BITMASK   (0xF << CID_CLASS_BITSHIFT) /* Class AND bitmsk */
#define CID_GET_CLASS(compId)   ((compId & CID_CLASS_BITMASK) >> CID_CLASS_BITSHIFT)
#define CID_CLASS_NONE      (0x1 << CID_CLASS_BITSHIFT) /* No class information */

/*-----------------------------------------------------------------------------
// Component Type definitions (bits 27:24, 4 bits)
*/
#define CID_TYPE_BITSHIFT   24                          /* Component type bit shift */
#define CID_TYPE_BITMASK    (0xF << CID_TYPE_BITSHIFT)  /* Type AND bitmask */
#define CID_GET_TYPE(compId)    ((compId & CID_TYPE_BITMASK) >> CID_TYPE_BITSHIFT)
#define CID_TYPE_NONE       (0x0 << CID_TYPE_BITSHIFT)  /* No data connections */

/*-----------------------------------------------------------------------------
// Component Tag definitions (bits 23:16, 8 bits)
// NOTE: Component tags are defined in groups, dependent on the class and type.
*/
#define CID_TAG_BITSHIFT    16                          /* Component tag bit shift */
#define CID_TAG_BITMASK     (0xFF << CID_TAG_BITSHIFT)  /* Comp tag AND bitmask */
#define TAG(number)         ((number) << CID_TAG_BITSHIFT) /* Create tag from num */

/*-----------------------------------------------------------------------------
// Component Layer definitions (bits 15:12, 4 bits)
*/
#define CID_LAYER_BITSHIFT  12                           /* Component layer bit shift */
#define CID_LAYER_BITMASK   (0xF << CID_LAYER_BITSHIFT)  /* Layer AND bitmask */
#define CID_GET_LAYER(compId)   ((compId & CID_LAYER_BITMASK) >> CID_LAYER_BITSHIFT)

#define CID_LAYER_NONE      (0x0 << CID_LAYER_BITSHIFT)  /* No layer info */
#define CID_LAYER_BTM       (0x1 << CID_LAYER_BITSHIFT)  /* Boot manager layer */
#define CID_LAYER_HWAPI     (0x2 << CID_LAYER_BITSHIFT)  /* Hardware API layer */
#define CID_LAYER_BSL       (0x3 << CID_LAYER_BITSHIFT)  /* Board Supp. Lib lyr */
#define CID_LAYER_DEVLIB    (0x4 << CID_LAYER_BITSHIFT)  /* Device Library lyr */
#define CID_LAYER_TMAL      (0x5 << CID_LAYER_BITSHIFT)  /* Application layer */
#define CID_LAYER_TMOL      (0x6 << CID_LAYER_BITSHIFT)  /* OSDependent layer */
#define CID_LAYER_CUSTOMER  (0xF << CID_LAYER_BITSHIFT)  /* Customer Defined Layer */
                            /* Up to 0xF = Philips reserved layer IDs */

/*-----------------------------------------------------------------------------
// Component Identifier definitions (bits 31:12, 20 bits)
// NOTE: These DVP platform component identifiers are designed to be unique
//       within the system.  The component identifier encompasses the class
//       (CID_CLASS_XXX), type (CID_TYPE_XXX), tag, and layer (CID_LAYER_XXX)
//       fields to form the unique component identifier.  This allows any
//       error/progress status value to be identified as to its original
//       source, whether or not the source component's header file is present.
//       The standard error/progress status definitions should be used
//       whenever possible to ease status interpretation.  No layer
//       information is defined at this point; it should be OR'd into the API
//       status values defined in the API's header file.
*/
#if     (CID_LAYER_NONE != 0)
#error  ERROR: DVP component identifiers require the layer type 'NONE' = 0 !
#endif

/*
// Classless Types/Components (don't fit into other class categories)
*/
#define CTYP_NOCLASS_NOTYPE     (CID_CLASS_NONE | CID_TYPE_NONE)

#define CID_COMP_TIMER          (TAG(0x0A) | CTYP_NOCLASS_NOTYPE)
#define CID_COMP_PARPORT        (TAG(0x0E) | CTYP_NOCLASS_NOTYPE)
#define CID_COMP_TUNER          (TAG(0x1E) | CTYP_NOCLASS_NOTYPE)
#define CID_COMP_DEMOD          (TAG(0x1F) | CTYP_NOCLASS_NOTYPE)


/*-----------------------------------------------------------------------------
// Component Standard Error/Progress Status definitions (bits 11:0, 12 bits)
// NOTE: These status codes are OR'd with the component identifier to create
//       component unique 32 bit status values.  The component status values
//       should be defined in the header files where the APIs are defined.
*/
#define CID_ERR_BITMASK                 0xFFF /* Component error AND bitmask */
#define CID_ERR_BITSHIFT                0     /* Component error bit shift */
#define CID_GET_ERROR(compId)   ((compId & CID_ERR_BITMASK) >> CID_ERR_BITSHIFT)

#define TM_ERR_COMPATIBILITY            0x001 /* SW Interface compatibility */
#define TM_ERR_MAJOR_VERSION            0x002 /* SW Major Version error */
#define TM_ERR_COMP_VERSION             0x003 /* SW component version error */
#define TM_ERR_BAD_MODULE_ID            0x004 /* SW - HW module ID error */
#define TM_ERR_BAD_UNIT_NUMBER          0x005 /* Invalid device unit number */
#define TM_ERR_BAD_INSTANCE             0x006 /* Bad input instance value */
#define TM_ERR_BAD_HANDLE               0x007 /* Bad input handle */
#define TM_ERR_BAD_INDEX                0x008 /* Bad input index */
#define TM_ERR_BAD_PARAMETER            0x009 /* Invalid input parameter */
#define TM_ERR_NO_INSTANCES             0x00A /* No instances available */
#define TM_ERR_NO_COMPONENT             0x00B /* Component is not present */
#define TM_ERR_NO_RESOURCES             0x00C /* Resource is not available */
#define TM_ERR_INSTANCE_IN_USE          0x00D /* Instance is already in use */
#define TM_ERR_RESOURCE_OWNED           0x00E /* Resource is already in use */
#define TM_ERR_RESOURCE_NOT_OWNED       0x00F /* Caller does not own resource */
#define TM_ERR_INCONSISTENT_PARAMS      0x010 /* Inconsistent input params */
#define TM_ERR_NOT_INITIALIZED          0x011 /* Component is not initialized */
#define TM_ERR_NOT_ENABLED              0x012 /* Component is not enabled */
#define TM_ERR_NOT_SUPPORTED            0x013 /* Function is not supported */
#define TM_ERR_INIT_FAILED              0x014 /* Initialization failed */
#define TM_ERR_BUSY                     0x015 /* Component is busy */
#define TM_ERR_NOT_BUSY                 0x016 /* Component is not busy */
#define TM_ERR_READ                     0x017 /* Read error */
#define TM_ERR_WRITE                    0x018 /* Write error */
#define TM_ERR_ERASE                    0x019 /* Erase error */
#define TM_ERR_LOCK                     0x01A /* Lock error */
#define TM_ERR_UNLOCK                   0x01B /* Unlock error */
#define TM_ERR_OUT_OF_MEMORY            0x01C /* Memory allocation failed */
#define TM_ERR_BAD_VIRT_ADDRESS         0x01D /* Bad virtual address */
#define TM_ERR_BAD_PHYS_ADDRESS         0x01E /* Bad physical address */
#define TM_ERR_TIMEOUT                  0x01F /* Timeout error */
#define TM_ERR_OVERFLOW                 0x020 /* Data overflow/overrun error */
#define TM_ERR_FULL                     0x021 /* Queue (etc.) is full */
#define TM_ERR_EMPTY                    0x022 /* Queue (etc.) is empty */
#define TM_ERR_NOT_STARTED              0x023 /* Component stream not started */
#define TM_ERR_ALREADY_STARTED          0x024 /* Comp. stream already started */
#define TM_ERR_NOT_STOPPED              0x025 /* Component stream not stopped */
#define TM_ERR_ALREADY_STOPPED          0x026 /* Comp. stream already stopped */
#define TM_ERR_ALREADY_SETUP            0x027 /* Component already setup */
#define TM_ERR_NULL_PARAMETER           0x028 /* Null input parameter */
#define TM_ERR_NULL_DATAINFUNC          0x029 /* Null data input function */
#define TM_ERR_NULL_DATAOUTFUNC         0x02A /* Null data output function */
#define TM_ERR_NULL_CONTROLFUNC         0x02B /* Null control function */
#define TM_ERR_NULL_COMPLETIONFUNC      0x02C /* Null completion function */
#define TM_ERR_NULL_PROGRESSFUNC        0x02D /* Null progress function */
#define TM_ERR_NULL_ERRORFUNC           0x02E /* Null error handler function */
#define TM_ERR_NULL_MEMALLOCFUNC        0x02F /* Null memory alloc function */
#define TM_ERR_NULL_MEMFREEFUNC         0x030 /* Null memory free  function */
#define TM_ERR_NULL_CONFIGFUNC          0x031 /* Null configuration function */
#define TM_ERR_NULL_PARENT              0x032 /* Null parent data */
#define TM_ERR_NULL_IODESC              0x033 /* Null in/out descriptor */
#define TM_ERR_NULL_CTRLDESC            0x034 /* Null control descriptor */
#define TM_ERR_UNSUPPORTED_DATACLASS    0x035 /* Unsupported data class */
#define TM_ERR_UNSUPPORTED_DATATYPE     0x036 /* Unsupported data type */
#define TM_ERR_UNSUPPORTED_DATASUBTYPE  0x037 /* Unsupported data subtype */
#define TM_ERR_FORMAT                   0x038 /* Invalid/unsupported format */
#define TM_ERR_INPUT_DESC_FLAGS         0x039 /* Bad input  descriptor flags */
#define TM_ERR_OUTPUT_DESC_FLAGS        0x03A /* Bad output descriptor flags */
#define TM_ERR_CAP_REQUIRED             0x03B /* Capabilities required ??? */
#define TM_ERR_BAD_TMALFUNC_TABLE       0x03C /* Bad TMAL function table */
#define TM_ERR_INVALID_CHANNEL_ID       0x03D /* Invalid channel identifier */
#define TM_ERR_INVALID_COMMAND          0x03E /* Invalid command/request */
#define TM_ERR_STREAM_MODE_CONFUSION    0x03F /* Stream mode config conflict */
#define TM_ERR_UNDERRUN                 0x040 /* Data underflow/underrun */
#define TM_ERR_EMPTY_PACKET_RECVD       0x041 /* Empty data packet received */
#define TM_ERR_OTHER_DATAINOUT_ERR      0x042 /* Other data input/output err */
#define TM_ERR_STOP_REQUESTED           0x043 /* Stop in progress */
#define TM_ERR_PIN_NOT_STARTED          0x044 /* Pin not started */
#define TM_ERR_PIN_ALREADY_STARTED      0x045 /* Pin already started */
#define TM_ERR_PIN_NOT_STOPPED          0x046 /* Pin not stopped */
#define TM_ERR_PIN_ALREADY_STOPPED      0x047 /* Pin already stopped */
#define TM_ERR_STOP_PIN_REQUESTED       0x048 /* Stop of a single pin is in progress (obsolescent) */
#define TM_ERR_PAUSE_PIN_REQUESTED      0x048 /* Stop of a single pin is in progress */
#define TM_ERR_ASSERTION                0x049 /* Assertion failure */
#define TM_ERR_HIGHWAY_BANDWIDTH        0x04A /* Highway bandwidth bus error */
#define TM_ERR_HW_RESET_FAILED          0x04B /* Hardware reset failed */
#define TM_ERR_PIN_PAUSED               0x04C /* Pin Paused */

/* Add new standard error/progress status codes here */

#define TM_ERR_COMP_UNIQUE_START        0x800 /* 0x800-0xDFF: Component unique */

#define TM_ERR_CUSTOMER_START           0xC00  

/* DVP Standard assert error code start offset
// NOTE: This define should be added to the component's base error value and
//       standard error code(s) to define assert error codes.  For example:
// #define TMBSL_ERR_MGR_ASSERT_BAD_PARAM 
//          (TMBSL_ERR_MGR_BASE + TM_ERR_ASSERT_START + TM_ERR_BAD_PARAMETER)
*/
#define TM_ERR_ASSERT_START             0xE00 /* 0xE00-0xEFF: Assert failures */
#define TM_ERR_ASSERT_LAST              0xEFF /* Last assert error range value */
#define CID_IS_ASSERT_ERROR(compId)     \
    ((CID_GET_ERROR(compId) >= TM_ERR_ASSERT_START) && \
     (CID_GET_ERROR(compId) <= TM_ERR_ASSERT_LAST))

/* DVP Standard fatal error code start offset
// NOTE: This define should be added to the component's base error value and
//       standard error code(s) to define fatal error codes.  For example:
// #define TMML_ERR_FATAL_OUT_OF_MEMORY  
//          (TMML_ERR_BASE + TM_ERR_FATAL_START + TM_ERR_OUT_OF_MEMORY)
*/
#define TM_ERR_FATAL_START              0xF00 /* 0xF00-0xFFF: Fatal failures */
#define TM_ERR_FATAL_LAST               0xFFF /* Last fatal error range value */
#define CID_IS_FATAL_ERROR(compId)      \
    ((CID_GET_ERROR(compId) >= TM_ERR_FATAL_START) && \
     (CID_GET_ERROR(compId) <= TM_ERR_FATAL_LAST))


#ifdef __cplusplus
}
#endif

#endif /* TM_COMP_ID_H */

