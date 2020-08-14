

/*
 *  SDK Version
 * 
 */
#define SDK_VERSION_STRING	"HiSTBLinuxV100R002"


/*
 *  nand flash support
 *  #define CONFIG_NAND_SUPPORT
 */


/*
 *  spi flash support
 *  #define CONFIG_SPIFLASH_SUPPORT
 */


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
#define CFG_ENV_ADDR	0x100000


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
 * 
 */
#define CONFIG_PRODUCT_WITH_BOOT	y


/*
 *  build auximg from auxcode
 *  #define CONFIG_AUXCODE_COMPILE_SUPPORT
 */


/*
 *  FPGA
 *  #define CFG_HI3798MX_FPGA
 */


/*
 *  Simulate boot
 *  #define CONFIG_BOOT_SIMULATE
 */


/*
 *  Chip type
 * 
 */
#define CHIP_TYPE_hi3798mv100	1


/*
 *  Gpio Light config
 *  #define CONFIG_GPIO_LIGHT_SET
 */


/*
 *  AVS config
 * 
 */
#define CONFIG_AVS_SUPPORT	y


/*
 *  PMU Device config
 *  #define CONFIG_PMU_DEVICE
 */


/*
 *  USB Driver config
 * 
 */
#define CONFIG_USB_SUPPORT	y


/*
 *  Secure boot config
 *  #define CONFIG_TEE_SUPPORT
 */


/*
 *  ARM64 toolchains config
 * 
 */
#define CONFIG_ARM64_TOOLCHAINS	arm-histbv310-linux


/*
 *  ARM64 support config
 *  #define CONFIG_ARM64_SUPPORT
 */
