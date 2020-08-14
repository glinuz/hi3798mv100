#ifndef _HISI_TUI_TOUCHSCREEN_
#define _HISI_TUI_TOUCHSCREEN_

#include "sre_list.ph"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
#define TS_TUI_MAX_FINGER (10)

struct ts_tui_finger {
	int status;
	int x;
	int y;
	int area;
	int pressure;
	int orientation;
	int major;
	int minor;
	int event;
};

/*lint -e754 -esym(754,*)*/
struct ts_tui_fingers {
	struct ts_tui_finger fingers[TS_TUI_MAX_FINGER];
	int cur_finger_number;
	unsigned int gesture_wakeup_value;
	unsigned int special_button_key;
	unsigned int special_button_flag;
};
/*lint -e754 +esym(754,*)*/

typedef struct {
	int irq_type;
	union {
		struct ts_tui_fingers tui_notify_data;
		int reserved[32];/*lint !e754 */
	};
} tp_notify_data_t;

extern char *tp_notify_addr;

enum touch_device_type {
	SYNATPICS_DEVICE,
	ATMEL_DEVICE,
};

struct tp_rect {
	int	left;
	int	top;
	int	right;
	int	bottom;
	int	bpp;
};

struct emui_tp {
	int status;
	int x;
	int y;
};

struct tui_tp_t {
	int secure;
	unsigned int tp_info_addr;
	char *notify_addr;
	int type;
};

#define VOID void
#define MASK_16BIT (0xFFFF)
#define MASK_8BIT (0xFF)
#define MASK_7BIT (0x7F)
#define MASK_6BIT (0x3F)
#define MASK_5BIT (0x1F)
#define MASK_4BIT (0x0F)
#define MASK_3BIT (0x07)
#define MASK_2BIT (0x03)
#define MASK_1BIT (0x01)

#define F12_FINGERS_TO_SUPPORT 10
#define PDT_PROPS (0x00EF)
#define PDT_START (0x00E9)
#define PDT_END (0x000A)
#define PDT_ENTRY_SIZE (0x0006)
#define PAGES_TO_SERVICE (10)

struct synaptics_rmi4_f12_extra_data {
	unsigned char data1_offset;
	unsigned char data15_offset;
	unsigned char data15_size;
	unsigned char data15_data[(F12_FINGERS_TO_SUPPORT + 7) / 8];
};

struct synaptics_rmi4_f12_query_8 {
	union {
		struct {
			unsigned char size_of_query9;
			struct {
				unsigned char data0_is_present: 1;
				unsigned char data1_is_present: 1;
				unsigned char data2_is_present: 1;
				unsigned char data3_is_present: 1;
				unsigned char data4_is_present: 1;
				unsigned char data5_is_present: 1;
				unsigned char data6_is_present: 1;
				unsigned char data7_is_present: 1;
			};
			struct {
				unsigned char data8_is_present: 1;
				unsigned char data9_is_present: 1;
				unsigned char data10_is_present: 1;
				unsigned char data11_is_present: 1;
				unsigned char data12_is_present: 1;
				unsigned char data13_is_present: 1;
				unsigned char data14_is_present: 1;
				unsigned char data15_is_present: 1;
			};
		};
		unsigned char data[3];
	};
};

/*
 * struct synaptics_rmi4_fn_desc - function descriptor fields in PDT
 * @query_base_addr: base address for query registers
 * @cmd_base_addr: base address for command registers
 * @ctrl_base_addr: base address for control registers
 * @data_base_addr: base address for data registers
 * @intr_src_count: number of interrupt sources
 * @fn_number: function number
 */
struct synaptics_rmi4_fn_desc {
	union {
		struct {
			unsigned char query_base_addr;
			unsigned char cmd_base_addr;
			unsigned char ctrl_base_addr;
			unsigned char data_base_addr;
			unsigned char intr_src_count: 3;
			unsigned char reserved_1: 2;
			unsigned char fn_version: 2;
			unsigned char reserved_2: 1;
			unsigned char fn_number;
		};
		unsigned char data[6];
	};
};

/*
 * synaptics_rmi4_fn_full_addr - full 16-bit base addresses
 * @query_base: 16-bit base address for query registers
 * @cmd_base: 16-bit base address for data registers
 * @ctrl_base: 16-bit base address for command registers
 * @data_base: 16-bit base address for control registers
 */
struct synaptics_rmi4_fn_full_addr {
	unsigned short query_base;
	unsigned short cmd_base;
	unsigned short ctrl_base;
	unsigned short data_base;
};

/*
 * struct synaptics_rmi4_fn - function handler data structure
 * @fn_number: function number
 * @num_of_data_sources: number of data sources
 * @num_of_data_points: maximum number of fingers supported
 * @size_of_data_register_block: data register block size
 * @data1_offset: offset to data1 register from data base address
 * @intr_reg_num: index to associated interrupt register
 * @intr_mask: interrupt mask
 * @full_addr: full 16-bit base addresses of function registers
 * @link: linked list for function handlers
 * @data_size: size of private data
 * @data: pointer to private data
 */
struct synaptics_rmi4_fn {
	unsigned char fn_number;
	unsigned char num_of_data_sources;
	unsigned char num_of_data_points;
	unsigned char size_of_data_register_block;
	unsigned char data1_offset;
	unsigned char intr_reg_num;
	unsigned char intr_mask;
	struct synaptics_rmi4_fn_full_addr full_addr;
	LIST_OBJECT_S link;
	int data_size;
	void *data;
	void *extra;
};

struct mxt_tui_data {
	char device_name[10];
	u8 max_reportid;
	u16 T5_address;
	u8 T5_msg_size;
	u16 T44_address;
	u16 T100_address;
	u8 T100_reportid_min;
	u8 T100_reportid_max;
	u16 addr;
} tui_mxt_data;


/*
 *   Read/Write interface:
 *
 *   chip_addr:         I2C chip address
 *   buf:               Where to read/write the data,
 *                      reg should be writen in buf[0]
 *   len:               How many bytes to read/write
 *
 *   Returns:           0 on success, not 0 on failure
 */
extern int hisi_tui_get_tpdata(struct ts_tui_finger *finger_data_buf);
void set_kb_rect(int mode);
#endif
