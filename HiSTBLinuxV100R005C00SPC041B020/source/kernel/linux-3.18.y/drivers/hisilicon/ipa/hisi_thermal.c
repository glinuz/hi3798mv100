#include <linux/cpu_cooling.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/thermal.h>
#include <linux/topology.h>

#include "hisi_thermal.h"

static struct hisi_sensor hisi_temp_sensor;
extern struct cluster_power_coefficients cluster_data[];

/**
 * mul_frac() - multiply two fixed-point numbers
 * @x:	first multiplicand
 * @y:	second multiplicand
 *
 * Return: the result of multiplying two fixed-point numbers.  The
 * result is also a fixed-point number.
 */
static inline s64 mul_frac(s64 x, s64 y)
{
	return (x * y) >> FRAC_BITS;
}

static unsigned long get_temperature_scale(unsigned long temp)
{
	int i, t_exp = 1, t_scale = 0;
	
    t_scale = 29;
    t_scale += (897 * temp * temp) / 100000;
    t_scale -= (892 * temp) / 1000;

	return t_scale; /* the value returned needs to be /1E3 */
}

static unsigned long get_voltage_scale(unsigned long u_volt)
{
	unsigned long m_volt = u_volt / 1000;
	unsigned long v_scale;

	v_scale = m_volt * m_volt * m_volt; /* = (m_V^3) / (900 ^ 3) = */

	return v_scale / 1000000; /* the value returned needs to be /(1E3) */
}

/* voltage in uV and temperature in mC */
static int get_static_power(cpumask_t *cpumask, int interval,
			    unsigned long u_volt, u32 *static_power)
{
	unsigned long temperature, t_scale, v_scale;
	u32 cpu_coeff;
	int nr_cpus = cpumask_weight(cpumask);
	enum cluster_type cluster =
		topology_physical_package_id(cpumask_any(cpumask));

	if (!hisi_temp_sensor.tzd)
		return -ENODEV;

	cpu_coeff = cluster_data[cluster].static_coeff;

	/* temperature in mC */
	temperature = hisi_temp_sensor.tzd->temperature / 1000;

	t_scale = get_temperature_scale(temperature);
	v_scale = get_voltage_scale(u_volt);

	*static_power = nr_cpus * (cpu_coeff * t_scale * v_scale) / 1000;

	if (nr_cpus) {
		u32 cache_coeff = cluster_data[cluster].cache_static_coeff;

		/* cache leakage */
		*static_power += (cache_coeff * v_scale * t_scale) / 1000000;
	}
	
	return 0;
}

static int get_temp_value(void *data, long *temp)
{
	struct hisi_sensor *sensor = (struct hisi_sensor *)data;
	u32 val;
	int ret;
	unsigned long est_temp;

	ret = get_sensor_value(&val);
	
	if (ret)
		return ret;

	if (!sensor->prev_temp)
		sensor->prev_temp = val;

	est_temp = mul_frac(sensor->alpha, val) +
		mul_frac((int_to_frac(1) - sensor->alpha), sensor->prev_temp);

	/**
	 * alpha = 24, val = 55000
	 * ((24 * 55000) >> 8) + ((((1 << 8) - 24) * 55000) >> 8)
	 */

	sensor->prev_temp = est_temp;
	*temp = est_temp;

	return 0;
}

static void update_debugfs(struct hisi_sensor *sensor_data)
{
	struct dentry *dentry_f, *filter_d;

	filter_d = debugfs_create_dir("hisi_thermal", NULL);
	if (IS_ERR_OR_NULL(filter_d)) {
		pr_warning("unable to create debugfs directory for the LPF filter\n");
		return;
	}

	dentry_f = debugfs_create_u32("alpha", S_IWUSR | S_IRUGO, filter_d,
					&sensor_data->alpha);

	if (IS_ERR_OR_NULL(dentry_f)) {
		pr_warn("Unable to create debugfsfile: alpha\n");
		return;
	}
}

static const struct thermal_zone_of_device_ops hisi_of_thermal_ops = {
	.get_temp = get_temp_value,
};

static int hisi_thermal_probe(struct platform_device *pdev)
{
	struct hisi_sensor *sensor_data = &hisi_temp_sensor;
	struct device_node *np;
	int cpu;
	int i, j;

	if (!cpufreq_frequency_get_table(0))
		return -EPROBE_DEFER;

	platform_set_drvdata(pdev, sensor_data);

	platform_thermal_init();

	/* If not support big.LITTLE, config it as CLUSTER_BIG */
	for_each_possible_cpu(cpu) {
		int cluster_id = topology_physical_package_id(cpu);
		if (cluster_id > NUM_CLUSTERS) {
			dev_warn(&pdev->dev, "Cluster id: %d > %d\n",
				 cluster_id, NUM_CLUSTERS);
			goto error;
		}

		cpumask_set_cpu(cpu, &sensor_data->cluster[cluster_id]);
	}

	for (i = 0, j = 0; i < NUM_CLUSTERS; i++) {
		char node[16];

		snprintf(node, 16, "cluster%d", i);
		np = of_find_node_by_name(NULL, node);

		if (!np) {
			dev_info(&pdev->dev, "Node not found: %s\n", node);
			continue;
		}

		sensor_data->cdevs[j] =
			of_cpufreq_power_cooling_register(np,
							  &sensor_data->cluster[i],
							  cluster_data[i].dyn_coeff,
							  get_static_power);

		if (IS_ERR(sensor_data->cdevs[i])) {
			dev_warn(&pdev->dev,
				"Error registering cooling device: %s\n", node);
			continue;
		}
		j++;
	}

	/* TODO: should get value by DTS */
	sensor_data->sensor_id = (u16)HISI_THERMAL_SENSOR_ID;
	dev_info(&pdev->dev, "Probed sensor. Id=%hu\n", sensor_data->sensor_id);

	/*
	 * alpha ~= 2 / (N + 1) with N the window of a rolling mean We
	 * use 8-bit fixed point arithmetic.  For a rolling average of
	 * window 20, alpha = 2 / (20 + 1) ~= 0.09523809523809523 .
	 * In 8-bit fixed point arigthmetic, 0.09523809523809523 * 256
	 * ~= 24
	 */
	sensor_data->alpha = 24;

	sensor_data->tzd = thermal_zone_of_sensor_register(&pdev->dev,
							sensor_data->sensor_id,
							sensor_data,
							&hisi_of_thermal_ops);

	if (IS_ERR(sensor_data->tzd)) {
		dev_warn(&pdev->dev, "Error registering sensor: %ld\n", PTR_ERR(sensor_data->tzd));
		return PTR_ERR(sensor_data->tzd);
	}

	update_debugfs(sensor_data);

	thermal_zone_device_update(sensor_data->tzd);

	dev_info(&pdev->dev, "probe suc\n");

	return 0;

error:
	return -ENODEV;
}

static int hisi_thermal_remove(struct platform_device *pdev)
{
	struct hisi_sensor *sensor = platform_get_drvdata(pdev);

	thermal_zone_device_unregister(sensor->tzd);
	platform_set_drvdata(pdev, NULL);

	return 0;
}

static struct of_device_id hisi_thermal_of_match[] = {
	{ .compatible = "arm,hisi-thermal" },
	{},
};
MODULE_DEVICE_TABLE(of, hisi_thermal_of_match);

static struct platform_driver hisi_thermal_platdrv = {
	.driver = {
		.name		= "hisi-thermal",
		.owner		= THIS_MODULE,
		.of_match_table = hisi_thermal_of_match,
	},
	.probe	= hisi_thermal_probe,
	.remove	= hisi_thermal_remove,
};
module_platform_driver(hisi_thermal_platdrv);

MODULE_LICENSE("GPL");
