#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* skyworth */
	{ 0x0000000c, KEY_POWER},
	{ 0x0000001c, KEY_COFFEE}, /* screensaver */
	{ 0x00000019, KEY_FN_F1},  /* dimensional sound */
	{ 0x0000001a, KEY_FN_F2}, /* surround sound */
	{ 0x0000000d, KEY_MUTE},
	{ 0x0000000e, KEY_TIME},
	{ 0x0000001d, KEY_TEEN},
	{ 0x00000021, KEY_CALENDAR},
	{ 0x0000002f, KEY_FN_F3},  /* color mode selecter */
	{ 0x00000029, KEY_FN_F4}, /* listen onlly */
	{ 0x00000001, KEY_1},
	{ 0x00000002, KEY_2},
	{ 0x00000003, KEY_3},
	{ 0x00000004, KEY_4},
	{ 0x00000005, KEY_5},
	{ 0x00000006, KEY_6},
	{ 0x00000007, KEY_7},
	{ 0x00000008, KEY_8},
	{ 0x00000009, KEY_9},
	{ 0x00000000, KEY_0},
	{ 0x0000000a, KEY_SLASH},
	{ 0x0000000f, KEY_AUDIO},
	{ 0x00000016, KEY_CLEAR},
	{ 0x00000010, KEY_INFO}, /* screen show */
	{ 0x0000000b, KEY_MODE},  /* switch */
	{ 0x00000011, KEY_OK},
	{ 0x00000012, KEY_UP},
	{ 0x00000015, KEY_LEFT},
	{ 0x00000014, KEY_RIGHT},
	{ 0x00000013, KEY_DOWN},
	{ 0x00000017, KEY_FN_F5}, /* image mode */
	{ 0x00000022, KEY_FN_F6},/* interactive platform */
	{ 0x0000002a, KEY_FN_F7},/* data workstation */
	{ 0x00000028, KEY_FN_F8}, /* sound mode */
};

static struct rc_map_list hisi_map = {
	.map = {
		.scan	= hisi_keymap,
		.size	= ARRAY_SIZE(hisi_keymap),
		.rc_type= RC_TYPE_RC5,	/* Legacy IR type */
		.name	= "rc-hisi",
	}
};

static int __init init_rc_map_hisi(void)
{
	return rc_map_register(&hisi_map);
}

static void __exit exit_rc_map_hisi(void)
{
	rc_map_unregister(&hisi_map);
}

module_init(init_rc_map_hisi)
module_exit(exit_rc_map_hisi)

MODULE_LICENSE("GPL");