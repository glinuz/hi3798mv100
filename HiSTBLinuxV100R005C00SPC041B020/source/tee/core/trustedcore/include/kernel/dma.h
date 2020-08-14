#define DMA_MAX_SIZE							0xfffc

#define INT_STAT								0x00
#define INT_TC1									0x04
#define INT_ERR1								0x0c
#define INT_ERR2								0x10
#define INT_ERR3								0x14
#define INT_TC1_MASK							0x18
#define INT_ERR1_MASK							0x20
#define INT_ERR2_MASK							0x24
#define INT_ERR3_MASK							0x28
#define INT_TC1_RAW								0x600
#define INT_ERR1_RAW							0x608
#define INT_ERR2_RAW							0x610
#define CH_PRI									0x688
#define CH_STAT									0x690
#define CX_CUR_CNT								0x704
#define CX_LLI									0x800
#define CX_CNT									0x810
#define CX_SRC									0x814
#define CX_DST									0x818
#define CX_CONFIG								0x81c
#define AXI_CONFIG								0x820
#define DEF_AXI_CONFIG							0x201201

#define CX_LLI_CHAIN_EN							0x2
#define CCFG_EN									0x1
#define CCFG_SRCINCR							(0x1 << 31)
#define CCFG_DSTINCR							(0x1 << 30)

// DMA clk value
#define CRG_CLK_DMA_ENABLE						0x30
#define CRG_CLK_DMA_DISABLE						0x34
#define CRG_CLK_DMA_STATUS						0x38
#define GT_CLK_DMA0							0x00000001
#define GT_CLK_DMA1							0x00000002

// CRG PERI DMA
#define CRG_RST_DMA_ENABLE						0x84
#define CRG_RST_DMA_DISABLE						0x88
#define CRG_RST_DMA_STATUS						0x8c
#define GT_RST_DMA0_BIT							0

// DMA return value
#define DMA_TRUE                                1
#define DMA_FALSE                               0

struct hisi_desc_hw {
	u32	lli;
	u32 reserved[3];
	u32 count;
	u32 saddr;
	u32 daddr;
	u32 config;
};

struct hisi_dma_desc_sw {
	u32     reserved[5];
	u32	desc_hw_lli;
	u32	desc_num;
	u32	size;
	struct hisi_desc_hw desc_hw[0];
};

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BIT(x)  ((u32)1<<(x))

extern int hisi_dma_memcpy(void *dst, void *src, u32 len);
extern void hisi_dma_reset(void);
