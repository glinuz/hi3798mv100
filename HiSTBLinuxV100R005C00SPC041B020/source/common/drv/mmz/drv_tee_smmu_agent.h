#ifndef _DRV_TEE_SMMU_AGENT_H
#define _DRV_TEE_SMMU_AGENT_H

#ifdef CONFIG_TZDRIVER

int smmu_agent_register(void);

int smmu_agent_unregister(void);

#else
static inline int smmu_agent_register(void)
{
	return 0;
}

static inline int smmu_agent_unregister(void)
{
	return 0;
}

#endif






#endif
