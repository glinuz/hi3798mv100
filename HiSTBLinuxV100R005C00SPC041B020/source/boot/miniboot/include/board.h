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

#ifndef _BOARD_H_
#define _BOARD_H_

#include <sys/types.h>

int get_eth_phyaddr(int index, int defval);

enum eth_phyintf_t {
	ETH_PHY_MII   = 0,
	ETH_PHY_RMII  = 1,
	ETH_PHY_RGMII = 2,
	ETH_PHY_UNKNOWN = 3,
};
const char *ethphy_intfname(enum eth_phyintf_t phyintf);

enum eth_phyintf_t ethphy_intf(const char *intfname);

enum eth_phyintf_t get_eth_phyintf(int index, enum eth_phyintf_t defval);

int get_eth_phymdio(int index, int defval);

int get_eth_phygpio(int index, uint32 *gpio_base, uint32 *gpio_bit);

char *get_eth_phyaddr_str(void);

char *get_eth_phyintf_str(void);

char *get_eth_phymdio_str(void);

char *get_eth_phygpio_str(void);

void eth_config_init(void);

void setup_eth_param(void);

#endif /* _BOARD_H_ */
