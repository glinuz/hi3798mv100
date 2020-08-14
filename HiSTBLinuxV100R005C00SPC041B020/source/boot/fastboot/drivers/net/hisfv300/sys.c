#include <config.h>
#include "hieth.h"
#include "mdio.h"
#include "mac.h"
#include "ctrl.h"
#include "glb.h"
#include "sys.h"

#ifdef CONFIG_ARCH_GODBOX
#  include "sys-godbox.c"
#endif

#ifdef CONFIG_ARCH_S40
#  include "sys-s40.c"
#endif

#ifdef CONFIG_ARCH_HI3798MX
#  include "sys-hi3798mx.c"
#endif

#ifdef CONFIG_ARCH_HI3716MV410
#  include "sys-hi3716mv410.c"
#endif

#ifdef CONFIG_ARCH_HI3798MV2X
#  include "sys-hi3798mv2x.c"
#endif
/**************************************************/
void hieth_sys_startup(void)
{
	hieth_clk_ena();
	/* undo reset */
	hieth_reset(0);
}

void hieth_sys_allstop(void)
{
}

void hieth_sys_init(void)
{
	hieth_funsel_config();
	hieth_sys_allstop();
	hieth_reset(0);
	/* first disable ETH clock, then reset PHY to load PHY address */
	hieth_clk_dis();
	hieth_phy_reset();
	hieth_clk_ena();
	/* After MDCK clock giving, delay 5ms before MDIO access */
	mdelay(5);
}

void hieth_sys_exit(void)
{
	hieth_funsel_restore();
	hieth_sys_allstop();
}
