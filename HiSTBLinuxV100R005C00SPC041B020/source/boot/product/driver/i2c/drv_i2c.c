/*******************************************************************************
 *              Copyright 2005 - 2011, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName:    hi_i2c.c
 * Description: This  file for i2c read and write
 *
 * History:
 * Version          Date            Author       DefectNum    Description
 *  0.01    2011-03-15     chenqiang      NULL       Create this file
 ******************************************************************************/

#include "hi_drv_i2c.h"
#include "hi_common.h"
#include "hi_reg.h"
#include "hi_reg_common.h"

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)

#define I2C_DFT_CLK       (100000000)
#define I2C_DFT_RATE      (400000)

#define I2C0_PHY_ADDR     (0xF8B10000)
#define I2C1_PHY_ADDR     (0xF8B11000)
#define I2C2_PHY_ADDR     (0xF8B12000)
#define I2C3_PHY_ADDR     (0xF8B13000)
#define I2C4_PHY_ADDR     (0xF8B16000)//I2C ADC
#define QAMI2C_PHY_ADDR   (0xF8B17000)//I2C QAM,not used

#elif defined(CHIP_TYPE_hi3798mv100) \
	|| defined(CHIP_TYPE_hi3796mv100) \
	|| defined(CHIP_TYPE_hi3716dv100) \
	|| defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)

#define I2C_DFT_CLK       (100000000)
#define I2C_DFT_RATE      (400000)

#define I2C0_PHY_ADDR     (0xF8B10000)
#define I2C1_PHY_ADDR     (0xF8B11000)
#define I2C2_PHY_ADDR     (0xF8B12000)

#elif defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)

#define I2C_DFT_CLK       (12500000)
#define I2C_DFT_RATE      (100000)

#define I2C0_PHY_ADDR       (0xF8B10000)
#define I2C1_PHY_ADDR       (0xF8B11000)
#define I2C2_PHY_ADDR       (0xF8B12000)
#define I2C3_PHY_ADDR       (0xF8B13000)
#define I2C_QAMC_PHY_ADDR   (0xF8B16000)//I2C QAM-C
#define I2C_QAMADC_PHY_ADDR (0xF8B17000)//I2C ADC
#define I2C_QAMT_PHY_ADDR   (0xF8B1c000)//I2C QAM-T
#define I2C7_PHY_ADDR       (0xF8B1d000)

#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)

#define I2C_DFT_CLK       (100000000)
#define I2C_DFT_RATE      (400000)

#define I2C0_PHY_ADDR     (0xF8B10000)
#define I2C1_PHY_ADDR     (0xF8B11000)
#define I2C2_PHY_ADDR     (0xF8B12000)
#define I2C3_PHY_ADDR     (0xF8B13000)
#define I2C4_PHY_ADDR     (0xF8B14000)
#define ADCI2C_PHY_ADDR   (0xF8B16000)//I2C_ADC
#define QAMI2C_PHY_ADDR   (0xF8B17000)//I2C QAM

#elif  defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)

#define I2C_DFT_CLK       (100000000)
#define I2C_DFT_RATE      (400000)

#define I2C0_PHY_ADDR       (0xF8B10000)
#define I2C1_PHY_ADDR       (0xF8B11000)
#define I2C2_PHY_ADDR       (0xF8B12000)
#define I2C3_PHY_ADDR       (0xF8B13000)
#define I2C4_PHY_ADDR   	(0xF8B35000)
#define I2C5_PHY_ADDR 		(0xF8B36000)
#define ADCI2C_PHY_ADDR   	(0xF8B16000)// 	I2C_ADC
#define QAMI2C_PHY_ADDR     (0xF8B17000)// 	I2C_QAM

#elif  defined(CHIP_TYPE_hi3798cv200)

#define I2C_DFT_CLK       (100000000)
#define I2C_DFT_RATE      (400000)

#define I2C0_PHY_ADDR       (0xF8B10000)
#define I2C1_PHY_ADDR       (0xF8B11000)
#define I2C2_PHY_ADDR       (0xF8B12000)
#define I2C3_PHY_ADDR       (0xF8B13000)
#define I2C4_PHY_ADDR   	(0xF8B14000)

#else
 #error YOU MUST DEFINE  CHIP_TYPE!
#endif

#define I2C_CTRL_REG      (0x000)
#define I2C_COM_REB       (0x004)
#define I2C_ICR_REG       (0x008)
#define I2C_SR_REG        (0x00C)
#define I2C_SCL_H_REG     (0x010)
#define I2C_SCL_L_REG     (0x014)
#define I2C_TXR_REG       (0x018)
#define I2C_RXR_REG       (0x01C)


#define READ_OPERATION     (1)
#define WRITE_OPERATION    0xfe



/* I2C_CTRL_REG */
#define I2C_ENABLE             (1 << 8)
#define I2C_UNMASK_TOTAL       (1 << 7)
#define I2C_UNMASK_START       (1 << 6)
#define I2C_UNMASK_END         (1 << 5)
#define I2C_UNMASK_SEND        (1 << 4)
#define I2C_UNMASK_RECEIVE     (1 << 3)
#define I2C_UNMASK_ACK         (1 << 2)
#define I2C_UNMASK_ARBITRATE   (1<< 1)
#define I2C_UNMASK_OVER        (1 << 0)
#define I2C_UNMASK_ALL         (I2C_UNMASK_START | I2C_UNMASK_END | \
                                I2C_UNMASK_SEND | I2C_UNMASK_RECEIVE | \
                                I2C_UNMASK_ACK | I2C_UNMASK_ARBITRATE | \
                                I2C_UNMASK_OVER)


/* I2C_COM_REB */
#define I2C_SEND_ACK (~(1 << 4))
#define I2C_START (1 << 3)
#define I2C_READ (1 << 2)
#define I2C_WRITE (1 << 1)
#define I2C_STOP (1 << 0)

/* I2C_ICR_REG */
#define I2C_CLEAR_START (1 << 6)
#define I2C_CLEAR_END (1 << 5)
#define I2C_CLEAR_SEND (1 << 4)
#define I2C_CLEAR_RECEIVE (1 << 3)
#define I2C_CLEAR_ACK (1 << 2)
#define I2C_CLEAR_ARBITRATE (1 << 1)
#define I2C_CLEAR_OVER (1 << 0)
#define I2C_CLEAR_ALL (I2C_CLEAR_START | I2C_CLEAR_END | \
                       I2C_CLEAR_SEND | I2C_CLEAR_RECEIVE | \
                       I2C_CLEAR_ACK | I2C_CLEAR_ARBITRATE | \
                       I2C_CLEAR_OVER)

/* I2C_SR_REG */
//#define I2C_BUSY (1 << 7)
//#define I2C_START_INTR (1 << 6)
//#define I2C_END_INTR (1 << 5)
//#define I2C_SEND_INTR (1 << 4)
#define I2C_RECEIVE_INTR (1 << 3)
#define I2C_ACK_INTR (1 << 2)
//#define I2C_ARBITRATE_INTR (1 << 1)
#define I2C_OVER_INTR (1 << 0)


#define I2C_WAIT_TIME_OUT   0x1000

#define I2C_WRITE_REG(Addr, Value) ((*(volatile HI_U32 *)(Addr)) = (Value))
#define I2C_READ_REG(Addr)         (*(volatile HI_U32 *)(Addr))

#define HI_MAX_I2C_NUM  8

static int  i2cState = 0;
static HI_U32 g_I2cKernelAddr[HI_MAX_I2C_NUM];

HI_S32 HI_DRV_I2C_SetRate(HI_U32 I2cNum, HI_U32 I2cRate)
{
    HI_U32  Value = 0;
    HI_U32  SclH = 0;
    HI_U32  SclL = 0;

    if (I2cNum >= HI_STD_I2C_NUM)
    {
        I2C_DEGUB("%s I2cNum[%d] Error! Max:%d\n ", __FUNCTION__, I2cNum, HI_STD_I2C_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    /* read i2c I2C_CTRL register*/
    Value = I2C_READ_REG((g_I2cKernelAddr[I2cNum] + I2C_CTRL_REG));

    /* close all i2c  interrupt */
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_CTRL_REG), (Value & (~I2C_UNMASK_TOTAL)));

    SclH = (I2C_DFT_CLK / (I2cRate * 2)) / 2 - 1;
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_SCL_H_REG), SclH);

    SclL = (I2C_DFT_CLK / (I2cRate * 2)) / 2 - 1;
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_SCL_L_REG), SclL);

    /*enable i2c interrupt£¬resume original  interrupt*/
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_CTRL_REG), Value);

    return HI_SUCCESS;
}

HI_S32 DRV_WaitWriteEnd(HI_U32 I2cNum)
{
    HI_U32  I2cSrReg;
    HI_U32  i = 0;

    do
    {
            I2cSrReg = I2C_READ_REG((g_I2cKernelAddr[I2cNum] + I2C_SR_REG));

            if (i > I2C_WAIT_TIME_OUT)
            {
                I2C_DEGUB("wait write data timeout!\n");
                return HI_ERR_I2C_WRITE_TIMEOUT;
            }
            i++;
    }while((I2cSrReg & I2C_OVER_INTR) != I2C_OVER_INTR);

    if (I2cSrReg & I2C_ACK_INTR)
    {
        return HI_ERR_I2C_READ_ACK_ERR;
    }
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_ICR_REG), I2C_CLEAR_ALL);

    return HI_SUCCESS;
}

HI_S32 I2C_DRV_WaitRead(HI_U32 I2cNum)
{
    HI_U32  I2cSrReg;
    HI_U32  i = 0;

    do
    {
        I2cSrReg = I2C_READ_REG((g_I2cKernelAddr[I2cNum] + I2C_SR_REG));

        if (i > I2C_WAIT_TIME_OUT)
        {
            I2C_DEGUB("wait write data timeout!\n");
            return HI_ERR_I2C_READ_TIMEOUT;
        }
        i++;
    }while((I2cSrReg & I2C_RECEIVE_INTR) != I2C_RECEIVE_INTR);

    return HI_SUCCESS;
}


/*
I2C write finished acknowledgement function
it use to e2prom device ,make sure it finished write operation.
i2c master start next write operation must waiting when it acknowledge e2prom write cycle finished.

*/
HI_S32 HI_DRV_I2C_WriteConfig(HI_U32 I2cNum, HI_U8 I2cDevAddr)
{
    HI_U32          i = 0;
    HI_U32          j = 0;
    HI_U32          I2cSrReg;

    do
    {
        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), (I2cDevAddr & WRITE_OPERATION));
        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), (I2C_WRITE | I2C_START));

        j = 0;
        do
        {
            I2cSrReg = I2C_READ_REG((g_I2cKernelAddr[I2cNum] + I2C_SR_REG));

            if (j > I2C_WAIT_TIME_OUT)
            {
                I2C_DEGUB("wait write data timeout!\n");
                return HI_ERR_I2C_WRITE_TIMEOUT;
            }
            j++;
        }while((I2cSrReg & I2C_OVER_INTR) != I2C_OVER_INTR);

        I2cSrReg = I2C_READ_REG((g_I2cKernelAddr[I2cNum] + I2C_SR_REG));
        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_ICR_REG), I2C_CLEAR_ALL);

        i++;

        if (i > 0x200000) /*I2C_WAIT_TIME_OUT*/
        {
            I2C_DEGUB("wait write ack ok timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }while ((I2cSrReg & I2C_ACK_INTR));

    return HI_SUCCESS;
}

HI_S32 HI_DRV_I2C_Write(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData, HI_U32 DataLen)
{
    HI_U32          i;
    HI_U32          RegAddr;

    if (I2cNum >= HI_STD_I2C_NUM)
    {
        I2C_DEGUB("%s I2cNum[%d] Error! Max:%d\n ", __FUNCTION__, I2cNum, HI_STD_I2C_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    /*  clear interrupt flag*/
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_ICR_REG), I2C_CLEAR_ALL);

    /* send devide address */
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), (I2cDevAddr & WRITE_OPERATION));
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB),(I2C_WRITE | I2C_START));

    if (DRV_WaitWriteEnd(I2cNum))
    {
        I2C_DEGUB("wait write data timeout!\n");
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    /* send register address which will need to write */
    for(i=0; i<I2cRegAddrByteNum; i++)
    {
        RegAddr = I2cRegAddr >> ((I2cRegAddrByteNum -i -1) * 8);
        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), RegAddr);

        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_WRITE);

        if (DRV_WaitWriteEnd(I2cNum))
        {
            I2C_DEGUB("wait write data timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /* send data */
    for (i=0; i<DataLen; i++)
    {
        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), (*(pData+i)));
        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_WRITE);

        if (DRV_WaitWriteEnd(I2cNum))
        {
            I2C_DEGUB("wait write data timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /*   send stop flag bit*/
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_STOP);
    if (DRV_WaitWriteEnd(I2cNum))
    {
        I2C_DEGUB("wait write data timeout!\n");
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_I2C_Read(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData, HI_U32 DataLen)
{
    HI_U32          dataTmp = 0xff;
    HI_U32          i;
    HI_U32          RegAddr;

    if (I2cNum >= HI_STD_I2C_NUM)
    {
        I2C_DEGUB("%s I2cNum[%d] Error! Max:%d\n ", __FUNCTION__, I2cNum, HI_STD_I2C_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

    /* clear interrupt flag*/
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_ICR_REG), I2C_CLEAR_ALL);

    /* send devide address*/
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), (I2cDevAddr & WRITE_OPERATION));
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB),(I2C_WRITE | I2C_START));

    if (DRV_WaitWriteEnd(I2cNum))
    {
        I2C_DEGUB("wait write data timeout!\n");
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    /* send register address which will need to write*/
    for(i=0; i<I2cRegAddrByteNum; i++)
    {
        RegAddr = I2cRegAddr >> ((I2cRegAddrByteNum -i -1) * 8);
        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), RegAddr);

        I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_WRITE);

        if (DRV_WaitWriteEnd(I2cNum))
        {
            I2C_DEGUB("wait write data timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /* send register address which will need to read */
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), (I2cDevAddr | READ_OPERATION));
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_WRITE | I2C_START);

    if (DRV_WaitWriteEnd(I2cNum))
    {
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    /* read data */
    for(i=0; i<DataLen; i++)
    {
        /*  the last byte don't need send ACK*/
        if (i == (DataLen - 1))
        {
            I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), (I2C_READ | (~I2C_SEND_ACK)));
        }
        /*  if i2c master receive data will send ACK*/
        else
        {
            I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_READ);
        }

        if (I2C_DRV_WaitRead(I2cNum))
        {
            I2C_DEGUB("wait read data timeout!\n");
            return HI_ERR_I2C_READ_TIMEOUT;
        }

        dataTmp = I2C_READ_REG((g_I2cKernelAddr[I2cNum] + I2C_RXR_REG));
        *(pData+i)= dataTmp & 0xff;

        if (DRV_WaitWriteEnd(I2cNum))
        {
            I2C_DEGUB("wait write data timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /* send stop flag bit*/
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_STOP);
    if (DRV_WaitWriteEnd(I2cNum))
    {
        I2C_DEGUB("wait write data timeout!\n");
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_I2C_Read_si(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData, HI_U32 DataLen)
{
    HI_U32          dataTmp = 0xff;
    HI_U32          i;

    if (I2cNum >= HI_STD_I2C_NUM)
    {
        I2C_DEGUB("%s I2cNum[%d] Error! Max:%d\n ", __FUNCTION__, I2cNum, HI_STD_I2C_NUM);
        return HI_ERR_I2C_INVALID_PARA;
    }

       /* clear interrupt flag*/
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_ICR_REG), I2C_CLEAR_ALL);
	
    /* send register address which will need to read */
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), (I2cDevAddr | READ_OPERATION));
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_WRITE | I2C_START);

    if (DRV_WaitWriteEnd(I2cNum))
    {
        //local_irq_restore(IntFlag);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    /* repetitivily read data */
    for (i = 0; i < 1; i++)
    {
        /*  the last byte don't need send ACK*/
        if (i == (DataLen - 1))
        {
            I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), (I2C_READ | (~I2C_SEND_ACK)));
        }
        /*  if i2c master receive data will send ACK*/
        else
        {
            I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_READ);
        }

        if (I2C_DRV_WaitRead(I2cNum))
        {
            //local_irq_restore(IntFlag);
            //HI_ERR_I2C("wait read data timeout!\n");
            return HI_ERR_I2C_READ_TIMEOUT;
        }

        dataTmp = I2C_READ_REG((g_I2cKernelAddr[I2cNum] + I2C_RXR_REG));
        *(pData + i) = dataTmp & 0xff;

        if (DRV_WaitWriteEnd(I2cNum))
        {
            //local_irq_restore(IntFlag);
            //HI_ERR_I2C("wait write data timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }



    /* send register address which will need to read */
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_TXR_REG), (I2cDevAddr | READ_OPERATION));
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_WRITE | I2C_START);

    if (DRV_WaitWriteEnd(I2cNum))
    {
        //local_irq_restore(IntFlag);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    /* repetitivily read data */
    for (i = 0; i < DataLen; i++)
    {
        /*  the last byte don't need send ACK*/
        if (i == (DataLen - 1))
        {
            I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), (I2C_READ | (~I2C_SEND_ACK)));
        }
        /*  if i2c master receive data will send ACK*/
        else
        {
            I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_READ);
        }

        if (I2C_DRV_WaitRead(I2cNum))
        {
            //local_irq_restore(IntFlag);
            //HI_ERR_I2C("wait read data timeout!\n");
            return HI_ERR_I2C_READ_TIMEOUT;
        }

        dataTmp = I2C_READ_REG((g_I2cKernelAddr[I2cNum] + I2C_RXR_REG));
        *(pData + i) = dataTmp & 0xff;

        if (DRV_WaitWriteEnd(I2cNum))
        {
            //local_irq_restore(IntFlag);
            //HI_ERR_I2C("wait write data timeout!\n");
            return HI_ERR_I2C_WRITE_TIMEOUT;
        }
    }

    /* send stop flag bit*/
    I2C_WRITE_REG((g_I2cKernelAddr[I2cNum] + I2C_COM_REB), I2C_STOP);
    if (DRV_WaitWriteEnd(I2cNum))
    {
        //local_irq_restore(IntFlag);
        //HI_ERR_I2C("wait write data timeout!\n");
        return HI_ERR_I2C_WRITE_TIMEOUT;
    }

    return HI_SUCCESS;
}


HI_S32 HI_DRV_I2C_Init(HI_VOID)
{
    HI_U32  i;
    HI_U32 u32RegVal = 0;

    if (1 == i2cState)
    {
        return HI_SUCCESS;
    }

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)

    g_I2cKernelAddr[0] = (I2C0_PHY_ADDR);
    g_I2cKernelAddr[1] = (I2C1_PHY_ADDR);
    g_I2cKernelAddr[2] = (I2C2_PHY_ADDR);
    g_I2cKernelAddr[3] = (I2C3_PHY_ADDR);
    g_I2cKernelAddr[4] = (I2C4_PHY_ADDR);
    g_I2cKernelAddr[5] = (QAMI2C_PHY_ADDR);

    u32RegVal  = g_pstRegCrg->PERI_CRG27.u32;
    u32RegVal  &= ~0x222222;
    u32RegVal  |= 0x111111;
    g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;

#elif defined(CHIP_TYPE_hi3798mv100) \
	|| defined(CHIP_TYPE_hi3796mv100) \
	|| defined(CHIP_TYPE_hi3716dv100) 
    g_I2cKernelAddr[0] = (I2C0_PHY_ADDR);
    g_I2cKernelAddr[1] = (I2C1_PHY_ADDR);
    g_I2cKernelAddr[2] = (I2C2_PHY_ADDR);

    u32RegVal  = g_pstRegCrg->PERI_CRG27.u32;
    u32RegVal  &= ~0x222222;
    u32RegVal  |= 0x111111;
    g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;
#elif defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    g_I2cKernelAddr[0] = (I2C0_PHY_ADDR);
    g_I2cKernelAddr[1] = (I2C1_PHY_ADDR);
    g_I2cKernelAddr[2] = (I2C2_PHY_ADDR);

    u32RegVal  = g_pstRegCrg->PERI_CRG27.u32;
    u32RegVal  &= ~0x2220;
    u32RegVal  |= 0x1110;
    g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;
#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
	g_I2cKernelAddr[0] = (I2C0_PHY_ADDR);
    g_I2cKernelAddr[1] = (I2C1_PHY_ADDR);
    g_I2cKernelAddr[2] = (I2C2_PHY_ADDR);
    g_I2cKernelAddr[3] = (I2C3_PHY_ADDR);
    g_I2cKernelAddr[4] = (I2C4_PHY_ADDR);
	g_I2cKernelAddr[5] = (ADCI2C_PHY_ADDR);
    g_I2cKernelAddr[6] = (QAMI2C_PHY_ADDR);

    u32RegVal  = g_pstRegCrg->PERI_CRG27.u32;
    u32RegVal  &= ~0x2222222;
    u32RegVal  |= 0x1111111;
    g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;

#elif defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)

    g_I2cKernelAddr[0] = (I2C0_PHY_ADDR);
    g_I2cKernelAddr[1] = (I2C1_PHY_ADDR);
    g_I2cKernelAddr[2] = (I2C2_PHY_ADDR);
    g_I2cKernelAddr[3] = (I2C3_PHY_ADDR);
    g_I2cKernelAddr[4] = (I2C_QAMC_PHY_ADDR);
    g_I2cKernelAddr[5] = (I2C_QAMADC_PHY_ADDR);
    g_I2cKernelAddr[6] = (I2C_QAMT_PHY_ADDR);
    g_I2cKernelAddr[7] = (I2C7_PHY_ADDR);

    u32RegVal = g_pstRegCrg->PERI_CRG27.u32;
    u32RegVal &= ~0x78f78f;
    u32RegVal |= 0x78f;
    g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;

#elif defined(CHIP_TYPE_hi3798cv200)
	
		g_I2cKernelAddr[0] = (I2C0_PHY_ADDR);
		g_I2cKernelAddr[1] = (I2C1_PHY_ADDR);
		g_I2cKernelAddr[2] = (I2C2_PHY_ADDR);
		g_I2cKernelAddr[3] = (I2C3_PHY_ADDR);
		g_I2cKernelAddr[4] = (I2C4_PHY_ADDR);
	
		u32RegVal = g_pstRegCrg->PERI_CRG27.u32;
		u32RegVal &= ~0x22222222;
		u32RegVal |= 0x11111111;
		g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;
	
#elif defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
	
		g_I2cKernelAddr[0] = (I2C0_PHY_ADDR);
		g_I2cKernelAddr[1] = (I2C1_PHY_ADDR);
		g_I2cKernelAddr[2] = (I2C2_PHY_ADDR);
		g_I2cKernelAddr[3] = (I2C3_PHY_ADDR);
		g_I2cKernelAddr[4] = (I2C4_PHY_ADDR);
		g_I2cKernelAddr[5] = (I2C5_PHY_ADDR);
		g_I2cKernelAddr[6] = (ADCI2C_PHY_ADDR);
		g_I2cKernelAddr[7] = (QAMI2C_PHY_ADDR);
	
		u32RegVal = g_pstRegCrg->PERI_CRG27.u32;
		u32RegVal &= ~0x22222222;
		u32RegVal |= 0x11111111;
		g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;
#endif

    for (i = 0; i < HI_STD_I2C_NUM; i++)
    {
        /* disable all i2c interrupt */
        I2C_WRITE_REG((g_I2cKernelAddr[i] + I2C_CTRL_REG), 0x0);

        /*  config scl clk rate*/
        HI_DRV_I2C_SetRate(i, I2C_DFT_RATE);

        /* clear all i2c interrupt*/
        I2C_WRITE_REG((g_I2cKernelAddr[i] + I2C_ICR_REG), I2C_CLEAR_ALL);

        /*enable relative interrupt*/
        I2C_WRITE_REG((g_I2cKernelAddr[i] + I2C_CTRL_REG), (I2C_ENABLE | I2C_UNMASK_TOTAL | I2C_UNMASK_ALL));
    }

    i2cState = 1;

    //I2C_DEGUB("hi_i2c init success.  \t \n");

    return HI_SUCCESS;
}

HI_VOID HI_DRV_I2C_Exit(HI_VOID)
{
    HI_U32 u32RegVal;

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798mv200)    \
    || defined(CHIP_TYPE_hi3798mv200_a)
    u32RegVal  = g_pstRegCrg->PERI_CRG27.u32;
    u32RegVal  |= 0x222222;
    g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;
#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
	u32RegVal  = g_pstRegCrg->PERI_CRG27.u32;
	u32RegVal  |= 0x2222222;
	g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;
#elif defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) || defined(CHIP_TYPE_hi3798cv200)
	u32RegVal  = g_pstRegCrg->PERI_CRG27.u32;
    u32RegVal  |= 0x22222222;
    g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;	
#elif defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)
    u32RegVal = g_pstRegCrg->PERI_CRG27.u32;
    u32RegVal &= ~0x78f78f;
    u32RegVal |= 0x78f000;
    g_pstRegCrg->PERI_CRG27.u32 = u32RegVal;
#endif

    i2cState = 0;
    I2C_DEGUB("hi_i2c Release success.  \t \n");

    return;
}

