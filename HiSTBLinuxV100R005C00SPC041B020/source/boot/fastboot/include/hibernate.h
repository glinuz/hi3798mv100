#ifndef HIBERNATE_H
#define HIBERNATE_H

#define HIBERNATE_HEADER_ID          0x00
#define HIBERNATE_HEADER_COPY_SIZE   0x04
#define HIBERNATE_HEADER_DRV_SIZE    0x08
#define HIBERNATE_HEADER_VERSION     0x0c
#define HIBERNATE_HEADER_SNAPSHOT    0x20
#define HIBERNATE_HEADER_HIBERNATE   0x28
#define HIBERNATE_HEADER_SWITCH      0x30

#define HIBERNATE_ID_DRIVER          0x44483457      /* W4HD */
#define HIBERNATE_ID_BOOTFLAG        0x46423457      /* W4BF */

#define HIBERNATE_PART_SHIFT         0
#define HIBERNATE_LUN_SHIFT          8
#define HIBERNATE_DEV_SHIFT          16

#define HIBERNATE_PART_MASK          (0xff << HIBERNATE_PART_SHIFT)
#define HIBERNATE_LUN_MASK           (0xff << HIBERNATE_LUN_SHIFT)
#define HIBERNATE_DEV_MASK           (0xff << HIBERNATE_DEV_SHIFT)

#define HIBERNATE_DEV_NOR            (0x01 << HIBERNATE_DEV_SHIFT)
#define HIBERNATE_DEV_NAND           (0x02 << HIBERNATE_DEV_SHIFT)
#define HIBERNATE_DEV_ATA            (0x03 << HIBERNATE_DEV_SHIFT)
#define HIBERNATE_DEV_SD             (0x04 << HIBERNATE_DEV_SHIFT)
#define HIBERNATE_DEV_MEM            (0x05 << HIBERNATE_DEV_SHIFT)
#define HIBERNATE_DEV_USER           (0x7e << HIBERNATE_DEV_SHIFT)
#define HIBERNATE_DEV_EXT            (0x7f << HIBERNATE_DEV_SHIFT)

#define HIBERNATE_DEV(dev, lun, part)        (HIBERNATE_DEV_##dev | \
                                         ((lun) << HIBERNATE_LUN_SHIFT) | \
                                         ((part) << HIBERNATE_PART_SHIFT))

#ifndef HIBERNATE_LUN_CONV
#define HIBERNATE_LUN_CONV(dev)      (dev)
#endif

#define HIBERNATE_DEV_TO_LUN(dev)    HIBERNATE_LUN_CONV(((dev) & HIBERNATE_LUN_MASK) >> \
                                              HIBERNATE_LUN_SHIFT)
#define HIBERNATE_DEV_TO_PART(dev)   (((dev) & HIBERNATE_PART_MASK) >> HIBERNATE_PART_SHIFT)

#define HIBERNATE_BF_LEN             0x100

#define HIBERNATE_SAVEAREA_NUM       (sizeof(hibernate_savearea) / \
                                 sizeof(struct hibernate_savearea))

#define HIBERNATE_DRV_PRELOAD        4096

struct hibernate_savearea {
    unsigned int bootflag_dev;
    unsigned int bootflag_area;
    unsigned int bootflag_size;
};

extern struct hibernate_savearea hibernate_savearea[];

extern int hibernate_saveno;

extern void *hibernate_bfaddr;

int hibernate_drvload(void);
int hibernate_checkboot(int saveno);
int hibernate_boot(int saveno);

#endif
