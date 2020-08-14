
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

/* Register Map */
#define HII2C_CTRL			0x00
#define HII2C_COM			0x04
#define HII2C_ICR			0x08
#define HII2C_SR			0x0c
#define HII2C_SCL_H			0x10
#define HII2C_SCL_L			0x14
#define HII2C_TXR			0x18
#define HII2C_RXR			0x1c

/* I2C_CTRL_REG */
#define I2C_ENABLE			BIT(8)
#define I2C_UNMASK_TOTAL		BIT(7)
#define I2C_UNMASK_START		BIT(6)
#define I2C_UNMASK_END			BIT(5)
#define I2C_UNMASK_SEND			BIT(4)
#define I2C_UNMASK_RECEIVE		BIT(3)
#define I2C_UNMASK_ACK			BIT(2)
#define I2C_UNMASK_ARBITRATE		BIT(1)
#define I2C_UNMASK_OVER			BIT(0)
#define I2C_UNMASK_ALL			(I2C_UNMASK_ACK | I2C_UNMASK_OVER)

/* I2C_COM_REG */
#define I2C_NO_ACK			BIT(4)
#define I2C_START			BIT(3)
#define I2C_READ			BIT(2)
#define I2C_WRITE			BIT(1)
#define I2C_STOP			BIT(0)
#define I2C_READ_NOACK 			(I2C_READ | I2C_NO_ACK)

/* I2C_ICR_REG */
#define I2C_CLEAR_START			BIT(6)
#define I2C_CLEAR_END			BIT(5)
#define I2C_CLEAR_SEND			BIT(4)
#define I2C_CLEAR_RECEIVE		BIT(3)
#define I2C_CLEAR_ACK			BIT(2)
#define I2C_CLEAR_ARBITRATE		BIT(1)
#define I2C_CLEAR_OVER			BIT(0)
#define I2C_CLEAR_ALL			(I2C_CLEAR_START | I2C_CLEAR_END |      \
					 I2C_CLEAR_SEND | I2C_CLEAR_RECEIVE |   \
					 I2C_CLEAR_ACK | I2C_CLEAR_ARBITRATE |  \
					 I2C_CLEAR_OVER)

/* I2C_SR_REG */
#define I2C_BUS_BUSY			BIT(7)
#define I2C_START_INTR			BIT(6)
#define I2C_END_INTR			BIT(5)
#define I2C_SEND_INTR			BIT(4)
#define I2C_RECEIVE_INTR		BIT(3)
#define I2C_ACKERR_INTR			BIT(2)
#define I2C_ARBLOST_INTR		BIT(1)
#define I2C_OVER_INTR			BIT(0)

#define HII2C_MAX_FREQ			400000		/* 400k */
#define HII2C_READ_OPERATION		0x01

/* I2C extend flags */
#define HII2C_M_NOSTOP			0x0100
#define HII2C_M_NOSTART			0x0200
#define HII2C_M_NOSTART_NOADDR		0x0400

enum hi_i2c_state {
	HII2C_STAT_RW_ERR = -1,
	HII2C_STAT_INIT,
	HII2C_STAT_RW,
	HII2C_STAT_SND_STOP,
	HII2C_STAT_RW_SUCCESS,
};

struct hi_i2c_host {
	struct device *dev;
	void __iomem *regs;
	struct clk *clk;
	int irq;

	struct i2c_adapter adapter;
	struct completion completion;

	struct i2c_msg *msg;
	unsigned int pos;
	unsigned int msg_len;

	int stop;

	int err;
	unsigned int freq;
	enum hi_i2c_state state;
};

#define __I2C_REG_DUMP

#ifdef I2C_REG_DUMP
struct i2c_reg_name {
	u32 reg;
	char *name;
} i2c_reg_name [] = {
	{0x00, "CTRL"},
	{0x04, "COM"},
	{0x08, "ICR"},
	{0x0c, "SR"},
	{0x10, "SCL_H"},
	{0x14, "SCL_L"},
	{0x18, "TXR"},
	{0x1c, "RXR"},
};
#endif

static void i2c_writel(struct hi_i2c_host *i2c_dev, u32 val, u32 offset)
{
#ifdef I2C_REG_DUMP
	unsigned int idx = (unsigned int)((i2c_dev->regs + offset) & 0x00FF);
	if (idx <= 0x1C)
		pr_notice("write 0x%04x to %s\n", val, i2c_reg_name[idx>>2].name);
#endif
	writel(val, i2c_dev->regs + offset);
}

static u32 i2c_readl(struct hi_i2c_host *i2c_dev, u32 offset)
{
	u32 val = readl(i2c_dev->regs + offset);
#ifdef I2C_REG_DUMP
	unsigned int idx = (unsigned int)((i2c_dev->regs + offset) & 0x00FF);
	if (idx <= 0x1C)
		pr_notice("read 0x%04x from %s\n", val, i2c_reg_name[idx>>2].name);
#endif
	return val;
}

static u32 hi_i2c_clr_pending_irq(struct hi_i2c_host *i2c_dev)
{
	u32 val = i2c_readl(i2c_dev, HII2C_SR);

	i2c_writel(i2c_dev, (val & I2C_CLEAR_ALL), HII2C_ICR);

	return val;
}

static void hi_i2c_clr_all_irq(struct hi_i2c_host *i2c_dev)
{
	i2c_writel(i2c_dev, I2C_CLEAR_ALL, HII2C_ICR);
}

static void hi_i2c_disable_irq(struct hi_i2c_host *i2c_dev)
{
	i2c_writel(i2c_dev, 0, HII2C_CTRL);
}

static void hi_i2c_enable_irq(struct hi_i2c_host *i2c_dev)
{
	i2c_writel(i2c_dev, I2C_ENABLE | I2C_UNMASK_TOTAL |
		  I2C_UNMASK_ALL | I2C_UNMASK_ARBITRATE,
		  HII2C_CTRL);
}

static void hi_i2c_drv_setrate(struct hi_i2c_host *i2c_dev)
{
	u32 rate, val;
	u32 scl, sysclock;

	/* close all i2c interrupt */
	val = i2c_readl(i2c_dev, HII2C_CTRL);
	i2c_writel(i2c_dev, val & (~I2C_UNMASK_TOTAL), HII2C_CTRL);

	rate = i2c_dev->freq;
	sysclock = clk_get_rate(i2c_dev->clk);
	scl = (sysclock / (rate * 2)) / 2 - 1;
	i2c_writel(i2c_dev, scl, HII2C_SCL_H);
	i2c_writel(i2c_dev, scl, HII2C_SCL_L);

	/* restore original interrupt*/
	i2c_writel(i2c_dev, val, HII2C_CTRL);

	pr_debug("%s: sysclock=%d, rate=%d, scl=%d\n",
		__func__, sysclock, rate, scl);
}

static void hi_i2c_init(struct hi_i2c_host *i2c_dev)
{
	hi_i2c_disable_irq(i2c_dev);
	hi_i2c_drv_setrate(i2c_dev);
	hi_i2c_clr_all_irq(i2c_dev);
	hi_i2c_enable_irq(i2c_dev);
}

static void hi_i2c_reset(struct hi_i2c_host *i2c_dev)
{
	clk_disable_unprepare(i2c_dev->clk);
	clk_prepare_enable(i2c_dev->clk);
	hi_i2c_init(i2c_dev);
}

static int hi_i2c_wait_bus_idle(struct hi_i2c_host *i2c_dev)
{
	u32 stat;
	unsigned long stop_time;

	/* wait for 100 milli seconds for the bus to be idle */
	stop_time = jiffies + msecs_to_jiffies(100);
	do {
		stat = hi_i2c_clr_pending_irq(i2c_dev);
		if (!(stat & I2C_BUS_BUSY))
			return 0;

		usleep_range(50, 200);
	} while (time_before(jiffies, stop_time));

	return -EBUSY;
}

static void hi_i2c_rw_over(struct hi_i2c_host *i2c_dev)
{
	if (i2c_dev->state == HII2C_STAT_SND_STOP)
		pr_debug("%s: rw and send stop over\n", __func__);
	else
		pr_debug("%s: have not data to send\n", __func__);

	i2c_dev->state = HII2C_STAT_RW_SUCCESS;
	i2c_dev->err = 0;
}

static void hi_i2c_rw_handle_stop(struct hi_i2c_host *i2c_dev)
{
	if (i2c_dev->stop) {
		i2c_dev->state = HII2C_STAT_SND_STOP;
		i2c_writel(i2c_dev, I2C_STOP, HII2C_COM);
	} else
		hi_i2c_rw_over(i2c_dev);
}

static void hi_i2c_read_handle(struct hi_i2c_host *i2c_dev)
{
	if (i2c_dev->msg_len == 1) {
		/* the last byte don't need send ACK */
		i2c_writel(i2c_dev, I2C_READ | I2C_NO_ACK, HII2C_COM);
	} else if (i2c_dev->msg_len > 1) {
		/* if i2c master receive data will send ACK */
		i2c_writel(i2c_dev, I2C_READ, HII2C_COM);
	} else {
		hi_i2c_rw_handle_stop(i2c_dev);
	}
}

static void hi_i2c_write_handle(struct hi_i2c_host *i2c_dev)
{
	u8 data;

	if (i2c_dev->msg_len > 0) {
		data = i2c_dev->msg->buf[i2c_dev->pos++];
		i2c_writel(i2c_dev, data, HII2C_TXR);
		i2c_writel(i2c_dev, I2C_WRITE, HII2C_COM);
	} else {
		hi_i2c_rw_handle_stop(i2c_dev);
	}
}

static int hi_i2c_rw_preprocess(struct hi_i2c_host *i2c_dev)
{
	u8 data;

	if (i2c_dev->state == HII2C_STAT_INIT) {
		i2c_dev->state = HII2C_STAT_RW;
	} else if (i2c_dev->state == HII2C_STAT_RW) {
		if (i2c_dev->msg->flags & I2C_M_RD) {
			data = i2c_readl(i2c_dev, HII2C_RXR);
			i2c_dev->msg->buf[i2c_dev->pos++] = data;
		}
		i2c_dev->msg_len--;
	} else {
		pr_debug("%s: error: priv->state = %d, msg_len = %d\n",
			__func__, i2c_dev->state, i2c_dev->msg_len);
		return -EAGAIN;
	}
	return 0;
}

static irqreturn_t hi_i2c_irq_isr(int irqno, void *dev_id)
{
	u32 stat;
	int ret;
	struct hi_i2c_host *i2c_dev = dev_id;
	struct i2c_msg *msg = i2c_dev->msg;

	stat = hi_i2c_clr_pending_irq(i2c_dev);

	/* handle error */
	if (stat & I2C_ARBLOST_INTR) {
		/* bus error */
		pr_debug("ARB bus loss\n");
		i2c_dev->err = -EAGAIN;
		i2c_dev->state = HII2C_STAT_RW_ERR;
		goto stop;
	} else if (stat & I2C_ACKERR_INTR) {
		/* ack error */
		pr_debug("No ACK from device\n");
		i2c_dev->err = -ENXIO;
		i2c_dev->state = HII2C_STAT_RW_ERR;
		goto stop;
	}

	if (stat & I2C_OVER_INTR) {
		if (i2c_dev->msg_len > 0) {
			ret = hi_i2c_rw_preprocess(i2c_dev);
			if (ret) {
				i2c_dev->err = ret;
				i2c_dev->state = HII2C_STAT_RW_ERR;
				goto stop;
			}
			if (msg->flags & I2C_M_RD)
				hi_i2c_read_handle(i2c_dev);
			else
				hi_i2c_write_handle(i2c_dev);
		} else {
			hi_i2c_rw_over(i2c_dev);
		}
	}

stop:
	if ((i2c_dev->state == HII2C_STAT_RW_SUCCESS &&
	     msg->len == i2c_dev->pos) ||
	    (i2c_dev->state == HII2C_STAT_RW_ERR)) {
		hi_i2c_disable_irq(i2c_dev);
		hi_i2c_clr_pending_irq(i2c_dev);
		complete(&i2c_dev->completion);
	}

	return IRQ_HANDLED;
}

static void hi_i2c_message_start(struct hi_i2c_host *i2c_dev)
{
	struct i2c_msg *msg;

	hi_i2c_clr_all_irq(i2c_dev);
	hi_i2c_enable_irq(i2c_dev);

	msg = i2c_dev->msg;
	if (msg->flags & HII2C_M_NOSTART_NOADDR) {
		i2c_writel(i2c_dev, I2C_READ, HII2C_COM);
		return;
	}

	if (msg->flags & I2C_M_RD)
		i2c_writel(i2c_dev, (msg->addr << 1) | HII2C_READ_OPERATION,
			   HII2C_TXR);
	else
		i2c_writel(i2c_dev, msg->addr << 1, HII2C_TXR);

	if (msg->flags & HII2C_M_NOSTART)
		i2c_writel(i2c_dev, I2C_WRITE, HII2C_COM);
	else
		i2c_writel(i2c_dev, I2C_WRITE | I2C_START, HII2C_COM);
}

static int hi_i2c_xfer_msg(struct hi_i2c_host *i2c_dev, struct i2c_msg *msgs,
			   int stop)
{
	int ret;
	unsigned long timeout;

	i2c_dev->msg = msgs;
	i2c_dev->pos = 0;
	i2c_dev->msg_len = i2c_dev->msg->len;
	i2c_dev->stop = stop;
	i2c_dev->err = 0;
	i2c_dev->state = HII2C_STAT_INIT;

	reinit_completion(&i2c_dev->completion);
	hi_i2c_message_start(i2c_dev);

	timeout = wait_for_completion_timeout(&i2c_dev->completion,
			i2c_dev->adapter.timeout);
	if (!timeout) {
		i2c_dev->state = HII2C_STAT_RW_ERR;
		i2c_dev->err = -ETIMEDOUT;
		pr_warning("%s timeout=%d\n",
			 msgs->flags & I2C_M_RD ? "rx" : "tx",
			 i2c_dev->adapter.timeout);
	}
	ret = i2c_dev->state;

	/*
	 * If this is the last message to be transfered (stop == 1)
	 * Then check if the bus can be brought back to idle.
	 */
	if (i2c_dev->state == HII2C_STAT_RW_SUCCESS && stop)
		ret = hi_i2c_wait_bus_idle(i2c_dev);

	if (ret < 0)
		hi_i2c_reset(i2c_dev);

	return i2c_dev->err;
}

static int hi_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
{
	int i, ret, stop;
	struct hi_i2c_host *i2c_dev = i2c_get_adapdata(adap);

	pm_runtime_get_sync(i2c_dev->dev);

	for (i = 0; i < num; i++, msgs++) {
		if (((i == num - 1) && !(msgs->flags & HII2C_M_NOSTOP)) ||
		    (msgs->flags & I2C_M_STOP))
			stop = 1;
		else
			stop = 0;

		ret = hi_i2c_xfer_msg(i2c_dev, msgs, stop);
		if (ret < 0)
			goto out;
	}

	if (i == num) {
		ret = num;
	} else {
		/* Only one message, cannot access the device */
		if (i == 1)
			ret = -EREMOTEIO;
		else
			ret = i;

		pr_warning("xfer message failed\n");
	}

out:
	pm_runtime_mark_last_busy(i2c_dev->dev);
	pm_runtime_put_autosuspend(i2c_dev->dev);
	return ret;
}

static u32 hi_i2c_func(struct i2c_adapter *adapter)
{
	return I2C_FUNC_I2C | (I2C_FUNC_SMBUS_EMUL & ~I2C_FUNC_SMBUS_QUICK);
}

static const struct i2c_algorithm hi_i2c_algo = {
	.master_xfer		= hi_i2c_xfer,
	.functionality		= hi_i2c_func,
};

static int hi_i2c_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct hi_i2c_host *i2c_dev;
	unsigned int freq;
	int ret;

	i2c_dev = devm_kzalloc(&pdev->dev, sizeof(*i2c_dev), GFP_KERNEL);
	if (!i2c_dev)
		return -ENOMEM;
	platform_set_drvdata(pdev, i2c_dev);

	if (of_property_read_u32(np, "clock-frequency", &freq)) {
		/* use 100k as default value */
		i2c_dev->freq = 100000;
	} else {
		if (freq > HII2C_MAX_FREQ) {
			freq = HII2C_MAX_FREQ;
			dev_warn(i2c_dev->dev, "use max freq %d instead\n",
				 HII2C_MAX_FREQ);
		}
		i2c_dev->freq = freq;
	}

	i2c_dev->regs = of_iomap(np, 0);
	if (IS_ERR(i2c_dev->regs))
		return PTR_ERR(i2c_dev->regs);

	i2c_dev->irq = of_irq_get(np, 0);
	if (i2c_dev->irq <= 0) {
		pr_err("cannot find HI-I2C IRQ\n");
		return i2c_dev->irq;
	}

	i2c_dev->clk = of_clk_get(np, 0);
	if (IS_ERR(i2c_dev->clk)) {
		pr_err("cannot find HI-I2C clock\n");
		return PTR_ERR(i2c_dev->clk);
	}
	clk_prepare_enable(i2c_dev->clk);

	strlcpy(i2c_dev->adapter.name, "hi-i2c", sizeof(i2c_dev->adapter.name));
	i2c_dev->dev = &pdev->dev;
	i2c_dev->adapter.owner = THIS_MODULE;
	i2c_dev->adapter.algo = &hi_i2c_algo;
	i2c_dev->adapter.retries = 3;
	i2c_dev->adapter.dev.of_node = np;
	i2c_dev->adapter.algo_data = i2c_dev;
	i2c_dev->adapter.dev.parent = &pdev->dev;
	i2c_set_adapdata(&i2c_dev->adapter, i2c_dev);

	init_completion(&i2c_dev->completion);

	hi_i2c_init(i2c_dev);

	ret = devm_request_irq(&pdev->dev, i2c_dev->irq, hi_i2c_irq_isr,
			IRQF_NO_SUSPEND | IRQF_ONESHOT,
			dev_name(&pdev->dev), i2c_dev);
	if (ret != 0) {
		pr_err("cannot request HS-I2C IRQ %d\n", i2c_dev->irq);
		goto err_clk;
	}

	pm_suspend_ignore_children(&pdev->dev, true);
	pm_runtime_set_autosuspend_delay(i2c_dev->dev, MSEC_PER_SEC);
	pm_runtime_use_autosuspend(i2c_dev->dev);
	pm_runtime_set_active(i2c_dev->dev);
	pm_runtime_enable(i2c_dev->dev);

	ret = i2c_add_adapter(&i2c_dev->adapter);
	if (ret < 0) {
		pr_err("failed to add bus to i2c core\n");
		goto err_runtime;
	}
	return ret;

err_runtime:
	pm_runtime_disable(i2c_dev->dev);
	pm_runtime_set_suspended(i2c_dev->dev);

err_clk:
	clk_disable_unprepare(i2c_dev->clk);
	return ret;
}

static int hi_i2c_remove(struct platform_device *pdev)
{
	struct hi_i2c_host *i2c_dev = platform_get_drvdata(pdev);

	i2c_del_adapter(&i2c_dev->adapter);
	pm_runtime_disable(i2c_dev->dev);
	pm_runtime_set_suspended(i2c_dev->dev);

	return 0;
}

#ifdef CONFIG_PM
static int hi_i2c_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct hi_i2c_host *i2c_dev = platform_get_drvdata(pdev);

	clk_disable_unprepare(i2c_dev->clk);

	return 0;
}

static int hi_i2c_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct hi_i2c_host *i2c_dev = platform_get_drvdata(pdev);

	clk_prepare_enable(i2c_dev->clk);
	hi_i2c_init(i2c_dev);

	return 0;
}

static const struct dev_pm_ops hi_i2c_pm_ops = {
	SET_PM_RUNTIME_PM_OPS(hi_i2c_runtime_suspend,
			      hi_i2c_runtime_resume,
			      NULL)
};
#endif

static const struct of_device_id hi_i2c_match[] = {
	{ .compatible = "hisilicon,hi-i2c" },
	{},
};
//MODULE_DEVICE_TABLE(of, hi_i2c_match);

static struct platform_driver hi_i2c_driver = {
	.probe		= hi_i2c_probe,
	.remove		= hi_i2c_remove,
	.driver		= {
		.name	= "hi-i2c",
#ifdef CONFIG_PM
		.pm	= &hi_i2c_pm_ops,
#endif
		.of_match_table = hi_i2c_match,
	},
};

static int __init hi_i2c_platform_init(void)
{
	return platform_driver_register(&hi_i2c_driver);
}
subsys_initcall_sync(hi_i2c_platform_init);

static void __exit hi_i2c_platform_exit(void)
{
	platform_driver_unregister(&hi_i2c_driver);
}
module_exit(hi_i2c_platform_exit);


//module_platform_driver(hi_i2c_driver);
MODULE_DESCRIPTION("Hisi I2C Bus driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:i2c-hi");
