#ifndef _SOCKIOCTL_H_
#define _SOCKIOCTL_H_

#include <linux/sockios.h>

#define SIOCSETPM	(SIOCDEVPRIVATE + 4)	/* set pmt wake up config */
#define SIOCSETSUSPEND	(SIOCDEVPRIVATE + 5)	/* call dev->suspend, debug */
#define SIOCSETRESUME	(SIOCDEVPRIVATE + 6)	/* call dev->resume, debug */

int higmac_ioctl(struct net_device *net_dev, struct ifreq *rq, int cmd);

#endif
