/*
 * FileName:
 * Author:         v00297436
 * Description:
 * Version:
 * Function List:
 */

#ifndef _SMC_H_
#define _SMC_H_

#include <linux/of_device.h>
#include "tee_client_constants.h"
#include "teek_ns_client.h"

enum TC_NS_CMD_type {
	TC_NS_CMD_TYPE_INVALID = 0,
	TC_NS_CMD_TYPE_NS_TO_SECURE,
	TC_NS_CMD_TYPE_SECURE_TO_NS,
	TC_NS_CMD_TYPE_SECURE_TO_SECURE,
	TC_NS_CMD_TYPE_SECURE_CONFIG = 0xf,
	TC_NS_CMD_TYPE_MAX
};

extern struct device_node *np;

int smc_init_data(struct device *class_dev);
void smc_free_data(void);

unsigned int TC_NS_SMC(TC_NS_SMC_CMD *cmd, uint8_t flags);
unsigned int TC_NS_SMC_WITH_NO_NR(TC_NS_SMC_CMD *cmd, uint8_t flags);
/* Post command to TrustedCore without waiting for answer or result */
unsigned int TC_NS_POST_SMC(TC_NS_SMC_CMD *cmd);

void tc_smc_wakeup(void);
int teeos_log_exception_archive(void);

#endif
