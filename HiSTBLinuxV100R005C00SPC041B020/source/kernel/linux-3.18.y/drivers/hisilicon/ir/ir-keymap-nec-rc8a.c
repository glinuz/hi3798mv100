#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* RC-8A */
	{ 0x0000040d, KEY_POWER},
	{ 0x0000040c, KEY_MUTE},
	{ 0x00000401, KEY_1},
	{ 0x00000402, KEY_2},
	{ 0x00000403, KEY_3},
	{ 0x00000404, KEY_4},
	{ 0x00000405, KEY_5},
	{ 0x00000406, KEY_6},
	{ 0x00000407, KEY_7},
	{ 0x00000408, KEY_8},
	{ 0x00000409, KEY_9},
	{ 0x00000400, KEY_0},
	{ 0x0000040e, KEY_PAGEUP},
	{ 0x0000040f, KEY_PAGEDOWN},
	{ 0x0000041a, KEY_VOLUMEUP},
	{ 0x0000041b, KEY_VOLUMEDOWN},
	{ 0x0000044c, KEY_CHANNELUP},
	{ 0x0000044d, KEY_CHANNELDOWN},
	{ 0x00000414, KEY_FAVORITES},
	{ 0x00000444, KEY_MENU},
	{ 0x00000419, KEY_OK},
	{ 0x0000040a, KEY_UP},
	{ 0x00000410, KEY_LEFT},
	{ 0x00000411, KEY_RIGHT},
	{ 0x0000040b, KEY_DOWN},
	{ 0x00000418, KEY_EXIT}, /* interactive */
	{ 0x00000415, KEY_BACK},
	{ 0x00000412, KEY_TV},
	{ 0x00000413, KEY_AUDIO},
	{ 0x00000416, KEY_FN_1}, /* live tv */
	{ 0x00000417, KEY_FN_2}, /* vod */
	{ 0x00000449, KEY_INFO},
	{ 0x00000445, KEY_FN_E}, /* loop play */
	{ 0x00000446, KEY_FN_F}, /* replay */
	{ 0x00000447, KEY_LIST},
	{ 0x00000440, KEY_RED},
	{ 0x00000441, KEY_GREEN},
	{ 0x00000442, KEY_YELLOW},
	{ 0x00000443, KEY_BLUE},
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