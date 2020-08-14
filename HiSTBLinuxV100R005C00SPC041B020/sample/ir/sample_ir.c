#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_ir.h"
//#include "hi_adp.h"

static char *prot_names[] = 
{
	"nec full 2headers",
	"nec simple 2headers changshu",
	"nec simple uPD6121G like",
	"nec simple LC7461MCL13 like",
	"nec full dezhou like",
	"nec full huizhou like",
	"nec simple 2headers gd",
	"credit",
	"sony d7c5",
	"sony d7c8",
	"tc9012",
	"rc6 32bit data",
	"rc6 20bit data",
	"rc6 16bit data",
	"rc5 14bit data",
	"extended rc5 14bit"
};

void usage(char *base)
{
	printf("usage: %s [-m 0|1] [-u 0|1] [-U num] [-r 0|1] [-R num] [-T readtimeout] [-E 0|1]"\
		" [-g protocolname] [-l] [-s protocolname -e 0|1] [-h]\n"\
		"\t-m : key fetch mode, 0 to read key parsed by driver, 1 to read symbols received.\n"\
		"\t-u : set keyup event, 0 to disable keyup event, 1 to enable keyup event.\n"\
		"\t-U : set keyup timeout, e.g. -U 300, driver will send a keyup event of last key parsed.\n"\
		"\t     after 300ms if no follow key recevied. Not supported yet.\n"\
		"\t-r : set repeat key event report or not. 0 to disable repeat event, 1 to enable.\n"\
		"\t-R : set repeat key event interval time. Default 200ms.\n"\
		"\t     e.g. -R 100 was set, then driver will report a key repeat event each 100ms.\n"\
		"\t-T : set read timeout value. e.g. -T 1000 was set, then read syscall will return after 1000 ms if no key recevied.\n"\
		"\t-E : enable or disable ir module. 0 to disable, 1 to enable. If 0 is set, ir module will do nothing.\n"
		"\t-g : get the enable/disable status of protocol which is specified by protocolname and then exit.\n"\
		"\t-l : list all default protocol name and exit. Only constains IR module, no customer's included!\n"\
		"\t-s : set the enable/disable status of protocol which is specified by protocolname, 0 to enable, 1 to disable.\n"\
		"\t     while set disable state, the respect protocol will not be considerd as a protocol parser,\n"\
		"\t     and if some corresponding infrared symbol received, they are not parsed and may be discarded.\n"\
		"\t-e : 0 to disable, 1 to enable, must be used with -s\n"\
		"\t-h : display this message and exit.\n"\
		"\tNOTES: Only [-s -e] option can be set more than once!\n",
		base);
}

extern char *optarg;
extern int optind, opterr, optopt;

/*lint -save -e729 */
int mode, mode_set;
int keyup_event, keyup_event_set;
unsigned int keyup_timeout, keyup_timeout_set;
int repeat_event, repeat_event_set;
unsigned int repeat_interval, repeat_interval_set;
unsigned int read_timeout = 200;
int ir_enable, ir_enable_set;
int prot_status;
char *prot_name;
int prot_name_set;
int prot_enable, prot_enable_set;

int g_thread_should_stop = 0;

static int check_params(void)
{
	if (mode_set >  1 || ( mode != 0 && mode != 1)) {
		printf("fetch mode error!\n");
		return -1;
	}

	if (keyup_event_set > 1 || (keyup_event != 0 && keyup_event != 1)) {
		printf("keyup event error!\n");
		return -1;
	}
	if (repeat_event_set > 1 || repeat_interval_set > 1) {
		printf("Only -g [-s -e] option can be set more than once!\n");
		return -1;
	}
	return 0;
}
static int ir_get_prot_status(char *prot)
{
	int ret;
	HI_BOOL enabled;

	ret = HI_UNF_IR_Init();
	if (ret) {
		perror("Fail to open ir device!\n");
		return -1;
	}

	ret = HI_UNF_IR_GetProtocolEnabled(prot, &enabled);
	if (ret) {
		HI_UNF_IR_DeInit();
		printf("Fail to get protocol status!\n");
		return -1;
	}

	HI_UNF_IR_DeInit();
	return enabled;

}
static int ir_sample_set_prot_enable(char *prot, int enable)
{
	int ret;
	
	ret = HI_UNF_IR_Init();
	if (ret) {
		perror("Fail to open ir device!\n");
		return -1;
	}

	printf("Try set enable status %d to protocol %s.\n", enable, prot);
	if (enable) 
		ret = HI_UNF_IR_EnableProtocol(prot);
	else
		ret = HI_UNF_IR_DisableProtocol(prot);
	if (ret) {
		printf("Fail to set enable status of %s\n",prot);
		HI_UNF_IR_DeInit();
		return -1;
	}

	printf("Check it out if we do have set succussfully!\n");
	if (enable == ir_get_prot_status(prot))
		printf("Check pass!\n");
	else
		printf("Check fail!\n");

	HI_UNF_IR_DeInit();
	return 0;
}
static void process_options(int argc, char *argv[])
{
	int op, ret;
	opterr = 0;
	while ((op = getopt(argc, argv, "m:u:U:r:R:T:E:g:ls:e:h")) != -1) {
		switch (op) {
			case 'm' :
				mode = atoi(optarg);
				mode_set ++;
				printf("key fetch mode set to :%d\n", mode);
				break;
			case 'u' :
				keyup_event = atoi(optarg);
				keyup_event_set ++;
				printf("keyup event set to %d\n",keyup_event);
				break;
			case 'U' :
				keyup_timeout = strtoul(optarg, 0, 10);
				keyup_timeout_set ++;
				printf("keyup timeout set to %d\n",keyup_timeout);
				break;
			case 'r' :
				repeat_event = atoi(optarg);
				repeat_event_set ++;
				printf("repeat event set to %d\n",repeat_event);
				break;
			case 'R' :
				repeat_interval = strtoul(optarg, 0, 10);
				repeat_interval_set ++;
				printf("repeat interval set to %d\n",repeat_interval);
				break;
			case 'T' :
				read_timeout = strtoul(optarg, 0, 10);
				printf("read syscall timeout set to %d\n",read_timeout);
				break;
			case 'E':
				ir_enable = atoi(optarg);
				if (ir_enable != 0 && ir_enable != 1) {
					printf("-E parameter not correct, only 0 or 1 allowed!\n");
					break;
				}
				ir_enable_set = 1;
				printf("Set ir mode to %s state!\n", ir_enable ? "enable": "disable");
				break;
			case 'g' :
				printf("try to get the enable status of %s\n",optarg);
				
				prot_status = ir_get_prot_status(optarg);
				if (prot_status >= 0)
					printf("The status of %s is :%s\n",
							optarg,
							prot_status == 0 ? "disabled" : "enabled");
				else
					printf("Fail to get protocol status!\n");

				exit(0);
			case 'l' :
				{
					int i;
					printf("Available default protocols list:\n");
					for (i = 0; i < (HI_S32)(sizeof(prot_names) / sizeof(prot_names[0])); i++)
						printf("%s\n",prot_names[i]);
				}
				exit(0);
			case 's' :
				prot_name = optarg;
				prot_name_set = 1;

				printf("prot_name :%s.\n", prot_name);

				if (prot_enable_set && prot_name_set) {
					prot_enable_set = 0;
					prot_name_set = 0;
					ret = ir_sample_set_prot_enable(prot_name, prot_enable);
					if (ret) {
						perror("Fail to set prot_enable!\n");
						exit(-1);
					}
				}

				break;
			case 'e' :
				prot_enable = atoi(optarg);
				prot_enable_set = 1;

				printf("protocol enable status will set to :%d\n",prot_enable);

				if (prot_enable_set && prot_name_set) {
					prot_enable_set = 0;
					prot_name_set = 0;
					ret = ir_sample_set_prot_enable(prot_name, prot_enable);
					if (ret) {
						perror("Fail to set prot_enable!\n");
						exit(-1);
					}
				}
				break;
			case 'h' :
            default :
				usage(argv[0]);
				exit(0);
		}
	}
}
/*lint -save -e715 */
void *ir_sample_thread(void *arg)
{
	int ret;
	HI_U64 key, lower, upper;
	char name[64];
	HI_UNF_KEY_STATUS_E status;

	while (!g_thread_should_stop) {
		if (mode) {
			ret = HI_UNF_IR_GetSymbol(&lower, &upper, read_timeout);
			if (!ret) {
				printf("Received symbols [%d, %d].\n",(HI_U32)lower, (HI_U32)upper);
			}
		} else {
			ret = HI_UNF_IR_GetValueWithProtocol(&status, &key, name, sizeof(name), read_timeout);
			if (!ret) {
				printf("Received key: 0x%.08llx, %s,\tprotocol: %s.\n",
						key, status == HI_UNF_KEY_STATUS_DOWN ? "DOWN" :
							(status == HI_UNF_KEY_STATUS_UP ? "UP" : "HOLD"),
						name);
			}
		}
	}

	return (void *)0;
}
int main(int argc, char *argv[])
{
	int ret;
	pthread_t thread;
    /*lint -save -e550 */
	char c;

	process_options(argc, argv);

	if(check_params() < 0) {
		usage(argv[0]);
		exit(-1);
	}

	ret = HI_UNF_IR_Init();
	if (ret) {
		perror("Fail to open ir dev!");
		exit(-1);
	}

	/* key mode. */
	if (!mode) {
		HI_UNF_IR_SetCodeType(0);		
		if (keyup_event_set) {
			ret = HI_UNF_IR_EnableKeyUp(keyup_event);
			if (ret) {
				perror("Fail to set keyup event!");
				exit(-1);
			}
		}

		if (keyup_timeout_set) {
			printf("This version cannot support dynamicly change keyup timeout!\n");
		}

		if (repeat_event_set) {
			ret = HI_UNF_IR_EnableRepKey(repeat_event);
			if (ret) {
				perror("Fail to set repeat key event!");
				exit(-1);
			}
		}

		if (repeat_interval_set) {
			ret = HI_UNF_IR_SetRepKeyTimeoutAttr(repeat_interval);
			if (ret) {
				perror("Fail to set repeat interval!");
				exit(-1);
			}
		}
	}
	
	ret = HI_UNF_IR_SetFetchMode(mode);
	if (ret) {
		perror("Fail to set key fetch mode!");
		if (ret == HI_ERR_IR_UNSUPPORT) {
			printf("You may not use s2 mode, please set -E 1 parameter to enable ir module!\n");
		} else {
			exit(-1);
		}
	}

	if (ir_enable_set) {
		ret = HI_UNF_IR_Enable(ir_enable);
		if (ret) {
			perror("Fail to set enable state of ir module!");
			exit(-1);
		}
	}

	printf("Create ir sample thread, press q to exit!\n");

	ret = pthread_create(&thread, NULL, ir_sample_thread, NULL);
	if (ret < 0) {
		perror("Failt to create thread!");
		exit(-1);
	}
	while ((c = getchar()) != 'q')
		sleep(3);

	g_thread_should_stop = 1;

	pthread_join(thread, NULL);

	printf("Ir sample thread exit! Bye!\n");

	HI_UNF_IR_DeInit();

	return 0;
}
