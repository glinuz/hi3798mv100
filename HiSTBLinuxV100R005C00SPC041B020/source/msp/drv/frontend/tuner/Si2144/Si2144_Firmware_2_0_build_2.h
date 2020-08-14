#ifndef _SI2144_FIRMWARE_2_0_BUILD_2_NVM_H_
#define _SI2144_FIRMWARE_2_0_BUILD_2_NVM_H_

#define FIRMWARE_MAJOR        2
#define FIRMWARE_MINOR        0
#define BUILD_VERSION         2

#ifndef __FIRMWARE_STRUCT__
#define __FIRMWARE_STRUCT__
typedef struct firmware_struct {
	unsigned char firmware_len;
	unsigned char firmware_table[16];
} firmware_struct;
#endif /* __FIRMWARE_STRUCT__ */

firmware_struct Si2144_FW_2_0b2[] = {
{ 8 , { 0x05,0x11,0xBA,0x3D,0x51,0xBB,0x0C,0xBF } } // Unpack the NVM into RAM
};

#define FIRMWARE_LINES_2_0b2 (sizeof(Si2144_FW_2_0b2)/(sizeof(firmware_struct)))
#define RAM_SIG_2_0b2 0xBF0CBB51
#define RAM_CRC_2_0b2 0x3CA5

#endif /* _SI2144_FIRMWARE_2_0_BUILD_2_NVM_H_ */
