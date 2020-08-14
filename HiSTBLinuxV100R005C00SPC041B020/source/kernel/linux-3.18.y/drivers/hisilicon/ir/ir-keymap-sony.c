#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* sony */
	{ 0x00010015, KEY_POWER},
	{ 0x00010014, KEY_MUTE},
	{ 0x00010038, KEY_PLAY},
	{ 0x0001003f, KEY_PAUSE},
	{ 0x00010025, KEY_STOP},
	{ 0x00010000, KEY_1},
	{ 0x00010001, KEY_2},
	{ 0x00010002, KEY_3},
	{ 0x00010003, KEY_4},
	{ 0x00010004, KEY_5},
	{ 0x00010005, KEY_6},
	{ 0x00010006, KEY_7},
	{ 0x00010007, KEY_8},
	{ 0x00010008, KEY_9},
	{ 0x00010009, KEY_0},
	{ 0x0001001d, KEY_FN_1}, /* slash */
	{ 0x0001003a, KEY_FN_2}, /* info */
	{ 0x00010065, KEY_OK},
	{ 0x00010060, KEY_MENU},
	{ 0x0003004d, KEY_UP},
	{ 0x0003004c, KEY_LEFT},
	{ 0x0003004e, KEY_RIGHT},
	{ 0x0003004f, KEY_DOWN},
	{ 0x00010012, KEY_VOLUMEUP},
	{ 0x00010013, KEY_VOLUMEDOWN},
	{ 0x00010010, KEY_CHANNELUP},
	{ 0x00010011, KEY_CHANNELDOWN},
};

static struct rc_map_list hisi_map = {
	.map = {
		.scan	= hisi_keymap,
		.size	= ARRAY_SIZE(hisi_keymap),
		.rc_type= RC_TYPE_SONY12,	/* Legacy IR type */
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