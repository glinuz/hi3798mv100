/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCO4ir2/uQ7BFDYEYESXdzFFpPTRP1CkEuTQUs7
G00Y4YmBNRU4pFbTyNhVLMHu+SH4BAfq40LtlXHPmd/5Cqx2Kl1OY3kqiE7/UpRzoX+b62z7
zUU+iFzzvfGkQyhY3EpwIniyFXu4idqbnGpCM5rvBHeQiBIqYxRk3p8tjkx3Xw==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

/*
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/in.h>
#include <linux/ioport.h>
#include <linux/version.h>
*/

#include "iMedia_kernel.h"
#include "iMedia_common.h"
#include "iMedia_viddec.h"
#include "iMedia_error.h"
#include "iMedia_util.h"

MODULE_AUTHOR("Jack Song(songxg@huawei.com)");
MODULE_DESCRIPTION("iMedia TEST");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("1.1");
/*MODULE_FIRMWARE(FIRMWARE_TG3);*/

IMEDIA_CODEC_CTX g_pctx = 0;
void *g_pmem = 0;
int g_width = 352;
int g_height = 288;
int g_bufcount = 3;

char *mode = "huawei";
int tuner[4] = {1, 0, 1, 0};
int tuner_c = 1;

module_param(g_width, int, 0644);
module_param(g_height, int, 0644);
module_param(g_bufcount, int, 0644);

module_param(mode, charp, 0644);
module_param_array(tuner, int, &tuner_c, 0644);

#if 0
struct file_operations hwmedia_fops =
{
    .owner = THIS_MODULE,
};

static int hwmedia_major;
#endif

static int imedia_init(void)
{
    int iRet = 0;
#if 1
    STRU_IVIDDEC_MEMORY_INFO memPar;
    STRU_IVIDDEC_PARAMS stParam;
    int codec_id = IVIDEO_CODEC_H263;
    int buf_size = 0;
#endif

#if 1
    memPar.usWidth = g_width;
    memPar.usHeight = g_height;
    memPar.usBufNum = g_bufcount;
    memPar.eColorFormat = IVIDEO_CSP_IYUV;
    iRet = IMedia_Viddec_QueryMemSize(codec_id, &memPar, &buf_size);

    g_pmem = kmalloc(buf_size, GFP_KERNEL);

    stParam.usMaxWidth = memPar.usWidth;
    stParam.usMaxHeight = memPar.usHeight;
    stParam.usMaxFrameBufNum = memPar.usBufNum;
    stParam.usMaxRefFrame = 1;
    stParam.eColorSpaceType = memPar.eColorFormat;
    stParam.iFlags = 0;
    stParam.bForceOutYUV420Flag = 0;
    stParam.pucBuf = g_pmem;
    stParam.iBufLength = buf_size;

    iRet = IMedia_Viddec_Create(codec_id, &stParam, &g_pctx);

    iRet = IMedia_Viddec_FrameParse(g_pctx, 0, 0);

    iRet = IMedia_Viddec_FrameDecode(g_pctx, 0, 0);
    iRet = IMedia_Viddec_Control(g_pctx, IMEDIA_PICTURE_RELEASE, 0, 0);

#endif

    return 0;
}

static void imedia_exit(void)
{
    int iRet = 0;
#if 1

    /*iMedia_Viddec_exit();*/
    if (0 != g_pctx)
    {
        iRet = IMedia_Viddec_Delete(g_pctx);
        g_pctx = 0;
    }

    if (0 != g_pmem)
    {
        kfree(g_pmem);
        g_pmem = 0;
    }

#endif
#if 0
    unregister_chrdev(hwmedia_major);
#endif

    return;
}

module_init(imedia_init);
module_exit(imedia_exit);
