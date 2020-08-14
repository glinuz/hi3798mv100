#include <linux/fs.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/kthread.h>

#include "tee_client_constants.h"
#include "teek_ns_client.h"
#include "agent.h"
#include "mem.h"

#include "drv_media_mem.h"
#include "drv_mmz.h"
#include "drv_tzsmmu.h"

#include "hi_debug.h"

typedef enum{
	HISI_MEM_ALLOC = 0x0,
	HISI_MEM_FREE,
	HISI_MEM_GET_MEMINFO,
	HISI_MEM_PUT_MEMINFO,
	HISI_AGENT_CLOSE,
} smmu_cmd_t;

struct smmu_ctrl_t{
	smmu_cmd_t cmd;
	int memtype; //0:cma mem; 1: system mem;
	u32 size;
	char name[HIL_MAX_NAME_LEN];
	u32	phys_addr;
	u32	normal_smmu;
	u32	sec_smmu;
	u32 tz_mblock_phys;
};

#define CLOSE_AGENT	0x12

static int smmu_agent_init(struct tee_agent_kernel_ops* agent_instance)
{
	return 0;
}

static int smmu_agent_exit(struct tee_agent_kernel_ops* agent_instance)
{
	return 0;
}

static int smmu_tee_agent_work(void *data)
{
	int ret = 0;
	int agent_station = 0;
	struct tee_agent_kernel_ops *agent_instance =
			(struct tee_agent_kernel_ops *)data;


	while (!kthread_should_stop()) {
		ret = TC_NS_wait_event(agent_instance->agent_id);
		if (ret) {
			HI_PRINT("%s wait event fail\n", agent_instance->agent_name);
			break;
		}

		if (agent_instance->tee_agent_work)
			agent_station = agent_instance->tee_agent_work(agent_instance);

		ret = TC_NS_send_event_response(agent_instance->agent_id);
		if (ret) {
			HI_PRINT("%s send event response fail\n", agent_instance->agent_name);
			break;
		}

		if (CLOSE_AGENT == agent_station) {
			/*
			 * agent will be closed.
			 **/
			break;
		}
	}

	return ret;
}

#define AGENT_BUFF_SIZE (4*1024)
static int smmu_tee_agent_run(struct tee_agent_kernel_ops* agent_instance)
{
	TC_NS_Shared_MEM *shared_mem = NULL;
	TC_NS_DEV_File dev;
	int ret;

	/*1. Allocate agent buffer */
	shared_mem = tc_mem_allocate(&dev, AGENT_BUFF_SIZE);
	if (IS_ERR(shared_mem)) {
		HI_PRINT("allocate agent buffer fail\n");
		ret = PTR_ERR(shared_mem);
		goto out;
	}
	agent_instance->agent_buffer = shared_mem;

	/*2. Register agent buffer to TEE */
	ret = TC_NS_register_agent(&dev, agent_instance->agent_id, shared_mem);
	if (ret) {
		HI_PRINT("register agent buffer fail\n");
		ret = -1;
		goto out;
	}

	/*3. Creat thread to run agent */
	agent_instance->agent_thread
		= kthread_run(smmu_tee_agent_work, (void *)agent_instance, "agent_%s", agent_instance->agent_name);
	if (IS_ERR(agent_instance->agent_thread)) {
		HI_PRINT("kthread creat fail\n");
		ret = PTR_ERR(agent_instance->agent_thread);
		agent_instance->agent_thread = NULL;
		goto out;
	}
	return 0;

out:
	if(!IS_ERR_OR_NULL(shared_mem))
		tc_mem_free(shared_mem);
	if (!IS_ERR_OR_NULL(agent_instance->agent_thread)) {
		kthread_stop(agent_instance->agent_thread);
		agent_instance->agent_thread = NULL;
	}

	return ret;
}

static int smmu_agent_work(struct tee_agent_kernel_ops* agent_instance)
{
	struct smmu_ctrl_t *smmu_ctrl = (struct smmu_ctrl_t *)agent_instance->agent_buffer->kernel_addr;
	mmb_addr_t addr;
	int iommu;
	int ret = 0;

	if (!smmu_ctrl) {
		HI_PRINT("%s :system err!\n", __func__);
		return -1;
	}

	switch (smmu_ctrl->cmd) {
	case HISI_MEM_ALLOC:
	{
		if (smmu_ctrl->memtype) {
			addr = new_mmb(smmu_ctrl->name, smmu_ctrl->size, 0x1000, "iommu");
			smmu_ctrl->normal_smmu = addr;
			smmu_ctrl->phys_addr = 0;
			iommu = 1;
		} else {
			addr = new_mmb(smmu_ctrl->name, smmu_ctrl->size, 0x1000, NULL);
			smmu_ctrl->normal_smmu = 0;
			smmu_ctrl->phys_addr = addr;
			iommu = 0;
		}
		if (MMB_ADDR_INVALID == addr) {
			HI_PRINT("%s alloc mem failed!\n", __func__);
			return -1;
		}

		smmu_ctrl->tz_mblock_phys = hi_tee_smmumem_into_ta(addr, iommu); 
		if (!smmu_ctrl->tz_mblock_phys) {
			HI_PRINT("%s : hi_tee_smmumem_into_ta failed!\n", __func__);
			delete_mmb(addr, iommu);
			return -1;
		}

		set_sec_mmb_flag(addr, iommu);
		sec_mmb_get(addr, iommu, 0x0);

		ret = 0;
		break;
	}
	case HISI_MEM_FREE:
	{
		if (smmu_ctrl->memtype) {
			iommu = 1;
			addr = smmu_ctrl->normal_smmu; 
		} else {
			iommu = 0;
			addr = smmu_ctrl->phys_addr;
		}
		clr_sec_mmb_flag(addr, iommu);
		if (!sec_mmb_put(addr, iommu)) {
			hi_tee_smmumem_outfrom_ta(smmu_ctrl->tz_mblock_phys);
			delete_mmb(addr, iommu);
		}

		ret = 0;
		break;
	}
	case HISI_MEM_GET_MEMINFO:
	{
		if (smmu_ctrl->memtype) {
			iommu = 1;
			smmu_ctrl->tz_mblock_phys = hi_tee_smmumem_into_ta(smmu_ctrl->normal_smmu, 1);
			addr = smmu_ctrl->normal_smmu;
		} else {
			iommu = 0;
			smmu_ctrl->tz_mblock_phys = hi_tee_smmumem_into_ta(smmu_ctrl->phys_addr, 0);
			addr = smmu_ctrl->phys_addr;
		}
		sec_mmb_get(addr, iommu, 0x0);

		ret = 0;
		break;
	}
	case HISI_MEM_PUT_MEMINFO:
	{
		if (smmu_ctrl->memtype) {
			iommu = 1;
			addr = smmu_ctrl->normal_smmu;
		} else {
			iommu = 0;
			addr = smmu_ctrl->phys_addr;
		}

		if (!sec_mmb_put(addr, iommu))
			hi_tee_smmumem_outfrom_ta(smmu_ctrl->tz_mblock_phys);

		ret = 0;
		break;
	}
	case HISI_AGENT_CLOSE:
	{
		ret = CLOSE_AGENT;
		break;
	}
	default :
	{
		HI_PRINT("%s cmd err!\n", __func__);
		ret = -1;
		return ret;
	}
	}
	return ret;
}

static int smmu_tee_agent_init(struct tee_agent_kernel_ops* agent_instance)
{
	int ret;

	if ((NULL == agent_instance) || (agent_instance->agent_id == 0) ||
				(agent_instance->agent_name == NULL) ||
				(agent_instance->tee_agent_work == NULL)) {
		HI_PRINT("agent is invalid\n");
		return -1;;
	}

	/* Initialize the agent */
	if (agent_instance->tee_agent_init)
		ret = agent_instance->tee_agent_init(agent_instance);
	else {
		HI_PRINT("tee_agent_init %s should not be null\n", agent_instance->agent_name);
		return -1;
	}

	if (ret) {
		HI_PRINT("tee_agent_init %s failed\n", agent_instance->agent_name);
		return -1;
	}

	/* Run the agent */
	if (agent_instance->tee_agent_run)
		ret = agent_instance->tee_agent_run(agent_instance);
	else {
		HI_PRINT("tee_agent_run %s should not be null\n", agent_instance->agent_name);
		return -1;
	}

	if (ret) {
		HI_PRINT("tee_agent_run %s failed\n", agent_instance->agent_name);
		if (agent_instance->tee_agent_exit)
			agent_instance->tee_agent_exit(agent_instance);
		return -1;;
	}

	return 0;

}

static int smmu_agent_stop(struct tee_agent_kernel_ops* agent_instance)
{
	int ret;

	/*
	 * make agent in secure os to send message to agent in
	 * no secure world to stop wait event, and stop the thread
	 */
	ret = hi_tee_agent_end();
	if (ret) {
		HI_PRINT("%s  close agend failed!\n", __func__);
		return -1;
	}
//	TC_NS_send_event_reponse(agent_instance->agent_id);
	TC_NS_unregister_agent(agent_instance->agent_id);
	if (!IS_ERR_OR_NULL(agent_instance->agent_thread)) {
		kthread_stop(agent_instance->agent_thread);
	}
	return 0;
}

static struct tee_agent_kernel_ops smmu_agent_ops = {
	.agent_name = "smmu",
	.agent_id = 0x1234,	//to do
	.tee_agent_init = smmu_agent_init,
	.tee_agent_run = smmu_tee_agent_run,
	.tee_agent_work = smmu_agent_work,
	.tee_agent_exit = smmu_agent_exit,
	.tee_agent_stop = smmu_agent_stop,
	.list = LIST_HEAD_INIT(smmu_agent_ops.list)
};

int smmu_agent_register(void)
{
	tee_agent_kernel_register(&smmu_agent_ops);

	smmu_tee_agent_init(&smmu_agent_ops);

	return 0;
}

int smmu_agent_unregister(void)
{
	int ret = 0;
	struct tee_agent_kernel_ops* agent_instance = &smmu_agent_ops;

	if (agent_instance->tee_agent_exit)
		ret = agent_instance->tee_agent_exit(agent_instance);
	else {
		HI_PRINT("tee_agent_init %s should not be null\n", agent_instance->agent_name);
		return -1;
	}

	if (agent_instance->tee_agent_stop)
		agent_instance->tee_agent_stop(agent_instance);

	return 0;

}
