/**
**  @file bsd_test_cmd.c
**
**  @brief
**    Bare Security Driver Test Application
**
**  @defgroup
**    BSDTA
**
*/

/**
 * @mainpage Overview
 *  - @subpage p_preface
 *  - @subpage p_history
 *
 *  <hr> <b>Note:\n</b>
 *  Depending on the platform, the CHM file may not open properly if it is stored on a network drive.
 *  So either the file should be moved on a local drive or add the following registry entry on
 *  Windows platform (regedit):\n
 *  [HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\HTMLHelp\1.x\ItssRestrictions] "MaxAllowedZone"=dword:00000003
 *
 *
 *  <hr>Copyright &copy; 2012 Nagravision. All rights reserved.\n
 *  CH-1033 Cheseaux, Switzerland\n
 *  Tel: +41 21 7320311  Fax: +41 21 7320100\n
 *  http://www.nagra.com
 *
 *  All trademarks and registered trademarks are the property of their respective
 *  owners.
 *
 *  This document is supplied with an understanding that the notice(s) herein or
 *  any other contractual agreement(s) made that instigated the delivery of a
 *  hard copy, electronic copy, facsimile or file transfer of this document are
 *  strictly observed and maintained.
 *
 *  The information contained in this document is subject to change without notice.
 *
 *  <b>Security Policy of Nagravision Kudelski Group</b>\n
 *  Any recipient of this document, without exception, is subject to a
 *  Non-Disclosure Agreement (NDA) and access authorization.
 *
 */

/** @page p_preface Preface
 *
 *  <h2>Purpose</h2>
 *  This document is intended for the manufacturers of MPEG secure chipsets who
 *  are partners of Nagravision and whose chipset implements security features as
 *  required  by NOCS requirement specification. \n
 *  This document specifies the Test Harness for the BSD API \n
 *  It is assumed that the reader is familiar with the NOCS concepts, requirements
 *  and recommendations.\n
 *  This reference code can be used by the by the chipset manufacturers for any
 *  new NOCS secure chipset that is candidate to a Nagra NOCS certification.
 *
 *  <hr><h2>Audience</h2>
 *  This document is intended for MPEG secure chipset manufacturers implementing NOCS
 *  requirements and candidate to a NOCS certification.
 *
 *  <hr><h2>References</h2>
 *  [EMI] Nagravision, System Architecture, Encryption Method Indicator Specification,
 *         Version 1.0.7
 *
 */

/*  @author
**    NOCS team
**
**  @version
**    Revision: 2.0.0
**
** COPYRIGHT:
**   2012 NagraVision S.A.
**


/** @page p_history History
*
*   <b>Revision 1.0.0 2011/11/07 11:30:00 Nocs team<b>
*   - BSD TA first Release
*
*   <b>Revision 1.1.0 2012/02/27 11:30:00 Nocs team</b>
*   - Removed the following tests fo RSA functions bsdRsaPrivateEncrypt(),
*     bsdRsaPrivateDecrypt() and bsdRsaGenerateKey().
*   - Removed the bsdEncryptDataWithClearTextFlashProtectionKey() function
*   - Removed the bsdGetNUID64CheckNumber() function
*   - Added bsdGetNuid() and bsdGetNUIDCheckNumber().
*   - Added bsdSetSecretContentKey().
*
*   <b>Revision 1.2.0 2012/05/08 16:30:00 Prasad Somwanshi</b>
*   - Made the changes as per the latest modifications in the BSD API ,
*     version 3.2.1
*
*   <b>Revision 1.3.0 2012/05/30 10:30:00 Prasad Somwanshi</b>
*   - Made the changes as per the latest modifications in the BSD API ,
*     version 3.2.3
*   - Fixed the compilation errors
*   - Added support for CERT.
*
*   <b>Revision 2.0.0 2012/08/13 11:22:19 Prasad Somwanshi</b>
*   - Made the changes as per the latest modifications in the BSD API ,
*     version 3.3.0
*   - Made the BSD application compatible for both NOCS1x and NOCS3x
*
*
*/

/*
** REMARK:
**    Comments in this file use special tags to allow automatic API
**    documentation generation in HTML format, using the
**    GNU-General Public Licensed Doxygen tool.
**    For more information about Doxygen, please check www.doxygen.org
*/


//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stddef.h>
#include "nocs_bsd.h"
#include "nocs_bsd_impl.h"
#include "hi_type.h"
#ifndef HI_MINIBOOT_SUPPORT
#include "common.h"
#endif
#include "hi_flash.h"

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define HI_ERR_NAGRA(format, arg...)
#define HI_DEBUG_NAGRA(format...)
#else
#define HI_ERR_NAGRA(format, arg...) printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_DEBUG_NAGRA(format...)    printf(format)
#endif

#ifdef HI_MINIBOOT_SUPPORT
#define U_BOOT_CMD(name,maxargs,rep,cmd,usage,help)
#endif


/*TODO: Update the header file name */

#define NOCS1x 1 /*TODO: If NOCS1.x certification set to '1' otherwise set to '0' */
#define NOCS3x 0 /*TODO: If NOCS3.x certification set to '1' otherwise set to '0' */

#define SELECTED_FLASH_TYPE NAND /* NOR Parallel, Serial NOR (SPI) - used for
																		NOCS1x Unique Data Ref */
#define UNIQUE_DATA_REF_OFFSET 0x1800 /*0x1000 is an example value*/
#define BOOTAREA_SIZE 0x200000

#define BSD_MAX_CMDLINE_LEN 1280
#define BSD_MAX_ARGS_COUNT 10
#define BSD_MAX_ARGS_LEN 128

#if NOCS1x	
typedef TUnsignedInt8 TBsdUnsignedInt8;
#include "hi3716cv2_0_Test_Vectors_1_x.h"
#endif

#if NOCS3x
#include "ca_cert.h"

#include "test_sec_header.h"
#include "cert_testvectors.h"
#include "chipsetnamecut_nocs3x_bsd_testvectors.h"
#endif

#define BSD_DEBUG_Print(x) printf x
#define BSD_TA_Print(x) printf x

#define CHECK_ERROR(Function, Error) \
    if (Error != BSD_NO_ERROR) \
    {  \
        printf("*** ERROR: function '%s' returned 0x%08x\n", # Function, Error); \
        return Error; \
    }

#define CHECK_ERROR_CODE(Error) \
    if (Error != BSD_NO_ERROR)                                           \
    {                                                                    \
        BSD_TA_Print(("***ERROR: BSD Error code (%08x)\n", Error)); \
        return Error;                                                      \
    }

TBsdStatus			bsdTestAppMenu(void);
char				bsdPrintMenu(void);

TBsdStatus			bsd_reset(void);
TBsdStatus			bsd_chip_information(void);
TBsdStatus			bsd_ram2ram(void);
TBsdStatus			bsd_flash(void);
TBsdStatus			bsd_cert(void);
TBsdStatus			bsd_rsa(void);
TBsdStatus			bsd_digest(void);
TBsdStatus			bsd_pv(void);
TBsdStatus			bsd_rng(void);

#if NOCS3x
void				certGetNuid(TUnsignedInt32 *xpNuid);
TBsdStatus			cert_ram2ram_op(unsigned long int intNuid, TUnsignedInt16 xEmi);
TBsdStatus			cert_activate(TUnsignedInt32 intNuid);
TBsdStatus			cert_genkey();
TCertFunctionTable* certGetFunctionTable();
#endif

unsigned long int	Convert4BytesArrayToInt32(unsigned char *pBytes );
void				ConvertInt32To4BytesArray( unsigned long int val, unsigned char *pBytes );

TBsdStatus			bsd_ram2ram_op(unsigned long int intNuid, TUnsignedInt16 xEmi);
TBsdStatus			bsd_rsa_op(TBsdRsaPadding xPadding);
TBsdStatus			bsd_flash_op(TUnsignedInt16 xEmi);
TBsdStatus			bsd_flash_rootkey_op(void);

#if NOCS1x
TBsdStatus			bsd_unique_data_ref_op(unsigned int intNuid);
#elif NOCS3x
TBsdStatus			bsd_unique_data_ref_op(unsigned long int intNuid);
#endif

static TUnsignedInt8 UniqueDataRef[1024] =
{0x9b, 0x10, 0x36, 0x1f, 0xe4, 0x7f, 0x11, 0x42, 0x0c, 0xc6, 0x53, 0x6b, 0x67, 0x6e, 0x08, 0x21,
 0x36, 0x98, 0x9c, 0x37, 0x2b, 0xc5, 0x6f, 0x24, 0x76, 0x25, 0x4e, 0x30, 0x0c, 0x50, 0xb0, 0xbb,
 0x55, 0x7f, 0x19, 0x58, 0x23, 0x7b, 0x00, 0x40, 0x0c, 0x09, 0xbb, 0x5b, 0x22, 0x87, 0xc4, 0x28,
 0x42, 0x5f, 0xa4, 0x71, 0xc1, 0xa6, 0xc9, 0x7c, 0x07, 0x16, 0xd7, 0x3e, 0xcb, 0x1f, 0xb0, 0x8d,
 0x16, 0x51, 0x9f, 0x6f, 0x7c, 0x55, 0x60, 0x9b, 0x44, 0x7a, 0xe4, 0xea, 0xeb, 0x8b, 0x13, 0xdd,
 0x42, 0xef, 0x1a, 0x17, 0xc2, 0x2d, 0x89, 0xbf, 0x27, 0xf2, 0xe9, 0x6f, 0x8f, 0x72, 0x76, 0x1b,
 0x92, 0x8d, 0x8d, 0xe3, 0x97, 0x90, 0x5f, 0xd3, 0x36, 0xd5, 0x1f, 0xbd, 0x3e, 0xc9, 0x12, 0xbf,
 0x9d, 0x78, 0x4b, 0x92, 0xf9, 0x76, 0x6e, 0x51, 0xe1, 0x02, 0x8e, 0xcb, 0x09, 0xfc, 0xa0, 0x63,
 0x1e, 0xce, 0x0f, 0x5f, 0x68, 0xd3, 0x6b, 0xb8, 0x65, 0x6b, 0x2f, 0x35, 0xc7, 0xce, 0x43, 0x76,
 0xe1, 0x3e, 0xa7, 0x38, 0x58, 0x77, 0x91, 0x20, 0x5b, 0x7c, 0x51, 0x7e, 0x08, 0x14, 0xe2, 0x7d,
 0xd4, 0xf1, 0xf7, 0x10, 0xca, 0xc7, 0x73, 0x85, 0x52, 0x05, 0x9b, 0xf0, 0x9f, 0x42, 0x39, 0x0f,
 0xcb, 0xe0, 0xf1, 0x66, 0x89, 0x52, 0xc0, 0x9e, 0x3a, 0x24, 0xb3, 0x1b, 0xd7, 0x3f, 0x13, 0xa1,
 0x81, 0x01, 0xfb, 0x1a, 0x99, 0xdd, 0x01, 0xcf, 0xed, 0x0a, 0x01, 0x35, 0x7c, 0x15, 0x0a, 0xb3,
 0x88, 0x69, 0x44, 0x10, 0x88, 0xc0, 0x4f, 0x77, 0x02, 0xc1, 0xbe, 0x08, 0x40, 0x33, 0x86, 0x23,
 0x67, 0xa4, 0xd2, 0x22, 0xef, 0xc8, 0x14, 0x70, 0x44, 0x4f, 0x12, 0x6d, 0x3c, 0x17, 0xc8, 0x22,
 0xb0, 0x0c, 0x71, 0xb1, 0xcd, 0x8c, 0xe0, 0x24, 0x66, 0x26, 0x4e, 0x0d, 0xfb, 0x55, 0x71, 0xc9,
 0x4e, 0x2b, 0x9a, 0x6d, 0x9f, 0xf6, 0xcf, 0x73, 0x76, 0x1a, 0x80, 0x52, 0xaa, 0xaf, 0x98, 0x25,
 0x24, 0xdf, 0x6d, 0xc8, 0xea, 0x4b, 0x6e, 0xeb, 0x64, 0x41, 0x4d, 0xb7, 0x5a, 0xa7, 0x3b, 0x20,
 0x75, 0x56, 0x93, 0x44, 0x62, 0x8d, 0xf4, 0x1e, 0x49, 0x3a, 0xd3, 0x48, 0x78, 0xb0, 0x4f, 0xb0,
 0x5f, 0xe7, 0x3e, 0x07, 0xea, 0x2a, 0x58, 0x15, 0x22, 0xc9, 0x8d, 0x7f, 0x37, 0x8b, 0xef, 0xf4,
 0x71, 0x9c, 0xd5, 0x6a, 0x95, 0x6e, 0x68, 0xe7, 0xe6, 0xaa, 0x0f, 0xac, 0x1f, 0xaa, 0xa8, 0x9a,
 0xba, 0x40, 0xc0, 0x94, 0x27, 0x66, 0x3d, 0x76, 0xee, 0xb7, 0xf5, 0xa0, 0x26, 0x5d, 0xb9, 0x17,
 0xa7, 0x57, 0xbc, 0x2a, 0xe8, 0x14, 0x6c, 0xb4, 0x42, 0xc0, 0x11, 0x86, 0x76, 0xb8, 0x0d, 0x89,
 0xd8, 0xac, 0xcf, 0x07, 0x44, 0xd9, 0xa1, 0xaf, 0xe5, 0x70, 0x22, 0xae, 0x77, 0x76, 0x46, 0x99,
 0xc8, 0xdc, 0xd0, 0xe4, 0x47, 0xc1, 0x54, 0xdc, 0x5d, 0x0f, 0xb9, 0x90, 0xd1, 0xeb, 0xff, 0xa8,
 0x12, 0x7b, 0x4c, 0x25, 0x77, 0x09, 0xdb, 0x70, 0x62, 0x8e, 0x7c, 0x0f, 0xb0, 0xb9, 0xe8, 0x6a,
 0xba, 0xc8, 0xa0, 0x5f, 0x74, 0x56, 0xc5, 0x92, 0xa4, 0x4d, 0xc7, 0xeb, 0xd8, 0xe3, 0xa4, 0xc7,
 0x10, 0x4e, 0x25, 0x4f, 0x39, 0xe1, 0x78, 0xe9, 0x8d, 0x1c, 0x56, 0xef, 0x05, 0x64, 0xc6, 0x87,
 0xb9, 0x02, 0xa9, 0xc4, 0x99, 0x7f, 0xaa, 0xbb, 0xa1, 0x5c, 0xa8, 0xcc, 0x10, 0x62, 0xef, 0xb8,
 0xc1, 0xaa, 0x3b, 0xc4, 0x2b, 0xaa, 0xe3, 0x97, 0xf2, 0x6e, 0xbe, 0x83, 0x52, 0x43, 0x75, 0x83,
 0x0f, 0xe3, 0x1a, 0x15, 0x02, 0x61, 0x71, 0x17, 0xc3, 0xa2, 0xaa, 0x1a, 0x10, 0x0d, 0x30, 0xaf,
 0xd9, 0xbc, 0x4c, 0x32, 0xbc, 0x11, 0x4a, 0x20, 0xae, 0x14, 0xc2, 0x55, 0x43, 0xa5, 0x70, 0x4f,
 0x99, 0x40, 0xc1, 0x33, 0x6e, 0x69, 0xa2, 0x0c, 0x01, 0x11, 0xcd, 0xd6, 0xa7, 0xe1, 0x49, 0x87,
 0xb8, 0x98, 0x23, 0x38, 0xe8, 0x52, 0xe3, 0xed, 0x9f, 0x62, 0x42, 0xc3, 0x81, 0xe7, 0xaa, 0xe1,
 0xfb, 0x1f, 0xf2, 0xe6, 0x6a, 0x12, 0xd5, 0x70, 0x5d, 0xcd, 0xcb, 0x1f, 0xe5, 0x09, 0x99, 0x28,
 0x3c, 0xdb, 0x62, 0xa3, 0xb2, 0x67, 0x22, 0x21, 0x00, 0xa9, 0xd1, 0x0f, 0x0c, 0xea, 0x59, 0xcc,
 0x54, 0x95, 0xc0, 0x0d, 0xaf, 0x3b, 0x8e, 0x20, 0xcf, 0x4e, 0xd1, 0xf1, 0xd9, 0x50, 0x7c, 0xce,
 0x61, 0x36, 0x93, 0x1b, 0x41, 0x85, 0xeb, 0x2d, 0x02, 0x99, 0xc1, 0xe7, 0x85, 0xf4, 0x26, 0x1d,
 0x66, 0xf7, 0x67, 0x85, 0x4a, 0xf0, 0x39, 0x7a, 0x25, 0x92, 0xef, 0x4f, 0x58, 0x0f, 0x19, 0x9d,
 0x59, 0xbb, 0x2e, 0xfe, 0x5b, 0x8c, 0xa6, 0x29, 0x6c, 0xf9, 0x64, 0x87, 0x3c, 0xf2, 0xff, 0x42,
 0x11, 0xf2, 0xb8, 0x13, 0x4b, 0x58, 0x91, 0xa9, 0x71, 0x6a, 0xac, 0x9b, 0x92, 0xd4, 0xfe, 0xf9,
 0x4d, 0x17, 0xec, 0x2d, 0xcc, 0x65, 0xd5, 0x7b, 0xf0, 0xe1, 0x75, 0x0b, 0xfd, 0xd2, 0xe3, 0x8e,
 0x32, 0x62, 0x30, 0x71, 0x6c, 0x33, 0xc5, 0xd5, 0x99, 0xd4, 0xbe, 0xfb, 0x91, 0xc3, 0xaa, 0x0a,
 0xbf, 0xe9, 0x36, 0x66, 0x17, 0xb3, 0x66, 0xe8, 0x5b, 0x1c, 0x63, 0xf7, 0x62, 0x44, 0x01, 0x65,
 0x7b, 0xdf, 0x82, 0xbc, 0x2b, 0xf5, 0x84, 0xe0, 0xec, 0x59, 0xe3, 0xd2, 0xe7, 0x78, 0xae, 0xea,
 0x10, 0xf3, 0xc6, 0x21, 0xe1, 0x90, 0xc1, 0x64, 0xd1, 0x4f, 0x26, 0x26, 0x27, 0x1d, 0x71, 0x00,
 0x0b, 0x75, 0xac, 0xa7, 0x72, 0x23, 0x9d, 0xec, 0x20, 0x6d, 0x8a, 0x8e, 0x6b, 0x2a, 0x4f, 0xa1,
 0xa3, 0x64, 0x62, 0xa0, 0x48, 0xe0, 0x76, 0xfd, 0xc7, 0x26, 0x5a, 0x57, 0x4e, 0x3e, 0xcf, 0x3d,
 0xc2, 0x38, 0xa5, 0xb5, 0xeb, 0x5c, 0x59, 0xf9, 0x3a, 0x24, 0x77, 0x2a, 0x64, 0xad, 0xa1, 0x07,
 0x59, 0xf5, 0xa7, 0x18, 0xa7, 0xdd, 0xc8, 0x84, 0xd3, 0x04, 0x8b, 0x7f, 0x79, 0xfd, 0x6f, 0xb9,
 0x33, 0xa5, 0x16, 0xed, 0xa2, 0x31, 0xf1, 0xbc, 0x36, 0x3d, 0xc1, 0x2b, 0x43, 0xc8, 0xa7, 0x64,
 0x8f, 0x83, 0x46, 0x68, 0x3e, 0xa9, 0x8d, 0xb5, 0x50, 0x39, 0xe7, 0x47, 0xdd, 0xd1, 0xa8, 0xa7,
 0xf1, 0x78, 0xbd, 0x1e, 0xc5, 0x72, 0x90, 0xcd, 0xd0, 0x63, 0x9d, 0x05, 0x38, 0xc8, 0xa0, 0x92,
 0xca, 0x6e, 0x2d, 0x1a, 0x53, 0x3d, 0xca, 0x08, 0x61, 0xbb, 0xd6, 0x21, 0xda, 0xb4, 0xf1, 0x4e,
 0x08, 0xa6, 0x4c, 0x3e, 0x76, 0x63, 0xe5, 0x1e, 0x13, 0x6c, 0xa5, 0xd6, 0x89, 0xdc, 0x65, 0xbe,
 0x1f, 0xd1, 0x9d, 0x54, 0x95, 0xdb, 0xb2, 0x59, 0x23, 0xbc, 0x54, 0x6b, 0xad, 0xf3, 0x90, 0xf6,
 0xcf, 0x89, 0x21, 0x13, 0xf8, 0x1c, 0x7a, 0x5d, 0x3f, 0x61, 0xfb, 0x27, 0xd2, 0x19, 0xe1, 0x6d,
 0xe5, 0x35, 0xaf, 0xc5, 0xec, 0x7f, 0xce, 0x83, 0xae, 0x7d, 0x24, 0xf0, 0x26, 0x4a, 0x77, 0x66,
 0xbf, 0x16, 0x72, 0x78, 0x67, 0x40, 0x5b, 0xeb, 0x0b, 0x41, 0xe6, 0xd0, 0xe4, 0xe3, 0xc3, 0x3c,
 0x74, 0x00, 0x76, 0xa4, 0x59, 0x80, 0xe9, 0x42, 0xd7, 0x2a, 0xe8, 0x3a, 0xcf, 0xea, 0x9a, 0xfc,
 0x17, 0x6a, 0x93, 0xd0, 0x21, 0x89, 0x56, 0x98, 0x73, 0xd9, 0xff, 0xb0, 0xc9, 0xae, 0x38, 0x0e,
 0xb8, 0xd4, 0xa2, 0xd5, 0xd1, 0x3a, 0xa8, 0x3f, 0x8d, 0x04, 0x5b, 0x55, 0xf2, 0xd1, 0x80, 0xf2,
 0xf2, 0x37, 0x55, 0x23, 0xb2, 0x46, 0x3e, 0xc7, 0x4a, 0xd0, 0x5c, 0x26, 0xdb, 0x03, 0x7a, 0x18,
 0x9d, 0xca, 0xad, 0x5a, 0xa3, 0x5e, 0x18, 0x5f, 0xeb, 0xc6, 0x71, 0x25, 0x29, 0x8e, 0xd5, 0x6b};

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION RESET                                        */
/*                                                                            */
/******************************************************************************/
TBsdStatus bsd_reset()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;

    /* Reset the chipset */
    bsdPrint("[1010] bsdChipReset() to be executed... \n");
    bsdChipReset();
    CHECK_ERROR_CODE(bsdStatus);

    /* The following code should never be executed! */
    bsdPrint("\n-------------------------------------\n");
    BSD_TA_Print(("[1011][#E#%d] bsdChipReset FAILED!\n", bsdStatus));
    bsdPrint("-------------------------------------\n\n");
}

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION CHIP INFORMATION                              */
/*                                                                            */
/******************************************************************************/
TBsdStatus bsd_chip_information()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TUnsignedInt32 xBsdApiVersion;
    TBsd20CharsString xSoftwareVersion;
    TBsd4BytesVector xNUIDCheckNumber;
    TBsd8BytesVector xNuid64;
    TBsd4BytesVector xNuid;
    TBsd4BytesVector xStbCaSn;
    TBsd4BytesVector xMarketSegmentId;
    TUnsignedInt8 *pxData;
    TUnsignedInt32 xDataSize;
    TBsd4BytesVector xDataIntegrityCheckNumber;
    TBsdInfoChipset xInfoChipset;
    unsigned long int intNuid;
    unsigned int strsize;

#if NOCS3x
    /*
     *  Get the BSD API version
     */
    bsdStatus = bsdGetApiVersion(&xBsdApiVersion);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[2010]BSD API version %d.%d.%d\n", ((xBsdApiVersion >> 16) & 0xff), ((xBsdApiVersion >> 8) & 0xff), (xBsdApiVersion & 0xff)));

    /* COMPARE with Reference */
    if (refBsdApiVersion == xBsdApiVersion)
    {
        bsdPrint("[2010] bsdGetApiVersion PASSED\n");
    }
    else
    {
        bsdPrint("[2011][#E#] bsdGetApiVersion FAILED\n");
    }

    /*
     * Get the BSD Software version
     */
    bsdStatus = bsdGetSoftwareVersion(xSoftwareVersion);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[2020]BSD Software version %s\n", xSoftwareVersion));

    /* COMPARE with Reference */
    strsize = strlen((char *)refSoftwareVersion) + 1;
    if (memcmp(refSoftwareVersion, xSoftwareVersion, strsize))
    {
        bsdPrint("[2021][#E#] bsdGetSoftwareVersion FAILED\n");
    }
    else
    {
        bsdPrint("[2020] bsdGetSoftwareVersion PASSED\n");
    }

    /*
     * Get the BSD Nuid64
     */
    bsdStatus = bsdGetNuid64(xNuid64);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[2030]BSD NUID64 0x%02x%02x%02x%02x%02x%02x%02x%02x\n", xNuid64[0], xNuid64[1], xNuid64[2],
                  xNuid64[3], xNuid64[4], xNuid64[5], xNuid64[6], xNuid64[7]));

    /* The Nuid64 is a MASBF 8-byte vector, must be converted into integer. */
    intNuid = Convert4BytesArrayToInt32((unsigned char *)xNuid64);

    /* COMPARE with Reference */

    if (memcmp(refNuid64, xNuid64, 4))
    {
        bsdPrint("[2031][#E#] bsdGetNuid64 FAILED\n");
    }
    else
    {
        bsdPrint("[2030] bsdGetNuid64 PASSED\n");
    }

#elif NOCS1x
    /*
     * Get the BSD Nuid
     */
    bsdStatus = bsdGetNuid(xNuid);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[2030]BSD NUID 0x%02x%02x%02x%02x\n", xNuid[0], xNuid[1], xNuid[2], xNuid[3]));

    /* COMPARE with Reference */

    if (memcmp(refNuid, xNuid, 4))
    {
        bsdPrint("[2031][#E#] bsdGetNuid FAILED\n");
    }
    else
    {
        bsdPrint("[2030] bsdGetNuid PASSED\n");
    }
#endif

#if NOCS3x
    /*
     * Get the BSD Market SegmentID
     */
    bsdStatus = bsdGetMarketSegmentId(xMarketSegmentId);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[2040] Market SegmentID 0x%02x%02x%02x%02x\n", xMarketSegmentId[0], xMarketSegmentId[1],
                  xMarketSegmentId[2], xMarketSegmentId[3]));

    /* COMPARE with Reference */
    if (memcmp(refMarketSegmentId, xMarketSegmentId, 4))
    {
        bsdPrint("[2041][#E#] bsdGetMarketSegmentId FAILED\n");
    }
    else
    {
        bsdPrint("[2040] bsdGetMarketSegmentId PASSED\n");
    }

    /*
     * Get the BSD StbCaSn
     */
    bsdStatus = bsdGetStbCaSn(xStbCaSn);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[2050] StbCaSn 0x%02x%02x%02x%02x\n", xStbCaSn[0], xStbCaSn[1], xStbCaSn[2], xStbCaSn[3]));

    /* COMPARE with Reference */
#if NOCS1x
    if (memcmp(refSTBCASNDataIn, xStbCaSn, 4))
#elif NOCS3x
    if (memcmp(refStbCaSn, xStbCaSn, 4))
#endif
    {
        bsdPrint("[2051][#E#] bsdGetStbCaSn FAILED\n");
    }
    else
    {
        bsdPrint("[2050] bsdGetStbCaSn PASSED\n");
    }
#endif

    /* Get the BSD Chipset Information */
    bsdStatus = bsdChipsetInformation(&xInfoChipset);
    CHECK_ERROR_CODE(bsdStatus);

    BSD_TA_Print(("[2060] InfoChipset CM - %s\n", xInfoChipset.BSD_CM));
    BSD_TA_Print(("[2060] InfoChipset CT - %s\n", xInfoChipset.BSD_CT));
    BSD_TA_Print(("[2060] InfoChipset CUT - %s\n", (char *)&xInfoChipset.BSD_CUT));
    BSD_TA_Print(("[2060] InfoChipset EXT - %s\n", (char *)&xInfoChipset.BSD_EXT));

    /* compare the fields BSD_CM, BSD_CT, BSD_CUT, BSD_EXT*/
    /* COMPARE with Reference */
    if (memcmp(refBsdChipsetManufacturer, xInfoChipset.BSD_CM, strlen((unsigned char *)refBsdChipsetManufacturer)))
    {
        BSD_TA_Print(("[2061][#E#] xInfoChipset.BSD_CM FAILED\n"));
    }
    else
    {
        BSD_TA_Print(("[2060] xInfoChipset.BSD_CM PASSED\n"));
    }

    if (memcmp(refBsdChipsetType, xInfoChipset.BSD_CT, strlen((char *)refBsdChipsetType)))
    {
        BSD_TA_Print(("[2061][#E#] xInfoChipset.BSD_CT FAILED\n"));
    }
    else
    {
        BSD_TA_Print(("[2060] xInfoChipset.BSD_CT PASSED\n"));
    }

    if (memcmp(refBsdCut, xInfoChipset.BSD_CUT, strlen((char *)refBsdCut)))
    {
        BSD_TA_Print(("[2061][#E#] xInfoChipset.BSD_CUT FAILED\n"));
    }
    else
    {
        BSD_TA_Print(("[2060] xInfoChipset.BSD_CUT PASSED\n"));
    }
    
    if (memcmp(refBsdExtension, xInfoChipset.BSD_EXT, strlen((char *)refBsdExtension)))
    {
        BSD_TA_Print(("[2061][#E#] xInfoChipset.BSD_EXT FAILED\n"));
    }
    else
    {
        BSD_TA_Print(("[2060] xInfoChipset.BSD_EXT PASSED\n"));
    }

    /* The Nuid Check Number is a MASBF 4-byte vector, must be converted into integer. */
#if NOCS1x
    intNuid = Convert4BytesArrayToInt32((unsigned char *)xNuid);
#elif NOCS3x
    intNuid = Convert4BytesArrayToInt32((unsigned char *)xNuid64);
#endif

    /*
     *  Get the BSD Data Integrity Check Number
     */
#if NOCS1x
    pxData = (TUnsignedInt8 *)DataIntegrityIn;
#elif NOCS3x
    pxData = (TUnsignedInt8 *)refDataIntegrityIn;
#endif
    xDataSize = 4;
    bsdStatus = bsdGetDataIntegrityCheckNumber(xNuid, xDataSize, xDataIntegrityCheckNumber);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[2070] NUID Check Number 0x%02x%02x%02x%02x\n", xDataIntegrityCheckNumber[0], xDataIntegrityCheckNumber[1],
                  xDataIntegrityCheckNumber[2], xDataIntegrityCheckNumber[3]));

    /* COMPARE with Reference */
#if NOCS1x
    if (memcmp(NUIDCheckNumber[intNuid], xDataIntegrityCheckNumber, 4))
#elif NOCS3x
    if (memcmp(refDataIntegrityCN, xDataIntegrityCheckNumber, 4))
#endif
    {
        BSD_TA_Print(("[2071][#E#] bsdGetDataIntegrityCheckNumber FAILED\n"));
    }
    else
    {
        BSD_TA_Print(("[2070] bsdGetDataIntegrityCheckNumber PASSED\n"));
    }

#if NOCS3x
    /*
     * Get the BSD Nuid Check Number
     */
    bsdStatus = bsdGetNUIDCheckNumber(xNUIDCheckNumber);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[2080]BSD NUID Check Number 0x%02x%02x%02x%02x\n", xNUIDCheckNumber[0], xNUIDCheckNumber[1],
                  xNUIDCheckNumber[2], xNUIDCheckNumber[3]));

    /* COMPARE with Reference */
#if NOCS1x
    if (memcmp(NUIDCheckNumber[intNuid], xNUIDCheckNumber, 4))
#elif NOCS3x
    if (memcmp(refNUIDCheckNumber, xNUIDCheckNumber, 4))
#endif
    {
        bsdPrint("[2081][#E#] bsdGetNUIDCheckNumber FAILED\n");
    }
    else
    {
        bsdPrint("[2080] bsdGetNUIDCheckNumber PASSED\n");
    }
#endif

    /*
    *  Print the Chipset Information 
    */
    BSD_TA_Print(("\n------------------------------------- \n"));
    BSD_TA_Print(("- CHIPSET STATUS \n"));
    BSD_TA_Print(("------------------------------------- \n"));
#if NOCS3x
    BSD_TA_Print(("- BSD API version %d.%d.%d\n", ((xBsdApiVersion >> 16) & 0xff), ((xBsdApiVersion >> 8) & 0xff), (xBsdApiVersion & 0xff)));
#endif
    BSD_TA_Print(("- Manufacturer               : %s\n", xInfoChipset.BSD_CM));
    BSD_TA_Print(("- Name	                    : %s\n", xInfoChipset.BSD_CT));
    BSD_TA_Print(("- Cut	                    : %s\n", xInfoChipset.BSD_CUT));
    BSD_TA_Print(("- NUID	                    : 0x%02x%02x%02x%02x\n", xNuid[0],xNuid[1],xNuid[2],xNuid[3]));
#if NOCS3x
    BSD_TA_Print(("- MSID	                    : 0x%02x%02x%02x%02x\n", xMarketSegmentId[0],xMarketSegmentId[1],xMarketSegmentId[2],xMarketSegmentId[3]));
    BSD_TA_Print(("- StbCaSn                    : 0x%02x%02x%02x%02x\n",  xStbCaSn[0], xStbCaSn[1], xStbCaSn[2], xStbCaSn[3]));
    BSD_TA_Print(("- NUIDCN	                    : 0x%02x%02x%02x%02x\n", xNUIDCheckNumber[0],xNUIDCheckNumber[1],xNUIDCheckNumber[2],xNUIDCheckNumber[3]));
#endif
    BSD_TA_Print(("- NUID Check Number     : 0x%02x%02x%02x%02x\n", xDataIntegrityCheckNumber[0],xDataIntegrityCheckNumber[1],
                        xDataIntegrityCheckNumber[2],xDataIntegrityCheckNumber[3]));
    BSD_TA_Print(("------------------------------------- \n"));

    /* BSD Chip Information Completed */
    BSD_TA_Print(("\n-------------------------------------\n"));
    BSD_TA_Print(("[2000] BSD Chip Information Completed\n"));
    BSD_TA_Print(("-------------------------------------\n\n"));

    return bsdStatus;
}

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION RAM2RAM                                       */
/*                                                                            */
/******************************************************************************/
TBsdStatus bsd_ram2ram()
{
#if NOCS3x
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TBsd8BytesVector xNuid64;
    unsigned long int intNuid;
    TUnsignedInt16 xEmi;

    /*
     * The Nuid64 is a MASBF 8-byte vector,
     * must be converted into integer.
     */
    bsdGetNuid64(xNuid64);
    intNuid = Convert4BytesArrayToInt32((unsigned char *)xNuid64);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[3010] BSD Ram2Ram testing EMI 0x4021\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_ram2ram_op(intNuid, 0x4021);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[3020] BSD Ram2Ram testing EMI 0x4026\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_ram2ram_op(intNuid, 0x4026);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[3030] BSD Ram2Ram testing EMI 0x4041\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_ram2ram_op(intNuid, 0x4041);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[3040] BSD Ram2Ram testing EMI 0x4043\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_ram2ram_op(intNuid, 0x4043);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[3010] BSD STB Unique Data Decryption \n");
    bsdPrint("-------------------------------------\n\n");
    bsd_unique_data_ref_op(intNuid);

    /* The following code should never be executed! */
    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[3000] BSD Ram2Ram Completed\n");
    bsdPrint("-------------------------------------\n\n");
#elif NOCS1x
    TBsd4BytesVector xNuid32;
    unsigned int intNuid;

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[3010] BSD STB Unique Data Decryption \n");
    bsdPrint("-------------------------------------\n\n");

    /*
     * The Nuid32 is a MASBF 4-byte vector,
     * must be converted into integer.
     */
    bsdGetNuid(xNuid32);

    intNuid = Convert4BytesArrayToInt32(xNuid32);

    bsd_unique_data_ref_op(intNuid);
#endif

}

#if NOCS1x
TBsdStatus bsd_unique_data_ref_op(unsigned int intNuid)
#elif NOCS3x
TBsdStatus bsd_unique_data_ref_op(unsigned long int intNuid)
#endif
{
    TBsdStatus bsdStatus  = BSD_NO_ERROR;
    TSize xUniqueDataSize = 0x400;
    TUnsignedInt8 pxInputData[0x400]  = {0};
    TUnsignedInt8 pxOutputData[0x400] = {0};
    TUnsignedInt32 u32UniqueDataStartAddress = 0;
    TUnsignedInt8 *dataBuf = NULL;
    HI_S32 Ret = HI_SUCCESS;
    TUnsignedInt8 needEncrypt = 0;
    TUnsignedInt32 i = 0;
    TUnsignedInt32 inputCmd = 0;
    HI_HANDLE hFlash = 0;
    HI_Flash_InterInfo_S flashInfo;
    HI_FLASH_TYPE_E bootType;
    HI_U32 bootMedia = 0;

#if NOCS3x
    TBsdCipherSession*  pxSession;
    TSize xCipheredContentKeySize;
    TUnsignedInt8 *pxCipheredContentKey;
    TSize xCipheredProtectingKeySize;
    TUnsignedInt8 *pxL1CipheredProtectingKey;
    TUnsignedInt8 *pxL2CipheredProtectingKey;
    TUnsignedInt8 *pxInitVector;
    TSize xInitVectorSize;
    TBsdCipherSession r2rSession;
    TUnsignedInt16 xEmi;

    pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredProtectingKeyL2[intNuid][0];
    pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredProtectingKeyL1[intNuid][0];
    pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredContentKey[intNuid][0];
    pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4041_IVCbc[intNuid][0];
    xInitVectorSize = 16;
    xEmi = 0x4041;
#endif

    //	  pxInputData = (TUnsignedInt8*)malloc(xUniqueDataSize * sizeof(unsigned char));
    //	  pxOutputData = (TUnsignedInt8*)malloc(xUniqueDataSize * sizeof(unsigned char));
    //    memset (pxOutputData, 0, xUniqueDataSize);

    /*TODO: Chipset Manufacturer to insert the code related to Flash Initialize
            and Flash Read (into pxInputData buffer) for the SELECTED_FLASH_TYPE */

    /* Here Data is read from SCS_Unchecked_Area*/

    /* Assumption is the very first time Flash and the SCS_Unchecked_Area is empty
       and following operation will be failed*/
    u32UniqueDataStartAddress = UNIQUE_DATA_REF_OFFSET;

    bootMedia = get_bootmedia(NULL, NULL);

    if (BOOT_MEDIA_SPIFLASH == bootMedia)
    {
        bootType = HI_FLASH_TYPE_SPI_0;
    }
    else if (BOOT_MEDIA_NAND == bootMedia)
    {
        bootType = HI_FLASH_TYPE_NAND_0;
    }
    else
    {
        BSD_TA_Print(("Get Boot Mode ERROR!\n"));
        bsdStatus = BSD_ERROR;
        return bsdStatus;
    }

    /*Open the Boot Flash*/
    hFlash = HI_Flash_OpenByTypeAndAddr(bootType, 0, BOOTAREA_SIZE);
    if (HI_INVALID_HANDLE == hFlash)
    {
        BSD_TA_Print(("HI_Flash_Open failed\n"));
        bsdStatus = BSD_ERROR;
        return bsdStatus;
    }

    if (HI_FLASH_TYPE_SPI_0 == bootType)
    {
        BSD_TA_Print(("Open SPI FLASH Successful!\n"));
    }
    else if (HI_FLASH_TYPE_NAND_0 == bootType)
    {
        BSD_TA_Print(("Open NAND FLASH Successful!\n"));
    }

    dataBuf = 0x01000000;
    
    /*Get the blocksize of the boot flash*/
    Ret = HI_Flash_GetInfo(hFlash, &flashInfo);
    if (HI_SUCCESS != Ret)
    {
        BSD_TA_Print(("HI_Flash_GetInfo Failed:%#x\n",Ret));
        bsdStatus = BSD_ERROR;
        goto FLASHCLOSE;
    }

    /* read the whole blocksize data from boot code */
    Ret = HI_Flash_Read(hFlash, 0, dataBuf, flashInfo.BlockSize, HI_FLASH_RW_FLAG_RAW);
    if (Ret < 0)
    {
        BSD_TA_Print(("HI_Flash_Read Failed:%#x\n",Ret));
        bsdStatus = BSD_ERROR;
        goto FLASHCLOSE;
    }

    /*Get the unique data enc from the SCS_Uncheck_Area*/
    memcpy(pxInputData, dataBuf + u32UniqueDataStartAddress, xUniqueDataSize);

    /* Check the start and end of the encrypted form of the STB Unique
      Data Decryption Boot Code stored in the Flash device */
    if ((!memcmp(&(pxInputData[0]), &(UniqueDataEnc[intNuid][0]),
                0x10)) && (!memcmp(&(pxInputData[0x400 - 16]), &(UniqueDataEnc[intNuid][16]), 0x10)))
    {
        printf("[30-0]STB Unique Data Decryption Boot Code Encrypted form Successful\n");

#if NOCS1x
        bsdDecryptDataWithSecretContentKey(BSD_R2R_ALGORITHM_TDES,
                                           pxInputData,
                                           pxOutputData,
                                           xUniqueDataSize);
#elif NOCS3x
        bsdStatus = (bsdOpenRam2RamDecryptSession (&r2rSession));
        CHECK_ERROR_CODE(bsdStatus);

        printf("[30-0]BSD set 2 Level Ram2Ram Protected Keys\n");
        bsdStatus = bsdSet2LevelProtectedKey  (r2rSession, xEmi, \
                                               xCipheredContentKeySize, pxCipheredContentKey, \
                                               xCipheredProtectingKeySize, pxL1CipheredProtectingKey,
                                               pxL2CipheredProtectingKey);
        CHECK_ERROR_CODE(bsdStatus);

        bsdStatus = bsdSessionDecrypt (r2rSession, pxInputData, pxOutputData, xUniqueDataSize, pxInitVector,
                                       xInitVectorSize);
        CHECK_ERROR_CODE(bsdStatus);

        bsdCloseSession(r2rSession);
#endif

        /* Compare OutputData with the content of UniqueDataRef.bin */
        if (!memcmp (pxOutputData, UniqueDataRef, xUniqueDataSize))
        {
            printf("[30-0]STB Unique Data Decryption Boot Code Successful\n");
            bsdStatus = BSD_NO_ERROR;
            needEncrypt = 0;
        }
        else
        {
            printf("[30-0]STB Unique Data Decryption Boot Code Failed\n");
            needEncrypt = 1;
        }
    }
    else
    {
        printf("[30-0]STB Unique Data Decryption Boot Code Encrypted form Failed\n");
        needEncrypt = 1;
    }

    /*print the UniqueDataRef*/
    printf("UniqueDataRef = ");
    for (i = 0; i< xUniqueDataSize - 1; i++)
    {
        if (i % 16 == 0)
        {
            printf("\n");
        }

        printf("0x%02x,", UniqueDataRef[i]);
    }
    printf("0x%02x\n", UniqueDataRef[xUniqueDataSize - 1]);

    if (1 == needEncrypt)
    {
        /* Assumption is the very first time Flash and the SCS_Unchecked_Area is empty
           and following operation will be failed*/

        /* Decrypt the STB Unique Data Decryption Boot Code from the
                SCS_Uncheck_Area */;

        /* Assumption is the very first time uniqueDataRef will be correctly
           encrypted*/

        /* Encrypt the STB Unique Data Decryption Boot Code from the
            SCS_UNCHECK_AREA */;

        //pxInputData = UniqueDataRef;
        printf("Please select:\n"
            "[a] Update the content of SCS_Unchecked_Area\n"
            "[b] Terminate the STB Unique Data Decryption Boot Code\n");

        inputCmd = fgetc(stdin);

        if ('a' == inputCmd)
        {
            printf("encrypting and writing to the flash...\n");
            /* Encrypt the STB Unique Data Decryption Boot Code from the
            SCS_UNCHECK_AREA */;
            memcpy(pxInputData, UniqueDataRef, xUniqueDataSize);

#if NOCS1x
            bsdEncryptDataWithSecretContentKey(BSD_R2R_ALGORITHM_TDES,
                                               pxInputData,
                                               pxOutputData,
                                               xUniqueDataSize);
#elif NOCS3x
            bsdStatus = (bsdOpenRam2RamEncryptSession (&r2rSession));
            CHECK_ERROR_CODE(bsdStatus);

            printf("[30-0]BSD set 2 Level Ram2Ram Protected Keys\n");
            bsdStatus = bsdSet2LevelProtectedKey  (r2rSession, xEmi, \
                                                   xCipheredContentKeySize, pxCipheredContentKey, \
                                                   xCipheredProtectingKeySize, pxL1CipheredProtectingKey,
                                                   pxL2CipheredProtectingKey);
            CHECK_ERROR_CODE(bsdStatus);

            bsdStatus = bsdSessionEncrypt (r2rSession, pxInputData, pxOutputData, xUniqueDataSize, pxInitVector,
                                           xInitVectorSize);
            CHECK_ERROR_CODE(bsdStatus);

            bsdCloseSession(r2rSession);
#endif

            printf("The encrypted data is:\n");
            for (i = 0; i< xUniqueDataSize - 1; i++)
            {
                if (i % 16 == 0)
                {
                    printf("\n");
                }

                printf("0x%02x,", pxOutputData[i]);
            }
            printf("0x%02x\n", pxOutputData[xUniqueDataSize - 1]);

            /*TODO: Chipset Manufacturer to insert the code related to Flash Flash
              Write (from pxOutputData buffer) for the SELECTED_FLASH_TYPE
              to SCS_UNCHECK_AREA */
            memcpy(dataBuf + u32UniqueDataStartAddress, pxOutputData, xUniqueDataSize);

            /*write the encrypted data into flash*/
            Ret = HI_Flash_Write(hFlash, 0, dataBuf, flashInfo.BlockSize, HI_FLASH_RW_FLAG_ERASE_FIRST);
            if (Ret < 0)
            {
                printf("HI_Flash_Write Failed:%#x\n", Ret);
                bsdStatus = BSD_ERROR;
                goto FLASHCLOSE;
            }

            printf("STB Unique Data Write Boot Code Successful\n");
        }
        else if ('b' == inputCmd)
        {
            printf("quit!\n");
        }
        else
        {
            printf("Invalid Choice!\n");
        }
    }

FLASHCLOSE:
    HI_Flash_Close(hFlash);

#if NOCS1x
    /*Encrypt the Unique data using AES algorithm*/
    memcpy(pxInputData, UniqueDataRef, xUniqueDataSize);
    bsdStatus = bsdEncryptDataWithSecretContentKey(BSD_R2R_ALGORITHM_AES,
                                       pxInputData,
                                       pxOutputData,
                                       xUniqueDataSize);
    if (BSD_NO_ERROR != bsdStatus)
    {
        printf("Unique Data encrypt failed!\n");
        return bsdStatus;
    }
    
    /*Decrypt to get the Unique data using AES algorithm*/
    memcpy(pxInputData, pxOutputData, xUniqueDataSize);
    bsdStatus = bsdDecryptDataWithSecretContentKey(BSD_R2R_ALGORITHM_AES,
                                       pxInputData,
                                       pxOutputData,
                                       xUniqueDataSize);
    if (BSD_NO_ERROR != bsdStatus)
    {
        printf("Unique Data encrypt failed!\n");
        return bsdStatus;
    }
    
    /* Compare OutputData with the content of UniqueDataRef.bin */
    if (!memcmp (pxOutputData, UniqueDataRef, xUniqueDataSize))
    {
        printf("STB Unique Data Encryption & Decryption with AES algorithm Successful\n");
        bsdStatus = BSD_NO_ERROR;
    }
    else
    {
        printf("STB Unique Data Encryption & Decryption with AES algorithm Failed\n");
        bsdStatus = BSD_ERROR;
    }
#endif

    return bsdStatus;
}

#if NOCS3x
TBsdStatus bsd_ram2ram_op(unsigned long int intNuid, TUnsignedInt16 xEmi)
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TBsdCipherSession*  pxSession;
    TSize xClearTextKeySize;
    TUnsignedInt8 *pxClearTextKey;
    TSize xCipheredContentKeySize;
    TUnsignedInt8 *pxCipheredContentKey;
    TSize xCipheredProtectingKeySize;
    TUnsignedInt8 *pxL1CipheredProtectingKey;
    TUnsignedInt8 *pxL2CipheredProtectingKey;
    TUnsignedInt8 *pxInput, *pxDecInput, *pxEncInput;
    TUnsignedInt8 *pxOutput, *pxDecRef, *pxEncRef;
    TSize xDataSize;
    TUnsignedInt8 *pxInitVector;
    TSize xInitVectorSize;
    TBsdCipherSession r2rSession;

    /*TODO: Chipset Manufacturer to instanciate the pxKeyPathHandle parameter */

    switch (xEmi)
    {
    case 0x4021:
        pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4021_CipheredProtectingKeyL2[intNuid][0];
        pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4021_CipheredProtectingKeyL1[intNuid][0];
        pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4021_CipheredContentKey[intNuid][0];
        pxClearTextKey = (TUnsignedInt8*)&ram2ram_emi_4021_HostKey[intNuid][0];
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4021_IVCbc[intNuid][0];
        xDataSize = ram2ram_emi_4021_enc_in_size;
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4021_IVCbc[intNuid][0];
        xInitVectorSize = 8;
        pxDecRef   = (TUnsignedInt8*)ram2ram_emi_4021_dec_ref;
        pxEncRef   = (TUnsignedInt8*)ram2ram_emi_4021_enc_ref;
        pxDecInput = (TUnsignedInt8*)ram2ram_emi_4021_dec_in;
        pxEncInput = (TUnsignedInt8*)ram2ram_emi_4021_enc_in;

        break;
    case 0x4026:
        pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4026_CipheredProtectingKeyL2[intNuid][0];
        pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4026_CipheredProtectingKeyL1[intNuid][0];
        pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4026_CipheredContentKey[intNuid][0];
        pxClearTextKey = (TUnsignedInt8*)&ram2ram_emi_4026_HostKey[intNuid][0];
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4026_IVCbc[intNuid][0];
        xDataSize = ram2ram_emi_4026_enc_in_size;
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4026_IVCbc[intNuid][0];
        xInitVectorSize = 8;
        pxDecRef   = (TUnsignedInt8*)ram2ram_emi_4026_dec_ref;
        pxEncRef   = (TUnsignedInt8*)ram2ram_emi_4026_enc_ref;
        pxDecInput = (TUnsignedInt8*)ram2ram_emi_4026_dec_in;
        pxEncInput = (TUnsignedInt8*)ram2ram_emi_4026_enc_in;
        break;
    case 0x4041:
        pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredProtectingKeyL2[intNuid][0];
        pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredProtectingKeyL1[intNuid][0];
        pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredContentKey[intNuid][0];
        pxClearTextKey = (TUnsignedInt8*)&ram2ram_emi_4041_HostKey[intNuid][0];
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4041_IVCbc[intNuid][0];
        xDataSize = ram2ram_emi_4041_enc_in_size;
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4041_IVCbc[intNuid][0];
        xInitVectorSize = 16;
        pxDecRef   = (TUnsignedInt8*)ram2ram_emi_4041_dec_ref;
        pxEncRef   = (TUnsignedInt8*)ram2ram_emi_4041_enc_ref;
        pxDecInput = (TUnsignedInt8*)ram2ram_emi_4041_dec_in;
        pxEncInput = (TUnsignedInt8*)ram2ram_emi_4041_enc_in;
        break;
    case 0x4043:
        pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4043_CipheredProtectingKeyL2[intNuid][0];
        pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4043_CipheredProtectingKeyL1[intNuid][0];
        pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4043_CipheredContentKey[intNuid][0];
        pxClearTextKey = (TUnsignedInt8*)&ram2ram_emi_4043_HostKey[intNuid][0];
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4043_IVCbc[intNuid][0];
        xDataSize = ram2ram_emi_4043_enc_in_size;
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4043_IVCbc[intNuid][0];
        xInitVectorSize = 16;
        pxDecRef   = (TUnsignedInt8*)ram2ram_emi_4043_dec_ref;
        pxEncRef   = (TUnsignedInt8*)ram2ram_emi_4043_enc_ref;
        pxDecInput = (TUnsignedInt8*)ram2ram_emi_4043_dec_in;
        pxEncInput = (TUnsignedInt8*)ram2ram_emi_4043_enc_in;
        break;
    default:
        break;
    }

    bsdPrint("[30-0]BSD set Clear Text Ram2Ram Key\n");

    /*
     * Encrypt with Clear Text Key
     */

    bsdStatus = (bsdOpenRam2RamEncryptSession (&r2rSession));
    CHECK_ERROR_CODE(bsdStatus);

    bsdStatus = bsdSetClearTextKey (r2rSession, xEmi, xClearTextKeySize, pxClearTextKey);
    CHECK_ERROR_CODE(bsdStatus);

    pxOutput = (TUnsignedInt8*)malloc(xDataSize * sizeof(char));
    if (pxOutput == NULL)
    {
        return BSD_ERROR;
    }

    pxInput = pxEncInput;

    bsdStatus = bsdSessionEncrypt (r2rSession, pxInput, pxOutput, xDataSize, pxInitVector, xInitVectorSize);
    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    if (memcmp(pxEncRef, pxOutput, xDataSize))
    {
        bsdPrint("[30-0][#E#] R2R Encrypt with Clear Text Key FAILED\n");
    }
    else
    {
        bsdPrint("[30-0] R2R Encrypt with Clear Text Key PASSED\n");
    }

    bsdCloseSession(r2rSession);
    free(pxOutput);

    /*
     * Decrypt with Clear Text Key
     */

    bsdStatus = (bsdOpenRam2RamDecryptSession (&r2rSession));
    CHECK_ERROR_CODE(bsdStatus);

    bsdStatus = bsdSetClearTextKey (r2rSession, xEmi, xClearTextKeySize, pxClearTextKey);
    CHECK_ERROR_CODE(bsdStatus);

    pxOutput = (TUnsignedInt8*)malloc(xDataSize * sizeof(char));
    if (pxOutput == NULL)
    {
        return BSD_ERROR;
    }

    pxInput = pxDecInput;

    bsdStatus = bsdSessionDecrypt (r2rSession, pxInput, pxOutput, xDataSize, pxInitVector, xInitVectorSize);
    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    if (memcmp(pxDecRef, pxOutput, xDataSize))
    {
        bsdPrint("[30-0][#E#] R2R Decrypt with Clear Text Key FAILED\n");
    }
    else
    {
        bsdPrint("[30-0] R2R Decrypt with Clear Text Key PASSED\n");
    }

    bsdCloseSession(r2rSession);
    free(pxOutput);

    /*
     * Encrypt with Ciphered Content Key
     */

    bsdStatus = (bsdOpenRam2RamEncryptSession (&r2rSession));
    CHECK_ERROR_CODE(bsdStatus);

    bsdPrint("[30-0]BSD set 2 Level Ram2Ram Protected Keys\n");
    bsdStatus = bsdSet2LevelProtectedKey  (r2rSession, xEmi, \
                                           xCipheredContentKeySize, pxCipheredContentKey, \
                                           xCipheredProtectingKeySize, pxL1CipheredProtectingKey,
                                           pxL2CipheredProtectingKey);
    CHECK_ERROR_CODE(bsdStatus);

    pxOutput = (TUnsignedInt8*)malloc(xDataSize * sizeof(char));
    if (pxOutput == NULL)
    {
        return BSD_ERROR;
    }

    pxInput = pxEncInput;

    bsdStatus = bsdSessionEncrypt (r2rSession, pxInput, pxOutput, xDataSize, pxInitVector, xInitVectorSize);
    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    if (memcmp(pxEncRef, pxOutput, xDataSize))
    {
        bsdPrint("[30-0][#E#] R2R Encrypt with Ciphered Content Key FAILED\n");
    }
    else
    {
        bsdPrint("[30-0] R2R Encrypt with Ciphered Content PASSED\n");
    }

    bsdCloseSession(r2rSession);
    free(pxOutput);

    /*
     * Decrypt with Ciphered Content Key
     */

    bsdStatus = (bsdOpenRam2RamDecryptSession (&r2rSession));
    CHECK_ERROR_CODE(bsdStatus);

    bsdPrint("[30-0]BSD set 2 Level Ram2Ram Protected Keys\n");
    bsdStatus = bsdSet2LevelProtectedKey  (r2rSession, xEmi, \
                                           xCipheredContentKeySize, pxCipheredContentKey, \
                                           xCipheredProtectingKeySize, pxL1CipheredProtectingKey,
                                           pxL2CipheredProtectingKey);
    CHECK_ERROR_CODE(bsdStatus);

    pxOutput = (TUnsignedInt8*)malloc(xDataSize * sizeof(char));
    if (pxOutput == NULL)
    {
        return BSD_ERROR;
    }

    pxInput = pxDecInput;

    bsdStatus = bsdSessionDecrypt (r2rSession, pxInput, pxOutput, xDataSize, pxInitVector, xInitVectorSize);
    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    if (memcmp(pxDecRef, pxOutput, xDataSize))
    {
        bsdPrint("[30-0][#E#] R2R Decrypt with Ciphered Content Key FAILED\n");
    }
    else
    {
        bsdPrint("[30-0] R2R Decrypt with Ciphered Content PASSED\n");
    }

    bsdCloseSession(r2rSession);
    free(pxOutput);

    return bsdStatus;
}

#endif

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION FLASH PROTECTION                              */
/*                                                                            */
/******************************************************************************/
#if NOCS3x

TBsdStatus bsd_flash()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TUnsignedInt16 xEmi;

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[4010] BSD Flash Protection Root Key testing\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_flash_rootkey_op();

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[4020] BSD Flash Protection testing EMI 0x4021\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_flash_op(0x4021);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[4030] BSD Flash Protection testing EMI 0x4026\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_flash_op(0x4026);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[4040] BSD Flash Protection testing EMI 0x4041\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_flash_op(0x4041);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[4050] BSD Flash Protection testing EMI 0x4043\n");
    bsdPrint("-------------------------------------\n\n");
    bsd_flash_op(0x4043);

    /* The following code should never be executed! */
    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[4000] BSD Flash Protection Completed\n");
    bsdPrint("-------------------------------------\n\n");
}

TBsdStatus bsd_flash_op(TUnsignedInt16 xEmi)
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TUnsignedInt8 *pxClearTextKey;
    TUnsignedInt8 pxOutputData[16];
    TSize xDataSize;
    TBsdCipherSession r2rSession;
    TUnsignedInt8 *pxInput, *pxDecInput, *pxEncInput;
    TUnsignedInt8 *pxOutput, *pxDecRef, *pxEncRef;
    TUnsignedInt8 *pxInitVector;
    TSize xInitVectorSize;

    switch (xEmi)
    {
    case 0x4021:
        pxClearTextKey = (TUnsignedInt8*)&refClearTextFlashProtectionKey;
        pxInitVector = (TUnsignedInt8*)&flash_aes_IVCbc;
        xDataSize = refClearTextFlashDataSize;
        xInitVectorSize = 16;
        pxDecRef   = (TUnsignedInt8*)flashRefDecAesEcb;
        pxEncRef   = (TUnsignedInt8*)flashRefEncAesEcb;
        pxDecInput = (TUnsignedInt8*)flashInDecAesEcb;
        pxEncInput = (TUnsignedInt8*)flashInEncAesEcb;

        break;
    case 0x4026:
        pxClearTextKey = (TUnsignedInt8*)&refClearTextFlashProtectionKey;
        pxInitVector = (TUnsignedInt8*)&flash_aes_IVCbc;
        xDataSize = refClearTextFlashDataSize;
        xInitVectorSize = 16;
        pxDecRef   = (TUnsignedInt8*)flashRefDecAesCbc;
        pxEncRef   = (TUnsignedInt8*)flashRefEncAesCbc;
        pxDecInput = (TUnsignedInt8*)flashInDecAesCbc;
        pxEncInput = (TUnsignedInt8*)flashInEncAesCbc;
        break;
    case 0x4041:
        pxClearTextKey = (TUnsignedInt8*)&refClearTextFlashProtectionKey;
        pxInitVector = (TUnsignedInt8*)&flash_tdes_IVCbc;
        xDataSize = refClearTextFlashDataSize;
        xInitVectorSize = 16;
        pxDecRef   = (TUnsignedInt8*)flashRefDecTdesEcb;
        pxEncRef   = (TUnsignedInt8*)flashRefEncTdesEcb;
        pxDecInput = (TUnsignedInt8*)flashInDecTdesEcb;
        pxEncInput = (TUnsignedInt8*)flashInEncTdesEcb;
        break;
    case 0x4043:
        pxClearTextKey = (TUnsignedInt8*)&refClearTextFlashProtectionKey;
        pxInitVector = (TUnsignedInt8*)&flash_tdes_IVCbc;
        xDataSize = refClearTextFlashDataSize;
        xInitVectorSize = 16;
        pxDecRef   = (TUnsignedInt8*)flashRefDecTdesCbc;
        pxEncRef   = (TUnsignedInt8*)flashRefEncTdesCbc;
        pxDecInput = (TUnsignedInt8*)flashInDecTdesCbc;
        pxEncInput = (TUnsignedInt8*)flashInEncTdesCbc;
        break;
    default:
        break;
    }

    /*******************************/
    /*  CLEAR TEXT FLASH PROT KEY  */
    /*******************************/

    /*
     * Encrypt with Clear Text Key
     */

    /* Open Encryption Session */

    bsdStatus = (bsdOpenRam2RamEncryptSession (&r2rSession));
    CHECK_ERROR_CODE(bsdStatus);

    /* Use the Clear Text Flash Protection Key */
    bsdStatus = bsdUseFlashProtKey(r2rSession, xEmi);
    CHECK_ERROR_CODE(bsdStatus);

    pxOutput = (TUnsignedInt8*)malloc(xDataSize * sizeof(char));
    if (pxOutput == NULL)
    {
        return BSD_ERROR;
    }

    pxInput = pxEncInput;

    bsdStatus = bsdSessionEncrypt (r2rSession, pxInput, pxOutput, xDataSize, pxInitVector, xInitVectorSize);
    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    if (memcmp(pxEncRef, pxOutput, xDataSize))
    {
        bsdPrint("[40-0][#E#] Flash Encrypt with Clear Text Flash Protection Key  FAILED\n");
    }
    else
    {
        bsdPrint("[40-0] Flash Encrypt with Clear Text Flash Protection Key PASSED\n");
    }

    bsdCloseSession(r2rSession);
    free(pxOutput);

    /*
     * Decrypt with Clear Text Key
     */

    /* Open Decryption Session */

    bsdStatus = (bsdOpenRam2RamDecryptSession (&r2rSession));
    CHECK_ERROR_CODE(bsdStatus);

    /* Use the Clear Text Flash Protection Key */
    bsdStatus = bsdUseFlashProtKey(r2rSession, xEmi);
    CHECK_ERROR_CODE(bsdStatus);

    pxOutput = (TUnsignedInt8*)malloc(xDataSize * sizeof(char));
    if (pxOutput == NULL)
    {
        return BSD_ERROR;
    }

    pxInput = pxDecInput;

    bsdStatus = bsdSessionDecrypt (r2rSession, pxInput, pxOutput, xDataSize, pxInitVector, xInitVectorSize);
    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    if (memcmp(pxDecRef, pxOutput, xDataSize))
    {
        bsdPrint("[40-0][#E#] Flash Decrypt with Clear Text Flash Protection Key  FAILED\n");
    }
    else
    {
        bsdPrint("[40-0] Flash Decrypt with Clear Text Flash Protection Key PASSED\n");
    }

    bsdCloseSession(r2rSession);
    free(pxOutput);

    return bsdStatus;
}

#endif
#if NOCS3x

TBsdStatus bsd_flash_rootkey_op()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TUnsignedInt8 *pxInputData;
    TUnsignedInt8 *pxOutputData;
    TBsd8BytesVector xNuid64;
    size_t xDataSize;
    TBsdCipherSession r2rSession;
    unsigned long int intNuid;

    /*************************/
    /*  FLASH PROT-ROOT KEY  */
    /*************************/

    bsdPrint("\n------------------------------------------------------\n");
    bsdPrint("[4010] BSD Flash Protection - Flash Prot-Root Key\n");
    bsdPrint("------------------------------------------------------\n\n");

    /*
     * The Nuid64 is a MSBF 8-byte vector,
     * must be converted into integer.
     */
    bsdGetNuid64(xNuid64);
    intNuid = Convert4BytesArrayToInt32((unsigned char *)xNuid64);
    intNuid = 0x0;

    pxOutputData = malloc(sizeof(16));
    memset (pxOutputData, 0, sizeof(16));

    /* Set Reference Values */
    pxInputData = (TUnsignedInt8*)refProtRootEncryptInputData;
    xDataSize = refProtRootDataSize;

    /* Encrypt Data with Flash Prot-Root Key */
    bsdStatus = bsdEncryptFlashProtKey( pxInputData, pxOutputData, xDataSize);
    CHECK_ERROR_CODE(bsdStatus);

    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    bsdPrint("[4010] bsdEncryptFlashProtKey PASSED\n");

    /* COMPARE with Reference */
    if (memcmp(&refProtRootOutputEncryptionData[intNuid][0], pxOutputData, xDataSize))
    {
        bsdPrint("[40-0][#E#] bsdEncryptFlashProtKey FAILED\n");
    }
    else
    {
        bsdPrint("[40-0]bsdEncryptFlashProtKey PASSED\n");
    }

    return bsdStatus;
}

#endif

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION CERT                                           */
/*                                                                            */
/******************************************************************************/

#if NOCS3x

TBsdStatus bsd_cert()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TUnsignedInt32 intNuid;

    certGetNuid(&intNuid);

    bsdStatus = cert_activate(intNuid);
    CHECK_ERROR(cert_activate, bsdStatus);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[5010] CERT Ram2Ram testing EMI 0x4021\n");
    bsdPrint("-------------------------------------\n\n");
    bsdStatus = cert_ram2ram_op(intNuid, 0x4021);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[5020] CERT Ram2Ram testing EMI 0x4026\n");
    bsdPrint("-------------------------------------\n\n");
    bsdStatus = cert_ram2ram_op(intNuid, 0x4026);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[5030] CERT Ram2Ram testing EMI 0x4041\n");
    bsdPrint("-------------------------------------\n\n");
    bsdStatus = cert_ram2ram_op(intNuid, 0x4041);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[5040] CERT Ram2Ram testing EMI 0x4043\n");
    bsdPrint("-------------------------------------\n\n");
    bsdStatus = cert_ram2ram_op(intNuid, 0x4043);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[5000] CERT Ram2Ram Completed\n");
    bsdPrint("-------------------------------------\n\n");

    return bsdStatus;
}

#endif

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION RSA                                           */
/*                                                                            */
/******************************************************************************/
#if NOCS3x

TBsdStatus bsd_rsa()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TBsdRsaPadding xPadding;
    TUnsignedInt8 *pxInput;
    TSize xInputSize;
    TSize *pxOutputSize;
    TUnsignedInt8 *pxOutput;
    TUnsignedInt32 xE;
    TUnsignedInt8 *pxD;
    TUnsignedInt8 *pxN;
    TSize xKeySize;
    TUnsignedInt8 *pxTmp1 = NULL, *pxTmp2 = NULL, *pxRefIn;

    /*
     * Testing the supported padding
     */

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[6010] BSD RSA - PKCS1_V1_5_PADDING \n");
    bsdPrint("-------------------------------------\n\n");
    xPadding  = BSD_RSA_PKCS1_V1_5_PADDING;
    bsdStatus = bsd_rsa_op(xPadding);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[6020] BSD RSA - OAEP_SHA_1_PADDING \n");
    bsdPrint("-------------------------------------\n\n");
    xPadding  = BSD_RSA_OAEP_SHA_1_PADDING;
    bsdStatus = bsd_rsa_op(xPadding);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[6030] BSD RSA - OAEP_SHA_256_PADDING \n");
    bsdPrint("-------------------------------------\n\n");
    xPadding  = BSD_RSA_OAEP_SHA_256_PADDING;
    bsdStatus = bsd_rsa_op(xPadding);

    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[6040] BSD RSA - NO_PADDING \n");
    bsdPrint("-------------------------------------\n\n");
    xPadding  = BSD_RSA_NO_PADDING;
    bsdStatus = bsd_rsa_op(xPadding);

    /* RSA Module completed */
    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[6000] BSD RSA Completed\n");
    bsdPrint("-------------------------------------\n");

    return bsdStatus;
}

#endif

#if NOCS3x

TBsdStatus bsd_rsa_op(TBsdRsaPadding xPadding)
{
    TBsdStatus bsdStatus  = BSD_NO_ERROR;
    TUnsignedInt8 *pxTmp1 = NULL, *pxTmp2 = NULL, *pxRefIn;
    TUnsignedInt8 *pxInput;
    TSize xInputSize;
    TSize *pxOutputSize;
    TUnsignedInt8 *pxOutput;
    TUnsignedInt32 xE;
    TUnsignedInt8 *pxD;
    TUnsignedInt8 *pxN;
    TSize xKeySize;

    /*
     * Encryption Mode
     */

    /* OAEP only support encryption */

    /* Set INPUT Reference */
    switch (xPadding)
    {
    case BSD_RSA_PKCS1_V1_5_PADDING:
        pxRefIn = (TUnsignedInt8*)rsa_padding_0_pub_enc_in;
        pxInput = pxRefIn;
        xInputSize = rsa_padding_0_pub_enc_in_size;
        pxTmp1 = (TUnsignedInt8*)malloc(rsa_padding_0_pub_enc_in_size * sizeof(char));
        pxTmp2 = (TUnsignedInt8*)malloc(rsa_padding_0_pub_enc_in_size * sizeof(char));
        if (pxTmp1 == NULL)
        {
            return BSD_ERROR;
        }

        if (pxTmp2 == NULL)
        {
            return BSD_ERROR;
        }

        pxOutput = pxTmp1;
        memset(pxOutput, 0x0, rsa_padding_0_pub_enc_in_size);
        xE  = rsa_padding_0_pub_enc_exp_e;
        pxN = (TUnsignedInt8*)rsa_padding_0_pub_enc_n;
        xKeySize = rsa_padding_0_pub_enc_key_size;

        break;
    case BSD_RSA_OAEP_SHA_1_PADDING:
        pxRefIn = (TUnsignedInt8*)rsa_padding_1_pub_enc_in;
        pxInput = pxRefIn;
        xInputSize = rsa_padding_1_pub_enc_in_size;
        pxTmp1 = (TUnsignedInt8*)malloc(rsa_padding_1_pub_enc_in_size * sizeof(char));
        pxTmp2 = (TUnsignedInt8*)malloc(rsa_padding_1_pub_enc_in_size * sizeof(char));
        if (pxTmp1 == NULL)
        {
            return BSD_ERROR;
        }

        if (pxTmp2 == NULL)
        {
            return BSD_ERROR;
        }

        pxOutput = pxTmp1;
        memset(pxOutput, 0x0, rsa_padding_1_pub_enc_in_size);
        xE  = rsa_padding_1_pub_enc_exp_e;
        pxN = (TUnsignedInt8*)rsa_padding_1_pub_enc_n;
        xKeySize = rsa_padding_1_pub_enc_key_size;

        break;

    case BSD_RSA_OAEP_SHA_256_PADDING:
        pxRefIn = (TUnsignedInt8*)rsa_padding_2_pub_enc_in;
        pxInput = pxRefIn;
        xInputSize = rsa_padding_2_pub_enc_in_size;
        pxTmp1 = (TUnsignedInt8*)malloc(rsa_padding_2_pub_enc_in_size * sizeof(char));
        pxTmp2 = (TUnsignedInt8*)malloc(rsa_padding_2_pub_enc_in_size * sizeof(char));
        if (pxTmp1 == NULL)
        {
            return BSD_ERROR;
        }

        if (pxTmp2 == NULL)
        {
            return BSD_ERROR;
        }

        pxOutput = pxTmp1;
        memset(pxOutput, 0x0, rsa_padding_2_pub_enc_in_size);
        xE  = rsa_padding_2_pub_enc_exp_e;
        pxN = (TUnsignedInt8*)rsa_padding_2_pub_enc_n;
        xKeySize = rsa_padding_2_pub_enc_key_size;

        break;
    case BSD_RSA_NO_PADDING:
        pxRefIn = (TUnsignedInt8*)rsa_padding_3_pub_enc_in;
        pxInput = pxRefIn;
        xInputSize = rsa_padding_3_pub_enc_in_size;
        pxTmp1 = (TUnsignedInt8*)malloc(rsa_padding_3_pub_enc_in_size * sizeof(char));
        pxTmp2 = (TUnsignedInt8*)malloc(rsa_padding_3_pub_enc_in_size * sizeof(char));
        if (pxTmp1 == NULL)
        {
            return BSD_ERROR;
        }

        if (pxTmp2 == NULL)
        {
            return BSD_ERROR;
        }

        pxOutput = pxTmp1;
        memset(pxOutput, 0x0, rsa_padding_3_pub_enc_in_size);
        xE  = rsa_padding_3_pub_enc_exp_e;
        pxN = (TUnsignedInt8*)rsa_padding_3_pub_enc_n;
        xKeySize = rsa_padding_3_pub_enc_key_size;

        break;
    default:
        return BSD_ERROR;
    }

    /* Execute RSA Public Encrypt */
    bsdStatus = bsdRsaPublicEncrypt (pxInput, xInputSize, pxOutput, xE, pxN, xKeySize, xPadding);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[60%d0] bsdRsaPublicEncrypt PASSED\n", xPadding));

    // result random

    /* Set INPUT */
    pxInput  = pxOutput;
    pxOutput = pxTmp2;

    /*COMPARE */
    if (memcmp(pxRefIn, pxOutput, xInputSize))
    {
        BSD_TA_Print(("\n-------------------------------------\n"));
        BSD_TA_Print(("[60%d1][#E#] RSA Encryption Mode FAILED!\n", xPadding));
        BSD_TA_Print(("-------------------------------------\n\n"));
    }
    else
    {
        BSD_TA_Print(("\n-------------------------------------\n"));
        BSD_TA_Print(("[60%d0] RSA Encryption Mode PASSED\n", xPadding));
        BSD_TA_Print(("-------------------------------------\n\n"));
    }

    if (pxTmp1 != NULL)
    {
        free(pxTmp1);
    }

    if (pxTmp2 != NULL)
    {
        free(pxTmp2);
    }

    /*
     * SIGNATURE MODE
     */

    if ((xPadding == BSD_RSA_PKCS1_V1_5_PADDING) || (xPadding == BSD_RSA_NO_PADDING))
    {
        /* Set INPUT Reference */
        switch (xPadding)
        {
        case BSD_RSA_PKCS1_V1_5_PADDING:
            pxRefIn = (TUnsignedInt8*)rsa_padding_0_priv_enc_in;
            pxInput = pxRefIn;
            xInputSize = rsa_padding_0_priv_enc_in_size;
            pxTmp1 = (TUnsignedInt8*)malloc(rsa_padding_0_priv_enc_in_size * sizeof(char));
            pxTmp2 = (TUnsignedInt8*)malloc(rsa_padding_0_priv_enc_in_size * sizeof(char));
            if (pxTmp1 == NULL)
            {
                return BSD_ERROR;
            }

            if (pxTmp2 == NULL)
            {
                return BSD_ERROR;
            }

            pxOutput = pxTmp1;
            memset(pxOutput, 0x0, rsa_padding_0_priv_enc_in_size);
            xE  = rsa_padding_0_pub_dec_exp_e;
            pxN = (TUnsignedInt8*)rsa_padding_0_pub_dec_n;
            xKeySize = rsa_padding_0_pub_dec_n_size;

            break;
        case BSD_RSA_NO_PADDING:
            pxRefIn = (TUnsignedInt8*)rsa_padding_3_priv_enc_in;
            pxInput = pxRefIn;
            xInputSize = rsa_padding_3_priv_enc_in_size;
            pxTmp1 = (TUnsignedInt8*)malloc(rsa_padding_3_priv_enc_in_size * sizeof(char));
            pxTmp2 = (TUnsignedInt8*)malloc(rsa_padding_3_priv_enc_in_size * sizeof(char));
            if (pxTmp1 == NULL)
            {
                return BSD_ERROR;
            }

            if (pxTmp2 == NULL)
            {
                return BSD_ERROR;
            }

            pxOutput = pxTmp1;
            memset(pxOutput, 0x0, rsa_padding_3_priv_enc_in_size);
            xE  = rsa_padding_3_pub_dec_exp_e;
            pxN = (TUnsignedInt8*)rsa_padding_3_pub_dec_n;
            xKeySize = rsa_padding_3_pub_dec_n_size;

            break;
        }

        /* Set INPUT */
        pxInput  = pxOutput;
        pxOutput = pxTmp2;

        bsdStatus = bsdRsaPublicDecrypt (pxInput, pxOutput, pxOutputSize, xE, pxN, xKeySize, xPadding);
        CHECK_ERROR_CODE(bsdStatus);
        BSD_TA_Print(("[60%d0] bsdRsaPublicDecrypt PASSED\n", xPadding));

        /*COMPARE */
        if (memcmp(pxRefIn, pxOutput, xInputSize))
        {
            BSD_TA_Print(("\n-------------------------------------\n"));
            BSD_TA_Print(("[60%d1][#E#] RSA Signature Mode FAILED!\n", xPadding));
            BSD_TA_Print(("-------------------------------------\n\n"));
        }
        else
        {
            BSD_TA_Print(("\n-------------------------------------\n"));
            BSD_TA_Print(("[60%d0] RSA Signature Mode PASSED\n", xPadding));
            BSD_TA_Print(("-------------------------------------\n\n"));
        }

        if (pxTmp1 != NULL)
        {
            free(pxTmp1);
        }

        if (pxTmp2 != NULL)
        {
            free(pxTmp2);
        }
    }

    return bsdStatus;
}

#endif

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION DIGEST                                        */
/*                                                                            */
/******************************************************************************/
#if NOCS3x

TBsdStatus bsd_digest()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TBsdHashContext xContext;
    TUnsignedInt8 *pxMessageChunk;
    TSize xChunkSize;
    TUnsignedInt8 *pxMessageDigest;
    TUnsignedInt8 *pxKey;
    TSize xKeySize;
    TUnsignedInt8* pxTmp1;

    /*TODO: Chipset Manufacturer to instanciate the xContext parameter */

    /* Set INPUT Reference & parameters */
    pxMessageChunk = (TUnsignedInt8*)sha256_in;
    xChunkSize = sha256_in_size;
    pxTmp1 = (TUnsignedInt8*)malloc(sha256_in_size * sizeof(char));
    if (pxTmp1 == NULL)
    {
        return BSD_ERROR;
    }

    pxMessageDigest = pxTmp1;

    /*
     * SHA256
     */
    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[7010] BSD SHA256  \n");
    bsdPrint("-------------------------------------\n\n");

    bsdStatus = bsdSha256Init (&xContext);
    CHECK_ERROR_CODE(bsdStatus);

    bsdStatus = bsdSha256Update (xContext, pxMessageChunk, xChunkSize);
    CHECK_ERROR_CODE(bsdStatus);

    bsdStatus = bsdSha256Final (xContext, pxMessageDigest);
    CHECK_ERROR_CODE(bsdStatus);

    /*COMPARE */
    if (memcmp(sha256_ref, pxMessageDigest, sha256_ref_size))
    {
        BSD_TA_Print(("\n-------------------------------------\n"));
        BSD_TA_Print(("[7011][#E#] SHA256 FAILED!\n"));
        BSD_TA_Print(("-------------------------------------\n\n"));
    }
    else
    {
        BSD_TA_Print(("\n-------------------------------------\n"));
        BSD_TA_Print(("[7010] SHA256 PASSED\n"));
        BSD_TA_Print(("-------------------------------------\n\n"));
    }

    free(pxTmp1);

    /* Set INPUT Reference & parameters */
    pxMessageChunk = (TUnsignedInt8*)hmac_sha256_in;
    xChunkSize = hmac_sha256_in_size;
    pxTmp1 = (TUnsignedInt8*)malloc(hmac_sha256_in_size * sizeof(char));
    if (pxTmp1 == NULL)
    {
        return BSD_ERROR;
    }

    pxMessageDigest = pxTmp1;

    /*
     * HMAC-SHA256
     */
    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[7020] BSD HMAC-SHA256  \n");
    bsdPrint("-------------------------------------\n\n");

    bsdStatus = bsdHmacSha256Init (pxKey, xKeySize, &xContext);
    CHECK_ERROR_CODE(bsdStatus);

    bsdStatus = bsdHmacSha256Update (xContext, pxMessageChunk, xChunkSize);
    CHECK_ERROR_CODE(bsdStatus);

    bsdStatus = bsdHmacSha256Final (xContext, pxMessageDigest);
    CHECK_ERROR_CODE(bsdStatus);

    /*COMPARE */
    if (memcmp(hmac_sha256_ref, pxMessageDigest, hmac_sha256_ref_size))
    {
        BSD_TA_Print(("\n-------------------------------------\n"));
        BSD_TA_Print(("[7021][#E#] HMAC-SHA256 FAILED!\n"));
        BSD_TA_Print(("-------------------------------------\n\n"));
    }
    else
    {
        BSD_TA_Print(("\n-------------------------------------\n"));
        BSD_TA_Print(("[7020] HMAC-SHA256 PASSED\n"));
        BSD_TA_Print(("-------------------------------------\n\n"));
    }

    free(pxTmp1);

    /* DIGEST Module completed */
    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[7000] BSD DIGEST Completed\n");
    bsdPrint("-------------------------------------\n");

    return bsdStatus;
}

#endif

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION PV                                            */
/*                                                                            */
/******************************************************************************/
static HI_U8 BSD_TA_PVIsHex(HI_CHAR *ptr)
{
    int i, strlength;

    strlength = strlen(ptr);

    if (strlength > 2)
    {
        return 0;
    }

    for (i = 0; i < strlength; i++)
    {
        if ((ptr[i] >= '0') && (ptr[i] <= '9'))
        {}
        else if ((ptr[i] >= 'a') && (ptr[i] <= 'f'))
        {}
        else if ((ptr[i] >= 'A') && (ptr[i] <= 'F'))
        {}
        else
        {
            return 0;
        }
    }

    return 1;
}

/*Turn the user's input into HEX*/
HI_U8 BSD_TA_ConvertToHex(HI_CHAR* ptr)
{
    HI_U32 i, strlength;
    HI_U8 u8Value = 0;

    strlength = strlen(ptr);

    for (i = 0; i < strlength; i++)
    {
        if ((ptr[i] >= '0') && (ptr[i] <= '9'))
        {
            u8Value += ptr[i] - '0';
        }
        else if ((ptr[i] >= 'a') && (ptr[i] <= 'f'))
        {
            u8Value += ptr[i] - 'a' + 0x0a;
        }
        else if ((ptr[i] >= 'A') && (ptr[i] <= 'F'))
        {
            u8Value += ptr[i] - 'A' + 0x0a;
        }

        if ((0 == i) && (2 == strlength))
        {
            u8Value <<= 4;
        }
    }

    return u8Value;
}

HI_S32 BSD_TA_ParseBSDcmdline(HI_CHAR*pCmdLine, HI_S32*pArgc, HI_CHAR pArgv[BSD_MAX_ARGS_COUNT][BSD_MAX_ARGS_LEN])
{
    HI_CHAR *ptr = pCmdLine;
    int i;

    while ((*ptr == ' ') && (*ptr++ != '\0'))
    {
        ;
    }

    for (i = strlen(ptr); i > 0; i--)
    {
        if ((*(ptr + i - 1) == 0x0a) || (*(ptr + i - 1) == ' '))
        {
            *(ptr + i - 1) = '\0';
        }
        else
        {
            break;
        }
    }

    for (i = 0; i < BSD_MAX_ARGS_COUNT; i++)
    {
        int j = 0;
        while ((*ptr == ' ') && (*ptr++ != '\0'))
        {
            ;
        }

        while ((*ptr != ' ') && (*ptr != '\0') && (j < BSD_MAX_ARGS_LEN))
        {
            pArgv[i][j++] = *ptr++;
        }

        pArgv[i][j] = '\0';
        if ('\0' == *ptr)
        {
            i++;
            break;
        }
    }

    *pArgc = i;

    return HI_SUCCESS;
}

HI_S32 BSD_TA_GetInputValue(HI_U32 length, HI_U8 *pxValue)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 offset = 0;
    HI_CHAR bsdCmdLine[BSD_MAX_CMDLINE_LEN];
    HI_S32 bsdArgc;
    HI_CHAR bsdArgv[BSD_MAX_ARGS_COUNT][BSD_MAX_ARGS_LEN];

    offset = 0;
    bsdCmdLine[0] = (HI_CHAR)fgetc(stdin);
    printf("%c", bsdCmdLine[0]);
    while (bsdCmdLine[offset++] != '\r')
    {
        bsdCmdLine[offset] = (HI_CHAR)fgetc(stdin);
        printf("%c", bsdCmdLine[offset]);
    }

    if (offset != 1)
    {
        bsdCmdLine[offset - 1] = '\0';
    }

    printf("\n");

    s32Ret = BSD_TA_ParseBSDcmdline(bsdCmdLine, &bsdArgc, bsdArgv);

    if ((HI_SUCCESS != s32Ret) || (length != 4))
    {
        return HI_FAILURE;
    }

    for (offset = 0; offset < length; offset++)
    {
        if (!BSD_TA_PVIsHex(bsdArgv[offset]))
        {
            return HI_FAILURE;
        }

        pxValue[offset] = BSD_TA_ConvertToHex(bsdArgv[offset]);
    }

    return s32Ret;
}

TBsdStatus bsd_pv()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    HI_S32 Ret = HI_SUCCESS;
    char sel;
    TBsd4BytesVector xMarketSegmentId;
    TBsd4BytesVector xStbCaSn;
    unsigned int MSID = 0, STBCASN = 0;

    /*
     * WARNING! Set PV Value
     */
    BSD_TA_Print(("\n-------------------------------------\n"));
    BSD_TA_Print(("!!!WARN: Setting PV MSID & StbCaSn  \n"));
    BSD_TA_Print(("-------------------------------------\n"));
    BSD_TA_Print(("- Press 'c' to continue  "));
    sel = (char)fgetc(stdin);
    BSD_TA_Print(("\n"));

    BSD_TA_Print(("-------------------------------------\n\n"));
    if (sel == 'c')
    {
        BSD_TA_Print(("- Format 0x78 AB CD EF\n"));
        BSD_TA_Print(("- Enter MSID    : 0x"));
        Ret = BSD_TA_GetInputValue(4, xMarketSegmentId);
        if (HI_SUCCESS != Ret)
        {
            BSD_TA_Print(("Market Segment ID invalid!\n"));
            return BSD_ERROR;
        }
        BSD_TA_Print(("- Enter StbCaSn : 0x"));
        Ret = BSD_TA_GetInputValue(4, xStbCaSn);
        if (HI_SUCCESS != Ret)
        {
            BSD_TA_Print(("STBCASN invalid!\n"));
            return BSD_ERROR;
        }

        //     ConvertInt32To4BytesArray( MSID, xMarketSegmentId );
        //     ConvertInt32To4BytesArray( STBCASN, xStbCaSn );

        bsdStatus = bsdSetMarketSegmentId (xMarketSegmentId);
        CHECK_ERROR_CODE(bsdStatus);
        BSD_TA_Print(("Market Segment Id Set successfully!\n"));

        bsdStatus = bsdSetStbCaSn (xStbCaSn);
        CHECK_ERROR_CODE(bsdStatus);
        BSD_TA_Print(("STBCASN Set successfully!\n"));

        /* PV Module completed */
        BSD_TA_Print(("\n-------------------------------------\n"));
        BSD_TA_Print(("[8000] BSD PV (MSID&STBCASN) Completed\n"));
        BSD_TA_Print(("[8000] !!Warn:Perform a RESET: -1- \n"));
        BSD_TA_Print(("-------------------------------------\n"));
        return bsdStatus;
    }
}

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION RNG                                            */
/*                                                                            */
/******************************************************************************/
#if NOCS3x

TBsdStatus bsd_rng()
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    unsigned int nbbytes;
    unsigned int i, j;
    TSize xNumOfBytes;
    TUnsignedInt8 *  pxRandomBytes;
    TUnsignedInt8 *  pxTmp1;

    /*
     * WARNING! Set PV Value
     */
    BSD_TA_Print(("\n-------------------------------------\n"));
    BSD_TA_Print(("- RNG  \n"));
    BSD_TA_Print(("-------------------------------------\n"));
    BSD_TA_Print(("- How many Random bytes:  "));
    scanf("%d", &nbbytes);
    BSD_TA_Print(("-------------------------------------\n\n"));
    if (nbbytes > 1024)
    {
        nbbytes = 1100;
    }                                  /* malloc() protection */

    xNumOfBytes = nbbytes;
    pxTmp1 = (TUnsignedInt8*)malloc(xNumOfBytes * sizeof(char));
    if (pxTmp1 == NULL)
    {
        return BSD_ERROR;
    }

    pxRandomBytes = pxTmp1;

    for (i = 0; i < 40; i++)
    {
        bsdGenerateRandomBytes(xNumOfBytes, pxRandomBytes);
        CHECK_ERROR_CODE(bsdStatus);
        BSD_TA_Print(("-RNG%d- 0x", i));
        for (j = 0; j < xNumOfBytes; j++)
        {
            BSD_TA_Print(("%02x", pxRandomBytes[j]));
        }

        BSD_TA_Print(("\n"));
    }

    /* DIGEST Module completed */
    bsdPrint("\n-------------------------------------\n");
    bsdPrint("[9000] BSD RNG Completed\n");
    bsdPrint("-------------------------------------\n");
    return bsdStatus;
}

#endif

#if NOCS3x

void certGetNuid(TUnsignedInt32 *xpNuid)
{
    TCertFunctionTable *pFt;
    TCertStatus certStatus = CERT_NO_ERROR;
    TCertResourceHandle xResourceHandle;
    TCertCommand pxCommands[2];
    size_t processedCmds;

    memset(pxCommands, 0, sizeof(TCertCommand) * 2);
    memcpy(pxCommands[0].opcodes, "\x04\x01\x00\x01", 4);/* GPD */
    memcpy(pxCommands[1].opcodes, "\x04\x04\x00\x01", 4);
    pxCommands[0].timeout = CERT_TIMEOUT_DEFAULT;
    pxCommands[1].timeout = CERT_TIMEOUT_DEFAULT;

    pFt = certGetFunctionTable();
    if (pFt == NULL)
    {
        bsdPrint("[#E#] could not get CERT function table\n");
        exit(10);
    }

    certStatus = pFt->certLock(&xResourceHandle);
    if (certStatus != CERT_NO_ERROR)
    {
        bsdPrint("[#E#] certLock error.\n");
        exit(11);
    }

    certStatus = pFt->certExchange(xResourceHandle, 2, pxCommands, &processedCmds);
    if (certStatus != CERT_NO_ERROR)
    {
        BSD_TA_Print(("[#E#] certExchange failed on GPD command %d.\n", processedCmds + 1));
        exit(12);
    }

    certStatus = pFt->certUnlock(xResourceHandle);
    if (certStatus != CERT_NO_ERROR)
    {
        bsdPrint("[#E#] certUnlock error.\n");
        exit(13);
    }

    /* pxCommands[0].outputData byte 0 to 5 contains the NUID in MSBF */
    if ((pxCommands[0].outputData[0] != 0)
       || (pxCommands[0].outputData[1] != 0))
    {
        *xpNuid = (TUnsignedInt32) - 1;
    }
    else
    {
        *xpNuid =
            pxCommands[0].outputData[2] << 24 |
            pxCommands[0].outputData[3] << 16 |
            pxCommands[0].outputData[4] << 8 |
            pxCommands[0].outputData[5];
    }
}

#endif

#if NOCS3x

TBsdStatus cert_activate(TUnsignedInt32 intNuid)
{
    TCertFunctionTable *pFt;
    TCertStatus certStatus = CERT_NO_ERROR;
    TCertResourceHandle xResourceHandle;
    TCertCommand pxCommands[3];
    size_t numCmds, processedCmds;

    int i;

    numCmds = sizeof(act) / sizeof(TCertInputCmd);

    memset(pxCommands, 0, sizeof(pxCommands));
    for (i = 0; i < numCmds; i++)
    {
        memcpy(pxCommands[i].inputData, act[intNuid][i].inputData, 32);
        memcpy(pxCommands[i].opcodes, act[intNuid][i].opcodes, 4);
        pxCommands[i].timeout = act[intNuid][i].timeout;
    }

    pFt = certGetFunctionTable();
    if (pFt == NULL)
    {
        bsdPrint("[#E#] could not get CERT function table\n");
        exit(10);
    }

    certStatus = pFt->certLock(&xResourceHandle);
    if (certStatus != CERT_NO_ERROR)
    {
        bsdPrint("[#E#] certLock error.\n");
        return BSD_ERROR;
    }

    certStatus = pFt->certExchange(xResourceHandle, numCmds, pxCommands, &processedCmds);
    if (certStatus != CERT_NO_ERROR)
    {
        BSD_TA_Print(("[#E#] certExchange failed on ACT command %d.\n", processedCmds + 1));
        return BSD_ERROR;
    }

    certStatus = pFt->certUnlock(xResourceHandle);
    if (certStatus != CERT_NO_ERROR)
    {
        bsdPrint("[#E#] certUnlock error.\n");
        return BSD_ERROR;
    }

    return BSD_NO_ERROR;
}

#endif

#if NOCS3x

TBsdStatus cert_genkey()
{
    TCertFunctionTable *pFt;
    TCertStatus certStatus = CERT_NO_ERROR;
    TCertResourceHandle xResourceHandle;
    TCertCommand pxCommands[4];
    size_t numCmds, processedCmds;
    int i;

    numCmds = sizeof(lcpouk) / sizeof(TCertInputCmd);

    memset(pxCommands, 0, sizeof(lcpouk));
    for (i = 0; i < numCmds; i++)
    {
        memcpy(pxCommands[i].inputData, lcpouk[i].inputData, 32);
        memcpy(pxCommands[i].opcodes, lcpouk[i].opcodes, 4);
        pxCommands[i].timeout = lcpouk[i].timeout;
    }

    pFt = certGetFunctionTable();
    if (pFt == NULL)
    {
        bsdPrint("[#E#] could not get CERT function table\n");
        exit(10);
    }

    certStatus = pFt->certLock(&xResourceHandle);
    if (certStatus != CERT_NO_ERROR)
    {
        bsdPrint("[#E#] certLock error.\n");
        return BSD_ERROR;
    }

    certStatus = pFt->certExchange(xResourceHandle, numCmds, pxCommands, &processedCmds);
    if (certStatus != CERT_NO_ERROR)
    {
        BSD_TA_Print(("[#E#] certExchange failed on ACT command %d.\n", processedCmds + 1));
        return BSD_ERROR;
    }

    certStatus = pFt->certUnlock(xResourceHandle);
    if (certStatus != CERT_NO_ERROR)
    {
        bsdPrint("[#E#] certUnlock error.\n");
        return BSD_ERROR;
    }

    return BSD_NO_ERROR;
}

#endif

#if NOCS3x

TBsdStatus cert_ram2ram_op(unsigned long int intNuid, TUnsignedInt16 xEmi)
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TBsdCipherSession*  pxSession;
    TSize xClearTextKeySize;
    TSize xCipheredContentKeySize;
    TSize xCipheredProtectingKeySize;
    TUnsignedInt8 *pxInput, *pxDecInput, *pxEncInput;
    TUnsignedInt8 *pxOutput, *pxDecRef, *pxEncRef;
    TSize xDataSize;
    TUnsignedInt8 *pxInitVector;
    TSize xInitVectorSize;
    TBsdCipherSession r2rSession;

    /*TODO: Chipset Manufacturer to instanciate the pxKeyPathHandle parameter */

    switch (xEmi)
    {
    case 0x4021:

        /* pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4021_CipheredProtectingKeyL2[intNuid][0]; */
        /* pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4021_CipheredProtectingKeyL1[intNuid][0]; */
        /* pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4021_CipheredContentKey[intNuid][0]; */
        /* pxClearTextKey = (TUnsignedInt8*)&ram2ram_emi_4021_HostKey[intNuid][0]; */
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4021_IVCbc[intNuid][0];
        xDataSize = ram2ram_emi_4021_enc_in_size;
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4021_IVCbc[intNuid][0];
        xInitVectorSize = 8;
        pxDecRef   = (TUnsignedInt8*)ram2ram_emi_4021_dec_ref;
        pxEncRef   = (TUnsignedInt8*)ram2ram_emi_4021_enc_ref;
        pxDecInput = (TUnsignedInt8*)ram2ram_emi_4021_dec_in;
        pxEncInput = (TUnsignedInt8*)ram2ram_emi_4021_enc_in;

        break;
    case 0x4026:

        /* pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4026_CipheredProtectingKeyL2[intNuid][0]; */
        /* pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4026_CipheredProtectingKeyL1[intNuid][0]; */
        /* pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4026_CipheredContentKey[intNuid][0]; */
        /* pxClearTextKey = (TUnsignedInt8*)&ram2ram_emi_4026_HostKey[intNuid][0]; */
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4026_IVCbc[intNuid][0];
        xDataSize = ram2ram_emi_4026_enc_in_size;
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4026_IVCbc[intNuid][0];
        xInitVectorSize = 8;
        pxDecRef   = (TUnsignedInt8*)ram2ram_emi_4026_dec_ref;
        pxEncRef   = (TUnsignedInt8*)ram2ram_emi_4026_enc_ref;
        pxDecInput = (TUnsignedInt8*)ram2ram_emi_4026_dec_in;
        pxEncInput = (TUnsignedInt8*)ram2ram_emi_4026_enc_in;
        break;
    case 0x4041:

        /* pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredProtectingKeyL2[intNuid][0]; */
        /* pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredProtectingKeyL1[intNuid][0]; */
        /* pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4041_CipheredContentKey[intNuid][0]; */
        /* pxClearTextKey = (TUnsignedInt8*)&ram2ram_emi_4041_HostKey[intNuid][0]; */
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4041_IVCbc[intNuid][0];
        xDataSize = ram2ram_emi_4041_enc_in_size;
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4041_IVCbc[intNuid][0];
        xInitVectorSize = 16;
        pxDecRef   = (TUnsignedInt8*)ram2ram_emi_4041_dec_ref;
        pxEncRef   = (TUnsignedInt8*)ram2ram_emi_4041_enc_ref;
        pxDecInput = (TUnsignedInt8*)ram2ram_emi_4041_dec_in;
        pxEncInput = (TUnsignedInt8*)ram2ram_emi_4041_enc_in;
        break;
    case 0x4043:

        /* pxL2CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4043_CipheredProtectingKeyL2[intNuid][0]; */
        /* pxL1CipheredProtectingKey = (TUnsignedInt8*)&ram2ram_emi_4043_CipheredProtectingKeyL1[intNuid][0]; */
        /* pxCipheredContentKey = (TUnsignedInt8*)&ram2ram_emi_4043_CipheredContentKey[intNuid][0]; */
        /* pxClearTextKey = (TUnsignedInt8*)&ram2ram_emi_4043_HostKey[intNuid][0]; */
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4043_IVCbc[intNuid][0];
        xDataSize = ram2ram_emi_4043_enc_in_size;
        pxInitVector = (TUnsignedInt8*)&ram2ram_emi_4043_IVCbc[intNuid][0];
        xInitVectorSize = 16;
        pxDecRef   = (TUnsignedInt8*)ram2ram_emi_4043_dec_ref;
        pxEncRef   = (TUnsignedInt8*)ram2ram_emi_4043_enc_ref;
        pxDecInput = (TUnsignedInt8*)ram2ram_emi_4043_dec_in;
        pxEncInput = (TUnsignedInt8*)ram2ram_emi_4043_enc_in;
        break;
    default:
        break;
    }

    bsdPrint("[50-0]BSD set CERT Ram2Ram Key\n");

    /*
     * Encrypt
     */

    bsdStatus = bsdOpenRam2RamEncryptSession (&r2rSession);
    CHECK_ERROR_CODE(bsdStatus);

    bsdStatus = cert_genkey();
    CHECK_ERROR(cert_genkey, bsdStatus);

    bsdStatus = bsdUseCertKey(r2rSession, xEmi);
    CHECK_ERROR_CODE(bsdStatus);

    pxOutput = (TUnsignedInt8*)malloc(xDataSize * sizeof(char));
    if (pxOutput == NULL)
    {
        return BSD_ERROR;
    }

    pxInput = pxEncInput;

    bsdStatus = bsdSessionEncrypt (r2rSession, pxInput, pxOutput, xDataSize, pxInitVector, xInitVectorSize);
    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    if (memcmp(pxEncRef, pxOutput, xDataSize))
    {
        bsdPrint("[50-0][#E#] R2R Encrypt with CERT Key FAILED\n");
    }
    else
    {
        bsdPrint("[50-0] R2R Encrypt with CERT Key PASSED\n");
    }

    bsdCloseSession(r2rSession);
    free(pxOutput);

    /*
     * Decrypt
     */

    bsdStatus = bsdOpenRam2RamDecryptSession (&r2rSession);
    CHECK_ERROR_CODE(bsdStatus);

    bsdStatus = cert_genkey();
    CHECK_ERROR(cert_genkey, bsdStatus);

    bsdStatus = bsdUseCertKey(r2rSession, xEmi);
    CHECK_ERROR_CODE(bsdStatus);

    pxOutput = (TUnsignedInt8*)malloc(xDataSize * sizeof(char));
    if (pxOutput == NULL)
    {
        return BSD_ERROR;
    }

    pxInput = pxDecInput;

    bsdStatus = bsdSessionDecrypt (r2rSession, pxInput, pxOutput, xDataSize, pxInitVector, xInitVectorSize);
    CHECK_ERROR_CODE(bsdStatus);

    /* COMPARE with Reference */
    if (memcmp(pxDecRef, pxOutput, xDataSize))
    {
        bsdPrint("[50-0][#E#] R2R Decrypt with CERT Key FAILED\n");
    }
    else
    {
        bsdPrint("[50-0] R2R Decrypt with CERT Key PASSED\n");
    }

    bsdCloseSession(r2rSession);
    free(pxOutput);

    return bsdStatus;
}

#endif

/******************************************************************************/
/*                                                                            */
/*                     FUNCTION BSD Test Application MENU                     */
/*                                                                            */
/******************************************************************************/

char bsdPrintMenu(void)
{
    char sel;

    BSD_TA_Print(("\n\n-------------------------------------\n"));
    BSD_TA_Print(("- BSD Test Application MENU         -\n"));
    BSD_TA_Print(("-------------------------------------\n"));
#if NOCS3x
    BSD_TA_Print(("-1- [BSD_1] BSD TA Reset\n"));
#endif
    BSD_TA_Print(("-2- [BSD_2] BSD TA Chipset Information\n"));
    BSD_TA_Print(("-3- [BSD_3] BSD TA Ram2Ram\n"));
#if NOCS3x
    BSD_TA_Print(("-4- [BSD_4] BSD TA Flash Protection\n"));
    BSD_TA_Print(("-5- [BSD_5] BSD TA CERT\n"));
    BSD_TA_Print(("-6- [BSD_6] BSD TA RSA\n"));
    BSD_TA_Print(("-7- [BSD_7] BSD TA Digest\n"));
    BSD_TA_Print(("-8- [BSD_8] BSD TA Persistent Value\n"));
    BSD_TA_Print(("-9- [BSD_9] BSD TA RNG\n"));
#endif
    BSD_TA_Print(("-q- Quit the BSD Application\n"));
    BSD_TA_Print(("-------------------------------------\n"));
    BSD_TA_Print(("-Menu selection:"));
    sel = (char)fgetc(stdin);
    printf("\n");
    BSD_TA_Print(("-------------------------------------\n"));

    return sel;
}

TBsdStatus bsdTestAppMenu(void)
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    char menu_selection = '0';

    while (menu_selection != 'q')
    {
        menu_selection = bsdPrintMenu();
        switch (menu_selection)
        {
#if NOCS3x
        case '1':
            bsdStatus = bsd_reset();
            break;
#endif
        case '2':
            bsdStatus = bsd_chip_information();
            break;
        case '3':
            bsdStatus = bsd_ram2ram();
            break;
#if NOCS3x
        case '4':
            bsdStatus = bsd_flash();
            break;
        case '5':
            bsdStatus = bsd_cert();
            break;
        case '6':
            bsdStatus = bsd_rsa();
            break;
        case '7':
            bsdStatus = bsd_digest();
            break;
        case '8':
            bsdStatus = bsd_pv();
            break;
        case '9':
            bsdStatus = bsd_rng();
            break;
#endif
        default:
            break;
        }
    }

    return bsdStatus;
}

/******************************************************************************/
/*                                                                            */
/*                             MAIN                                           */
/*                                                                            */
/******************************************************************************/

int bsd_TestApplication(void)
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TBsdInitParameters *xInitParameters;
    TBsdTerminateParameters *xTerminateParameters;

    /*TODO: Chipset Manufacturer to instanciate the xInitParameters parameter */
    /*TODO: Chipset Manufacturer to instanciate the xTerminateParameters parameter */
    xInitParameters = HI_NULL;
    xTerminateParameters = HI_NULL;

    /*
     *  Init BSD Driver
     */
    bsdStatus = bsdInitialize(xInitParameters);
    CHECK_ERROR_CODE(bsdStatus);
    BSD_TA_Print(("[INIT] BSD Init completed\n"));

    /*
     *  Launch the BSD Test Application Menu
     */
    bsdTestAppMenu();

    /*
     *  Terminate BSD Driver
     */
    bsdStatus = bsdTerminate(xTerminateParameters);
    CHECK_ERROR_CODE(bsdStatus);

    return 0;
}

/******************************************************************************/
/*                                                                            */
/*                             Generic Routines                               */
/*                                                                            */
/******************************************************************************/

unsigned long int Convert4BytesArrayToInt32( unsigned char *pBytes )
{
    return (unsigned long int)(*pBytes << 24 | *(pBytes + 1) << 16 | *(pBytes + 2) << 8 | *(pBytes + 3));
}

void ConvertInt32To4BytesArray( unsigned long int val, unsigned char *pBytes )
{
    pBytes[0] = (unsigned char)val;
    pBytes[1] = (unsigned char)(val >> 8);
    pBytes[2] = (unsigned char)(val >> 16);
    pBytes[3] = (unsigned char)(val >> 24);
}


int do_nagra_pvprogrammer (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (!strncmp(argv[1], "help", 4))
    {
        HI_ERR_NAGRA ("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    
    if (argc == 1)
    {
#if 0
        return pvprogrammer();
#endif
    }

    return 0;
}

int do_bsd_test_application (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (!strncmp(argv[1], "help", 4))
    {
        HI_ERR_NAGRA ("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    
    if (argc == 1)
    {
        return bsd_TestApplication();
    }

    return 0;
}

U_BOOT_CMD(
	pvprogrammer,	2,	1,	do_nagra_pvprogrammer,
	"pvprogrammer      - Set or Get PV values from OTP\n",
	""
);

U_BOOT_CMD(
	bsd_test_application,	2,	1,	do_bsd_test_application,
	"bsd_test_application      - The BSD Test Application\n",
	""
);

