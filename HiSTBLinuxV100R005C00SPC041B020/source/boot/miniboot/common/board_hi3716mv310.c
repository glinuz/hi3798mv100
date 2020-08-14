/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Lai Yingjun
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <config.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm/io.h>
#include <delay.h>
#include <errno.h>
#include <string.h>
#include <module.h>
#include <platform.h>
#include <cpuinfo.h>
#include <board.h>
#include <env_set.h>
#include <kern_param.h>

#define ENV_ERROR       "Env config error: "
#define EXCEL_ERR       "Excel config error:"
#define MAX_INFNAME     16


static const char *_intfnames[] = {
	"mii", "rmii", "rgmii", "unknown", NULL,
};

static struct eth_config {
	char phyaddr[32];
	char phymdio[32];
	char phygpio[32];
	char phyintf[32];
} eth_config;

static int hi3716mv310_phyinf[] = {/* array index is the hw config */
	[0] = ETH_PHY_MII,
	[1] = ETH_PHY_RMII,
	[3] = ETH_PHY_UNKNOWN,
};

static char *substr(const char *str, int index, int dem)
{
	while (index > 0 && str) {
		str = strchr(str, dem);
		if (str)
			str++;
		index--;
	}
	if (str && (!*str || *str == (char)dem))
		str = NULL;

	return (char *)str;
}

char *get_eth_phyaddr_str(void)
{
	char *env = NULL;

	env = env_get("phyaddr");
	if (!env) 
		env = eth_config.phyaddr;

	return env;
}

/*
 * setenv phyaddr 'addr0,addr1'
 * addr(x): [0, 31];
 */
int get_eth_phyaddr(int index, int defval)
{
	char *env  = NULL;
	int phyaddr;

	env = env_get("phyaddr");
	if (!env) 
		env = eth_config.phyaddr;

	env = substr(env, index, ',');
	if (!env || !isxdigit(*env)) {
		printf("eth%d phyaddr invalid, use default:%d\n",
		       index, defval);
		return defval;
	}

	phyaddr = strtoul(env, NULL, 0);
	if (phyaddr < 0 || phyaddr > 31) {
		printf("eth%d phyaddr(%d) out of range, use default:%d\n",
		       index, phyaddr, defval);
		phyaddr = defval;
	}

	return phyaddr;
}

char *get_eth_phyintf_str(void)
{
	char *env = NULL;

	env = env_get("phyintf");
	if (!env)
		env = eth_config.phyintf;

	return env;
}
/*
 * setenv phyintf 'mii,rmii'
 * mii/rmii/rgmii
 */
enum eth_phyintf_t get_eth_phyintf(int index, enum eth_phyintf_t defval)
{
	char *env  = NULL;
	char *ptr  = NULL;
	enum eth_phyintf_t phyintf;
	char intfname[MAX_INFNAME];

	env = env_get("phyintf");
	if (!env)
		env = eth_config.phyintf;

	env = substr(env, index, ',');
	if (!env) {
		printf("eth%d phyintf invalid, use default:%s\n",
		       index, ethphy_intfname(defval));
		return defval;
	}

	strncpy(intfname, env, sizeof(intfname));
	intfname[sizeof(intfname) - 1] = '\0';
	ptr = strchr(intfname, ',');
	if (ptr)
		*ptr = '\0';

	phyintf = ethphy_intf(intfname);
	if (phyintf == ETH_PHY_UNKNOWN) {
		printf("eth%d phyintf invalid, use default:%s\n",
		       index, ethphy_intfname(defval));
		return defval;
	}

	return phyintf;
}

char *get_eth_phymdio_str(void)
{
	char *env = NULL;

	env = env_get("phymdio");
	if (!env)
		env = eth_config.phymdio;

	return env;
}
int get_eth_phymdio(int index, int defval)
{
	char *env = NULL;
	int phymdio;

	env = env_get("phymdio");
	if (!env)
		env = eth_config.phymdio;

	env = substr(env, index, ',');
	if (!env || !isxdigit(*env)) {
		printf(
		       "eth%d phymdio invalid, use default:%d\n",
		       index, defval);
		return defval;
	}

	phymdio = strtoul(env, NULL, 0);
	if (phymdio != 0 && phymdio != 1) {
		printf("eth%d phymdio(%d) invalid, use default:%d\n",
		       index, phymdio, defval);
		phymdio = defval;
	}

	return phymdio;
}

const char *ethphy_intfname(enum eth_phyintf_t phyintf)
{
	if (phyintf > ETH_PHY_UNKNOWN)
		phyintf = ETH_PHY_UNKNOWN;

	return _intfnames[phyintf];
}

enum eth_phyintf_t ethphy_intf(const char *intfname)
{
	int ix = 0;

	while (_intfnames[ix]) {
		if (!strncmp(intfname, _intfnames[ix], MAX_INFNAME))
			return ix;
		ix++;
	}
	return ETH_PHY_UNKNOWN;
}


uint32 get_gpiobase(int gpio_index)
{
	if (gpio_index >= MAX_GPIO_NUMS)
		return 0;

	if (gpio_index == 5)
		return REG_BASE_GPIO5;
	else
		return REG_BASE_GPIO0 + (GPIO_SIZE * gpio_index);
}

char *get_eth_phygpio_str(void)
{
	char *env = NULL;

	env = env_get("phygpio");
	if (!env)
		env = eth_config.phygpio;

	return env;
}
/*
 * gpio for eth phy reset
 * setenv phygpio '1-3,3-4'
 */
int get_eth_phygpio(int index, uint32 *gpio_base, uint32 *gpio_bit)
{
	char *env  = NULL;
	char *endp = NULL;
	uint32 base, bit, base_addr;

	*gpio_base = 0;
	*gpio_bit = 0;

	env = env_get("phygpio");
	if (!env)
		env = eth_config.phygpio;

	env = substr(env, index, ',');
	if (!env) {
		printf("eth%d phygpio invalid, use format 2-3,4-5\n", index);
		return -1;
	}

	if (isxdigit(*env)) {
		/* need more check whether usr set gpio0 */
		base = strtoul(env, &endp, 0);
		base_addr = get_gpiobase(base);
		if (!base_addr) {
			printf("eth%d phygpio base(%u) out of range.\n",
			       index, base);
			return -2;
		}
	} else {
		if (strncmp(env, "none", 4)) {
			printf("eth%d phygpio(%s) invalid\n",
			       index, env);
			return -2;
		}
		return 0;
	}

	endp++; /* skip the leading char '-' */

	bit = strtoul(endp, NULL, 0);
	if (bit > 7) {
		printf("eth%d phygpio bit(%u) invalid, "
		       "it should be less than 8.\n",
		       index, bit);
		return -3;
	}

	*gpio_base = base_addr;
	*gpio_bit = bit;

	return 0;
}

static void set_hi3716mv310_eth_phyintf(void)
{
	uint32 regval;
	uint32 phy0intf;

	regval = readl(REG_BASE_SF + HISFV300_MAC0_PORTSEL);
	phy0intf = ((regval >> 1) & 0x1);
	if (phy0intf != 0 && phy0intf != 1) {
		printf("MAC_PORTSEL register should be 0(MII) or 1(RMII), "
		       "but current is %d\n",
		       phy0intf);
		return;
	}

	snprintf(eth_config.phyintf, sizeof(eth_config.phyintf), "%s,%s",
		 ethphy_intfname(hi3716mv310_phyinf[phy0intf]),
		 "none");
}

static void set_eth_phymdio(uint32 regval)
{
	unsigned int phy0mdio;
	unsigned int phy1mdio;

	phy0mdio = (regval >> 8) & 0xff;
	phy1mdio = (regval >> 24) & 0xff;

	if (phy0mdio > 1 && phy0mdio != 0xff) {
		printf(EXCEL_ERR
		       "MAC0_MDIO register should be 0 or 1, "
		       "but current is %d\n", phy0mdio);
		return;
	}

	if (phy1mdio > 1 && phy1mdio != 0xff) {
		printf(EXCEL_ERR
		       "MAC1_MDIO register should be 0 or 1, "
		       "but current is %d\n", phy1mdio);
		return;
	}

	snprintf(eth_config.phymdio, sizeof(eth_config.phymdio),
		 "%d,%d", phy0mdio, phy1mdio);
}

static void set_eth_phyaddr(uint32 regval)
{
	unsigned int phy0addr;
	unsigned int phy1addr;

	phy0addr = regval & 0xff;
	phy1addr = (regval >> 16) & 0xff;

	if (phy0addr > 31) {
		printf(EXCEL_ERR
		       "MAC0_PHY_ADDR register should be 0-31, "
		       "but current is %d\n", phy0addr);
		return;
	}

	if (phy1addr > 31) {
		printf(EXCEL_ERR
		       "MAC1_PHY_ADDR register should be 0-31, "
		       "but current is %d\n", phy1addr);
		return;
	}

	snprintf(eth_config.phyaddr, sizeof(eth_config.phyaddr),
		 "%d,%d", phy0addr, phy1addr);
}

static void set_eth_phygpio(uint32 regval)
{
	unsigned int phy0gpio_base, phy0gpio_bit;
	unsigned int phy1gpio_base, phy1gpio_bit;
	uint32 len = 0;
	char *p = eth_config.phygpio;
	const int buflen = sizeof(eth_config.phygpio);

	phy0gpio_base	= regval & 0xff;
	phy0gpio_bit	= (regval >> 8) & 0xff;
	phy1gpio_base	= (regval >> 16) & 0xff;
	phy1gpio_bit	= (regval >> 24) & 0xff;

	/* 0xff means we don't use gpio to reset phy */
	if (phy0gpio_base == 0xff || phy0gpio_bit == 0xff)
		len = snprintf(p, buflen, "none,");
	else
		len = snprintf(p, buflen, "%x-%x,",
				phy0gpio_base, phy0gpio_bit);

	p += len;

	if (phy1gpio_base == 0xff || phy1gpio_bit == 0xff)
		len = snprintf(p, buflen - len, "none");
	else
		len = snprintf(p, buflen - len, "%x-%x",
				phy1gpio_base, phy1gpio_bit);
}


/*****************************************************************************/
void setup_eth_param(void)
{
	int i;
	char *env;
#define MAX_MAC_NUMS    (2)

	env = get_eth_phymdio_str();
	if (env)
		set_param_data("phymdio", env, strlen(env));

	env = get_eth_phyaddr_str();
	if (env)
		set_param_data("phyaddr", env, strlen(env));

	env = get_eth_phyintf_str();
	if (env)
		set_param_data("phyintf", env, strlen(env));

	env = env_get("ethaddr");
	if (env)
		set_param_data("ethaddr", env, strlen(env));

	for (i = 0; i < MAX_MAC_NUMS; i++) {
		char name[16];
		uint32 gpio_base, gpio_bit, data[2];

		get_eth_phygpio(i, &gpio_base, &gpio_bit);
		if (!gpio_base)
			continue;
		snprintf(name, sizeof(name), "phyio%d", i);
		data[0] = gpio_base;
		data[1] = gpio_bit;
		set_param_data(name, (const char *)&data, sizeof(data));
	}
}

/*****************************************************************************/

void eth_config_init(void)
{
	uint32 phyaddrmdio_val;
	uint32 phygpio_val;

	memset(&eth_config, 0, sizeof(eth_config));
	/* REG_SC_GEN10
	 * bit 0 -- 7:  phyaddr for mac0
	 * bit 8 -- 15: phymdio for mac0
	 * bit16 -- 23: phyaddr for mac1
	 * bit24 -- 31: phymdio for mac1
	 */
	phyaddrmdio_val = readl(REG_BASE_SCTL + REG_SC_GEN10);
	if (!phyaddrmdio_val) {
		printf("No phy addr or mdio config in register table.\n");
		return;
	}

	/* REG_SC_GEN11
	 * bit 0 -- 7:  phygpio base for mac0
	 * bit 8 -- 15: phygpio bit for mac0
	 * bit16 -- 23: phygpio base for mac1
	 * bit24 -- 31: phygpio bit for mac1
	 */
	phygpio_val = readl(REG_BASE_SCTL + REG_SC_GEN11);
	if (!phygpio_val) {
		printf("No phy gpio config in register table.\n");
		return;
	}

	set_eth_phygpio(phygpio_val);
	set_eth_phyaddr(phyaddrmdio_val & 0xff1fff1f);
	set_eth_phymdio(phyaddrmdio_val);
	set_hi3716mv310_eth_phyintf();
}
