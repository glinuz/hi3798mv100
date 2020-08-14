#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* hisi ir */
	{ 0x0000009c, KEY_POWER},
	{ 0x000000d1, KEY_AUDIO},
	{ 0x000000dd, KEY_MUTE},
	{ 0x00000086, KEY_CHANNELUP},
	{ 0x00000085, KEY_CHANNELDOWN},
	{ 0x00000081, KEY_VOLUMEUP},
	{ 0x00000080, KEY_VOLUMEDOWN},
	{ 0x00000092, KEY_1},
	{ 0x00000093, KEY_2},
	{ 0x000000cc, KEY_3},
	{ 0x0000008e, KEY_4},
	{ 0x0000008f, KEY_5},
	{ 0x000000c8, KEY_6},
	{ 0x0000008a, KEY_7},
	{ 0x0000008b, KEY_8},
	{ 0x000000c4, KEY_9},
	{ 0x00000087, KEY_0},
	{ 0x000000cb, KEY_SLASH},
	{ 0x00000088, KEY_BACKSPACE},
	{ 0x0000009f, KEY_F1}, /* input method */
	{ 0x00000094, KEY_FAVORITES},
	{ 0x000000c6, KEY_F3}, /* interactive */
	{ 0x00000097, KEY_SEARCH},
	{ 0x000000ce, KEY_OK},
	{ 0x000000ca, KEY_UP},
	{ 0x00000099, KEY_LEFT},
	{ 0x000000c1, KEY_RIGHT},
	{ 0x000000d2, KEY_DOWN},
	{ 0x0000009d, KEY_MENU},
	{ 0x000000cf, KEY_PAGEUP},
	{ 0x00000098, KEY_PAGEDOWN},
	{ 0x00000090, KEY_BACK},
	{ 0x0000009b, KEY_FN_1}, /* live tv */
	{ 0x0000009a, KEY_FN_2}, /* vod */
	{ 0x000000c0, KEY_FN_E}, /* loop play */
	{ 0x000000c2, KEY_FN_F}, /* replay */
	{ 0x000000c3, KEY_PLAYPAUSE},
	{ 0x000000da, KEY_REWIND},
	{ 0x000000d6, KEY_FORWARD},
	{ 0x000000d0, KEY_STOP},
	{ 0x00000084, KEY_RED},
	{ 0x00000089, KEY_GREEN},
	{ 0x000000d9, KEY_YELLOW},
	{ 0x00000096, KEY_BLUE},
	{ 0x00000095, KEY_SETUP},
	{ 0x0000008c, KEY_SUBTITLE},
	{ 0x00000082, KEY_FN_D}, /* location */
	{ 0x000000c7, KEY_INFO}, 
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