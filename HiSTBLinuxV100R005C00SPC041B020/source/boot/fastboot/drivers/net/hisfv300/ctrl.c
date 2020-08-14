
#define HIETH_SFV300

#include "hieth.h"
#include "ctrl.h"

static inline int _hieth_irq_enable(struct hieth_netdev_local *ld, int irqs)
{
	int old;

	old = hieth_readl(ld, GLB_RW_IRQ_ENA);

	hieth_writel(ld, old | irqs, GLB_RW_IRQ_ENA);

	return old;
}

static inline int _hieth_irq_disable(struct hieth_netdev_local *ld, int irqs)
{
	int old;

	old = hieth_readl(ld, GLB_RW_IRQ_ENA);

	hieth_writel(ld, old & (~irqs), GLB_RW_IRQ_ENA);

	return old;
}

static inline int _hieth_read_irqstatus(struct hieth_netdev_local *ld)
{
	int status;

	status = hieth_readl(ld, GLB_RO_IRQ_STAT);

	return status;
}

static inline int _test_xmit_queue_ready(struct hieth_netdev_local *ld)
{
	return hieth_readl_bits(ld, UD_REG_NAME(GLB_RO_QUEUE_STAT),
				BITS_XMITQ_RDY);
}

static inline int _test_recv_queue_ready(struct hieth_netdev_local *ld)
{
	return hieth_readl_bits(ld, UD_REG_NAME(GLB_RO_QUEUE_STAT),
				BITS_RECVQ_RDY);
}

int hieth_irq_enable(struct hieth_netdev_local *ld, int irqs)
{
	int old;

	local_lock(ld);
	old = _hieth_irq_enable(ld, irqs);
	local_unlock(ld);

	return old;
}

int hieth_irq_disable(struct hieth_netdev_local *ld, int irqs)
{
	int old;

	local_lock(ld);
	old = _hieth_irq_disable(ld, irqs);
	local_unlock(ld);

	return old;
}

int hieth_read_irqstatus(struct hieth_netdev_local *ld)
{
	int status;

	local_lock(ld);
	status = _hieth_read_irqstatus(ld);
	local_unlock(ld);

	return status;
}

int hieth_read_raw_irqstatus(struct hieth_netdev_local *ld)
{
	int status;

	local_lock(ld);
	status = hieth_readl(ld, GLB_RO_IRQ_STAT);
	local_unlock(ld);

	return status;
}

int hieth_clear_irqstatus(struct hieth_netdev_local *ld, int irqs)
{
	int status;

	local_lock(ld);
	hieth_writel(ld, irqs, GLB_RW_IRQ_RAW);
	status = _hieth_read_irqstatus(ld);
	local_unlock(ld);

	return status;
}

int hieth_set_endian_mode(struct hieth_netdev_local *ld, int mode)
{
	int old;

	local_lock(ld);
	old = hieth_readl_bits(ld, GLB_ENDIAN_MOD, BITS_ENDIAN);
	hieth_writel_bits(ld, mode, GLB_ENDIAN_MOD, BITS_ENDIAN);
	local_unlock(ld);

	return old;
}
