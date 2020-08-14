#include "lib.h"
#include "hash.h"

u32 hash_dma_rcv_len;

void hash_cfg_len(HASH_TYPE_E enType, u32 *size, u32 *pad_size)
{
    u32 total_len;
    u32 last_size;
    u32 val;
    u32 i;
    u32 count = 0;

    if ((size == NULL) || (pad_size == NULL))
    {
        return;
    }
    /*
    PERI_CRG49 为SHA 相关的时钟及软复位控制寄存器。
    [4] RW sha_srst_req
    sha 软复位请求。
    0：不复位；
    1：复位。
    [3:1] RW reserved 保留。
    [0] RW sha_cken
    sha 时钟门控。
    0：关断；
    1：打开。
    */
    //reset
    val = 0x10; 
    reg_set(HASH_REG_CRG, val);
    for(i=0; i<10000; i++)
    {
        count ++;//just for delay
    }

    //work
    val = 0x01; 
    reg_set(HASH_REG_CRG, val);    
    for(i=0; i<10000; i++)
    {
        count ++;//just for delay
    }

    hash_dma_rcv_len = 0;

    *size &= ~3; /* align word */
    total_len = (*size + 9 + 63) & (~63);
	last_size = *size % 64;      
	*pad_size = (last_size < 56) ? (55 - last_size) : (119 - last_size);
    /* Actually, pad_size at least is 3. When last_size equal 52, pad_size is 3. It's impossible less than 3.*/
    
    /* wait hash_rdy to be 1 */
    while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_HASH_RDY));

    /* total len */
    reg_set(HASH_REG_TOTAL_LEN1, total_len);
    reg_set(HASH_REG_TOTAL_LEN2, 0);

    /* sha control */
    val = reg_get(HASH_REG_SHA_CTRL);
    val &= ~(3<<1);
    reg_set(HASH_REG_SHA_CTRL, val | (enType << 1));
    
    /* sha start */
    reg_set(HASH_REG_SHA_START, 1);
}

void hash_cfg_dma(u8 *buf, u32 size)
{
    if (buf == NULL)
    {
        return;
    }
    /* sha control, select dma */
    reg_set(HASH_REG_SHA_CTRL, reg_get(HASH_REG_SHA_CTRL) & (~1));
    
    /* wait rec_rdy to be 1 */
    while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_REC_RDY));

    /* dma start addr, len */
    reg_set(HASH_REG_DMA_START_ADDR, (u32)buf);
    reg_set(HASH_REG_DMA_LEN, size);
    
    /* fix dma bug, wait for dma rev len == configed len */
    hash_dma_rcv_len += size;
    while(hash_dma_rcv_len != (reg_get(HASH_REG_REC_LEN1)));
}

void hash_cfg_cpu(u8 *buf, u32 size)
{
    u32 i;
    u32 data;
    u32 *ptr = (u32*)buf;
    
    if (buf == NULL)
    {
        return;
    }

    /* sha control, select cpu */
    reg_set(HASH_REG_SHA_CTRL, reg_get(HASH_REG_SHA_CTRL) | 1);

    size >>= 2;     /* size / 4 */
    for(i = 0; i < size; i++)
    {        
        data = ptr[i];
        /* wait rec_rdy to be 1 */
        while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_REC_RDY));
        
        reg_set(HASH_REG_DATA_IN, data);
    }
}

void hash_cfg_pad(u32 size, u32 pad_size)
{
    u32 i;
    u32 data;
    u8 len[8]={0};

    /* wait rec_rdy to be 1 */
    while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_REC_RDY));

    /* sha control */
    reg_set(HASH_REG_SHA_CTRL, reg_get(HASH_REG_SHA_CTRL) | 1);

    /* cal len */
    for (i = 0; i < 8; i++)
    {
        len[i] = ((size * 8) >> ((7 - i) * 8)) & 0xff;
    }
    
    /* wait rec_rdy to be 1 */
    while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_REC_RDY));

    /* first word */
    data = 0x80;
    reg_set(HASH_REG_DATA_IN, data);

    /* middle word */
    for(i = 3; i < pad_size; i+=4)
    {        
        /* wait rec_rdy to be 1 */
        while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_REC_RDY));
        
        reg_set(HASH_REG_DATA_IN, 0);
    }

    /* wait rec_rdy to be 1 */
    while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_REC_RDY));

    /* the last 2 word */
    data = (len[3]<<24) | (len[2]<<16) | (len[1]<<8) | len[0];
    reg_set(HASH_REG_DATA_IN, data);

    while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_REC_RDY));
    data = (len[7]<<24) | (len[6]<<16) | (len[5]<<8) | len[4];
    reg_set(HASH_REG_DATA_IN, data);
}

void hash_get_result(HASH_TYPE_E enType, u8 *out)
{
    u32 data, i;
    u32 len = (enType == MOD_HASH1) ? 20 : 32;

    if (out == NULL)
    {
        return;
    }

    /* wait hash_rdy to be 1 */
    while(!(reg_get(HASH_REG_SHA_STATUS) & SHA_STATUS_BIT_HASH_RDY));

    if((reg_get(HASH_REG_SHA_STATUS) & 0xf0) != 0)
    {
        out[0] = 0xee; /* error */
        return;
    }

    for(i = 0; i < len; i += 4)
    {
        data = reg_get(HASH_REG_SHA_OUT1 + i);
        out[i] = data & 0xff;
        out[i+1] = (data >> 8) & 0xff;
        out[i+2] = (data >> 16) & 0xff;
        out[i+3] = (data >> 24) & 0xff;
    }
    
}

