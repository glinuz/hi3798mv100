#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* skyworyh yk-60hb tc9012 */
	{ 0x000e0e0c, KEY_POWER},
	{ 0x000e0e17, KEY_IMAGES}, /* image mode */
	{ 0x000e0e18, KEY_AUDIO}, /* audio mode */
	{ 0x000e0e4f, KEY_VIDEO}, /* video mode */
	{ 0x000e0e01, KEY_1},
	{ 0x000e0e02, KEY_2},
	{ 0x000e0e03, KEY_3},
	{ 0x000e0e04, KEY_4},
	{ 0x000e0e05, KEY_5},
	{ 0x000e0e06, KEY_6},
	{ 0x000e0e07, KEY_7},
	{ 0x000e0e08, KEY_8},
	{ 0x000e0e09, KEY_9},
	{ 0x000e0e00, KEY_0},
	{ 0x000e0e0b, KEY_FN_1}, /* switch */
	{ 0x000e0e0a, KEY_FN_2}, /* slash */
	{ 0x000e0e1a, KEY_FN_B}, /* frame pause */
	{ 0x000e0e1c, KEY_FN_D}, /* V */
	{ 0x000e0e16, KEY_INFO}, /* screen display */
	{ 0x000e0e11, KEY_MENU},
	{ 0x000e0e53, KEY_FN_E}, /* kukai */
	{ 0x000e0e0f, KEY_FN_F}, /* signal source */
	{ 0x000e0e46, KEY_OK},
	{ 0x000e0e42, KEY_UP},
	{ 0x000e0e44, KEY_LEFT},
	{ 0x000e0e45, KEY_RIGHT},
	{ 0x000e0e43, KEY_DOWN},
	{ 0x000e0e5b, KEY_BACK},
	{ 0x000e0e51, KEY_FN_S},
	{ 0x000e0e12, KEY_CHANNELUP},
	{ 0x000e0e13, KEY_CHANNELDOWN},
	{ 0x000e0e14, KEY_VOLUMEUP},
	{ 0x000e0e15, KEY_VOLUMEDOWN},
	{ 0x000e0e0d, KEY_MUTE},
	{ 0x000e0e40, KEY_REWIND},
	{ 0x000e0e49, KEY_PLAYPAUSE},
	{ 0x000e0e4a, KEY_STOP},
	{ 0x000e0e41, KEY_FORWARD},
	{ 0x000e0e47, KEY_PREVIOUS},
	{ 0x000e0e48, KEY_NEXT},
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