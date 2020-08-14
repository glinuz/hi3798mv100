/* 
 * the drv_vinput.h is connceted with the vinput driver 
 * the vinput driver locates at kernel/driver/misc/vinput.c
 *
 * */
#ifndef __VINPUT_H__
#define __VINPUT_H__

#define IOCTL_MOUSE_STATUS      _IOW('i', 0x100, unsigned int)
#define IOCTK_KBD_STATUS        _IOW('i', 0x101, unsigned int)
#define IOCTK_TC_STATUS         _IOW('i', 0x102, unsigned int)
#define IOCTK_MUTITC_STATUS     _IOW('i', 0x103, unsigned int)

#define INPUT_UNBLOCK       0
#define INPUT_BLOCK         1
#define INPUT_HALFBLOCK     2
#define INPUT_POWER         116
#define INPUT_RESERVED      0

#endif
