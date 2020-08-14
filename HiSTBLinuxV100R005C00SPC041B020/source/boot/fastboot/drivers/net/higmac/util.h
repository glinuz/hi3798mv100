#ifndef __HIGMAC_UTIL_H__
#define __HIGMAC_UTIL_H__

#define HIGMAC_TRACE_LEVEL 8

#define higmac_trace(level, msg...) do { \
        if ((level) >= HIGMAC_TRACE_LEVEL) { \
            printf("higmac_trace:%s:%d: ", __FILE__, __LINE__); \
            printf(msg); \
            printf("\n"); \
        } \
    } while (0)

#define higmac_error(args...) do { \
        printf("higmac:%s:%d: ", __FILE__, __LINE__); \
        printf(args); \
        printf("\n"); \
    } while (0)

#define higmac_assert(cond) do { \
        if (!(cond)) \
            printf("Assert:higmac:%s:%d\n", \
                   __FILE__, \
                   __LINE__);\
    } while (0)

#define higmac_readl(ld, ofs) ({ unsigned long reg = readl((ld)->iobase + (ofs)); \
			higmac_trace(2, "readl(0x%04X) = 0x%08lX", (ofs), reg); \
			reg; })
			
#define higmac_writel(ld, v, ofs) do { writel(v, (ld)->iobase + (ofs)); \
			higmac_trace(2, "writel(0x%04X) = 0x%08lX", (ofs), (unsigned long)(v)); \
    } while (0)

#define higmac_writel_bits(ld, v, ofs, bits_desc) do { \
			unsigned long _bits_desc = bits_desc; \
			unsigned long _shift = (_bits_desc) >> 16; \
			unsigned long _reg  = higmac_readl(ld, ofs); \
			unsigned long _mask = ((_bits_desc & 0x3F) < 32) ? (((1 << (_bits_desc & 0x3F)) - 1) << (_shift)) : 0xffffffff; \
			higmac_writel(ld, (_reg & (~_mask)) | (((v) << (_shift)) & _mask), ofs); \
    } while (0)

#define higmac_readl_bits(ld, ofs, bits_desc) ({ \
			unsigned long _bits_desc = bits_desc; \
			unsigned long _shift = (_bits_desc) >> 16; \
			unsigned long _mask = ((_bits_desc & 0x3F) < 32) ? (((1 << (_bits_desc & 0x3F)) - 1) << (_shift)) : 0xffffffff; \
			(higmac_readl(ld, ofs) & _mask) >> (_shift); })

#define MK_BITS(shift, nbits) ((((shift) & 0x1F) << 16) | ((nbits) & 0x3F))

#endif

