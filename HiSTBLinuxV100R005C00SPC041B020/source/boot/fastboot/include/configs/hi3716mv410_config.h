

/*
 *  SDK Version
 * 
 */
#define SDK_VERSION_STRING	"HiSTBLinuxV100R002"


/*
 *  nand flash support
 *  #define CONFIG_NAND_SUPPORT
 */
#define CONFIG_NAND_SUPPORT


/*
 *  spi flash support
 *  #define CONFIG_SPIFLASH_SUPPORT
 */
#define CONFIG_SPIFLASH_SUPPORT	y


/*
 *  eth support
 * 
 */
#define CONFIG_NET_SUPPORT	y


/*
 *  eth upport phy address
 *  #define HISFV_PHY_U
 */


/*
 *  eth downport phy address
 *  #define HISFV_PHY_D
 */


/*
 *  emmc flash support
 * 
 */
#define CONFIG_EMMC_SUPPORT	y


/*
 *  boot environment variables start address
 * 
 */
#define CFG_ENV_ADDR	0x80000


/*
 *  boot environment variables size
 * 
 */
#define CFG_ENV_SIZE	0x10000


/*
 *  boot environment variables range
 * 
 */
#define CFG_ENV_RANGE	0x10000


/*
 *  Bakup boot environment variables support
 *  #define CONFIG_ENV_BACKUP
 */


/*
 *  Bakup boot environment variables start address
 *  #define CFG_ENV_BACKUP_ADDR
 */


/*
 *  build product with boot
 *  #define CONFIG_PRODUCT_WITH_BOOT
 */


/*
 *  platform hi3712
 *  #define CONFIG_HI3712_PLATFORM
 */


/*
 *  platform hi3716c/h/m
 *  #define CONFIG_HI3716X_PLATFORM
 */


/*
 *  platform hi3715
 *  #define CONFIG_HI3715_PLATFORM
 */


/*
 *  Hi3716cv200/Hi3716cv200es FPGA
 * 
 */
#define CFG_HI3716MV410_FPGA	0


/*
 *  Simulate boot
 *  #define CONFIG_BOOT_SIMULATE
 */


/*
 *  Chip type
 * 
 */
#define CHIP_TYPE_hi3716mv410	1


/*
 *  AVS config
 *  #define CONFIG_AVS_SUPPORT
 */


/*
 *  PMU Device config
 *  #define CONFIG_PMU_DEVICE
 */


/*
 *  USB Driver config
 *  #define CONFIG_USB_SUPPORT
 */
