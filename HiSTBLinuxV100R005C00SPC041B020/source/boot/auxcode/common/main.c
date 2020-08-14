#include <config.h>
#include <version.h>
#include <ddr_interface.h>
#include "aux_common.h"

extern void set_jtag_pinmux(void);
extern void timer_init(void);

#ifdef CONFIG_ARCH_HI3798CV2X
extern void emmc_quirk(int board_type);
#endif
/******************************************************************************/

static void show_version(void)
{
#ifdef CONFIG_SHOW_AUXCODE_INFO
	serial_puts("\nAuxiliary code - ");
	serial_puts(AUXCODE_VERSION"\n");
	serial_puts("DDR code - ");
	serial_puts(DDR_TRAINING_VER"\n");
	serial_puts("Build: ");
	serial_puts(__DATE__" - "__TIME__"\n");
#endif
}
/******************************************************************************/

int start_auxcode(enum cmd_type cmd, void *args)
{
	int ret = 0;
	struct emmc_cfg *param;
	unsigned long long chipid;

	step(2);
#ifdef CONFIG_EN_JTAG
	set_jtag_pinmux();
#endif
	timer_init();


	switch (cmd) {
	case CMD_GET_BD_TYPE:
		ret = get_board_type();
#ifdef CONFIG_ARCH_HI3798CV2X
		emmc_quirk(ret);
#endif
		break;

	case CMD_INIT_REG:
		show_version();
		ret = init_register(args);
		break;

#ifdef CONFIG_ARCH_HI3798MV2X
	case CMD_READ_EMMC_BOOT:
		param = (struct emmc_cfg *)args;
		ret = mmc_read(param->ptr, param->size, param->offset);
		break;

	case CMD_READ_EMMC_PARAM:
		
		chipid = get_chipid_reg();
		chipid &= _HI3798M_V200_MASK;

		if (_HI3798M_V200 == chipid) {
			param = (struct emmc_cfg *)args;
			ret = mmc_read(param->ptr, param->size, param->offset);
		} else {
			/* Hi3798MV200_a */
			ret = init_emmc();
		}
		break;
#endif
	default:
		break;
	}

	return ret;
}
/******************************************************************************/

