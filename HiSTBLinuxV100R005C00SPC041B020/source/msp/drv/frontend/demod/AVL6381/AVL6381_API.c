#include "AVL6381_API.h"
#include "AVL6381.h"
#include "AVL6381_Internal.h"

//AVL6381_ErrorCode AVL6381_I2CBypassOn(HI_U8 IICchannel, HI_U32 IICaddr)
AVL6381_ErrorCode AVL6381_I2CBypassOn(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
	//struct AVL6381_Chip AVL6381_Chip_Temp;
	
	//AVL6381_Chip_Temp.m_I2CChn = IICchannel;
	//AVL6381_Chip_Temp.m_SlaveAddr = IICaddr;
    r = AVL6381_II2C_Write32( pAVLChip, hw_AVL6381_tuner_hw_i2c_bit_rpt_cntrl, AVL6381_BIT_REPEATER_WAIT_ACK|AVL6381_BIT_REPEATER_SEL|AVL6381_BIT_REPEATER_ENABLE);
    r = AVL6381_II2C_Write32( pAVLChip, hw_AVL6381_tuner_hw_i2c_bit_rpt_cntrl, AVL6381_BIT_REPEATER_WAIT_ACK|AVL6381_BIT_REPEATER_SEL|AVL6381_BIT_REPEATER_ENABLE);
	r = AVL6381_II2C_Write32( pAVLChip, hw_AVL6381_tuner_hw_i2c_bit_rpt_cntrl, AVL6381_BIT_REPEATER_WAIT_ACK|AVL6381_BIT_REPEATER_SEL|AVL6381_BIT_REPEATER_ENABLE);

    return r;
}

//AVL6381_ErrorCode AVL6381_I2CBypassOff(HI_U8 IICchannel, HI_U32 IICaddr)
AVL6381_ErrorCode AVL6381_I2CBypassOff(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
	//struct AVL6381_Chip AVL6381_Chip_Temp;

	//AVL6381_Chip_Temp.m_I2CChn = IICchannel;
	//AVL6381_Chip_Temp.m_SlaveAddr = IICaddr;
    r = AVL6381_II2C_Write32( pAVLChip, hw_AVL6381_tuner_hw_i2c_bit_rpt_cntrl, AVL6381_BIT_REPEATER_WAIT_ACK|AVL6381_BIT_REPEATER_SEL|!AVL6381_BIT_REPEATER_ENABLE);
    r = AVL6381_II2C_Write32( pAVLChip, hw_AVL6381_tuner_hw_i2c_bit_rpt_cntrl, AVL6381_BIT_REPEATER_WAIT_ACK|AVL6381_BIT_REPEATER_SEL|!AVL6381_BIT_REPEATER_ENABLE);
	r = AVL6381_II2C_Write32( pAVLChip, hw_AVL6381_tuner_hw_i2c_bit_rpt_cntrl, AVL6381_BIT_REPEATER_WAIT_ACK|AVL6381_BIT_REPEATER_SEL|!AVL6381_BIT_REPEATER_ENABLE);
	return r;
}

AVL6381_uint16 AVL6381_GetChipID(struct AVL6381_Chip * pAVLChip, AVL6381_puint32 puiChipID)
{
	AVL6381_uint16 r = AVL6381_EC_OK;
	AVL6381_uint32 ChipIDRegAddr = hw_AVL6381_chip_id;
    AVL6381_uchar pucBuffAddr[3] = {0};
    AVL6381_uchar pucBuffData[4]= {0};
    AVL6381_uint16 usAddrSize = 3;
    AVL6381_uint16 usDataSize = 4;

	AVL6381_ChunkAddr(ChipIDRegAddr,pucBuffAddr);
	r = AVL6381_I2CWrite(pAVLChip,  pucBuffAddr,  &usAddrSize );
	r |= AVL6381_I2CRead(pAVLChip,pucBuffData, &usDataSize);
	*puiChipID = AVL6381_DeChunk32(pucBuffData);	
	return r;
}

AVL6381_ErrorCode AVL6381_GetVersion(AVL6381_VerInfo * pVerInfo, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_uint32 uiTemp;
    AVL6381_uchar ucBuff[4];
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r =  AVL6381_II2C_Read32(pAVLChip, hw_AVL6381_rom_ver, &uiTemp);

    if( AVL6381_EC_OK == r )
    {
        AVL6381_Chunk32(uiTemp, ucBuff);
        pVerInfo->m_Chip.m_Major = ucBuff[0];
        pVerInfo->m_Chip.m_Minor = ucBuff[1];
        pVerInfo->m_Chip.m_Build = ucBuff[2];
        pVerInfo->m_Chip.m_Build = ((AVL6381_uint16)((pVerInfo->m_Chip.m_Build)<<8)) + ucBuff[3];

        r |=  AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_DTMB_patch_ver_iaddr, &uiTemp);
        if( AVL6381_EC_OK == r )
        {
            AVL6381_Chunk32(uiTemp, ucBuff);
            pVerInfo->m_Patch.m_Major = ucBuff[0];
            pVerInfo->m_Patch.m_Minor = ucBuff[1];
            pVerInfo->m_Patch.m_Build = ucBuff[2];
            pVerInfo->m_Patch.m_Build = ((AVL6381_uint16)((pVerInfo->m_Patch.m_Build)<<8)) + ucBuff[3];

            pVerInfo->m_API.m_Major = AVL6381_API_VER_MAJOR;
            pVerInfo->m_API.m_Minor = AVL6381_API_VER_MINOR;
            pVerInfo->m_API.m_Build = AVL6381_API_VER_BUILD;
        }
    }
    
    return r;
}


HI_S32 AVL6381_Mutex_init(struct semaphore * stS)
{
	HI_INIT_MUTEX(stS);
    return 0;
}

void AVL6381_Mutex_Lock(struct semaphore * stS)
{
    HI_S32 s32Ret;
    s32Ret = down_interruptible(stS);
}

void AVL6381_Mutex_Unlock(struct semaphore * stS)
{
    up(stS);
}


