#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

#include "hi_unf_pmoc.h"

/*
when you want use your x5hdtest.bin, please re-define USR_BIN_SWITCH to 1.
Also don't forget put x5hdtest.bin to this path: "SDK\sample\pmoc"
*/
#define USR_BIN_SWITCH 0

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define HI_INFO_PMOC(format, arg ...)
#else
#define HI_INFO_PMOC(format, arg ...) printf( format, ## arg)
#endif

static HI_U32 g_u32CodeSize = 0;
static HI_U8 g_u8BinBuf[0x10000];  /*64KB buffer*/

static HI_U32 g_TimerCount = 0;
static HI_U8 g_DisplayMode = 2;
static HI_U32 g_RunningCount = 1;
static HI_U8 g_Scene = 0;
static HI_U8 g_IrType = 0;
static HI_U8 g_KeyledType = 2;
static const char PID_FILE[] = "/dev/wifi/sample.pid";

HI_S32 LoadUsrBin(HI_VOID)
{
    FILE *fb;
    HI_U32 len;

    fb = fopen("./x5hdtest.bin", "r");
    if (NULL == fb)
    {
        HI_INFO_PMOC("open file x5hdtest.bin failed!\n");
        return HI_FAILURE;
    }

    fseek(fb, 0, SEEK_END);
    len = (HI_U32)ftell(fb);
    HI_INFO_PMOC("pmoc code len = %d \n", len);
    if (len > sizeof(g_u8BinBuf) / sizeof(HI_U8))
    {
        HI_INFO_PMOC("x5hdtest.bin is bigger than %zu Bytes!\n", sizeof(g_u8BinBuf) / sizeof(HI_U8));
        fclose(fb);
        return HI_FAILURE;
    }

    fseek(fb, 0, SEEK_SET);
    if (fread(g_u8BinBuf, 1, len, fb) != len)
    {
        HI_INFO_PMOC("fread err:%s :%d !\n", __FUNCTION__, __LINE__);
        fclose(fb);
        return HI_FAILURE;
    }

    fclose(fb);
    g_u32CodeSize = len;

    return HI_SUCCESS;
}

HI_S32 SetWifiStandbyResume(HI_VOID)
{
    HI_S32 fd;
    HI_S8 fbuf[100];
    pid_t pid;

    fd = open(PID_FILE, O_RDONLY, 0666);
    if(fd < 0) 
    {
        HI_INFO_PMOC("SAMPLE_PS: Cann't open '%s'\n", PID_FILE);
        return HI_FAILURE;
    }

    /* read pid in pid file */
    memset(fbuf, 0, sizeof(fbuf));
    if (read(fd, fbuf, sizeof(fbuf)) < 0) 
    {
        HI_INFO_PMOC("SAMPLE_PS: Cann't read '%s'\n", PID_FILE);
        close(fd);
        return HI_FAILURE;
    }
    close(fd);

    pid = atoi((char*)fbuf);

    kill(pid, SIGPWR);

    sleep(2);

    return HI_SUCCESS;
}

HI_S32  main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_U32 i;
    HI_S32 ret;
    HI_U32 u32Period;
    time_t time0;
    struct tm *time1;
    HI_UNF_PMOC_MODE_E mode;
    HI_UNF_PMOC_WKUP_S wakeup = {
        0
    };
    HI_UNF_PMOC_DEV_TYPE_S devType = {
        HI_UNF_IR_CODE_NEC_SIMPLE, 
        HI_UNF_KEYLED_TYPE_74HC164
    };
    HI_UNF_PMOC_ACTUAL_WKUP_E actual;
    HI_UNF_PMOC_STANDBY_MODE_S standbyMode = {
        0
    };

    HI_U8 u8EthUsbStandbyMode = 0;

    HI_BOOL bResetFlag = 0;

    HI_U32 u32GpioNumber = 0;
    
    if ((argc <= 1) || (argc > 10))
    {
        printf("Usage: %s [TimerCount] [Display] [Running Count] [Scene] [IR Type] [Keyled Type] [Eth & USB Type] [Reset] [GPIO No.]\n"
               "       TimerCount: 0: Disable timer wake up; other: Wake up time(in Second). \n"
               "       Display: 0: no display; 1: display number; 2: display time.\n"
               "       Running Count: the number of running.                      \n"
               "       Scene: 0: standard; 1: eth forward.                        \n"
               "       IR Type: 0:nec simple; 1:tc9012; 2:nec full; 3:sony_12bit; 4:raw.\n"
               "       Keyled Type: 0: 74hc164; 1: pt6961; 2: ct1642; 3: pt6964; 4: fd650 5: gpiokey. \n"
               "       Eth & USB Type: 0: Disable; 1: USB Wakeup; 2: Single packet; 3: Magic Packet; 4: Frame Filter. \n"
               "       Reset: 0: resume to ddr; 1: resume to reset system. \n"
               "       GPIO No.: 0: disable gpio wake up ; other: gpio wake up number. \n",
                argv[0]);
        printf("Example: %s 0 2 1 0 0 2 0 0 0\n", argv[0]);
        return HI_FAILURE;
    } 
    
    if (argc > 1)
    {
        g_TimerCount  = strtol(argv[1],NULL,0);
    }

    if (argc > 2)
    {
	    g_DisplayMode = strtol(argv[2],NULL,0);
    }

    if (argc > 3)
    {
	    g_RunningCount = strtol(argv[3],NULL,0);
    }

    if (argc > 4)
    {
        g_Scene = strtol(argv[4],NULL,0);
    }

    if (argc > 5)
    {
	    g_IrType = strtol(argv[5],NULL,0);
    }

    if (argc > 6)
    { 
    	g_KeyledType = strtol(argv[6],NULL,0);
    }

    if (argc > 7)
    {
	    u8EthUsbStandbyMode = strtol(argv[7],NULL,0);
    }

    if (argc > 8)
    {
	    bResetFlag = strtol(argv[8],NULL,0);
    }

    if (argc > 9)
    {
        u32GpioNumber = strtol(argv[9],NULL,0);
    }
    
    ret = HI_UNF_PMOC_Init();
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PMOC("HI_UNF_PMOC_Init err, 0x%08x !\n", ret);
        return ret;
    }

#if USR_BIN_SWITCH
    ret = LoadUsrBin();
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PMOC("call LoadUsrBin() err!\n");
        HI_UNF_PMOC_DeInit();
        return HI_FAILURE;
    }

    ret = HI_UNF_PMOC_LoadMcuCode(g_u8BinBuf, g_u32CodeSize);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PMOC("HI_UNF_PMOC_LoadMcuCode err, 0x%08x !\n", ret);
        HI_UNF_PMOC_DeInit();
        return ret;
    }
#endif
   
    memset(&devType, 0, sizeof(HI_UNF_PMOC_DEV_TYPE_S));
    
    if (g_IrType == 4)
    {
        devType.irtype = HI_UNF_IR_CODE_RAW;
    }
    else if (g_IrType == 3)
    {
        devType.irtype = HI_UNF_IR_CODE_SONY_12BIT;
    }
    else if (g_IrType == 2)
    {
        devType.irtype = HI_UNF_IR_CODE_NEC_FULL;
    }
    else if (g_IrType == 1)
    {
        devType.irtype = HI_UNF_IR_CODE_TC9012;
    }
    else if (g_IrType == 0)
    {
        devType.irtype = HI_UNF_IR_CODE_NEC_SIMPLE;
    }
    
    if (g_KeyledType == 1)
    {
        devType.kltype = HI_UNF_KEYLED_TYPE_PT6961;
    }
    else if (g_KeyledType == 2)
    {
        devType.kltype = HI_UNF_KEYLED_TYPE_CT1642;
    }
    else if (g_KeyledType == 3)
    {
        devType.kltype = HI_UNF_KEYLED_TYPE_PT6964;
    }
    else if (g_KeyledType == 4)
    {
        devType.kltype = HI_UNF_KEYLED_TYPE_FD650;
    }
	else if (g_KeyledType == 5)
    {
        devType.kltype = HI_UNF_KEYLED_TYPE_GPIOKEY;
    }
    else
    {
        devType.kltype = HI_UNF_KEYLED_TYPE_74HC164;
    }

    ret = HI_UNF_PMOC_SetDevType(&devType);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PMOC("HI_UNF_PMOC_SetDevType err, 0x%08x !\n", ret);
        HI_UNF_PMOC_DeInit();
        return ret;
    }

    memset(&wakeup, 0, sizeof(HI_UNF_PMOC_WKUP_S));

    if (0 == g_TimerCount)
    {
        wakeup.u32WakeUpTime = 0xffffffff;
    }
    else
    {
        wakeup.u32WakeUpTime = g_TimerCount;
    }
    
    if (devType.irtype == HI_UNF_IR_CODE_RAW)
    {
        // Here is ir powerkey code values of support
        wakeup.u32IrPmocNum = 6;
     
		wakeup.u32IrPowerKey0[0] = 0x6d92ff00; //nec simple key 1
    	wakeup.u32IrPowerKey1[0] = 0x0;
    	wakeup.u32IrPowerKey0[1] = 0x6c93ff00; //nec simple key 2
    	wakeup.u32IrPowerKey1[1] = 0x0;    
    	wakeup.u32IrPowerKey0[2] = 0x00000100; //nec full huizhou like
    	wakeup.u32IrPowerKey1[2] = 0x0;  
    	wakeup.u32IrPowerKey0[3] = 0x8010260c; // RC6 32bit
    	wakeup.u32IrPowerKey1[3] = 0x0000001c;
    	wakeup.u32IrPowerKey0[4] = 0x0000000c; // RC5 14bit
    	wakeup.u32IrPowerKey1[4] = 0x00000000;
    	wakeup.u32IrPowerKey0[5] = 0x58535754; // nec full dezhou like
    	wakeup.u32IrPowerKey1[5] = 0x00005750;  
    }
    else if (devType.irtype == HI_UNF_IR_CODE_SONY_12BIT)
    {
        wakeup.u32IrPmocNum = 1;
        wakeup.u32IrPowerKey0[0] = 0x00000095;
        wakeup.u32IrPowerKey1[0] = 0x0;
    }
    else if (devType.irtype == HI_UNF_IR_CODE_NEC_FULL)
    {
        wakeup.u32IrPmocNum = 1;
        wakeup.u32IrPowerKey0[0] = 0x58595a53;
        wakeup.u32IrPowerKey1[0] = 0x5750;
    }
    else if (devType.irtype == HI_UNF_IR_CODE_TC9012)
    {
        wakeup.u32IrPmocNum = 1;
        wakeup.u32IrPowerKey0[0] = 0xf30c0e0e; //tc9012;
        wakeup.u32IrPowerKey1[0] = 0x0;
    }
    else
    {
        wakeup.u32IrPmocNum = 1;
        wakeup.u32IrPowerKey0[0] = 0x639cff00;
        wakeup.u32IrPowerKey1[0] = 0x0;
    }

    if (devType.kltype == HI_UNF_KEYLED_TYPE_PT6961 || devType.kltype == HI_UNF_KEYLED_TYPE_CT1642)
    {
        wakeup.u32KeypadPowerKey = 0x8;
    }
    else if (devType.kltype == HI_UNF_KEYLED_TYPE_PT6964)
    {
        wakeup.u32KeypadPowerKey = 0x10;
    }
    else if(devType.kltype == HI_UNF_KEYLED_TYPE_FD650)
    {
        wakeup.u32KeypadPowerKey = 0x57;
    }
    else
    {
        wakeup.u32KeypadPowerKey = 0x0;
    }

    if (1 == u8EthUsbStandbyMode)
    {
        wakeup.bMouseKeyboardEnable = HI_TRUE;
    }
    else if (2 == u8EthUsbStandbyMode)
    {
        wakeup.stNetwork.enEthIndex = HI_UNF_PMOC_ETH_0 | HI_UNF_PMOC_ETH_1; 
        wakeup.stNetwork.bUcPacketEnable = HI_TRUE;
    }
    else if (3 == u8EthUsbStandbyMode)
    {
        wakeup.stNetwork.enEthIndex = HI_UNF_PMOC_ETH_0 | HI_UNF_PMOC_ETH_1; 
        wakeup.stNetwork.bMagicPacketEnable = HI_TRUE;        
    }
    else if (4 == u8EthUsbStandbyMode)
    {
	    wakeup.stNetwork.enEthIndex = HI_UNF_PMOC_ETH_0 | HI_UNF_PMOC_ETH_1;
	    wakeup.stNetwork.bWakeupFrameEnable = HI_TRUE;

        /* 正常filter */
        wakeup.stNetwork.stFrame[0].u32MaskBytes = 0xffffffff;
        wakeup.stNetwork.stFrame[0].u8Offset = 15;
        wakeup.stNetwork.stFrame[0].bFilterValid = HI_TRUE;

        for (i = 0; i < 31; i++)
        {
            wakeup.stNetwork.stFrame[0].u8Value[i] = i;
        }

        /* 使能部分字节 */
        wakeup.stNetwork.stFrame[1].u32MaskBytes = 0xf0ffffff;
        wakeup.stNetwork.stFrame[1].u8Offset = 15;
        wakeup.stNetwork.stFrame[1].bFilterValid = HI_TRUE;

        for (i = 0; i < 31; i++)
        {
            wakeup.stNetwork.stFrame[1].u8Value[i] = i;
        }

        /* offset > 128 */
        wakeup.stNetwork.stFrame[2].u32MaskBytes = 0xffffffff;
        wakeup.stNetwork.stFrame[2].u8Offset = 130;
        wakeup.stNetwork.stFrame[2].bFilterValid = HI_TRUE;

        for (i = 0; i < 31; i++)
        {
            wakeup.stNetwork.stFrame[2].u8Value[i] = 0x2;
        }

        /* filter为false, invalid */
        wakeup.stNetwork.stFrame[3].u32MaskBytes = 0xffffffff;
        wakeup.stNetwork.stFrame[3].u8Offset = 9;
        wakeup.stNetwork.stFrame[3].bFilterValid = HI_FALSE;

        for (i = 0; i < 31; i++)
        {
            wakeup.stNetwork.stFrame[3].u8Value[i] = 0x1;
        }
        
    }

    wakeup.bResumeResetEnable = bResetFlag;

    if (u32GpioNumber != 0)
    {
        wakeup.bGpioWakeUpEnable = 1;
        wakeup.u32GpioNo = u32GpioNumber;
    }
    
    ret = HI_UNF_PMOC_SetWakeUpAttr(&wakeup);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PMOC("HI_UNF_PMOC_SetWakeUpAttr err, 0x%08x !\n", ret);
        HI_UNF_PMOC_DeInit();
        return ret;
    }

    ret = HI_UNF_PMOC_SetScene(g_Scene);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PMOC("HI_UNF_PMOC_SetScene err, 0x%08x !\n", ret);
        HI_UNF_PMOC_DeInit();
        return ret;
    }

    memset(&standbyMode, 0, sizeof(HI_UNF_PMOC_STANDBY_MODE_S));
   
    standbyMode.u32Mode = g_DisplayMode;
    if (g_DisplayMode == 1)
    {
        standbyMode.u32DispCode = 1234;
    }
    else if (g_DisplayMode == 2)
    {
        ret = time(&time0);
        if (ret == -1)
        {
            standbyMode.stTimeInfo.u32Hour = 6;
            standbyMode.stTimeInfo.u32Minute = 9;
            standbyMode.stTimeInfo.u32Second = 36;
        }
        else
        {
            time1 = gmtime(&time0);
            standbyMode.stTimeInfo.u32Hour = (HI_U32)time1->tm_hour;
            standbyMode.stTimeInfo.u32Minute = (HI_U32)time1->tm_min;
            standbyMode.stTimeInfo.u32Second = (HI_U32)time1->tm_sec;
        }
    }
    
    ret = HI_UNF_PMOC_SetStandbyDispMode(&standbyMode);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PMOC("HI_UNF_PMOC_SetStandbyDispMode err, 0x%08x !\n", ret);
        HI_UNF_PMOC_DeInit();
        return ret;
    }
    
    for (i = 0; i < g_RunningCount; i++)
    {     
#ifndef CONFIG_SUPPORT_CA_RELEASE
#ifndef CONFIG_SUPPORT_CA_DEBUG
        /* Please umount the usb device first */
        //system("rmmod ehci-hcd.ko;rmmod ohci-hcd.ko");   //uninstall usb driver
#endif
#endif
        SetWifiStandbyResume();
        mode = HI_UNF_PMOC_MODE_SLOW;
        ret  = HI_UNF_PMOC_SwitchSystemMode(mode, &actual);
        if (HI_SUCCESS != ret)
        {
            HI_INFO_PMOC("HI_UNF_PMOC_SwitchSystemMode err, 0x%08x !\n", ret);
            HI_UNF_PMOC_DeInit();
            return ret;
        }
        
        SetWifiStandbyResume();
#ifndef CONFIG_SUPPORT_CA_RELEASE
#ifndef CONFIG_SUPPORT_CA_DEBUG
        /* After install usb driver, please mount the usb device again */
        //system("insmod /kmod/usb/ehci-hcd.ko;insmod /kmod/usb/ohci-hcd.ko");  //install usb driver
#endif
#endif
        if (actual == HI_UNF_PMOC_WKUP_IR)
        {
            HI_INFO_PMOC("wakeup by ir !!!\n");
        }
        else if (actual == HI_UNF_PMOC_WKUP_KEYLED)
        {
            HI_INFO_PMOC("wakeup by key !!!\n");
        }
        else if (actual == HI_UNF_PMOC_WKUP_TIMEOUT)
        {
            HI_INFO_PMOC("wakeup by timeout !!!\n");
        }
        else if (actual == HI_UNF_PMOC_WKUP_ETH)
        {
            HI_INFO_PMOC("wakeup by ethernet !!!\n");
        }
        else if (actual == HI_UNF_PMOC_WKUP_USB)
        {
            HI_INFO_PMOC("wakeup by usb !!!\n");
        }
        else if (actual == HI_UNF_PMOC_WKUP_GPIO)
        {
            HI_INFO_PMOC("wakeup by gpio !!!\n");
        }
        else
        {
            HI_INFO_PMOC("wakeup by no mode ??? \n");
        }

        HI_UNF_PMOC_GetStandbyPeriod(&u32Period);
        
        HI_INFO_PMOC("\n standby period = %d \n", u32Period);
        
        HI_INFO_PMOC("%%%%%%%% cnt = %d %%%%%%%% \n", i);
    }

    HI_UNF_PMOC_DeInit();

    return HI_SUCCESS;
}
