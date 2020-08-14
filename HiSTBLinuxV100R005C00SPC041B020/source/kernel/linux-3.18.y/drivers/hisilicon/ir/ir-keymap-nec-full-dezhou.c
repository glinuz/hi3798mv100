#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* dongguan guang dian GBN, dezhou like */
	{ 0x57505853, KEY_POWER},
	{ 0x554d5853, KEY_MUTE},
	{ 0x4e525853, KEY_BACK},
	{ 0x31305853, KEY_1},
	{ 0x32305853, KEY_2},
	{ 0x33305853, KEY_3},
	{ 0x34305853, KEY_4},
	{ 0x35305853, KEY_5},
	{ 0x36305853, KEY_6},
	{ 0x37305853, KEY_7},
	{ 0x38305853, KEY_8},
	{ 0x39305853, KEY_9},
	{ 0x30305853, KEY_0},
	{ 0x48585853, KEY_FAVORITES},
	{ 0x44535853, KEY_AUDIO},
	{ 0x575a5853, KEY_TV},
	{ 0x42475853, KEY_RADIO},
	{ 0x585a5853, KEY_WWW},  /* web */
	{ 0x50445853, KEY_VIDEO}, /* VOD */
	{ 0x454d5853, KEY_PROGRAM}, /*  Program list */
	{ 0x4e5a5853, KEY_INFO},
	{ 0x4e455853, KEY_OK},
	{ 0x49525853, KEY_VOLUMEUP},
	{ 0x454c5853, KEY_VOLUMEDOWN},
	{ 0x50555853, KEY_CHANNELDOWN},
	{ 0x4f445853, KEY_CHANNELUP},
	{ 0x53505853, KEY_PAGEUP},
	{ 0x41505853, KEY_PAGEDOWN},
	{ 0x58455853, KEY_MENU},
	{ 0x58585853, KEY_EXIT},
	{ 0x45525853, KEY_RED},
	{ 0x52475853, KEY_GREEN},
	{ 0x45595853, KEY_YELLOW},
	{ 0x4c425853, KEY_BLUE},
};

static struct rc_map_list hisi_map = {
	.map = {
		.scan	= hisi_keymap,
		.size	= ARRAY_SIZE(hisi_keymap),
		.rc_type= RC_TYPE_NEC_FULL_48BIT,	/* Legacy IR type */
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