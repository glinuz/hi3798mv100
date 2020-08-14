/**
\file
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author xxxx
\date 2015-03-13
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_Index.h>
#include <OMX_Image.h>
#include <OMX_Audio.h>
#include <OMX_Video.h>
#include <OMX_IVCommon.h>
#include <OMX_Other.h>

#include "omx_audio_base.h"
#include "hi_unf_audio.h"
#include "hi_unf_sound.h"
#include "hi_common.h"
#include "hi_adp_mpi.h"




#ifdef  ANDROID
#undef  LOG_TAG
#define LOG_TAG    "HIOMX_SAMPLEPLAY"
#endif



#define APP_DPRINT(...)   fprintf(stderr, __VA_ARGS__)
#define PRINTF_ERR(str)   fprintf(stderr, "\n\n\e[40m\e[31m\e[1m**Err :  %s**\e[0m\n\n", (str))

#define ERR_NO_ERROR          0
#define ERR_INVALID_COMMAND  -1


#define AMR_TIMING_PROFILE
#ifdef AMR_TIMING_PROFILE
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
struct timeval     timeBegin, timeEnd;
unsigned long long u64TimeB,  u64TimeE;
#endif


typedef struct
{
    unsigned int   dwRIFF;
    unsigned int   dwFileLen;
    unsigned int   dwWAVE;
    unsigned int   dw_fmt;
    unsigned int   dwFmtLen;
    unsigned short wDataType;
    unsigned short wNChannels;
    unsigned int   dwSamplingRate;
    unsigned int   dwNBytesPerSec;
    unsigned short wAlignment;
    unsigned short wNBitsPerSam;
    unsigned int   dwdata;
    unsigned int   dwDataLen;
} WAVEHEADER;


static void GT_START()
{
#ifdef AMR_TIMING_PROFILE
    gettimeofday(&timeBegin, NULL);
    u64TimeB = timeBegin.tv_sec * 1000000LLU + timeBegin.tv_usec;
#endif
}

static void GT_END(char* funcName)
{
#ifdef AMR_TIMING_PROFILE
    gettimeofday(&timeEnd, NULL);
    u64TimeE = timeEnd.tv_sec * 1000000LLU + timeEnd.tv_usec;
    fprintf (stderr, "%s time cosumed =%f \n", funcName, (u64TimeE - u64TimeB) / 1000.0);
#endif
}


OMX_STRING HA_OMX_DecTable[5] =
{
    "OMX.hisi.audio.aacdec",
    "OMX.hisi.audio.mp3dec",
    "OMX.hisi.audio.ddpdec",
    NULL,
};


typedef struct AudioDecAppDATATYPE
{
    OMX_STATETYPE state;
    HI_HANDLE     hTrack;
    HI_BOOL       bAudioPlay;
    HI_BOOL       bSaveOutData;
    char          infilename[256];
    char          outfilename[256];
    WAVEHEADER    WvH;
    OMX_S32       mEndPlaying;
    OMX_S32       mEndInputBuf;
    OMX_S32       s32Samplerate;
    OMX_S32       s32ChannelOut;
    FILE*         fileIn;
    FILE*         fileOut;
    OMX_STRING    strDecoder;
} AudioDecAppDATATYPE;


/****************************************************************************
* NAME        : WriteHeader()                                              *
* DESCRIPTION : Write wav file header structure                            *
****************************************************************************/
static HI_VOID WriteHeader(WAVEHEADER* WvH, int samplerate, int channelnumber)
{
    WvH->dwSamplingRate = samplerate;

    WvH->dwRIFF       = 0x46464952;
    WvH->dwWAVE       = 0x45564157;
    WvH->dw_fmt       = 0x20746d66;
    WvH->dwFmtLen     = 0x10;
    WvH->wDataType    = 1;
    WvH->wNChannels   = channelnumber;
    WvH->wNBitsPerSam = 16;
    WvH->dwdata = 0x61746164;
    WvH->dwNBytesPerSec = WvH->dwSamplingRate * 2 * WvH->wNChannels;
    WvH->wAlignment = 4;

    WvH->dwFileLen = WvH->dwDataLen + 16 + 20;
}


static HI_U32 fill_data (OMX_BUFFERHEADERTYPE* pBuf, FILE* fileIn)
{
    HI_U32 nRead;

    nRead = fread(pBuf->pBuffer, 1, pBuf->nAllocLen, fileIn);

    pBuf->nFilledLen = nRead;
    pBuf->nOffset    = 0;
    return nRead;
}


static OMX_ERRORTYPE send_input_buffer(OMX_HANDLETYPE        pHandle,
                                       OMX_BUFFERHEADERTYPE* pBuffer,
                                       AudioDecAppDATATYPE*  pAppPriData)
{
    HI_U32        nRead;
    OMX_ERRORTYPE error = OMX_ErrorNone;

    nRead = fill_data(pBuffer, pAppPriData->fileIn);

    /*Don't send more buffers after OMX_BUFFERFLAG_EOS*/
    if (pAppPriData->mEndInputBuf)
    {
        pBuffer->nFlags = 0;
        APP_DPRINT("%d : APP:: Entering send_input_buffer finish,pBuffer=%p \n", __LINE__, pBuffer);
        return error;
    }

    if ((nRead < pBuffer->nAllocLen) && (pAppPriData->mEndInputBuf == 0))
    {
        pBuffer->nFlags = OMX_BUFFERFLAG_EOS;
        pAppPriData->mEndInputBuf = 1;
        APP_DPRINT("%d : APP:: Entering send_input_buffer OMX_BUFFERFLAG_EOS, pBuffer=%p \n", __LINE__, pBuffer);
    }
    else
    {
        pBuffer->nFlags = 0;
    }

    /*time stamp & tick count value*/
    pBuffer->nTimeStamp = rand() % 100;
    pBuffer->nTickCount = rand() % 70;

    pBuffer->nFilledLen = nRead;
    OMX_EmptyThisBuffer(pHandle, pBuffer);

    return error;
}


/* This method will wait for the component to get to the state
 * specified by the DesiredState input. */
static OMX_ERRORTYPE WaitForState(OMX_HANDLETYPE* pHandle, OMX_STATETYPE DesiredState)
{
    int nCnt = 0;
    OMX_STATETYPE CurState = OMX_StateReserved_0x00000000;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_COMPONENTTYPE* pComponent = (OMX_COMPONENTTYPE*)pHandle;

    eError = pComponent->GetState(pHandle, &CurState);
    if (CurState == OMX_StateReserved_0x00000000)
    {
        eError = OMX_ErrorReserved_0x8000100A;
    }

    while ((eError == OMX_ErrorNone) && (CurState != DesiredState))
    {
        sched_yield();
        if (nCnt++ == 10)
        {
            APP_DPRINT( "Still Waiting, press CTL-C to continue\n");
        }

        eError = pComponent->GetState(pHandle, &CurState);
        if (CurState == OMX_StateReserved_0x00000000)
        {
            printf("\nINVALID STATE \n\n");
            eError = OMX_ErrorReserved_0x8000100A;
        }
    }

    if (eError != OMX_ErrorNone)
    {
        return eError;
    }

    return OMX_ErrorNone;
}


static OMX_ERRORTYPE EventHandler(OMX_OUT OMX_HANDLETYPE aComponent,
                                  OMX_OUT OMX_PTR        aAppData,
                                  OMX_OUT OMX_EVENTTYPE  aEvent,
                                  OMX_OUT OMX_U32        aData1,
                                  OMX_OUT OMX_U32        aData2,
                                  OMX_OUT OMX_PTR        aEventData)
{
    AudioDecAppDATATYPE* pAppPriData = (AudioDecAppDATATYPE*)aAppData;
    if (OMX_EventCmdComplete == aEvent)
    {
        if (OMX_CommandStateSet == aData1)
        {
            switch ((OMX_S32) aData2)
            {
                    //Falling through next case
                case OMX_StateReserved_0x00000000:
                    printf("OMX_StateReserved_0x00000000\n");
                    break;
                case OMX_StatePause:
                    printf("OMX_StatePause\n");
                    break;
                case OMX_StateWaitForResources:
                    printf("OMX_StateWaitForResources\n");
                    break;
                case OMX_StateLoaded:
                    printf("OMX_StateLoaded\n");
                    break;
                case OMX_StateIdle:
                    printf("OMX_StateIdle\n");
                    break;
                case OMX_StateExecuting:
                    printf("OMX_StateExecuting\n");
                    break;
                default:
                    printf("OMX_State unknow cmd=%ld\n", (OMX_U32)aData2);
                    break;
            }
        }
        else if (OMX_CommandPortDisable == aData1)
        {
            printf("OMX_CommandPortDisable\n");
        }
        else if (OMX_CommandPortEnable == aData1)
        {
            //Change the state from Reconfig to Executing on receiving this callback
            printf("OMX_CommandPortEnable\n");
        }
    }
    else if (OMX_EventPortSettingsChanged == aEvent)
    {
        printf("OMX_EventPortSettingsChanged\n");
    }
    else if (OMX_EventBufferFlag == aEvent)
    {
        //Change the state on receiving EOS callback, aData1==nOutputPortIndex
        if ((aData2 == (OMX_U32)OMX_BUFFERFLAG_EOS) && (aData1 == (OMX_U32)1))
        {
            pAppPriData->mEndPlaying = 1;
        }

        printf("EOS Event recieved\n");
    }
    else if (OMX_EventError == aEvent)
    {
        if (OMX_ErrorSameState == (OMX_ERRORTYPE)aData1)
        {
            printf("Component reported SameState Error, try to proceed\n");
        }
        else if (OMX_ErrorStreamCorrupt == (OMX_ERRORTYPE)aData1)
        {
            /* Don't do anything right now for the stream corrupt error,
             * just count the number of such callbacks and let the component to proceed */

            //printf("Component/Encoder reported Stream Corrupt Error\n");
        }
        else
        {
            // do nothing, just try to proceed normally
        }
    }

    return OMX_ErrorNone;
}


static OMX_ERRORTYPE EmptyBufferDone(OMX_OUT OMX_HANDLETYPE        hComponent,
                                     OMX_OUT OMX_PTR               pAppData,
                                     OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer)
{
    OMX_ERRORTYPE error;

    //Check the validity of buffer
    if ((NULL == pBuffer) || (NULL == pAppData))
    {
        return OMX_ErrorBadParameter;
    }

    error = send_input_buffer(hComponent, pBuffer, (AudioDecAppDATATYPE*)pAppData);
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT( "In %s Error %08x Calling FillThisBuffer\n", __func__, error);
        return error;
    }

    return OMX_ErrorNone;
}


static OMX_ERRORTYPE FillBufferDone(OMX_OUT OMX_HANDLETYPE        hComponent,
                                    OMX_OUT OMX_PTR               pAppData,
                                    OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer)
{
    HI_S32        s32Ret;
    OMX_ERRORTYPE error;
    HI_U32        u32PcmBytesPerFrame;
    OMX_COMPONENTTYPE*    pComponent  = (OMX_COMPONENTTYPE*)hComponent;
    AudioDecAppDATATYPE*  pAppPriData = (AudioDecAppDATATYPE*)pAppData;
    HI_UNF_AO_FRAMEINFO_S stAOFrame;
    HI_AUDDATATYPE* pHAData = (HI_AUDDATATYPE*)(pComponent->pComponentPrivate);
    
    u32PcmBytesPerFrame = pHAData->sPcm.nSize * sizeof(HI_S16) * pHAData->sPcm.nChannels;
    
    //Check the validity of buffer
    if ((NULL == pBuffer) || (NULL == pAppPriData))
    {
        return OMX_ErrorBadParameter;
    }

    if (HI_TRUE == pAppPriData->bSaveOutData)
    {
        if ((u32PcmBytesPerFrame > 0) && pAppPriData->fileOut)
        {
            fwrite(pBuffer->pBuffer, 1, u32PcmBytesPerFrame, pAppPriData->fileOut);
            pAppPriData->WvH.dwDataLen += u32PcmBytesPerFrame;
        }
    }

    if (HI_TRUE == pAppPriData->bAudioPlay)
    {
        while (1)
        {
            stAOFrame.u32FrameIndex         = 0;
            stAOFrame.s32BitPerSample       = 16;
            stAOFrame.bInterleaved          = HI_TRUE;
            stAOFrame.u32PcmSamplesPerFrame = pHAData->sPcm.nSize;
            stAOFrame.u32Channels           = pHAData->sPcm.nChannels;
            stAOFrame.u32SampleRate         = pHAData->sPcm.nSamplingRate;
            stAOFrame.u32PtsMs              = pBuffer->nTimeStamp;
            stAOFrame.ps32PcmBuffer         = (HI_S32*)(pBuffer->pBuffer);

            if (pHAData->stAdec.u32BitstreamOffset != 0)
            {
                stAOFrame.ps32BitsBuffer       = (HI_S32*)(pBuffer->pBuffer + pHAData->stAdec.u32BitstreamOffset);
                stAOFrame.u32BitsBytesPerFrame = pBuffer->nFilledLen - pHAData->sPcm.nSize * sizeof(HI_S16) * pHAData->sPcm.nChannels;
            }
            else
            {
                stAOFrame.ps32BitsBuffer       = HI_NULL;
                stAOFrame.u32BitsBytesPerFrame = 0;
            }

            s32Ret = HI_UNF_SND_SendTrackData(pAppPriData->hTrack, &stAOFrame);
            if (HI_SUCCESS == s32Ret)
            {
                break;
            }
            else if (HI_FAILURE == s32Ret)
            {
                printf("HI_UNF_SND_SendTrackData error\n");
                break;
            }
            else
            {
                usleep(1000);
            }
        }
    }


    pBuffer->nFilledLen = 0;
    error = OMX_FillThisBuffer(hComponent, pBuffer);
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT( "In %s Error %08x Calling FillThisBuffer\n", __func__, error);
        return error;
    }

    return OMX_ErrorNone;
}


OMX_CALLBACKTYPE AdecCallbacks =
{
    .EventHandler    = EventHandler,
    .EmptyBufferDone = EmptyBufferDone,
    .FillBufferDone  = FillBufferDone
};


/*****************************************************************\
* showusage: show the usage message
\*****************************************************************/
static int showusage()
{
    puts("\n"
         "Usage:  sample_omx_play -i [inputfilename] [OPTIONS]\n"
         "\n");
    puts(
         "         -h     Show this usage message and abort\n"
         "         -i     Input file name\n");
    puts("         -t     The type of play audio(aac/mp3/ddp)\n"
         "         -p     Whether play the audio online\n"
         "                  0 = not  play (default)\n"
         "                  1 = need play\n"
         "         -o     The output file name to save\n"
         "                NOTE:If using the parameter, it would save the WAV file\n"
        );
    puts("\n"
         "For example: ./sample_omx_play -i /mnt/afraic_48k.ac3 -t ddp -p1 -o /mnt/afraic.wav\n"
         "\n");

    return (ERR_NO_ERROR);
}


/*****************************************************************
  ParseCmdLine: parses the command-line input
*****************************************************************/
static int ParseCmdLine(int argc, char* p_argv[], AudioDecAppDATATYPE* pAppPriData)
{
    /* declare local variables */
    int         count;
    const char* p_input;

    /* check input arguments */
    if ((p_argv == NULL) || (argc < 2))
    {
        showusage();
        return ERR_INVALID_COMMAND;
    }

    /* parse command line */
    for (count = 1; count < argc; count++)
    {
        if (*(p_argv[count]) == '-')
        {
            p_input = p_argv[count] + 1;

            switch (*(p_input))
            {
                case 'h':
                case 'H':
                    if (*(p_input + 1) != '\0')
                    {
                        fprintf(stderr, "Invalid help usage (-%s)\n", p_input);
                        return ERR_INVALID_COMMAND;
                    }
                    showusage();
                    return ERR_INVALID_COMMAND;
                case 'i':
                case 'I':
                    if (count + 1 < argc)
                    {
                        strcpy(pAppPriData->infilename, p_argv[count + 1]);
                        count++;
                    }
                    else
                    {
                        fprintf(stderr, "Invalid command line argument\n");
                        return ERR_INVALID_COMMAND;
                    }
                    break;
                case 'o':
                case 'O':
                    if (count + 1 < argc)
                    {
                        pAppPriData->bSaveOutData = HI_TRUE;
                        strcpy(pAppPriData->outfilename, p_argv[count + 1]);
                        count++;
                    }
                    else
                    {
                        fprintf(stderr, "Invalid command line argument\n");
                        return ERR_INVALID_COMMAND;                        
                    }
                    break;
                case 'p':
                case 'P':
                    pAppPriData->bAudioPlay = (HI_BOOL)atoi(p_input + 1);
                    break;
                case 't':
                case 'T':
                    if (count + 1 < argc)
                    {
                        if (!strcasecmp("aac", p_argv[count + 1]))
                        {
                            pAppPriData->strDecoder = HA_OMX_DecTable[0];
                        }
                        else if (!strcasecmp("mp3", p_argv[count + 1]))
                        {
                            pAppPriData->strDecoder = HA_OMX_DecTable[1];
                        }
                        else if (!strcasecmp("ddp", p_argv[count + 1]))
                        {
                            pAppPriData->strDecoder = HA_OMX_DecTable[2];
                        }
                        else
                        {
                            fprintf(stderr, "unsupported adec type (-%s)\n", p_argv[count + 1]);
                            return ERR_INVALID_COMMAND;
                        }
                        count++;
                    }
                    else
                    {
                        fprintf(stderr, "Invalid command line argument\n");
                        return ERR_INVALID_COMMAND;                         
                    }
                    break;
                default:
                    fprintf(stderr, "Invalid command line argument (-%s)\n", p_input);
                    return ERR_INVALID_COMMAND;
            }
        }
        else
        {
            fprintf(stderr, "Invalid command line argument (?)\n");
            return ERR_INVALID_COMMAND;
        }
    }

    return ERR_NO_ERROR;
}


int main(int argc, char** argv)
{
    HI_S32         s32Ret = HI_SUCCESS;
    OMX_S32        nInportBufSize;
    OMX_S32        nOutportBufSize;
    OMX_S32        i, j;
    OMX_ERRORTYPE  error;
    OMX_HANDLETYPE audiodechandle;
    AudioDecAppDATATYPE sAppPriData;
    OMX_BUFFERHEADERTYPE*  pInputBuffer[NUM_MAX_BUFFERS];
    OMX_BUFFERHEADERTYPE*  pOutputBuffer[NUM_MAX_BUFFERS];
    HI_UNF_AUDIOTRACK_ATTR_S   stTrackAttr;
    OMX_AUDIO_PARAM_PCMMODETYPE  sPcmParam;
    OMX_PARAM_PORTDEFINITIONTYPE* pCompPrivateStruct = NULL;

    memset(&sAppPriData, 0, sizeof(sAppPriData));

    /* Parse the parameters */
    s32Ret = ParseCmdLine(argc, argv, &sAppPriData);
    if (s32Ret != 0)
    {
        return -1;
    }

    /* Open input file */
    if (strncmp(sAppPriData.infilename, "\0", 1) != 0)
    {
        fprintf(stderr, "\e[40m\e[36m\e[1mInput file name: %s\e[0m\n", sAppPriData.infilename);
        sAppPriData.fileIn = fopen(sAppPriData.infilename, "rb");
        if (NULL == sAppPriData.fileIn)
        {
            fprintf(stderr, "Open file %s error!\n", sAppPriData.infilename);
            return -1;
        }

        if (strncmp(sAppPriData.infilename + strlen(sAppPriData.infilename) - 4, ".mp4", 4) == 0)
        {
            PRINTF_ERR("Sorry, The Sample Unsupport mp4 play.");
            goto OMX_EXIT;
        }
    }
    else
    {
        PRINTF_ERR("No input file to Open");
        return -1;
    }

    /* If save output, Open output file */
    if (HI_TRUE == sAppPriData.bSaveOutData)
    {
        sAppPriData.fileOut = fopen(sAppPriData.outfilename, "w");
        if (sAppPriData.fileOut == NULL)
        {
            printf("Error:  failed to create the output file \n");
            goto OMX_EXIT;
        }

        if (sAppPriData.fileOut)
        {
            /* Write default WAV head */
            sAppPriData.WvH.dwDataLen = 0;
            WriteHeader(&sAppPriData.WvH, 44100, 2);
            fseek(sAppPriData.fileOut, 0, SEEK_SET);
            fwrite(&sAppPriData.WvH, sizeof(WAVEHEADER), 1, sAppPriData.fileOut);
        }
    }

    // If play audio online, do some initialization
    if (HI_TRUE == sAppPriData.bAudioPlay)
    {
        HI_SYS_Init();
        s32Ret = HIADP_Snd_Init();
        if (s32Ret != HI_SUCCESS)
        {
            fprintf(stderr, "Call SndInit failed\n");
            goto SYS_DEINIT;
        }

        s32Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
        if (s32Ret != HI_SUCCESS)
        {
            fprintf(stderr, "Call HI_UNF_SND_GetDefaultTrackAttr failed\n");
            goto SND_DEINIT;
        }
        s32Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &sAppPriData.hTrack);
        if (s32Ret != HI_SUCCESS)
        {
            fprintf(stderr, "Call HI_UNF_SND_CreateTrack failed\n");
            goto SND_DEINIT;
        }
        s32Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
        if (s32Ret != HI_SUCCESS)
        {
            fprintf(stderr, "Call DispInit failed\n");
            goto TCK_DEINIT;
        }
    }
    //Set HDMI RAW Mode, For DDP Passthrough
    HI_UNF_SND_SetHdmiMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_RAW);


    // Begin to do OMX Operation
    error = OMX_Init();
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT("%d :: Error returned by OMX_Init()\n", __LINE__);
        goto ALL_EXIT;
    }

    fprintf(stderr, "\n\e[40m\e[36m\e[1mThis is <%s> Test Application\e[0m\n\n", sAppPriData.strDecoder);

    GT_START();
    error = OMX_GetHandle(&audiodechandle, sAppPriData.strDecoder, &sAppPriData, &AdecCallbacks);
    GT_END("OMX_GetHandle");

    if (error != OMX_ErrorNone)
    {
        APP_DPRINT( "%s Component Not Found\n", sAppPriData.strDecoder);
        goto ALL_EXIT;
    }

    pCompPrivateStruct = malloc(sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
    if (NULL == pCompPrivateStruct)
    {
        printf("%d :: App: Malloc Failed\n", __LINE__);
        goto ALL_EXIT;
    }

    pCompPrivateStruct->nPortIndex = OMX_IN_PORT_IDX;
    error = OMX_GetParameter(audiodechandle, OMX_IndexParamPortDefinition, pCompPrivateStruct);
    if (error != OMX_ErrorNone)
    {
        error = OMX_ErrorBadParameter;
        printf ("%d :: OMX_ErrorBadParameter\n", __LINE__);
        goto ALL_EXIT;
    }

    nInportBufSize = pCompPrivateStruct->nBufferSize;
    GT_START();
    error = OMX_SetParameter(audiodechandle, OMX_IndexParamPortDefinition, pCompPrivateStruct);
    GT_END("Set Parameter OMX_IndexParamPortDefinition");
    if (error != OMX_ErrorNone)
    {
        error = OMX_ErrorBadParameter;
        printf ("%d :: OMX_ErrorBadParameter\n", __LINE__);
        goto ALL_EXIT;
    }

    pCompPrivateStruct->nPortIndex = OMX_OUT_PORT_IDX;
    error = OMX_GetParameter(audiodechandle, OMX_IndexParamPortDefinition, pCompPrivateStruct);
    if (error != OMX_ErrorNone)
    {
        error = OMX_ErrorBadParameter;
        printf ("%d :: OMX_ErrorBadParameter\n", __LINE__);
        goto ALL_EXIT;
    }

    nOutportBufSize = pCompPrivateStruct->nBufferSize;
    GT_START();
    error = OMX_SetParameter(audiodechandle, OMX_IndexParamPortDefinition, pCompPrivateStruct);
    GT_END("Set Parameter OMX_IndexParamPortDefinition");
    if (error != OMX_ErrorNone)
    {
        error = OMX_ErrorBadParameter;
        printf ("%d :: OMX_ErrorBadParameter\n", __LINE__);
        goto ALL_EXIT;
    }

    for (i = 0; i < NUM_IN_BUFFERS; i++)
    {
        error = OMX_AllocateBuffer(audiodechandle, &pInputBuffer[i], 0, NULL, nInportBufSize);
        if (error != OMX_ErrorNone)
        {
            APP_DPRINT("%d :: Error returned by OMX_AllocateBuffer()\n", __LINE__);
            goto ALL_EXIT;
        }
        APP_DPRINT("%d :: OMX_AllocateBuffer pInputBuffer[%ld]  = %p\n", __LINE__, i, pInputBuffer[i]);
    }

    for (i = 0; i < NUM_OUT_BUFFERS; i++)
    {
        error = OMX_AllocateBuffer(audiodechandle, &pOutputBuffer[i], 1, NULL, nOutportBufSize);
        if (error != OMX_ErrorNone)
        {
            APP_DPRINT("%d :: Error returned by OMX_AllocateBuffer()\n", __LINE__);
            goto ALL_EXIT;
        }
        APP_DPRINT("%d :: OMX_AllocateBuffer pOutputBuffer[%ld] = %p\n", __LINE__, i, pOutputBuffer[i]);
    }

    GT_START();
    error = OMX_SendCommand(audiodechandle, OMX_CommandStateSet, OMX_StateIdle, NULL);
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT ("Error from SendCommand-Idle(Init) State function - error = %d\n", error);
        goto ALL_EXIT;
    }
    error = WaitForState(audiodechandle, OMX_StateIdle);
    GT_END("Call to SendCommand <OMX_StateIdle>");
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT( "Error:  hAmrEncoder->WaitForState reports an error %X\n", error);
        goto ALL_EXIT;
    }

    GT_START();
    error = OMX_SendCommand(audiodechandle, OMX_CommandStateSet, OMX_StateExecuting, NULL);
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT ("Error from SendCommand-Executing State function\n");
        goto ALL_EXIT;
    }
    error = WaitForState(audiodechandle, OMX_StateExecuting);
    GT_END("Call to SendCommand <OMX_StateExecuting>");
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT( "Error:  hAmrEncoder->WaitForState reports an error %X\n", error);
        goto ALL_EXIT;
    }

    for (j = 0; j < NUM_OUT_BUFFERS; j++)
    {
        APP_DPRINT("%d APP: Before OMX_FillThisBuffer = %ld\n", __LINE__, j);
        OMX_FillThisBuffer(audiodechandle, pOutputBuffer[j]);
    }

    for (j = 0; j < NUM_IN_BUFFERS; j++)
    {
        APP_DPRINT("%d APP: Before send_input_buffer  = %ld\n", __LINE__, j);
        error = send_input_buffer(audiodechandle, pInputBuffer[j], &sAppPriData);
        if (error != OMX_ErrorNone)
        {
            goto ALL_EXIT;
        }
    }

    sPcmParam.nPortIndex = OMX_OUT_PORT_IDX;
    error = OMX_GetParameter(audiodechandle, OMX_IndexParamAudioPcm, &sPcmParam);
    if (error != OMX_ErrorNone)
    {
        sAppPriData.s32Samplerate = sPcmParam.nSamplingRate;
        sAppPriData.s32ChannelOut = sPcmParam.nChannels;
        APP_DPRINT( "Error:  OMX_GetParameter reports an error 0x%x\n", error);
        goto ALL_EXIT;
    }


    while (error == OMX_ErrorNone)
    {
        if (sAppPriData.mEndPlaying)
        {
            GT_START();
            error = OMX_SendCommand(audiodechandle, OMX_CommandStateSet, OMX_StateIdle, NULL);
            if (error != OMX_ErrorNone)
            {
                fprintf (stderr, "\nError from SendCommand-Idle(Stop) State function!!!!!!!!\n");
                goto ALL_EXIT;
            }

            error = WaitForState(audiodechandle, OMX_StateIdle);
            GT_END("Call to SendCommand <OMX_StateIdle>");
            if (error != OMX_ErrorNone)
            {
                fprintf(stderr, "\nError:  hAACDecoder->WaitForState reports an error %X!!!!!!!\n", error);
                goto ALL_EXIT;
            }

            APP_DPRINT ("Quit Main Loop\n");
            sAppPriData.mEndPlaying = 0;
            break;
        }

        usleep(1000);
    }


    /* free the Allocate Buffers */
    for (i = 0; i < NUM_IN_BUFFERS; i++)
    {
        APP_DPRINT("%d :: App: Freeing %p IP BufHeader\n", __LINE__, pInputBuffer[i]);
        error = OMX_FreeBuffer(audiodechandle, OMX_IN_PORT_IDX, pInputBuffer[i]);
        if ((error != OMX_ErrorNone))
        {
            APP_DPRINT ("%d:: Error in Free Handle function\n", __LINE__);
            goto ALL_EXIT;
        }
    }

    for (i = 0; i < NUM_OUT_BUFFERS; i++)
    {
        APP_DPRINT("%d :: App: Freeing %p OP BufHeader\n", __LINE__, pOutputBuffer[i]);
        error = OMX_FreeBuffer(audiodechandle, OMX_OUT_PORT_IDX, pOutputBuffer[i]);
        if ((error != OMX_ErrorNone))
        {
            APP_DPRINT ("%d:: Error in Free Handle function\n", __LINE__);
            goto ALL_EXIT;
        }
    }

    sPcmParam.nPortIndex = OMX_OUT_PORT_IDX;
    /* Get samplerate & ch info for wav fileformat after success decoded audiofile */
    error = OMX_GetParameter (audiodechandle, OMX_IndexParamAudioPcm, &sPcmParam);
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT( "Error:  OMX_GetParameter reports an error %X\n", error);
        goto ALL_EXIT;
    }
    sAppPriData.s32Samplerate = sPcmParam.nSamplingRate;
    sAppPriData.s32ChannelOut = sPcmParam.nChannels;    

    GT_START();
    error = OMX_SendCommand(audiodechandle, OMX_CommandStateSet, OMX_StateLoaded, NULL);
    error = WaitForState(audiodechandle, OMX_StateLoaded);
    GT_END("Call to SendCommand <OMX_StateLoaded>");
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT ("%d:: Error from SendCommand-Idle State function\n", __LINE__);
        goto ALL_EXIT;
    }

    error = OMX_FreeHandle(audiodechandle);
    if (error != OMX_ErrorNone)
    {
        APP_DPRINT("%s Component err in Free Handle\n", sAppPriData.strDecoder);
    }
    APP_DPRINT("Call to OMX_FreeHandle ");


ALL_EXIT:

    if (HI_TRUE == sAppPriData.bAudioPlay)
    {
        HIADP_Disp_DeInit();
TCK_DEINIT:
        s32Ret = HI_UNF_SND_DestroyTrack(sAppPriData.hTrack);
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_UNF_SND_DestroyTrack err: 0x%x\n", s32Ret);
        }
SND_DEINIT:
        HIADP_Snd_DeInit();
SYS_DEINIT:
        HI_SYS_DeInit();
    }


OMX_EXIT:
    if (pCompPrivateStruct != NULL)
    {
        free(pCompPrivateStruct);
        pCompPrivateStruct  = NULL;
    }
    if (sAppPriData.fileIn)
    {
        fclose(sAppPriData.fileIn);
        sAppPriData.fileIn = NULL;
    }
    if (HI_TRUE == sAppPriData.bSaveOutData)
    {
        if (sAppPriData.fileOut)
        {
            WriteHeader(&sAppPriData.WvH, sAppPriData.s32Samplerate, sAppPriData.s32ChannelOut);
            fseek(sAppPriData.fileOut, 0, SEEK_SET);
            fwrite(&sAppPriData.WvH, sizeof(WAVEHEADER), 1, sAppPriData.fileOut);
            fclose(sAppPriData.fileOut);
            sAppPriData.fileOut = NULL;
        }
    }

    return -1;
}
