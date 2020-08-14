#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* philips */
	{ 0x000004c7, KEY_POWER},
	{ 0x000004d1, KEY_MENU},
	{ 0x0000040f, KEY_INFO}, /* display */
	{ 0x0000045c, KEY_OK},
	{ 0x00000458, KEY_UP},
	{ 0x0000045a, KEY_LEFT},
	{ 0x0000045b, KEY_RIGHT},
	{ 0x00000459, KEY_DOWN},
	{ 0x00000483, KEY_TITLE},
	{ 0x00000482, KEY_SETUP},
	{ 0x00000421, KEY_PREVIOUS},
	{ 0x00000420, KEY_NEXT},
	{ 0x0000042c, KEY_PLAYPAUSE},
	{ 0x00000431, KEY_STOP},
	{ 0x0000040d, KEY_MUTE},
	{ 0x00000401, KEY_1},
	{ 0x00000402, KEY_2},
	{ 0x00000403, KEY_3},
	{ 0x00000404, KEY_4},
	{ 0x00000405, KEY_5},
	{ 0x00000406, KEY_6},
	{ 0x00000407, KEY_7},
	{ 0x00000408, KEY_8},
	{ 0x00000409, KEY_9},
	{ 0x00000000, KEY_0},
	{ 0x0000044b, KEY_SUBTITLE},
	{ 0x0000044e, KEY_AUDIO},
	{ 0x000004f7, KEY_ZOOM},
	{ 0x000004e7, KEY_FN_1}, /* vocall */
	{ 0x000004e8, KEY_FN_2}, /* karaoke */
};

static struct rc_map_list hisi_map = {
	.map = {
		.scan	= hisi_keymap,
		.size	= ARRAY_SIZE(hisi_keymap),
		.rc_type= RC_TYPE_RC6_6A_20,	/* Legacy IR type */
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