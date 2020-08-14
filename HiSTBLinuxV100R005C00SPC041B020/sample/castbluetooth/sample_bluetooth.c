#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "hi_bluetooth.h"

HI_S32 Sample_BT_GetAdapter(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp)
{
    HI_S32 ret;
    HI_BLUETOOTH_ADP_ATTR_S *pstBTAdpList = NULL;
    HI_U32 num,i;
    
    if(NULL == pstBTAdp)
    {
        printf("%s(), %d: pstBTAdp is NULL.\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    ret = HI_BLUETOOTH_GetDefaultAdapter(pstBTAdp);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Get default bluetooth adapter failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
    }
   
    return ret;
    
    pstBTAdpList = malloc(HI_BLUETOOTH_DEV_LIST_MAX*sizeof(HI_BLUETOOTH_ADP_ATTR_S));
    if(NULL == pstBTAdpList)
    {
        printf("%s(), %d: Malloc bluetooth adapter list failure.\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    memset(pstBTAdpList,0,HI_BLUETOOTH_DEV_LIST_MAX*sizeof(HI_BLUETOOTH_ADP_ATTR_S));
    
    ret = HI_BLUETOOTH_GetAdapterList(pstBTAdpList,HI_BLUETOOTH_DEV_LIST_MAX,&num);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Get bluetooth adapter list failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        goto free_mem;
    }
    if(num == 0)
    {
        printf("%s(), %d: No bluetooth adapter is available.\n",__FUNCTION__,__LINE__);
        goto free_mem;
    }
    else if(num > HI_BLUETOOTH_DEV_LIST_MAX)
    {
        printf("%s(), %d: Bluetooth adapter number %d is over limit.\n",__FUNCTION__,__LINE__,num);
        num = HI_BLUETOOTH_DEV_LIST_MAX;
    }
    
    printf("Get bluetooth adapter success, %d adapters are available, please select:\n",num);
    for(i=0;i<num;i++)
    {
        printf("%d: %s %s\n",i,pstBTAdpList[i].szName,pstBTAdpList[i].szAdpAddr);
    }
    printf("\n");

    do
    {
        if(scanf("%d",&i)!=1)
        {
            ret = HI_FAILURE;
            goto free_mem;
        }
    }while(i>=num);
    
    memcpy(pstBTAdp,&pstBTAdpList[i],sizeof(HI_BLUETOOTH_ADP_ATTR_S));
    
free_mem:
    if(NULL != pstBTAdpList)
    {
        free(pstBTAdpList);
    }
    return ret;
}

HI_S32 Sample_BT_ScanDevice(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev)
{
    HI_S32 ret;
    HI_BLUETOOTH_DEV_ATTR_S * pstBTDevList = NULL;    
    HI_U32 num,i;
    
    printf("%s(), %d: Scan bluetooth device start.\n",__FUNCTION__,__LINE__);
    if(NULL == pstBTAdp)
    {
        printf("%s(), %d: pstBTAdp is NULL.\n",__FUNCTION__,__LINE__);
    }

    if(NULL == pstBTDev)
    {
        printf("%s(), %d: pstBTDev is NULL.\n",__FUNCTION__,__LINE__);
    }

    pstBTDevList = malloc(HI_BLUETOOTH_DEV_LIST_MAX*sizeof(HI_BLUETOOTH_DEV_ATTR_S));
    if(NULL == pstBTDevList)
    {
        printf("%s(), %d: Malloc bluetooth device list failure.\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    memset(pstBTDevList,0,HI_BLUETOOTH_DEV_LIST_MAX*sizeof(HI_BLUETOOTH_DEV_ATTR_S));
    ret = HI_BLUETOOTH_ScanDevice(pstBTAdp, pstBTDevList, HI_BLUETOOTH_DEV_LIST_MAX, &num);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Scan bluetooth device failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        goto free_mem;
    }
    if(num == 0)
    {
        printf("%s(), %d: No bluetooth device is available.\n",__FUNCTION__,__LINE__);
        ret = HI_FAILURE;
        goto free_mem;
    }
    else if(num > HI_BLUETOOTH_DEV_LIST_MAX)
    {
        printf("%s(), %d: Bluetooth device number %d is over limit.\n",__FUNCTION__,__LINE__,num);
        num = HI_BLUETOOTH_DEV_LIST_MAX;
    }
    
    printf("Scan bluetooth device success, %d devices are available, please select:\n",num);
    for(i=0;i<num;i++)
    {
        printf("%d  %s  %s  %s\n",i,pstBTDevList[i].szDevAddr,pstBTDevList[i].szName,pstBTDevList[i].s32Paired?"paired":"not paired");
    }
    printf("\n");

    do
    {
        if(scanf("%d",&i)!=1)
        {
            ret = HI_FAILURE;
            goto free_mem;
        }
    }while(i>=num);
    
    memcpy(pstBTDev,&pstBTDevList[i],sizeof(HI_BLUETOOTH_DEV_ATTR_S));

free_mem:
    if(NULL != pstBTDevList)
    {
        free(pstBTDevList);
    }
    return ret;
}

HI_S32 Sample_BT_Pairing(HI_VOID)
{
    HI_S32 ret;
    HI_BLUETOOTH_ADP_ATTR_S stBTAdp;
    HI_BLUETOOTH_DEV_ATTR_S stBTDev;
    HI_CHAR szPassword[8];
    
    printf("%s(), %d: Pair bluetooth device start.\n",__FUNCTION__,__LINE__);
    
    ret = Sample_BT_GetAdapter(&stBTAdp);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Get bluetooth adapter failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        return ret;
    }
    else
    {
        printf("%s(), %d: Get bluetooth adapter success, %s  %s\n",__FUNCTION__,__LINE__,stBTAdp.szName,stBTAdp.szAdpAddr);
    }

    printf("Input char 's' to scan the bluetooth device.\n");
    while(getchar()!='s');

    ret = Sample_BT_ScanDevice(&stBTAdp, &stBTDev);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Scan bluetooth device failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        return ret;
    }
    else
    {
        printf("%s(), %d: Find bluetooth device , %s  %s\n",__FUNCTION__,__LINE__,stBTDev.szDevAddr,stBTDev.szName);
    }

    printf("please input the bluetooth device password:\n");
    scanf("%s",szPassword);
    ret = HI_BLUETOOTH_Pairing(&stBTAdp, &stBTDev, szPassword);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Pairing bluetooth device failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        return ret;
    }
    
    printf("%s(), %d: Pair the bluetooth device success.\n",__FUNCTION__,__LINE__);

    return ret;
}

HI_BOOL g_bFirstDevice = HI_TRUE;
HI_U8 g_fileNameFir[] = "/etc/bluetooth/FirstConf/asound.conf";  
HI_U8 g_fileNameSec[] = "/etc/bluetooth/SecondConf/asound.conf";

static HI_VOID BLUETOOTH_SetConfig(HI_CHAR *pszDevAddr)
{
    FILE* fp = HI_NULL; 
    HI_CHAR aszBuf[32];
    snprintf(aszBuf,sizeof(aszBuf), "device %s", pszDevAddr);
    
    if( access("/etc/bluetooth/", F_OK)!=0 )
    {  
        mkdir("/etc/bluetooth/", 0777 );
    }
    if(g_bFirstDevice == HI_TRUE) 
    {
        if(access("/etc/bluetooth/FirstConf/", F_OK)!=0 )
        {    
            mkdir("/etc/bluetooth/FirstConf/", 0777 );
        }
        fp = fopen((char*)g_fileNameFir, "wr");
        g_bFirstDevice = HI_FALSE;
    }
    else
    {
        if( access("/etc/bluetooth/SecondConf/", F_OK)!=0 )
        {    
            mkdir("/etc/bluetooth/SecondConf/", 0777 );
        }
        fp = fopen((char*)g_fileNameSec, "wr");
        g_bFirstDevice = HI_TRUE;
    }
    
    if (fp)
    {
       fputs("pcm.bluetooth{\n", fp);
       fputs("type bluetooth\n", fp);
       fputs(aszBuf, fp);
       fputs("\n}\n", fp);
       fclose(fp);        
    }

}


HI_S32 Sample_BT_PairedDevices(HI_VOID)
{
    HI_S32 ret;
    HI_BLUETOOTH_ADP_ATTR_S stBTAdp;
    HI_BLUETOOTH_DEV_ATTR_S * pstBTDevList = NULL;    
    HI_U32 num,i;
    HI_S32 s32Status;
    HI_CHAR cInput, devclass, command[50];
    
    ret = Sample_BT_GetAdapter(&stBTAdp);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Get bluetooth adapter failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        return ret;
    }
    else
    {
        printf("%s(), %d: Get bluetooth adapter success, %s  %s\n",__FUNCTION__,__LINE__,stBTAdp.szName,stBTAdp.szAdpAddr);
    }

    pstBTDevList = malloc(HI_BLUETOOTH_DEV_LIST_MAX*sizeof(HI_BLUETOOTH_DEV_ATTR_S));
    if(NULL == pstBTDevList)
    {
        printf("%s(), %d: Malloc bluetooth device list failure.\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    memset(pstBTDevList,0,HI_BLUETOOTH_DEV_LIST_MAX*sizeof(HI_BLUETOOTH_DEV_ATTR_S));

    ret = HI_BLUETOOTH_GetPairedDevList(&stBTAdp, pstBTDevList, HI_BLUETOOTH_DEV_LIST_MAX, &num);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Get paired bluetooth device failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        goto free_mem;
    }
    if(num == 0)
    {
        printf("%s(), %d: No paired bluetooth device is available.\n",__FUNCTION__,__LINE__);
        goto free_mem;
    }
    else if(num > HI_BLUETOOTH_DEV_LIST_MAX)
    {
        printf("%s(), %d: Paired bluetooth device number %d is over limit.\n",__FUNCTION__,__LINE__,num);
        goto free_mem;
    }
    
    printf("Get paired bluetooth device success, %d devices are available, please select:\n",num);
    for(i=0;i<num;i++)
    {
        devclass = (pstBTDevList[i].s32Class>>8)&0x1f;
        ret = HI_BLUETOOTH_GetDevStatus(&stBTAdp, &pstBTDevList[i], (HI_BLUETOOTH_DEVICE_STATUS_E*)&s32Status);
        printf("%d: addr [%s], name [%s],  class [%d],  status [%x]\n",i,pstBTDevList[i].szDevAddr,pstBTDevList[i].szName,
              devclass,
              (ret!=0)?ret:s32Status);
    }
    printf("\n");

    do
    {
        if(scanf("%d",&i)!=1)
        {
            ret = HI_FAILURE;
            goto free_mem;
        }
    }while(i>=num);

    while(1)
    {
        printf("Please select the operation :\n");
        printf("      0 -- Exit.\n");
        printf("      1 -- Status.\n");
        printf("      2 -- Connect.\n");
        printf("      3 -- Disconnect.\n");
        printf("      4 -- Remove.\n");
        printf("      5 -- SetTrust.\n");
        printf("\n");

        do
        {
            cInput = getchar();
        }while((cInput<'0')||(cInput>'5'));
        
        switch(cInput)
        {
            case '0':
                goto free_mem;
            case '1':
                ret = HI_BLUETOOTH_GetDevStatus(&stBTAdp, &pstBTDevList[i], (HI_BLUETOOTH_DEVICE_STATUS_E *)&s32Status);
                if(HI_ERR_BLUETOOTH_PTHREAD_BUSY == ret)
                {
                    printf("%s(), %d: Get bluetooth device status failure, pthread is busy, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Get bluetooth device status failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    printf("%s(), %d: Get bluetooth device status success, status is %d.\n",__FUNCTION__,__LINE__,s32Status);
                }
                break;
            case '2':
                devclass =(pstBTDevList[i].s32Class>>8)&0x1f;
                switch (devclass)
                {
                   case IPANEL_BLUETOOTH_DEV_AUDIO : 
                     ret = HI_BLUETOOTH_Connect(&stBTAdp, &pstBTDevList[i], HI_TRUE);
                     break;
                   case IPANEL_BLUETOOTH_DEV_PERIPHERAL: 
                     snprintf(command, 50 , "hidd --connect %s",(&pstBTDevList[i])->szDevAddr);
                     system(command);
                     ret = HI_SUCCESS;
                     break;
                   default:
                     break;
                }
                
                if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Connect bluetooth device failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    BLUETOOTH_SetConfig(pstBTDevList[i].szDevAddr);
                    printf("arrd %s\n",pstBTDevList[i].szDevAddr);
                    printf("%s(), %d: Connect bluetooth device success.\n",__FUNCTION__,__LINE__);
                }
                break;
            case '3':
                devclass =(pstBTDevList[i].s32Class>>8)&0x1f;
                switch (devclass)
                {
                   case IPANEL_BLUETOOTH_DEV_AUDIO : 
                     ret = HI_BLUETOOTH_Connect(&stBTAdp, &pstBTDevList[i], HI_FALSE);
                     break;
                   case IPANEL_BLUETOOTH_DEV_PERIPHERAL: 
                     snprintf(command, 50 , "hidd --kill %s",(&pstBTDevList[i])->szDevAddr);
                     system(command);
                     ret = HI_SUCCESS;
                     break;
                   default:
                     break;
                }
                if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Disconnect bluetooth device failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    printf("%s(), %d: Disconnect bluetooth device success.\n",__FUNCTION__,__LINE__);
                }
                break;
            case '4':
                ret = HI_BLUETOOTH_RemovePairedDev(&stBTAdp, &pstBTDevList[i]);
                if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Remove paired bluetooth device failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    printf("%s(), %d: Remove paired bluetooth device success.\n",__FUNCTION__,__LINE__);
                }
                goto free_mem;
              case '5':
                ret = HI_BLUETOOTH_SetTrust(&stBTAdp, &pstBTDevList[i]);
                if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Set trust bluetooth device failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    printf("%s(), %d: Set trust bluetooth device success.\n",__FUNCTION__,__LINE__);
                }
                break;

            default:
                break;
        }
    }
    
free_mem:
    if(NULL != pstBTDevList)
    {
        free(pstBTDevList);
    }
    return ret;
}


HI_S32 Sample_BT_GetConnectedHeadset(HI_VOID)
{
    HI_S32 ret;
    HI_BLUETOOTH_ADP_ATTR_S stBTAdp;
    HI_BLUETOOTH_DEV_ATTR_S stBTDev;    

    ret = HI_BLUETOOTH_GetDefaultAdapter(&stBTAdp);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Get bluetooth adapter failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        return ret;
    }

    ret = HI_BLUETOOTH_GetConnectedHeadset(&stBTAdp,&stBTDev);
    if(HI_ERR_BLUETOOTH_PTHREAD_BUSY == ret)
    {
        printf("%s(), %d: Get connected bluetooth headset failure, pthread is busy, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        return ret;
    }
    else if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Get connected bluetooth headset failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        return ret;
    }
    else
    {
        printf("adapter : %s %s\n",stBTAdp.szAdpAddr,stBTAdp.szName);
        printf("headset : %s %s\n",stBTDev.szDevAddr,stBTDev.szName);
    }
    return ret;
}

HI_S32 Sample_BT_AdapterCtrl(HI_VOID)
{
    HI_S32 ret;
    HI_BLUETOOTH_ADP_ATTR_S stBTAdp;
    HI_S32 s32Status = 0;
    HI_CHAR cInput;

    ret = Sample_BT_GetAdapter(&stBTAdp);
    if(HI_SUCCESS != ret)
    {
        printf("%s(), %d: Get bluetooth adapter failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
        return ret;
    }
    else
    {
        printf("%s(), %d: Get bluetooth adapter success, %s  %s\n",__FUNCTION__,__LINE__,stBTAdp.szName,stBTAdp.szAdpAddr);
    }

    while(1)
    {
        printf("Please select the operation :\n");
        printf("      0 -- Exit.\n");
        printf("      1 -- Status.\n");
        printf("      2 -- Up.\n");
        printf("      3 -- Down.\n");
        printf("      4 -- Reset.\n");
        printf("\n");

        do
        {
            cInput = getchar();
        }while((cInput<'0')||(cInput>'4'));
        
        switch(cInput)
        {
            case '0':
                return HI_SUCCESS;
            case '1':
                ret = HI_BLUETOOTH_GetAdapterStatus(&stBTAdp, (HI_BLUETOOTH_ADAPTER_STATUS_E *)&s32Status);
                if(HI_ERR_BLUETOOTH_PTHREAD_BUSY == ret)
                {
                    printf("%s(), %d: Get bluetooth adapter status failure, pthread is busy, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Get bluetooth adapter status failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    printf("%s(), %d: Get bluetooth adapter status success, status is %d.\n",__FUNCTION__,__LINE__,s32Status);
                }
                break;
            case '2':
                ret = HI_BLUETOOTH_AdapterCtrl(&stBTAdp, HI_BLUETOOTH_ADP_CTRL_UP, NULL);
                if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Power up bluetooth adapter failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    printf("%s(), %d: Power up bluetooth adapter success.\n",__FUNCTION__,__LINE__);
                }
                break;
            case '3':
                ret = HI_BLUETOOTH_AdapterCtrl(&stBTAdp, HI_BLUETOOTH_ADP_CTRL_DOWN, NULL);
                if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Power down bluetooth adapter failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    printf("%s(), %d: Power down bluetooth adapter success.\n",__FUNCTION__,__LINE__);
                }
                break;
            case '4':
                ret = HI_BLUETOOTH_AdapterCtrl(&stBTAdp, HI_BLUETOOTH_ADP_CTRL_RESET, NULL);
                if(HI_SUCCESS != ret)
                {
                    printf("%s(), %d: Reset bluetooth adapter failure, ret is 0x%x.\n",__FUNCTION__,__LINE__,ret);
                }
                else
                {
                    printf("%s(), %d: Reset bluetooth adapter success.\n",__FUNCTION__,__LINE__);
                }
                break;
            default:
                break;
        }
    }
    
    return ret;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR cInput;
    
    while(1)
    {
        printf("\nPlease select the bluetooth function:\n");
        printf("      0 -- Exit.\n");
        printf("      1 -- Pair.\n");
        printf("      2 -- Paired devices.\n");
        printf("      3 -- Connected headset.\n");
        printf("      4 -- Adapter control.\n");
        printf("\n");

        do
        {
            cInput = getchar();
        }while((cInput<'0')||(cInput>'4'));
        switch(cInput)
        {
            case '0':
                return (0);
            case '1':
                Sample_BT_Pairing();
                break;
            case '2':
                Sample_BT_PairedDevices();
                break;
            case '3':
                Sample_BT_GetConnectedHeadset();
                break;
            case '4':
                Sample_BT_AdapterCtrl();
                break;
            default:
                break;
        }
    }

    return s32Ret;
}


