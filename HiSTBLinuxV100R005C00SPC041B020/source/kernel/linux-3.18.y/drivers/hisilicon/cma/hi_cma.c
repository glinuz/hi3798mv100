#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/dma-contiguous.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/cma.h>

#define NAME_LEN_MAX   64
#define ZONE_MAX       64

struct cma_zone {
	struct device pdev;
	char name[NAME_LEN_MAX];
	u32 gfp;
	u32 phys_start;
	u32 nbytes;
	u32 alloc_type;
	u32 block_align;
};

static u32 num_zones;
static struct cma_zone hisi_zone[ZONE_MAX];

static int use_bootargs;

unsigned int get_cma_size(void)
{
	int i;
	u64 total = 0;

	for (i = 0; i < num_zones; i++)
		total += hisi_zone[i].nbytes;

	/* unit is M */
	return (unsigned int)(total >> 20);
}

static int __init hisi_mmz_parse_cmdline(char *s)
{
	char *line, *tmp;
	char tmpline[256];

	if (NULL == s) {
		pr_info("There is no cma zone!\n");
		return 0;
	}
	strncpy(tmpline, s, sizeof(tmpline));
	tmpline[sizeof(tmpline)-1] = '\0';
	tmp = tmpline;

	while ((line = strsep(&tmp, ":")) != NULL) {
		int i;
		char *argv[6];

		for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;)
			if (++i == ARRAY_SIZE(argv))
				break;

		hisi_zone[num_zones].pdev.coherent_dma_mask = DMA_BIT_MASK(64);
		if (i == 4) {
			strlcpy(hisi_zone[num_zones].name, argv[0], NAME_LEN_MAX);
			hisi_zone[num_zones].gfp = memparse(argv[2], NULL);
			hisi_zone[num_zones].phys_start = memparse(argv[2], NULL);
			hisi_zone[num_zones].nbytes = memparse(argv[3], NULL);
		}

		else if (i == 6) {
			strlcpy(hisi_zone[num_zones].name, argv[0], NAME_LEN_MAX);
			hisi_zone[num_zones].gfp = memparse(argv[1], NULL);
			hisi_zone[num_zones].phys_start = memparse(argv[2], NULL);
			hisi_zone[num_zones].nbytes = memparse(argv[3], NULL);
			hisi_zone[num_zones].alloc_type = memparse(argv[4], NULL);
			hisi_zone[num_zones].block_align = memparse(argv[5], NULL);
		} else {
			pr_err("hisi ion parameter is not correct\n");
			continue;
		}

		num_zones++;
	}
	if (num_zones != 0)
		use_bootargs = 1;

	return 0;
}
early_param("mmz", hisi_mmz_parse_cmdline);

struct cma_zone *hisi_get_cma_zone(const char *name)
{
	int i = 0;

	for (i = 0; i < num_zones; i++)
		if (strcmp(hisi_zone[i].name, name) == 0)
			break;

	if (i == num_zones)
		return NULL;

	return &hisi_zone[i];
}
EXPORT_SYMBOL(hisi_get_cma_zone);

struct device *hisi_get_cma_device(const char *name)
{
	int i = 0;

	for (i = 0; i < num_zones; i++)
		if (strcmp(hisi_zone[i].name, name) == 0)
			break;

	if (i == num_zones)
		return NULL;

	return &hisi_zone[i].pdev;
}
EXPORT_SYMBOL(hisi_get_cma_device);

int hisi_declare_heap_memory(void)
{
	int i;
	int ret = 0;

	if (use_bootargs == 0) {
		pr_info("cmz zone is not set!\n");
		return ret;
	}

	for (i = 0; i < num_zones; i++) {
		ret = dma_declare_contiguous(&hisi_zone[i].pdev,
			hisi_zone[i].nbytes, hisi_zone[i].phys_start, 0);
		if (ret)
			panic("declare cma zone %s base: %u size:%uMB failed. ret:%d",
			      hisi_zone[i].name, hisi_zone[i].phys_start,
			      hisi_zone[i].nbytes>>20, ret);
		hisi_zone[i].phys_start = cma_get_base(hisi_zone[i].pdev.cma_area);
		hisi_zone[i].nbytes = cma_get_size(hisi_zone[i].pdev.cma_area);

		/*FIXME need to fix dma_declare_contiguous return value &&value type*/
	}
	return ret;
}
