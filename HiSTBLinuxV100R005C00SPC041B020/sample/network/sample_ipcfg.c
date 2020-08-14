#include <string.h>

#include "hi_unf_common.h"
#include "hi_api_eth.h"
#include "hi_adp.h"


const HI_CHAR eth_name[2][16] =
{
    "eth0",
    "eth1"
};

int main(HI_S32 argc, HI_CHAR *argv[])
{
    Eth_LinkStatus_E linkflag;
    HI_CHAR ipaddr[16];
    HI_CHAR ipmask[16];
    HI_CHAR getway[16];
    HI_CHAR dns[16];
    HI_CHAR mac[18];
    HI_S32 ret;
    Eth_Port_E eth = ETH_PORT_UP;

    if(argc == 2)
    {
    	if(strcmp(argv[1],"eth0") == 0)
    		eth = ETH_PORT_UP;
    	else if(strcmp(argv[1],"eth1") == 0)
    		eth = ETH_PORT_DOWN;
    }

    HIAPI_RUN_RETURN(HI_ETH_GetLinkStatus(eth, &linkflag));
    if (linkflag == ETH_LINK_STATUS_OFF)
    {
        printf("%s Port Is not link\n", eth_name[eth]);
        return 0;
    }
    else
    {
        printf(" %s Port Is link\n", eth_name[eth]);
    }

    HIAPI_RUN_RETURN(HI_ETH_Open(eth));
    memset(ipaddr, 0, sizeof(ipaddr));
    ret = HI_ETH_IPAddressGet(eth, ipaddr);
    if (ret != HI_SUCCESS)
    {
        strcpy(ipaddr, "10.85.180.59");
        ret = HI_ETH_IPAddressSet(eth, ipaddr);
    }

    printf("ipaddr = %s\n", ipaddr);

    memset(ipmask, 0, sizeof(ipmask));
    ret = HI_ETH_SubNetmaskGet(eth, ipmask);
    if (ret != HI_SUCCESS)
    {
        strcpy(ipmask, "255.255.254.0");
        ret = HI_ETH_SubNetmaskSet(eth, ipmask);
    }

    printf("ipmask = %s\n", ipmask);

    memset(getway, 0, sizeof(getway));
    ret = HI_ETH_GatewayGet(eth, getway);
    if (ret != HI_SUCCESS)
    {
        strcpy(getway, "10.85.180.1");
        ret = HI_ETH_GatewaySet(eth, getway);
    }

    printf("getway = %s\n", getway);

    memset(dns, 0, sizeof(dns));
    ret = HI_ETH_DNSGet(eth, dns);
    if (ret != HI_SUCCESS)
    {
        strcpy(dns, "10.73.55.101");
        ret = HI_ETH_DNSSet(eth, HI_TRUE, dns);
    }

    printf("dns = %s\n", dns);

    memset(mac, 0, sizeof(mac));
    HIAPI_RUN_RETURN(HI_ETH_GetMac(eth, mac));
    printf("mac = %s\n", mac);

    (void)HI_ETH_SetMac(eth, "00:01:02:03:04:05");

    memset(mac, 0, sizeof(mac));
    HIAPI_RUN_RETURN(HI_ETH_GetMac(eth, mac));
    printf("set mac = 00:01:02:03:04:05 get mac = %s\n", mac);

    return 1;
}
