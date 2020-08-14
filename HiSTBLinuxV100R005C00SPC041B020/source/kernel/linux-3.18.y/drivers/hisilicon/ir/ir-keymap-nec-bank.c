#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* bank */
	{ 0xb34c5a53, KEY_POWER},
	{ 0xaf505a53, KEY_PROGRAM},
	{ 0xb24d5a53, KEY_MUTE},
	{ 0x9e615a53, KEY_RED},
	{ 0x9d625a53, KEY_GREEN},
	{ 0x9c635a53, KEY_YELLOW},
	{ 0xa9565a53, KEY_BLUE},
	{ 0xb7485a53, KEY_VOLUMEUP},
	{ 0xb6495a53, KEY_VOLUMEDOWN},
	{ 0xb54a5a53, KEY_PAGEUP},
	{ 0xb44b5a53, KEY_PAGEDOWN},
	{ 0xbb445a53, KEY_CHANNELUP},
	{ 0xba455a53, KEY_CHANNELDOWN},
	{ 0xad525a53, KEY_FN_1},
	{ 0x956a5a53, KEY_STOP},
	{ 0x97685a53, KEY_REWIND},
	{ 0x98675a53, KEY_FORWARD},
	{ 0x99665a53, KEY_PLAY},
	{ 0x96695a53, KEY_PAUSE},
	{ 0xbe415a53, KEY_HOME},
	{ 0xa8575a53, KEY_SEARCH},
	{ 0xbc435a53, KEY_OK},
	{ 0xbb445a53, KEY_UP},
	{ 0xb9465a53, KEY_LEFT},
	{ 0xb8475a53, KEY_RIGHT},
	{ 0xba455a53, KEY_DOWN},
	{ 0xbd425a53, KEY_BACK},
	{ 0xe41b5a53, KEY_EXIT},
	{ 0xce315a53, KEY_1},
	{ 0xcd325a53, KEY_2},
	{ 0xcc335a53, KEY_3},
	{ 0xcb345a53, KEY_4},
	{ 0xca355a53, KEY_5},
	{ 0xc9365a53, KEY_6},
	{ 0xc8375a53, KEY_7},
	{ 0xc7385a53, KEY_8},
	{ 0xc6395a53, KEY_9},
	{ 0xcf305a53, KEY_0},
	{ 0xd52a5a53, KEY_FN_E},  /* * */
	{ 0xdc235a53, KEY_FN_F}, /* # */
	{ 0x659a5a53, KEY_FN_2}, /* jiafutong */
	{ 0xb04f5a53, KEY_FAVORITES},
	{ 0xae515a53, KEY_FN_B}, /* program guide */
	{ 0xc03f5a53, KEY_INFO},
	{ 0x5fa05a53, KEY_FN_F1}, /* shuaka */
};

static struct rc_map_list hisi_map = {
	.map = {
		.scan	= hisi_keymap,
		.size	= ARRAY_SIZE(hisi_keymap),
		.rc_type= RC_TYPE_NEC_FULL_40BIT,	/* Legacy IR type */
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