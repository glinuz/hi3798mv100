#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define IO_SPACE_LIMIT  0xffffffff

#define __io(a)         __typesafe_io(a)
#define __mem_pci(a)    (a)

/*  phys_addr		virt_addr 
 * 0xF800_0000 <-----> 0xF900_0000
 */
#define S40_IOCH1_VIRT (0xF9000000)
#define S40_IOCH1_PHYS (0xF8000000)
#define S40_IOCH1_SIZE (0x02000000)

/*  phys_addr		virt_addr 
 * 0xFF000000 <-----> 0xFB000000
 * Only for s5 platform
 */
#define S40_IOCH2_VIRT (0xFB000000)
#define S40_IOCH2_PHYS (0xFF000000)
#define S40_IOCH2_SIZE (0x00430000)

#define IO_OFFSET_LOW  (0x1000000)
#define IO_OFFSET_HIGH (0x4000000)

#define IO_ADDRESS(x)   ((x) >= S40_IOCH2_PHYS ? (x)- IO_OFFSET_HIGH\
		: (x) + IO_OFFSET_LOW)

#define IO_ADDRESS_LOW(x)  ((x) + IO_OFFSET_LOW)
#define IO_ADDRESS_HIGH(x) ((x) - IO_OFFSET_HIGH)

#endif
