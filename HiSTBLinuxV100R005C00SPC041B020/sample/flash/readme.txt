Procedure

Usage:
./flash_read start_position length  withOOB device_name(can be partition name or blank)
For example: ./flash_read 0x2c00000 0x1000 1 /dev/mtd3
or
./flash_erase start_position length (device_name)
For example: ./flash_erase 0x0 0x80000 (rootfs)
During erase, the start_position and the length must be block aligned.
or
./flash_write start_position length fill value 
For example: ./flash_write 0x80000 0x1000 0x20
or 
./flash_write_yaffs new_rootfs.yaffs

Note:
1)
write_yaffs can use two ways, one way is: calling HI_Flash_Write only once(For example: flash_write_yaffs.c)
the other is: writing yaffs block by block(For example: flash_write_yaffs2.c)
2)
/** 
Run the \brief command to open the flash partition.
\attention \n
\param[in] enFlashType      flash type
\param[in] pPartitionName   partition name
\param[in] u64Address       start position of partition, used only when pPartitionName is NULL
\param[in] u64Len           length of partition, used only when pPartitionName is NULL
\retval fd (file handle) success 
\retval INVALID_FD   failure
\see \n
N/A
*/
HI_HANDLE HI_Flash_Open(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName, HI_U64 u64Address, HI_U64 u64Len);

The device name and access mode are modified in the corresponding test case. For example, in the partition shown below, the device name can be data or /dev/mtd3(not /dev/mtdblock3).
 DevName        TotalSize    BlockSize  PartName
 /dev/mtdblock3,0x100000,    0x20000,   data

