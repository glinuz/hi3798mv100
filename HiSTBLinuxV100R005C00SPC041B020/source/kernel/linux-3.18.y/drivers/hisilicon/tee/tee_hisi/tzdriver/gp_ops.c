#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/version.h>
#include <asm/memory.h>
#ifndef CONFIG_DEVCHIP_PLATFORM
modify by y00201671:ion donot supported in stb
#include <linux/hisi/hisi_ion.h>
#endif
#include <linux/ion.h>
#include "securec.h"


#include "smc.h"
#include "tee_client_constants.h"
#include "tc_ns_client.h"
#include "teek_ns_client.h"
#include "agent.h"
#include "tc_ns_log.h"
#include "gp_ops.h"
#ifdef CONFIG_TEELOG
#include "tlogger.h"
#endif

#define MAX_SHARED_SIZE 0x100000	/* 1 MiB */

#define TEEC_PARAM_TYPES(param0Type, param1Type, param2Type, param3Type) \
	((param3Type) << 12 | (param2Type) << 8 | \
	(param1Type) << 4 | (param0Type))

#define TEEC_PARAM_TYPE_GET(paramTypes, index) \
	(((paramTypes) >> (4*(index))) & 0x0F)
#define ROUND_UP(N, S) ((((N) + (S) - 1) / (S)) * (S))

struct ion_handle *drm_ion_handle;

int tc_user_param_valid(TC_NS_ClientContext *client_context, int n)
{
	TC_NS_ClientParam *client_param;
	unsigned int param_type;

	if (!client_context) {
		tloge("client_context is null.\n");
		return -EINVAL;
	}

	if ((n < 0) || (n > 3)) {
		tloge("n is invalid.\n");
		return -EINVAL;
	}

	client_param = &(client_context->params[n]);
	param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, n);
	tlogd("Param %u type is %x\n", n, param_type);

	if (TEEC_NONE == param_type) {
		tlogd("param_type is TEEC_NONE.\n");
		return 0;
	}

	/* temp buffers we need to allocate/deallocate for every
	 * operation */
	if ((TEEC_MEMREF_TEMP_INPUT == param_type) ||
	    (TEEC_MEMREF_TEMP_OUTPUT == param_type) ||
	    (TEEC_MEMREF_TEMP_INOUT  == param_type) ||
	    (TEEC_MEMREF_PARTIAL_INPUT == param_type) ||
	    (TEEC_MEMREF_PARTIAL_OUTPUT == param_type) ||
	    (TEEC_MEMREF_PARTIAL_INOUT == param_type)) {
		uint32_t size;
		/* Check the size and buffer addresses
		 * have valid userspace addresses */
		if (!access_ok(VERIFY_READ, client_param->memref.size_addr,
			       sizeof(uint32_t)))
			return -EFAULT;

		get_user(size, (uint32_t *) client_param->memref.size_addr);
		/* Check if the buffer address is valid user space address */
		if (!access_ok(VERIFY_READ, client_param->memref.buffer, size))
			return -EFAULT;
	}
	/* value */
	else if ((TEEC_VALUE_INPUT == param_type) ||
		 (TEEC_VALUE_OUTPUT == param_type) ||
		 (TEEC_VALUE_INOUT == param_type) ||
		 (TEEC_ION_INPUT == param_type)) {
		if (!access_ok
		    (VERIFY_READ, client_param->value.a_addr, sizeof(uint32_t)))
			return -EFAULT;
		if (!access_ok
		    (VERIFY_READ, client_param->value.b_addr, sizeof(uint32_t)))
			return -EFAULT;
	} else {
		tloge("param_types is not supported.\n");
		return -EFAULT;
	}
	return 0;
}

/* These 2 functions handle copying from client. Because client here can be
 * kernel client or user space client, we must use the proper copy function */
static inline int copy_from_client(void *dest, void __user *src, size_t size,
				   uint8_t kernel_api)
{
	int ret = 0;
	int s_ret = 0;

	if ((!dest) || (!src)) {
		tloge("src or dest is NULL input buffer\n");
		return -EINVAL;
	}
	if (0 == size)
		return ret;

	/* Source is kernel valid address */
#ifdef CONFIG_DEVCHIP_PLATFORM
	//TODO:XXX, virt_addr_valid is not valid for kernel modules(*.ko), by laiyingjun
	//if (kernel_api && virt_addr_valid(src)) {
	if (kernel_api) {
#else
	if (kernel_api && (virt_addr_valid(src) || vmalloc_addr_valid(src))) {
#endif
		s_ret = memcpy_s(dest, size, src, size);
		if (EOK != s_ret) {
			tloge("copy_from_client _s fail. line=%d, s_ret=%d.\n",
					__LINE__, s_ret);
			return s_ret;
		}
	}

	/* buffer is in user space(CA call TEE API) */
	else if (copy_from_user(dest, src, size))
		ret = -EFAULT;
	return ret;
}

static inline int copy_to_client(void __user *dest, void *src, size_t size,
				 uint8_t kernel_api)
{
	int ret = 0;
	int s_ret = 0;

	if ((!dest) || (!src)) {
		tloge("src or dest is NULL input buffer\n");
		return -EINVAL;
	}
	if (0 == size)
		return ret;

	/* Source is kernel valid address */
#ifdef CONFIG_DEVCHIP_PLATFORM
	if (kernel_api && virt_addr_valid(dest)) {
#else
	if (kernel_api && (virt_addr_valid(dest) || vmalloc_addr_valid(dest))) {
#endif
		s_ret = memcpy_s(dest, size, src, size);
		if (EOK != s_ret) {
			tloge("copy_to_client _s fail. line=%d, s_ret=%d.\n",
					__LINE__, s_ret);
			return s_ret;
		}
	}

	/* buffer is in user space(CA call TEE API) */
	else if (copy_to_user(dest, src, size))
		ret = -EFAULT;
	return ret;
}

static TC_NS_Operation *alloc_operation(TC_NS_DEV_File *dev_file,
					TC_NS_ClientContext *client_context,
					TC_NS_Temp_Buf local_temp_buffer[4],
					uint8_t flags)
{
	TC_NS_Operation *operation;
	TC_NS_ClientParam *client_param;
	TC_NS_Shared_MEM *shared_mem = NULL;
	ion_phys_addr_t drm_ion_phys = 0x0;
	size_t drm_ion_size = 0;
	int ret = 0;
	unsigned int param_type;
	int i;
	uint32_t buffer_size = 0;
	void *temp_buf;
	unsigned int trans_paramtype_to_tee[4];
	uint8_t kernel_params;

	if (!dev_file) {
		tloge("dev_file is null");
		return ERR_PTR(-EINVAL);
	}

	if (!client_context) {
		tloge("client_context is null");
		return ERR_PTR(-EINVAL);
	}

	if (!client_context->paramTypes)
		return NULL;

	kernel_params = ((flags & TC_CALL_LOGIN) || (dev_file->kernel_api));
	operation = kzalloc(sizeof(TC_NS_Operation), GFP_KERNEL);
	if (!operation) {
		tloge("operation malloc failed");
		return ERR_PTR(-ENOMEM);
	}
	tlogd("Allocating param types %08X\n", client_context->paramTypes);
	/* Get the 4 params from the client context */
	for (i = 0; i < 4; i++) {
		client_param = &(client_context->params[i]);

		param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
		tlogd("Param %u type is %x\n", i, param_type);
		/* temp buffers we need to allocate/deallocate for every
		 * operation */
		if ((TEEC_MEMREF_TEMP_INPUT == param_type) ||
		    (TEEC_MEMREF_TEMP_OUTPUT == param_type) ||
		    (TEEC_MEMREF_TEMP_INOUT == param_type)) {
			/* For interface compatibility sake we assume buffer
			 * size to be 32bits */
			if (copy_from_client(&buffer_size,
					     (uint32_t __user *)
					     client_param->memref.size_addr,
					     sizeof(uint32_t), kernel_params)) {
				tloge("copy memref.size_addr failed\n");
				ret = -EFAULT;
				break;
			}
			/* Don't allow unbounded malloc requests */
			if (buffer_size > MAX_SHARED_SIZE) {
				tloge("buffer_size %u from user is too large\n",
				      buffer_size);
				ret = -EFAULT;
				break;
			}
			temp_buf = (void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO,
							    get_order(ROUND_UP(buffer_size, SZ_4K)));
			/* If buffer size is zero or malloc failed */
			if (!temp_buf) {
				tloge("temp_buf malloc failed");
				ret = -ENOMEM;
				break;
			}
			if ((TEEC_MEMREF_TEMP_INPUT == param_type) ||
			    (TEEC_MEMREF_TEMP_INOUT == param_type)) {
				tlogv("client_param->memref.buffer=0x%llx\n",
					  client_param->memref.buffer);
				/* Kernel side buffer */
				if (copy_from_client(temp_buf,
						     (void *)
						     client_param->memref.
						     buffer, buffer_size,
						     kernel_params)) {
					tloge("copy memref.buffer failed\n");
					ret = -EFAULT;
					break;
				}
			}
			operation->params[i].memref.buffer = virt_to_phys((void *)temp_buf);
			operation->buffer_h_addr[i] = virt_to_phys((void *)temp_buf) >> 32;
			local_temp_buffer[i].temp_buffer = temp_buf;
			local_temp_buffer[i].size = buffer_size;
			operation->params[i].memref.size = buffer_size;
			/*TEEC_MEMREF_TEMP_INPUT equal
			 * to TEE_PARAM_TYPE_MEMREF_INPUT*/
			trans_paramtype_to_tee[i] = param_type;
		}
		/* MEMREF_PARTIAL buffers are already allocated so we just need
		 * to search for the shared_mem ref */
		else if ((TEEC_MEMREF_PARTIAL_INPUT == param_type) ||
			 (TEEC_MEMREF_PARTIAL_OUTPUT == param_type) ||
			 (TEEC_MEMREF_PARTIAL_INOUT == param_type)) {
			/* For interface compatibility sake we assume buffer
			 * size to be 32bits */
			if (copy_from_client(&buffer_size,
					     (uint32_t __user *)
					     client_param->memref.size_addr,
					     sizeof(buffer_size),
					     kernel_params)) {
				tloge("copy memref.size_addr failed\n");
				ret = -EFAULT;
				break;
			}
			if (!buffer_size) {
				tloge("buffer_size from user is 0\n");
				ret = -ENOMEM;
				break;
			}
			operation->params[i].memref.buffer = 0;
			/* find kernel addr refered to user addr */
			list_for_each_entry(shared_mem,
					    &dev_file->shared_mem_list, head) {
				if (shared_mem->user_addr ==
				    (void *)client_param->memref.buffer) {
					/* arbitrary CA can control offset by ioctl, so in here
					 * offset must be checked, and avoid integer overflow. */
					if (((shared_mem->len - client_param->memref.offset) >= buffer_size)
					    && (shared_mem->len > client_param->memref.offset)) {
						operation->params[i].memref.buffer =
							virt_to_phys((void *)shared_mem->kernel_addr
								     + client_param->memref.offset);
						operation->buffer_h_addr[i] =
							virt_to_phys((void *)shared_mem->kernel_addr
								     + client_param->memref.offset) >> 32;
					} else {
						tloge("Unexpected size %u vs %u",
						      shared_mem->len,
						      buffer_size);
					}
					break;
				}
			}
			/* for 8G physical memory device, there is a chance that
			 * operation->params[i].memref.buffer could be all 0,
			 * buffer_h_addr cannot be 0 in the same time. */
			if ((!operation->params[i].memref.buffer)
			    && (!operation->buffer_h_addr[i])) {
				tloge("can not find shared buffer, exit\n");
				ret = -EINVAL;
				break;
			}
			operation->params[i].memref.size = buffer_size;
			/* TEEC_MEMREF_PARTIAL_INPUT -
			 * TEE_PARAM_TYPE_MEMREF_INPUT = 8 */
			trans_paramtype_to_tee[i] =
				param_type - (TEEC_MEMREF_PARTIAL_INPUT -
					      TEE_PARAM_TYPE_MEMREF_INPUT);
		}
		/* value */
		else if ((TEEC_VALUE_INPUT == param_type) ||
			 (TEEC_VALUE_OUTPUT == param_type) ||
			 (TEEC_VALUE_INOUT == param_type)) {
			if (copy_from_client(&operation->params[i].value.a,
					     client_param->value.a_addr,
					     sizeof(operation->params[i].
						    value.a), kernel_params)) {
				tloge("copy value.a_addr failed\n");
				ret = -EFAULT;
				break;
			}
			if (copy_from_client(&operation->params[i].value.b,
					     client_param->value.b_addr,
					     sizeof(operation->params[i].
						    value.b), kernel_params)) {
				tloge("copy value.b_addr failed\n");
				ret = -EFAULT;
				break;
			}
			/* TEEC_VALUE_INPUT equal
			 * to TEE_PARAM_TYPE_VALUE_INPUT */
			trans_paramtype_to_tee[i] = param_type;
		}
		/*ion*/
		else if (TEEC_ION_INPUT == param_type) {
			if (copy_from_client(&operation->params[i].value.a,
					     client_param->value.a_addr,
					     sizeof(operation->params[i].value.a),
					     kernel_params)) {
				tloge("value.a_addr copy failed\n");
				ret = -EFAULT;
				break;
			}

			if (copy_from_client(&operation->params[i].value.b,
					     client_param->value.b_addr,
					     sizeof(operation->params[i].value.b),
					     kernel_params)) {
				tloge("value.b_addr copy failed\n");
				ret = -EFAULT;
				break;
			}

			if ((int)operation->params[i].value.a >= 0) {
				unsigned int ion_shared_fd =
					operation->params[i].value.a;
				drm_ion_handle =
					ion_import_dma_buf(drm_ion_client,
							   ion_shared_fd);
				if (IS_ERR(drm_ion_handle)) {
					tloge("in %s err: client=%p handle=%p fd=%d\n",
					      __func__, drm_ion_client,
					      drm_ion_handle, ion_shared_fd);
					ret = -EFAULT;
					break;
				}


				ret = ion_phys(drm_ion_client, drm_ion_handle,
					       &drm_ion_phys, &drm_ion_size);

				if (ret) {
					tloge("in %s err:ret=%d client=%p handle=%p fd=%d\n",
					      __func__, ret, drm_ion_client,
					      drm_ion_handle, ion_shared_fd);
					ret = -EFAULT;
					break;
				}

				if (drm_ion_size > operation->params[i].value.b)
					drm_ion_size = operation->params[i].value.b;

				operation->params[i].memref.buffer =
					(unsigned int)drm_ion_phys;
				operation->params[i].memref.size =
					(unsigned int)drm_ion_size;
				trans_paramtype_to_tee[i] = param_type;
				ion_free(drm_ion_client, drm_ion_handle);
				drm_ion_handle = NULL;
			} else {
				tloge("in %s err: drm ion handle invaild!\n", __func__);
				ret = -EFAULT;
				break;
			}
		} else {
			tlogd("param_type = TEEC_NONE\n");
			trans_paramtype_to_tee[i] = TEE_PARAM_TYPE_NONE;
		}
	}
	if (!ret) {
		operation->paramTypes =
			TEEC_PARAM_TYPES(trans_paramtype_to_tee[0],
					 trans_paramtype_to_tee[1],
					 trans_paramtype_to_tee[2],
					 trans_paramtype_to_tee[3]);
		return operation;
	}

	kfree(operation);
	return ERR_PTR(ret);
}

static int update_client_operation(TC_NS_DEV_File *dev_file,
				   TC_NS_ClientContext *client_context,
				   TC_NS_Operation *operation,
				   TC_NS_Temp_Buf local_temp_buffer[4],
				   bool incomplete)
{
	TC_NS_ClientParam *client_param;
	int ret = 0;
	unsigned int param_type;
	int i;
	uint32_t buffer_size = 0;

	if (!dev_file) {
		tloge("dev_file is null");
		return -EINVAL;
	}

	if (!client_context) {
		tloge("client_context is null");
		return -EINVAL;
	}

	/*if paramTypes is NULL, no need to update*/
	if (!client_context->paramTypes)
		return 0;

	for (i = 0; i < 4; i++) {
		client_param = &(client_context->params[i]);
		param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
		if ((TEEC_MEMREF_TEMP_OUTPUT == param_type) ||
		    (TEEC_MEMREF_TEMP_INOUT == param_type)) {
			/* temp buffer */
			buffer_size = operation->params[i].memref.size;
			/* Size is updated all the time */
			if (copy_to_client
			    ((void *)client_param->memref.size_addr,
			     &buffer_size, sizeof(buffer_size),
			     dev_file->kernel_api)) {
				tloge("copy tempbuf size failed\n");
				ret = -EFAULT;
				break;
			}

			/* Only update the buffer is the buffer size is valid in
			 * incomplete case, otherwise see next param */
			if (buffer_size > local_temp_buffer[i].size) {
				if (!incomplete)
					continue;
				else {
					tloge("client_param->memref.size has been changed larger than the initial\n");
					ret = -EFAULT;
				}
				break;
			}

			if (copy_to_client((void *)client_param->memref.buffer,
					   local_temp_buffer[i].temp_buffer,
					   operation->params[i].memref.size,
					   dev_file->kernel_api)) {
				tloge("copy tempbuf failed\n");
				ret = -ENOMEM;
				break;
			}

		} else if ((TEEC_MEMREF_PARTIAL_OUTPUT == param_type) ||
			   (TEEC_MEMREF_PARTIAL_INOUT == param_type)) {
			/* update size */
			buffer_size = operation->params[i].memref.size;
			if (copy_to_client
			    ((void *)client_param->memref.size_addr,
			     &buffer_size, sizeof(buffer_size),
			     dev_file->kernel_api)) {
				tloge("copy buf size failed\n");
				ret = -EFAULT;
				break;
			}
		} else if (incomplete && ((TEEC_VALUE_OUTPUT == param_type) ||
					  (TEEC_VALUE_INOUT == param_type))) {
			/* value */
			if (copy_to_client(client_param->value.a_addr,
					   &operation->params[i].value.a,
					   sizeof(operation->params[i].value.a),
					   dev_file->kernel_api)) {
				tloge("inc copy value.a_addr failed\n");
				ret = -EFAULT;
				break;
			}
			if (copy_to_client(client_param->value.b_addr,
					   &operation->params[i].value.b,
					   sizeof(operation->params[i].value.b),
					   dev_file->kernel_api)) {
				tloge("inc copy value.b_addr failed\n");
				ret = -EFAULT;
				break;
			}
		} else {
			tlogd("param_type:%d don't need to update.\n", param_type);
		}
	}
	return ret;
}

static int free_operation(TC_NS_ClientContext *client_context,
			  TC_NS_Operation *operation,
			  TC_NS_Temp_Buf local_temp_buffer[4])
{
	int ret = 0;
	unsigned int param_type;
	int i;
	void *temp_buf;

	if (!operation)
		return -EFAULT;

	if (!client_context)
		return -EFAULT;

	for (i = 0; i < 4; i++) {
		param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
		if ((TEEC_MEMREF_TEMP_INPUT == param_type) ||
		    (TEEC_MEMREF_TEMP_OUTPUT == param_type) ||
		    (TEEC_MEMREF_TEMP_INOUT == param_type)) {
			/* free temp buffer */
			/* TODO: this is all sorts of bad */
			temp_buf = local_temp_buffer[i].temp_buffer;
			tlogd("Free temp buf %p\n", temp_buf);
			if (virt_addr_valid(temp_buf) &&
			    !ZERO_OR_NULL_PTR(temp_buf))
				free_pages((unsigned long)temp_buf,
					   get_order(ROUND_UP
						     (local_temp_buffer[i].size,
						      SZ_4K)));
		}
	}
	kfree(operation);
	return ret;
}

unsigned char g_ca_auth_hash_buf[MAX_SHA_256_SZ];
int tc_client_call(TC_NS_ClientContext *client_context,
		   TC_NS_DEV_File *dev_file, uint8_t flags)
{
	int ret = 0;
	unsigned int tee_ret = 0;
	int s_ret = 0;
	TC_NS_Operation *operation = NULL;
	TC_NS_SMC_CMD *smc_cmd = NULL;
	TC_NS_Session *session = NULL;
	TC_NS_Service *service = NULL;
	struct TC_wait_data *wq = NULL;
	TC_NS_Temp_Buf local_temp_buffer[4] = {{0, 0}, {0, 0}, {0, 0}, {0, 0} };
	unsigned char uuid[17];
	bool global = flags & TC_CALL_GLOBAL;
	uint32_t uid;
	unsigned char *hash_buf = NULL;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0))
	kuid_t kuid;

	kuid = current_uid(); /*lint !e666 */
	uid = kuid.val;
#else
	uid = current_uid();
#endif

	if (!dev_file) {
		tloge("dev_file is null");
		return -EINVAL;
	}
	if (!client_context) {
		tloge("client_context is null");
		return -EINVAL;
	}

	smc_cmd = kzalloc(sizeof(TC_NS_SMC_CMD), GFP_KERNEL);
	if (!smc_cmd) {
		tloge("smc_cmd malloc failed");
		return -ENOMEM;
	}

	tlogd("Calling command %08x\n", client_context->cmd_id);

	if (client_context->paramTypes != 0) {
		operation = alloc_operation(dev_file, client_context,
					    local_temp_buffer, flags);
		if (IS_ERR_OR_NULL(operation)) {
			ret = PTR_ERR(operation);
			operation = NULL;
			tloge("alloc_operation malloc failed");
			goto error;
		}
	}

	uuid[0] = ((global == true) ? 1 : 0);
	s_ret = memcpy_s(uuid + 1, 16, client_context->uuid, 16);
	if (EOK != s_ret) {
		tloge("alloc_operation _s error.\n");
		goto error;
	}
	smc_cmd->uuid_phys = virt_to_phys((void *)uuid);
	smc_cmd->uuid_h_phys = virt_to_phys((void *)uuid) >> 32;
	smc_cmd->cmd_id = client_context->cmd_id;
	smc_cmd->dev_file_id = dev_file->dev_file_id;
	smc_cmd->context_id = client_context->session_id;
	smc_cmd->err_origin = 0;
	smc_cmd->started = client_context->started;
	smc_cmd->uid = uid;
	tlogv("current uid is %d\n", smc_cmd->uid);
	tlogv("operation start is :%d\n", smc_cmd->started);
	if (operation) {
		smc_cmd->operation_phys = virt_to_phys(operation);
		smc_cmd->operation_h_phys = virt_to_phys(operation) >> 32;
	} else {
		smc_cmd->operation_phys = 0;
		smc_cmd->operation_h_phys = 0;
	}

	smc_cmd->login_method = client_context->login.method;
	if (sizeof(uint32_t) == dev_file->pub_key_len &&
	    GLOBAL_CMD_ID_OPEN_SESSION == smc_cmd->cmd_id &&
	    (current->mm != NULL)) {
		hash_buf = g_ca_auth_hash_buf;
		smc_cmd->login_data_phy = virt_to_phys(hash_buf);
		smc_cmd->login_data_h_addr = virt_to_phys(hash_buf) >> 32;
		smc_cmd->login_data_len = MAX_SHA_256_SZ;
	} else {
		smc_cmd->login_data_phy = 0;
		smc_cmd->login_data_h_addr = 0;
		smc_cmd->login_data_len = 0;
	}

	/* send smc to secure world */
	tee_ret = TC_NS_SMC(smc_cmd, flags);

	client_context->session_id = smc_cmd->context_id;

	if (tee_ret != 0) {
#ifdef TC_ASYNC_NOTIFY_SUPPORT
		while (TEEC_PENDING == tee_ret) {
			mutex_lock(&dev_file->service_lock);
			service = tc_find_service(&dev_file->services_list,
					client_context->uuid);
			if (service) {
				session = tc_find_session
					(&service->session_list,
					 client_context->session_id);
				if (session)
					wq = &session->wait_data;
			}
			mutex_unlock(&dev_file->service_lock);

			if (wq) {
				tlogv("before wait event\n");
				if (wait_event_interruptible
						(wq->send_cmd_wq,
						 wq->send_wait_flag)) {
					tlogd("wait event error\n");
				}
				wq->send_wait_flag = 0;
			}

			tlogv("operation start is :%d\n",
					smc_cmd->started);
			tee_ret = TC_NS_SMC_WITH_NO_NR(smc_cmd, flags);
		}
#endif
		/* Client was interrupted, return and let it handle it's own
		 * signals first then retry */
		if (TEEC_CLIENT_INTR == tee_ret) {
			ret = -ERESTARTSYS;
			goto error;
		} else if (tee_ret) {
			tloge("smc_call returns error ret 0x%x\n", tee_ret);
			tloge("smc_call smc cmd ret 0x%x\n", smc_cmd->ret_val);
			goto error1;
		}

		client_context->session_id = smc_cmd->context_id;
	}

	/* wake_up tee log reader */
#ifdef CONFIG_TEELOG
	tz_log_write();
#endif

	if (operation) {
		ret = update_client_operation(dev_file, client_context,
					      operation, local_temp_buffer,
					      true);
		if (ret)
			goto error;
	}

	ret = 0;
	goto error;

error1:
	if (TEEC_ERROR_SHORT_BUFFER == tee_ret) {
		/* update size */
		if (operation) {
			ret = update_client_operation(dev_file, client_context,
						      operation,
						      local_temp_buffer,
						      false);
			if (ret)
				goto error;
		}
	}

	client_context->returns.code = tee_ret;
	client_context->returns.origin = smc_cmd->err_origin;
	ret = EFAULT;
error:
	/* kfree(NULL) is safe and this check is probably not required*/
	kfree(smc_cmd);

	if (operation)
		free_operation(client_context, operation, local_temp_buffer);

	return ret;
}
