#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "hi_type.h"
#include "hi_flash.h"

enum hiflash_command
{
	CMD_NONE,
	CMD_READ,
	CMD_WRITE,
	CMD_ERASE,
	CMD_BUTT
};

static char *device;
static HI_FLASH_TYPE_E type = HI_FLASH_TYPE_BUTT;
int type_set;
static char *file;
static int input_file_set;
static int output_file_set;
static unsigned long long offset;
static int offset_set;
static unsigned long len;
static int len_set;
static int withoob;

static enum hiflash_command cmd;

HI_HANDLE handle;


/*
本命令主要用于hiflash自测。。
sample命令:
擦除:
sample_flash -E [-D device] -T nand|spi|emmc:擦除设备device
sample_flash -E [-D device] -T nand|spi|emmc [-s 0x0] [-l 0x100000]
擦除设备device上从0开始到长度1M结束,emmc 不需要擦除。

读:
sample_flash -R [-D device] -T nand|spi|emmc [-o file] [-s 0] [-l 0x100000] [-O]
把设备device上从0开始，长度为1M的数据读出到file里.如指定-O，则将oob区域读出，前提是device是nand分区

写:
sample_flash -W [-D device] -T nand|spi|emmc [-i file] [-s 0] [-l 0x100000] [-O]
将文件file写入到device的0位置开始,长度为1M的位置。如制定-O,则写入oob区域。

参数解释:
-E 擦除命令。
-R 读命令。
-W 写命令。
-D 设备名.只在操作emmc时可以为空，其他flash类型本例要求必须带上这个参数。因emmc比较特殊，可以不带-D参数，用于操作kernel、boot等分区。
-s 分区内偏移地址。如未指定，默认从分区0偏移处操作。
-l 将要操作的长度。如未指定，默认操作整个分区。所有操作，都不算oob长度在内，程序自动计算。但本例不会做块、页等对齐操作。需要用户自行对齐。
-o 输出文件，如未指定，默认输出到串口.
-i 输入文件，如未制定，默认从内存中生成随机数据。
-O 操作是否带OOB区域。

注:
a. 用[]括起来的参数不是必须的，但-E -T这两个参数必须带。
b. |表示或则，几个数据选其一。比如nand|spi|emmc表示选nand或者spi或者emmc。

!!!!!!!!!!!!!!!! 必须注意的是使用本例时，不能操作操作系统正在使用的分区，包括根文件系统、被mount的分区等。

样例命令:
1. ./sample_flash -R -D /dev/mtd3 -T nand -s 0 -l 0x20000 -O
	从/dev/mtd3上0偏移处开始，读数据长度为0x20000的数据，并且读出oob区域，/dev/mtd3是一个nand分区
2. ./sample_flash -R -D /dev/mtd4 -T nand -s 0 -l 0x10000
	从/dev/mtd4上0偏移处读取0x10000的数据长度，不读数据区，/dev/mtd4是一个nand分区。
3. ./sample_flash  -E -D /dev/mtd5 -T nand
	擦出/dev/mtd5整个分区，整个分区是一个nand分区。
4. ./sample_flash  -W -D /dev/mtd5 -T nand -i rootfs.yaffs -O
	将rootfs.yaffs以带oob写入的方式，写入到/dev/mtd5整个nand 分区上。

5. ./sample_flash -W -D /dev/mmcblk0p7  -T emmc -i /mnt/image/rootfs_80M.ext3 -s 0 -l 0x5000000
	将/mnt/image/rootfs_80M.ext3写入到/dev/mmcblk0p7从0开始长度为0x5000000的位置，flash类型为emmc

6. ./sample_flash -R -D /dev/mmcblk0p7  -T emmc -o /mnt/tmp/read_rootfs_80M.ext3 -s 0 -l 0x5000000
	从/dev/mmcblk0p7的0偏移处开始读取0x5000000长的数据到/mnt/tmp/read_rootfs_80M.ext3，flash类型为emmc

7. ./sample_flash -R -T emmc -s 0x200000 -l 0x300000 -o /mnt/2M3M
	从emmc flash上偏移0x200000处开始，读取0x300000Byte的数据，写入/mnt/2M3M文件里。

8. ./sample_flash -W -T emmc -s 0x200000 -l 0x2e0000 -i ./hi_kernel.bin
	将./hi_kernel.bin文件写入到emmc flash的0x200000M偏移处，最多写0x2e0000Byte数据。

	当不带-D 参数时，隐式表明要以裸读裸写的方式操作emmc器件。因此，必须指定-s和-l参数告诉hiflash将要操作的flash具体位置。
*/
static void usage(char* base)
{
	printf("%s: sample for flash operation.\n"\
		"\t\t%s -R -D device -T nand|spi|emmc [-o output_file] [-s offset_in_partition] [-l length] [-O]\n"\
		"\t\t%s -W -D device -T nand|spi|emmc [-i input_file] [-s offset_in_partition] [-l length] [-O]\n"\
		"\t\t%s -E -D device -T nand|spi|emmc [-s offset_in_partition] [-l length]\n"\
		"\t\t -E: Erase command.\n"\
		"\t\t -R: Read command.\n"\
		"\t\t -W: Write command.\n"\
		"\t\t -D: Device name.\n"\
		"\t\t -T: Tell me which type of flash the device is.\n"\
		"\t\t -s: Start address in device(partition).Default to offset 0.\n"\
		"\t\t -l: Operation length. defualt to full partition length. and not inlcude oob size if -O set\n"\
		"\t\t -o: Output file. Defualt to print to serial.\n"\
		"\t\t -i: Input file. defualt to random value in memory.\n"\
		"\t\t -O: If you wish to operate the oob area, set -O.defualt no.\n",
		  base, base, base, base);

	exit(-1);
}

extern char *optarg;
extern int optind, opterr, optopt;

static void process_option(int argc, char *argv[])
{
	int op;
	opterr = 0;

	while ((op = getopt(argc, argv, "RWED:T:s:l:o:i:O")) != -1) {
		switch (op) {
			case 'R' :
				cmd = CMD_READ;
				printf("command read, ");
				break;
			case 'W' :
				cmd = CMD_WRITE;
				printf("command write, ");
				break;
			case 'E':
				cmd = CMD_ERASE;
				printf("command erase, ");
				break;
			case 'D':
				device = optarg;
				printf("device:%s, ", device);
				break;
			case 'T':
				printf("flash :");
				if (optarg[0] == 'N' || optarg[0] == 'n') {
					type = HI_FLASH_TYPE_NAND_0;
					printf("nand, ");
				} else if (optarg[0] == 'S' || optarg[0] == 's') {
					type = HI_FLASH_TYPE_SPI_0;
					printf("spi, ");
				} else if (optarg[0] == 'E' || optarg[0] == 'e') {
					type = HI_FLASH_TYPE_EMMC_0;
					printf("emmc, ");
				} else {
					type_set = 0;
					break;
				}
				type_set = 1;
				break;
			case 's':
				offset_set = 1;
				offset = strtoull(optarg, 0, 16);
				printf("offset: 0x%llx, ",offset);
				break;
			case 'l':
				len_set = 1;
				len = strtoul(optarg, 0, 16);
				printf("len: 0x%lx, ",len);
				break;
			case 'o':
				output_file_set = 1;
				file = optarg;
				printf("output file: %s, ",file);
				break;
			case 'i':
				input_file_set = 1;
				file = optarg;
				printf("input file: %s, ",file);
				break;
			case 'O':
				withoob = 1;
				printf("with oob, ");
				break;
			case 'h':
			case 'H':
			default:
				usage(argv[0]);
				exit(0);
		}
	}
	printf("\r\n");
}
static int check_param(void)
{
	if (cmd != CMD_ERASE && cmd != CMD_WRITE && cmd != CMD_READ) {
		printf("command %d not right!\n",cmd);
		return -1;
	}
	if (type >= HI_FLASH_TYPE_BUTT || !type_set) {
		printf("flash %d type not right!\n", type);
		return -1;
	}

	if (cmd == CMD_ERASE && (output_file_set || input_file_set)) {
		printf("cmd is erase, but file set!\n");
		return -1;
	}

	if (cmd == CMD_WRITE && output_file_set) {
		printf("cmd is write but output file set!\n");
		return -1;
	}

	if (cmd == CMD_READ && input_file_set) {
		printf("cmd is read but input file set\n");
		return -1;
	}

	if (input_file_set && output_file_set) {
		printf("input file and output file set both!\n");
		return -1;
	}

	if (type != HI_FLASH_TYPE_EMMC_0 && (!device || !device[0])) {
		printf("this program require device param if flash is nand or spi!\n");
		return -1;
	}
	if (type == HI_FLASH_TYPE_EMMC_0 && !offset_set && (!device || !device[0])) {
		printf("emmc require device or offset at least!\n");
		return -1;
	}

	if (withoob && type != HI_FLASH_TYPE_NAND_0) {
		printf("operate oob but flash is not nand!\n");
		return -1;
	}

	return 0;

}
static int print_sn = 0;
static void print_data(unsigned char *bf, int len)
{
	int i;
	for (i = 0; i < len/4; i+=4)
	{
		if (i%16 == 0) {
			printf("%08x: ", i);
		}
		printf("%08x ", *(unsigned int*)(bf + i));

		if (((print_sn + 1) % 4) == 0)
			printf("\n");

		print_sn++;
	}
}
#define BUF_SZ 0x10000UL /* 64KB buffer */
static int flash_sample_read(void)
{
	int fd = -1;
	unsigned char tmp[BUF_SZ];
	int ret, read= 0;
	HI_Flash_InterInfo_S info;
	int step, sz;
	int cnt;
	print_sn = 0;

	if (output_file_set) {
		fd = open(file,O_RDWR|O_CREAT|O_LARGEFILE);
		if (fd < 0) {
			printf("Fail to open %s\n", file);
			perror("Error:");
			exit(-1);
		}
	}

	ret = HI_Flash_GetInfo(handle, &info);
	if (ret) {
		printf("Fail to get info, ret: 0x%x\n",ret);
		exit(-1);
	}

	if (!offset_set)
		offset = 0;
	if (!len_set)
		len = info.PartSize;

	if (info.PageSize)
		cnt = step = info.PageSize;
	else if (info.BlockSize) {
		if (BUF_SZ >= info.BlockSize)
			cnt = step = info.BlockSize;
		else
			cnt = step = BUF_SZ;
	} else {
		printf("Both of pagesize and block size is 0!\n");
		exit(-1);
	}

	if (withoob) {

		if (offset % info.PageSize) {
			printf("Offset must page(%d) aliagn!\n", info.PageSize);
			exit(-1);
		}

		cnt = info.PageSize + info.OobSize;
	}

	if (type != HI_FLASH_TYPE_EMMC_0) {
		/* find max step */
		for (sz = 0; ; sz ++) {
			if (withoob) {
				if (step + info.OobSize +info.PageSize > BUF_SZ && cnt + info.PageSize + info.OobSize > BUF_SZ) {
					break;
				}
				step += info.PageSize;
				cnt += info.PageSize + info.OobSize;
			}
			else {
				if (step + info.PageSize > BUF_SZ && cnt + info.PageSize > BUF_SZ) {
					break;
				}
				step += info.PageSize;
				cnt += info.PageSize;
			}
		}
	}
	printf("Read info: read cnt: %d(may include oob, lie on -O, step :%d(not include oob), len:0x%lx\n", cnt, step, len);
	/* Fix emmc read no-ext partitions. the -s param will be use to open the desired start address .
	 * The -l param will be used to set the desired partition size.
	 * The purpose of command like : ./sample_flash -R -T emmc -s 0x200000 -l 0x200
	 * is want to open partition from 0x200000 and len is 0x200, then read  this partiton whole out.
	 * Notes: In this command, no extended partition specified.
	 */
	if (type == HI_FLASH_TYPE_EMMC_0 && (!device || !device[0])) {
		offset = 0;
	}
	for (;len >= BUF_SZ;) {
		ret = HI_Flash_Read(handle, offset, tmp, cnt,
				withoob ? HI_FLASH_RW_FLAG_WITH_OOB : HI_FLASH_RW_FLAG_RAW);
		if (ret <= 0 || ret != cnt) {
			printf("\nFail to read,ret:0x%x, offset:0x%llx, len:0x%lx, %s, already read:0x%x\n",
				ret, offset, len, withoob?"with oob":"no oob", read);
			if (output_file_set)
				close(fd);
			exit(-1);
		}

		if (output_file_set)
			write(fd, tmp, ret);
		else
			print_data(tmp, ret);

		len -=step;
		read += cnt;
		offset += step;
	}

	if (len < BUF_SZ && len > 0) {

		if (withoob)
			len += (len / info.PageSize) * info.OobSize;

		ret = HI_Flash_Read(handle, offset, tmp, len,
				withoob ? HI_FLASH_RW_FLAG_WITH_OOB : HI_FLASH_RW_FLAG_RAW);
		if (ret < 0 || ret != len) {
			printf("\nFail to read 2, ret:0x%x, offset:0x%llx, len:0x%lx, %s, already read 0x%x\n",
				ret, offset, len, withoob?"with oob":"no oob", read);
			if (output_file_set)
				close(fd);
			exit(-1);
		}

		if (output_file_set)
			write(fd, tmp, ret);
		else
			print_data(tmp, ret);

		read += ret;
		offset += BUF_SZ;
	}

	printf("\ntotal read 0x%x Bytes.\n",read);

	if (output_file_set)
		close(fd);

	return 0;
}
static int flash_sample_write(void)
{
	int fd = -1;
	unsigned char data[BUF_SZ];
	int ret;
	HI_Flash_InterInfo_S info;
	int written = 0;
	int cnt;
	int step;
	int eof = 0;
	int sz ;

	if (input_file_set) {
		fd = open(file, O_RDONLY);
		if (fd < 0) {
			printf("Fail to open %s!\n", file);
			perror("Error:");
			exit(-1);
		}
	}


	ret = HI_Flash_GetInfo(handle, &info);
	if (ret) {
		printf("Failt to getinfo, ret 0x%x\n",ret);
		exit(-1);
	}

	if (!offset_set)
		offset = 0;
	if (!len_set)
		len = info.PartSize;


	if (info.PageSize)
		cnt = step = info.PageSize;
	else if (info.BlockSize) {
		if (BUF_SZ >= info.BlockSize)
			cnt = step = info.BlockSize;
		else
			cnt = step = BUF_SZ;
	} else {
		printf("Both of pagesize and block size is 0!\n");
		exit(-1);
	}

	if (withoob)
	{
		cnt = info.OobSize +info.PageSize;
		if (cnt > BUF_SZ) {
			printf("BUF_SZ too small. Enlarge it and re-compile this program!\n");
			exit(-1);
		}
		step = info.PageSize;
	}

	if (type != HI_FLASH_TYPE_EMMC_0) {

		/* find max step */
		for (sz = 0; ; sz ++) {
			if (withoob) {
				if (step + info.PageSize > BUF_SZ || cnt + info.OobSize +info.PageSize > BUF_SZ) {
					break;
				}
				step += info.PageSize;
				cnt += info.OobSize +info.PageSize;
			}
			else {
				if (step + info.PageSize > BUF_SZ || cnt +info.PageSize > BUF_SZ) {
					break;
				}
				step += info.PageSize;
				cnt += info.PageSize;
			}
		}
	}

	if (info.PageSize)
		printf("Write info: per pages:%d, write step(not include oob) :%d, read count(may include oob, lie on -O) :%d\n",
				step / info.PageSize, step, cnt);
	else
		printf("Write info: Write step(not include oob) :%d, read count(may include oob, lie on -O) :%d\n",
				 step, cnt);

	ret = HI_Flash_Erase(handle, offset, len);
	if (type != HI_FLASH_TYPE_EMMC_0 &&ret != len) {
		printf("Fail to erase offset 0x%llx, len 0x%lx, ret:0x%x\n",offset, len,ret);
		exit(-1);
	}
	else if (type == HI_FLASH_TYPE_EMMC_0 && ret) {
		printf("Fail to erase offset 0x%llx, len 0x%lx, ret:0x%x\n",offset, len,ret);
		exit(-1);
	}
	printf("Erase from 0x%08llx size 0x%lx done!\n",offset, len);

	/* Fix emmc read no-ext partitions. the -s param will be use to open the desired start address .
	 * The -l param will be used to set the desired partition size.
	 * The purpose of command like : ./sample_flash -R -T emmc -s 0x200000 -l 0x200
	 * is want to open partition from 0x200000 and len is 0x200, then read  this partiton whole out.
	 * Notes: In this command, no extended partition specified.
	 */
	if (type == HI_FLASH_TYPE_EMMC_0 && (!device || !device[0])) {
		offset = 0;
	}
	while (len >= step && !eof) {
		if (input_file_set)
			ret = read(fd, data, cnt);
		if (ret < cnt && ret > 0) {
			cnt = ret;
			eof = 1;
		}

		ret = HI_Flash_Write(handle, offset, data, cnt,
				(withoob ? HI_FLASH_RW_FLAG_WITH_OOB : HI_FLASH_RW_FLAG_RAW));
		if (ret < 0 || cnt != ret) {
			printf("Fail to write, ret:0x%x, offset:0x%llx, cnt:%d\n",
				ret, offset,cnt);
			exit(-1);
		}
		written += cnt;
		len -= step;
		offset += step;
	}

	if (len < BUF_SZ && len > 0 && !eof) {

		if (withoob)
			len += (len / info.PageSize) * info.OobSize;

		if (input_file_set)
			ret = read(fd, data, len);
		if (ret < len && ret > 0) {
			len = ret;
			eof = 1;
		}

		ret = HI_Flash_Write(handle, offset, data, len,
				withoob ? HI_FLASH_RW_FLAG_WITH_OOB : HI_FLASH_RW_FLAG_RAW);
		if (ret < 0 || len != ret) {
			printf("Fail to write, ret:0x%x, offset:0x%llx, cnt:%d\n",
				ret, offset,cnt);
			exit(-1);
		}
		written += len;
		len -= len;
		offset += len;
	}

	printf("Written 0x%x Bytes!\n",written);
	if (input_file_set)
		close(fd);

	return 0;

}
static int flash_sample_erase(void)
{
	int ret;
	HI_Flash_InterInfo_S info;

	ret = HI_Flash_GetInfo(handle, &info);
	if (ret) {
		printf("Fail to get info, ret:x0%x\n", ret);
		exit(-1);
	}

	if (!offset_set)
		offset = 0;
	if (!len_set)
		len = info.PartSize;

	ret = HI_Flash_Erase(handle, offset, len);
	if (ret < 0 || ret != len) {
		printf("Fail to erase offset:0x%llx, len:0x%lx, ret:0x%x\n", offset, len, ret);
		exit(-1);
	}

	return 0;
}
int main(int argc, char *argv[])
{
	process_option(argc, argv);

	if (check_param())
		usage(argv[0]);

	handle = HI_Flash_Open(type, device, offset,len);
	if (INVALID_FD == handle) {
		printf("Fail to open %s\n",device);
		exit(-1);
	}

	if (cmd == CMD_READ)
		flash_sample_read();
	else if (cmd == CMD_WRITE)
		flash_sample_write();
	else if (cmd == CMD_ERASE)
		flash_sample_erase();
	else
		usage(argv[0]);

	HI_Flash_Close(handle);
	exit(0);
}
