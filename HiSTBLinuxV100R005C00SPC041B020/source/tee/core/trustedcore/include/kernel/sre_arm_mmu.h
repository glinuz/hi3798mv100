
#ifndef __SRE_ARM_MMU__
#define __SRE_ARM_MMU__

#define ALIGN_UP(x, align) (((x) + ((align)-1)) & ~((align)-1))
#define ALIGN_DOWN(x, align)  ((x) & ~((align)-1))

#define PAGE_SHIFT              12
#define PAGE_SIZE               (1 << PAGE_SHIFT)
#define PAGE_MASK               (~(PAGE_SIZE-1)) /* 0xFFFFF000 */
#define PAGE_MASK_64BIT         0xFFFFFFF000
#define PAGE_OFFSET_MASK        (PAGE_SIZE-1) /* 0xFFF*/

#define PAGE_ALIGN_UP(x)        ALIGN_UP(x, PAGE_SIZE)
#define PAGE_ALIGN_DOWN(x)      ALIGN_DOWN(x, PAGE_SIZE)
#define IS_PAGE_ALIGNED(addr)   (((addr) & PAGE_OFFSET_MASK) == 0)


#ifdef ARM_PAE
#define PMD_ORDER               3
#define SECTION_SHIFT           21
#else
#define PMD_ORDER               2
#define SECTION_SHIFT           20
#endif

#define SECTION_SIZE                (1 << SECTION_SHIFT)
#define SECTION_MASK                (~(SECTION_SIZE-1)) /* 0xFFF00000 */
#define SECTION_MASK_64BIT          0xFFFFE00000
#define SECTION_OFFSET_MASK         (SECTION_SIZE - 1) /* 0x000FFFFF */

#define SECTION_ALIGN_UP(x)         ALIGN_UP(x, SECTION_SIZE)
#define SECTION_ALIGN_DOWN(x)       ALIGN_DOWN(x, SECTION_SIZE)
#define IS_SECTION_ALIGNED(addr)    (((addr) & SECTION_OFFSET_MASK) == 0)

#define VIR_ASPACE_SIZE         (1 << 26)

#define PAGE_TABLE_ENTRIES      0x1000
#define PAGE_TABLE_ENTRY_WIDTH  4
#define PAGE_TABLE_SIZE         (PAGE_TABLE_ENTRIES * PAGE_TABLE_ENTRY_WIDTH)
#define PG_DIR_SIZE              PAGE_TABLE_SIZE
#define KERNEL_ALIGN_SIZE       (1 << 20)





#define SECURE_ACCESS_DOMAIN    0

#define L1_SECTION_AP_MASK      0x3

/* XN Bit Values */
#define L1_EXECUTE_NEVER        1
#define L1_EXECUTE_ALLOW        0

#define L1_TYPE_MASK            0x3
#define L1_TYPE_FAULT           0x0
#define L1_TYPE_COARSE          0x1
#define L1_TYPE_PAGE_TABLE      0x1
#define L1_TYPE_SECTION         0x2
#define L1_TYPE_SMALL_PAGE      0x01

#define L1_SECTION_PXN_SHIFT    0
#define L1_SECTION_B_SHIFT      2
#define L1_SECTION_C_SHIFT      3
#define L1_SECTION_XN_SHIFT     4
#define L1_DOMAIN_SHIFT         5
#define L1_SECTION_AP_SHIFT     10
#define L1_SECTION_TEX_SHIFT    12
#define L1_SECTION_AP2_SHIFT    15
#define L1_SECTION_S_SHIFT      16
#define L1_SECTION_NG_SHIFT     17  /* Non-Global bit*/
#define L1_SECTION_NS_SHIFT     19  /* Non-Secure bit*/
#define L1_SECTION_NS_MASK      0x1
#define L1_SECTION_NS_IDX_MASK  (L1_SECTION_NS_MASK << L1_SECTION_NS_SHIFT)
#define L1_DOMAIN_MASK          0xf
#define L1_DOMAIN_IDX_MASK      (L1_DOMAIN_MASK << L1_DOMAIN_SHIFT)
#define L1_PAGE_NS_SHIFT        3
#define L1_PAGE_PXN_SHIFT       2


#define L2_PAGE_XN_SHIFT      0
#define L2_PAGE_B_SHIFT       2
#define L2_PAGE_C_SHIFT       3
#define L2_PAGE_AP_SHIFT      4
#define L2_PAGE_TEX_SHIFT     6
#define L2_PAGE_AP2_SHIFT     9
#define L2_PAGE_S_SHIFT       10
#define L2_PAGE_NG_SHIFT      11  /* Non-Global bit*/


#define SECTION_BASE_MASK       0xfff00000
#define SECTION_BASE_MASK_64BIT 0xfffffffffff00000
#define SMALL_PAGE_MASK         0xfffff000
#define SMALL_PAGE_MASK_64BIT   0xfffffffffffff000

#define FSR_TYPE_MASK           0x40f
#define FSR_ALIGN_FAULT         0x1
#define FSR_EXT_ABORT_L1        0xc
#define FSR_EXT_ABORT_L2        0xe
#define FSR_TRANS_SEC           0x5
#define FSR_TRANS_PAGE          0x7
#define FSR_DOMAIN_SEC          0x9
#define FSR_DOMAIN_PAG          0xb
#define FSR_PERM_SEC            0xd
#define FSR_PERM_PAGE           0xf

#define FSR_DOMAIN_MASK         0xf0
#define FSR_WNR_MASK            0x800
#define FSR_EXT_MASK            0x1000

/*h00206996 modify begin, pagetable attributes*/
#define TTBR_FLAGS_C_BIT        (0x1 << 0)
#define TTBR_FLAGS_S_BIT        (0x1 << 1)
#define TTBR_FLAGS_RGN(val)     ((val & 0x3) << 3)
#define TTBR_FLAGS_NOS_BIT      (0x1 << 5)
#define TTBR_FLAGS_IRGN_BIT     (0x1 << 6)

/*outer and Inner Write-Back, Write-Allocate
  consistent with pagetable memory attribtue TEX C B 111
 */
#define TTBR_FLAGS (TTBR_FLAGS_IRGN_BIT |TTBR_FLAGS_RGN(0x01) |TTBR_FLAGS_S_BIT | TTBR_FLAGS_NOS_BIT)
/*h00206996 modify end*/


#define MAIR_ATTR_SET(attr, index) (((uint64_t)attr) << ((index) << 3))

/* Normal memory, Inner/Outer Non-cacheable */
#define TTBCR_XRGNX_NC          0x0
/* Normal memory, Inner/Outer Write-Back Write-Allocate Cacheable */
#define TTBCR_XRGNX_WB          0x1
/* Normal memory, Inner/Outer Write-Through Cacheable */
#define TTBCR_XRGNX_WT          0x2
/* Normal memory, Inner/Outer Write-Back no Write-Allocate Cacheable */
#define TTBCR_XRGNX_WBWA        0x3

/* Non-shareable */
#define TTBCR_SHX_NSH           0x0
/* Outer Shareable */
#define TTBCR_SHX_OSH           0x2
/* Inner Shareable */
#define TTBCR_SHX_ISH           0x3

#define TTBCR_EAE               (1 << 31)

/* When TTBCR.EAE == 1 */
#define TTBCR_T0SZ_SHIFT        0
#define TTBCR_EPD0              (1 << 7)
#define TTBCR_IRGN0_SHIFT       8
#define TTBCR_ORGN0_SHIFT       10
#define TTBCR_SH0_SHIFT         12
#define TTBCR_T1SZ_SHIFT        16
#define TTBCR_A1                (1 << 22)
#define TTBCR_EPD1              (1 << 23)
#define TTBCR_IRGN1_SHIFT       24
#define TTBCR_ORGN1_SHIFT       26
#define TTBCR_SH1_SHIFT         28


/* LPAE only defines */
#define LPAE_TYPE_INVALID_DESC          0x0
#define LPAE_TYPE_BLOCK_DESC            0x1
#define LPAE_TYPE_TABLE_DESC            0x3
#define LPAE_TYPE_PAGE_DESC             0x3

#define LPAE_TYPE_HIDDEN_DESC           0x4
#define LPAE_TYPE_PHYSPAGE_DESC       	0x8

#define LPAE_ATTR_NS_TABLE             (1ull << 4)
#define LPAE_ATTR_TABLE_AP_RO          (3ull << 2)
#define LPAE_ATTR_TABLE_AP_RW          (1ull << 2)
#define LPAE_ATTR_TABLE_AP_UNPRIV      (0ull << 2)
#define LPAE_ATTR_XN_TABLE             (1ull << 1)
#define LPAE_ATTR_PXN_TABLE            (1ull << 0)

#define LPAE_TABLE_ATTRS(x)            (((x) & 0xf) << 59)

#define LPAE_ATTR_XN                    (1ull << 2)
#define LPAE_ATTR_PXN                   (1ull << 1)
#define LPAE_ATTR_CONT_HINT             (1ull << 0)

#define LPAE_UPPER_ATTRS(x)            	(((x) & 0x7) << 52)
#define LPAE_ATTR_NON_GLOBAL            (1ull << 9)
#define LPAE_ATTR_ACCESS_FLAG           (1 << 8)
#define LPAE_ATTR_NSH                   (0x0 << 6)
#define LPAE_ATTR_OSH                 	(0x2 << 6)
#define LPAE_ATTR_ISH                 	(0x3 << 6)

/* Priv RO, USR NO */
#define LPAE_ATTR_AP_PRIV_RO            (0x2 << 4)
/* Priv RW, USR NO */
#define LPAE_ATTR_AP_RW               	(0x0 << 4)
/* Priv RO, USR RO */
#define LPAE_ATTR_AP_RO                 (0x3 << 4)
/* Priv RW, USR RW */
#define LPAE_ATTR_AP_UNPRIV           	(0x1 << 4)
/* Mask for the above for changing */
#define LPAE_ATTR_AP_MASK             	(0x3 << 4)

#define LPAE_NS_ATTR                    (0x1 << 3)
#define LPAE_LOWER_ATTRS(x)             (((x) & 0xfff) << 2)

#define MAIR_ATTR_DEVICE            	(0x4)
#define MAIR_ATTR_IWBWA_OWBWA_NTR		(0xff)

#define MAIR_ATTR_DEVICE_INDEX              0x1
#define MAIR_ATTR_IWBWA_OWBWA_NTR_INDEX 	0x0

#if defined(TESTSUITE_RTOSck_UT) || defined(TESTSUITE_RTOSck_PT) || defined(TESTSUITE_RTOSck_IT)
#define MMU_PAE_FLAG_CACHE_USR_NO  ((LPAE_TYPE_BLOCK_DESC) | \
            LPAE_LOWER_ATTRS(LPAE_ATTR_ACCESS_FLAG) | \
            LPAE_LOWER_ATTRS(LPAE_ATTR_AP_UNPRIV) | \
            LPAE_LOWER_ATTRS(MAIR_ATTR_IWBWA_OWBWA_NTR_INDEX | \
            LPAE_ATTR_ISH))
#else
#define MMU_PAE_FLAG_CACHE_USR_NO  ((LPAE_TYPE_BLOCK_DESC) | \
            LPAE_LOWER_ATTRS(LPAE_ATTR_ACCESS_FLAG) | \
            LPAE_LOWER_ATTRS(LPAE_ATTR_AP_RW) | \
            LPAE_LOWER_ATTRS(MAIR_ATTR_IWBWA_OWBWA_NTR_INDEX | \
            LPAE_ATTR_ISH))
#endif

#define MMU_PAE_FLAG_TABLE         ((LPAE_TYPE_TABLE_DESC))

#define ACCESS_READ 0x0
#define ACCESS_WRITE 0x1


#endif // __SRE_ARM_MMU__
