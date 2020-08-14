
#include <malloc.h>

#include "device.h"
#include "dev_descriptors.h"

#define USTART	0xFA
#define UHEAD 	0xFE
#define UDATA	0xDA
#define UTAIL	0xED
#define UCMD	0XAB

char uACK[2] ={0xAA,'\0'};
char uNAK[2] ={0x55,'\0'};
struct usb_request * ep0req =NULL;
struct usb_request * tx_req  =NULL;
struct usb_request * rx_req  =NULL;
unsigned char   usb_config_value = 0;
static unsigned  rx_addr=0x1000000;
static uint32_t  rx_length;
static char *cmdbuf;

void tx_status(const char *status);
void rx_cmd(void);
void rx_data(void);

void usb_rx_data_complete(struct usb_request *req)
{
	if (req->actual > rx_length) {
		req->actual = rx_length;
	}

	rx_addr += req->actual;
	rx_length -= req->actual;

	if (rx_length > 0) {
		rx_data();
	} else {
		rx_cmd();
	}
}
 void usb_rx_cmd_complete(struct usb_request *req)
{
	int rc = 0;
	
	if (req->status != 0) 
		return;

	if (req->actual > MAX_PACKET_SIZE) 
		req->actual = MAX_PACKET_SIZE-1;    
	
	cmdbuf[req->actual] = 0;
	
	if (cmdbuf[0]==USTART) {
		tx_status(uACK);
	} else if(cmdbuf[0]==UHEAD) {
		rx_addr = (cmdbuf[5] <<24)|(cmdbuf[6] <<16)|(cmdbuf[7] <<8)|(cmdbuf[8]);
		rx_length =(cmdbuf[1]<<24)|(cmdbuf[2] <<16)|(cmdbuf[3] <<8)|(cmdbuf[4] );
		rx_data();
		tx_status(uACK);
		return;
	} else if(cmdbuf[0]==UCMD) {
		tx_status(uACK);
		rc= run_command(cmdbuf+3,0);
		if (rc < 0) {
 			tx_status("[EOT](ERROR)\n");
 		} else {
			tx_status("[EOT](OK)\n");
		}
	}else if (cmdbuf[0]==UTAIL) {
		if(rx_length>0) {
			tx_status(uNAK);
		} else {
			tx_status(uACK);
		}
	}
	rx_cmd();
}

void usb_tx_status_complete(struct usb_request *req)
{
	req->actual = 0;
	req->length = 0;
	free(req->buf);
	free(req);
}

void usb_ep0_complete( struct usb_request *req)
{
	//TODO:XXX
}

void rx_cmd(void)
{
	struct usb_request *req = rx_req;
	req->length = MAX_PACKET_SIZE;
	req->buf =cmdbuf;
	req->complete = usb_rx_cmd_complete;
	req->status = -EINPROGRESS;
	req->actual = 0;
	ep_queue(req,EPOUT);
}

void rx_data(void)
{
	struct usb_request *req = rx_req;
	req->length = MAX_PACKET_SIZE;
	req->buf =(char *)rx_addr;
	req->complete = usb_rx_data_complete;
	req->status = -EINPROGRESS;
	req->actual = 0;
	ep_queue( req,EPOUT);
}

void tx_status(const char *status)
{
	struct usb_request *req ;
	int len;
	
	req = (struct usb_request *)malloc(sizeof(*req));
	if (!req) {
		printf("Not enough malloc space available!\n");
		return;
	}
	len = strlen(status)+1;
	req->buf = (char *)malloc(MAX_PACKET_SIZE) ;
	if (!(req->buf)) {
		printf("Not enough malloc space available!\n");
		free(req);
		return;
	}
	memcpy(req->buf,status,len);
	req->length = len;
	req->complete = usb_tx_status_complete;
	req->status = -EINPROGRESS;
	req->actual = 0;
	ep_queue(req,EPIN);
}

/*
* Reset interface setting and re-init endpoint state (toggle etc).
* Call with altsetting < 0 to disable the interface.  The only other
* available altsetting is 0, which enables the interface.
*/
int usb_init_ep12(void)
{
	int rc = 0;
	struct usb_request *usb_req;

	dwc_pcd_ep_enable(UE_DIR_IN);
	dwc_pcd_ep_enable(UE_DIR_OUT);

	tx_req =(struct usb_request *)malloc(sizeof(*usb_req));
	if (!tx_req) {
		printf("Not enough malloc space available!\n");
		return -1;
	}
	memset(tx_req, 0, sizeof(*usb_req));
	tx_req->context =0 ;

	rx_req=(struct usb_request *)malloc(sizeof(*usb_req));
	if (!rx_req) {
		printf("Not enough malloc space available!\n");
		return -1;
	}
	memset(rx_req, 0, sizeof(*usb_req));
	cmdbuf = rx_req->buf =(char *)malloc(MAX_PACKET_SIZE) ;
	if (!cmdbuf) {
		printf("Not enough malloc space available!\n");
		return -1;
	}
	rx_req->complete = 0;
	
	return rc;
}

int usb_init_ep0(void)
{
	int rc = 0;
	struct usb_request *usb_req;

	ep0req = (struct usb_request *) malloc(sizeof(*usb_req));
	if (!ep0req) {
		printf("Not enough malloc space available!\n");
		return -1;
	}
	memset(ep0req, 0, sizeof(*usb_req));
	ep0req->buf = (char *)malloc(MAX_PACKET_SIZE);
	if (!(ep0req->buf)) {
		printf("Not enough malloc space available!\n");
		free(ep0req);
		return -1;
	}
	ep0req->complete = usb_ep0_complete;

	usb_config_value = 0;

	return rc;
}

 int usb_setup(const struct usb_ctrlrequest *ctrl)
{
	int rc= -EOPNOTSUPP;
	uint16_t w_index = ctrl->wIndex;
	uint16_t w_value = ctrl->wValue;
	uint16_t w_length = ctrl->wLength;
	dtable *d = descr_hs;
	ep0req->context = NULL;
	ep0req->length = 0;

	switch (ctrl->bRequest) {

	case GET_DESCRIPTOR:
		while(d->data) {
			if(w_value == d->id) {
				unsigned len = d->length;
				if(len > w_length) len = w_length;
				rc =len;
				memcpy(ep0req->buf, d->data,len);
				break;
			}
			d++;
		}

		rc = min(rc, w_length);
		ep0req->length = rc;
		ep0req->zero = rc < w_length;
		rc = ep_queue(ep0req,EP0);
		break;

	/* One config, two speeds */
	case SET_CONFIGURATION:
		if (w_value == 1) {
			usb_config_value = w_value;
			usb_init_ep12();
			rc =ep_queue(ep0req,EP0);
			rx_cmd();
		}else{
			usb_config_value = 0;
		}

		break;
	case GET_CONFIGURATION:

		if((w_value == 0) && (w_index == 0) && (w_length == 1))
			*(u8 *) ep0req->buf =usb_config_value;
		ep0req->length = 1;
		ep0req->zero = 0;
		rc = ep_queue(ep0req,EP0);
		break;
	default:
		break;
	}

	return rc;
}


