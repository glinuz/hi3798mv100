#include <stdio.h>
#include "fv_dbg.h" 
#include "fv_config.h"
#include "minIni.h"

int fv_get_configs(const char* ini_file, struct fv_config_item *config)
{
	config->tuner.freq = ini_getl(TUNER_SECTION, FREQ_KEY, 0, ini_file);
	config->tuner.symbol_rate_or_band = ini_getl(TUNER_SECTION, SYMBOL_RATE_BAND_KEY, 0, ini_file);
	config->tuner.mode_type_or_polar = ini_getl(TUNER_SECTION, MODE_TYPE_POLAR_KEY, 0, ini_file);
	config->tuner.reverse = ini_getl(TUNER_SECTION, REVERSE_KEY, 0, ini_file);
	return 0;
}
