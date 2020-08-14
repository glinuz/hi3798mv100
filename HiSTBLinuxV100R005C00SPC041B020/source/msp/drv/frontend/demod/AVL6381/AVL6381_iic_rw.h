#ifndef AVL6381_IIC_RW_H
#define AVL6381_IIC_RW_H

#include "AVL6381.h"
#include "AVL6381_data_type.h"

#define AVL6381_MAX_II2C_Write_SIZE		64
#define AVL6381_MAX_II2C_READ_SIZE		64

#define AVL6381_EC_OK                   0           // There is no error.
#define AVL6381_EC_WARNING              1           // There is warning.
#define AVL6381_EC_GeneralFail	        2           // A general failure has occurred.
#define AVL6381_EC_I2C_FAIL             4           // An I2C operation failed during communication with the AVL6381 through the BSP.
#define AVL6381_EC_I2C_REPEATER_FAIL    8           // An error ocurred during communication between AVL6381 I2C master and tuner. This error code is defined by enum AVL6381_MessageType_II2C_Repeater_Status.                         
#define AVL6381_EC_RUNNING              16          // The AVL6381 device is busy processing a previous receiver/i2c repeater command.
#define AVL6381_EC_TIMEOUT              32          // Operation failed in a given time period
#define AVL6381_EC_SLEEP                64          // Demod in sleep mode
#define AVL6381_EC_NOT_SUPPORTED        128         // Specified operation isn't support in current senario
#define AVL6381_EC_BSP_ERROR1           256         // BSP Error 1, if it's used, need to be customized
#define AVL6381_EC_BSP_ERROR2           512         // BSP Error 2, if it's used, need to be customized


extern AVL6381_IIC_ErrorCode AVL6381_I2CRead(  const struct AVL6381_Chip * pAVLChip,  AVL6381_puchar pucBuff, AVL6381_puint16 puiSize );
extern AVL6381_IIC_ErrorCode AVL6381_I2CWrite(  const struct AVL6381_Chip * pAVLChip,  AVL6381_puchar pucBuff,  AVL6381_puint16  puiSize );
extern AVL6381_IIC_ErrorCode AVL6381_II2C_Read( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiOffset, AVL6381_puchar pucBuff, AVL6381_uint16 uiSize);
extern AVL6381_IIC_ErrorCode AVL6381_II2C_Write( const struct AVL6381_Chip * pAVLChip, AVL6381_puchar pucBuff, AVL6381_uint16 uiSize);
extern AVL6381_ErrorCode 	 AVL6381_II2C_Read8(const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_puchar puiData);
extern AVL6381_IIC_ErrorCode AVL6381_II2C_Read16( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_puint16 puiData );
extern AVL6381_IIC_ErrorCode AVL6381_II2C_Read32( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_puint32 puiData );
extern AVL6381_IIC_ErrorCode AVL6381_II2C_Write16( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_uint16 uiData );
extern AVL6381_IIC_ErrorCode AVL6381_II2C_Write32( const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_uint32 uiData );
extern AVL6381_IIC_ErrorCode AVL6381_II2C_Initialize(void);
extern AVL6381_IIC_ErrorCode AVL6381_II2C_Write8(const struct AVL6381_Chip * pAVLChip, AVL6381_uint32 uiAddr, AVL6381_uchar ucData);

extern void AVL6381_ChunkAddr(AVL6381_uint32 uiaddr, AVL6381_puchar pBuff);
extern void AVL6381_Chunk16(AVL6381_uint16 uidata, AVL6381_puchar pBuff);
extern AVL6381_uint16 AVL6381_DeChunk16(const AVL6381_puchar pBuff);
extern void AVL6381_Chunk32(AVL6381_uint32 uidata, AVL6381_puchar pBuff);
extern AVL6381_uint32 AVL6381_DeChunk32(const AVL6381_puchar pBuff);
extern AVL6381_uint32 AVL6381_Divide64(struct AVL6381_uint64 divisor, struct AVL6381_uint64 dividend);
extern void AVL6381_AddScaled32To64(struct AVL6381_uint64 *pDst, AVL6381_uint32 a);
extern void AVL6381_Multiply32(struct AVL6381_uint64 *pDst, AVL6381_uint32 m1, AVL6381_uint32 m2);
extern void AVL6381_Add32To64(struct AVL6381_uint64 *pSum, AVL6381_uint32 uiAddend);
extern void AVL6381_Multiply32By16(AVL6381_int64 *pY, AVL6381_int32 a, AVL6381_int16 b);


#endif

