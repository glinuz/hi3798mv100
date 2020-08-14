#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table hisi_keymap[] = {
/* guang dong guang dian and U hudong */
	{ 0xef411047, KEY_POWER},
	{ 0xdc412347, KEY_TV},
	{ 0xe5411a47, KEY_PROGRAM},
	{ 0xe6411947, KEY_EMAIL},
	{ 0xe8411747, KEY_FAVORITES},
	{ 0xee411147, KEY_MUTE},
	{ 0xe4411b47, KEY_INFO},
	{ 0xdd412247, KEY_OK},
	{ 0xe1411e47, KEY_UP},
	{ 0xdf412047, KEY_LEFT},
	{ 0xde412147, KEY_RIGHT},
	{ 0xe0411f47, KEY_DOWN},
	{ 0xe3411c47, KEY_BACK},
	{ 0xe2411d47, KEY_EXIT},
	{ 0xea411547, KEY_PAGEUP},
	{ 0xe9411647, KEY_PAGEDOWN},
	{ 0xec411347, KEY_VOLUMEUP},
	{ 0xeb411447, KEY_VOLUMEDOWN},
	{ 0xe7411847, KEY_HOME},
	{ 0xd5412a47, KEY_RED},
	{ 0xd4412b47, KEY_GREEN},
	{ 0xd3412c47, KEY_YELLOW},
	{ 0xd2412d47, KEY_BLUE},
	{ 0xd0412f47, KEY_1},
	{ 0xcf413047, KEY_2},
	{ 0xce413147, KEY_3},
	{ 0xcd413247, KEY_4},
	{ 0xcc413347, KEY_5},
	{ 0xcb413447, KEY_6},
	{ 0xca413547, KEY_7},
	{ 0xc9413647, KEY_8},
	{ 0xc8413747, KEY_9},
	{ 0xd1412e47, KEY_0},
	{ 0xc7413847, KEY_FN_1},  /*  */
	{ 0xc6413947, KEY_FN_2},  /* # */
	{ 0xd6412947, KEY_RECORD},
	{ 0xd9412647, KEY_PAUSE},
	{ 0xda412547, KEY_STOP},
	{ 0xdb412447, KEY_PLAY},
	{ 0xd7412847, KEY_REWIND},
	{ 0xd8412747, KEY_FORWARD},
	{ 0xed411247, KEY_AUDIO},
};

static struct rc_map_list hisi_map = {
	.map = {
		.scan	= hisi_keymap,
		.size	= ARRAY_SIZE(hisi_keymap),
		.rc_type= RC_TYPE_NEC_2HDR_40BIT,	/* Legacy IR type */
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