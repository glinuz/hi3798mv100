/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_hdmi_cec.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"

#define CEC_PRINT  printf

//return PARAM
#define CEC_MESSAGE_RETURN_OK_WITH_RESPONES     0x01
#define CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES  0x02
#define CEC_MESSAGE_RETURN_ERROR_NOSUPPORT      0x03
#define CEC_MESSAGE_RETURN_BUTT                 0x04

//cec power status
#define CEC_POWER_STATUS_ON                     0x00
#define CEC_POWER_STATUS_STANDBY                0x01
#define CEC_POWER_STATUS_STANDBY2ON             0x02
#define CEC_POWER_STATUS_ON2STANDBY             0x03

#define CEC_READY_TIME                          5

HI_UNF_HDMI_CALLBACK_FUNC_S g_stCallbackFunc;
static HI_S32      g_cecCmdId = 1;
static HI_U8       g_cecPowerStatus = CEC_POWER_STATUS_STANDBY;
static HI_U32 g_HDMIUserCallbackFlag = HI_FALSE;
static User_HDMI_CallBack pfnHdmiUserCallback = NULL;

HI_VOID HDMI_CEC_PrintCmd(HI_UNF_HDMI_CEC_CMD_S *pstCECCmd)
{
    HI_U32 i;

    CEC_PRINT("\n==============================================================================\n");
    CEC_PRINT("Cec Cmd Index: %d \n",g_cecCmdId++);
    CEC_PRINT("Src Addr:0x%x\n",pstCECCmd->enSrcAdd);
    CEC_PRINT("Dst Addr:0x%x\n",pstCECCmd->enDstAdd);
    CEC_PRINT("OpCode  :0x%x\n",pstCECCmd->u8Opcode);
    CEC_PRINT("cmd len :0x%x\n",pstCECCmd->unOperand.stRawData.u8Length);

    for(i = 0;i < pstCECCmd->unOperand.stRawData.u8Length;i++)
    {
        CEC_PRINT("OpData[%d]:0x%x\n",i+1,pstCECCmd->unOperand.stRawData.u8Data[i]);
    }
    CEC_PRINT("\n");

    return ;
}

// some TV maybe triggle hotplug event when change the route.
// In this case,we will send <active source> msg every hotplug.so user can not change routting
HI_VOID ONE_TOUCH_PLAY_ACTIVE()
{
    HI_S32 Ret,i = 0;
    HI_UNF_HDMI_CEC_CMD_S stReplyCmd;
    HI_UNF_HDMI_CEC_STATUS_S  stCECStatus;

    //sleep(20);

    CEC_PRINT("Active source In One touch play ! \n");

    Ret = HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_0, &stCECStatus);
    if(Ret != HI_SUCCESS)
    {
        CEC_PRINT("HI_UNF_HDMI_CECStatus %d time ret:%d \n",i++ ,Ret);
        return ;
    }


    stReplyCmd.u8Opcode = CEC_OPCODE_ACTIVE_SOURCE;//0x82;
    stReplyCmd.enSrcAdd = stCECStatus.u8LogicalAddr;// our logical Addr
    stReplyCmd.enDstAdd = HI_UNF_CEC_LOGICALADD_BROADCAST; //(sii_cec->bDestOrRXHeader & 0xf0) >> 4 ;
    stReplyCmd.unOperand.stRawData.u8Length = 2;
    stReplyCmd.unOperand.stRawData.u8Data[0]     = ((stCECStatus.u8PhysicalAddr[0] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
    stReplyCmd.unOperand.stRawData.u8Data[1]     = ((stCECStatus.u8PhysicalAddr[2] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[3] & 0x0f) ; // [Physical Address(A.B.C.D):C D]

    HDMI_CEC_PrintCmd(&stReplyCmd);

    Ret = HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_0,&stReplyCmd);

    if(Ret != HI_SUCCESS)
    {
        CEC_PRINT("HI_UNF_HDMI_SetCECCommand ret:%d \n",Ret);
    }

}


HI_VOID ONE_TOUCH_PLAY_WakeUp()
{
    HI_S32 Ret,i = 0;
    HI_UNF_HDMI_CEC_CMD_S stReplyCmd;
    HI_UNF_HDMI_CEC_STATUS_S  stCECStatus;

    //sleep(20);

    CEC_PRINT("WakeUp In One touch play ! \n");

    Ret = HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_0, &stCECStatus);
    if(Ret != HI_SUCCESS)
    {
        CEC_PRINT("HI_UNF_HDMI_CECStatus %d time ret:%d \n",i++ ,Ret);
        return ;
    }

    // In spec defined follower as TV
    // send to tv
    stReplyCmd.u8Opcode = CEC_OPCODE_IMAGE_VIEW_ON;//0x04;
    stReplyCmd.enSrcAdd = stCECStatus.u8LogicalAddr;// our logical Addr
    stReplyCmd.enDstAdd = HI_UNF_CEC_LOGICALADD_TV; //(sii_cec->bDestOrRXHeader & 0xf0) >> 4 ;
    stReplyCmd.unOperand.stRawData.u8Length = 0;

    HDMI_CEC_PrintCmd(&stReplyCmd);

    Ret = HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_0,&stReplyCmd);

    if(Ret != HI_SUCCESS)
    {
        CEC_PRINT("HI_UNF_HDMI_SetCECCommand ret:%d \n",Ret);
    }

    return;

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    // but maybe in some special device,we need send a broad cast msg
    // broadcast
    stReplyCmd.u8Opcode = CEC_OPCODE_IMAGE_VIEW_ON;//0x82;
    stReplyCmd.enSrcAdd = stCECStatus.u8LogicalAddr;// our logical Addr
    stReplyCmd.enDstAdd = HI_UNF_CEC_LOGICALADD_BROADCAST; //(sii_cec->bDestOrRXHeader & 0xf0) >> 4 ;
    stReplyCmd.unOperand.stRawData.u8Length = 0;

    HDMI_CEC_PrintCmd(&stReplyCmd);

    Ret = HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_0,&stReplyCmd);

    if(Ret != HI_SUCCESS)
    {
        CEC_PRINT("HI_UNF_HDMI_SetCECCommand ret:%d \n",Ret);
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
}

HI_VOID ONE_TOUCH_PLAY()
{
    HI_UNF_HDMI_STATUS_S      stHdmiStatus;
    HI_UNF_HDMI_CEC_STATUS_S  stCECStatus;
    HI_U32                    Ret = HI_SUCCESS,i = 0;

    HI_UNF_HDMI_GetStatus(HI_UNF_HDMI_ID_0,&stHdmiStatus);
    if (HI_FALSE == stHdmiStatus.bConnected)
    {
        printf("No Connect\n");
        return;
    }

    //sleep(20);

    CEC_PRINT("ONE_TOUCH_PLAY Hotplug ! \n");

    do
    {
        Ret = HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_0, &stCECStatus);
        if(Ret != HI_SUCCESS)
        {
            CEC_PRINT("HI_UNF_HDMI_CECStatus %d time ret:%d \n",i++ ,Ret);
        }

        if(i == CEC_READY_TIME)
        {
            CEC_PRINT("Can not Get CEC Status %d times,return \n",i);
            return;
        }

        sleep(1);
    }
    while(Ret != HI_SUCCESS);

    if(HI_FALSE == stHdmiStatus.bSinkPowerOn)
    {
        CEC_PRINT("Wake Up \n");
        ONE_TOUCH_PLAY_WakeUp();
    }
    else
    {
        CEC_PRINT("Active Source \n");
        ONE_TOUCH_PLAY_ACTIVE();
    }
}

HI_VOID HDMI_CEC_CALLBACK(HI_UNF_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData)
{
    CEC_PRINT("Enter HDMI_CEC_CALLBACK event:%d \n",event);
    switch ( event )
    {
        case HI_UNF_HDMI_EVENT_HOTPLUG:
            ONE_TOUCH_PLAY();
            break;
        case HI_UNF_HDMI_EVENT_NO_PLUG:
            break;
        case HI_UNF_HDMI_EVENT_RSEN_CONNECT:
            //CEC_PRINT("HI_UNF_HDMI_EVENT_RSEN_CONNECT**********\n");
            break;
        case HI_UNF_HDMI_EVENT_RSEN_DISCONNECT:
            //CEC_PRINT("HI_UNF_HDMI_EVENT_RSEN_DISCONNECT**********\n");
            break;
        default:
            break;
    }
    /* Private Usage */
    if ((g_HDMIUserCallbackFlag == HI_TRUE) && (pfnHdmiUserCallback != NULL))
    {
        pfnHdmiUserCallback(event, NULL);
    }

    return;
}

//HI_U32 HDMI_Parse_CEC_Msg(HI_U8 Init, HI_U8 Dest, HI_U8 opcode, HI_U8 *pOperand, HI_U8 OperandLength, )
HI_U32 HDMI_Parse_CEC_Msg(HI_UNF_HDMI_CEC_CMD_S *pCECCmdGet,HI_UNF_HDMI_CEC_CMD_S *pCECCmdSet)
{
    //Default Return is Error NoSupprot!
    HI_U32 Ret = CEC_MESSAGE_RETURN_ERROR_NOSUPPORT;
    HI_U32 Rec;
    HI_UNF_HDMI_CEC_STATUS_S  stCECStatus;
    HI_U8 Init, Dest, opcode, OperandLength;
    HI_U8 *pOperand;
    HI_U8   u8Para1,u8Para2;

    Rec = HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_0, &stCECStatus);
    if(Rec != HI_SUCCESS)
    {
        CEC_PRINT("Get CEC Status Fail! return \n");
        return Ret;
    }

    if(stCECStatus.bEnable != HI_TRUE)
    {
        CEC_PRINT("CEC Disable! return \n");
        return Ret;
    }

    //add by sample
    Init = pCECCmdGet->enSrcAdd;
    Dest = pCECCmdGet->enDstAdd;
    opcode = pCECCmdGet->u8Opcode;
    pOperand = pCECCmdGet->unOperand.stRawData.u8Data;
    OperandLength = pCECCmdGet->unOperand.stRawData.u8Length;

    pCECCmdSet->enSrcAdd = stCECStatus.u8LogicalAddr;

    switch(opcode)
    {
    /*General Protocol messages*/
    case CEC_OPCODE_FEATURE_ABORT:                   //0X00
        CEC_PRINT("FEATURE ABORT Init:%d, Dest:%d, opcode:0x%x\n", Init, Dest, opcode);
        Ret = CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES; //need not response
        break;
    case CEC_OPCODE_ABORT_MESSAGE:                   //0XFF
        CEC_PRINT("ABORT MESSAGE\n");
        Ret = CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES; //need not response
        break;

    /*One Touch Play Feature*/
    case CEC_OPCODE_ACTIVE_SOURCE:                   //0X82
        CEC_PRINT("ACTIVE_SOURCE\n");
         // [Physical Address(A.B.C.D):A B]

        if(pOperand[0] != (((stCECStatus.u8PhysicalAddr[0] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[1] & 0x0f)) ||
            pOperand[1] != (((stCECStatus.u8PhysicalAddr[2] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[3] & 0x0f)))
        {
            g_cecPowerStatus = CEC_POWER_STATUS_STANDBY;
            CEC_PRINT("change to other output port,hdmi standby \n");
        }

        //CEC_PRINT("It should be source cmd, ignore!\n");
        break;
    case CEC_OPCODE_IMAGE_VIEW_ON:                   //0X04
        CEC_PRINT("IMAGE_VIEW_ON\n");
        CEC_PRINT("It should be source cmd, ignore!\n");
        break;
    case CEC_OPCODE_TEXT_VIEW_ON:                    //0X0D
        CEC_PRINT("TEXT_VIEW_ON\n");
        CEC_PRINT("It should be source cmd, ignore!\n");
        break;

    /*Routing Control Feature*/
    case CEC_OPCODE_INACTIVE_SOURCE:                 //0X9D
        CEC_PRINT("INACTIVE_SOURCE, No supprot\n");
        Ret = CEC_MESSAGE_RETURN_ERROR_NOSUPPORT;
        break;
    case CEC_OPCODE_REQUEST_ACTIVE_SOURCE:           //0X85
    //When a device(TV) comes out of the standy state, it maybe broacast a
    //<Request Active Source> message to discover active source.
    //the active source device shall respond by broadcasting an <Active Source> message.
    {
        CEC_PRINT("REQUEST_ACTIVE_SOURCE, return Active Source\n");
        //we should send back a message of ACTIVE_SOURCE
        pCECCmdSet->u8Opcode  = CEC_OPCODE_ACTIVE_SOURCE;
        pCECCmdSet->enDstAdd  = HI_UNF_CEC_LOGICALADD_BROADCAST;
        pCECCmdSet->unOperand.stRawData.u8Length         = 2;
        pCECCmdSet->unOperand.stRawData.u8Data[0]     = ((stCECStatus.u8PhysicalAddr[0] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
        pCECCmdSet->unOperand.stRawData.u8Data[1]     = ((stCECStatus.u8PhysicalAddr[2] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[3] & 0x0f) ; // [Physical Address(A.B.C.D):C D]
    }
        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_ROUTING_CHANGE:                  //0X80
        CEC_PRINT("ROUTING_CHANGE, it should be brocast message\n");
        break;
    case CEC_OPCODE_ROUTING_INFORMATION:             //0X81
        CEC_PRINT("ROUTING_INFORMATION, it should be brocast message\n");
        break;
    case CEC_OPCODE_SET_STREAM_PATH:                 //0X86
    //the <Set Stream Path> is sent by the TV to the source device
    //to request it to broacst its path using an <Active Source> message.

        CEC_PRINT("SET_STREAM_PATH\n");

        u8Para1 = ((stCECStatus.u8PhysicalAddr[0] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
        u8Para2 = ((stCECStatus.u8PhysicalAddr[2] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[3] & 0x0f) ;// [Physical Address(A.B.C.D):C D]


        if((pCECCmdGet->unOperand.stRawData.u8Data[0] == u8Para1) &&
            (pCECCmdGet->unOperand.stRawData.u8Data[1] == u8Para2))
        {
            CEC_PRINT("STREAM_PATH same as our Tx ,request \n");
            //we should send back a message of ACTIVE_SOURCE
            pCECCmdSet->u8Opcode = CEC_OPCODE_ACTIVE_SOURCE;
            pCECCmdSet->enDstAdd = HI_UNF_CEC_LOGICALADD_BROADCAST;
            pCECCmdSet->unOperand.stRawData.u8Length = 2;
            pCECCmdSet->unOperand.stRawData.u8Data[0] = u8Para1;
            pCECCmdSet->unOperand.stRawData.u8Data[1] = u8Para2;
        }

        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    /*Standby Feature*/
    case CEC_OPCODE_STANDBY:                         //0X36
        CEC_PRINT("STANDBY! It should brocast message\n");
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
        CEC_PRINT("CEC_VERSION:\n");
        CEC_PRINT("OperandLength:0x%02x \n",OperandLength);
        switch (pOperand[0])
        {
            case 0x04 :
                CEC_PRINT("Version 1.3a\n");
                break;
            case 0x05 :
                CEC_PRINT("Version 1.4 or Version 1.4a or Version 1.4b \n");
                break;
            default:
                CEC_PRINT("unknown CEC_VERSION \n");
        }
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        {
            int index;
            for(index = 0; index < OperandLength; index ++)
            {
                if( ((pOperand[index] >= 'a') && (pOperand[index] <= 'z'))
                 || ((pOperand[index] >= 'A') && (pOperand[index] <= 'Z')) )
                {
                    CEC_PRINT("%c", pOperand[index]);
                }
                else
                {
                    CEC_PRINT("%02x", pOperand[index]);
                }
            }
        }
        CEC_PRINT("\n");
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        break;
    case CEC_OPCODE_GET_CEC_VERSION:                 //0X9F
        CEC_PRINT("GET_CEC_VERSION, Response\n");
        //we should send back a message of ACTIVE_SOURCE
        pCECCmdSet->u8Opcode   = CEC_OPCODE_CEC_VERSION;
        pCECCmdSet->enDstAdd = Init;
        pCECCmdSet->unOperand.stRawData.u8Length         = 1;
        //hdmi spec1.4b cec spec page.108
        // 0x04 cec 1.3a   0x05 cec 1.4/1.4a/1.4b
        pCECCmdSet->unOperand.stRawData.u8Data[0]     = 0x04;

#if 0
        pCECframe->bCount          = 3;
        pCECframe->bOperand[0]     = '1';
        pCECframe->bOperand[1]     = '3' ;
        pCECframe->bOperand[2]     = 'a' ;
#endif
        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_GIVE_PHYSICAL_ADDRESS:           //0X83
        CEC_PRINT("GIVE_PHYSICAL_ADDRESS, Response\n");
    {
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        HI_U8 PhyAddr[4], LogAddr;
        HI_U32 Rec;
        Rec = DRV_HDMI_GetCECAddress(PhyAddr, &LogAddr);

        if(Rec != HI_SUCCESS)
        {
            break;
        }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        CEC_PRINT("return REPORT_PHYSICAL_ADDRESS\n");
        //we should send back a message of ACTIVE_SOURCE
        pCECCmdSet->u8Opcode   = CEC_OPCODE_REPORT_PHYSICAL_ADDRESS;
        pCECCmdSet->enDstAdd = Init;
        pCECCmdSet->unOperand.stRawData.u8Length          = 3;
        pCECCmdSet->unOperand.stRawData.u8Data[0]     = ((stCECStatus.u8PhysicalAddr[0] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
        pCECCmdSet->unOperand.stRawData.u8Data[1]     = ((stCECStatus.u8PhysicalAddr[2] << 4) & 0xf0) | (stCECStatus.u8PhysicalAddr[3] & 0x0f) ; // [Physical Address(A.B.C.D):C D]
        pCECCmdSet->unOperand.stRawData.u8Data[2]     = stCECStatus.u8LogicalAddr;
    }
        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_REPORT_PHYSICAL_ADDRESS:         //0X84
        CEC_PRINT("REPORT_PHYSICAL_ADDRESS\n");
        if(OperandLength != 3)
        {
            CEC_PRINT("Error oprand length:%d\n", OperandLength);
        }
        CEC_PRINT("PhysicalAddr:%01d.%01d.%01d.%01d, LogicalAddr:%d\n", (pOperand[0] & 0xf0) >> 4, (pOperand[0] & 0x0f), \
         (pOperand[1] & 0xf0) >> 4, (pOperand[1] & 0x0f), pOperand[2]);

        Ret = CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;
    case CEC_OPCODE_GET_MENU_LANGUAGE:               //0X91
        CEC_PRINT("GET_MENU_LANGUAGE, response\n");
        pCECCmdSet->u8Opcode   = CEC_OPCODE_SET_MENU_LANGUAGE;
        pCECCmdSet->enDstAdd   = Init;
        pCECCmdSet->unOperand.stRawData.u8Length          = 3;


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
        pCECCmdSet->unOperand.stRawData.u8Data[0]     = 'c';
        pCECCmdSet->unOperand.stRawData.u8Data[1]     = 'h' ;
        pCECCmdSet->unOperand.stRawData.u8Data[2]     = 'i' ;
#endif

        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_SET_MENU_LANGUAGE:               //0X32
        CEC_PRINT("SET_MENU_LANGUAGE:");
        {
            int index;
            for(index = 0; index < OperandLength; index ++)
            {
                if( ((pOperand[index] >= 'a') && (pOperand[index] <= 'z'))
                 || ((pOperand[index] >= 'A') && (pOperand[index] <= 'Z')) )
                {
                    CEC_PRINT("%c", pOperand[index]);
                }
                else
                {
                    CEC_PRINT("%02x", pOperand[index]);
                }
            }
        }
        CEC_PRINT("\n");
        break;

    /*Deck Control Feature*/
    case CEC_OPCODE_DECK_CONTROL:                    //0X42
        CEC_PRINT("DECK_CONTROL\n");
        break;
    case CEC_OPCODE_DECK_STATUS:                     //0X1B
        CEC_PRINT("DECK_STATUS\n");
        break;
    case CEC_OPCODE_GIVE_DECK_STATUS:                //0X1A
        CEC_PRINT("GIVE_DECK_STATUS\n");
        break;
    case CEC_OPCODE_PLAY:                            //0X41
        CEC_PRINT("PLAY\n");
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
        CEC_PRINT("DEVICE_VENDOR_ID:");
        {
            int index;
            for(index = 0; index < OperandLength; index ++)
            {
                CEC_PRINT("%02x ", pOperand[index]);
            }
        }
        CEC_PRINT("\n");
        break;
    case CEC_OPCODE_GIVE_DEVICE_VENDOR_ID:           //0X8C
        CEC_PRINT("GIVE_DEVICE_VENDOR_ID, Response\n");
        pCECCmdSet->u8Opcode   = CEC_OPCODE_DEVICE_VENDOR_ID;
        pCECCmdSet->enDstAdd = 0x0f;
        pCECCmdSet->unOperand.stRawData.u8Length          = 3;
        pCECCmdSet->unOperand.stRawData.u8Data[0]     = 'h';
        pCECCmdSet->unOperand.stRawData.u8Data[1]     = 'i' ;
        pCECCmdSet->unOperand.stRawData.u8Data[2]     = 's' ;

        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
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
        CEC_PRINT("SET_OSD_NAME:%s\n", pOperand);
        Ret = CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;
    case CEC_OPCODE_GIVE_OSD_NAME:                   //0X46
        CEC_PRINT("CEC_OPCODE_GIVE_OSD_NAME, Response\n");
        pCECCmdSet->u8Opcode   = CEC_OPCODE_SET_OSD_NAME;
        pCECCmdSet->enDstAdd = Init;
        pCECCmdSet->unOperand.stRawData.u8Length          = 3;
        pCECCmdSet->unOperand.stRawData.u8Data[0]     = 'h';
        pCECCmdSet->unOperand.stRawData.u8Data[1]     = 'i' ;
        pCECCmdSet->unOperand.stRawData.u8Data[2]     = 's' ;

        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_SET_OSD_NAME:                    //0X47
        CEC_PRINT("SET_OSD_NAME:");
        {
            int index;
            for(index = 0; index < OperandLength; index ++)
            {
                if( ((pOperand[index] >= 'a') && (pOperand[index] <= 'z'))
                 || ((pOperand[index] >= 'A') && (pOperand[index] <= 'Z')) )
                {
                    CEC_PRINT("%c", pOperand[index]);
                }
                else
                {
                    CEC_PRINT("%02x", pOperand[index]);
                }
            }
        }
        CEC_PRINT("\n");
        break;

    /*Device Menu Control Feature*/
    case CEC_OPCODE_MENU_REQUEST:                    //0X8D
        CEC_PRINT("MENU_REQUEST\n");
        if(OperandLength < 1)
        {
            CEC_PRINT("Invalid OperandLength:%d\n", OperandLength);
            break;
        }

        // Send back Menu STATUS
        pCECCmdSet->u8Opcode   = CEC_OPCODE_MENU_STATUS;
        pCECCmdSet->enDstAdd = Init;
        pCECCmdSet->unOperand.stRawData.u8Length          = 1;

        if(pOperand[0] == 0x00)
        {
            CEC_PRINT("Active\n");
            pCECCmdSet->unOperand.stRawData.u8Data[0]     = 0X00;//Active
        }
        else if(pOperand[0] == 0x01)
        {
            CEC_PRINT("Deactive\n");
            pCECCmdSet->unOperand.stRawData.u8Data[0]     = 0X01;//Deactive
        }
        else if(pOperand[0] == 0x02)
        {
            CEC_PRINT("Query\n");
        }
        else
        {
            CEC_PRINT("Unknown, pOperand[0]:0x%x\n", pOperand[0]);
            break;
        }

        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_MENU_STATUS:                     //0X8E
        CEC_PRINT("MENU_STATUS:\n");
        if(OperandLength < 1)
        {
            CEC_PRINT("Invalid OperandLength:%d\n", OperandLength);
            break;
        }
        if(pOperand[0] == 0x00)
        {
            CEC_PRINT("Active\n");
        }
        else if(pOperand[0] == 0x01)
        {
            CEC_PRINT("Deactive\n");
        }

        Ret = CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;
    case CEC_OPCODE_USER_CONTROL_PRESSED:            //0X44
        CEC_PRINT("USER_CONTROL_PRESSED\n");
        CEC_PRINT("Press RC:0x%02x\n", pOperand[0]);
        Ret = CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;
    case CEC_OPCODE_USER_CONTROL_RELEASED:           //0X45
        CEC_PRINT("USER_CONTROL_RELEASED\n");
        CEC_PRINT("Release RC:0x%02x\n", pOperand[0]);
        Ret = CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;

    /*Power Status Feature*/
    case CEC_OPCODE_GIVE_DEVICE_POWER_STATUS:        //0X8F
        CEC_PRINT("GIVE_DEVICE_POWER_STATUS, response\n");

        pCECCmdSet->u8Opcode   = CEC_OPCODE_REPORT_POWER_STATUS;
        pCECCmdSet->enDstAdd = Init;
        pCECCmdSet->unOperand.stRawData.u8Length          = 1;
        pCECCmdSet->unOperand.stRawData.u8Data[0]     = g_cecPowerStatus;//Power on

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        if(g_cecPowerStatus < CEC_POWER_STATUS_ON2STANDBY)
        {
            g_cecPowerStatus++;
        }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        switch(g_cecPowerStatus)
        {
            case CEC_POWER_STATUS_ON:
                //g_cecPowerStatus = CEC_POWER_STATUS_ON2STANDBY;
                break;
            case CEC_POWER_STATUS_STANDBY:
                g_cecPowerStatus = CEC_POWER_STATUS_STANDBY2ON;
                break;
            case CEC_POWER_STATUS_STANDBY2ON:
                g_cecPowerStatus = CEC_POWER_STATUS_ON;
                break;
            case CEC_POWER_STATUS_ON2STANDBY:
                g_cecPowerStatus = CEC_POWER_STATUS_STANDBY;
                break;
            default:
                CEC_PRINT("Unknown Power status \n");
        }

        Ret = CEC_MESSAGE_RETURN_OK_WITH_RESPONES;
        break;
    case CEC_OPCODE_REPORT_POWER_STATUS:             //0X90
        CEC_PRINT("REPORT_POWER_STATUS\n");
        if(OperandLength < 1)
        {
            CEC_PRINT("Invalid OperandLength:%d\n", OperandLength);
            break;
        }
        if(pOperand[0] == 0x00)
        {
            CEC_PRINT("Power On\n");
        }
        else if(pOperand[0] == 0x01)
        {
            CEC_PRINT("Stand By\n");
        }
        else
        {
            CEC_PRINT("Unknown, pOperand[0]:0x%x\n", pOperand[0]);
        }
        Ret = CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES;
        break;

    /*System Audio Control Feature*/
    case CEC_OPCODE_GIVE_AUDIO_STATUS:               //0X71
        CEC_PRINT("GIVE_AUDIO_STATUS\n");
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



HI_VOID HDMI_CEC_Proc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S *pstCECCmd, HI_VOID *pData)
{
    HI_UNF_HDMI_CEC_CMD_S stReplyCmd;
    int ret;


    HDMI_CEC_PrintCmd(pstCECCmd);

    ret = HDMI_Parse_CEC_Msg(pstCECCmd,&stReplyCmd);

    if(ret == CEC_MESSAGE_RETURN_OK_WITH_RESPONES)
    {
        CEC_PRINT("response \n");
        HDMI_CEC_PrintCmd(&stReplyCmd);
        HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_0,&stReplyCmd);
        return ;
    }
    else if(ret == CEC_MESSAGE_RETURN_OK_WITHOUT_RESPONES)
    {
        CEC_PRINT("not need response \n");
        return ;//direct return!
    }
    else if(ret == CEC_MESSAGE_RETURN_ERROR_NOSUPPORT)
    {
        CEC_PRINT("unsupport cec cmd \n");
        //return ;
    }

    // Deal with Unsupport Message!    //
    // Do not reply to Broadcast msgs, otherwise send Feature Abort
    // for all unsupported features.
    if( (pstCECCmd->enSrcAdd & 0x0F) != 0x0F)
    {
        HI_U32 Rec;
        HI_UNF_HDMI_CEC_STATUS_S  stCECStatus;

        Rec = HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_0, &stCECStatus);
        if(Rec != HI_SUCCESS)
        {
            CEC_PRINT("Get CEC Status Fail! return \n");
            return ;
        }

        /*All devices shall support the message <Feature Abort>.
          It is used to allow devices to indicate if they do not support an opcode that has been
          sent to them, if it is unable to deal with this message.
          <Feature Abort> has 2 parameters: opcode and the reason of its reject of the frame.
          <Feature Abort> is used as a response to any failure!
        */
        // Unsupported opcode; send feature abort
        //<Feature Abort>has 2 parameters, the opcode and a reason for its rejection of the frame.
        memset(&stReplyCmd, 0, sizeof(HI_UNF_HDMI_CEC_CMD_S));
        stReplyCmd.u8Opcode = CEC_OPCODE_FEATURE_ABORT;//0x00;
        stReplyCmd.enSrcAdd = stCECStatus.u8LogicalAddr;// our logical Addr
        stReplyCmd.enDstAdd = pstCECCmd->enSrcAdd; //(sii_cec->bDestOrRXHeader & 0xf0) >> 4 ;
        stReplyCmd.unOperand.stRawData.u8Data[0]     = pstCECCmd->u8Opcode;
        stReplyCmd.unOperand.stRawData.u8Data[1]     = 0x00; //Unrecognized opcode:0x00
        stReplyCmd.unOperand.stRawData.u8Length = 2;

        HDMI_CEC_PrintCmd(&stReplyCmd);
        HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_0,&stReplyCmd);
        CEC_PRINT("\n!Unsupported bOpcode:0x%x Feature!\n", pstCECCmd->u8Opcode);
    }
    else
    {
        // Unsupported Broadcast Msg
        CEC_PRINT("\n! Unsupported Brocast Msg!\n");
    }

    return;
}


extern HI_U32 HDMI_Test_CMD(HI_CHAR * u8String);

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_U32 Ret = HI_FAILURE;
    HI_CHAR InputCmd[128];

    HI_SYS_Init();
    CEC_PRINT("\n It's a sub process\n");

    Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    if (HI_SUCCESS != Ret)
    {
        CEC_PRINT("call HIADP_Disp_Init failed.\n");
        return HI_SUCCESS;
    }

    HI_UNF_HDMI_CEC_Enable(HI_UNF_HDMI_ID_0);

    //only use for ont touch play
    g_stCallbackFunc.pfnHdmiEventCallback = HDMI_CEC_CALLBACK;
    g_stCallbackFunc.pPrivateData = HI_NULL;

    Ret = HI_UNF_HDMI_RegCallbackFunc(HI_UNF_HDMI_ID_0, &g_stCallbackFunc);
    if (Ret != HI_SUCCESS)
    {
        CEC_PRINT("hdmi reg failed:%#x\n",Ret);
        HI_UNF_HDMI_DeInit();
        return HI_FAILURE;
    }

    //HI_UNF_HDMI_CEC_Enable(HI_UNF_HDMI_ID_0);
    HI_UNF_HDMI_RegCECCallBackFunc(HI_UNF_HDMI_ID_0, HDMI_CEC_Proc);

    //for bk color
    HI_UNF_DISP_BG_COLOR_S BgColor;

    BgColor.u8Red   = 0;
    BgColor.u8Green = 200;
    BgColor.u8Blue  = 200;
    HI_UNF_DISP_SetBgColor(HI_UNF_DISPLAY1, &BgColor);


    while(1)
    {
        CEC_PRINT("please input 'h' to get help or 'q' to quit!\n");
        CEC_PRINT("hdmi_cmd >");
        memset(InputCmd, 0, 128);
        SAMPLE_GET_INPUTCMD(InputCmd);
        if ('q' == InputCmd[0])
        {
            CEC_PRINT("prepare to quit!\n");
            break;
        }


        if ('o' == InputCmd[0])
        {
            CEC_PRINT("One Touch Play! \n");
            ONE_TOUCH_PLAY();
            continue;
        }

        HDMI_Test_CMD(InputCmd);
    }

    HI_UNF_HDMI_UnRegCECCallBackFunc(HI_UNF_HDMI_ID_0, HDMI_CEC_Proc);
    HI_UNF_HDMI_UnRegCallbackFunc(HI_UNF_HDMI_ID_0, &g_stCallbackFunc);

    HI_UNF_HDMI_CEC_Disable(HI_UNF_HDMI_ID_0);
    HIADP_Disp_DeInit();

    return HI_SUCCESS;
}



