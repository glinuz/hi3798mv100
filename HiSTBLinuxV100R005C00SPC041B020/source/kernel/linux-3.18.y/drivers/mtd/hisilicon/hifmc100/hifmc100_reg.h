/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.29
 *
******************************************************************************/

#ifndef HIFMC100_REG_H
#define HIFMC100_REG_H

/*****************************************************************************/
#define HIFMC100_CFG                                       0x00
#define HIFMC100_CFG_SPI_NOR_ADDR_MODE_4B        0x400

#define HIFMC100_CFG_ECC_TYPE_MASK               0xE0
#define HIFMC100_CFG_ECC_TYPE_NONE               0x00
#define HIFMC100_CFG_ECC_TYPE_8BIT               0x20
#define HIFMC100_CFG_ECC_TYPE_16BIT              0x40
#define HIFMC100_CFG_ECC_TYPE_24BIT              0x60
#define HIFMC100_CFG_ECC_TYPE_28BIT              0x80
#define HIFMC100_CFG_ECC_TYPE_40BIT              0xA0
#define HIFMC100_CFG_ECC_TYPE_64BIT              0xC0

#define HIFMC100_CFG_PAGE_SIZE_MASK              0x18
#define HIFMC100_CFG_PAGE_SIZE_2K                0x00
#define HIFMC100_CFG_PAGE_SIZE_4K                0x08
#define HIFMC100_CFG_PAGE_SIZE_8K                0x10
#define HIFMC100_CFG_PAGE_SIZE_16K               0x18

#define HIFMC100_CFG_FLASH_SEL_MASK              0x6
#define HIFMC100_CFG_FLASH_SEL_SPINOR            0x0
#define HIFMC100_CFG_FLASH_SEL_SPINAND           0x2
#define HIFMC100_CFG_FLASH_SEL_NAND              0x4

#define HIFMC100_CFG_OP_MODE_NORMAL              0x01
/*****************************************************************************/
#define HIFMC100_GLOBAL_CFG                                0x04
#define HIFMC100_GLOBAL_CFG_RANDOMIZER_EN        (1 << 2)
#define HIFMC100_GLOBAL_CFG_EDO_MODE             (1 << 9)

/*****************************************************************************/
#define HIFMC100_TIMING_SPI_CFG                            0x08
#define HIFMC100_PND_PWIDTH_CFG                            0x0c

/*****************************************************************************/
#define HIFMC100_INT                                       0x18
#define HIFMC100_INT_CACHE_PAGE_DONE             BIT(8)
#define HIFMC100_INT_AHB_OP                      BIT(7)
#define HIFMC100_INT_WR_LOCK                     BIT(6)
#define HIFMC100_INT_DMA_ERR                     BIT(5)
#define HIFMC100_INT_ERR_ALARM                   BIT(4)
#define HIFMC100_INT_ERR_INVALID                 BIT(3)
#define HIFMC100_INT_ERR_VALID                   BIT(2)
#define HIFMC100_INT_OP_FAIL                     BIT(1)
#define HIFMC100_INT_OP_DONE                     BIT(0)
/*****************************************************************************/
#define HIFMC100_INT_EN                                    0x1c
#define HIFMC100_INT_EN_CACHE_PAGE_DONE          BIT(8)
#define HIFMC100_INT_EN_AHB_OP                   BIT(7)
#define HIFMC100_INT_EN_WR_LOCK                  BIT(6)
#define HIFMC100_INT_EN_DMA_ERR                  BIT(5)
#define HIFMC100_INT_EN_ERR_ALARM                BIT(4)
#define HIFMC100_INT_EN_ERR_INVALID              BIT(3)
#define HIFMC100_INT_EN_ERR_VALID                BIT(2)
#define HIFMC100_INT_EN_OP_FAIL                  BIT(1)
#define HIFMC100_INT_EN_OP_DONE                  BIT(0)
#define HIFMC100_INT_EN_ALL                      0x1FF
/*****************************************************************************/
#define HIFMC100_INT_CLR                                   0x20
#define HIFMC100_INT_CLR_CACHE_PAGE_DONE         BIT(8)
#define HIFMC100_INT_CLR_AHB_OP                  BIT(7)
#define HIFMC100_INT_CLR_WR_LOCK                 BIT(6)
#define HIFMC100_INT_CLR_DMA_ERR                 BIT(5)
#define HIFMC100_INT_CLR_ERR_ALARM               BIT(4)
#define HIFMC100_INT_CLR_ERR_INVALID             BIT(3)
#define HIFMC100_INT_CLR_ERR_VALID               BIT(2)
#define HIFMC100_INT_CLR_OP_FAIL                 BIT(1)
#define HIFMC100_INT_CLR_OP_DONE                 BIT(0)
#define HIFMC100_INT_CLR_ALL                     0x1FF
/*****************************************************************************/
#define HIFMC100_CMD                                       0x24

/*****************************************************************************/
#define HIFMC100_ADDRH                                     0x28
#define HIFMC100_ADDRH_BYTE5(_addr)              ((_addr) & 0xff)

/*****************************************************************************/
#define HIFMC100_ADDRL                                     0x2c

/*****************************************************************************/
#define HIFMC100_OP_CFG                                    0x30
#define HIFMC100_OP_CFG_FM_CS                    0x800
#define HIFMC100_OP_CFG_NUM_CS                   1
#define HIFMC100_OP_CFG_FORCE_CS_EN              0x400
#define HIFMC100_OP_CFG_MEM_IFTYPE_SHIFT         7
#define HIFMC100_OP_CFG_MEM_IFTYPE_MASK          0x380
#define HIFMC100_OP_CFG_MEM_IFTYPE_STD           0x0
#define HIFMC100_OP_CFG_MEM_IFTYPE_DUAL          0x80
#define HIFMC100_OP_CFG_MEM_IFTYPE_DUAL_ADDR     0x100
#define HIFMC100_OP_CFG_MEM_IFTYPE_QUAD          0x180
#define HIFMC100_OP_CFG_MEM_IFTYPE_QUAD_ADDR     0x200
#define HIFMC100_OP_CFG_ADDR_NUM(_addr)          (((_addr) & 0x7) << 4)
#define HIFMC100_OP_CFG_ADDR_MASK                0x70
#define HIFMC100_OP_CFG_DUMMY_NUM(_dummy)        ((_dummy) & 0xf)
#define HIFMC100_OP_CFG_DUMMY_MASK               0x0f

/*****************************************************************************/
#define HIFMC100_DATA_NUM                                  0x38
#define HIFMC100_DATA_NUM_CNT(_n)                ((_n) & 0x3FFF)

/*****************************************************************************/
#define HIFMC100_OP                                        0x3c
#define HIFMC100_OP_DUMMY_EN                     0x100
#define HIFMC100_OP_CMD1_EN                      0x80
#define HIFMC100_OP_ADDR_EN                      0x40
#define HIFMC100_OP_WRITE_DATA_EN                0x20
#define HIFMC100_OP_CMD2_EN                      0x10
#define HIFMC100_OP_WAIT_READY_EN                0x8
#define HIFMC100_OP_READ_DATA_EN                 0x4
#define HIFMC100_OP_READ_STATUS_EN               0x2
#define HIFMC100_OP_REG_OP_START                 0x1

/*****************************************************************************/
#define HIFMC100_DMA_LEN                                   0x40
#define HIFMC100_DMA_LEN_MASK(_len)              ((_len) & 0x0fffffff)

/*****************************************************************************/

#define HIFMC100_DMA_AHB_CTRL                              0x48
#define HIFMC100_DMA_SADDR_D0                              0x4c
#define HIFMC100_DMA_SADDR_D1                              0x50
#define HIFMC100_DMA_SADDR_D2                              0x54
#define HIFMC100_DMA_SADDR_D3                              0x58
#define HIFMC100_DMA_SADDR_OOB                             0x5c

/*****************************************************************************/
#define HIFMC100_OP_CTRL                                   0x68
#define HIFMC100_OP_CTRL_RD_OPCODE(_code)        (((_code) & 0xff) << 16)
#define HIFMC100_OP_CTRL_WR_OPCODE(_code)        (((_code) & 0xff) << 8)
#define HIFMC100_OP_CTRL_RD_OP_SEL_MASK          0x30
#define HIFMC100_OP_CTRL_RD_OP_SEL_CACHE         0x30
#define HIFMC100_OP_CTRL_RD_OP_SEL_BLK           0x20
#define HIFMC100_OP_CTRL_RD_OP_SEL_OOB           0x10
#define HIFMC100_OP_CTRL_RD_OP_SEL_PAGE          0x00
#define HIFMC100_OP_CTRL_RW_OP_WRITE             0x02
#define HIFMC100_OP_CTRL_DMA_OP_READY            0x01
/*****************************************************************************/
#define HIFMC100_OP_PARA                                   0x70
#define HIFMC100_OP_PARA_RD_OOB_ONLY             BIT(1)

/*****************************************************************************/
#define HIFMC100_STATUS                                    0xac

/*****************************************************************************/
#define HIFMC100_VERSION                                   0xbc
#define HIFMC100_VERSION_VALUE                   (0x100)

#define HIFMC100_FMC_ERR_NUM0_BUF0                         0xc0

#define HIFMC100_FMC_ECC_INVALID_ADDRH                     0xD8
#define HIFMC100_FMC_ECC_INVALID_ADDRL                     0xDC

#define HIFMC100_READ_TIMING_TUNE                          0xE0
#define HIFMC100_READ_TIMING_TUNE_VALUE          (2)

#define HIFMC100_FMC_CACHE_CTRL                            0x150
#define HIFMC100_FMC_CACHE_EXIT_OP_EN            0x800
#define HIFMC100_FMC_CACHE_TERMINATE             0x200
#define HIFMC100_FMC_CACHE_PAGE_NUM_MASK         0x0F

#define HIFMC100_FMC_DMA_SADDR_D0_P1                       0x154
/*****************************************************************************/

#define DUMP_READ(_s)

#define hifmc_read(_host, _reg) ({\
	u32 _regval = readl(_host->regbase + (_reg)); \
	DUMP_READ(printk("%s:%d: readl(0x%08x)=0x%08x\n", __FILE__, __LINE__, (u32)_reg, _regval)); \
	_regval;})

#define DUMP_WRITE(_s)

#define hifmc_write(_host, _value, _reg) do {\
		DUMP_WRITE(printk("%s:%d: writel(0x%08x)=0x%08x\n", __FILE__, __LINE__, (u32)_reg, (u32)_value)); \
		writel((_value), _host->regbase + (_reg)); \
	} while (0)

/******************************************************************************/
#endif /* HIFMC100_REG_H */
