#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <mach/platform.h>
#include <mach/io.h>

#ifndef __ASSEMBLY__
#  include <asm/io.h>
#endif

#define __io_address(n) (IOMEM(IO_ADDRESS(n)))

#endif
