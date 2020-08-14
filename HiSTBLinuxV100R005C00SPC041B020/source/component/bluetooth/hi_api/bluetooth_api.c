#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <signal.h>
#include <getopt.h>

#include "hi_bluetooth.h"
#include "hci.h"
#include "hci_lib.h"
#include "dbus.h"

#include "hi_module.h"
#include "hi_mpi_mem.h"

#include "bluetooth_api.h"

/*==============================================================================================================*/
int hci_read_bd_addr(int dd, bdaddr_t *bdaddr, int to)
{
	read_bd_addr_rp rp;
	struct hci_request rq;

	memset(&rq, 0, sizeof(rq));
	rq.ogf    = OGF_INFO_PARAM;
	rq.ocf    = OCF_READ_BD_ADDR;
	rq.rparam = &rp;
	rq.rlen   = READ_BD_ADDR_RP_SIZE;

	if (hci_send_req(dd, &rq, to) < 0)
		return -1;

	if (rp.status) 
		return -1;

	if (bdaddr)
		bacpy(bdaddr, &rp.bdaddr);

	return 0;
}


int get_adapter_bdaddr(const char * hci_name,bdaddr_t *bdaddr)
{
	int ctl,sock;
	int result;
	int id;
	
	struct hci_dev_info di;
	
	if(!hci_name)
		id = 0;
	else
		id = atoi(hci_name + 3);
		
	if(id < 0 || id > HCI_MAX_DEV){
		perror("invalid hci name,use default\n");
		id = hci_get_route(NULL);
	}
	
	if ((ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI)) < 0) {
		perror("Can't open HCI socket.check kernel network config\n");
		return -1;
	}
	
	di.dev_id = id;	
	result = ioctl(ctl, HCIGETDEVINFO, (void *) &di);
	if(result < 0){
		perror("cannot get adapter %s info\n" ,hci_name ? hci_name :"hci0");
		return result;
	}		
	
	sock = hci_open_dev(di.dev_id);
	if(sock < 0){
		perror("open adpter failed,reult=%d\n",sock);
		return sock;
	}		
	
	result= hci_read_bd_addr(sock, &di.bdaddr, 1000);
	if(result < 0){
		perror("read bd addr failed\n");
		return result;
	}
	
	if(bdaddr)
		bacpy(bdaddr,&di.bdaddr);
	
	hci_close_dev(sock);
	close(ctl);	
	
	return 0;
}



int get_adapter_updown_status(const char *hci_name,int *updown_status)
{
	int ctl;
	int result;
	int id;
	
	struct hci_dev_info di;
	
	if(!hci_name)
		id = 0;
	else
		id = atoi(hci_name + 3);
	if(id < 0 || id > HCI_MAX_DEV){
		perror("invalid hci name,use default\n");
		id = hci_get_route(NULL);
	}
	
	if ((ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI)) < 0) {
		perror("Can't open HCI socket.check kernel network config\n");
		return -1;
	}

	
	di.dev_id = id;	
	result = ioctl(ctl, HCIGETDEVINFO, (void *) &di);
	if(result < 0){
		perror("cannot get adapter %s info\n" ,hci_name ? hci_name :"hci0");
		close(ctl);
		return result;
	}		

	if (!hci_test_bit(HCI_UP, &di.flags)){
		*updown_status = 0;
	}
	else
		*updown_status = 1;
	
	close(ctl);
	return 0;
}



int enable_adapter(const char* hci_name, int enable)
{
	/* Start HCI device */
	int request = enable ? HCIDEVUP : HCIDEVDOWN;
	int id,ctl;
    int result = 0;
	struct hci_dev_info di;
    
	if(!hci_name)
		id = 0;
	else
		id = atoi(hci_name + 3);	
	if(id < 0 || id > HCI_MAX_DEV){
		perror("invalid hci name\n");
		id = hci_get_route(NULL) ;	
	}
	
	if ((ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI)) < 0) {
		perror("Can't open HCI socket.check kernel network config\n");
		return -1 ;
	}	
	
	/* no adapter , error*/
	di.dev_id = id;	
	if (ioctl(ctl, HCIGETDEVINFO, (void *)&di)<0) {
		perror("Can't get device info\n");
        close(ctl);
		return -1;
	}
    
	result = ioctl(ctl, request, di.dev_id);
    if(result < 0) {
		perror("Can't init device %s %s (%d)\n",hci_name ? hci_name: "hci0",
			strerror(errno), errno);
		if (errno == EALREADY)
			result = 0;
	}
	
	close(ctl);
	return result;
}



int adapter_scan_devices(const char *hci_name,struct bt_device *pdevice,int max)
{ 
    inquiry_info *ii = NULL;
    int num_rsp;
    int id, sock, len, flags;
    int i,result = 0;
    char *dev_name;
    
    if(!hci_name)
		id = 0;
    else
		id = atoi(hci_name + 3);

    if( id < 0 || id > HCI_MAX_DEV ){	
    	perror("invalid hci name:%s,use default\n",hci_name);
    	id = hci_get_route(NULL);
    }

    sock = hci_open_dev( id );
    if (id < 0 || sock < 0) {
        perror("opening socket failed\n");
        return -1;
    }
    
    ii = (inquiry_info*)HI_MALLOC(HI_ID_BT, max * sizeof(inquiry_info));
    if(!ii){
    	perror("malloc inquiry_info failed\n");
    	result = -1;
    	goto end1;
    }

    len   = 10;
    flags = 0;
    //flags = IREQ_CACHE_FLUSH;

    num_rsp = hci_inquiry(id, len, max, NULL, &ii, flags);
    if( num_rsp < 0 ){ 
    	perror("hci_inquiry failed,id=%d,len=%d,result=%d\n",id,len,num_rsp);
    	result = -1;
    	goto end2;
    }
		
    for (i = 0; i < num_rsp; i++) {
    	bacpy(&(pdevice+i)->bdaddr,&(ii+i)->bdaddr);		
        dev_name = (pdevice+i)->dev_name;
        len = sizeof((pdevice+i)->dev_name);
        
        memset(dev_name, 0,len );
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, len, dev_name, 0) < 0)    
        	strcpy(dev_name, "[unknown]");      	
    }
    
    result = num_rsp;
    
end2:    
    HI_FREE(HI_ID_BT,  ii );
end1:    
    close( sock );
    return result;
}

static char *passkey_value=NULL;
static int passkey_delay = 0;
static int do_reject = 0;
static volatile sig_atomic_t __io_canceled = 0;
static volatile sig_atomic_t __io_terminated = 0;

static void sig_term(int sig)
{
	__io_canceled = 1;
}

static DBusHandlerResult agent_filter(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	const char *name, *old, *new;

	if (!dbus_message_is_signal(msg, DBUS_INTERFACE_DBUS,
						"NameOwnerChanged"))
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

	if (!dbus_message_get_args(msg, NULL,
					DBUS_TYPE_STRING, &name,
					DBUS_TYPE_STRING, &old,
					DBUS_TYPE_STRING, &new,
					DBUS_TYPE_INVALID)) {
		perror("Invalid arguments for NameOwnerChanged signal");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (!strcmp(name, "org.bluez") && *new == '\0') {
		perror( "Agent has been terminated\n");
		__io_terminated = 1;
	}

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}


static DBusHandlerResult request_pincode_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;
	const char *path;

	if (!passkey_value)
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_OBJECT_PATH, &path,
							DBUS_TYPE_INVALID)) {
		perror( "Invalid arguments for RequestPinCode method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (do_reject) {
		reply = dbus_message_new_error(msg, "org.bluez.Error.Rejected", "");
		goto send;
	}

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		perror( "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	HI_BT_ERR("Pincode request for device %s,passkey: %s\n", path,passkey_value);
	/*
	if (passkey_delay) {
		HI_BT_ERR("Waiting for %d seconds\n", passkey_delay);
		sleep(passkey_delay);
	}
	*/
	dbus_message_append_args(reply, DBUS_TYPE_STRING, &passkey_value,
							DBUS_TYPE_INVALID);

send:
	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}


static DBusHandlerResult request_passkey_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;
	const char *path;
	unsigned int passkey;

	if (!passkey_value)
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_OBJECT_PATH, &path,
							DBUS_TYPE_INVALID)) {
		perror( "Invalid arguments for RequestPasskey method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (do_reject) {
		reply = dbus_message_new_error(msg, "org.bluez.Error.Rejected", "");
		goto send;
	}

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		perror( "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	HI_BT_ERR("Passkey request for device %s\n", path);

	if (passkey_delay) {
		HI_BT_ERR("Waiting for %d seconds\n", passkey_delay);
		sleep(passkey_delay);
	}

	passkey = strtoul(passkey_value, NULL, 10);

	dbus_message_append_args(reply, DBUS_TYPE_UINT32, &passkey,
							DBUS_TYPE_INVALID);

send:
	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}


static DBusHandlerResult request_confirmation_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;
	const char *path;
	unsigned int passkey;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_OBJECT_PATH, &path,
						DBUS_TYPE_UINT32, &passkey,
							DBUS_TYPE_INVALID)) {
		perror( "Invalid arguments for RequestPasskey method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (do_reject) {
		reply = dbus_message_new_error(msg, "org.bluez.Error.Rejected", "");
		goto send;
	}

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		perror( "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	HI_BT_ERR("Confirmation request of %u for device %s\n", passkey, path);

	if (passkey_delay) {
		HI_BT_ERR("Waiting for %d seconds\n", passkey_delay);
		sleep(passkey_delay);
	}

send:
	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}



static DBusHandlerResult authorize_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;
	const char *path, *uuid;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_OBJECT_PATH, &path,
						DBUS_TYPE_STRING, &uuid,
							DBUS_TYPE_INVALID)) {
		perror( "Invalid arguments for Authorize method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (do_reject) {
		reply = dbus_message_new_error(msg, "org.bluez.Error.Rejected", "");
		goto send;
	}

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		perror( "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	HI_BT_ERR("Authorizing request for %s\n", path);

send:
	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}



static DBusHandlerResult cancel_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_INVALID)) {
		perror( "Invalid arguments for passkey Confirm method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	HI_BT_ERR("Request canceled\n");

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		perror( "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}



static DBusHandlerResult release_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_INVALID)) {
		perror( "Invalid arguments for Release method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (!__io_canceled)
		perror( "Agent has been released\n");

	__io_terminated = 1;

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		perror( "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}



static DBusHandlerResult agent_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestPinCode"))
		return request_pincode_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestPasskey"))
		return request_passkey_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestConfirmation"))
		return request_confirmation_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Authorize"))
		return authorize_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Cancel"))
		return cancel_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Release"))
		return release_message(conn, msg, data);

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}




static const DBusObjectPathVTable agent_table = {
	.message_function = agent_message,
};




static int create_paired_device(DBusConnection *conn, const char *adapter_path,
						const char *agent_path,
						const char *capabilities,
						const char *device)
{
	dbus_bool_t success;
	DBusMessage *msg;

	msg = dbus_message_new_method_call("org.bluez", adapter_path,
						"org.bluez.Adapter",
						"CreatePairedDevice");
	if (!msg) {
		perror( "Can't allocate new method call\n");
		return -1;
	}

	dbus_message_append_args(msg, DBUS_TYPE_STRING, &device,
					DBUS_TYPE_OBJECT_PATH, &agent_path,
					DBUS_TYPE_STRING, &capabilities,
					DBUS_TYPE_INVALID);

	success = dbus_connection_send(conn, msg, NULL);

	dbus_message_unref(msg);

	if (!success) {
		perror( "Not enough memory for message send\n");
		return -1;
	}

	dbus_connection_flush(conn);

	return 0;
}



static char *get_default_adapter_path(DBusConnection *conn)
{
	DBusMessage *msg, *reply;
	DBusError err;
	const char *reply_path;
	char *path;

	msg = dbus_message_new_method_call("org.bluez", "/",
					"org.bluez.Manager", "DefaultAdapter");

	if (!msg) {
		perror( "Can't allocate new method call\n");
		return NULL;
	}
	
	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);

	dbus_message_unref(msg);

	if (!reply) {
		perror(
			"Can't get default adapter\n");
		if (dbus_error_is_set(&err)) {
			perror( "%s\n", err.message);
			dbus_error_free(&err);
		}
		return NULL;
	}

	if (!dbus_message_get_args(reply, &err,
					DBUS_TYPE_OBJECT_PATH, &reply_path,
					DBUS_TYPE_INVALID)) {
		perror(
			"Can't get reply arguments\n");
		if (dbus_error_is_set(&err)) {
			perror( "%s\n", err.message);
			dbus_error_free(&err);
		}
		return NULL;
	}

	path = strdup(reply_path);

	dbus_message_unref(reply);

	dbus_connection_flush(conn);

	return path;
}



static char *get_adapter_path(DBusConnection *conn, const char *adapter)
{
	DBusMessage *msg, *reply;
	DBusError err;
	const char *reply_path;
	char *path;

	if (!adapter)
		return get_default_adapter_path(conn);

	msg = dbus_message_new_method_call("org.bluez", "/",
					"org.bluez.Manager", "FindAdapter");

	if (!msg) {
		perror( "Can't allocate new method call\n");
		return NULL;
	}

	dbus_message_append_args(msg, DBUS_TYPE_STRING, &adapter,
					DBUS_TYPE_INVALID);

	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);

	dbus_message_unref(msg);

	if (!reply) {
		perror("Can't find adapter %s\n", adapter);
		if (dbus_error_is_set(&err)) {
			perror( "%s\n", err.message);
			dbus_error_free(&err);
		}
		return NULL;
	}

	if (!dbus_message_get_args(reply, &err,
					DBUS_TYPE_OBJECT_PATH, &reply_path,
					DBUS_TYPE_INVALID)) {
		perror(
			"Can't get reply arguments\n");
		if (dbus_error_is_set(&err)) {
			perror( "%s\n", err.message);
			dbus_error_free(&err);
		}
		return NULL;
	}

	path = strdup(reply_path);

	dbus_message_unref(reply);

	dbus_connection_flush(conn);

	return path;
}





int pair_with_device_inter(const char * hci_name,struct bt_device *pdev,char *pass_key)
{
	const char *capabilities = "DisplayYesNo";
	struct sigaction sa;
	DBusConnection *conn;
	DBusError err;
	char match_string[128];
	char agent_path[128];
	char bd_str[20];
	
	int  result = -1;
	int  i = 0;
#define PARI_RETRIES	100 	
	
	char *adapter_path = NULL;

	passkey_value = pass_key;

	memset(bd_str,0,sizeof(bd_str));
	ba2str(&pdev->bdaddr,bd_str);

	/*invalid mac address*/
	if(bachk(bd_str))
		return -1;
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus");
		return -1;
	}
	
	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		HI_BT_ERR("failed to get %s path\n",hci_name);
		result = -1;
		goto end1;
	}
  
	snprintf(agent_path, sizeof(agent_path),"/org/bluez/agent_%s",
		hci_name ? hci_name : "hci0" );

	perror("adapter path %s,agent_path:%s,passkey:%s\n",
					adapter_path,agent_path,passkey_value);
	
	dbus_error_init(&err);
	
	if (!dbus_connection_try_register_object_path(conn, agent_path,
							&agent_table, NULL,&err)) {
		perror( "Can't register object path for agent\n");
		
		if (dbus_error_is_set(&err)) {
			perror( "%s\n", err.message);
			dbus_error_free(&err);
		}
		
		result = -1;
		goto end2;
	}
	
	/*
	if (!dbus_connection_register_object_path(conn, agent_path,
						&agent_table, NULL)) {
		perror( "Can't register object path for agent\n");
		result = -1;
		goto end2;
	}
	*/
	

	//pdbg("device bd is %s ,capabi:%s\n",bd_str,capabilities);
	if (create_paired_device(conn, adapter_path, agent_path,
					capabilities, bd_str) < 0) {
		result = -1;
		perror("paired with device failed\n");
		goto end3;
	}

	if (!dbus_connection_add_filter(conn, agent_filter, NULL, NULL))
		perror( "Can't add signal filter");
	
	snprintf(match_string, sizeof(match_string),
			"interface=%s,member=NameOwnerChanged,arg0=%s",
			DBUS_INTERFACE_DBUS, "org.bluez");
	
	dbus_bus_add_match(conn, match_string, NULL);

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags   = SA_NOCLDSTOP;
	sa.sa_handler = sig_term;
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT,  &sa, NULL);
	
	result = 0;
	__io_terminated = 0;

	while (!__io_canceled && !__io_terminated) {
		if (dbus_connection_read_write_dispatch(conn, 500) != TRUE){
			result= -1;
			break;
		}
		
		if(i++ > PARI_RETRIES){
			result = -1;
			break;
		}
	}

	sleep(2);
	
	if(__io_terminated && !result){
//		pinfo("pair with device %s ok\n",bd_str);
	}
	
end3:	
	dbus_connection_unregister_object_path(conn,agent_path);

end2:	
	HI_FREE(HI_ID_BT, adapter_path);
end1:	
	dbus_connection_unref(conn);
	return result;
}



/*==================================================================================================*/
/*
	note: 
	the memory or string we get with dbus_message_iter_get_basic donnt need to free.
	but the string we get from dbus_message_get_args must free.
*/
static char* parse_dbus_msg_str(DBusMessageIter *iter)
{
	char* tmp;
	int type = dbus_message_iter_get_arg_type(iter);
	if((type != DBUS_TYPE_STRING) && (type != DBUS_TYPE_OBJECT_PATH))
		return NULL;
	
	dbus_message_iter_get_basic(iter, &tmp);
	//pdbg("%s,tmp:%s\n",__func__,tmp);
	return tmp;
}


static int parse_dbus_msg_uint(DBusMessageIter *iter)
{
	int value;
	int type = dbus_message_iter_get_arg_type(iter);
	if( (type != DBUS_TYPE_UINT32) && (type != DBUS_TYPE_UINT16)
			&&(type != DBUS_TYPE_BOOLEAN) ){
		perror("invalid dbus type,type=%c\n",(char)type);
		return -1;
	}
	
	dbus_message_iter_get_basic(iter, &value);
//	pdbg ("%s uint32 %u\n",__func__, value);
	return value;
}


/*becaues of malloc ,so need to release it*/
static void* get_dbus_msg_subiter(DBusMessageIter *iter)
{
	DBusMessageIter *psubiter;
	int type = dbus_message_iter_get_arg_type(iter);
	if( (type != DBUS_TYPE_ARRAY) && (type != DBUS_TYPE_VARIANT) 
			&& (type != DBUS_TYPE_DICT_ENTRY) ){
		perror("invalied dbus type,type=%c\n",(char)type);
		return NULL;
	}	
	
	psubiter =(DBusMessageIter*)HI_MALLOC(HI_ID_BT, sizeof(DBusMessageIter));
	if(!psubiter)
		return NULL;

	dbus_message_iter_recurse (iter, psubiter);
	return psubiter;
}


static void release_subiter(DBusMessageIter *iter)
{
	if(!iter)	
		return;
	
	HI_FREE(HI_ID_BT, iter);	
}



/*
 corresponding dbus cmd example:
	#dbus-send --system --type=method_call --print-reply --dest=org.bluez  
		/org/bluez/381/hci0 org.bluez.Adapter.GetProperties	
"powered"
"Discoverable"
"Pairable"
"DiscoverableTimeout"
"Discovering"
*/
int get_adpater_property_uint(const char * hci_name,char * property,unsigned int *value)
{
	DBusConnection *conn;
	int result= 0;
	int type;
	char *adapter_path = NULL;
	char *tag = NULL;
	
	DBusError error;
	DBusMessage *message, *reply;
	DBusMessageIter reply_iter;
 	DBusMessageIter *iter_array;
 	DBusMessageIter *subiter;
 	DBusMessageIter *subsubiter;

	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus");
		return -1;
	}
	
	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		result = -1;
		goto end1;		
	}

	
	message = dbus_message_new_method_call("org.bluez", adapter_path,
						"org.bluez.Adapter",
						"GetProperties");
	if (message == NULL){
		perror("failed to alloc message\n");
		result = -1;
		goto end2;
	}
	
	dbus_error_init(&error);
	
	reply = dbus_connection_send_with_reply_and_block(conn, message,
							-1, &error);

	dbus_message_unref(message);
	
	if(!reply){	
		if (dbus_error_is_set(&error) == TRUE) {
			perror( "%s\n", error.message);
			dbus_error_free(&error);
		} else
			perror( "Failed to get properties\n");
		
		result = -1;
		goto end3;
	}

	dbus_message_iter_init(reply, &reply_iter);
	type = dbus_message_iter_get_arg_type(&reply_iter);
	if(type != DBUS_TYPE_ARRAY){
		result = -1;
		goto end3;
	}
	
	iter_array = (DBusMessageIter *)get_dbus_msg_subiter(&reply_iter);

	type = dbus_message_iter_get_arg_type(iter_array);
	pdbg("subiter type:%c\n",(char)type);
	
	while (type == DBUS_TYPE_DICT_ENTRY) {
		subiter = (DBusMessageIter *)get_dbus_msg_subiter(iter_array);
		tag = parse_dbus_msg_str(subiter);
		if(!strcmp(tag,property)){
			  pdbg("properties name :%s\n",tag);
			  result = 0;
			  
			  dbus_message_iter_next(subiter);
			  type = dbus_message_iter_get_arg_type(subiter);
			  pdbg("subiter type:%c\n",(char)type);
			  if(type != DBUS_TYPE_VARIANT){
			  	pdbg("subiter type:%c\n",(char)type);
			  	release_subiter(subiter);
			  	result = -1;
			  	break;
			  }
			  
			  subsubiter = get_dbus_msg_subiter(subiter);  
			  result = parse_dbus_msg_uint(subsubiter);
			  if(result < 0){
			  	pdbg("parse uint failed\n");
			  	release_subiter(subsubiter);
			  	release_subiter(subiter);
			  	result = -1;
			  	break;
			  }	

			  *value = result;
			  result= 0;
			  release_subiter(subsubiter);
			  release_subiter(subiter);
			  break;	  
		}		
		release_subiter(subiter);	
		
		dbus_message_iter_next(iter_array);
		type = dbus_message_iter_get_arg_type(iter_array);
	}
	
	release_subiter(iter_array);
end3:	
	dbus_message_unref(reply);
end2:
	HI_FREE(HI_ID_BT, adapter_path);
end1:	
	dbus_connection_unref(conn);
	
	return result;
}


/*
	device_path transfer example:
	00:0A:94:80:1B:1E -> /org/bluez/381/hci0/dev_00_0A_94_80_1B_1E : 
*/
static int  get_dev_objpath(char *dev_path, char *adpater_path, bdaddr_t *dev_bdaddr)
{
	uint8_t b[6];
	char str[40];

	if(!dev_path || ! adpater_path || !dev_bdaddr )
		return -1;
	
	strcpy(dev_path,adpater_path);
	
	baswap((bdaddr_t *) b, dev_bdaddr);
	
	sprintf(str, "/dev_%2.2X_%2.2X_%2.2X_%2.2X_%2.2X_%2.2X",
		b[0], b[1], b[2], b[3], b[4], b[5]);
		
	strcat(dev_path,str);

	return 0;
}


/*
	device_path transfer example:
	/org/bluez/381/hci0/dev_00_0A_94_80_1B_1E  ->  00:0A:94:80:1B:1E
*/
static int get_bdaddr_from_objpath(const char *dev_path,bdaddr_t *dev_addr)
{
	 char *tmp = NULL;
	 char *base = NULL;

	if(!dev_path || !dev_addr)
		return -1;

	 tmp = strchr(dev_path,'_');
	 if(!tmp)
	 	return -1;
	 
	 tmp++; //skip '_'
	 base = tmp;
	 
	// pdbg("%s: %s\n",__func__,tmp);
	 
	 while(*tmp != '\0'){
	 	if(*tmp == '_')
	 		*tmp=':';
		tmp++;
	 }	 	
	 
	 str2ba(base,dev_addr);
	 
	 return 0;
}


/*======================================================================================================*/
//#define USE_LISTDEVICE_METHOD
int get_paired_device_name(DBusConnection *conn,const char* dev_path,char *dev_name);
int get_paired_device_class(DBusConnection *conn,const char* dev_path,unsigned int *dev_class);
static int __get_paird_devices(DBusConnection *conn, const char* adapter_path,struct bt_device *pdev,int max)
{
	int type;
	int numbers = 0;
	int result = -1;
	
	DBusError error;
	DBusMessage *message, *reply;
	DBusMessageIter reply_iter;
 	DBusMessageIter *iter_array;
 	DBusMessageIter *subiter;
 	DBusMessageIter *subsubiter;
 	DBusMessageIter *subsubsubiter;
 	char *tag,*dev_path;
 
	message = dbus_message_new_method_call("org.bluez", adapter_path,
						"org.bluez.Adapter",
						"GetProperties");
	if (message == NULL){	
		perror( "failed to alloc memory\n");
		return -1;
	}
	
	dbus_error_init(&error);
	reply = dbus_connection_send_with_reply_and_block(conn, message,
								-1, &error);
	dbus_message_unref(message);
	
	if(!reply){
		if (dbus_error_is_set(&error)) {
			perror( "%s\n", error.message);
			dbus_error_free(&error);
		} else
			perror( "Failed to get properties\n");	
		return -1;
	}

	dbus_message_iter_init(reply, &reply_iter);
	type = dbus_message_iter_get_arg_type(&reply_iter);
	if(type != DBUS_TYPE_ARRAY){
		perror("%s type:%c\n",__func__,(char)type);
		dbus_message_unref(reply);
		return -1;
	}
	
	iter_array = (DBusMessageIter *)get_dbus_msg_subiter(&reply_iter);
	while (dbus_message_iter_get_arg_type(iter_array) == DBUS_TYPE_DICT_ENTRY) {

		subiter = (DBusMessageIter *)get_dbus_msg_subiter(iter_array);
		tag = parse_dbus_msg_str(subiter);
		if(!strcmp("Devices",tag)){			
			dbus_message_iter_next(subiter);
			type = dbus_message_iter_get_arg_type(subiter);
			if(type != DBUS_TYPE_VARIANT){
				pdbg("%d subiter type:%c\n",__LINE__,(char)type);
				result = -1;
				release_subiter(subiter);
				break; 
			}

			subsubiter = get_dbus_msg_subiter(subiter);
			type = dbus_message_iter_get_arg_type(subsubiter);
			if(type !=  DBUS_TYPE_ARRAY){
				pdbg("%d subiter type:%c\n",__LINE__,(char)type);
				result = -1;
				release_subiter(subsubiter);
				release_subiter(subiter);
				break; 
			}

			subsubsubiter = get_dbus_msg_subiter(subsubiter);
			type = dbus_message_iter_get_arg_type(subsubsubiter);
			if(type !=  DBUS_TYPE_OBJECT_PATH){
				pdbg("%d subiter type:%c\n",__LINE__,(char)type);
				
				/*
				  no device paired ,so this path is NULL
				  at this case ,just return 0 to indicate no device
				  paired
				*/
				result = 0;
				release_subiter(subsubsubiter);
				release_subiter(subsubiter);
				release_subiter(subiter);
				break; 
			}	
			
			
			while (type == DBUS_TYPE_OBJECT_PATH) {	
				dev_path = parse_dbus_msg_str(subsubsubiter);
				pdbg("%s %s\n",__func__,dev_path);
                
                if( numbers >= max )
					break;
                
                result = get_paired_device_name(conn,dev_path,(pdev+numbers)->dev_name);
				if(result < 0 ){
					strcpy((pdev+numbers)->dev_name,"unknown");
				}

				get_paired_device_class(conn,dev_path,&((pdev+numbers)->dev_class));
				get_bdaddr_from_objpath(dev_path,&(pdev+numbers)->bdaddr);	
					
				numbers++;
				dbus_message_iter_next(subsubsubiter);
				type = dbus_message_iter_get_arg_type(subsubsubiter);
			}

			release_subiter(subsubsubiter);
			release_subiter(subsubiter);
			release_subiter(subiter);
			result = numbers;		
			break;
		}
				
		release_subiter(subiter);
		dbus_message_iter_next(iter_array);
	}
	
	release_subiter(iter_array);
	dbus_message_unref(reply);
	
	return result;
}


/*
	examples:
	dbus-send --system --type=method_call --print-reply --dest=org.bluez /org/bluez/381/hci0 
	org.bluez.Adapter.ListDevices
*/

int get_paired_devices(const char * hci_name,struct bt_device *pdev,int max)
{
	DBusConnection *conn;
	int result = 0;
	char *adapter_path = NULL;

	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus");
		return -1;
	}
	
	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		dbus_connection_unref(conn);
		return -1;		
	}
	
	result = __get_paird_devices(conn,adapter_path,pdev,max);
	if(result < 0){
		perror("get paird devices failed\n");
	}
	
	HI_FREE(HI_ID_BT, adapter_path);
	
	dbus_connection_unref(conn);

	return result;
}


/*======================================================================================================*/
/*
	examples:
	dbus-send --system --dest=org.bluez --print-reply /org/bluez/381/hci0 org.bluez.Adapter.RemoveDevice 
	objpath:/org/bluez/381/hci0/dev_00_1E_DE_20_18_1B
*/

int remove_paried_device(const char * hci_name,struct bt_device *pdev )
{
	DBusMessage *msg, *reply;
	DBusError err;
	DBusConnection *conn;
	char dev_path[128];
	char *tmp,*adapter_path;
	int result;
	
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus");
		return -1;
	}
	
	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		dbus_connection_unref(conn);
		return -1;		
	}

	memset(dev_path,0,sizeof(dev_path));
	tmp = dev_path;
	
	get_dev_objpath(tmp, adapter_path, &pdev->bdaddr);	
	msg = dbus_message_new_method_call("org.bluez", adapter_path,
					"org.bluez.Adapter", "RemoveDevice");
	if (!msg){
		perror("%s:%d\n",__func__,__LINE__);	
		result = -1;
		goto end;
	}	

	dbus_message_append_args(msg, DBUS_TYPE_OBJECT_PATH, &tmp,DBUS_TYPE_INVALID);
	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(conn,msg, -1, &err);

	dbus_message_unref(msg);
	
	if (!reply) {
		if (dbus_error_is_set(&err) == TRUE) {
			perror( "error is %s\n", err.message);
			dbus_error_free(&err);
		} else
			perror( "Failed to set property\n");
		result = -1;
		goto end;
	}
	
	dbus_message_unref(reply);
	result = 0;
	
end:
	HI_FREE(HI_ID_BT, adapter_path);
	dbus_connection_unref(conn);
	
	pdbg("Removed device %s,result:%d\n", dev_path,result);
	
	return result;
	
}


int pair_with_device(const char * hci_name,struct bt_device *pdev,char *pass_key)
{

	int ret = 0;
	int i = 0;
	struct bt_device tmp[HI_BLUETOOTH_DEV_LIST_MAX];
	memset(tmp,0,HI_BLUETOOTH_DEV_LIST_MAX*sizeof(struct bt_device));
	ret=pair_with_device_inter(hci_name,pdev,pass_key);
	if(ret==0)
	{
		ret=get_paired_devices(hci_name,tmp,HI_BLUETOOTH_DEV_LIST_MAX);
		if(ret <= 0)
			return -1;
		else 
		{
			for(i = 0; i < ret; i++)
			{
				if(bacmp(&(pdev->bdaddr),&(tmp[i].bdaddr))==0)
					return 0;
			}
			return -1;
		}	
	}
	return ret;

}

/* 
 example:
 dbus-send --system --type=method_call --print-reply --dest=org.bluez 
 	/org/bluez/381/hci0/dev_00_1E_DE_20_18_1B org.bluez.Control.IsConnected
*/
static int get_device_properties(
    const char *serverpath,
	DBusConnection *conn,
	const char *dev_path,
	const char *property,
	int p_type,
	void *p_value){
	
	DBusMessage *msg, *reply;	
	DBusError err;
	DBusMessageIter reply_iter;
	DBusMessageIter *iter_array;
	DBusMessageIter *subiter;
	DBusMessageIter *subsubiter;

	int result = -1;
	int type;
	char *tmp = NULL;

	msg = dbus_message_new_method_call("org.bluez", dev_path,
			serverpath, "GetProperties");
	if (!msg){
		perror("%s:%d\n",__func__,__LINE__);	
		return -1;
	}	

	dbus_error_init(&err);
	reply = dbus_connection_send_with_reply_and_block(conn,msg, -1, &err);

	dbus_message_unref(msg);

	if (!reply) {
		if (dbus_error_is_set(&err) == TRUE) {
			perror( "%s\n", err.message);
			dbus_error_free(&err);
		} else
			perror( "Failed to set property\n");
		return -1;
	}

	dbus_message_iter_init(reply, &reply_iter);
	type = dbus_message_iter_get_arg_type(&reply_iter);
	if(type != DBUS_TYPE_ARRAY){
		perror("reply_iter type:%c\n",(char)type);
		dbus_message_unref(reply);
		return -1;
		
	}

	iter_array = (DBusMessageIter *)get_dbus_msg_subiter(&reply_iter);
	type = dbus_message_iter_get_arg_type(iter_array);
	while (type == DBUS_TYPE_DICT_ENTRY) {
		subiter = (DBusMessageIter *)get_dbus_msg_subiter(iter_array);
		tmp = parse_dbus_msg_str(subiter);
		if(!strcmp(tmp,property)){
			dbus_message_iter_next(subiter);
			type = dbus_message_iter_get_arg_type(subiter);
			if(type != DBUS_TYPE_VARIANT){
				perror("subiter type:%c\n",(char)type);
				release_subiter(subiter);
				result = -1;
				break;
			}

			subsubiter = get_dbus_msg_subiter(subiter);
			type = dbus_message_iter_get_arg_type(subsubiter);
			if(p_type != type){
				perror("invalid properties type,desire_type:%c,actual_type:%c\n",
							(char)p_type,(char)type);

				release_subiter(subsubiter);
				release_subiter(subiter);
				result = -1;
				break;
			}	

			switch(p_type){
			case DBUS_TYPE_STRING:
				{
					char *str;
					str = parse_dbus_msg_str(subsubiter);
					if(!str){
						perror("parse uint failed\n");
						result = -1;
						break;
					}
					pdbg("%s value is:%s\n",property,str);
					strcpy((char*)p_value,str);
				}
				result = 0;
				break;

			case DBUS_TYPE_UINT32:
			case DBUS_TYPE_BOOLEAN:
			case DBUS_TYPE_UINT16:
				result = parse_dbus_msg_uint(subsubiter);
				if(result < 0){
					perror("parse uint failed,result:%d\n",result);
					result = -1;
					break;
				}	

				//pdbg("%s value is:%d\n",property,result);
				*(int *)p_value = result;
				result = 0;
				break;

			default:
				perror("%s:invalid type:%d",__func__,type);
				result = -1;
				break;
			}	
			release_subiter(subsubiter);	
			release_subiter(subiter);
			break;
		}
		
		release_subiter(subiter);
		dbus_message_iter_next(iter_array);
		type = dbus_message_iter_get_arg_type(iter_array);
	}

	release_subiter(iter_array);
	dbus_message_unref(reply);

	pdbg("%s ok ,result=%d\n",__func__,result);
	return result;
}


/*
dbus-send --system --dest=org.bluez --print-reply /org/bluez/136/hci0/dev_00_1F_20_E1_7A_1F 
org.bluez.Device.SetProperty string:Trusted variant:boolean:true 
*/
static int set_device_trust_properties(
	const char *serverpath,
	DBusConnection *conn,
	const char *dev_path,
	const char *property){
	
	DBusMessage *msg, *reply;	
	DBusError err;
	DBusMessageIter iter;
	DBusMessageIter *target_iter;
	DBusMessageIter container_iter;
	dbus_bool_t v_BOOLEAN;
	char sig[2];
	int result = 0;

	msg = dbus_message_new_method_call("org.bluez", dev_path,
			serverpath, "SetProperty");
	if (!msg){
		perror("%s:%d\n",__func__,__LINE__);	
		return -1;
	}	

	dbus_message_iter_init_append (msg, &iter);
	
	target_iter = &iter;
	dbus_message_iter_append_basic (target_iter, DBUS_TYPE_STRING, &property);
	
	sig[0] = DBUS_TYPE_BOOLEAN;
	sig[1] = '\0';
	dbus_message_iter_open_container (&iter,
					DBUS_TYPE_VARIANT,
					sig,
					&container_iter);
	target_iter = &container_iter;
	v_BOOLEAN = TRUE;
	dbus_message_iter_append_basic (target_iter, DBUS_TYPE_BOOLEAN, &v_BOOLEAN);
	dbus_message_iter_close_container (&iter,
					&container_iter);

	dbus_error_init(&err);
	reply = dbus_connection_send_with_reply_and_block(conn,msg, -1, &err);
	dbus_message_unref(msg);

	if (!reply) {
		if (dbus_error_is_set(&err) == TRUE) {
			perror( "%s\n", err.message);
			dbus_error_free(&err);
		} else
			perror( "Failed to set property\n");
		return -1;
	}
	
	dbus_message_unref(reply);

	return result;
}



int get_paired_device_name(DBusConnection *conn,const char* dev_path,char *dev_name)
{
	return get_device_properties("org.bluez.Device",conn,dev_path,"Name",DBUS_TYPE_STRING,dev_name);
}

int get_paired_device_class(DBusConnection *conn,const char* dev_path,unsigned int *dev_class)
{
	return get_device_properties("org.bluez.Device",conn,dev_path,"Class",DBUS_TYPE_UINT32,dev_class);
}

int get_device_conn_status(const char *hci_name,struct bt_device *pdev,unsigned int *status)
{	
	DBusConnection *conn;
	char * adapter_path;
	char dev_path[128];

	int result;
	
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus");
		return -1;
	}
	
	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		dbus_connection_unref(conn);
		return -1;		
	}

	memset(dev_path,0,sizeof(dev_path));
	get_dev_objpath(dev_path, adapter_path, &pdev->bdaddr);	
	
	result = get_device_properties("org.bluez.Device",conn,dev_path,"Connected",DBUS_TYPE_BOOLEAN,status);

	HI_FREE(HI_ID_BT, adapter_path);
	
	dbus_connection_unref(conn);
	
	return result;
	
}


int get_headset_conn_status(const char *hci_name,struct bt_device *pdev,unsigned int *status)
{	
	DBusConnection *conn;
	char * adapter_path;
	char dev_path[128];
	char str_status[20];

	int result;
	
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus");
		return -1;
	}
	
	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		dbus_connection_unref(conn);
		return -1;		
	}

	memset(dev_path,0,sizeof(dev_path));
	get_dev_objpath(dev_path, adapter_path, &pdev->bdaddr);	
	
	result = get_device_properties("org.bluez.Audio",conn,dev_path,"State",DBUS_TYPE_STRING,str_status);
	if(strcmp(str_status, "connected") == 0) 
		* status = 1;
	else 
		* status = 0;	

	HI_FREE(HI_ID_BT, adapter_path);
	
	dbus_connection_unref(conn);
	
	return result;
	
}

int get_connected_headset(const char *hci_name,struct bt_device *pdev)
{	
    int ret = 0;
	struct hci_conn_list_req *cl;
	struct hci_conn_info *ci;
	int i, sk, id = 0;
	DBusConnection *conn;
	char * adapter_path;
	char dev_path[128];
       char status[20];

	if(!hci_name)
		id = 0;
	else
		id = atoi(hci_name + 3);	
    
	if(id < 0 || id > HCI_MAX_DEV){
		perror("invalid hci name\n");
		id = hci_get_route(NULL) ;	
	}
    
    if(!pdev){
        return -1;
    }

	if (!(cl = HI_MALLOC(HI_ID_BT, 10 * sizeof(*ci) + sizeof(*cl)))) {
		perror("Can't allocate memory");
		return -1;
	}
	cl->dev_id = id;
	cl->conn_num = 10;
	ci = cl->conn_info;

	sk = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
	if (sk < 0)
    {   
	    HI_FREE(HI_ID_BT, cl);
		return -1;
    }

	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus");
        ret = -1;
		goto finished1;
	}
	
	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		dbus_connection_unref(conn);
        ret = -1;
		goto finished1;
	}

	if (ioctl(sk, HCIGETCONNLIST, (void *) cl)) {
		perror("Can't get connection list");
        ret = -1;
		goto finished2;
	}
    
	for (i = 0; i < cl->conn_num; i++, ci++) {
    	memset(dev_path,0,sizeof(dev_path));
    	get_dev_objpath(dev_path, adapter_path, &ci->bdaddr);	
	    ret = get_device_properties("org.bluez.Audio",conn,dev_path,"State",DBUS_TYPE_STRING,status);
        if((ret == 0)&&(strncmp(status, "connected", 9)==0))
        {
            pdev->bdaddr = ci->bdaddr;
            memset(pdev->dev_name,0,sizeof(pdev->dev_name));
        	get_device_properties("org.bluez.Device",conn,dev_path,"Name",DBUS_TYPE_STRING,pdev->dev_name);
            goto finished2;
        }
	}
    
    ret = -1;
finished2:
    HI_FREE(HI_ID_BT, adapter_path);
    dbus_connection_unref(conn);    
finished1:
	close(sk);
	HI_FREE(HI_ID_BT, cl);
	return ret;
}

static int disconnect_with_device_admin(const char *dev_path,DBusConnection *conn)
{
	DBusMessage *message, *reply;	
	DBusError error;
	
	message = dbus_message_new_method_call("org.bluez", dev_path,"org.bluez.Device","Disconnect");
	if (message == NULL){
		perror( "failed to alloc memory\n");
		return -1;
	}
	
	dbus_error_init(&error);
	reply = dbus_connection_send_with_reply_and_block(conn, message,
								-1, &error);
	dbus_message_unref(message);
	
	if(!reply){
		if (dbus_error_is_set(&error)) {
			perror( "%s\n", error.message);
			dbus_error_free(&error);
		} else
			perror( "Failed to connect to device\n");	
		return -1;
	}
	
	return 0;
}

#if 0
static DBusHandlerResult headset_signal_filter(DBusConnection *conn,DBusMessage *msg, void *data)
{
    HI_BT_INFO("\n******* Head set signal filter , data [%d]\n",(int)data);
    if(dbus_message_is_signal(msg,"org.bluez.Headset","AnswerRequested"))
        HI_BT_INFO("Headset signal : AnswerRequested, data [%d]\n",(int)data);
    if(dbus_message_is_signal(msg,"org.bluez.Headset","Connected"))
        HI_BT_INFO("Headset signal : Connected, data [%d]\n",(int)data);
    if(dbus_message_is_signal(msg,"org.bluez.Headset","Disconnected"))
        HI_BT_INFO("Headset signal : Disconnected, data [%d]\n",(int)data);
	return DBUS_HANDLER_RESULT_HANDLED;
}
#endif

int connect_with_device(const char *hci_name,struct bt_device *pdev,unsigned char connect)
{
	DBusMessage *message, *reply;	
	DBusError error;
	DBusConnection *conn;

	char * adapter_path;
	char dev_path[128];
	int result=0;
	unsigned int status;

	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus\n");
		return -1;
	}
    
#if 0    
	if (!dbus_connection_add_filter(conn, headset_signal_filter, (void *)1234, NULL))
    {
		perror("Can't add signal filter\n");
    }
    else
    {
    	char match_string[256];
    	snprintf(match_string, sizeof(match_string),
    			"type='signal',"
    			"interface='org.bluez.Headset'");
        dbus_bus_add_match(conn, match_string, NULL);
    //    	snprintf(match_string,"type=signal,interface=org.bluez.Headset,member='%s'",
    //    		"AnswerRequested");
//        dbus_bus_add_match(conn, "type=signal,interface=org.bluez.Headset", NULL);
    }
#endif

	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		dbus_connection_unref(conn);
		return -1;		
	}

	memset(dev_path,0,sizeof(dev_path));
	get_dev_objpath(dev_path, adapter_path, &pdev->bdaddr);
	if(connect){
		result = get_headset_conn_status(hci_name, pdev, &status);
        if(result != 0){
		    perror("failed to get headset connect status\n");
            goto finished;
        }else if(status == 1){
            goto finished;
        }
        
		result = get_device_conn_status(hci_name,pdev,&status);
        if(result != 0){
		    perror("failed to get device connect status\n");
            goto finished;
        }else if(status == 1){
//			HI_BT_ERR("socket alreay connect,should disconnect,getuid %d\r\n",getuid());
			disconnect_with_device_admin(dev_path,conn);	
		}
        
		message = dbus_message_new_method_call("org.bluez", dev_path,
				"org.bluez.Audio",
				"Connect");
	}
	else{
        result = get_headset_conn_status(hci_name, pdev, &status);
        if(result != 0){
		    perror("failed to get headset connect status\n");
            goto finished;
        }else if(status == 0){
            goto finished;
        }
            
		message = dbus_message_new_method_call("org.bluez", dev_path,
				"org.bluez.Audio",
				"Disconnect");
    }
 
	if (message == NULL){	
		perror( "failed to alloc memory\n");
        result = -1;
        goto finished;
	}

	result = 0;
	dbus_error_init(&error);
	reply = dbus_connection_send_with_reply_and_block(conn, message,
								-1, &error);
	dbus_message_unref(message);
	if(!reply){
		if (dbus_error_is_set(&error)) {
			perror( "connect error message1: %s\n", error.message);
			dbus_error_free(&error);
		}
		if(connect){
				sleep(4);
				message = dbus_message_new_method_call("org.bluez", dev_path,
				"org.bluez.AudioSink",
				"Connect");
				if (message == NULL){	
					perror( "failed to alloc memory\n");
        				result = -1;
        				goto finished;
				}
				result = 0;
				dbus_error_init(&error);
				reply = dbus_connection_send_with_reply_and_block(conn, message,
								-1, &error);
				dbus_message_unref(message);
				if(!reply){
					if (dbus_error_is_set(&error)) {
						perror( "connect error message2: %s\n", error.message);
						dbus_error_free(&error);
					}
				} else {
					goto finished;
				}
				message = dbus_message_new_method_call("org.bluez", dev_path,
				"org.bluez.Headset",
				"Connect");
				if (message == NULL){	
					perror( "failed to alloc memory\n");
        				result = -1;
        				goto finished;
				}
				result = 0;
				dbus_error_init(&error);
				reply = dbus_connection_send_with_reply_and_block(conn, message,
								-1, &error);
				dbus_message_unref(message);
				if(!reply){
					if (dbus_error_is_set(&error)) {
						perror( "connect error message2: %s\n", error.message);
						dbus_error_free(&error);
					}
				} else {
					goto finished;
				}
		}
        result = -1;
	}
    
finished:    
	dbus_connection_unref(conn);
    HI_FREE(HI_ID_BT, adapter_path);
	return result;
}

int set_trust_device(const char *hci_name,struct bt_device *pdev)
{
	DBusConnection *conn;
	char * adapter_path;
	char dev_path[128];
	int result=0;

	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus\n");
		return -1;
	}

	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		dbus_connection_unref(conn);
		return -1;
	}

	memset(dev_path,0,sizeof(dev_path));
	get_dev_objpath(dev_path, adapter_path, &pdev->bdaddr);

	perror("%s\n",dev_path);

	result = set_device_trust_properties("org.bluez.Device",conn,dev_path,"Trusted");

	dbus_connection_unref(conn);
	HI_FREE(HI_ID_BT, adapter_path);
	return result;
}

int headset_ring(const char *hci_name,struct bt_device *pdev,unsigned char ring)
{
	DBusMessage *message, *reply;	
	DBusError error;
	DBusConnection *conn;

	char * adapter_path;
	char dev_path[128];

	conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!conn) {
		perror("Can't get on system bus");
		return -1;
	}
	
	adapter_path = get_adapter_path(conn, hci_name);
	if (!adapter_path){
		perror("failed to get %s path\n",hci_name);
		dbus_connection_unref(conn);
		return -1;		
	}

	memset(dev_path,0,sizeof(dev_path));
	get_dev_objpath(dev_path, adapter_path, &pdev->bdaddr);
	if(ring){
		message = dbus_message_new_method_call("org.bluez", dev_path,
				"org.bluez.Headset",
				"IndicateCall");
	}
	else{
		message = dbus_message_new_method_call("org.bluez", dev_path,
				"org.bluez.Headset",
				"CancelCall");
    }
	if (message == NULL){	
		perror( "failed to alloc memory\n");
		dbus_connection_unref(conn);
		return -1;
	}
	
	dbus_error_init(&error);
	reply = dbus_connection_send_with_reply_and_block(conn, message,
								-1, &error);
	dbus_message_unref(message);
	
	if(!reply){
		if (dbus_error_is_set(&error)) {
			perror( "%s\n", error.message);
			dbus_error_free(&error);
		} else
			perror( "Failed to connect to device\n");	
		dbus_connection_unref(conn);
		return -1;
	}
	//result=get_device_conn_status(hci_name,pdev,&status);
	dbus_connection_unref(conn);
	return 0;
}

