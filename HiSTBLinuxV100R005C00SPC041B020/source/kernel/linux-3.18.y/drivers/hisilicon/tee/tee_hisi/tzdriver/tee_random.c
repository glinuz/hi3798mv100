#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include "teek_client_api.h"
#include "libhwsecurec/securec.h"

#define TEE_SERVICE_MDPP \
{ \
	0x41d21d0f, \
	0x9757, \
	0x41f1, \
	{ \
		0x87, 0x06, 0xb3, 0xba, 0xa3, 0x5f, 0xe8, 0x2f \
	} \
}

enum SERVICE_MDPP_CMD {
	MDPP_CMD_GET_RANDOM,
	MDPP_CMD_WRAP_KEY,
	MDPP_CMD_CC_ENTROPY_TEST = 0xf0,
};

struct tee_random_data {
	TEEC_Session session;
};

static TEEC_Context mdpp_ctx;

static int init_mdpp_context(TEEC_Context *ctx)
{
	TEEC_Result ret;

	ret = TEEK_InitializeContext(NULL, ctx);
	if (ret != TEEC_SUCCESS) {
		pr_err("init mdpp context failed, ret=0x%x\n", ret);
		return -EBUSY;
	}

	return TEEC_SUCCESS;
}

static void destroy_mdpp_context(TEEC_Context *ctx)
{
	TEEK_FinalizeContext(ctx);
}

static int open_tee_random_session(TEEC_Session *session)
{
	TEEC_UUID mdpp_uuid = TEE_SERVICE_MDPP;
	int ret = TEEC_ERROR_GENERIC;
	TEEC_Operation operation = {0};
	u8 package_name[] = "tee_random";
	u32 root_id = 0;

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_INPUT);
	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = sizeof(root_id);
	operation.params[3].tmpref.buffer = (void *)(package_name);
	operation.params[3].tmpref.size = strlen(package_name) + 1;

	ret = TEEK_OpenSession(
		      &mdpp_ctx,
		      session,
		      &mdpp_uuid,
		      TEEC_LOGIN_IDENTIFY,
		      NULL,
		      &operation,
		      NULL);
	if (ret != TEEC_SUCCESS) {
		pr_err("open mdpp session failed, ret=0x%x\n", ret);
		return -EBUSY;
	}

	return TEEC_SUCCESS;
}

static int tee_random_open(struct inode *inode, struct file *file)
{
	struct tee_random_data *data;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	if (open_tee_random_session(&data->session)) {
		kfree(data);
		return -EBUSY;
	}

	file->private_data = data;

	return TEEC_SUCCESS;
}

static int tee_random_close(struct inode *inode, struct file *file)
{
	struct tee_random_data *data = file->private_data;

	TEEK_CloseSession(&data->session);
	kfree(data);

	return TEEC_SUCCESS;
}

static ssize_t __get_tee_random_bytes(TEEC_Session *session, void *buf,
				  int nbytes, int from_user)
{
	TEEC_Operation operation = { 0 };
	TEEC_Result ret = TEEC_SUCCESS;
	ssize_t count = 0, ret_val = 0, nleft = 0;
	char *random_buffer = NULL;
	size_t random_len = 0;
	uint32_t cmd_id = 0;

	if (!buf || 0 == nbytes)
		return 0;

	random_len = nbytes > 4096 ? 4096 : nbytes;
	random_buffer = kmalloc(random_len, GFP_KERNEL);
	if (!random_buffer) {
		pr_err("alloc random buffer failed\n");
		return -ENOMEM;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_TEMP_OUTPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].tmpref.buffer = (void *)random_buffer;

	cmd_id = MDPP_CMD_GET_RANDOM;

	nleft = nbytes;
	while (nleft) {
		random_len = nleft > 4096 ? 4096 : nleft;
		operation.params[0].tmpref.size = random_len;

		ret = TEEK_InvokeCommand(
			      session,
			      cmd_id,
			      &operation,
			      NULL);
		if (ret != TEEC_SUCCESS) {
			pr_err("invoke tee random failed, ret=0x%x\n", ret);
			ret_val = -EBUSY;
			break;
		}

		if (from_user) {
			if (copy_to_user(buf + count, random_buffer,
					 random_len)) {
				ret_val = -ENOMEM;
				break;
			}
		} else {
			if (memcpy_s(buf + count, nleft, random_buffer,
					random_len)) {
				ret_val = -ENOMEM;
				break;
			}
		}

		count += random_len;

		if (nleft <= random_len)
			break;

		nleft -= random_len;
		pr_debug("get tee random %lu, remain %lu, total %d\n",
			 count, nleft, nbytes);
	}

	kfree(random_buffer);

	return ret_val ? ret_val : count;
}

static ssize_t
tee_random_read(struct file *file, char __user *buf, size_t nbytes,
		loff_t *ppos)
{
	struct tee_random_data *data = file->private_data;
	TEEC_Session *session = &data->session;

	return __get_tee_random_bytes(session, buf, nbytes, 1);
}

ssize_t get_tee_random_bytes(void *buf, int nbytes)
{
	TEEC_Session session;
	ssize_t ret;

	if (open_tee_random_session(&session))
		return -EBUSY;

	ret = __get_tee_random_bytes(&session, buf, nbytes, 0);

	TEEK_CloseSession(&session);

	return ret;
}
EXPORT_SYMBOL(get_tee_random_bytes);

const struct file_operations tee_random_fops = {
	.open = tee_random_open,
	.release = tee_random_close,
	.read  = tee_random_read,
};

static int tee_dev_major;
static struct class *tee_random_class;
static struct device *tee_random_dev;
#define TEE_RANDOM_NAME "tee_random"

static __init int tee_random_init(void)
{
	int ret = 0;
	int major = 0;

	major = register_chrdev(0, TEE_RANDOM_NAME, &tee_random_fops);
	if (major <= 0) {
		pr_err("unable to get major for tee random devs.\n");
		return -EFAULT;
	}

	tee_random_class = class_create(THIS_MODULE, TEE_RANDOM_NAME);
	if (IS_ERR(tee_random_class)) {
		ret = -EFAULT;
		pr_err("tee random class_create error.\n");
		goto error1;
	}

	tee_random_dev = device_create(tee_random_class, NULL,
				       MKDEV(major, 0), NULL, TEE_RANDOM_NAME);
	if (IS_ERR(tee_random_dev)) {
		ret = -EFAULT;
		pr_err("tee random device_create error.\n");
		goto error2;
	}

	ret = init_mdpp_context(&mdpp_ctx);
	if (ret) {
		pr_err("tee random failed init tee context\n");
		goto error3;
	}

	tee_dev_major = major;

	return 0;

error3:
	device_destroy(tee_random_class, MKDEV(major, 0));
	tee_random_dev = NULL;
error2:
	class_destroy(tee_random_class);
	tee_random_class = NULL;
error1:
	unregister_chrdev(major, TEE_RANDOM_NAME);
	return ret;
}

static void tee_random_exit(void)
{
	destroy_mdpp_context(&mdpp_ctx);
	device_destroy(tee_random_class, MKDEV(tee_dev_major, 0));
	tee_random_dev = NULL;
	class_destroy(tee_random_class);
	tee_random_class = NULL;
	unregister_chrdev(tee_dev_major, TEE_RANDOM_NAME);
}

late_initcall(tee_random_init);
module_exit(tee_random_exit);

MODULE_DESCRIPTION("TEE random driver");
MODULE_LICENSE("GPL");
