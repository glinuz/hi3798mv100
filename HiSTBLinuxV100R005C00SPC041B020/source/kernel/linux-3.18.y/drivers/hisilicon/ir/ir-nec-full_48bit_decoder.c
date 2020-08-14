/* ir-nec-decoder.c - handle NEC IR Pulse/Space protocol
 *
 * Copyright (C) 2010 by Mauro Carvalho Chehab
 *
 * This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <linux/bitrev.h>
#include <linux/module.h>
#include "rc-core-priv.h"

/* used for nec full repeat 48bits format */
#define NEC_UNIT                     562500  /* ns */
#define NEC_FULL_48_HEADER_PULSE    (13 * NEC_UNIT / 2)
#define NEC_FULL_48_HEADER_SPACE    (3  * NEC_UNIT)
#define NEC_FULL_48_BIT_PULSE       (1  * NEC_UNIT)
#define NEC_FULL_48_BIT_0_SPACE     (1  * NEC_UNIT)
#define NEC_FULL_48_BIT_1_SPACE     (2  * NEC_UNIT)
#define NEC_FULL_48_TRAILER_PULSE   (1  * NEC_UNIT)
#define NEC_FULL_48_TRAILER_SPACE   (10 * NEC_UNIT)
#define NEC_FULL_48_NBITS           48

enum nec_state {
	STATE_INACTIVE,
	STATE_HEADER_SPACE,
	STATE_BIT_USER_PLUSE,
	STATE_BIT_USER_SPACE,
	STATE_BIT_PULSE,
	STATE_BIT_SPACE,
	STATE_TRAILER_PULSE,
	STATE_TRAILER_SPACE,
};

/**
 * ir_nec_full_48bit_decode() - Decode full repeat 48bit NEC pulse or space
 * such as dezhou
 * @dev: the struct rc_dev descriptor of the device
 * @duration: the struct ir_raw_event descriptor of the pulse/space
 *
 * This function returns -EINVAL if the pulse violates the state machine
 */
static int ir_nec_full_48bit_decode(struct rc_dev *dev, struct ir_raw_event ev)
{
	struct nec_dec_ext *data = &dev->raw->nec_ext;
	u32 scancode;
	u32 user;
	u8 address, command;

	if (!(dev->enabled_protocols & RC_BIT_NEC_FULL_48BIT))
		return 0;

	if (!is_timing_event(ev)) {
		if (ev.reset)
			data->state = STATE_INACTIVE;
		return 0;
	}

	IR_dprintk(2, "NEC-full-48bit decode started at state %d (%uus %s)\n",
		   data->state, TO_US(ev.duration), TO_STR(ev.pulse));

	switch (data->state) {
	case STATE_INACTIVE:
		if (!ev.pulse)
			break;

		if (!eq_margin(ev.duration, NEC_FULL_48_HEADER_PULSE, NEC_UNIT))
			break;

		data->count = 0;
		data->bits = 0;

		data->state = STATE_HEADER_SPACE;
		return 0;

	case STATE_HEADER_SPACE:
		if (ev.pulse)
			break;

		if (eq_margin(ev.duration, NEC_FULL_48_HEADER_SPACE, NEC_UNIT)) {
			data->state = STATE_BIT_PULSE;
			return 0;
		}

		break;

	case STATE_BIT_PULSE:
		if (!ev.pulse)
			break;

		if (!eq_margin(ev.duration, NEC_FULL_48_BIT_PULSE, NEC_UNIT / 2))
			break;

		data->state = STATE_BIT_SPACE;
		return 0;

	case STATE_BIT_SPACE:
		if (ev.pulse)
			break;

		data->bits <<= 1;
		if (eq_margin(ev.duration, NEC_FULL_48_BIT_1_SPACE, NEC_UNIT / 2))
			data->bits |= 1;
		else if (!eq_margin(ev.duration, NEC_FULL_48_BIT_0_SPACE, NEC_UNIT / 2))
			break;
		data->count++;

		if (data->count == NEC_FULL_48_NBITS)
			data->state = STATE_TRAILER_PULSE;
		else
			data->state = STATE_BIT_PULSE;

		return 0;

	case STATE_TRAILER_PULSE:
		if (!ev.pulse)
			break;

		if (!eq_margin(ev.duration, NEC_FULL_48_TRAILER_PULSE, NEC_UNIT / 2))
			break;

		data->state = STATE_TRAILER_SPACE;
		return 0;

	case STATE_TRAILER_SPACE:
		if (ev.pulse)
			break;

		if (!geq_margin(ev.duration, NEC_FULL_48_TRAILER_SPACE, NEC_UNIT / 2))
			break;

		user = bitrev32((data->bits >> 16) & 0xFFFFFFFF);
		address = bitrev8((data->bits >> 8) & 0xFF);
		command = bitrev8((data->bits >>  0) & 0xFF);
		scancode = bitrev32(data->bits & 0xFFFFFFFF);

		IR_dprintk(1, "%s scancode = 0x%08x, user:%06x, address:%02x, command:%02x\n",
			__func__, scancode, user, address, command);

		IR_dprintk(1, "NEC-full-48bit scancode 0x%08x\n", scancode);

		rc_keydown(dev, RC_TYPE_NEC, scancode, 0);
		data->state = STATE_INACTIVE;
		return 0;
	}

	IR_dprintk(2, "NEC-full-48bit decode failed at count %d state %d (%uus %s)\n",
		   data->count, data->state, TO_US(ev.duration), TO_STR(ev.pulse));
	data->state = STATE_INACTIVE;
	return -EINVAL;
}

static struct ir_raw_handler nec_full_48bit_handler = {
	.protocols = RC_BIT_NEC_FULL_48BIT,
	.decode    = ir_nec_full_48bit_decode,
};

static int __init ir_nec_decode_init(void)
{
	ir_raw_handler_register(&nec_full_48bit_handler);

	printk(KERN_INFO "IR NEC-full-48bit protocol handler initialized\n");
	return 0;
}

static void __exit ir_nec_decode_exit(void)
{
	ir_raw_handler_unregister(&nec_full_48bit_handler);
}

module_init(ir_nec_decode_init);
module_exit(ir_nec_decode_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab");
MODULE_AUTHOR("Red Hat Inc. (http://www.redhat.com)");
MODULE_DESCRIPTION("NEC IR protocol decoder");
