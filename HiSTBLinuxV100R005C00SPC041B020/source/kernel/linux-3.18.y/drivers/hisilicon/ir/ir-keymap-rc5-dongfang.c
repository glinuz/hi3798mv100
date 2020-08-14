#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* dongfang youxian TV, rc5-14bits */
	{ 0x00001650, KEY_POWER},
	{ 0x00001651, KEY_MUTE},
	{ 0x00001657, KEY_VOLUMEUP},
	{ 0x00001656, KEY_VOLUMEDOWN},
	{ 0x0000165a, KEY_CHANNELUP},
	{ 0x00001659, KEY_CHANNELDOWN},
	{ 0x00001654, KEY_EXIT},
	{ 0x00001655, KEY_BACK},
	{ 0x00001668, KEY_PAGEUP},
	{ 0x00001669, KEY_PAGEDOWN},
	{ 0x00001662, KEY_PROGRAM},
	{ 0x00001644, KEY_VIDEO},
	{ 0x00001667, KEY_OK},
	{ 0x00001663, KEY_UP},
	{ 0x00001665, KEY_LEFT},
	{ 0x00001666, KEY_RIGHT},
	{ 0x00001664, KEY_DOWN},
	{ 0x00001661, KEY_FN_E}, /* service */
	{ 0x00001660, KEY_INFO},
	{ 0x0000166c, KEY_RED},
	{ 0x0000166d, KEY_GREEN},
	{ 0x0000166e, KEY_YELLOW},
	{ 0x0000166f, KEY_BLUE},
	{ 0x0000166b, KEY_FN_F}, /* flush */
	{ 0x00001645, KEY_TV},
	{ 0x00001646, KEY_FN_S}, /* music */
	{ 0x00001671, KEY_1},
	{ 0x00001672, KEY_2},
	{ 0x00001673, KEY_3},
	{ 0x00001674, KEY_4},
	{ 0x00001675, KEY_5},
	{ 0x00001676, KEY_6},
	{ 0x00001677, KEY_7},
	{ 0x00001678, KEY_8},
	{ 0x00001679, KEY_9},
	{ 0x00001670, KEY_0},
	{ 0x00001649, KEY_FN_1},
	{ 0x0000164a, KEY_FN_2},
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