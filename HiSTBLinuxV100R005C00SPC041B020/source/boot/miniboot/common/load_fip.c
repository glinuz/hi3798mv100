
#include <config.h>
#include <boot.h>
#include <compile.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <run.h>
#include <module.h>
#include <mmu.h>
#include <image.h>
#include <asm/io.h>
#include <cpu-arch.h>
#include <libfdt.h>

#define SECURE		0x0
#define NON_SECURE	0x1

#define EP_EE_MASK      0x2
#define EP_EE_LITTLE    0x0
#define EP_EE_BIG       0x2
#define EP_GET_EE(x) (x & EP_EE_MASK)
#define EP_SET_EE(x, ee) ((x) = ((x) & ~EP_EE_MASK) | (ee))

/* CPSR/SPSR definitions */
#define DAIF_FIQ_BIT		(1 << 0)
#define DAIF_IRQ_BIT		(1 << 1)
#define DAIF_ABT_BIT		(1 << 2)
#define DAIF_DBG_BIT		(1 << 3)
#define SPSR_DAIF_SHIFT		6
#define SPSR_DAIF_MASK		0xf

#define SPSR_AIF_SHIFT          6
#define SPSR_AIF_MASK           0x7

#define SPSR_E_SHIFT            9
#define SPSR_E_MASK                     0x1
#define SPSR_E_LITTLE           0x0
#define SPSR_E_BIG                      0x1

#define SPSR_T_SHIFT            5
#define SPSR_T_MASK                     0x1
#define SPSR_T_ARM                      0x0
#define SPSR_T_THUMB            0x1

#define MODE_SP_SHIFT		0x0
#define MODE_SP_MASK		0x1
#define MODE_SP_EL0		0x0
#define MODE_SP_ELX		0x1

#define MODE_RW_SHIFT		0x4
#define MODE_RW_MASK		0x1
#define MODE_RW_64			0x0
#define MODE_RW_32			0x1

#define MODE_EL_SHIFT		0x2
#define MODE_EL_MASK		0x3
#define MODE_EL3		0x3
#define MODE_EL2		0x2
#define MODE_EL1		0x1
#define MODE_EL0		0x0

#define MODE32_SHIFT            0
#define MODE32_MASK             0xf
#define MODE32_usr              0x0
#define MODE32_fiq              0x1
#define MODE32_irq              0x2
#define MODE32_svc              0x3
#define MODE32_mon              0x6
#define MODE32_abt              0x7
#define MODE32_hyp              0xa
#define MODE32_und              0xb
#define MODE32_sys              0xf

#define DISABLE_ALL_EXCEPTIONS \
		(DAIF_FIQ_BIT | DAIF_IRQ_BIT | DAIF_ABT_BIT | DAIF_DBG_BIT)

#define PARAM_EP_SECURITY_MASK    0x1
#define GET_SECURITY_STATE(x) (x & PARAM_EP_SECURITY_MASK)
#define SET_SECURITY_STATE(x, security) \
			((x) = ((x) & ~PARAM_EP_SECURITY_MASK) | (security))

#define SPSR_64(el, sp, daif)				\
	(MODE_RW_64 << MODE_RW_SHIFT |			\
	((el) & MODE_EL_MASK) << MODE_EL_SHIFT |	\
	((sp) & MODE_SP_MASK) << MODE_SP_SHIFT |	\
	((daif) & SPSR_DAIF_MASK) << SPSR_DAIF_SHIFT)

#define SPSR_MODE32(mode, isa, endian, aif)		\
	(MODE_RW_32 << MODE_RW_SHIFT |			\
	((mode) & MODE32_MASK) << MODE32_SHIFT |	\
	((isa) & SPSR_T_MASK) << SPSR_T_SHIFT |		\
	((endian) & SPSR_E_MASK) << SPSR_E_SHIFT |	\
	((aif) & SPSR_AIF_MASK) << SPSR_AIF_SHIFT)


/* Length of a node address (an IEEE 802 address). */
#define	_UUID_NODE_LEN		6

/*
 * See also:
 *      http://www.opengroup.org/dce/info/draft-leach-uuids-guids-01.txt
 *      http://www.opengroup.org/onlinepubs/009629399/apdxa.htm
 *
 * A DCE 1.1 compatible source representation of UUIDs.
 */
struct uuid {
	uint32_t	time_low;
	uint16_t	time_mid;
	uint16_t	time_hi_and_version;
	uint8_t		clock_seq_hi_and_reserved;
	uint8_t		clock_seq_low;
	uint8_t		node[_UUID_NODE_LEN];
};

/* XXX namespace pollution? */
typedef struct uuid uuid_t;

#define FIP_MAX_FILES			10

/* TODO: Update this number as required */
#define TOC_HEADER_SERIAL_NUMBER	0x12345678

#define FLAG_FILENAME			(1 << 0)

typedef struct entry_lookup_list {
	const char		*name;
	uuid_t			 name_uuid;
	const char		*command_line_name;
	struct file_info	*info;
	unsigned int		 flags;
} entry_lookup_list_t;

typedef struct file_info {
	uuid_t			 name_uuid;
	const char		*filename;
	unsigned int		 size;
	void			*image_buffer;
	entry_lookup_list_t	*entry;
} file_info_t;


/* This is used as a signature to validate the blob header */
#define TOC_HEADER_NAME	0xAA640001


/* ToC Entry UUIDs */
#define UUID_TRUSTED_BOOT_FIRMWARE_BL2 \
	{0x0becf95f, 0x224d, 0x4d3e, 0xa5, 0x44, {0xc3, 0x9d, 0x81, 0xc7, 0x3f, 0x0a} }
#define UUID_SCP_FIRMWARE_BL30 \
	{0x3dfd6697, 0xbe89, 0x49e8, 0xae, 0x5d, {0x78, 0xa1, 0x40, 0x60, 0x82, 0x13} }
#define UUID_EL3_RUNTIME_FIRMWARE_BL31 \
	{0x6d08d447, 0xfe4c, 0x4698, 0x9b, 0x95, {0x29, 0x50, 0xcb, 0xbd, 0x5a, 0x00} }
#define UUID_SECURE_PAYLOAD_BL32 \
	{0x89e1d005, 0xdc53, 0x4713, 0x8d, 0x2b, {0x50, 0x0a, 0x4b, 0x7a, 0x3e, 0x38} }
#define UUID_NON_TRUSTED_FIRMWARE_BL33 \
	{0xa7eed0d6, 0xeafc, 0x4bd5, 0x97, 0x82, {0x99, 0x34, 0xf2, 0x34, 0xb6, 0xe4} }

typedef struct fip_toc_header {
	uint32_t	name;
	uint32_t	serial_number;
	uint64_t	flags;
} fip_toc_header_t;

typedef struct fip_toc_entry {
	uuid_t		uuid;
	uint64_t	offset_address;
	uint64_t	size;
	uint64_t	flags;
} fip_toc_entry_t;

typedef struct aapcs64_params {
	uint64_t arg0;
	uint64_t arg1;
	uint64_t arg2;
	uint64_t arg3;
	uint64_t arg4;
	uint64_t arg5;
	uint64_t arg6;
	uint64_t arg7;
} aapcs64_params_t;

/***************************************************************************
 * This structure provides version information and the size of the
 * structure, attributes for the structure it represents
 ***************************************************************************/
typedef struct param_header {
	uint8_t type;		/* type of the structure */
	uint8_t version;    /* version of this structure */
	uint16_t size;      /* size of this structure in bytes */
	uint32_t attr;      /* attributes: unused bits SBZ */
} param_header_t;

/*****************************************************************************
 * This structure represents the superset of information needed while
 * switching exception levels. The only two mechanisms to do so are
 * ERET & SMC. Security state is indicated using bit zero of header
 * attribute
 * NOTE: BL1 expects entrypoint followed by spsr while processing
 * SMC to jump to BL31 from the start of entry_point_info
 *****************************************************************************/
typedef struct entry_point_info {
	param_header_t h;
	uint64_t pc;
	uint32_t spsr;
	aapcs64_params_t args;
} entry_point_info_t;

/*****************************************************************************
 * Image info binary provides information from the image loader that
 * can be used by the firmware to manage available trusted RAM.
 * More advanced firmware image formats can provide additional
 * information that enables optimization or greater flexibility in the
 * common firmware code
 *****************************************************************************/
typedef struct atf_image_info {
	param_header_t h;
	uint64_t image_base;   /* physical address of base of image */
	uint32_t image_size;    /* bytes read from image file */
	uint32_t copied_size;	/* image size copied in blocks */
} atf_image_info_t;

/*******************************************************************************
 * This structure represents the superset of information that can be passed to
 * BL31 e.g. while passing control to it from BL2. The BL32 parameters will be
 * populated only if BL2 detects its presence. A pointer to a structure of this
 * type should be passed in X0 to BL31's cold boot entrypoint.
 *
 * Use of this structure and the X0 parameter is not mandatory: the BL31
 * platform code can use other mechanisms to provide the necessary information
 * about BL32 and BL33 to the common and SPD code.
 *
 * BL31 image information is mandatory if this structure is used. If either of
 * the optional BL32 and BL33 image information is not provided, this is
 * indicated by the respective image_info pointers being zero.
 ******************************************************************************/
typedef struct bl31_params {
	param_header_t h;
	uint64_t bl31_image_info;//atf_image_info_t *bl31_image_info;
	uint64_t bl32_ep_info;//entry_point_info_t *bl32_ep_info;
	uint64_t bl32_image_info;//atf_image_info_t *bl32_image_info;
	uint64_t bl33_ep_info;//entry_point_info_t *bl33_ep_info;
	uint64_t bl33_image_info;//atf_image_info_t *bl33_image_info;
} bl31_params_t;

file_info_t files[FIP_MAX_FILES];
unsigned file_info_count = 0;
uuid_t uuid_null = {0};
uuid_t uuid_bl33 = UUID_NON_TRUSTED_FIRMWARE_BL33;
uuid_t uuid_bl32 = UUID_SECURE_PAYLOAD_BL32;
uuid_t uuid_bl31 = UUID_EL3_RUNTIME_FIRMWARE_BL31;

#ifdef CONFIG_TEE_SUPPORT
#if defined(CONFIG_TEE_MEM_LAYOUT_2G)
#define BL31_BASE                       0x7ca00000
#elif defined(CONFIG_TEE_MEM_LAYOUT_1G)
#define BL31_BASE                       0x3ca00000
#elif defined(CONFIG_TEE_MEM_LAYOUT_1_5G)
#define BL31_BASE                       0x5ca00000
#elif defined(CONFIG_TEE_MEM_LAYOUT_512M)
#define BL31_BASE                       0x1ca00000
#endif
#else
#define BL31_BASE                       0x4400000
#endif

#define BL31_SIZE                       0x200000

/*
 * TODO: Add ability to specify and flag different file types.
 * Add flags to the toc_entry?
 * const char* format_type_str[] = { "RAW", "ELF", "PIC" };
 */

/* The images used depends on the platform. */
static entry_lookup_list_t toc_entry_lookup_list[] = {
	{ "Trusted Boot Firmware BL2", UUID_TRUSTED_BOOT_FIRMWARE_BL2,
	  "bl2", NULL, FLAG_FILENAME },
	{ "SCP Firmware BL3-0", UUID_SCP_FIRMWARE_BL30,
	  "bl30", NULL, FLAG_FILENAME},
	{ "EL3 Runtime Firmware BL3-1", UUID_EL3_RUNTIME_FIRMWARE_BL31,
	  "bl31", NULL, FLAG_FILENAME},
	{ "Secure Payload BL3-2 (Trusted OS)", UUID_SECURE_PAYLOAD_BL32,
	  "bl32", NULL, FLAG_FILENAME},
	{ "Non-Trusted Firmware BL3-3", UUID_NON_TRUSTED_FIRMWARE_BL33,
	  "bl33", NULL, FLAG_FILENAME},
	{ NULL, {0}, 0 }
};

extern int fdt_check_header(const void *fdt);
extern int do_load_secure_os(unsigned long addr, unsigned long org_offset,
			     unsigned long img_dst, int run,
			     uint32_t *rtos_load_addr);
/* Return 0 for equal uuids */
static inline int compare_uuids(const uuid_t *uuid1, const uuid_t *uuid2)
{
	return memcmp(uuid1, uuid2, sizeof(uuid_t));
}


static inline void copy_uuid(uuid_t *to_uuid, const uuid_t *from_uuid)
{
	memcpy(to_uuid, from_uuid, sizeof(uuid_t));
}

static entry_lookup_list_t *get_entry_lookup_from_uuid(const uuid_t *uuid)
{
	unsigned int lookup_index = 0;

	while (toc_entry_lookup_list[lookup_index].command_line_name != NULL) {
		if (compare_uuids(&toc_entry_lookup_list[lookup_index].name_uuid,
		    uuid) == 0) {
			return &toc_entry_lookup_list[lookup_index];
		}
		lookup_index++;
	}
	return NULL;
}

#if 0
static file_info_t *find_file_info_from_uuid(const uuid_t *uuid)
{
	int index;

	for (index = 0; index < file_info_count; index++) {
		if (compare_uuids(&files[index].name_uuid, uuid) == 0) {
			return &files[index];
		}
	}
	return NULL;
}
#endif

static void dump_toc(void)
{
	unsigned int index = 0;
	unsigned int image_offset;
	unsigned int image_size = 0;

	image_offset = sizeof(fip_toc_header_t) +
		(sizeof(fip_toc_entry_t) * (file_info_count + 1));

	printf("Firmware Image Package ToC:\n");
	printf("---------------------------\n");
	for (index = 0; index < file_info_count; index++) {
		if (files[index].entry) {
			printf("- %s: ", files[index].entry->name);
		} else {
			printf("- Unknown entry: ");
		}
		image_size = files[index].size;

		printf("offset=0x%X, size=0x%X\n", image_offset, image_size);
		image_offset += image_size;

		if (files[index].filename) {
			printf("  file: '%s'\n", files[index].filename);
		}
	}
	printf("---------------------------\n");
}

/* Read and load existing package into memory. */
static int parse_fip(char *fip_buffer)
{
	fip_toc_header_t *toc_header;
	fip_toc_entry_t *toc_entry;
	int found_last_toc_entry = 0;
	file_info_t *file_info_entry;
	int ret = -1;

	if (fip_buffer == NULL) {
		printf("ERROR: Invalid fip buffer addr.\n");
		ret = -EINVAL;
		goto parse_fip_error;
	}

	/* Set the ToC Header at the base of the buffer */
	toc_header = (fip_toc_header_t *)fip_buffer;
	/* The first toc entry should be just after the ToC header */
	toc_entry = (fip_toc_entry_t *)(toc_header + 1);

	/* While the ToC entry is contained into the buffer */
	int cnt = 0;
	while (cnt < FIP_MAX_FILES) {
		/* Check if the ToC Entry is the last one */
		if (compare_uuids(&toc_entry->uuid, &uuid_null) == 0) {
			found_last_toc_entry = 1;
			ret = 0;
			break;
		}

		/* Add the entry into file_info */

		/* Get the new entry in the array and clear it */
		file_info_entry = &files[file_info_count++];
		memset(file_info_entry, 0, sizeof(file_info_t));

		/* Copy the info from the ToC entry */
		copy_uuid(&file_info_entry->name_uuid, &toc_entry->uuid);
		file_info_entry->image_buffer = fip_buffer +
		  toc_entry->offset_address;
		file_info_entry->size = toc_entry->size;

		/* Check if there is a corresponding entry in lookup table */
		file_info_entry->entry =
		  get_entry_lookup_from_uuid(&toc_entry->uuid);

		/* Go to the next ToC entry */
		toc_entry++;
		cnt++;
	}

	if (!found_last_toc_entry) {
		printf("ERROR: Given FIP does not have an end ToC entry.\n");
		ret = -EINVAL;
		goto parse_fip_error;
	}

 parse_fip_error:
	return ret;
}

int is_fip(char *buf)
{
	fip_toc_header_t *header = (fip_toc_header_t *)buf;

	if (NULL == buf) {
		printf("Invalid fip buffer address\n");
		return 0;
	}

	if ((header->name == TOC_HEADER_NAME) && (header->serial_number != 0)) {
		return 1;
	} else {
		return 0;
	}
}
entry_point_info_t bl32_ep_info;
entry_point_info_t bl33_ep_info;
bl31_params_t bl31_params;

int load_fip(char *buf)
{
	int ret = 0;
	unsigned int index = 0;
	entry_point_info_t *bl32_ep = &bl32_ep_info;
	entry_point_info_t *bl33_ep = &bl33_ep_info;
	bl31_params_t *bl31_p = &bl31_params;
	uint32_t bl31_pc = 0;
	unsigned int bl31_size = 0;
	char *bl31_img_buf = NULL;
	u32 val;

	memset(bl32_ep, 0, sizeof(entry_point_info_t));
	memset(bl33_ep, 0, sizeof(entry_point_info_t));
	memset(bl31_p, 0, sizeof(bl31_params_t));
	bl31_p->bl32_ep_info = (uint64_t)((uint32_t)bl32_ep);
	bl31_p->bl33_ep_info = (uint64_t)((uint32_t)bl33_ep);

	printf("Load fip from 0x%X ...\n", (uint32_t)buf);
	if (NULL == buf) {
		printf("Invalid fip buffer address\n");
		ret = -EINVAL;
		goto error;
	}
	ret = parse_fip(buf);
	if (ret)
		goto error;

	dump_toc();

	printf("Create Entry Point info ...\n");
	for (index = 0; index < file_info_count; index++) {
		if (!files[index].entry) {
			printf("- Invalid entry: ");
			continue;
		}

		if (compare_uuids(&files[index].name_uuid, &uuid_bl31) == 0) {
			printf("Get - %s \n", files[index].entry->name);
			bl31_pc = BL31_BASE;//(uint32_t)(files[index].image_buffer);
			bl31_img_buf = files[index].image_buffer;
			bl31_size = files[index].size;
		}
#if 0 //def CONFIG_TEE_SUPPORT
		if (compare_uuids(&files[index].name_uuid, &uuid_bl32) == 0) {
			uint32_t rtos_addr;
			printf("Get - %s \n", files[index].entry->name);
			ret = do_load_secure_os((u32)(files[index].image_buffer), 0, 0x7E700000, 0, &rtos_addr);
			if (ret)
				goto error;
			printf("rtos_addr = 0x%X\n", rtos_addr);
			bl32_ep->pc = (uint64_t)rtos_addr;
			SET_SECURITY_STATE(bl32_ep->h.attr,
					 SECURE);
			bl32_ep->spsr = 0;
		}
#endif
		if (compare_uuids(&files[index].name_uuid, &uuid_bl33) == 0) {
			printf("Get - %s \n", files[index].entry->name);
			image_header_t *hdr = (image_header_t *)(files[index].image_buffer);
			if (!(check_image_arch ((image_header_t *)(files[index].image_buffer), IH_ARCH_ARM64))) {
				char *fdt = (char *)get_image_end(hdr);
				ret = fdt_check_header(fdt);
				if (ret) {
					printf("Invalid FDT at 0x%p, hdr at 0x%p\n", fdt, hdr);
					goto error;
				}
				bl33_ep->pc = (uint64_t)get_image_data(hdr);
				bl33_ep->args.arg0 = (uint64_t)((uint32_t)fdt);
				bl33_ep->args.arg1 = (uint64_t)(get_image_ep(hdr));
				bl33_ep->args.arg2 = (uint64_t)(files[index].size - sizeof(image_header_t));
				bl33_ep->args.arg3 = CONFIG_TAG_OFFSET;
				bl33_ep->args.arg4 = CONFIG_TAG_OFFSET + CONFIG_TAG_MAX_SIZE;
				bl33_ep->args.arg5 = (uint64_t)(CONFIG_DTB_MAX_SIZE);
				bl33_ep->spsr = SPSR_64(MODE_EL1,
				MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);
			} else {
				bl33_ep->pc = (uint64_t)get_image_data(files[index].image_buffer);
				bl33_ep->args.arg0 = 0; //no dtb
				bl33_ep->spsr = SPSR_MODE32(MODE32_svc, 0x0 , EP_EE_LITTLE, DISABLE_ALL_EXCEPTIONS);
			}
			printf("bl33_ep->spsr = 0x%X\n", bl33_ep->spsr);
		}
	}

	if (!bl33_ep->pc) {
		printf("Invalid fip image, no kernel found!\n");
		ret = -EINVAL;
		goto error;
	}

	if (!bl31_pc) {
		printf("No ATF found!\n");
		ret = -EINVAL;
		goto error;
	}

#ifdef CONFIG_NET
	extern void setup_eth_param(void);
	setup_eth_param();
#endif
	get_kern_tags(0);


#ifdef CONFIG_TEE_SUPPORT
	extern u32 secure_entry_addr;
	extern u32 _text_end;
	if (secure_entry_addr) {
		unsigned int text_end = roundup(_text_end, 0x10); /* align for secure os clean boot */
		bl32_ep->pc = (uint64_t)secure_entry_addr;
		SET_SECURITY_STATE(bl32_ep->h.attr,
				 SECURE);
		bl32_ep->spsr = 0;
		bl32_ep->args.arg0 = TEXT_BASE;
		bl32_ep->args.arg1 = text_end;
	}
#endif

	printf("Move bl31 img from 0x%p to 0x%X, %d Bytes\n", bl31_img_buf, bl31_pc, bl31_size);
	memmove((void *)bl31_pc, bl31_img_buf, bl31_size);
	memset(bl31_img_buf, 0, bl31_size);
	writel(bl31_pc, REG_PERI_CPU_RVBARADDR);
	writel(readl(REG_PERI_CPU_AARCH_MODE) | 0xF,REG_PERI_CPU_AARCH_MODE);

	(*(volatile uint64_t *)0) = (uint64_t)((uint32_t)bl31_p);
	(*(volatile uint64_t *)8) = (uint64_t)((uint32_t)NULL);

	printf("Start Warm Reseting\n");

	module_exit();
	cpu_disable_irq();

	mmu_cache_disable();
	cache_flush_all();

	/* warm reseting */
	__asm__ __volatile__("isb\n\r"
		"dsb\n\r"
		"mrc p15, 0, %0, c12, c0, 2\n\r"
		"orr %0, %0, #0x3\n\r"
		"mcr p15, 0, %0, c12, c0, 2\n\r"
		"isb\n\r"
		"wfi":"=r"(val)::"cc");
	printf("Fail to warm resetting...\n");

	while (1)
		;

error:
	return ret;
}
