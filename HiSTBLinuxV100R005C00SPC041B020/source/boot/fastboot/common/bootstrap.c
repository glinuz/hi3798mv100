#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <malloc.h>
#include <boot/customer.h>
#include <environment.h>
#include <usb_if.h>

#define dump_buf(buf, len) do{\
	int i;\
	char *p = (void*)(buf);\
	printf("%s->%d, buf=0x%.8x, len=%d\n", \
			__FUNCTION__, __LINE__, \
			(int)(buf), (int)(len)); \
	for(i=0;i<(len);i++){\
		printf("0x%.2x ", *(p+i));\
		if( !((i+1) & 0x07) )\
		printf("\n");\
	}\
	printf("\n");\
}while(0)

#define	XHEAD	0xAB
#define	XCMD	0xCD
#define	ACK     0xAA            /* ACK VALUE */
#define	NAK     0x55            /* NAK VALUE */
#define	BOOTUP  0x20            /* secure CA download */

#define START_FRAME_LEN           5
#define MAX_BUFF_SIZE             1024

/* references to names in env_common.c */
DECLARE_GLOBAL_DATA_PTR;

extern env_t *env_ptr;

extern int dwc_dev_init(void);
extern void dwc_intr_poll(void);

long long get_chipid(void);

static char recv_buf[MAX_BUFF_SIZE];

static unsigned short crc16_table[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

static unsigned short calc_crc16(unsigned char *packet, unsigned long length)
{
	unsigned short crc16 = 0;
	unsigned long i;

	for (i = 0; i < length; i++) {
		crc16 = ((crc16<<8)|packet[i])^crc16_table[(crc16>>8)&0xFF];
	}

	for (i = 0; i < 2; i++){
		crc16 = ((crc16<<8)|0)^crc16_table[(crc16>>8)&0xFF];
	}

	return crc16;
}

static int recv_byte(void)
{
	if(serial_tstc()){
		return serial_getc();
	}

	return -1;
}

static char recv_data(void)
{
	int ret = -1;

	while(ret == -1)
		ret = recv_byte();

	return (char)ret;
}

void download_process(void)
{
	int i = 0, cr = 0, ret = -1;
	unsigned int head_frame_len = 0;
	unsigned int cmd_len = 0;
	unsigned char send_buf[20] = {0};
	unsigned short cksum;

	do {
		do {
			do {
				cr = recv_byte();
			} while (cr != XHEAD);

			head_frame_len = START_FRAME_LEN;
			recv_buf[0] = (char)cr;

			/* RECV: head frame */
			for (i = 0; i < (head_frame_len-1) && i < (sizeof(recv_buf) - 1); i++) {
				recv_buf[i+1] = recv_data();
			}

			/* crc check */
			cksum = calc_crc16((unsigned char*)recv_buf,3);
			if (cksum == ((recv_buf[3] << 8) | recv_buf[4])) {
				/* init */
				cmd_len = (unsigned short)((recv_buf[1] << 8) | recv_buf[2]) + 3;
				if (cmd_len >= sizeof(recv_buf))
					goto head_fail;

				/* SEND: ack */
				send_buf[0] = ACK;
				serial_putc(send_buf[0]);
				break;
			}

head_fail:
			/* init */
			cmd_len = 0;
			memset(recv_buf, 0, sizeof(recv_buf));

			/* SEND: nak */
			send_buf[0] = NAK;
			serial_putc(send_buf[0]);

		} while (1);

		do {
			do {
				cr = recv_byte();
			} while (cr != XCMD);

			recv_buf[0] = (char)cr;

			/* RECV: cmd data, cmd_len check in head flow */
			for (i = 0;i < (cmd_len - 1); i++) {
				recv_buf[i+1] = recv_data();
			}

			/* crc check */
			cksum = calc_crc16((unsigned char*)recv_buf, cmd_len - 2);
			if (cksum == ((recv_buf[cmd_len-2] << 8) | recv_buf[cmd_len-1])) {
				/* SEND: ack wait result */
				send_buf[0] = ACK;
				serial_putc(send_buf[0]);
				break;
			}

			memset(recv_buf, 0, sizeof(recv_buf));
			/* SEND: nak */
			send_buf[0] = NAK;
			serial_putc(send_buf[0]);
			//dump_buf(recv_buf, cmd_len);
		} while (1);

		/* clean crc */
		recv_buf[cmd_len-1] = 0;
		recv_buf[cmd_len-2] = 0;

		/* cmd process */
		ret = run_command((recv_buf+1), 0);
		if (ret < 0) {
			/* SEND: end flag */
			serial_puts("[EOT](ERROR)\n");
		} else{
			/* SEND: end flag */
			serial_puts("[EOT](OK)\n");
		}

	} while (1);
}

#ifdef CONFIG_USB_HOST_BOOTSTRAP
void bootargs_change(void)
{
	char *recovery_boot_args = NULL;

	if (getenv("recoverybootargs")) {
		recovery_boot_args = (char *)malloc(strlen(getenv("recoverybootargs")));
		if (recovery_boot_args) {
			strcpy(recovery_boot_args, getenv("recoverybootargs"));
			setenv("bootargs", recovery_boot_args);
			free(recovery_boot_args);
		} else {
			printf("malloc recovery_boot_args failed !\n");
		}
	}
}

int download_bootargs(void)
{
	int ret ;
	long filesize;

	/* step1: detect usb */
	usb_detect();

	/* step2: read bootargs.bin */
	ret = usb_file_check(CONFIG_FILE_BOOTARGS_NAME);
	if (ret)
		goto error;

	filesize = usb_file_read(CONFIG_FILE_BOOTARGS_NAME,
				 (void*)CONFIG_FILE_READ_ADDR,  0, CONFIG_FILE_MAX_SIZE);
	if(filesize <= 0)
		goto error;

	/* step2: set  bootargs  */
	memcpy (env_ptr, (void*)CONFIG_FILE_READ_ADDR, CONFIG_ENV_SIZE);

	/* flush cache after read */
	flush_cache((ulong)env_ptr, CONFIG_ENV_SIZE); /* FIXME */

	if (crc32(0, env_ptr->data, ENV_SIZE) != env_ptr->crc) {
		printf("\n bootargs crc failed !\n");
		goto error;
	}

	gd->env_valid = 1;

	return 0;

error:
	return 1;

}

int download_image(const char *imagename)
{
	int ret ;
	long filesize;
	char buf[20];

	if (!imagename) {
		printf("image name invalid!\n");
		return 1;
	}

	/* read recovery.img or apploader.bin and go */
	ret = usb_file_check(imagename);
	if (ret)
		goto error;

	filesize = usb_file_read(imagename,
				(void *)CONFIG_FILE_READ_ADDR,  0, CONFIG_FILE_MAX_SIZE);
	if(filesize <= 0)
		goto error;

	memset(buf, 0 ,20);
	snprintf(buf, 20, "bootm 0x%x", CONFIG_FILE_READ_ADDR);
#ifndef CONFIG_SYS_HUSH_PARSER
	run_command (buf, 0);
#else
	parse_string_outer(buf, FLAG_PARSE_SEMICOLON |
	FLAG_EXIT_FROM_LOOP);
#endif

	return 0;

error:
	return 1;

}
int download_process_usbhost(void)
{
	int ret ;

	/* setp1: android update need to replace bootargs from recoverybootargs */
	bootargs_change();

	/* setp2: read recovery.img and go */
	ret = download_image(CONFIG_FILE_RECOVERY_NAME);
	if (ret)
		goto error;

	return 0;

error:
	return 1;

}
#endif


void download_boot_normal(int (*handle)(void))
{
#ifdef CONFIG_USB_BOOTSTRAP
	unsigned int bootstraptype = 0;
#endif /* CONFIG_USB_BOOTSTRAP */

	if (CONFIG_START_MAGIC == (*(volatile unsigned int *)(REG_START_FLAG))){
		/* clear flag */
		*(volatile unsigned int *)(REG_START_FLAG) = 0;
		serial_puts("start download process.\n");

#ifdef CONFIG_USB_BOOTSTRAP
		bootstraptype = *(volatile unsigned int *)(REG_USB_UART_FLAG);
		switch (bootstraptype) {
		case SELF_BOOT_TYPE_UART:
			for (;;) {
				download_process();
			}
			break;
#ifdef CONFIG_USB_DEVICE_BOOTSTRAP
		case SELF_BOOT_TYPE_USBDEV:
			dwc_dev_init();
			for (;;) {
				dwc_intr_poll();
			}
			break;
#endif

#ifdef CONFIG_USB_HOST_BOOTSTRAP
		case SELF_BOOT_TYPE_USBHOST:
			break;
#endif

		default:
			printf("not supported bootstrap type: %d\n", bootstraptype);
			break;
		}
#else
		/* wait cmd from pc */
		for (;;) {
			download_process();
		}

#endif
	}
	if (handle)
		handle();
}

void download_boot_ca(int (*handle)(void))
{
	int i = 0, j = 0, cr = 0;
	unsigned char send_buf[20] = {0};

#ifdef CONFIG_USB_BOOTSTRAP
	if (SELF_BOOT_TYPE_UART != (*(volatile unsigned int *)(REG_USB_UART_FLAG))) {
		return;
	}
#endif

	for (i=0; i < 5;i++) {
		/* SEND: CA to PC, wait PC ACK */
		send_buf[0] = BOOTUP;
		for (j = 0; j < 5; j++){
			serial_putc(send_buf[0]);
		}

		for (j = 0; j < 100; j++){
			cr = recv_byte();
			if (cr == ACK){
				goto down_load;
			}
			udelay(1000);
		}
		if(i == 4){
			serial_puts("\n");
			return;
		}

	}

down_load:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	serial_puts("start download process.\n");
#endif
	/* wait cmd from pc */
	for (;;) {
		download_process();
	}

	if (handle)
		handle();
}

extern int get_ca_type(void);
void download_boot(int (*handle)(void))
{
	if ((_HI3798C_V200 == get_chipid() || _HI3798M_V200 == get_chipid() || _HI3798M_A_V200 == get_chipid()))
		download_boot_normal(handle);
	else if (CHIPSET_CATYPE_NORMAL == get_ca_type())
		download_boot_normal(handle);
	else
		download_boot_ca(handle);
}

void selfboot_init(void)
{
	unsigned int bootstraptype = 0;

#ifdef CONFIG_USB_BOOTSTRAP
	bootstraptype = *(volatile unsigned int *)(REG_USB_UART_FLAG);
	switch (bootstraptype) {
#ifdef CONFIG_USB_HOST_BOOTSTRAP
		case SELF_BOOT_TYPE_USBHOST:
			usb_detect();
			break;
#endif

		default:
			break;
	}
#endif
}

void selfboot_from_udisk(void)
{
#ifdef CONFIG_USB_BOOTSTRAP
	unsigned int bootstraptype = 0;

	bootstraptype = *(volatile unsigned int *)(REG_USB_UART_FLAG);
	*(volatile unsigned int *)(REG_USB_UART_FLAG) = SELF_BOOT_TYPE_NONE;

	switch (bootstraptype) {

#  ifdef CONFIG_USB_HOST_BOOTSTRAP
		case SELF_BOOT_TYPE_USBHOST:
			download_process_usbhost();
			break;
#  endif
		default:
			break;
	}
#endif
}

unsigned int get_selfboot_type(void)
{
	unsigned int rc = SELF_BOOT_TYPE_NONE;

#ifdef CONFIG_USB_BOOTSTRAP
	unsigned int bootstraptype = 0;

	bootstraptype = *(volatile unsigned int *)(REG_USB_UART_FLAG);
	switch (bootstraptype) {

#ifdef CONFIG_USB_HOST_BOOTSTRAP
	case SELF_BOOT_TYPE_USBHOST:
		rc = SELF_BOOT_TYPE_USBHOST;
		break;
#endif
	default:
		break;
	}
#endif

	return rc;
}

int read_ca_key_file(char *filename, void *buffer)
{
	long filesize = -1;
#ifdef CONFIG_USB_BOOTSTRAP
	unsigned int bootstraptype = 0;
	int ret = 0 ;
	int crc = 0;

	bootstraptype = *(volatile unsigned int *)(REG_USB_UART_FLAG);
	switch (bootstraptype) {

#ifdef CONFIG_USB_HOST_BOOTSTRAP
		case SELF_BOOT_TYPE_USBHOST:
			ret = usb_file_check(filename);
			if (ret) {
				return -1;
			}

			filesize = usb_file_read(filename, buffer, 0, 0);
			if(filesize <= 4) {
				return -1;
			}

			/* crc value is in the last 4 bytes of ca key file with big endian*/
			crc = *(int *)(buffer+filesize-4);
			crc= ((crc & 0xff000000) >> 24) |((crc & 0xff0000) >>  8) |
				((crc & 0xff00) <<  8) | ((crc & 0xff) << 24);
			if (crc32(0, buffer, filesize-4) != crc ) {
				return -1;
			}
			break;
#endif
		default:
			break;
	}
#endif
	return filesize;
}
