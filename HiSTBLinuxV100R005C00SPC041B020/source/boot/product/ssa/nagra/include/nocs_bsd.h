/**
**  @file nocs_bsd.h
**
**  @brief
**    Bare Security Driver API to access security features and crypto functions
**    of a NOCS secure chipset in a bare environment.
**
**  $Id: //CAK/components/bsdapi/RELEASE/BSDAPI_3_3_0B1/src/nocs_bsd.h#2 $
**  $Change: 148390 $
**
** REMARK:
**   Comments in this file use special tags to allow automatic API documentation 
**   generation in HTML format, using the GNU-General Public Licensed Doxygen 
**   tool. For more information about Doxygen, please check www.doxygen.org
**
**   Depending on the platform, the CHM file may not open properly if it is 
**   stored on a network drive. So either the file should be moved on a local 
**   drive or add the following registry entry on Windows platform (regedit):\n
**   [HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\HTMLHelp\1.x\\ItssRestrictions] "MaxAllowedZone"=dword:00000003
*/

#ifndef BSD_H
#define BSD_H

/**
 * Defines the version number of the BSD API. Note also that this version number 
 * does not correspond to the version number of the software version. 
 * @warning Do not update these values
*/
#define BSDAPI_VERSION_MAJOR  3
#define BSDAPI_VERSION_MEDIUM 3
#define BSDAPI_VERSION_MINOR  2


/******************************************************************************/
/*                                                                            */
/*                               INCLUDE FILES                                */
/*                                                                            */
/******************************************************************************/
#include "ca_defs.h"


/******************************************************************************/
/*                                                                            */
/*                             OVERALL DOCUMENTATION                          */
/*                                                                            */
/******************************************************************************/

/** @page p_history History
 *
 *   - <b> 3.3.2 2012/12/13 Jean-Luc Bussy</b>
 *     - Restricted NOCS1 API profile to strict minimum
 *
 *   - <b> 3.3.0 2012/06/28 Jean-Luc Bussy</b>
 *     - Added NOCS1.x specific functions bsdEncryptDataWithSecretContentKey() 
 *       and bsdDecryptDataWithSecretContentKey().
 *
 *   - <b> 3.2.4 2012/05/31 Jean-Luc Bussy</b>
 *     - Fixed missing casts in bsdUseRootLevelKey() macro that was causing
 *       compile warnings or even errors depending on the compiler.
 *
 *   - <b> 3.2.3 2012/05/25 Jean-Luc Bussy</b>
 *     - Fixed a problem with gIprNotice constant (replaced now by 
 *       #NAGRA_IPR_NOTICE) that was causing a compile error when this file was
 *       included several times.
 *
 *   - <b> 3.2.2 2012/05/15 Jean-Luc Bussy</b>
 *     - Removed unwanted typedef on bsdEncryptFlashProtKey() definition
 *     - Fixed typos
 *
 *   - <b> 3.2.1 2012/05/02 Jean-Luc Bussy</b>
 *     - Removed bsdDecryptDataWithFlashProtRootKey(), 
 *       bsdDecryptDataWithClearTextFlashProtectionKey() and 
 *       bsdEncryptDataWithFlashProtRootKey()
 *     - Added bsdUseFlashProtKey() and bsdEncryptFlashProtKey()
 *     - Added bsdUseCertKey()
 *     - Replaced bsdSetSecretContentKey() function by bsdUseRootLevelKey() macro
 *       so that implementation is no longer required.
 *     - Added implementation details as information for the FlashProtection
 *       and Ram2Ram related functions.
 *     - Added error status BSD_ERROR_NOT_IMPLEMENTED
 *  
 *   - <b> 3.1.0 2012/02/27 11:30:00 Nocs team</b>
 *     - Removed the following RSA functions bsdRsaPrivateEncrypt(), bsdRsaPrivateDecrypt() 
 *       and bsdRsaGenerateKey().
 *     - Removed the bsdEncryptDataWithClearTextFlashProtectionKey() function 
 *     - Removed the bsdGetNUID64CheckNumber() function 
 *     - Added bsdGetNuid() and bsdGetNUIDCheckNumber(). 
 *     - Added bsdSetSecretContentKey().
 *  
 *   - <b> 3.0.0 2012/01/05 11:30:00 Nocs team</b>
 *     - Added reference to ca_defs.h
 *     - Added bsdGetNuid64CheckNumber()
 *     - Removed and replaced TBsdChar, TBsdUnsignedInt8, TBsdUnsignedInt16, TBsdUnsignedInt32, TBsdSize 
 *       with TChar, TUnsignedInt8, TUnsignedInt16, TUnsignedInt32, TSize (declared in ca_defs.h)
 *     - Removed bsdSetClearTextFlashProtectionKey() and bsdSetFlashProtRootKey()
 *     - Updated bsdEncryptDataWithClearTextFlashProtectionKey(), 
 *       bsdDecryptDataWithClearTextFlashProtectionKey(), bsdEncryptDataWithFlashProtRootKey() and 
 *       bsdDecryptDataWithFlashProtRootKey().
 *     - Updated all R2R related functions, notion of open and close R2R sessions.
 *  
 *   - <b> 2.1.0 2011/11/02 11:30:00 Nocs team</b>
 *     - Renamed TChar into TBsdChar
 *     - Added library stddef.h
 *     - Removed SHA1 support
 *  
 *   - <b> 2.0.0 2011/10/07 11:30:00 Nocs team  </b>
 *     - BSD API updated to support NOCS3.0 chipset
 */

/**
 * @mainpage Overview
 *  - @subpage p_preface
 *  - @subpage p_history
 *  - @subpage p_sw_stack
 *  - @subpage p_emi
 *  - @subpage p_flash_prot
 *  - @subpage p_api_profile
 *
 *  <hr>Copyright &copy; 2012 Nagravision. All rights reserved.<br>
 *  CH-1033 Cheseaux, Switzerland<br>
 *  Tel: +41 21 7320311  Fax: +41 21 7320100<br>
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
 *  <b>Security Policy of Nagravision Kudelski Group</b><br>
 *  Any recipient of this document, without exception, is subject to a
 *  Non-Disclosure Agreement (NDA) and access authorization.
 *   
 */

/** @page p_preface Preface
 *
 *  <h2>Purpose</h2>
 *  This document is intended for the manufacturers of MPEG secure chipsets who 
 *  are partners of Nagravision and whose chipset implements security features as 
 *  required  by NOCS requirement specification.
 *
 *  This document specifies the BSD - Bare Security Driver Application Programming 
 *  Interface that exposes security functionalities defined in the NOCS specification 
 *  in Bare mode.
 *  
 *  It is assumed that the reader is familiar with the NOCS concepts, requirements 
 *  and recommendations.
 *
 *  This API shall be implemented by the chipset manufacturers for any new NOCS secure
 *  chipset that is candidate to a Nagra NOCS certification. This API applies
 *  to NOCS1.x and NOCS3 chipset. However, the list of functions to be implemented
 *  depends on the version of NOCS.
 *
 *  <hr><h2>Audience</h2>
 *  This document is intended for MPEG secure chipset manufacturers implementing NOCS 
 *  requirements and candidate to a NOCS certification.
 *  
 *  <hr><h2>References</h2>
 *  [EMI] Nagravision, System Architecture, Encryption Method Indicator Specification, 
 *         Version 1.0.7
 */

/** @page p_sw_stack Software stack context
 *  The figure below depicts the context of the BSD component.
 *  @image html bare_sw_stack.png
 *  @image rtf bare_sw_stack.png
 *
 *  The SoC vendor is in charge of providing a reference implementation of the
 *  BSD API to the STB manufacturer. However, this latter one has the
 *  responsibility of the final implementation of this API on its hardware
 *  platform. In particular, the STB manufacturer may need to modify the
 *  reference implementation in order to control and manage hardware resource
 *  allocation conflicts.
 */

/** @page p_emi Encryption Method Indicator (EMI)
 *  
 *  There are many ways to encrypt data. An encryption algorithm (like AES, 
 *  TDES, ...) with optionally its associated mode of operation (like ECB, CBC 
 *  or CTR …) has, of course, to be chosen but this is often not enough to 
 *  specify unequivocally an encryption method. Indeed, some less obvious 
 *  information like data handling and key formats have also to be well defined 
 *  if interoperability between different platforms shall be ensured. Moreover 
 *  it is impossible to build a pre-defined exhaustive list of parameters 
 *  qualifying explicitly each Encryption Method because the parameters needed 
 *  could be specific to a given Encryption Method only. 
 *  
 *  Therefore, in order to deal with this complexity and to have a mean to 
 *  uniquely identify the way data are encrypted/decrypted, Nagravision 
 *  allocates a unique 16-bits identifier, referred to as EMI, to all referenced 
 *  Encryption Methods (see [EMI]). Each Encryption Method identified by its EMI 
 *  has a dedicated detailed specification. That specification might either be 
 *  an international standard or a Nagravision specification.
 *  
 *  The following table defines the minimum set of cryptographic algorithms and 
 *  chaining modes to be supported along with their corresponding EMIs.
 *
 *  <table class="doxtable">
 *  <tr>
 *  <th>Algorithm</th><th>EMI [hex]</th></tr>
 *  <tr>
 *  <td>AES-128-ECB</td><td>4021</td></tr>
 *  <tr>
 *  <td>AES-128-CBC</td><td>4026</td></tr>
 *  <tr>
 *  <td>TDES-128-ECB</td><td>4041</td></tr>
 *  <tr>
 *  <td>TDES-128-CBC</td><td>4043</td></tr>
 *  </table>
 */

/** @page p_flash_prot Flash Protection Key (FPK)
 *  
 *  This section describes the context of the flash protection key (FPK)
 *  @image html flash_prot.png
 *
 *  @par STB production line
 *  - The cleartext version of the FPK is encrypted with the chipset specific 
 *    flash protection root key. This encryption is performed by means of either
 *    the BSD API bsdEncryptFlashProtKey() function or the SEC API 
 *    TSecEncryptFlashProtKey() function.
 *  - The encrypted version is then programmed in the flash SCS_Params_Area.
 *    Refer to the Chipset Integration Specification for further information 
 *    about this process.
 *    
 *  @par Secure chipset startup
 *  - SCS automatically decrypts the FPK located in the flash SCS_Params_Area. 
 *    This decryption does not use any Nagravision NOCS API.
 *  - The resulting cleartext FPK is copied in RAM and authenticated.
 *  - SCS uses this authenticated FPK to decrypt the boot code, which is the
 *    original purpose of the FPK.
 *
 *  @par Boot context
 *  - The FPK copied in RAM in the previous step is available in the STB boot
 *    context for RAM2RAM encryption/decryption operations through the BSD API
 *    bsdUseFlashProtKey() function.
 *  
 *  @par Application context
 *  - When the boot is completed and the OS is up and running, the FPK 
 *    previously copied in RAM by the SCS remains available to applications for 
 *    RAM2RAM encryption/decryption operations through the SEC API 
 *    TSecUseFlashProtKey() function.
*/

/** @page p_api_profile NOCS3/NOCS1 API Profiles
 *  
 *  The BSD API applies to NOCS1 and NOCS3 chipset certification. However, depending
 *  on the NOCS version, some functions need to be implemented and some others
 *  not. This section defines two profiles to know exactly which functions has
 *  to be implemented for the certification of a NOCS1 or NOCS3 chipset.
 *
 *
 *  <hr><h2>NOCS3 Profile</h2>
 *  The certification of a NOCS3 chipset requires <b>all functions</b> of this 
 *  API to be implemented, except:
 *  - bsdEncryptDataWithSecretContentKey()
 *  - bsdDecryptDataWithSecretContentKey()
 *
 *  <hr><h2>NOCS1 Profile</h2>
 *  The certification of a NOCS1 chipset requires the following functions to be
 *  implemented:
 *  <h3>Initialization</h3>
 *    - bsdInitialize()
 *    - bsdTerminate()
 *    
 *  <h3>Configuration</h3>
 *    - bsdChipsetInformation()         
 *    - bsdGetNuid()                    
 *    - bsdGetDataIntegrityCheckNumber()
 *    
 *  <h3>RAM2RAM</h3>
 *    - bsdEncryptDataWithSecretContentKey()
 *    - bsdDecryptDataWithSecretContentKey()
*/

/******************************************************************************/
/*                                                                            */
/*                              GROUPS DEFINITION                             */
/*                                                                            */
/******************************************************************************/
/** @defgroup driversetting Driver Settings */

/** @defgroup chipinfo Chipset Information */

/** @defgroup debug Debug Print */

/** @defgroup res Chipset Reset */

/** @defgroup flashprot Flash Protection */

/** @defgroup r2r Block Ciphers (RAM2RAM) */

/** @defgroup rsa RSA */

/** @defgroup digest Digest
 *
 *  @defgroup sha256 SHA-256   
 *  @ingroup digest
 *
 *  @defgroup hmacsha256 HMAC-SHA-256
 *  @ingroup digest
*/

/** @defgroup rand  True Random Number Generator  */


/******************************************************************************/
/*                                                                            */
/*                              VERSION TOOL                                  */
/*                                                                            */
/******************************************************************************/

#ifndef BSD_TOOL_VERSION
#define BSD_TOOL_STRINGIFY(s) BSD_TOOL_TOSTRING(s)
#define BSD_TOOL_TOSTRING(s)  #s

#define BSD_TOOL_VERSION_INT(a, b, c) (a<<16 | b<<8 | c)
#define BSD_TOOL_VERSION_DOT(a, b, c) a ##.## b ##.## c
#define BSD_TOOL_VERSION(a, b, c) BSD_TOOL_VERSION_DOT(a, b, c)
#endif

#define BSDAPI_VERSION_INTER BSD_TOOL_VERSION(BSDAPI_VERSION_MAJOR, \
              BSDAPI_VERSION_MEDIUM, \
              BSDAPI_VERSION_MINOR)

#define BSDAPI_VERSION_INT   BSD_TOOL_VERSION_INT(BSDAPI_VERSION_MAJOR, \
            BSDAPI_VERSION_MEDIUM, \
            BSDAPI_VERSION_MINOR)

#define BSDAPI_VERSION_STRING  "BSDAPI_" BSD_TOOL_STRINGIFY(BSDAPI_VERSION_INTER)


/******************************************************************************/
/*                                                                            */
/*                               TYPES & CONSTANTS                            */
/*                                                                            */
/******************************************************************************/

/**
 *  @brief
 *    Define the parameters for the initialization of the Bare Security
 *    Driver.
 *
 *    SBsdInitParameters is a chipset specific structure that has to be defined
 *    in nocs_bsd_impl.h.
*/
typedef struct SBsdInitParameters TBsdInitParameters;

/**
 *  @brief
 *    Define the parameters for the termination of the Bare Security 
 *    Driver.
 *
 *    SBsdTerminateParameters is a chipset specific structure that has to be 
 *    defined in nocs_bsd_impl.h.
*/
typedef struct SBsdTerminateParameters TBsdTerminateParameters;

/**
 *  @brief
 *    32 bits vector.
 *
 *  TBsd4BytesVector defines a 32 bits vector for the
 *  CheckNumber, STBCASN and NUID values. This value can be converted in integer type
 *  according to the platform architecture.
*/
typedef unsigned char TBsd4BytesVector[4];

/**
 *  @brief
 *    64 bits vector.
 *
 *  TBsd8BytesVector defines a 64 bits vector for the NUID 64 bit
 *  value. This value can be converted in integer type
 *  according to the platform architecture.
*/
typedef unsigned char TBsd8BytesVector[8];

/**
 *  @brief
 *    20 characters strings.
 *
 *  Defines a string buffer of 20 characters to be used to return the BSD Software 
 *  version number.
*/
typedef unsigned char TBsd20CharsString[20];

/**
 *  @ingroup r2r
 *
 *  @brief
 *    Pointer to an opaque structure to be defined by the entity in charge of
 *    developing the BSD API and used as a handle for block cipher encryption
 *    and decryption sessions.
 *
 *    SBsdCipherSession has to be defined in nocs_bsd_impl.h.
*/
typedef struct SBsdCipherSession* TBsdCipherSession;

/**
 *  @ingroup sha
 *
 *  @brief
 *    Type used to link a sequence of operations together in a given context
 *    (e.g. SHA init, update and final). It is defined as a pointer to an opaque
 *    structure to be defined by the entity in charge of implementing the API.
 *
 *    SBsdHashContext has to be defined in nocs_bsd_impl.h.
*/
typedef struct SBsdHashContext* TBsdHashContext;

/*
 *  @brief
 *   EXTERNAL_BSD_STATUS_OFFSET could be defined in nocs_bsd_impl.h and so,
 *   allows the chipset manufacturer to define an offset on the lowest status
 *   value returned by the BSD driver to specify its own return status code range.
*/
#ifdef EXTERNAL_BSD_STATUS_OFFSET
#define BSD_STATUS_OFFSET   EXTERNAL_BSD_STATUS_OFFSET
#else
#define BSD_STATUS_OFFSET   0
#endif

/**
 *  @brief
 *    Return values of the BSD functions.
 *
*/
typedef enum
{
  BSD_NO_ERROR = BSD_STATUS_OFFSET,
    /**< The intended operation was executed successfully.
     */
  BSD_ERROR,
    /**< The function terminated abnormally. The intended operation failed.
     */
  BSD_ERROR_DRIVER_ALREADY_INITIALIZED,
    /**<  The BSD is already initialized.
     */
  BSD_ERROR_INVALID_PARAMETERS,
    /**< The passed parameters are invalid.
     */
  BSD_ERROR_NOT_SUPPORTED,
    /**< The passed parameters are not supported.
     */
  BSD_ERROR_BAD_USAGE,
    /**< The usage is not allowed.
     */
  BSD_ERROR_NOT_IMPLEMENTED,
    /**< The function is not implemented
     */
  LAST_BSD_STATUS
    /**< TBsdStatus type upper boundary.
     */
} TBsdStatus;

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Structure containing information about the chipset.
 *
 *  @note
 *    -# When set, the fields must be NULL-terminated ASCII string.*
 */
typedef struct 
{
  unsigned char BSD_CM[4];     
    /**< Chipset manufacturer ( 4 bytes).
     */
  unsigned char BSD_CT[20];    
    /**< Chipset type (20 bytes).
     */
  unsigned char BSD_CUT[20];   
    /**< Chipset cut (20 bytes). 
     */
  unsigned char BSD_EXT[20];   
    /**< Chipset Extension (Persistent Value Configuration) (20 bytes).
     */
} TBsdInfoChipset;

/**
 *  @ingroup rsa
 *
 *  @brief
 *    Padding types used together with RSA.
*/
typedef enum
{
  BSD_RSA_PKCS1_V1_5_PADDING,
   /**<  PKCS #1 V1.5 padding scheme
    */
  BSD_RSA_OAEP_SHA_1_PADDING,
   /**<  OAEP padding scheme based on SHA-1. No label is provided.
    */
  BSD_RSA_OAEP_SHA_256_PADDING,
   /**<  OAEP padding scheme based on SHA-256. No label is provided.
    */
  BSD_RSA_NO_PADDING,
   /**<  No padding
    */
  LAST_BSD_RSA_PADDING
   /**<  Internal use only
    */
}TBsdRsaPadding;


/**
 *  @ingroup r2r
 *
 *  @brief
 *    Nagravision IPR notice. This constant is used in the macro
 *    #bsdUseRootLevelKey().
*/
#define NAGRA_IPR_NOTICE  \
  "\xA9\x32\x30\x31\x31\x4E\x61\x67\x72\x61\x76\x69\x73\x69\x6F\x6E"


/******************************************************************************/
/*                                                                            */
/*                             FUNCTION PROTOTYPES                            */
/*                                                                            */
/******************************************************************************/

/**
 *  @ingroup driversetting
 *
 *  @brief
 *    Initialize the BSD.
 *
 *  @applies
 *    NOCS1 and NOCS3
 *
 *  @param[in]  pxInitParameters
 *                Pointer to the structure including the initialization
 *                parameters of the Bare Security Driver.
 *
 *  @retval   BSD_NO_ERROR
 *              Initialization was completed successfully.
 *  @retval   BSD_ERROR_DRIVER_ALREADY_INITIALIZED
 *              BSD has been already initialized.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              Structure pointed to by pxInitParameters is invalid or corrupted.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function has to be called once before any other function of the BSD
 *  API. This function initializes the software module that implements the API,
 *  which can in turn initialize and configure hardware.
 *
 *  pxInitParameters is pointer to a chipset specific structure. It has to be
 *  set prior to any initialization of BSD driver. In the case where external
 *  drivers would be used by the BSD, these drivers shall be initialized outside
 *  the BSD and before any calls to the BSD API.
*/
TBsdStatus bsdInitialize
(
  TBsdInitParameters*  pxInitParameters
);

/**
 *  @ingroup driversetting
 *
 *  @brief
 *    Terminate and clean BSD internal structures.
 *
 *  @applies
 *    NOCS1 and NOCS3
 *
 *  @param[in]  pxTerminateParameters
 *                Pointer to the structure including termination parameters of 
 *                the Bare Security Driver.
 *
 *  @retval   BSD_NO_ERROR
 *              Termination was completed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              Structure pointed to by pxTerminateParameters is invalid or
 *              corrupted.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function terminates operations of the software that implements the BSD
 *  API. Among others, the function shall free all resources previously
 *  allocated by the BSD API.
*/
TBsdStatus bsdTerminate
(
  TBsdTerminateParameters*  pxTerminateParameters
);

/**
 *  @ingroup driversetting
 *
 *  @brief
 *    Return the BSD API version
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]     pxBsdApiVersion
 *                   32 bit value where the function has to write the BSD 
 *                   driver API version using the macro BSDAPI_VERSION_INT.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              xBsdApiVersion parameter is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function returns the version of the BSD API.  
 *  Use the macro BSDAPI_VERSION_INT to assign the right value.
*/
TBsdStatus bsdGetApiVersion
(
  TUnsignedInt32*  pxBsdApiVersion
);

/**
 *  @ingroup driversetting
 *
 *  @brief
 *    Return the current version of the BSD
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]     xSoftwareVersion
 *                   20 characters string where the function has to write the
 *                   software version string.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              xSoftwareVersion parameter is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function returns the version of the BSD. It is
 *  represented as a character string, the internal format being chipset
 *  manufacturer dependent. However, the length of the string is fixed at 20
 *  characters. The string must be terminated by the character '\\0'.
*/
TBsdStatus bsdGetSoftwareVersion
(
  TBsd20CharsString  xSoftwareVersion
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Read the chip Nagra Unique Id
 *
 *  @applies
 *    NOCS1 and NOCS3
 *
 *  @param[out]     xNuid
 *                   4-bytes vector where the function has to write the NUID.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              Address of xNuid is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function returns the NagraUniqueID32 (NUID) that
 *  is programmed into the on-chip non-modifiable memory by the chipset
 *  manufacturer.
*/
TBsdStatus bsdGetNuid
(
  TBsd4BytesVector xNuid
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Read the chip Nagra Unique Id (64 bits)
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]     xNuid64
 *                   8-bytes vector where the function has to write the NUID.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              Structure pointed to by xNuid64 is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function returns the NagraUniqueID64 (NUID) that
 *  is programmed into the on-chip non-modifiable memory by the chipset
 *  manufacturer.
*/
TBsdStatus bsdGetNuid64
(
  TBsd8BytesVector xNuid64
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Write the STBCASN of the chipset.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]     xStbCaSn
 *                   4-bytes vector to which the STBCASN is requested to
 *                   be set.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              xStbCaSn parameter is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function allows the client software to set the STBCASN. The
 *  input is a MSBF 4-bytes vector.
*/
TBsdStatus bsdSetStbCaSn
(
 const TBsd4BytesVector xStbCaSn
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Read the STBCASN of the chipset.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]     xStbCaSn
 *                   4-bytes vector to which the STBCASN is requested to
 *                   be set.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              xStbCaSn parameter is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function allows the client software to read the STBCASN. The
 *  output is a MSBF 4-bytes vector.
*/
TBsdStatus bsdGetStbCaSn
(
  TBsd4BytesVector xStbCaSn
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Write the chipset market segment ID.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]     xMarketSegmentId
 *                   4-bytes vector to which the MarketSegmentId is requested to
 *                   be set.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              The passed parameter is invalid.
 *  @retval   BSD_ERROR
 *              The market segment ID has already been set to a different value
 *              and has been locked.
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function allows the client software to set a value into the
 *  MarketSegmentId. The MarketSegmentId is defined as a MSBF 4-bytes vector.<br>
 *  If the MarketSegmentId is already set and the client software tries to set
 *  it to a different value, the function shall return BSD_ERROR.
*/
TBsdStatus bsdSetMarketSegmentId
(
  const TBsd4BytesVector xMarketSegmentId
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Read the market segment ID of the chipset.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]     xMarketSegmentId
 *                   4-bytes vector where the function has to write the
 *                   MarketSegmentId.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              The passed parameter is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function allows the client software to read the MarketSegmentId. The
 *  output is a MSBF 4-bytes vector.
*/
TBsdStatus bsdGetMarketSegmentId
(
  TBsd4BytesVector xMarketSegmentId
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Return the NUID check number of the chipset.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]     xNUIDCheckNumber
 *                   4-bytes vector where the function has to write the NUID
 *                   Check Number.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function returns NUID Check Number, as requested by the Nagravision
 *  NUID Reporting procedure. The computation of NUID Check Number is
 *  chipset-specific, and is the result of a computation involving the
 *  NUID64 and the Master Root Key. 
 *  
 *  In case of NOCS1.x implementation, the computation of the NUID check number 
 *  involves the R2R-RootKey128, the NagraUniqueID32 as well as the 
 *  RAM2RAM Crypto Engine. As there may be several ways of configuring the 
 *  RAM2RAM Crypto Engine and its Key Ladder to compute the NUID Check Number,
 *  Nagravision and the chipset manufacturer will commonly agree during the 
 *  NOCS certification process of a chipset on how to compute the NUID Check 
 *  Number for this secure chipset. This implementation will be detailed in the 
 *  "Security API Implementation Proposal" document. 
*/
TBsdStatus bsdGetNUIDCheckNumber
(
  TBsd4BytesVector xNUIDCheckNumber
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Return the Data Integrity check number of the chipset.
 *
 *  @applies
 *    NOCS1 and NOCS3
 *
 *  @param[in]      pxData
 *                   Pointer to the location where the function has to 
 *                   read the Data used to compute the check number.
 *  @param[in]      xDataSize
 *                   Size in bytes of the Data.
 *
 *  @param[out]     xDataIntegrityCheckNumber
 *                   4-bytes vector where the function has to write the Data 
 *                   Integrity Check Number.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              The passed parameter is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function returns the Data Integrity Check Number. The computation of 
 *  Data Integrity Check Number is chipset-specific, and is the result of a 
 *  computation involving the R2R-RootKey128, a data as well as the RAM2RAM 
 *  Crypto Engine. As there may be several ways of configuring the RAM2RAM 
 *  Crypto Engine and its Key Ladder to compute the Data Integrity Check Number,
 *  Nagravision and the chipset manufacturer will commonly agree during the NOCS
 *  certification process of a chipset on how to compute the Data Integrity 
 *  Check Number for this secure chipset. Refer to the Security Driver Implementation
 *  Proposal document [2] for more details about this computation.<br>
 *  The size in byte of xDataSize shall be defined between 1 and 16 bytes. 
 *  Otherwise, the function shall return BSD_ERROR_INVALID_PARAMETERS.
*/
TBsdStatus bsdGetDataIntegrityCheckNumber
(
  const TUnsignedInt8*    pxData,
        TUnsignedInt32    xDataSize,
        TBsd4BytesVector  xDataIntegrityCheckNumber
);

/**
 *  @ingroup chipinfo
 *
 *  @brief
 *    Read the chipset information.
 *
 *  @applies
 *    NOCS1 and NOCS3
 *
 *  @param[out]     pxInfoChipset
 *                   Pointer to the structure containing all the chipset information.  
 *                   
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              At least one of the passed parameters is invalid.
 *  @retval   BSD_ERROR
 *              The function terminated abnormally.
 *              The intended operation failed.
 *
 *  This function gives the information about the chipset. The information is 
 *  divided into several sub-categories defined in a structure:
 *  - Chipset manufacturer: defines from which chipset manufacturer the 
 *  chipset is produced.
 *  - Chipset type: defines the chipset model.
 *  - Chipset cut: defines the version of the die.
 *  - Chipset extension: defines the extension of the chipset.
 *  .
 *  All these fields are strings with a maximum characters length (defined in 
 *  the structure) and ended with the character '\\0'. The cut shall be retrieved
 *  by a hardware mean. In other words, the chipset shall contain a register 
 *  where the API can read this information. For the other fields, it is 
 *  recommended to have hardware means to retrieve the information. If this is 
 *  not possible this information can be hardcoded into the function.
*/
TBsdStatus bsdChipsetInformation
(
  TBsdInfoChipset*  pxInfoChipset
);

/******************************************************************************/
/*                                                                            */
/*                             DEBUG PRINT                                    */
/*                                                                            */
/******************************************************************************/

/**
 *  @ingroup debug
 *
 *  @brief
 *    This function sends a string to the debug link so as to print debug
 *    messages on a terminal.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in] pxString
 *               Null-terminated string to be displayed through the debug link.
 *
 *  @retval   BSD_NO_ERROR
 *              The intended operation was executed successfully.
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *              The passed parameters is invalid.<br>
 *  @retval   BSD_ERROR
 *              The function terminated abnormally.
 *              The intended operation failed.
 *  @note
 *    -# This function shall be able to handle strings at most 1024 characters
 *       long.
*/
TBsdStatus bsdPrint
(
  const TChar*  pxString
);



/******************************************************************************/
/*                                                                            */
/*                               FLASH PROTECTION                             */
/*                                                                            */
/******************************************************************************/


/**
 *  @ingroup flashprot
 *
 *  @brief
 *    This function encrypts the global flash protection key with the chipset
 *    specific flash protection root key.
 *
 *    This function is intended to be used on the STB production line to       
 *    encrypt the flash protection key before programming it in the flash      
 *    SCS_Params_Area. Refer to the "Secure Chipset Integration Specification" 
 *    of the chipset family concerned for further information about the process
 *    related to this key.                                                     
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  pxInput
 *    Buffer allocated by the caller containing the flash protection key to be
 *    encrypted.
 *
 *  @param[out]  pxOutput
 *    Buffer allocated by the caller where to write the encrypted flash protection
 *    key.
 *
 *  @param[in]  xSize
 *    Size in bytes of the flash protection key to be encrypted. It is equal to
 *    16 bytes for the time being.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    This status is returned if:
 *    - pxInput and/or pxOutput is NULL
 *    - xSize is different from 16
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    - The flash protection key is automatically decrypted, copied in RAM and
 *      authenticated by the secure chipset startup process (see [NOCS3REQ]). 
 *      This key must be then available for RAM2RAM operations from the boot 
 *      context through BSD API bsdUseFlashProtKey() and after the OS is up and 
 *      running through the SEC API (refer to the SEC API documentation for 
 *      further information)
*/   
TBsdStatus bsdEncryptFlashProtKey
(
  const TUnsignedInt8*     pxInput,
        TUnsignedInt8*     pxOutput,
        size_t              xSize
);



/******************************************************************************/
/*                                                                            */
/*                                    RAM2RAM                                 */
/*                                                                            */
/******************************************************************************/
/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function opens a RAM2RAM encryption session and returns a session
 *    handle that will be used by other functions to set keys and encrypt data.
 *
 *    It is possible to change the encryption key and/or the encryption
 *    algorithm (EMI) during the same session. The type of the encryption key
 *    (clear-text, key ladder protected, flash, CERT) may also change during a 
 *    session.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]  pxSession
 *    Session handle assigned by this function
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    This status is returned if pxSession is NULL.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    -# The pxSession allocation is handled by the driver.
 *    -# In a given open session, only one key is actived at a time and it is 
 *       the latest one set.
 */
TBsdStatus bsdOpenRam2RamEncryptSession
(
  TBsdCipherSession*  pxSession
);

/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function opens a RAM2RAM decryption session and returns a session
 *    handle that will be used by other functions to set keys and decrypt data.
 *
 *    It is possible to change the decryption key and/or the decryption
 *    algorithm (EMI) during the same session. The type of the decryption key
 *    (clear-text, key ladder protected, flash, CERT) may also change during a 
 *    session.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]  pxSession
 *    Session handle assigned by this function
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    This status is returned if pxSession is NULL.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    -# The pxSession allocation is handled by the driver.
 *    -# In a given open session, only one key is actived at a time and it is 
 *       the latest one set.
 */
TBsdStatus bsdOpenRam2RamDecryptSession
(
  TBsdCipherSession*  pxSession
);

/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function closes a block cipher encryption or decryption session.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  xSession
 *    Handle of the session to be closed
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    This status is returned if xSession is NULL or does not correspond to a
 *    valid session.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    -# The xSession deallocation is handled by the driver.
 */
TBsdStatus bsdCloseSession
(
  TBsdCipherSession  xSession
);

/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function sets a clear-text key in a RAM2RAM block cipher engine for
 *    encryption or decryption operations.
 *    
 *    The type of operation (encryption or decryption) is defined at session 
 *    opening (bsdOpenRam2RamEncryptSession or bsdOpenRam2RamDecryptSession). 
 *    The EMI provided as input parameter clearly identifies the cryptographic 
 *    algorithm and chaining mode. At this stage the driver should have all the 
 *    information required to allocate hardware resources.
 *
 *    EMI to be supported covers TDES and AES algorithms with ECB and CBC
 *    block cipher modes of operation. It corresponds to the following EMIs as
 *    defined in [EMI]:
 *      - 0x4021
 *      - 0x4026
 *      - 0x4041
 *      - 0x4043    
 *
 *  @applies
 *    NOCS3 only
 *
 *  @pre
 *    TBsdOpenRam2RamEncryptSession() or TBsdOpenRam2RamDecryptSession() must be 
 *    called before calling this function.
 *
 *  @param[in]  xSession
 *    Handle of the session
 *
 *  @param[in]  xEmi
 *    Identifier of the cryptographic algorithm (TDES, AES) and block
 *    cipher modes (ECB, CBC) to be used by the crypto engine.<br>
 *
 *  @param[in]  xClearTextKeySize
 *    Size in bytes of the clear-text key:
 *    - TDES: 2*8=16 bytes. TDES keying option is part of the EMI definition. 
        Today, keying option 2 (K1=K3) is required only.
 *    - AES: 16 bytes
 *
 *  @param[in]  pxClearTextKey
 *    Buffer containing the clear-text key. The buffer is allocated by the
 *    caller. In case of TDES keying option 2, K1 and K2 are provided as follows:
 *    - K1=pxClearTextKey[0:7]
 *    - K2=pxClearTextKey[8:15]
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    An invalid parameter is provided:
 *    - NULL or unknown session
 *    - NULL clear-text key pointer
 *    - Size of key does not match the crypto algorithm defined by the EMI
 *
 *  @retval  BSD_ERROR_NOT_SUPPORTED
 *    The EMI given as input parameter is not supported.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    This function may be called at any time during a given session to change
 *    the EMI and/or key associated to this session. A session is related to one
 *    single key at a time, the latest one set.
 */
TBsdStatus bsdSetClearTextKey
(
        TBsdCipherSession   xSession,
        TUnsignedInt16      xEmi,
        TSize               xClearTextKeySize,
  const TUnsignedInt8*     pxClearTextKey
);

/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function sets protected keys required by the 2-level key ladder  
 *    associated to a RAM2RAM block cipher engine for encryption or decryption
 *    operations.
 *    
 *    The type of operation (encryption or decryption) is defined at session 
 *    opening (bsdOpenRam2RamEncryptSession or bsdOpenRam2RamDecryptSession). 
 *    The EMI provided as input parameter clearly identifies the cryptographic 
 *    algorithm and chaining mode. At this stage the driver should have all 
 *    information required to allocate hardware resources.
 *
 *    EMI to be supported covers TDES and AES algorithms with ECB and CBC
 *    block cipher modes of operation. It corresponds to the following EMIs as 
 *    defined in [EMI]:
 *      - 0x4021
 *      - 0x4026
 *      - 0x4041
 *      - 0x4043
 *
 *    Beware that keys provided to the key ladder are all protected in TDES-ECB 
 *    keying option 2 in a NOCS chipset. This key ladder algorithm must not be 
 *    confused with the algorithm signaled by the EMI used for RAM2RAM 
 *    operations.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @pre
 *    bsdOpenRam2RamEncryptSession() or bsdOpenRam2RamDecryptSession() must be 
 *    called before calling this function.
 *
 *  @param[in]  xSession
 *    Handle of the session
 *
 *  @param[in]  xEmi
 *    Identifier of the cryptographic algorithm (TDES, AES) and block
 *    cipher modes (ECB, CBC) to be used by the crypto engine.<br>
 *
 *  @param[in]  xCipheredContentKeySize
 *    Size in bytes of the ciphered content key:
 *    - TDES keying option 2: 2*8=16 bytes. The keying option is signaled in
 *      the EMI. However, today only keying option 2 (K1=K3) is supported.
 *    - AES: 16 bytes
 *
 *  @param[in]  pxCipheredContentKey
 *    Buffer, allocated by the caller, containing the ciphered content key. It
 *    is equal to CipheredContentKey=TDES(ContentKey, L1ProtKey).
 *
 *  @param[in]  xCipheredProtectingKeySize
 *    Size in bytes of the intermediate level protecting keys used within the 
 *    key ladder. It is always equal to 16 bytes for NOCS chipset.
 *
 *  @param[in]  pxL1CipheredProtectingKey
 *    Buffer, allocated by the caller, containing the first ciphered protecting 
 *    key fed into the key ladder. It is equal to 
 *    L1CipheredProtKey=TDES(L1ProtKey, L2ProtKey).
 *
 *  @param[in]  pxL2CipheredProtectingKey
 *    Buffer, allocated by the caller, containing the second ciphered protecting 
 *    key fed into the key ladder. It is equal to 
 *    L2CipheredProtKey=TDES(L2ProtKey, R2rRootKey) where R2rRootKey is the 
 *    RAM2RAM crypto engine root key.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    An invalid parameter is provided:
 *    - NULL or unknown session
 *    - NULL key pointers
 *    - Size of content key does not match the crypto algorithm defined by the EMI
 *    - Unsupported protecting key size
 *
 *  @retval  BSD_ERROR_NOT_SUPPORTED
 *    The EMI given as input parameter is not supported.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    - This function may be called at any time during a given session to change
 *      the EMI and/or key associated to this session. A session is related to 
 *      one single key at a time, the latest one set.
 *      and pxL2CipheredProtectingKey must be ignored.
*/
TBsdStatus bsdSet2LevelProtectedKey
(
        TBsdCipherSession  xSession,
        TUnsignedInt16     xEmi,
        TSize              xCipheredContentKeySize,
  const TUnsignedInt8*    pxCipheredContentKey,
        TSize              xCipheredProtectingKeySize,
  const TUnsignedInt8*    pxL1CipheredProtectingKey,
  const TUnsignedInt8*    pxL2CipheredProtectingKey
);


/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function uses the key ladder root level key associated to a RAM2RAM 
 *    block cipher engine for encryption or decryption operations.
 *
 *    The type of operation (encryption or decryption) is defined at session 
 *    opening (bsdOpenRam2RamEncryptSession or bsdOpenRam2RamDecryptSession). 
 *    The EMI provided as input parameter clearly identifies the cryptographic 
 *    algorithm and chaining mode. At this stage the driver should have all the 
 *    information required to allocate hardware resources.
 *
 *    EMI to be supported covers TDES and AES algorithms with ECB and CBC
 *    block cipher modes of operation. It corresponds to the following EMIs as 
 *    defined in [EMI]:
 *      - 0x4021
 *      - 0x4026
 *      - 0x4041
 *      - 0x4043
 *
 *    This function is actually a macro that uses bsdSet2LevelProtectedKey() with
 *    constant parameters.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @pre
 *    TBsdOpenRam2RamEncryptSession() or TBsdOpenRam2RamDecryptSession() must be 
 *    called before calling this function.
 *
 *  @param[in]  xSession
 *    Handle of the session
 *
 *  @param[in]  xEmi
 *    Identifier of the cryptographic algorithm (TDES, AES) and block
 *    cipher modes (ECB, CBC) to be used by the crypto engine.<br>
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    An invalid parameter is provided:
 *    - NULL or unknown session
 *
 *  @retval  BSD_ERROR_NOT_SUPPORTED
 *    The EMI given as input parameter is not supported.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    -# This function may be called at any time during a given session to change
 *       the EMI and/or key associated to this session. A session is related to 
 *       one single key at a time, the latest one set.
 */
#define bsdUseRootLevelKey(xSession, xEmi) \
( \
  bsdSet2LevelProtectedKey( \
  (xSession), (xEmi), 16, (const TUnsignedInt8*)NAGRA_IPR_NOTICE, 16, (const TUnsignedInt8*)NAGRA_IPR_NOTICE, (const TUnsignedInt8*)NAGRA_IPR_NOTICE) \
)


/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function uses the flash protection key for RAM2RAM operations.
 *
 *    The type of operation (encryption or decryption) is defined at session
 *    opening (bsdOpenRam2RamEncryptSession or bsdOpenRam2RamDecryptSession).
 *    The EMI provided as input parameter clearly identifies the cryptographic 
 *    algorithm and chaining mode. At this stage the driver should have all the 
 *    information required to allocate hardware resources.
 *
 *    EMI to be supported covers TDES and AES algorithms with ECB and CBC
 *    block cipher modes of operation. It corresponds to the following EMIs as 
 *    defined in [EMI]:
 *      - 0x4021
 *      - 0x4026
 *      - 0x4041
 *      - 0x4043
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  xSession
 *    Handle of the session
 *
 *  @param[in]  xEmi
 *    Identifier of the cryptographic algorithm (TDES, AES) and block
 *    cipher modes (ECB, CBC) to be used by the crypto engine.<br>
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    This status is returned if pxSession is NULL or does not correspond to a
 *    valid session.
 *
 *  @retval  BSD_ERROR_NOT_SUPPORTED
 *    The EMI given as input parameter is not supported.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    - This function may be called at any time during a given session to change
 *      the EMI and/or key associated to this session. A session is related to 
 *      one single key at a time, the latest one set.
 *    - This function must rely on the authenticated cleartext instance of the 
 *      flash protection key made available by the secure chipset startup 
 *      process. It must never access the original flash protection key located 
 *      in the flash SCS_Params_Area.
 *
 *  @see bsdEncryptFlashProtKey()
*/
TBsdStatus bsdUseFlashProtKey
(
        TBsdCipherSession   xSession,
        TUnsignedInt16      xEmi
);


/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function uses the CERT key for RAM2RAM operations. It does not
 *    involve any key ladder.
 *
 *    The type of operation (encryption or decryption) is defined at session
 *    opening (bsdOpenRam2RamEncryptSession or bsdOpenRam2RamDecryptSession).
 *    The EMI provided as input parameter clearly identifies the cryptographic
 *    algorithm and chaining mode.
 *
 *    At this stage the driver should have all information required to allocate
 *    a RAM2RAM hardware resource and call CERT API functions
 *    certUseEncryptionKey() or certUseDecryptionKey() in order to fetch the key
 *    and usage rules pending on the CERT bus for RAM2RAM operations.
 *
 *    EMI to be supported covers TDES and AES algorithms with ECB and CBC block
 *    cipher modes of operation. It corresponds to the following EMIs as
 *    defined in [EMI]:
 *      - 0x4021
 *      - 0x4026
 *      - 0x4041
 *      - 0x4043    
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  xSession
 *    Handle of the session
 *
 *  @param[in]  xEmi
 *    Identifier of the cryptographic algorithm (TDES, AES) and block cipher 
 *    modes (ECB, CBC) to be used by the RAM2RAM crypto engine.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    This status is returned if pxSession is NULL or does not correspond to a
 *    valid session.
 *
 *  @retval  BSD_ERROR_NOT_SUPPORTED
 *    The EMI given as input parameter is not supported.
 *
 *  @retval  BSD_ERROR_BAD_USAGE
 *    Key usage rules provided by the CERT block are not respected. For instance
 *    the session has been opened for encryption operations and CERT usage rules
 *    allow decryption operations only.
 *
 *  @retval  BSD_ERROR_NOT_IMPLEMENTED
 *    The function is not implemented and is stubbed to avoid link errors.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    -# This function may be called at any time during a given session to change
 *       the EMI and/or key associated to this session. A session is related to 
 *       one single key at a time, the latest one set.
*/
TBsdStatus bsdUseCertKey
(
        TBsdCipherSession   xSession,
        TUnsignedInt16      xEmi
);



/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function performs a RAM2RAM data encryption.
 *    
 *    It encrypts the data contained in pxInput buffer and stores the resulted
 *    encrypted data in pxOutput buffer. It is assumed the encryption content
 *    key has already been set by calling either bsdSetClearTextKey(), 
 *    bsdSet2LevelProtectedKey(), bsdSetSecretContentKey() or bsdUseFlashProtKey()
 *
 *    This function may be called several times during a given session in order
 *    to encrypt a message split in several chunks. If the block cipher modes of
 *    operation requests an initialization vector (IV), the caller is in charge
 *    of the consistency of the IV passed at each chunk.
 *    
 *  @applies
 *    NOCS3 only
 *
 *  @pre
 *    bsdSetClearTextKey(), bsdSet2LevelProtectedKey(), bsdSetSecretContentKey(),
 *    or bsdUseFlashProtKey() must have been called before calling this function.
 *
 *  @param[in]  xSession
 *    Handle of the session
 *
 *  @param[in]  pxInput
 *    Buffer containing the input message to be encrypted. It is allocated by
 *    the caller.
 *
 *  @param[in]  pxOutput
 *    Buffer where to write the encrypted message. It is allocated by
 *    the caller.
 *
 *  @param[in]  xMessageSize
 *    Size in bytes of messages (pxInput, pxOutput) to operate on. It is a
 *    multiple of the cipher block size (8 bytes for DES/TDES and 16 bytes for
 *    AES).
 *
 *  @param[in] pxInitVector
 *    Buffer allocated by the caller containing the initialization vector (IV) 
 *    required by CBC. This cipher mode of operation is part of 
 *    the EMI definition. If no IV is required (ECB mode), pxInitVector is set 
 *    to NULL.<br>
 *    The caller is in charge of providing a correct and consistent IV, even 
 *    in case the data to be encrypted requires several calls to this function.
 *
 *  @param[in] xInitVectorSize
 *    Size in bytes of the initialization vector. It is equal to the cipher 
 *    block size (8 bytes for DES/TDES and 16 bytes for AES). If no IV is 
 *    required (ECB mode), xInitVectorSize is set to zero.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    An invalid parameter is provided:
 *    - NULL or unknown session
 *    - NULL input/output buffers
 *    - Size of message or IV is not a multiple of cipher block size
 *    - Unsupported protecting key size
 *
 *  @retval  BSD_ERROR_BAD_USAGE
 *    This error status must be returned if xSession refers to a decryption
 *    session.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    The engine used to perform such operation should be the Ram2Ram engine
 *    providing the best data throughput. Therefore it is recommended to use this
 *    function to achieve operations requiring high performences.
*/
TBsdStatus bsdSessionEncrypt
(
        TBsdCipherSession   xSession,
  const TUnsignedInt8*     pxInput,
        TUnsignedInt8*     pxOutput,
        TSize              xMessageSize,
  const TUnsignedInt8*     pxInitVector,
        TSize              xInitVectorSize
);

/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function performs a RAM2RAM data decryption.
 *    
 *    It decrypts the data contained in pxInput buffer and stores the resulted
 *    decrypted data in pxOutput buffer. It is assumed the decryption content
 *    key has already been set by calling either bsdSetClearTextKey(), 
 *    bsdSet2LevelProtectedKey(), bsdSetSecretContentKey() or bsdUseFlashProtKey().
 *
 *    This function may be called several times during a given session in order
 *    to decrypt a message split in several chunks. If the block cipher modes of
 *    operation requests an initialization vector (IV), the caller is in charge
 *    of the consistency of the IV passed at each chunk.
 *    
 *  @applies
 *    NOCS3 only
 *
 *  @pre
 *    bsdSetClearTextKey(), bsdSet2LevelProtectedKey(), bsdSetSecretContentKey(),
 *    or bsdUseFlashProtKey() must have been called before calling this function.
 *
 *  @param[in]  xSession
 *    Handle of the session
 *
 *  @param[in]  pxInput
 *    Buffer containing the input message to be decrypted. It is allocated by
 *    the caller.
 *
 *  @param[in]  pxOutput
 *    Buffer where to write the decrypted message. It is allocated by
 *    the caller.
 *
 *  @param[in]  xMessageSize
 *    Size in bytes of messages (pxInput, pxOutput) to operate on. It is a
 *    multiple of the cipher block size (8 bytes for DES/TDES and 16 bytes for
 *    AES).
 *
 *  @param[in] pxInitVector
 *    Buffer allocated by the caller containing the initialization vector (IV) 
 *    required by CBC. This cipher mode of operation is part of 
 *    the EMI definition. If no IV is required (ECB mode), pxInitVector is set 
 *    to NULL.<br>
 *    The caller is in charge of providing a correct and consistent IV, even 
 *    in case the data to be decrypted requires several calls to this function.
 *
 *  @param[in] xInitVectorSize
 *    Size in bytes of the initialization vector. It is equal to the cipher 
 *    block size (8 bytes for DES/TDES and 16 bytes for AES). If no IV is 
 *    required (ECB mode), xInitVectorSize is set to zero.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    An invalid parameter is provided:
 *    - NULL or unknown session
 *    - NULL input/output buffers
 *    - Size of message or IV is not a multiple of cipher block size
 *    - Unsupported protecting key size
 *
 *  @retval  BSD_ERROR_BAD_USAGE
 *    This error status must be returned if xSession refers to a encryption
 *    session.
 *
 *  @retval  BSD_ERROR
 *    Any other errors
 *
 *  @note
 *    The engine used to perform such operation should be the Ram2Ram engine
 *    providing the best data throughput. Therefore it is recommended to use this
 *    function to achieve operations requiring high performences.
*/
TBsdStatus bsdSessionDecrypt
(
        TBsdCipherSession   xSession,
  const TUnsignedInt8*     pxInput,
        TUnsignedInt8*     pxOutput,
        TSize               xMessageSize,
  const TUnsignedInt8*     pxInitVector,
        TSize               xInitVectorSize
);

/******************************************************************************/
/*                                                                            */
/*                                    RSA                                     */
/*                                                                            */
/******************************************************************************/

/**
 *  @ingroup rsa
 *
 *  @brief
 *    This function pads a cleartext input message and RSA encrypts the padded
 *    message with the public key.
 *
 *    Padding schemes supported are OAEP and PKCS #1 v1.5. It is also possible
 *    to use this function with the special padding scheme #BSD_RSA_NO_PADDING.
 *    In such a case, the caller is in charge of the padding so that the
 *    input message size is equal to key size.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  pxInput
 *    Input message to be encrypted.
 *
 *  @param[in]  xInputSize
 *    Size in bytes of input message to operate on. The maximum size depends on
 *    the key size and the padding scheme:
 *    - #BSD_RSA_PKCS1_V1_5_PADDING: maxSize = xKeySize - 11
 *    - #BSD_RSA_OAEP_SHA_1_PADDING: maxSize = xKeySize - 42
 *    - #BSD_RSA_OAEP_SHA_256_PADDING: maxSize = xKeySize - 66
 *    .
 *    If #BSD_RSA_NO_PADDING is used, xInputSize must be equal to xKeySize
 *
 *  @param[out]  pxOutput
 *    Buffer where to write resulting encrypted message. This buffer is always
 *    xKeySize bytes long.
 *
 *  @param[in]  xE
 *    Public RSA exponent e used for the operation. May be any of the following
 *    values: 3, 17, 65537.
 *
 *  @param[in]  pxN
 *    Buffer containing the RSA modulus N used for the operation. Buffer size is
 *    equal to key size.
 *
 *  @param[in]  xKeySize
 *    RSA key size in bytes
 *
 *  @param[in]  xPadding
 *    One of the valid padding types defined in TBsdRsaPadding. If xPadding is 
 *    equal to #BSD_RSA_NO_PADDING, the caller is in charge of padding of the 
 *    input message so that its size is equal to key size.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation. This includes, but not limited to:
 *    - Size of input message not consistent: either too long if a padding
 *      scheme is specified, or different from key size in case of no padding.
 *    - Unsupported exponent
 *    - Unsupported padding scheme
 *    - Unsupported key size
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and the operation could not be completed successfully.
 *
 *  @note
 *    -# All output parameters are allocated by the caller.
 */
TBsdStatus bsdRsaPublicEncrypt
(
  const TUnsignedInt8*      pxInput,
        TSize                xInputSize,
        TUnsignedInt8*      pxOutput,
        TUnsignedInt32       xE,
  const TUnsignedInt8*      pxN,
        TSize                xKeySize,
        TBsdRsaPadding       xPadding
);

/**
 *  @ingroup rsa
 *
 *  @brief
 *    This function RSA decrypts a ciphered input message with the public key
 *    and removes the padding.
 *
 *    Padding schemes supported are OAEP and PKCS #1 v1.5. It is also possible
 *    to use this function with the special padding scheme #BSD_RSA_NO_PADDING.
 *    In such a case, the output message size is equal to key size and the
 *    caller is in charge of removing the padding.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  pxInput
 *    Input message to be decrypted. This buffer is always xKeySize bytes long.
 *
 *  @param[out]  pxOutput
 *    Buffer where to write resulting decrypted message. This buffer is always
 *    xKeySize bytes long although the size of the decrypted message
 *    (pxOutputSize) may be smaller.
 *
 *  @param[out]  pxOutputSize
 *    Size in bytes of output message after decryption and padding removal.
 *
 *  @param[in]  xE
 *    Public RSA exponent e used for the operation. May be any of the following
 *    values: 3, 17, 65537.
 *
 *  @param[in]  pxN
 *    Buffer containing the RSA modulus N used for the operation. Buffer size is
 *    equal to key size.
 *
 *  @param[in]  xKeySize
 *    RSA key size in bytes
 *
 *  @param[in]  xPadding
 *    One of the valid padding types defined in TBsdRsaPadding. If xPadding is 
 *    equal to #BSD_RSA_NO_PADDING, the caller is in charge of removing the 
 *    padding if any.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation. This includes, but not limited to:
 *    - Unsupported exponent
 *    - Unsupported padding scheme
 *    - Unsupported key size
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and the operation could not be completed successfully.
 *
 *  @note
 *    -# All ouput parameters are allocated by the caller.
*/
TBsdStatus bsdRsaPublicDecrypt
(
  const TUnsignedInt8*      pxInput,
        TUnsignedInt8*      pxOutput,
        TSize*              pxOutputSize,
        TUnsignedInt32       xE,
  const TUnsignedInt8*      pxN,
        TSize                xKeySize,
        TBsdRsaPadding       xPadding
);

/******************************************************************************/
/*                                                                            */
/*                                    SHA-256                                 */
/*                                                                            */
/******************************************************************************/

/**
 *  @ingroup sha256
 *
 *  @brief
 *    Initializes a staged SHA-256 message digesting.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[out]  pxContext
 *    Context assigned to this SHA-256 message digesting. It is passed back to
 *    bsdSha256Update() and bsdSha256Final().
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation.
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and the operation could not be completed successfully.
 *
 *  @note
 *    -# The pxContext allocation is handled by the driver.
 */
TBsdStatus bsdSha256Init
(
  TBsdHashContext*  pxContext
);

/**
 *  @ingroup sha256
 *
 *  @brief
 *    This function processes the next chunk of the message to be hashed.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  xContext
 *    Message hashing context
 *
 *  @param[in]  pxMessageChunk
 *    Buffer containing the message chunk to be processed
 *
 *  @param[in]  xChunkSize
 *    Size in bytes of the message chunk, maximum size is 2^32 (4GB)
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation.
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and the operation could not be completed successfully.
*/
TBsdStatus bsdSha256Update
(
        TBsdHashContext  xContext,
  const TUnsignedInt8*  pxMessageChunk,
        TSize            xChunkSize
);

/**
 *  @ingroup sha256
 *
 *  @brief
 *    Finalizes SHA stages and returns the message digest.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  xContext
 *    Message hashing context
 *
 *  @param[out]  pxMessageDigest
 *    Buffer where to write the resulting message digest. The buffer is 
 *    allocated by the caller and is 32 bytes (256 bits) long.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation.
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and the operation could not be completed successfully.
 *
 *  @note
 *    -# The xContext deallocation is handled by the driver.
 */
TBsdStatus bsdSha256Final
(
  TBsdHashContext  xContext,
  TUnsignedInt8*  pxMessageDigest
);

/******************************************************************************/
/*                                                                            */
/*                                    HMAC                                    */
/*                                                                            */
/******************************************************************************/

/**
 *  @ingroup hmacsha256
 *
 *  @brief
 *    Initializes a staged HMAC-SHA-256 message authentication digesting.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  pxKey
 *    HMAC key used for the operation
 *
 *  @param[in]  xKeySize
 *    Size in bytes of the HMAC key. The maximum key size is equal to the
 *    SHA-256 block size, i.e. 64 bytes. If a longer key has to be used, the
 *    caller is in charge of hashing it beforehand.
 *
 *  @param[out]  pxContext
 *    Context assigned to this HMAC-SHA-256 message digesting. It is passed back
 *    to bsdHmacSha256Update() and bsdHmacSha256Final().
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation.
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and the operation could not be completed successfully.
 *
 *  @note
 *    -# The pxContext allocation is handled by the driver.
 */
TBsdStatus bsdHmacSha256Init
(
  const TUnsignedInt8*     pxKey,
        TSize               xKeySize,
        TBsdHashContext*   pxContext
);

/**
 *  @ingroup hmacsha256
 *
 *  @brief
 *    Updates the staged HMAC-SHA-256 authentication digesting with the next
 *    message chunk.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  xContext
 *    Message hashing context
 *
 *  @param[in]  pxMessageChunk
 *    Buffer containing the message chunk to be processed
 *
 *  @param[in]  xChunkSize
 *    Size in bytes of the message chunk, maximum size is 2^32 (4GB)
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation.
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and the operation could not be completed successfully.
*/
TBsdStatus bsdHmacSha256Update
(
        TBsdHashContext  xContext,
  const TUnsignedInt8*  pxMessageChunk,
        TSize            xChunkSize
);

/**
 *  @ingroup hmacsha256
 *
 *  @brief
 *    Finalizes HMAC-SHA-256 stages and returns the message digest.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  xContext
 *    Message hashing context
 *
 *  @param[out]  pxMessageDigest
 *    Buffer where to write the resulting message digest. The buffer is 
 *    allocated by the caller and is 32 bytes (256 bits) long.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation.
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and the operation could not be completed successfully.
 *
 *  @note
 *    -# The xContext deallocation is handled by the driver.
 */
TBsdStatus bsdHmacSha256Final
(
  TBsdHashContext  xContext,
  TUnsignedInt8*  pxMessageDigest
);

/******************************************************************************/
/*                                                                            */
/*                                  RANDOM                                    */
/*                                                                            */
/******************************************************************************/

/**
 *  @ingroup rand
 *
 *  @brief
 *    Generate a sequence of true random bytes and store it in specified buffer.
 *    The random number shall be non-deterministic and, at a minimum, comply with 
 *    random number generator tests as specified in FIPS 140-2, Security 
 *    Requirements for Cryptographic Modules, section 4.9.1.
 *
 *  @applies
 *    NOCS3 only
 *
 *  @param[in]  xNumOfBytes
 *    Number of random bytes to be generated. The maximum number is limited to
 *    1024 bytes.
 *
 *  @param[out]  pxRandomBytes
 *    Buffer where to write the xNumOfBytes generated random bytes.
 *
 *  @retval  BSD_NO_ERROR
 *    The operation was completed successfully.
 *
 *  @retval  BSD_ERROR_INVALID_PARAMETERS
 *    Missing or malformed parameters provided resulting in a premature
 *    termination of the operation.
 *
 *  @retval  BSD_ERROR
 *    Errors occurred and no random data could be generated.
*/
TBsdStatus bsdGenerateRandomBytes
(
  TSize            xNumOfBytes,
  TUnsignedInt8*  pxRandomBytes
);

/******************************************************************************/
/*                                                                            */
/*                                  RESET                                     */
/*                                                                            */
/******************************************************************************/

/**
 *  @ingroup res
 *
 *  @brief
 *    Performs a chipset reset, this reset must clear the registers, caches and
 *    other volatile memories. This function does not return.
 *
 *  @applies
 *    NOCS3 only
 *  
*/
void bsdChipReset
(
  void
);


/******************************************************************************/
/*                                                                            */
/*                              RAM2RAM (legacy)                              */
/*                                                                            */
/******************************************************************************/

/**
 *  @brief
 *    This type defines the cryptographic algorithms supported by Nagra 
 *    for the NOCS1.x RAM2RAM Crypto Engine.
*/
typedef enum
{
  BSD_R2R_ALGORITHM_AES = 0,
  BSD_R2R_ALGORITHM_TDES,
  LAST_BSD_R2R_ALGORITHM
}TBsdR2RAlgorithm;


/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function set the Secret Content Key (Key Ladder provided key) into the 
 *    Ram2Ram Crypto Engine and requests the Ram2Ram Crypto Engine to encrypt a 
 *    block of data stored in RAM and write the result into RAM.
 *
 *    The caller can select either TDES or AES as cryptographic algorithm. The 
 *    mode of operations is not configurable. It is fixed to ECB.
 *
 *  @applies
 *    NOCS1 only
 *
 *  @param[in]  xAlgorithm
 *    Cryptographic algorithm to be used by the RAM2RAM Crypto Engine. It can be
 *    either TDES or AES.
 *
 *  @param[in]  pxInputData
 *    Buffer, allocated by the caller, containing data to be encrypted.
 *
 *  @param[out] pxOutputData
 *    Buffer, allocated by the caller, where to write encrypted data.
 *
 *  @param[in]  xDataSize
 *    Size in bytes of input and output data buffers to operate on. It must be a
 *    multiple of the cipher block size (8 bytes for TDES and 16 bytes for
 *    AES).
 *
 *  @retval   BSD_NO_ERROR
 *    The intended operation was executed successfully.
 *
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *    At least one of the passed parameters is invalid:
 *    - pxInputData or pxOutputData is NULL
 *    - xDataSize does not match the algorithm cipher block size
 *
 *  @retval   BSD_ERROR_NOT_SUPPORTED
 *    Unkown or unsupported algorithm
 *
 *  @retval   BSD_ERROR
 *     The intended operation failed.
 *
 *  @note
 *  - If the key ladder has one or more key ladder elements, they shall be fed 
 *    with constants. Refer to Nagravision for the definition of these constants.
*/ 
TBsdStatus bsdEncryptDataWithSecretContentKey
(
        TBsdR2RAlgorithm             xAlgorithm,
  const TUnsignedInt8*              pxInputData,
        TUnsignedInt8*              pxOutputData,
        TUnsignedInt32               xDataSize
);


/**
 *  @ingroup r2r
 *
 *  @brief
 *    This function set the Secret Content Key (Key Ladder provided key) into the 
 *    Ram2Ram Crypto Engine and requests the Ram2Ram Crypto Engine to decrypt a 
 *    block of data stored in RAM and write the result into RAM.
 *
 *    The caller can select either TDES or AES as cryptographic algorithm. The 
 *    mode of operations is not configurable. It is fixed to ECB.
 *
 *  @applies
 *    NOCS1 only
 *
 *  @param[in]  xAlgorithm
 *    Cryptographic algorithm to be used by the RAM2RAM Crypto Engine. It can be
 *    either TDES or AES.
 *
 *  @param[in]  pxInputData
 *    Buffer, allocated by the caller, containing data to be decrypted.
 *
 *  @param[out] pxOutputData
 *    Buffer, allocated by the caller, where to write decrypted data.
 *
 *  @param[in]  xDataSize
 *    Size in bytes of input and output data buffers to operate on. It must be a
 *    multiple of the cipher block size (8 bytes for TDES and 16 bytes for
 *    AES).
 *
 *  @retval   BSD_NO_ERROR
 *    The intended operation was executed successfully.
 *
 *  @retval   BSD_ERROR_INVALID_PARAMETERS
 *    At least one of the passed parameters is invalid:
 *    - pxInputData or pxOutputData is NULL
 *    - xDataSize does not match the algorithm cipher block size
 *
 *  @retval   BSD_ERROR_NOT_SUPPORTED
 *    Unkown or unsupported algorithm
 *
 *  @retval   BSD_ERROR
 *     The intended operation failed.
 *
 *  @note
 *  - If the key ladder has one or more key ladder elements, they shall be fed 
 *    with constants. Refer to Nagravision for the definition of these constants.
*/
TBsdStatus bsdDecryptDataWithSecretContentKey
(
        TBsdR2RAlgorithm             xAlgorithm,
  const TUnsignedInt8*              pxInputData,
        TUnsignedInt8*              pxOutputData,
        TUnsignedInt32               xDataSize
);


#endif /* BSD_H */

/* nocs_bsd.h */
