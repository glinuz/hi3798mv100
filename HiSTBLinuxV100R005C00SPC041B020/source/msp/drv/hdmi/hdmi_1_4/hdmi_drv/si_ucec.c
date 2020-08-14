//------------------------------------------------------------------------------
// Copyright ?2002-2005, Silicon Image, Inc.  All rights reserved.
//
// No part of this work may be reproduced, modified, distributed, transmitted,
// transcribed, or translated into any language or computer format, in any form
// or by any means without written permission of: Silicon Image, Inc.,
// 1060 East Arques Avenue, Sunnyvale, California 94085
//------------------------------------------------------------------------------

// Turn down the warning level for this C file to prevent the compiler
// from complaining about the unused parameters in the stub functions below

#include <linux/delay.h>
#include <linux/sched.h>
#include <si_typedefs.h>
#include <si_cec_reg.h>
#include <si_cec.h>
#include <si_amf.h>
#include <si_config.h>
#include "si_hdmitx.h"
#include "drv_hdmi.h"

#if defined (CEC_SUPPORT)
enum siCECMessage_e
{
    SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES = 0X00,
    SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES,
    SI_CEC_MESSAGE_RETURN_ERROR_NOSUPPORT,
    SI_CEC_MESSAGE_RETURN_BUTT
};


static SiI_CEC_LogAddr_t CurLogAddr = SiI_CEC_LogAddr_STB1;
//------------------------------------------------------------------------------
// Function Name:
// Function Description:
//------------------------------------------------------------------------------
void cec_PrintLogAddr ( HI_U8 bLogAddr )
{

    if ( bLogAddr <= SiI_CEC_LogAddr_Unregister )
    {
        CEC_INFO (" [%02X] \n", (int) bLogAddr );
        switch ( bLogAddr )
        {
            case SiI_CEC_LogAddr_TV:
                CEC_INFO("TV \n");           break;
            case SiI_CEC_LogAddr_RecDev1:
                CEC_INFO("RecDev1 \n");      break;
            case SiI_CEC_LogAddr_RecDev2:
                CEC_INFO("RecDev2 \n");      break;
            case SiI_CEC_LogAddr_STB1:
                CEC_INFO("STB1 \n");         break;
            case SiI_CEC_LogAddr_DVD1:
                CEC_INFO("DVD1 \n");         break;
            case SiI_CEC_LogAddr_AudSys:
                CEC_INFO("AudSys \n");       break;
            case SiI_CEC_LogAddr_STB2:
                CEC_INFO("STB2 \n");         break;
            case SiI_CEC_LogAddr_STB3:
                CEC_INFO("STB3 \n");         break;
            case SiI_CEC_LogAddr_DVD2:
                CEC_INFO("DVD2 \n");         break;
            case SiI_CEC_LogAddr_RecDev3:
                CEC_INFO("RecDev3 \n");      break;
            case SiI_CEC_LogAddr_Res1:
                CEC_INFO("Res1 \n");         break;
            case SiI_CEC_LogAddr_Res2:
                CEC_INFO("Res2 \n");         break;
            case SiI_CEC_LogAddr_Res3:
                CEC_INFO("Res3 \n");         break;
            case SiI_CEC_LogAddr_Res4:
                CEC_INFO("Res4 \n");         break;
            case SiI_CEC_LogAddr_FreeUse:
                CEC_INFO("FreeUse \n");      break;
            case SiI_CEC_LogAddr_Unregister:
                CEC_INFO("BrocastMSG \n");    break;
        }
    }
}
//------------------------------------------------------------------------------
// Function Name: PrintCommand
// Function Description:
//------------------------------------------------------------------------------
static void PrintCommand( SiI_CEC_t * SiI_CEC )
{

    HI_U8 i;
    CEC_INFO ("[FROM][TO][OPCODE]  (OperandLegth){OPERANDS}: \n");
    cec_PrintLogAddr( (SiI_CEC->bDestOrRXHeader & 0xF0) >> 4 );
    cec_PrintLogAddr( SiI_CEC->bDestOrRXHeader & 0x0F );
    CEC_INFO (" [%02X]", (int) SiI_CEC->bOpcode);

    CEC_INFO ("   (%02X)", (int)(SiI_CEC->bCount & 0x0F));
    if (SiI_CEC->bCount & 0x0F)
    {
        for ( i = 0; i < ( SiI_CEC->bCount & 0x0F); i++ )
            CEC_INFO (" {%02X}", (int) SiI_CEC->bOperand[i]);
    }
    CEC_INFO ("\n");
}

#if 0
static void CEC_HDLR_ff_Abort( SiI_CEC_t * sii_cec )
{
    SiI_CEC_t cec_frame;
    //CEC_INFO("\n <abort> rcvd; send out feature abort");
    //
    // Unsupported opcode; send f e a t u r e   a b o r t
    //
    cec_frame.bOpcode         = 0x00;
    cec_frame.bDestOrRXHeader = (sii_cec->bDestOrRXHeader & 0xf0) >> 4 ;
    cec_frame.bOperand[0]     = 0xff;
    cec_frame.bOperand[1]     = 0;
    cec_frame.bCount          = 2;
    SiI_CEC_SetCommand( &cec_frame );
}

static void CEC_HDLR_83_GivePhysicalAddr( SiI_CEC_t * sii_cec )
{
    SiI_CEC_t cec_frame;
    //CEC_INFO("\n CEC_HDLR_83_GivePhysicalAddr");
    //
    // Transmit Physical Address: 0.0.0.0
    //
    cec_frame.bOpcode         = 0x84;
    cec_frame.bDestOrRXHeader = 0x0F ;
    cec_frame.bOperand[0]     = 0x00 ; // [Physical Address]
    cec_frame.bOperand[1]     = 0x00 ; // [Physical Address]
    cec_frame.bOperand[2]     = 0x00 ; // [Device Type] = 0 = TV
    cec_frame.bCount          = 3 ;
    SiI_CEC_SetCommand( &cec_frame ) ;
}

#endif

static HI_U32 cec_RX_MSG_Operation(HI_U8 Init, HI_U8 Dest, HI_U8 opcode, HI_U8 *pOperand, HI_U8 OperandLength, SiI_CEC_t *pCECframe)
{
    HI_U32 Ret = SI_CEC_MESSAGE_RETURN_ERROR_NOSUPPORT;//Default Return is Error NoSupprot!

    switch(opcode)
    {
    /*General Protocol messages*/
    case CEC_OPCODE_FEATURE_ABORT:                   //0X00
        CEC_INFO("FEATURE ABORT Init:%d, Dest:%d, opcode:0x%x\n", Init, Dest, opcode);
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES; //need not response
        break;
    case CEC_OPCODE_ABORT_MESSAGE:                   //0XFF
        CEC_INFO("ABORT MESSAGE\n");
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES; //need not response
        break;

    /*One Touch Play Feature*/
    case CEC_OPCODE_ACTIVE_SOURCE:                   //0X82
        CEC_INFO("ACTIVE_SOURCE\n");
        CEC_INFO("It should be source cmd, ignore!\n");
        break;
    case CEC_OPCODE_IMAGE_VIEW_ON:                   //0X04
        CEC_INFO("IMAGE_VIEW_ON\n");
        CEC_INFO("It should be source cmd, ignore!\n");
        break;
    case CEC_OPCODE_TEXT_VIEW_ON:                    //0X0D
        CEC_INFO("TEXT_VIEW_ON\n");
        CEC_INFO("It should be source cmd, ignore!\n");
        break;

    /*Routing Control Feature*/
    case CEC_OPCODE_INACTIVE_SOURCE:                 //0X9D
        CEC_INFO("INACTIVE_SOURCE, No supprot\n");
        Ret = SI_CEC_MESSAGE_RETURN_ERROR_NOSUPPORT;
        break;
    case CEC_OPCODE_REQUEST_ACTIVE_SOURCE:           //0X85
    //When a device(TV) comes out of the standy state, it maybe broacast a
    //<Request Active Source> message to discover active source.
    //the active source device shall respond by broadcasting an <Active Source> message.
    {
        HI_U8 PhyAddr[4], LogAddr;
        HI_U32 Rec;
        Rec = DRV_HDMI_GetCECAddress(PhyAddr, &LogAddr);

        if(Rec != HI_SUCCESS)
        {
            break;
        }
        CEC_INFO("REQUEST_ACTIVE_SOURCE, return Active Source\n");
        //we should send back a message of ACTIVE_SOURCE
        pCECframe->bOpcode   = CEC_OPCODE_ACTIVE_SOURCE;
        pCECframe->bDestOrRXHeader = HI_UNF_CEC_LOGICALADD_BROADCAST;
        pCECframe->bCount          = 2;
        pCECframe->bOperand[0]     = ((PhyAddr[0] << 4) & 0xf0) | (PhyAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
        pCECframe->bOperand[1]     = ((PhyAddr[2] << 4) & 0xf0) | (PhyAddr[3] & 0x0f) ; // [Physical Address(A.B.C.D):C D]
    }
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_ROUTING_CHANGE:                  //0X80
        CEC_INFO("ROUTING_CHANGE, it should be brocast message\n");
        break;
    case CEC_OPCODE_ROUTING_INFORMATION:             //0X81
        CEC_INFO("ROUTING_INFORMATION, it should be brocast message\n");
        break;
    case CEC_OPCODE_SET_STREAM_PATH:                 //0X86
    //the <Set Stream Path> is sent by the TV to the source device
    //to request it to broacst its path using an <Active Source> message.
        CEC_INFO("SET_STREAM_PATH\n");
    {
        HI_U8 PhyAddr[4], LogAddr;
        HI_U32 Rec;
        Rec = DRV_HDMI_GetCECAddress(PhyAddr, &LogAddr);

        if(Rec != HI_SUCCESS)
        {
            break;
        }
        CEC_INFO("REQUEST_ACTIVE_SOURCE, return Active Source\n");
        //we should send back a message of ACTIVE_SOURCE
        pCECframe->bOpcode   = CEC_OPCODE_ACTIVE_SOURCE;
        pCECframe->bDestOrRXHeader = HI_UNF_CEC_LOGICALADD_BROADCAST;
        pCECframe->bCount          = 2;
        pCECframe->bOperand[0]     = ((PhyAddr[0] << 4) & 0xf0) | (PhyAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
        pCECframe->bOperand[1]     = ((PhyAddr[2] << 4) & 0xf0) | (PhyAddr[3] & 0x0f) ; // [Physical Address(A.B.C.D):C D]

        //pCECframe->bOperand[2]     = LogAddr ; // [Device Type] = 3 Tuner1
    }
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    /*Standby Feature*/
    case CEC_OPCODE_STANDBY:                         //0X36
        CEC_INFO("STANDBY! It should brocast message\n");
        break;
    /*One Touch Record Feature*/
    case CEC_OPCODE_RECORD_OFF:                      //0X0B
        break;
    case CEC_OPCODE_RECORD_ON:                       //0X09
        break;
    case CEC_OPCODE_RECORD_STATUS:                   //0X0A
        break;
    case CEC_OPCODE_RECORD_TV_SCREEN:                //0X0F
        break;
    /*Timer Programming Feature*/
    case CEC_OPCODE_CLEAR_ANALOGUE_TIMER:            //0X33
        break;
    case CEC_OPCODE_CLEAR_DIGITAL_TIMER:             //0X99
        break;
    case CEC_OPCODE_CLEAR_EXTERNAL_TIMER:            //0XA1
        break;
    case CEC_OPCODE_SET_ANALOGUE_TIMER:              //0X34
        break;
    case CEC_OPCODE_SET_DIGITAL_TIMER:               //0X97
        break;
    case CEC_OPCODE_SET_EXTERNAL_TIMER:              //0XA2
        break;
    case CEC_OPCODE_SET_TIMER_PROGRAM_TITLE:         //0X67
        break;
    case CEC_OPCODE_TIMER_CLEARED_STATUS:            //0X43
        break;
    case CEC_OPCODE_TIMER_STATUS:                    //0X35
        break;

    /*System Information Feature*/
    case CEC_OPCODE_CEC_VERSION:                     //0X9E
        CEC_INFO("CEC_VERSION:\n");
        CEC_INFO("OperandLength:0x%02x \n",OperandLength);
        switch (pOperand[0])
        {
            case 0x04 :
                CEC_INFO("Version 1.3a\n");
                break;
            case 0x05 :
                CEC_INFO("Version 1.4 or Version 1.4a or Version 1.4b \n");
                break;
            default:
                CEC_INFO("unknown CEC_VERSION \n");
        }
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        {
            int index;
            for(index = 0; index < OperandLength; index ++)
            {
                if( ((pOperand[index] >= 'a') && (pOperand[index] <= 'z'))
                 || ((pOperand[index] >= 'A') && (pOperand[index] <= 'Z')) )
                {
                    CEC_INFO("%c", pOperand[index]);
                }
                else
                {
                    CEC_INFO("%02x", pOperand[index]);
                }
            }
        }
        CEC_INFO("\n");
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        break;
    case CEC_OPCODE_GET_CEC_VERSION:                 //0X9F
        CEC_INFO("GET_CEC_VERSION, Response\n");
        //we should send back a message of ACTIVE_SOURCE
        pCECframe->bOpcode   = CEC_OPCODE_CEC_VERSION;
        pCECframe->bDestOrRXHeader = Init;
        pCECframe->bCount          = 1;
        //hdmi spec1.4b cec spec page.108
        // 0x04 cec 1.3a   0x05 cec 1.4/1.4a/1.4b
        pCECframe->bOperand[0]     = 0x04;

#if 0
        pCECframe->bCount          = 3;
        pCECframe->bOperand[0]     = '1';
        pCECframe->bOperand[1]     = '3' ;
        pCECframe->bOperand[2]     = 'a' ;
#endif
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_GIVE_PHYSICAL_ADDRESS:           //0X83
        CEC_INFO("GIVE_PHYSICAL_ADDRESS, Response\n");
    {
        HI_U8 PhyAddr[4], LogAddr;
        HI_U32 Rec;
        Rec = DRV_HDMI_GetCECAddress(PhyAddr, &LogAddr);

        if(Rec != HI_SUCCESS)
        {
            break;
        }
        CEC_INFO("return REPORT_PHYSICAL_ADDRESS\n");
        //we should send back a message of ACTIVE_SOURCE
        pCECframe->bOpcode   = CEC_OPCODE_REPORT_PHYSICAL_ADDRESS;
        pCECframe->bDestOrRXHeader = Init;
        pCECframe->bCount          = 3;
        pCECframe->bOperand[0]     = ((PhyAddr[0] << 4) & 0xf0) | (PhyAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
        pCECframe->bOperand[1]     = ((PhyAddr[2] << 4) & 0xf0) | (PhyAddr[3] & 0x0f) ; // [Physical Address(A.B.C.D):C D]
        pCECframe->bOperand[2]     = LogAddr;
    }
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_REPORT_PHYSICAL_ADDRESS:         //0X84
        CEC_INFO("REPORT_PHYSICAL_ADDRESS\n");
        if(OperandLength != 3)
        {
            CEC_INFO("Error oprand length:%d\n", OperandLength);
        }
        CEC_INFO("PhysicalAddr:%01d.%01d.%01d.%01d, LogicalAddr:%d\n", (pOperand[0] & 0xf0) >> 4, (pOperand[0] & 0x0f), \
         (pOperand[1] & 0xf0) >> 4, (pOperand[1] & 0x0f), pOperand[2]);

        Ret = SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;
    case CEC_OPCODE_GET_MENU_LANGUAGE:               //0X91
        CEC_INFO("GET_MENU_LANGUAGE, response\n");
        pCECframe->bOpcode   = CEC_OPCODE_SET_MENU_LANGUAGE;
        pCECframe->bDestOrRXHeader = Init;
        pCECframe->bCount          = 3;


#if 0 /*--use ISO 639-2/T--*/
        //In general the T codes are favored; ISO 639-3 uses ISO 639-2/T.
        //However, ISO 15924 derives its codes when possible from ISO 639-2/B.
        //Language: 3 ASCII Bytes as defined in ISO/FDIS 639-2
        //"terminological" code (ISO 639-2/T)
        pCECframe->bOperand[0]     = 'z';
        pCECframe->bOperand[1]     = 'h' ;
        pCECframe->bOperand[2]     = 'o' ;
#else /*-- use ISO 639-2/B--*/
        //In sumsung TV,we found it use ISO 639-2/B
        //Language: 3 ASCII Bytes as defined in ISO/FDIS 639-2
        //"bibliographic" code (ISO 639-2/B)
        pCECframe->bOperand[0]     = 'c';
        pCECframe->bOperand[1]     = 'h' ;
        pCECframe->bOperand[2]     = 'i' ;
#endif

        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_SET_MENU_LANGUAGE:               //0X32
        CEC_INFO("SET_MENU_LANGUAGE:");
        {
            int index;
            for(index = 0; index < OperandLength; index ++)
            {
                if( ((pOperand[index] >= 'a') && (pOperand[index] <= 'z'))
                 || ((pOperand[index] >= 'A') && (pOperand[index] <= 'Z')) )
                {
                    CEC_INFO("%c", pOperand[index]);
                }
                else
                {
                    CEC_INFO("%02x", pOperand[index]);
                }
            }
        }
        CEC_INFO("\n");
        break;

    /*Deck Control Feature*/
    case CEC_OPCODE_DECK_CONTROL:                    //0X42
        CEC_INFO("DECK_CONTROL\n");
        break;
    case CEC_OPCODE_DECK_STATUS:                     //0X1B
        CEC_INFO("DECK_STATUS\n");
        break;
    case CEC_OPCODE_GIVE_DECK_STATUS:                //0X1A
        CEC_INFO("GIVE_DECK_STATUS\n");
        break;
    case CEC_OPCODE_PLAY:                            //0X41
        CEC_INFO("PLAY\n");
        break;

    /*Tuner Control Feature*/
    case CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS:        //0X08
        break;
    case CEC_OPCODE_SELECT_ANALOGUE_SERVICE:         //0X92
        break;
    case CEC_OPCODE_SELECT_DIGITAL_SERVICE:          //0X93
        break;
    case CEC_OPCODE_TUNER_DEVICE_STATUS:             //0X07
        break;
    case CEC_OPCODE_TUNER_STEP_DECREMENT:            //0X06
        break;
    case CEC_OPCODE_TUNER_STEP_INCREMENT:            //0X05
        break;
    /*Vendor Specific Command*/
    case CEC_OPCODE_DEVICE_VENDOR_ID:                //0X87
        CEC_INFO("DEVICE_VENDOR_ID:");
        {
            int index;
            for(index = 0; index < OperandLength; index ++)
            {
                CEC_INFO("%02x ", pOperand[index]);
            }
        }
        CEC_INFO("\n");
        break;
    case CEC_OPCODE_GIVE_DEVICE_VENDOR_ID:           //0X8C
        CEC_INFO("GIVE_DEVICE_VENDOR_ID, Response\n");
        pCECframe->bOpcode   = CEC_OPCODE_DEVICE_VENDOR_ID;
        pCECframe->bDestOrRXHeader = 0x0f;
        pCECframe->bCount          = 3;
        pCECframe->bOperand[0]     = 'h';
        pCECframe->bOperand[1]     = 'i' ;
        pCECframe->bOperand[2]     = 's' ;

        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_VENDOR_COMMAND:                  //0X89
        break;
    case CEC_OPCODE_VENDOR_COMMAND_WITH_ID:          //0XA0
        break;
    case CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN:       //0X8A
        break;
    case CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP:         //0X8B
        break;

    /*OSD Display Feature*/
    case CEC_OPCODE_SET_OSD_STRING:                  //0X64
        CEC_INFO("SET_OSD_NAME:%s\n", pOperand);
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;
    case CEC_OPCODE_GIVE_OSD_NAME:                   //0X46
        CEC_INFO("CEC_OPCODE_GIVE_OSD_NAME, Response\n");
        pCECframe->bOpcode   = CEC_OPCODE_SET_OSD_STRING;
        pCECframe->bDestOrRXHeader = Init;
        pCECframe->bCount          = 3;
        pCECframe->bOperand[0]     = 's';
        pCECframe->bOperand[1]     = 't' ;
        pCECframe->bOperand[2]     = 'b' ;

        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_SET_OSD_NAME:                    //0X47
        CEC_INFO("SET_OSD_NAME:");
        {
            int index;
            for(index = 0; index < OperandLength; index ++)
            {
                if( ((pOperand[index] >= 'a') && (pOperand[index] <= 'z'))
                 || ((pOperand[index] >= 'A') && (pOperand[index] <= 'Z')) )
                {
                    CEC_INFO("%c", pOperand[index]);
                }
                else
                {
                    CEC_INFO("%02x", pOperand[index]);
                }
            }
        }
        CEC_INFO("\n");
        break;

    /*Device Menu Control Feature*/
    case CEC_OPCODE_MENU_REQUEST:                    //0X8D
        CEC_INFO("MENU_REQUEST\n");
        if(OperandLength < 1)
        {
            CEC_INFO("Invalid OperandLength:%d\n", OperandLength);
            break;
        }

        // Send back Menu STATUS
        pCECframe->bOpcode   = CEC_OPCODE_MENU_STATUS;
        pCECframe->bDestOrRXHeader = Init;
        pCECframe->bCount          = 1;

        if(pOperand[0] == 0x00)
        {
            CEC_INFO("Active\n");
            pCECframe->bOperand[0]     = 0X00;//Active
        }
        else if(pOperand[0] == 0x01)
        {
            CEC_INFO("Deactive\n");
            pCECframe->bOperand[0]     = 0X01;//Deactive
        }
        else if(pOperand[0] == 0x02)
        {
            CEC_INFO("Query\n");
        }
        else
        {
            CEC_INFO("Unknown, pOperand[0]:0x%x\n", pOperand[0]);
            break;
        }

        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_MENU_STATUS:                     //0X8E
        CEC_INFO("MENU_STATUS:\n");
        if(OperandLength < 1)
        {
            CEC_INFO("Invalid OperandLength:%d\n", OperandLength);
            break;
        }
        if(pOperand[0] == 0x00)
        {
            CEC_INFO("Active\n");
        }
        else if(pOperand[0] == 0x01)
        {
            CEC_INFO("Deactive\n");
        }

        Ret = SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;
    case CEC_OPCODE_USER_CONTROL_PRESSED:            //0X44
        CEC_INFO("USER_CONTROL_PRESSED\n");
        CEC_INFO("Press RC:0x%02x\n", pOperand[0]);
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;
    case CEC_OPCODE_USER_CONTROL_RELEASED:           //0X45
        CEC_INFO("USER_CONTROL_RELEASED\n");
        CEC_INFO("Release RC:0x%02x\n", pOperand[0]);
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;

    /*Power Status Feature*/
    case CEC_OPCODE_GIVE_DEVICE_POWER_STATUS:        //0X8F
        CEC_INFO("GIVE_DEVICE_POWER_STATUS, response\n");

        pCECframe->bOpcode   = CEC_OPCODE_REPORT_POWER_STATUS;
        pCECframe->bDestOrRXHeader = Init;
        pCECframe->bCount          = 1;
        pCECframe->bOperand[0]     = 0X00;//Power on
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        pCECframe->bOpcode   = CEC_OPCODE_SET_OSD_STRING;
        pCECframe->bDestOrRXHeader = Init;
        pCECframe->bCount          = 1;
        pCECframe->bOperand[0]     = 0X00;//Power on
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

        Ret = SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_REPORT_POWER_STATUS:             //0X90
        CEC_INFO("REPORT_POWER_STATUS\n");
        if(OperandLength < 1)
        {
            CEC_INFO("Invalid OperandLength:%d\n", OperandLength);
            break;
        }
        if(pOperand[0] == 0x00)
        {
            CEC_INFO("Power On\n");
        }
        else if(pOperand[0] == 0x01)
        {
            CEC_INFO("Stand By\n");
        }
        else
        {
            CEC_INFO("Unknown, pOperand[0]:0x%x\n", pOperand[0]);
        }
        Ret = SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;

    /*System Audio Control Feature*/
    case CEC_OPCODE_GIVE_AUDIO_STATUS:               //0X71
        CEC_INFO("GIVE_AUDIO_STATUS\n");
        break;
    case CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:   //0x7D
        break;
    case CEC_OPCODE_REPORT_AUDIO_STATUS:             //0X7A
        break;
    case CEC_OPCODE_SET_SYSTEM_AUDIO_MODE:           //0X72
        break;
    case CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST:       //0X70
        break;
    case CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS:        //0X7E
        break;

    /*Audio Rate Control Feature*/
    case CEC_OPCODE_SET_AUDIO_RATE:                  //0X9A
        break;
    default:
        return 3;
    }
    return Ret;
}

// Check received MSG validation.
// return 0 if msg is 0K, return 1 if msg is 1nva1id
static HI_U8 cec_msg_validate( SiI_CEC_t * sii_cec )
{
    HI_U32 result = 0;
    SiI_CEC_t cec_frame;
    HI_U8 CEC_RX_INIT, CEC_RX_DEST;

    HI_U8 PhyAddr[4], LogAddr;
    HI_U32 Rec;
    Rec = DRV_HDMI_GetCECAddress(PhyAddr, &LogAddr);

    if(Rec != HI_SUCCESS)
    {
        return 1;
    }

    //Deal with CEC Rx message
    CEC_RX_DEST = (sii_cec->bDestOrRXHeader & 0x0f);
    CEC_RX_INIT = (sii_cec->bDestOrRXHeader & 0xf0) >> 4;

    if(CEC_RX_INIT == LogAddr)
    {
        CEC_INFO("Selft Message, ignore\n");
        return 1;
    }
    if ((CEC_RX_DEST != LogAddr) && (CEC_RX_DEST != SiI_CEC_LogAddr_Unregister))
    {
        CEC_INFO("Message is not sent to us CEC_RX_DEST:%d\n", CEC_RX_DEST);
        return 1;
    }

    memset(&cec_frame, 0, sizeof(SiI_CEC_t));
    result = cec_RX_MSG_Operation(CEC_RX_INIT, CEC_RX_DEST, sii_cec->bOpcode, sii_cec->bOperand, (sii_cec->bCount & CEC_RX_CMD_CNT), &cec_frame);

    if(result == SI_CEC_MESSAGE_RETURN_OK_WITH_RESPONES)
    {
        CEC_INFO("response!\n");
        //SiI_CEC_SetCommand(&cec_frame);
        return 0;
    }
    else if(result == SI_CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES)
    {
        return 0;//direct return!
    }
    else if(result == SI_CEC_MESSAGE_RETURN_ERROR_NOSUPPORT)
    {
        return 0;
    }

    //Deal with Unsupport Message!    //
    // Do not reply to Broadcast msgs, otherwise send Feature Abort
    // for all unsupported features.
    if( (sii_cec->bDestOrRXHeader & 0x0F) != 0x0F)
    {
        /*All devices shall support the message <Feature Abort>.
          It is used to allow devices to indicate if they do not support an opcode that has been
          sent to them, if it is unable to deal with this message.
          <Feature Abort> has 2 parameters: opcode and the reason of its reject of the frame.

          <Feature Abort> is used as a response to any failure!
        */
        // Unsupported opcode; send feature abort
        //<Feature Abort>has 2 parameters, the opcode and a reason for its rejection of the frame.
        memset(&cec_frame, 0, sizeof(SiI_CEC_t));
        cec_frame.bOpcode         = CEC_OPCODE_FEATURE_ABORT;//0x00;
        cec_frame.bDestOrRXHeader = CEC_RX_INIT; //(sii_cec->bDestOrRXHeader & 0xf0) >> 4 ;
        cec_frame.bOperand[0]     = sii_cec->bOpcode;
        cec_frame.bOperand[1]     = 0x00; //Unrecognized opcode:0x00
        cec_frame.bCount = 2;
        //SiI_CEC_SetCommand( &cec_frame );
        CEC_INFO("\n!Unsupported bOpcode:0x%x Feature!\n", sii_cec->bOpcode);
    }
    else
    {
        // Unsupported Broadcast Msg
        CEC_INFO("\n! Unsupported Brocast Msg!\n");
    }

    return 1;
}

#define CEC_MSG_POOL_SIZE 512
SiI_CEC_t cec_msg_pool[CEC_MSG_POOL_SIZE];
unsigned int msg_pool_rptr = 0;
unsigned int msg_pool_wptr = 0;
unsigned int can_read_num = 0;
unsigned int  get_msg_counter = 0;
wait_queue_head_t hdmi_cec_wait_queue_head ;

void get_buf_status(void)
{
    unsigned int i = 0;
    if(msg_pool_rptr == msg_pool_wptr){
        msg_pool_rptr = 0;
        msg_pool_wptr = 0;
        for(i=0; i < CEC_MSG_POOL_SIZE; i++)
            memset(&cec_msg_pool[i], 0, sizeof(SiI_CEC_t));
    }
}

void store_cec_msg(SiI_CEC_t *rx_cmd)
{
    get_buf_status();
    memcpy(&cec_msg_pool[msg_pool_wptr], rx_cmd, sizeof(SiI_CEC_t));

    if(msg_pool_wptr == CEC_MSG_POOL_SIZE - 1)
	{
        msg_pool_wptr = 0;
    }else
	{
        msg_pool_wptr++;
    }

    if(msg_pool_rptr == msg_pool_wptr)
	{
        can_read_num = CEC_MSG_POOL_SIZE;
    }
	else if(msg_pool_rptr <  msg_pool_wptr)
	{
        can_read_num = msg_pool_wptr - msg_pool_rptr;
    }
	else
	{
        can_read_num = CEC_MSG_POOL_SIZE -(msg_pool_rptr - msg_pool_wptr);
    }

    if((can_read_num > 0) && (get_msg_counter > 0))
	{
        wake_up_interruptible(&hdmi_cec_wait_queue_head);
    }

    CEC_INFO("store_cec_msg num:%d  \n",can_read_num);
}

unsigned int  get_cec_msg(HI_UNF_HDMI_CEC_CMD_S *rx_cmd, unsigned int num, HI_U32 timeout)
{
    HI_S32 s32Ret;
    if(get_msg_counter == 0)
	{
        init_waitqueue_head(&hdmi_cec_wait_queue_head);
	}
    get_msg_counter ++;

    s32Ret = wait_event_interruptible_timeout(hdmi_cec_wait_queue_head, (can_read_num > 0) , timeout);
    if((s32Ret <= 0) && (0 == can_read_num))
    {
        //CEC_INFO("no cec data,num:%d\n",can_read_num);
        return HI_FAILURE;
    }

    memset(rx_cmd, 0 ,sizeof(HI_UNF_HDMI_CEC_CMD_S));
    if(can_read_num > 0){
        rx_cmd->enSrcAdd = (cec_msg_pool[msg_pool_rptr].bDestOrRXHeader & 0xf0) >>4;
        rx_cmd->enDstAdd = cec_msg_pool[msg_pool_rptr].bDestOrRXHeader & 0x0f;
        rx_cmd->u8Opcode = cec_msg_pool[msg_pool_rptr].bOpcode;
		rx_cmd->unOperand.stRawData.u8Length = cec_msg_pool[msg_pool_rptr].bCount & CEC_RX_BYTE_CNT; //get byte count
        memcpy(rx_cmd->unOperand.stRawData.u8Data, cec_msg_pool[msg_pool_rptr].bOperand, rx_cmd->unOperand.stRawData.u8Length);

        if(msg_pool_rptr == CEC_MSG_POOL_SIZE - 1)
		{
            msg_pool_rptr = 0;
        }else
		{
            msg_pool_rptr ++;
        }
    }

    if(msg_pool_rptr == msg_pool_wptr)
	{
        can_read_num = 0;
	}
	else if(msg_pool_rptr <  msg_pool_wptr)
	{
        can_read_num = msg_pool_wptr - msg_pool_rptr;
    }
	else
	{
        can_read_num = CEC_MSG_POOL_SIZE -(msg_pool_rptr - msg_pool_wptr);
    }

    CEC_INFO("get_cec_msg num:%d  \n",can_read_num);

    return  HI_SUCCESS;
}



//------------------------------------------------------------------------------
// Function Name: ParsingReceivedData
//
// Function Description:
// 1. CEC7.3 Frame Validation
// 2. Protocol Extension
// 3. CEC12.3 Feature Abort
// 4. Amber Alert i.e. call to function specified by the handler that is expecting
//    a reply from the specified device, LA, LogicalAddress.
//------------------------------------------------------------------------------
static void cec_ParsingRecevedData ( HI_U8 bRXState )
{
    HI_U8 bAuxData;
    HI_U8 i;
    SiI_CEC_t SiI_CEC;

    //
    // CEC_RX_COUNT Register:  RX_ERROR | CEC_RX_CMD_CNT | CEC_RX_byte_CNT
    // See CPI document for details.
    //
    bAuxData = bRXState & CEC_RX_CMD_CNT;
    CEC_INFO ("\n");
    if( bAuxData )
    {
        memset(&SiI_CEC, 0, sizeof(SiI_CEC_t));
        SiI_CEC.bCount = bRXState;
        bAuxData >>=4;
        CEC_INFO ("%i frames in RX FIFO\n", (int) bAuxData );
        /* Get each Frame from RX FIFO */
        for ( i = 0; i < bAuxData; i++ )
        {
            CEC_INFO ("Deal with %i frames in RX FIFO\n", (int) bAuxData );
            SI_CEC_GetCommand( &SiI_CEC );

            if(((SiI_CEC.bDestOrRXHeader & 0xf0) >> 4) == CurLogAddr)
            {
                CEC_INFO("We should ignore this message, it is sent from ourself\n");
            }
            else
            {
                PrintCommand( &SiI_CEC );
                // Check BC msg && init==0x00
                // Validation Message
                if(0 == cec_msg_validate( &SiI_CEC )){
                    store_cec_msg(&SiI_CEC);
                }
            }
            /* Get the next message */
            memset(&SiI_CEC, 0, sizeof(SiI_CEC_t));
            if ( SiI_CEC.bRXNextCount)
                SiI_CEC.bCount = SiI_CEC.bRXNextCount;
        }
    }
    return;
}

//------------------------------------------------------------------------------
// Function Name: userSWTtask
// Function Description:
// This is function handler event from the CEC RX buffer.
// CEC Software Connection #5.
//------------------------------------------------------------------------------

static HI_U8 bCECTask = 0;
static HI_U8 bNewTask = SiI_CEC_Idle;
static HI_U8 result[16] = {0};
HI_U8 bTXState = SI_CEC_TX_WAITCMD;

void SI_CEC_Event_Handler( void )
{
    static HI_U8 i = 0;
    SiI_CEC_Int_t CEC_Int;

    if( bNewTask != bCECTask )
    {
        bNewTask = bCECTask;
        i = 0;
    }
    //CEC_INFO("xxx ping bTXState:%d, SI_CEC_TX_WAITCMD:%d\n", bTXState, SI_CEC_TX_WAITCMD);
    if (bCECTask)
    {
        //CEC_INFO ("\nCEC_Event_Handler CECTask %x %x\n",             bCECTask, bTXState);
    }

    // Process CEC Events; at this point doesn't do much
    SI_CEC_Interrupt_Processing(&CEC_Int);

    if( bCECTask == SiI_CEC_ReqPing )
    {
        CEC_INFO("ping bTXState:%d, SI_CEC_TX_WAITCMD:%d\n", bTXState, SI_CEC_TX_WAITCMD);
        //
        // Test function 1: Ping Button Pressed
        //
        if( bTXState == SI_CEC_TX_WAITCMD )
        {
            CEC_INFO("request ping\n");
            CEC_INFO("\n %X", (int)i );
            SI_SiI_CEC_SendPing( i );

            bTXState = SI_CEC_TX_SENDING;
        }
        else if(bTXState == SI_CEC_TX_SENDING )
        {
            if( CEC_Int.bTXState == SI_CEC_TX_SENDFAILURE )
            {
                result[i] = 0;

                CEC_INFO (" NoAck \n");
                cec_PrintLogAddr(i);
                i++;
                bTXState = SI_CEC_TX_WAITCMD;
            }
            if( CEC_Int.bTXState == SI_CEC_TX_SENDACKED )
            {
                result[i] = 1;

                CEC_INFO (" Ack \n"); cec_PrintLogAddr(i);
                i++;
                bTXState = SI_CEC_TX_WAITCMD;

            }
            if( i >= SiI_CEC_LogAddr_Unregister )
            {
                bCECTask = SiI_CEC_Idle;
                CEC_INFO("\nend of send command\n");
                {
                    int index;
                    for(index = 0;index < 16; index ++)
                    {
                        CEC_INFO("index:%d, result:%d\n", index, result[index]);
                    }
                    CEC_INFO("end\n");
                }
                i = 0;
            }
        }
    }
    else if(bCECTask == SiI_CEC_ReqCmd1)
    {
        //  Test function 2: Send test message
        if ( CEC_Int.bTXState == SI_CEC_TX_SENDFAILURE )
        {
            CEC_INFO (" NoAck ");
            bTXState = SI_CEC_TX_WAITCMD;
            bCECTask = SiI_CEC_Idle;
        }
        if ( CEC_Int.bTXState == SI_CEC_TX_SENDACKED )
        {
            CEC_INFO (" Ack ");
            bTXState = SI_CEC_TX_WAITCMD;
            bCECTask = SiI_CEC_Idle;
        }
    }

    // Check for incoming CEC frames in the Rx Fifo.
    if( CEC_Int.bRXState )
    {
        cec_ParsingRecevedData( CEC_Int.bRXState );
    }
}

HI_U32 SI_CEC_SendCommand(HI_UNF_HDMI_CEC_CMD_S  *pCECCmd)
{
    SiI_CEC_t cec_frame;
    memset(&cec_frame, 0, sizeof(SiI_CEC_t));

    SI_CEC_RegisterWrite(REG__CEC_TX_INIT, pCECCmd->enSrcAdd);

    CurLogAddr = pCECCmd->enSrcAdd;

    if(pCECCmd->u8Opcode == CEC_OPCODE_POLLING_MESSAGE)
    {
        SI_SiI_CEC_SendPing(pCECCmd->enDstAdd);
        bCECTask = SiI_CEC_ReqPing;

        CEC_INFO("\nSend Ping cmd\n");
        return 0;
    }

    cec_frame.bOpcode         = pCECCmd->u8Opcode;
    cec_frame.bDestOrRXHeader = pCECCmd->enDstAdd;
    cec_frame.bCount          = pCECCmd->unOperand.stRawData.u8Length;
    memcpy((cec_frame.bOperand), (pCECCmd->unOperand.stRawData.u8Data), pCECCmd->unOperand.stRawData.u8Length);

    bCECTask = SiI_CEC_ReqCmd1;

    SiI_CEC_SetCommand( &cec_frame );

    return HI_SUCCESS;
}

HI_U32 SI_CEC_AudioPing(HI_U32 *pu32Status)
{
    HI_U8 status, timeout = 0;
    HI_U32 Error = HI_SUCCESS;

    /* CEC Set Up Register */
    WriteByteHDMICEC(REG__CEC_AUTO_PING_CTRL, 0x02); //0xcc 0xe0

    WriteByteHDMICEC(REG__CEC_AUTO_PING_CTRL, 0x01);  //start cec ping

    while(timeout ++ < 10)
    {
        msleep(100);
        status = ReadByteHDMICEC(REG__CEC_AUTO_PING_CTRL);
        if(0x80 == (status & 0x80))
        {
            CEC_INFO("REG__CEC_AUTO_PING_CTRL:0x%x\n", status);
            break;
        }
    }

    if(timeout >= 100)
    {
        CEC_ERR("AutoPing timeout\n");
        return HI_FAILURE;
    }

    status = ReadByteHDMICEC(REG__CEC_AUTO_PING_MAP0);          //0xcc 0xe1
    //CEC_INFO("AUTO_PING_MAP0:0x%x\n",status);
    status = ReadByteHDMICEC(REG__CEC_AUTO_PING_MAP1) + status; //0xcc 0xe2

    CEC_INFO("CEC Auto Ping Result:0x%x\n", status);
    WriteByteHDMICEC(REG__CEC_AUTO_PING_CTRL, 0x00);

    *pu32Status = status;

    return Error;
}

HI_U32 SI_CEC_Open(HI_U8 u8LogicalAddr)
{
    //Set CEC Interrupt bit
    SI_CEC_RegisterWrite( REG__CEC_INT_ENABLE_0, 0xff );
    SI_CEC_RegisterWrite( REG__CEC_INT_ENABLE_1, 0xff );
    //Set Logical Address and Caputure address
    SI_CEC_CAPTURE_ID_Set(u8LogicalAddr);

    return 0;
}

HI_U32 SI_CEC_Close(void)
{
    //Clean CEC Interrupt bit
    SI_CEC_RegisterWrite( REG__CEC_INT_ENABLE_0, 0x00 );
    SI_CEC_RegisterWrite( REG__CEC_INT_ENABLE_1, 0x00 );

    return 0;
}
#endif

/*---------------------------END-----------------------*/

