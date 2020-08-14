#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* black */
	{ 0x00803f44, KEY_POWER},
	{ 0x00803f43, KEY_SWITCHVIDEOMODE},
	{ 0x00803f14, KEY_MUTE},
	{ 0x00803f09, KEY_MODE},  /* TV/RADIO */
	{ 0x00803f00, KEY_LAST},  /* recall */
	{ 0x00803f4f, KEY_ZOOM},
	{ 0x00803f52, KEY_FIND},
	{ 0x00803f05, KEY_SLEEP},
	{ 0x00803f4c, KEY_TEXT},  /* TEXT */
	{ 0x00803f0a, KEY_REWIND},
	{ 0x00803f04, KEY_FORWARD},
	{ 0x00803f0f, KEY_PREVIOUS}, /* revslow */
	{ 0x00803f40, KEY_SLOW},
	{ 0x00803f02, KEY_RECORD},
	{ 0x00803f08, KEY_PLAY},
	{ 0x00803f07, KEY_PAUSE},
	{ 0x00803f06, KEY_STOP},
	{ 0x00803f46, KEY_FN_1 },
	{ 0x00803f4a, KEY_EPG},
	{ 0x00803f51, KEY_PAGEUP},
	{ 0x00803f03, KEY_PAGEDOWN},
	{ 0x00803f0d, KEY_VOLUMEUP},
	{ 0x00803f0b, KEY_VOLUMEDOWN},
	{ 0x00803f11, KEY_OK},
	{ 0x00803f0c, KEY_UP},
	{ 0x00803f0e, KEY_DOWN},
	{ 0x00803f12, KEY_MENU},
	{ 0x00803f10, KEY_EXIT},
	{ 0x00803f49, KEY_FN_2},
	{ 0x00803f15, KEY_INFO},
	{ 0x00803f13, KEY_FN_B},
	{ 0x00803f47, KEY_FAVORITES},
	{ 0x00803f50, KEY_AUDIO},
	{ 0x00803f4b, KEY_SUBTITLE},
	{ 0x00803f42, KEY_RED},
	{ 0x00803f4e, KEY_GREEN},
	{ 0x00803f41, KEY_YELLOW},
	{ 0x00803f4d, KEY_BLUE},
	{ 0x00803f16, KEY_1},
	{ 0x00803f17, KEY_2},
	{ 0x00803f18, KEY_3},
	{ 0x00803f19, KEY_4},
	{ 0x00803f1a, KEY_5},
	{ 0x00803f1b, KEY_6},
	{ 0x00803f1c, KEY_7},
	{ 0x00803f1d, KEY_8},
	{ 0x00803f1e, KEY_9},
	{ 0x00803f1f, KEY_0},
};

static struct rc_map_list hisi_map = {
	.map = {
		.scan	= hisi_keymap,
		.size	= ARRAY_SIZE(hisi_keymap),
		.rc_type= RC_TYPE_NEC,	/* Legacy IR type */
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