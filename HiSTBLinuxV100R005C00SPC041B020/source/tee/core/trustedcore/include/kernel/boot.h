#ifndef _BOOT_H_
#define _BOOT_H_

#define SZ_1KB		1024
#define SZ_1MB		(SZ_1KB*SZ_1KB)

static inline void DWB(void) /* drain write buffer */
{
	asm volatile(
		"dsb"
	);
}

static inline void writel(unsigned val, unsigned addr)
{
	DWB();
	(*(volatile unsigned *)(addr)) = (val);
	DWB();
}

static inline void writew(unsigned val, unsigned addr)
{
	DWB();
	(*(volatile unsigned short *)(addr)) = (val);
	DWB();
}

static inline void writeb(unsigned val, unsigned addr)
{
	DWB();
	(*(volatile unsigned char *)(addr)) = (val);
	DWB();
}

static inline unsigned readl(unsigned addr)
{
	return (*(volatile unsigned *)(addr));
}

static inline unsigned readw(unsigned addr)
{
	return (*(volatile unsigned short *)(addr));
}

static inline unsigned char readb(unsigned addr)
{
	return (*(volatile unsigned char *)(addr));
}

static inline void clr_bit(unsigned val, unsigned bit)
{
	(val) = (val & (~(1 << bit)));
}

static inline void set_bit(unsigned val, unsigned bit)
{
	(val) = (val | (1 << bit));
}

void enable_irq(void);

/* manage a list of functions to call */
void boot_register_poll_func(void (*func)(void));
void boot_poll(void);
void boot_unregister_all_poll_func(void);

/* console.c */
void dprintf(const char *fmt, ...);
void dprintf_set_putc(void (*func)(unsigned));
void dprintf_set_flush(void (*func)(void));

/* gpio */
void gpio_output_enable(unsigned n, unsigned out);
void gpio_write(unsigned n, unsigned on);
int gpio_read(unsigned n);

/* misc.c */
void *alloc(unsigned sz); /* alloc 32byte aligned memory */
void *alloc_page_aligned(unsigned sz);

void *memcpy(void *dst, const void *src, unsigned len);
void *memset(void *dst, unsigned val, unsigned len);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *cs, const char *ct, unsigned int count);
int memcmp(const void *a, const void *b, unsigned len);
char *strstr(const char *s1, const char *s2);
int strlen(const char *s);
char *strncpy(char *dest, const char *src, unsigned int count);
char *strcat(char *s, const char *append);
char *strncat(char *dst, const char *src, unsigned int n);

/* clock */
unsigned cycles_per_second(void);
void print_cpu_speed(void);

/* LCD */
void console_init(void);
void console_putc(unsigned n);

void cprintf(const char *fmt, ...);

const char *get_fastboot_version(void);
void usbloader_init(void);

/* provided by board files */
void set_led(int on);

typedef void (*irq_handler)(unsigned n);

#define DIGEST_SIZE 20
#define SIGNATURE_SIZE 256

void compute_digest(void *data, unsigned len, void *digest_out);
int is_signature_okay(void *digest, void *signature, void *pubkey);

#define DBG_ERR		0x0001	/* report all error returns */
#define DBG_WARNING		0x0002	/* warning info*/
#define DBG_INFO		0x0004	/* debug info */
#define DBG_ALL		0xffff
#define DBG_NONE	0x0000

#define DEBUG_LEVEL	(DBG_ERR)
#define USB_DBG(level, fmt, ...)	\
	do {							\
		if ((level) & DEBUG_LEVEL)			\
			cprintf("USB: " fmt, ## __VA_ARGS__);	\
	} while (0)

#define DBG_DUMP_BUF(buf, len) do{\
	int i;\
	int *p = (void*)(buf);\
	for(i=0; i<(int)((len)/sizeof(*p)); i++){\
		cprintf("0x%x ", *(p+i));\
		if( !((i+1) & 0x07) )\
			cprintf("\n");\
	}\
	cprintf("\n");\
}while(0)

#endif
