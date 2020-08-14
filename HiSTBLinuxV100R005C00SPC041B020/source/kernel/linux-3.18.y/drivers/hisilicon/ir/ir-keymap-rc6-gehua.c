#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* gehua TV, rc6 */
	{ 0x8010a60c, KEY_POWER},
	{ 0x8010260c, KEY_POWER},
	{ 0x8010260a, KEY_EXIT},
	{ 0x8010a60a, KEY_EXIT},
	{ 0x8010a6cc, KEY_PROGRAM},
	{ 0x801026cc, KEY_PROGRAM},
	{ 0x8010260f, KEY_INFO},
	{ 0x8010a60f, KEY_INFO},
	{ 0x8010a683, KEY_BACK},
	{ 0x80102683, KEY_BACK},
	{ 0x8010265c, KEY_OK},
	{ 0x8010a65c, KEY_OK},
	{ 0x8010a658, KEY_UP},
	{ 0x80102658, KEY_UP},
	{ 0x8010265a, KEY_LEFT},
	{ 0x8010a65a, KEY_LEFT},
	{ 0x8010a65b, KEY_RIGHT},
	{ 0x8010265b, KEY_RIGHT},
	{ 0x80102659, KEY_DOWN},
	{ 0x8010a659, KEY_DOWN},
	{ 0x8010a654, KEY_MENU},
	{ 0x80102654, KEY_MENU},
	{ 0x801026f5, KEY_LIST},
	{ 0x8010a6f5, KEY_LIST},
	{ 0x8010a682, KEY_HOME},
	{ 0x80102682, KEY_HOME},
	{ 0x8010260d, KEY_MUTE},
	{ 0x8010a60d, KEY_MUTE},
	{ 0x8010a610, KEY_VOLUMEUP},
	{ 0x80102610, KEY_VOLUMEUP},
	{ 0x80102611, KEY_VOLUMEDOWN},
	{ 0x8010a611, KEY_VOLUMEDOWN},
	{ 0x8010a620, KEY_CHANNELUP},
	{ 0x80102620, KEY_CHANNELUP},
	{ 0x80102621, KEY_CHANNELDOWN},
	{ 0x8010a621, KEY_CHANNELDOWN},
	{ 0x8010a66d, KEY_RED},
	{ 0x8010266d, KEY_RED},
	{ 0x8010266e, KEY_GREEN},
	{ 0x8010a66e, KEY_GREEN},
	{ 0x8010a66f, KEY_YELLOW},
	{ 0x8010266f, KEY_YELLOW},
	{ 0x8010a670, KEY_BLUE},
	{ 0x80102670, KEY_BLUE},
	{ 0x8010268c, KEY_A},
	{ 0x8010a68c, KEY_A},
	{ 0x8010a68d, KEY_B},
	{ 0x8010268d, KEY_B},
	{ 0x8010268e, KEY_C},
	{ 0x8010a68e, KEY_C},
	{ 0x8010a68f, KEY_D},
	{ 0x8010268f, KEY_D},
	{ 0x80102601, KEY_1},
	{ 0x8010a601, KEY_1},
	{ 0x80102602, KEY_2},
	{ 0x8010a602, KEY_2},
	{ 0x80102603, KEY_3},
	{ 0x8010a603, KEY_3},
	{ 0x80102604, KEY_4},
	{ 0x8010a604, KEY_4},
	{ 0x80102605, KEY_5},
	{ 0x8010a605, KEY_5},
	{ 0x80102606, KEY_6},
	{ 0x8010a606, KEY_6},
	{ 0x80102607, KEY_7},
	{ 0x8010a607, KEY_7},
	{ 0x80102608, KEY_8},
	{ 0x8010a608, KEY_8},
	{ 0x80102609, KEY_9},
	{ 0x8010a609, KEY_9},
	{ 0x80102600, KEY_0},
	{ 0x8010a600, KEY_0},
	{ 0x8010269f, KEY_FN_1},
	{ 0x8010a69f, KEY_FN_1},
	{ 0x801026a0, KEY_FN_2},
	{ 0x8010a6a0, KEY_FN_2},
};

static struct rc_map_list hisi_map = {
	.map = {
		.scan	= hisi_keymap,
		.size	= ARRAY_SIZE(hisi_keymap),
		.rc_type= RC_TYPE_RC6_6A_32,	/* Legacy IR type */
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