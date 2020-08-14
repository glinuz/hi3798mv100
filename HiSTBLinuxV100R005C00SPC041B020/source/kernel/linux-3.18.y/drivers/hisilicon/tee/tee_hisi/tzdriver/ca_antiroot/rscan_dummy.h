#ifndef RSCAN_DUMMY_H
#define RSCAN_DUMMY_H

#ifndef HASH_DIGEST_SIZE
#define HASH_DIGEST_SIZE 32 /* here use sha256*/
#endif

#define MAX_RPROC_SIZE 3680

/**
 ** define dynamic root scanner operation bit masks
 **/
#define D_RSOPID_KCODE     (1 << 0)  /*check integrity of kernel code*/
#define D_RSOPID_SYS_CALL  (1 << 1)  /*check system call*/
#define D_RSOPID_SE_HOOKS  (1 << 2)  /*check seLinux hooks*/
#define D_RSOPID_SE_STATUS (1 << 3)  /*check seLinux enforce status*/
#define D_RSOPID_HID_PROC  (1 << 4)  /*check hidden process*/
#define D_RSOPID_RRPOCS    (1 << 5)  /*check root processes*/

struct rscan_scan_result {
	uint8_t kcode[HASH_DIGEST_SIZE];    /* kernel code HASH */
	uint8_t syscalls[HASH_DIGEST_SIZE]; /* system call table HASH */
	uint8_t sehooks[HASH_DIGEST_SIZE];
	int seenforcing;   /* SE-Linux state */
	int hidproc;    /* Hidden Process Found */
	char rprocs[MAX_RPROC_SIZE]; /* root processes */
};

struct rscan_result_dynamic {
	uint8_t kcode[HASH_DIGEST_SIZE];    /* kernel code HASH */
	uint8_t syscalls[HASH_DIGEST_SIZE]; /* system call table HASH */
/* struct security_operations HASH */
	uint8_t sehooks[HASH_DIGEST_SIZE];
	int seenforcing;   /* SE-Linux state */
	int hidproc;    /* Hidden Process Found */
	char rprocs[MAX_RPROC_SIZE]; /* root processes */
};

struct rscan_status {
	uint32_t cpuload;     /* CPU usage: 0~100 */
	uint32_t battery;     /* Remaining battery: 0~100 */
	uint32_t charging;    /* Is it charging? 0~1 */
	/* Current time in UNIX Epoch time.
	   same as the time() systemcall */
	uint32_t time;
	/* Timezone in minutes.
	 * see tz_minuteswest in
	 * gettimeofday().e.g. Beijing +8 is -480
	 * */
	uint32_t timezone;
};


static const char *rprocs_whitelist =
	"/init:/sbin/adbd:/sbin/check_root:/sbin/hdbd:\
	/sbin/healthd:/sbin/hw_ueventd:/sbin/kmsgcat:\
	/sbin/logctl_service:/sbin/oeminfo_nvm_server:\
	/sbin/teecd:/sbin/usbshell:/system/bin/app_process32:\
	/system/bin/app_process64:/vendor/bin/atcmdserver:\
	/system/bin/bastetd:/system/bin/chargelogcat:\
	/system/bin/chargemonitor:/vendor/bin/data_cleaner:\
	/system/bin/debuggerd:/system/bin/debuggerd64:\
	/system/bin/defragd:/system/bin/device_monitor:\
	/system/bin/dhcpcd:/vendor/bin/diagserver:\
	/system/bin/dmesgcat:/system/bin/dumpstate:\
	/system/bin/dumptool:/system/bin/eventcat:\
	/system/bin/factory_log_service:/system/bin/filebackup:\
	/system/bin/hilogcat:/system/bin/hiscoutmanager:\
	/system/bin/hw_cdmamodem_service:/vendor/bin/hwnffserver:\
	/system/bin/installd:/system/bin/isplogcat:\
	/system/bin/jankservice:/system/bin/lmkd:/system/bin/logcat:\
	/system/bin/logd:/system/bin/mac_addr_normalization:\
	/system/bin/modemlogcat_lte:/system/bin/modemlogcat_via:\
	/system/bin/netd:/system/bin/pmom_cat:/system/bin/racoon:\
	/system/bin/rild:/system/bin/sdcard:/system/bin/sh:\
	/system/bin/shex:/system/bin/shs:/system/bin/sleeplogcat:\
	/system/bin/statusd:/system/bin/systeminfo:\
	/system/bin/tlogcat:/system/bin/uncrypt:/system/bin/vdc:\
	/system/bin/vold:/system/bin/tui_daemon:\
	/system/bin/wifiwakeupsrc:/system/bin/wpa_supplicant:\
	/system/xbin/perfprofd:/system/bin/goldeneye:\
	/system/bin/toolbox:/system/bin/toybox:/system/bin/grep:\
	/system/bin/iked:/system/bin/iptables:\
	/system/bin/logserver:/system/bin/dumpsys:/system/bin/ip6tables";

static const char test_kcode[HASH_DIGEST_SIZE] =
				"\xa9\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e"
				"\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x9d";

static const char test_syscalls[HASH_DIGEST_SIZE] =
				"\xa8\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e"
				"\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x8d";

static const char test_sehooks[HASH_DIGEST_SIZE] =
				"\xa7\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e"
				"\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x7d";


enum {
	CA_DISABLE = 0,
	CA_ENABLE  = 1
};
enum {
	ENG_MODE = 0,
	USER_MODE  = 1
};



int get_rootagent_enable(void);
int rscan_dynamic_raw(int op_mask, struct rscan_result_dynamic *result);
int rscan_get_status(struct rscan_status *status);
int load_rproc_whitelist(char *rproclist, ssize_t size);
int rscan_dummy_init(void);
void rscan_dummy_exit(void);
int dummy_tee_init(void);
int get_ro_secure(void);
void dummy_fuzz(void);
#endif
