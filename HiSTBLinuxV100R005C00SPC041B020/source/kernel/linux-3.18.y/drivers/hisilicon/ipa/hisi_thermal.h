/**
 * hisi_thermal.h
 *
 * Copyright (c) 2009-2015, HiSilicon Technologies Co., Ltd. 
 * All rights reserved.
 */

#ifndef __HISI_THERMAL_H__
#define __HISI_THERMAL_H__

#define FRAC_BITS 10
#define int_to_frac(x) ((x) << FRAC_BITS)
#define frac_to_int(x) ((x) >> FRAC_BITS)

/* hisi only support one sensor current */
#define HISI_THERMAL_SENSOR_ID	0

enum cluster_type {
	CLUSTER_BIG = 0,
	CLUSTER_LITTLE,
	NUM_CLUSTERS
};

struct cluster_power_coefficients {
	int dyn_coeff;
	int static_coeff;
	int cache_static_coeff;
};

struct hisi_sensor {
	u16 sensor_id;
	unsigned long prev_temp;
	u32 alpha;
	struct thermal_zone_device *tzd;
	struct cpumask cluster[NUM_CLUSTERS];
	struct thermal_cooling_device *cdevs[NUM_CLUSTERS];
};

int get_sensor_value(u32 *val);
void platform_thermal_init(void);
#endif /* __HISI_THERMAL_H__ */
