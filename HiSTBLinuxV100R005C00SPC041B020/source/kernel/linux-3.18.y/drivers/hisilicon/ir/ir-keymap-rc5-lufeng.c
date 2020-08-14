#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* lufeng you xian */
	{ 0x00000c0c, KEY_POWER},
	{ 0x00000c0d, KEY_AUDIO},
	{ 0x00000c0e, KEY_MUTE},
	{ 0x00000c0f, KEY_SWITCHVIDEOMODE},
	{ 0x00000c10, KEY_FAVORITES},
	{ 0x00000c11, KEY_RED},
	{ 0x00000c12, KEY_GREEN},
	{ 0x00000c13, KEY_YELLOW},
	{ 0x00000c14, KEY_BLUE},
	{ 0x00000c19, KEY_OK},
	{ 0x00000c17, KEY_UP},
	{ 0x00000c18, KEY_LEFT},
	{ 0x00000c1a, KEY_RIGHT},
	{ 0x00000c1b, KEY_DOWN},
	{ 0x00000c15, KEY_MENU},
	{ 0x00000c16, KEY_BACK},
	{ 0x00000c1c, KEY_PROGRAM},
	{ 0x00000c1d, KEY_EXIT},
	{ 0x00000c20, KEY_PAGEUP},
	{ 0x00000c21, KEY_PAGEDOWN},
	{ 0x00000c22, KEY_VOLUMEUP},
	{ 0x00000c23, KEY_VOLUMEDOWN},
	{ 0x00000c1f, KEY_CHANNELDOWN},
	{ 0x00000c1e, KEY_CHANNELUP},
	{ 0x00000c01, KEY_1},
	{ 0x00000c02, KEY_2},
	{ 0x00000c03, KEY_3},
	{ 0x00000c04, KEY_4},
	{ 0x00000c05, KEY_5},
	{ 0x00000c06, KEY_6},
	{ 0x00000c07, KEY_7},
	{ 0x00000c08, KEY_8},
	{ 0x00000c09, KEY_9},
	{ 0x00000c00, KEY_0},
	{ 0x00000c0a, KEY_FN_1},
	{ 0x00000c0b, KEY_FN_2},
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