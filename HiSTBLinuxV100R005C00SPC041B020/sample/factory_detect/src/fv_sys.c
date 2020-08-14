#include <string.h>
#include <unistd.h>
#include "fv.h"
#include "hi_type.h"
#include "hi_go.h"
#include "hi_unf_disp.h"
#include "hi_adp_mpi.h"
#include "hi_adp_hdmi.h"
#include "hi_adp.h"
#include "fv_dbg.h"
#include "hi_flash.h"
static void setup_mac_address()
{
#define MAX_STORE_PARTITION "deviceinfo"
#define MAX_SEPARATOR	':'
	int ret;
	HI_HANDLE handle;
	unsigned char buf[32] = {0};
	char cmd_buf[256];
	handle = HI_Flash_OpenByName(MAX_STORE_PARTITION);
	if (INVALID_FD == handle) {
		fv_error("Fail to open partition %s\n",MAX_STORE_PARTITION);
		goto out;
	}
	ret = HI_Flash_Read(handle, 0, buf, 32, HI_FLASH_RW_FLAG_RAW);
	if (ret < 0 && ret != HI_FLASH_END_DUETO_BADBLOCK) {
		fv_error("Fail to read partition %s from %d to %d\n",
				MAX_STORE_PARTITION, 0, 32);
		goto close_out;
	}
	buf[20] = '\0';
	if (!strchr((char*)buf, MAX_SEPARATOR)) {
		fv_error("Fail to find an separator '%c'.\n", MAX_SEPARATOR);
		goto close_out;
	}
	printf("Mac address read is :%s\n",buf);
	printf("try ifconfig eth0 up\n");
	system("ifconfig eth0 up");
	printf("try ifconfig eth0 hw ether %s\n", buf);
	snprintf(cmd_buf, sizeof(cmd_buf), "ifconfig eth0 hw ether %s", buf);
	system(cmd_buf);
	system("ifconfig -a");
close_out:
	(void)HI_Flash_Close(handle);
out:
	return;
}
int fv_system_init(void)
{
	HI_S32 Ret;
	HI_UNF_ENC_FMT_E               defaultFmt = HI_UNF_ENC_FMT_1080i_50 ;
	
	Ret = HI_SYS_Init();
	if (HI_SUCCESS != Ret)
	{
		fv_error("failed to init system! Ret = %#x\n",Ret); 
		HI_SYS_DeInit();
		return -1;        
	}    	

	setup_mac_address();

	Ret = HIADP_Disp_Init(defaultFmt);
	if (HI_SUCCESS != Ret)
	{
		fv_error("failed to init DISP! Ret = %#x\n",Ret); 
		HI_SYS_DeInit();
		return -1;        
	}   
	
	return Ret;
}


void fv_system_exit(void)
{
	HIADP_Disp_DeInit();
	HI_SYS_DeInit();
	
	return;	
	
}

