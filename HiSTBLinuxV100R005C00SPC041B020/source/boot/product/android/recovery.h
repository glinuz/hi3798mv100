#ifndef __HI_CUSTOMER_H__
#define __HI_CUSTOMER_H__

/*******************do not change*********************************/
#ifndef SZ_1MB
#define SZ_1MB	(1024*1024)
#endif

#ifndef SZ_1KB
#define SZ_1KB	(1024)
#endif

//the  boot-recovery, boot-sellet locattion on eMMC
#define MISC_EMMC_WRITE_LABEL_COMMAND_PAGE  (3)
#define MISC_EMMC_COMMAND_PAGE              (0)

//the  boot-recovery, boot-sellet locattion on Nand
#define MISC_PAGES			                (2)
#define MISC_COMMAND_PAGE		            (1)

#define MAX_KEY_PRESS_COUNT					(500)


/*for front keyboad default for ct1642 */
/* s2=0x00,s3=0x01,s6=0x02,s7=0x03 */
/* s8=0x07,s1=0x06,s4=0x05,s5=0x04 */
#define RECOVERY_KEY                        0x01

#endif
