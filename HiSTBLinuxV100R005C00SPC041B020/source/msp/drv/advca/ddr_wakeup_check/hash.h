#ifndef __HASH_H__
#define __HASH_H__

typedef enum 
{
	MOD_HASH1 = 0, 
	MOD_HASH256,
	MOD_HMAC_HASH1,
	MOD_HMAC_HASH256,
	MOD_BUTT
}HASH_TYPE_E;

#if   defined (CHIP_TYPE_hi3716mv410) \
   || defined (CHIP_TYPE_hi3716mv420) \
   || defined (CHIP_TYPE_hi3716cv200) \
   || defined (CHIP_TYPE_hi3719cv100) \
   || defined (CHIP_TYPE_hi3719mv100) \
   || defined (CHIP_TYPE_hi3798mv100) \
   || defined (CHIP_TYPE_hi3796mv100) \
   || defined (CHIP_TYPE_hi3798cv200) \
   || defined(CHIP_TYPE_hi3798mv200) \
   || defined(CHIP_TYPE_hi3798mv200_a)
#define HASH_REG_BASE           (0xf9a10000)  /* hash reg base: 0xf9a1_0000*/
#define HASH_REG_CRG            (0xf8a220c4)  /* hash crg reg base: 0xf8a22000*/
#endif

#if  defined (CHIP_TYPE_hi3716mv310) \
  || defined (CHIP_TYPE_hi3110ev500) \
  || defined (CHIP_TYPE_hi3716mv330)
#define HASH_REG_BASE           (0x60170000)  /* hash reg base: 0x6017_0000*/
#define HASH_REG_CRG            (0x101f50c4)  /* hash crg reg base: 0x101f50c4*/
#endif

#define HASH_REG_TOTAL_LEN1     (HASH_REG_BASE + 0x00)
#define HASH_REG_TOTAL_LEN2     (HASH_REG_BASE + 0x04)
#define HASH_REG_SHA_STATUS     (HASH_REG_BASE + 0x08)
#define HASH_REG_SHA_CTRL       (HASH_REG_BASE + 0x0C)
#define HASH_REG_SHA_START      (HASH_REG_BASE + 0x10)
#define HASH_REG_DMA_START_ADDR (HASH_REG_BASE + 0x14)
#define HASH_REG_DMA_LEN        (HASH_REG_BASE + 0x18)
#define HASH_REG_DATA_IN        (HASH_REG_BASE + 0x1C)
#define HASH_REG_REC_LEN1       (HASH_REG_BASE + 0x20)
#define HASH_REG_REC_LEN2       (HASH_REG_BASE + 0x24)
#define HASH_REG_SHA_OUT1       (HASH_REG_BASE + 0x30)

#define SHA_STATUS_BIT_HASH_RDY     (1<<0)
#define SHA_STATUS_BIT_DMA_RDY      (1<<1)
#define SHA_STATUS_BIT_MSG_RDY      (1<<2)
#define SHA_STATUS_BIT_REC_RDY      (1<<3)

void hash_cfg_len(HASH_TYPE_E enType, unsigned int *size, unsigned int *pad_size);
void hash_cfg_dma(unsigned char *buf, unsigned int size);
void hash_cfg_cpu(unsigned char *buf, unsigned int size);
void hash_cfg_pad(unsigned int size, unsigned int pad_size);
void hash_get_result(HASH_TYPE_E enType, unsigned char *out);


#endif /*__HASH_H__*/
