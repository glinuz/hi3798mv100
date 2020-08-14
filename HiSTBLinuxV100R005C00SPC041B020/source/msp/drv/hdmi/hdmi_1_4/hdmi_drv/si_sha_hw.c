/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_sha_hw.h"
#include "si_sha.h"
#include "si_hdcp.h"
#include "si_hdcpdefs.h"
#include "si_fifo.h"
#include "si_defstx.h"
#include "si_hdmitx.h"

#ifdef _9032_SHA_

//------------------------------------------------------------------------------
// Function Name: ClearKSVFIFOPointer
// Function Description: This function clears KSV FIFO pointer in TX
//------------------------------------------------------------------------------
static void ClearKSVFIFOPointer( void ){

    WriteWordHDMITXP0( KSV_SHA_START_ADDR, 0 );

}
//------------------------------------------------------------------------------
// Function Name: GetBStatus
// Function Description: This function reads BStatus from Down Stream through DDC bus
//------------------------------------------------------------------------------
static void GetBStatus( HI_U8 * pbData ){

    MDDCBlockReadHDCPRX( 2, DDC_BSTATUS_ADDR, pbData);
}
//------------------------------------------------------------------------------
// Function Name: SetDS_BStatus
// Function Description:This function reads Down Steam BStatus from RX, and sets into TX
//                      BStatus will be used for SHA calculations
//------------------------------------------------------------------------------
static void SetDS_BStatus ( HI_U8 * pbData ){

    SI_WriteBlockHDMITXP0(DS_BSTATUS_ADDR, 2, pbData );
}
//------------------------------------------------------------------------------
// Function Name: GetKSVListAndSetTXKSVFIFO
// Function Description: This function reads KSV FIFO from RX usind DDC bus,
//                       and writes into KSV FIFO TX
//------------------------------------------------------------------------------
static void GetKSVListAndSetTXKSVFIFO( HI_U8 bNumberDS_Devices ){
HI_U8 bData[5], i;

    ClearKSVFIFOPointer();
    SI_MDDCInitReadBlockFromFIFO( DDC_KSV_FIFO_ADDR, bNumberDS_Devices * 5 );

    for (i = 0; i < bNumberDS_Devices; i++ ){
        SI_MDDCTakeBlockFromFIFO( 5, bData );
        SI_WriteBlockHDMITXP0( KSV_FIFO_ADDR, 5, bData );
    }

}
//------------------------------------------------------------------------------
// Function Name: SetKSVLength
// Function Description: This function sets, KSV length, parameter should be in bytes
//------------------------------------------------------------------------------
static void SetKSVLength( HI_U8 bNumberDS_Devices ){

    WriteWordHDMITXP0( SHA_LENGTH_ADDR, (HI_U16) bNumberDS_Devices * 5);

}

//------------------------------------------------------------------------------
// Function Name: SetSHATransformCommand
// Function Description: This function writes SHA transform command
//------------------------------------------------------------------------------
static void SetSHATransformCommand( HI_U8 bCommand ){

    WriteByteHDMITXP0( SHA_CONTROL_ADDR, bCommand );

}
//------------------------------------------------------------------------------
// Function Name: IsSHADoneSet
// Function Description: This function check if SHA Done is set
//------------------------------------------------------------------------------
static Bool IsSHADoneSet( void ) {
Bool qResult = FALSE;

    if( ReadByteHDMITXP0( SHA_CONTROL_ADDR ) & BIT_SHA_DONE )
        qResult = TRUE;
    return qResult;

}
//------------------------------------------------------------------------------
// Function Name: IsSHATransformReady
// Function Description:This function checks if SHA result is ready
//------------------------------------------------------------------------------
static Bool IsSHATransformReady( void ){
HI_U8 i;
Bool qResult = FALSE;

    do {
        if ( IsSHADoneSet() ){
            qResult = TRUE;
            break;
        }
    } while ( i < 5 );
    return qResult;

}
//------------------------------------------------------------------------------
// Function Name: SHA_HW_Transform
// Function Description: This function executes SHA transform
//------------------------------------------------------------------------------
static Bool SHA_HW_Transform( HI_U8 bNumberDS_Devices, Bool qMode){

    qMode = FALSE;
    SetKSVLength( bNumberDS_Devices );
    ClearKSVFIFOPointer( );
    SetSHATransformCommand( SHA_GO );
    return IsSHATransformReady();
}
//------------------------------------------------------------------------------
// Function Name: SHA_HW_CompareVi()
// Function Description: This function compares V calculated by TX itself and V'
//                       which we get from RX trough DDC bus
//------------------------------------------------------------------------------
static Bool SHA_HW_CompareVi( void )
{
HI_U8 bVPrime[4], bV[4];
Bool qResult = TRUE;
HI_U8 i, j;

    SI_MDDCInitReadBlockFromFIFO( DDC_V_ADDR, 20 );
    for(i = 0; i < 5; i ++) {
        SI_MDDCTakeBlockFromFIFO( 4, (HI_U8 *)&bVPrime);
        SI_ReadBlockHDMITXP0( V_ADDR + i * 4, 4, bV );
        for ( j = 0; j < 4; j++){
            if ( bVPrime[j] != bV[j] ){
                qResult = FALSE;
                break;
            }
        }

    }
    return qResult;
}
//------------------------------------------------------------------------------
// Function Name: SHA_HW_Handler
// Function Description:  Used for calculation V value by TX hardware
//
// Accepts: none
// Returns: Bool Result
// Globals: AuthState
//------------------------------------------------------------------------------

Bool SHA_HW_Handler( void ){
HI_U8 bBStatus[2];
Bool qResult = TRUE;

    if( AuthState == REQ_SHA_HW_CALC )
    {
        COM_INFO("-->AuthState == REQ_SHA_HW_CALC.\n");

        SI_AutoRiCheck ( OFF );

        GetBStatus( bBStatus );
        SetDS_BStatus ( bBStatus );
        GetKSVListAndSetTXKSVFIFO( bBStatus[0] & NDEV_MASK );
        if ( SHA_HW_Transform( bBStatus[0] & NDEV_MASK, FALSE ) ){
            if ( !SHA_HW_CompareVi())
                qResult = FALSE;
        }
        else qResult = FALSE;
    }
    if ( !qResult ){
        AuthState = FAILED_ViERROR;
    }
    else {
        AuthState = AUTHENTICATED;
        SI_AutoRiCheck ( ON );
    }
    return qResult;
}
#endif
