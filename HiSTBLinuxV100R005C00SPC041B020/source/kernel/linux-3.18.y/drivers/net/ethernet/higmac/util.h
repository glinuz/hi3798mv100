#ifndef __HIGMAC_UTIL_H__
#define __HIGMAC_UTIL_H__

#define HIGMAC_TRACE_LEVEL 10

#define higmac_trace(level, msg...) do { \
	if ((level) >= HIGMAC_TRACE_LEVEL) { \
		pr_info("higmac_trace:%s:%d: ", __FILE__, __LINE__); \
		printk(msg); \
		printk("\n"); \
	} \
} while (0)

#define higmac_error(args...) do { \
	pr_err("higmac:%s:%d: ", __FILE__, __LINE__); \
	printk(args); \
	printk("\n"); \
} while (0)

#define higmac_assert(cond) do { \
	if (!(cond)) \
		pr_alert("Assert:higmac:%s:%d\n", \
			__FILE__, \
			__LINE__);\
} while (0)

#define higmac_set_bits(var, v, bits_desc) do { \
			unsigned int _bits_desc = bits_desc; \
			unsigned int _shift = (_bits_desc) >> 16; \
			unsigned int _mask = ((_bits_desc & 0x3F) < 32) ?\
			(((1 << (_bits_desc & 0x3F)) - 1) << (_shift)) \
			: 0xffffffff; \
			var = (var & (~_mask)) | (((v) << (_shift)) & _mask); \
	} while (0)
#define higmac_get_bits(var, bits_desc) ({ \
			unsigned int _bits_desc = bits_desc; \
			unsigned int _shift = (_bits_desc) >> 16; \
			unsigned int _mask = ((_bits_desc & 0x3F) < 32) ? \
			(((1 << (_bits_desc & 0x3F)) - 1) << (_shift)) \
			: 0xffffffff; \
			(var & _mask) >> (_shift); })
#define MK_BITS(shift, nbits) ((((shift) & 0x1F) << 16) | ((nbits) & 0x3F))

#endif
