#ifdef ANDROID
#undef  LOG_TAG
#define LOG_TAG    "HIOMX_AAC_SAMPLE"
#endif

#include "OMX_Types.h"
#include "OMX_Component.h"
#include "OMX_Core.h"
#include "OMX_Index.h"
#include "OMX_Image.h"
#include "OMX_Audio.h"
#include "OMX_Video.h"
#include "OMX_IVCommon.h"
#include "OMX_Other.h"
#include "omx_audio_base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
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

/****************************************************************************
* NAME        : WriteHeader()                                              *
* DESCRIPTION : Write wav file header structure                            *
****************************************************************************/
static void WriteHeader(WAVEHEADER* WvH, int samplerate, int channelnumber)
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

WAVEHEADER WvH;
int g_samplerate = 44100;
int g_channels = 2;
int g_pcmSize = 2;

int g_playcompleted = 0;
int lastinputbuffersent   = 0;
OMX_S16 numOfInputBuffer  = 4;
OMX_S16 numOfOutputBuffer = 4;
OMX_S32 nIpBufSize;
OMX_S32 nOpBufSize;
int GT_FlagE = 0;  /* Fill Buffer  1 = First Buffer,  0 = Not First Buffer  */
int GT_FlagF = 0;  /*Empty Buffer  1 = First Buffer,  0 = Not First Buffer  */
FILE* fIn, *fOut;

#define INPUT_PORT 0
#define OUTPUT_PORT 1
#define MAX_NUM_OF_BUFS_AUDIODEC 16

int fill_data (OMX_BUFFERHEADERTYPE* pBuf, FILE* fIn)
{
    HI_U32 nRead;

    nRead = fread(pBuf->pBuffer, 1, pBuf->nAllocLen, fIn);

    pBuf->nFilledLen = nRead;
    pBuf->nOffset    = 0;
    return nRead;
}

OMX_ERRORTYPE send_input_buffer(OMX_HANDLETYPE        pHandle,
                                OMX_BUFFERHEADERTYPE* pBuffer,
                                FILE*                 fIn)
{
    OMX_ERRORTYPE error = OMX_ErrorNone;
    HI_U32        nRead;

    nRead = fill_data(pBuffer, fIn);

    /*Don't send more buffers after OMX_BUFFERFLAG_EOS*/
    if (lastinputbuffersent)
    {
        pBuffer->nFlags = 0;
        sample_printf("%d : APP:: Entering send_input_buffer finish,pBuffer=%p \n", __LINE__, pBuffer);
        return error;
    }

    if ((nRead < pBuffer->nAllocLen) && (lastinputbuffersent == 0))
    {
        pBuffer->nFlags = OMX_BUFFERFLAG_EOS;
        lastinputbuffersent = 1;
 //       sample_printf("%d : APP:: Entering send_input_buffer OMX_BUFFERFLAG_EOS,pBuffer=%p \n", __LINE__, pBuffer);
    }
    else
    {
        pBuffer->nFlags = 0;
    }

    /*time stamp & tick count value*/
    pBuffer->nTimeStamp = rand() % 100;
    pBuffer->nTickCount = rand() % 70;

    //sample_printf("TimeStamp Input: %lld\n",pBuffer->nTimeStamp);
    //sample_printf("TickCount Input: %ld\n",pBuffer->nTickCount);
    pBuffer->nFilledLen = nRead;
    OMX_EmptyThisBuffer(pHandle, pBuffer);

    return error;
}

typedef struct AudioDecAppDATATYPE
{
    OMX_STATETYPE state;
} AudioDecAppDATATYPE;

OMX_STRING strAacDecoder = "OMX.hisi.audio.aacdec";

/* This method will wait for the component to get to the state
 * specified by the DesiredState input. */
static OMX_ERRORTYPE WaitForState(OMX_HANDLETYPE* pHandle, OMX_STATETYPE DesiredState)
{
    OMX_STATETYPE CurState = OMX_StateReserved_0x00000000;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    int nCnt = 0;
    OMX_COMPONENTTYPE* pComponent = (OMX_COMPONENTTYPE*)pHandle;

    eError = pComponent->GetState(pHandle, &CurState);
    if (CurState == OMX_StateReserved_0x00000000 /* && bInvalidState == OMX_TRUE*/)
    {
        eError = OMX_ErrorReserved_0x8000100A;
    }

    while ((eError == OMX_ErrorNone) && (CurState != DesiredState))
    {
        sched_yield();
        if (nCnt++ == 10)
        {
  //          sample_printf("Still Waiting, press CTL-C to continue\n");
        }

        eError = pComponent->GetState(pHandle, &CurState);
        if (CurState == OMX_StateReserved_0x00000000 /*&& bInvalidState == OMX_TRUE*/)
        {
   //         sample_printf("\nINVALID STATE \n\n");
            eError = OMX_ErrorReserved_0x8000100A;
        }
    }

    if (eError != OMX_ErrorNone)
    {
        return eError;
    }

    return OMX_ErrorNone;
}

OMX_ERRORTYPE EventHandler(OMX_OUT OMX_HANDLETYPE aComponent,
                           OMX_OUT OMX_PTR        aAppData,
                           OMX_OUT OMX_EVENTTYPE  aEvent,
                           OMX_OUT OMX_U32        aData1,
                           OMX_OUT OMX_U32        aData2,
                           OMX_OUT OMX_PTR        aEventData)
{
    //OSCL_UNUSED_ARG(aComponent);
    //OSCL_UNUSED_ARG(aAppData);
    //OSCL_UNUSED_ARG(aEventData);

    if (OMX_EventCmdComplete == aEvent)
    {
        if (OMX_CommandStateSet == aData1)
        {
            switch ((OMX_S32) aData2)
            {
                    //Falling through next case
                case OMX_StateReserved_0x00000000:
                    sample_printf("OMX_StateReserved_0x00000000\n");
                    break;
                case OMX_StatePause:
                    sample_printf("OMX_StatePause\n");
                    break;
                case OMX_StateWaitForResources:
                    sample_printf("OMX_StateWaitForResources\n");
                    break;

                case OMX_StateLoaded:
                    sample_printf("OMX_StateLoaded\n");
                    break;

                case OMX_StateIdle:
                    sample_printf("OMX_StateIdle\n");
                    break;

                case OMX_StateExecuting:        //Change the state on receiving callback
                    sample_printf("OMX_StateExecuting\n");
                    break;

                default:
                    sample_printf("OMX_State unknow cmd=0x%lx\n", (OMX_U32) aData2);
                    break;
            }
        }
        else if (OMX_CommandPortDisable == aData1)
        {
            sample_printf("OMX_CommandPortDisable\n");
        }
        else if (OMX_CommandPortEnable == aData1)
        {
            //Change the state from Reconfig to Executing on receiving this callback
            sample_printf("OMX_CommandPortEnable\n");
        }
    }
    else if (OMX_EventPortSettingsChanged == aEvent)
    {
        sample_printf("OMX_EventPortSettingsChanged\n");
    }
    else if (OMX_EventBufferFlag == aEvent)
    {
        //callback for EOS  //Change the state on receiving EOS callback,aData1==nOutputPortIndex
        if ((aData2 == (OMX_U32)OMX_BUFFERFLAG_EOS) && (aData1 == (OMX_U32)1))
        {
            g_playcompleted = 1;
        }

        sample_printf("EOS Event recieved\n");
    }
    else if (OMX_EventError == aEvent)
    {
        if (OMX_ErrorSameState == (OMX_ERRORTYPE)aData1)
        {
            sample_printf("Component reported SameState Error, try to proceed\n");
        }
        else if (OMX_ErrorStreamCorrupt == (OMX_ERRORTYPE)aData1)
        {
            /* Don't do anything right now for the stream corrupt error,
             * just count the number of such callbacks and let the component to proceed */

            //sample_printf("Component/Encoder reported Stream Corrupt Error\n");
        }
        else
        {
            // do nothing, just try to proceed normally
        }
    }

    return OMX_ErrorNone;
}

OMX_ERRORTYPE EmptyBufferDone(OMX_OUT OMX_HANDLETYPE        hComponent,
                              OMX_OUT OMX_PTR               pAppData,
                              OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer)
{
    OMX_ERRORTYPE error;

    //Check the validity of buffer
    if (NULL == pBuffer)
    {
        return OMX_ErrorBadParameter;
    }

    error = send_input_buffer(hComponent, pBuffer, fIn);
    if (error != OMX_ErrorNone)
    {
        sample_printf("In %s Error %08x Calling FillThisBuffer\n", __func__, error);
        return error;
    }

    return OMX_ErrorNone;
}

OMX_ERRORTYPE FillBufferDone(OMX_OUT OMX_HANDLETYPE        hComponent,
                             OMX_OUT OMX_PTR               pAppData,
                             OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer)
{
    OMX_ERRORTYPE error;
    static int counter = 0;

    //Check the validity of buffer
    if (NULL == pBuffer)
    {
        return OMX_ErrorBadParameter;
    }

    if (pBuffer->nFilledLen > 0)
    {
        fwrite(pBuffer->pBuffer, 1, pBuffer->nFilledLen, fOut);
        WvH.dwDataLen += pBuffer->nFilledLen;
    }

    pBuffer->nFilledLen = 0;
    error = OMX_FillThisBuffer(hComponent, pBuffer);
    if (error != OMX_ErrorNone)
    {
        sample_printf("In %s Error %08x Calling FillThisBuffer\n", __func__, error);
        return error;
    }

    if (counter++ < 8)
    {
 //       sample_printf("--------------In %s FillBufferDone (%d)\n", __func__, counter );
    }

    //Output buffer has been freed by component &amp; can now be passed again in FillThisBuffer call

    return OMX_ErrorNone;
}

OMX_CALLBACKTYPE audiodeccallbacks =
{
    .EventHandler    = EventHandler,
    .EmptyBufferDone = EmptyBufferDone,
    .FillBufferDone  = FillBufferDone
};

OMX_ERRORTYPE test_OMX_ComponentNameEnum()
{
    char* name;
    int index;

    OMX_ERRORTYPE err = OMX_ErrorNone;

    sample_printf("GENERAL TEST %s\n", __func__);
    name  = malloc(OMX_MAX_STRINGNAME_SIZE);
    index = 0;
    while (1)
    {
        err = OMX_ComponentNameEnum (name, OMX_MAX_STRINGNAME_SIZE, index);
        if ((name != NULL) && (err == OMX_ErrorNone))
        {
            sample_printf("component %i is %s\n", index, name);
        }
        else
        {
            break;
        }

        if (err != OMX_ErrorNone)
        {
            break;
        }

        index++;
    }

    free(name);
    name = NULL;
    if (err == OMX_ErrorNoMore)
    {
        err = OMX_ErrorNone;
    }

    sample_printf("GENERAL TEST %s result %i\n", __func__, err);
    return err;
}

OMX_ERRORTYPE test_OMX_RoleEnum(OMX_STRING component_name)
{
    OMX_U32 no_of_roles;
    OMX_U8** string_of_roles;
    OMX_ERRORTYPE err = OMX_ErrorNone;
    OMX_U32 index;

    sample_printf("GENERAL TEST %s\n", __func__);
    sample_printf("Getting roles of %s. Passing Null first...\n", component_name);
    err = OMX_GetRolesOfComponent(component_name, &no_of_roles, NULL);
    if (err != OMX_ErrorNone)
    {
        sample_printf("Not able to retrieve the number of roles of the given component\n");
        sample_printf("GENERAL TEST %s result %i\n", __func__, err);
        return err;
    }

    sample_printf("The number of roles for the component %s is: %i\n", component_name, (int)no_of_roles);

    if (no_of_roles == 0)
    {
        sample_printf("The Number or roles is 0.\nThe component selected is not correct for the purpose of this test.\nExiting...\n");
        err = OMX_ErrorInvalidComponentName;
    }
    else
    {
        string_of_roles = (OMX_U8**)malloc(no_of_roles * sizeof(OMX_STRING));
        for (index = 0; index < no_of_roles; index++)
        {
            *(string_of_roles + index) = (OMX_U8*)malloc(no_of_roles * OMX_MAX_STRINGNAME_SIZE);
        }

        sample_printf("...then buffers\n");

        err = OMX_GetRolesOfComponent(component_name, &no_of_roles, string_of_roles);
        if (err != OMX_ErrorNone)
        {
            sample_printf("Not able to retrieve the roles of the given component\n");
        }
        else if (string_of_roles != NULL)
        {
            for (index = 0; index < no_of_roles; index++)
            {
                sample_printf("The role %ld for the component:  %s \n", (index + 1), *(string_of_roles + index));
            }
        }
        else
        {
            sample_printf("role string is NULL!!! Exiting...\n");
            err = OMX_ErrorInvalidComponentName;
        }

        for (index = 0; index < no_of_roles; index++)
        {
            free(*(string_of_roles + index));
        }

        free(string_of_roles);
    }

    sample_printf("GENERAL TEST %s result %i\n", __func__, err);
    return err;
}

OMX_ERRORTYPE test_OMX_ComponentEnumByRole(OMX_STRING role_name)
{
    OMX_U32 no_of_comp_per_role;
    OMX_U8** string_of_comp_per_role;
    OMX_ERRORTYPE err;
    OMX_U32 index;

    sample_printf("GENERAL TEST %s\n", __func__);

    sample_printf("Getting number of components per role for %s\n", role_name);

    err = OMX_GetComponentsOfRole(role_name, &no_of_comp_per_role, NULL);
    if (err != OMX_ErrorNone)
    {
        sample_printf("Not able to retrieve the number of components of a given role\n");
        sample_printf("GENERAL TEST %s result %i\n", __func__, err);
        return err;
    }

    sample_printf("Number of components per role for %s is %i\n", role_name, (int)no_of_comp_per_role);

    string_of_comp_per_role = (OMX_U8**)malloc(no_of_comp_per_role * sizeof(OMX_STRING));
    for (index = 0; index < no_of_comp_per_role; index++)
    {
        string_of_comp_per_role[index] = malloc(OMX_MAX_STRINGNAME_SIZE);
    }

    err = OMX_GetComponentsOfRole(role_name, &no_of_comp_per_role, string_of_comp_per_role);
    if (err != OMX_ErrorNone)
    {
        sample_printf("Not able to retrieve the components of a given role\n");
        sample_printf("GENERAL TEST %s result %i\n", __func__, err);
        for (index = 0; index < no_of_comp_per_role; index++)
        {
            if (string_of_comp_per_role[index])
            {
                free(string_of_comp_per_role[index]);
                string_of_comp_per_role[index] = NULL;
            }
        }

        if (string_of_comp_per_role)
        {
            free(string_of_comp_per_role);
            string_of_comp_per_role = NULL;
        }

        return err;
    }

    sample_printf(" The components are:\n");
    for (index = 0; index < no_of_comp_per_role; index++)
    {
        sample_printf("%s\n", string_of_comp_per_role[index]);
    }

    for (index = 0; index < no_of_comp_per_role; index++)
    {
        if (string_of_comp_per_role[index])
        {
            free(string_of_comp_per_role[index]);
            string_of_comp_per_role[index] = NULL;
        }
    }

    if (string_of_comp_per_role)
    {
        free(string_of_comp_per_role);
        string_of_comp_per_role = NULL;
    }

    sample_printf("GENERAL TEST %s  result %i\n \n", __func__, OMX_ErrorNone);
    return OMX_ErrorNone;
}

OMX_ERRORTYPE test_OpenClose(OMX_STRING component_name)
{
    OMX_ERRORTYPE err = OMX_ErrorNone;
    OMX_HANDLETYPE audiodechandle;
    AudioDecAppDATATYPE sAppPrivData;

    sample_printf("GENERAL TEST %s\n", __func__);
    err = OMX_GetHandle(&audiodechandle, strAacDecoder, &sAppPrivData, &audiodeccallbacks);
    if (err != OMX_ErrorNone)
    {
        sample_printf("No component found\n");
    }
    else
    {
        err = OMX_FreeHandle(audiodechandle);
        if (err != OMX_ErrorNone)
        {
            sample_printf("In %s err %08x in Free Handle\n", __func__, err);
        }
    }

    sample_printf("GENERAL TEST %s result %i\n", __func__, err);
    return err;
}

//#define AMR_TIMING_PROFILE
#ifdef AMR_TIMING_PROFILE
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
struct timeval timeBegin, timeEnd;
unsigned long long u64TimeB, u64TimeE;
#endif

void GT_START()
{
#ifdef AMR_TIMING_PROFILE
    gettimeofday(&timeBegin, NULL);
    u64TimeB = timeBegin.tv_sec * 1000000LLU + timeBegin.tv_usec;
#endif
}

void GT_END(char* funcName)
{
#ifdef AMR_TIMING_PROFILE
    gettimeofday(&timeEnd, NULL);
    u64TimeE = timeEnd.tv_sec * 1000000LLU + timeEnd.tv_usec;
    sample_printf("%s time cosumed =%f \n", funcName, (u64TimeE - u64TimeB) / 1000.0);
#endif
}

int main(int argc, char** argv)
{
    OMX_ERRORTYPE error;
    OMX_HANDLETYPE audiodechandle;
    AudioDecAppDATATYPE sAppPrivData;
    //OMX_STATETYPE CurState = OMX_StateReserved_0x00000000;
    OMX_PARAM_PORTDEFINITIONTYPE* pCompPrivateStruct = NULL;
    OMX_AUDIO_PARAM_PCMMODETYPE sPcmParam;
    //OMX_AUDIO_PARAM_MP3TYPE sMP3Param;
    OMX_BUFFERHEADERTYPE* pInputBufferHeader[MAX_NUM_OF_BUFS_AUDIODEC];
    OMX_BUFFERHEADERTYPE* pOutputBufferHeader[MAX_NUM_OF_BUFS_AUDIODEC];
    int i, j;

    fIn = fopen(argv[1], "rb");
    if (fIn == NULL)
    {
        sample_printf("Error:  failed to open the file %s for readonly\n", argv[1]);
        goto EXIT;
    }

    fOut = fopen(argv[2], "w");
    if (fOut == NULL)
    {
        sample_printf("Error:  failed to create the output file \n");
        goto EXIT;
    }

    if (fOut)
    {
        /* write default WAV head */
        WvH.dwDataLen = 0;
        WriteHeader(&WvH, 44100, 2);
        fseek(fOut, 0, SEEK_SET);
        fwrite(&WvH, sizeof(WAVEHEADER), 1, fOut);
    }

    error = OMX_Init();
    if (error != OMX_ErrorNone)
    {
        sample_printf("%d :: Error returned by OMX_Init()\n", __LINE__);
        goto EXIT;
    }

    GT_START();
    error = OMX_GetHandle(&audiodechandle, strAacDecoder, &sAppPrivData, &audiodeccallbacks);
    GT_END("OMX_GetHandle");

    if (error != OMX_ErrorNone)
    {
        sample_printf("%s Component Not Found\n", strAacDecoder);
        goto EXIT;
    }

    pCompPrivateStruct = malloc (sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
    if (NULL == pCompPrivateStruct)
    {
        sample_printf("%d :: App: Malloc Failed\n", __LINE__);
        goto EXIT;
    }

    pCompPrivateStruct->nPortIndex = INPUT_PORT;
    error = OMX_GetParameter(audiodechandle, OMX_IndexParamPortDefinition, pCompPrivateStruct);
    if (error != OMX_ErrorNone)
    {
        error = OMX_ErrorBadParameter;
        printf ("%d :: OMX_ErrorBadParameter\n", __LINE__);
        goto EXIT;
    }

    nIpBufSize = pCompPrivateStruct->nBufferSize;
    GT_START();
    error = OMX_SetParameter(audiodechandle, OMX_IndexParamPortDefinition, pCompPrivateStruct);
    GT_END("Set Parameter Test-SetParameter");
    if (error != OMX_ErrorNone)
    {
        error = OMX_ErrorBadParameter;
        printf ("%d :: OMX_ErrorBadParameter\n", __LINE__);
        goto EXIT;
    }

    pCompPrivateStruct->nPortIndex = OUTPUT_PORT;
    GT_START();
    error = OMX_GetParameter(audiodechandle, OMX_IndexParamPortDefinition, pCompPrivateStruct);
    GT_END("Set Parameter Test-SetParameter");
    if (error != OMX_ErrorNone)
    {
        error = OMX_ErrorBadParameter;
        printf ("%d :: OMX_ErrorBadParameter\n", __LINE__);
        goto EXIT;
    }

    nOpBufSize = pCompPrivateStruct->nBufferSize;
    GT_START();
    error = OMX_SetParameter(audiodechandle, OMX_IndexParamPortDefinition, pCompPrivateStruct);
    GT_END("Set Parameter Test-SetParameter");
    if (error != OMX_ErrorNone)
    {
        error = OMX_ErrorBadParameter;
        printf ("%d :: OMX_ErrorBadParameter\n", __LINE__);
        goto EXIT;
    }

#ifndef USE_BUFFER
    for (i = 0; i < numOfInputBuffer; i++)
    {
  //      sample_printf("%d :: About to call OMX_AllocateBuffer On Input\n", __LINE__);
        error = OMX_AllocateBuffer(audiodechandle, &pInputBufferHeader[i], 0, NULL, nIpBufSize);
        if (error != OMX_ErrorNone)
        {
            sample_printf("%d :: Error returned by OMX_AllocateBuffer()\n", __LINE__);
            goto EXIT;
        }

 //       sample_printf("%d :: pInputBufferHeader[%d] = %p\n", __LINE__, i, pInputBufferHeader[i]);
    }

    for (i = 0; i < numOfOutputBuffer; i++)
    {
  //      sample_printf("%d :: About to call OMX_AllocateBuffer On Output\n", __LINE__);
        error = OMX_AllocateBuffer(audiodechandle, &pOutputBufferHeader[i], 1, NULL, nOpBufSize);
        if (error != OMX_ErrorNone)
        {
            sample_printf("%d :: Error returned by OMX_AllocateBuffer()\n", __LINE__);
            goto EXIT;
        }

  //      sample_printf("%d :: pOutputBufferHeader[%d] = %p\n", __LINE__, i, pOutputBufferHeader[i]);
    }

#else
    for (i = 0; i < numOfInputBuffer; i++)
    {
        pInputBuffer[i] = (OMX_U8*)malloc(sizeof(nIpBufSize));
 //       sample_printf("%d :: [TESTAPPALLOC] pInputBuffer[%d] = %p\n", __LINE__, i, pInputBuffer[i]);
        if (NULL == pInputBuffer[i])
        {
            sample_printf("%d :: APP: Malloc Failed\n", __LINE__);
            error = OMX_ErrorInsufficientResources;
            goto EXIT;
        }

        /* pass input buffer */
 //       sample_printf("%d :: APP: About to call OMX_UseBuffer\n", __LINE__);
        error = OMX_UseBuffer(audiodechandle, &pInputBufferHeader[i], 0, NULL, nIpBufSize, pInputBuffer[i]);
        if (error != OMX_ErrorNone)
        {
            sample_printf("%d :: APP: Error returned by OMX_UseBuffer()\n", __LINE__);
            goto EXIT;
        }
    }

    for (i = 0; i < numOfOutputBuffer; i++)
    {
        pOutputBuffer[i] = (OMX_U8*) malloc (nOpBufSize);
 //       sample_printf("%d :: [TESTAPPALLOC] pOutputBuffer[%d] = %p\n", __LINE__, i, pOutputBuffer[i]);
        if (NULL == pOutputBuffer[i])
        {
            sample_printf("%d :: APP: Malloc Failed\n", __LINE__);
            error = OMX_ErrorInsufficientResources;
            goto EXIT;
        }

        /* pass output buffer */
 //       sample_printf("%d :: APP: About to call OMX_UseBuffer\n", __LINE__);
 //       sample_printf("%d :: pOutputBufferHeader[%d] = %p\n", __LINE__, i, pOutputBufferHeader[i]);
        error = OMX_UseBuffer(audiodechandle, &pOutputBufferHeader[i], 1, NULL, nOpBufSize, pOutputBuffer[i]);
        if (error != OMX_ErrorNone)
        {
            sample_printf("%d :: APP: Error returned by OMX_UseBuffer()\n", __LINE__);
            goto EXIT;
        }
    }
#endif


    GT_START();
    error = OMX_SendCommand(audiodechandle, OMX_CommandStateSet, OMX_StateIdle, NULL);
    if (error != OMX_ErrorNone)
    {
        sample_printf("Error from SendCommand-Idle(Init) State function - error = %d\n", error);
        goto EXIT;
    }

    error = WaitForState(audiodechandle, OMX_StateIdle);
    GT_END("Call to SendCommand <OMX_StateExecuting>");
    if (error != OMX_ErrorNone)
    {
        sample_printf("Error:  hAmrEncoder->WaitForState reports an error %X\n", error);
        goto EXIT;
    }

    GT_START();
    error = OMX_SendCommand(audiodechandle, OMX_CommandStateSet, OMX_StateExecuting, NULL);
    if (error != OMX_ErrorNone)
    {
        sample_printf("Error from SendCommand-Executing State function\n");
        goto EXIT;
    }

    error = WaitForState(audiodechandle, OMX_StateExecuting);
    GT_END("Call to SendCommand <OMX_StateExecuting>");
    if (error != OMX_ErrorNone)
    {
        sample_printf("Error:  hAmrEncoder->WaitForState reports an error %X\n", error);
        goto EXIT;
    }

    for (j = 0; j < numOfOutputBuffer; j++)
    {
        sample_printf("%d APP: Before OMX_FillThisBuffer = %d\n", __LINE__, j);
        if (j == 0)
        {
            GT_FlagF = 1;    /* 1 = First Buffer,  0 = Not First Buffer  */
            GT_START();      /* Fill Buffer */
        }

        OMX_FillThisBuffer(audiodechandle, pOutputBufferHeader[j]);
    }

    for (j = 0; j < numOfInputBuffer; j++)
    {
        sample_printf("%d APP: Before send_input_buffer = %d\n", __LINE__, j);
        if (j == 0)
        {
            GT_FlagE = 1;    /* 1 = First Buffer,  0 = Not First Buffer  */
            GT_START();      /* Empty Bufffer */
        }

        error = send_input_buffer(audiodechandle, pInputBufferHeader[j], fIn);
        if (error != OMX_ErrorNone)
        {
            goto EXIT;
        }
    }

    sPcmParam.nPortIndex = OUTPUT_PORT;
    error = OMX_GetParameter(audiodechandle, OMX_IndexParamAudioPcm, &sPcmParam);
    if (error == OMX_ErrorNone)
    {
        g_samplerate = sPcmParam.nSamplingRate;
        g_channels   = sPcmParam.nChannels;

    }
	else
	{
        sample_printf("Error:  OMX_GetParameter reports an error %X\n", error);
        goto EXIT;
	}

    //Main Process TODO
    while ((error == OMX_ErrorNone) /*&& (gState != OMX_StateIdle) && (gState != OMX_StateReserved_0x00000000)*/)
    {
        if (g_playcompleted)
        {
            GT_START();
            error = OMX_SendCommand(audiodechandle, OMX_CommandStateSet, OMX_StateIdle, NULL);
            if (error != OMX_ErrorNone)
            {
                fprintf (stderr, "\nError from SendCommand-Idle(Stop) State function!!!!!!!!\n");
                goto EXIT;
            }

            error = WaitForState(audiodechandle, OMX_StateIdle);
            GT_END("Call to SendCommand <OMX_StateIdle>");
            if (error != OMX_ErrorNone)
            {
                sample_printf("\nError:  hAACDecoder->WaitForState reports an error %X!!!!!!!\n", error);
                goto EXIT;
            }
            g_playcompleted = 0;
            break;
        }

        usleep(1000);
    }


    /* free the Allocate Buffers */
    for (i = 0; i < numOfInputBuffer; i++)
    {
 //       sample_printf("%d :: App: Freeing %p IP BufHeader\n", __LINE__, pInputBufferHeader[i]);
        error = OMX_FreeBuffer(audiodechandle, INPUT_PORT, pInputBufferHeader[i]);
        if ((error != OMX_ErrorNone))
        {
            sample_printf("%d:: Error in Free Handle function\n", __LINE__);
            goto EXIT;
        }
    }

    for (i = 0; i < numOfOutputBuffer; i++)
    {
 //       sample_printf("%d :: App: Freeing %p OP BufHeader\n", __LINE__, pOutputBufferHeader[i]);
        error = OMX_FreeBuffer(audiodechandle, OUTPUT_PORT, pOutputBufferHeader[i]);
        if ((error != OMX_ErrorNone))
        {
            sample_printf("%d:: Error in Free Handle function\n", __LINE__);
            goto EXIT;
        }
    }

#ifdef USE_BUFFER
    /* free the UseBuffers */
    for (i = 0; i < numOfInputBuffer; i++)
    {
        if (pInputBuffer[i] != NULL)
        {
            pInputBuffer[i] = pInputBuffer[i] - 128;
 //           sample_printf("%d :: [TESTAPPFREE] pInputBuffer[%d] = %p\n", __LINE__, i, pInputBuffer[i]);
            free(pInputBuffer[i]);
            pInputBuffer[i] = NULL;
        }
    }

    for (i = 0; i < numOfOutputBuffer; i++)
    {
        if (pOutputBuffer[i] != NULL)
        {
            pOutputBuffer[i] = pOutputBuffer[i] - 128;
//            sample_printf("%d :: [TESTAPPFREE] pOutputBuffer[%d] = %p\n", __LINE__, i, pOutputBuffer[i]);
            newfree(pOutputBuffer[i]);
            pOutputBuffer[i] = NULL;
        }
    }
#endif

    sPcmParam.nPortIndex = OUTPUT_PORT;
    /* get samplerate and ch info for wav fileformat after success decoded audiofile */
    error = OMX_GetParameter (audiodechandle, OMX_IndexParamAudioPcm, &sPcmParam);
    if (error != OMX_ErrorNone)
    {
        g_samplerate = sPcmParam.nSamplingRate;
        g_channels = sPcmParam.nChannels;
        sample_printf("Error:  OMX_GetParameter reports an error %X\n", error);
        goto EXIT;
    }

    GT_START();
    error = OMX_SendCommand(audiodechandle, OMX_CommandStateSet, OMX_StateLoaded, NULL);
    error = WaitForState(audiodechandle, OMX_StateLoaded);
    GT_END("Call to SendCommand <OMX_StateLoaded>");
    if (error != OMX_ErrorNone)
    {
        sample_printf("%d:: Error from SendCommand-Idle State function\n", __LINE__);
        goto EXIT;
    }

    GT_START();
    error = OMX_FreeHandle(audiodechandle);
    GT_END("Call to OMX_FreeHandle ");
    if (error != OMX_ErrorNone)
    {
        sample_printf("%s Component err in Free Handle\n", strAacDecoder);
    }

 //   sample_printf("Call to OMX_FreeHandle ");

EXIT:
    if (pCompPrivateStruct != NULL)
    {
        free(pCompPrivateStruct);
        pCompPrivateStruct = NULL;
    }

    if (fIn)
    {
        fclose(fIn);
    }

    if (fOut)
    {
        WriteHeader(&WvH, g_samplerate, g_channels);
        fseek(fOut, 0, SEEK_SET);
        fwrite(&WvH, sizeof(WAVEHEADER), 1, fOut);
        fclose(fOut);
    }

    return -1;
}
