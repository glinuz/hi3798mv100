/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Yan JianTao
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/
#include <command.h>
#include <cmd-words.h>
#include <debug.h>

/***************************************************************************/

#define CMD_ASSERT(api, ret) \
	do{ \
		int l_ret = api; \
		if (l_ret != 0) \
		{ \
			pr_error("run %s failed, ERRNO:%#x.\n", #api, l_ret); \
			return -1;\
		} \
	ret = l_ret;\
	}while(0)
/**************************************************************************/

int burnflash(uint32 addr, char *partname) __attribute__ ((weak));

static struct cmd_word_t cw_ca = {
	"ca", CMDWORD_TYPE_KEYWORD, _T("ca command sets.")
};

static struct cmd_word_t cw_ext_burnflash = {
	"burnflash", CMDWORD_TYPE_KEYWORD, _T("write data to flash part.")
};

static int do_burnflash(struct cmd_t * cmd)
{
	if (burnflash) {
		int param_index = param_first(cmd);
		uint32 addr = param_int32(cmd, param_index++);
		char *partname = param_str(cmd, param_index++);

		return burnflash(addr, partname);
	}

	pr_error("Not support this command.\n");
	return -1;
}

CMD({&cw_ca, NULL},
    {&cw_ext_burnflash, NULL},
    {&cw_addr, NULL},
    {&cw_string, 0, do_burnflash})

//CA command:
static struct cmd_word_t CA_otp_CustomerKey_show = {
	"getCustomerKey", CMDWORD_TYPE_KEYWORD, _T(" get CustomerKey. ")
};

static struct cmd_word_t CA_otp_GetChipID_show = {
	"getGetChipID", CMDWORD_TYPE_KEYWORD, _T(" get GetChipID. ")
};

static struct cmd_word_t CA_otp_GetStbSN_show = {
	"getSTBSN", CMDWORD_TYPE_KEYWORD, _T(" get STBSN. ")
};

static struct cmd_word_t CA_otp_GetMarketID_show = {
	"getMarketID", CMDWORD_TYPE_KEYWORD, _T(" get MarketID. ")
};

static struct cmd_word_t CA_otp_SecureBootEn_show = {
	"getSecureBootEn", CMDWORD_TYPE_KEYWORD, _T(" get Secure BootEn flag. ")
};

static struct cmd_word_t CA_otp_ReadAll_show = {
	"otpreadall", CMDWORD_TYPE_KEYWORD, _T(" read otp ,for example otpreadall ")
};

static struct cmd_word_t CA_otp_WriteAddress_show = {
	"otpwrite", CMDWORD_TYPE_KEYWORD, _T(" Set OTP  data. ")
};

static struct cmd_word_t cw_partionname = {
	"<partionname>", CMDWORD_TYPE_STRING, _T("partionname name, such as: boot, bootargs, kernel, system.")
};

static struct cmd_word_t cw_flag = {
	"flag", CMDWORD_TYPE_SIZE, _T("Flag, such as: 0 or 1")
};

static struct cmd_word_t CA_BurnFlashName_show = {
	"aa_BurnFlashName", CMDWORD_TYPE_KEYWORD, _T(" Encrypt DDR image with R2R Key-ladder and burn DDR image into flash, for example: BurnFlashName  DDRAddress FlashPartition (eg: BurnFlashName 82000000 kernel). ")
};

static struct cmd_word_t CA_BurnFlashNameByLen_show = {
	"b_BurnFlashNameByLen", CMDWORD_TYPE_KEYWORD, _T(" - burn DDR image to  flash_patition_name, for example: BurnFlashName  DDRAddress ImageLen FlashPartition (eg: BurnFlashName 82000000 80000 logo). ")
};

static struct cmd_word_t CA_DecryptFlashImage2DDR_show = {
	"cc_DecryptFlashPartition", CMDWORD_TYPE_KEYWORD, _T(" - decrypt flash_patition_name to DDR, for example: DecryptFlashPartition kernel. ")
};

static struct cmd_word_t CA_FlashAuthenticateDebug_show = {
	"verify", CMDWORD_TYPE_KEYWORD, _T(" - verify flash_patition_name, for example: verify bootpara. ")
};

static struct cmd_word_t CA_FlashAuthenticateAddr_show = {
	"dd_verifyaddr", CMDWORD_TYPE_KEYWORD, _T(" - verifyaddr flash_patition_Addr, for example: verifyaddr 0x100000. ")
};

static struct cmd_word_t CA_AuthenticateBootParaDebug_show = {
	"ee_verifybootargs", CMDWORD_TYPE_KEYWORD, _T(" - verify bootargs, for example: verifybootargs.")
};

#if 0
static struct cmd_word_t CA_cipher_cbc_mac_test_show = {
	"cipher_cbc_mac_test", CMDWORD_TYPE_KEYWORD, _T(" cipher cbc mac test. ")
};
#endif

static struct cmd_word_t CA_common_verify_image_signature_show = {
	"b_common_verify_image_signature", CMDWORD_TYPE_KEYWORD, _T(" verify pariton-image signature with tail mode, for example: common_verify_image_signature imgepartionname signpartionname signoffsetinpartion ")
};

static struct cmd_word_t CA_common_verify_signature_check_show = {
	"v_common_verify_signature_check", CMDWORD_TYPE_KEYWORD, _T(" verify BootArgs signature_check,for example: common_verify_signature_check, please set signature_check 'Image,Image_Signature,SignatureOffset' ")
};

static struct cmd_word_t CA_common_verify_encryptimage_show = {
	"g_common_verify_encryptimage", CMDWORD_TYPE_KEYWORD, _T(" Encrypt image for Digital Signature,for example: common_verify_encryptimage Image Image_Signature")
};

static struct cmd_word_t CA_common_verify_bootargs_show = {
	"common_verify_bootargs", CMDWORD_TYPE_KEYWORD, _T(" for example: common_verify_bootargs.")
};

static struct cmd_word_t CA_Hash_Test_show = {
	"hash", CMDWORD_TYPE_KEYWORD, _T("Clacluate hash, for example: hash type address length  <type:0:sha1, 1:sha2>.")
};

static struct cmd_word_t CA_encryptboot_show = {
	"encryptboot", CMDWORD_TYPE_KEYWORD, _T(" CA Encrypt Boot for example: encryptboot.")
};

static struct cmd_word_t CA_flashtest_show = {
	"flashtest", CMDWORD_TYPE_KEYWORD, _T(" flashtest.")
};

static struct cmd_word_t CA_logotest_show = {
	"ca_logo", CMDWORD_TYPE_KEYWORD, _T(" Display logo for CA, for example: ca_logo baseparamname logoname")
};

int OTP_Get_CustomerKey_test(struct cmd_t *cmd) __attribute__ ((weak));
CMD({&cw_ca, NULL}, {&CA_otp_CustomerKey_show, NULL, OTP_Get_CustomerKey_test}) 

int OTP_GetChipID_test(struct cmd_t *cmd) __attribute__ ((weak));
CMD({&cw_ca, NULL}, {&CA_otp_GetChipID_show, NULL, OTP_GetChipID_test})

int OTP_GetStbSN_test(struct cmd_t *cmd) __attribute__ ((weak));
CMD({&cw_ca, NULL}, {&CA_otp_GetStbSN_show, NULL, OTP_GetStbSN_test})

int OTP_GetMarketID_test(struct cmd_t *cmd) __attribute__ ((weak));
CMD({&cw_ca, NULL}, {&CA_otp_GetMarketID_show, NULL, OTP_GetMarketID_test})

int OTP_GetSecureBootEn(void) __attribute__ ((weak));

static int OTP_GetSecureBootEn_test(struct cmd_t *cmd)
{
	if (OTP_GetSecureBootEn) {
		OTP_GetSecureBootEn();
		return 0;
	}

	pr_error("Not support this command.\n");
	return -1;
}
CMD({&cw_ca, NULL}, {&CA_otp_SecureBootEn_show, NULL, OTP_GetSecureBootEn_test})

int OTP_ReadAll(void) __attribute__ ((weak));
static int OTP_ReadAll_test(struct cmd_t *cmd)
{
	if (OTP_ReadAll) {
		OTP_ReadAll();
		return 0;
	}

	pr_error("Not support this command.\n");
	return -1;
}
CMD({&cw_ca, NULL}, {&CA_otp_ReadAll_show, NULL, OTP_ReadAll_test})

int OTP_WriteAddress(unsigned int addr, unsigned int value) __attribute__ ((weak));
static int OTP_WriteAddress_test(struct cmd_t *cmd)
{
	if (OTP_WriteAddress) {
		unsigned int  addr = 0;
		unsigned int  value = 0;
		int parm_index = 0;

		parm_index = param_first(cmd);
		addr = param_int32(cmd, parm_index++);
		value = param_int32(cmd, parm_index++);

		pr_info("parm_index:%d, addr:0x%x, value:0x%x\n", 
			parm_index, addr, value);

		OTP_WriteAddress( addr,  value);
		return 0;
	}

	pr_error("Not support this command.\n");
	return -1;
}
CMD({&cw_ca, NULL}, {&CA_otp_WriteAddress_show, NULL, },
    {&cw_addr, 0, NULL},
    {&cw_data, 0, OTP_WriteAddress_test})

int CA_EncryptDDRImageAndBurnFlashName(unsigned char* pImageDDRAddress, unsigned char* pPartionName) __attribute__ ((weak));
static int CA_BurnFlashName(struct cmd_t *cmd)
{
	if (CA_EncryptDDRImageAndBurnFlashName) {
		int ret = 0;
		int parm_index = 0;
		unsigned int addr = 0;
		char *value = 0;

		parm_index = param_first(cmd);
		addr = param_int32(cmd, parm_index++);
		value = param_str(cmd, parm_index++);
		pr_info("burn DDR address 0x%x  to flash area %s\n", addr,value);
		CMD_ASSERT(CA_EncryptDDRImageAndBurnFlashName((unsigned char*)addr, 
			   (unsigned char*)value),ret); 
		pr_info("BurnFlashName success\n");
		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}

CMD({&cw_ca, NULL}, {&CA_BurnFlashName_show, NULL},
    {&cw_addr, 0, NULL},
    {&cw_partionname, NULL, CA_BurnFlashName})

int CA_EncryptDDRImageAndBurnFlashNameByLen(unsigned char* pImageDDRAddress, unsigned int u32Len, char* pPartionName) __attribute__ ((weak)); 
static int CA_BurnFlashNameByLen(struct cmd_t *cmd)
{
	if (CA_EncryptDDRImageAndBurnFlashNameByLen) {
		int ret = 0;
		unsigned int addr = 0;
		unsigned int len = 0;
		unsigned char* pImageDDRAddress = NULL;
		int parm_index = 0;
		char *partitionname = NULL;

		parm_index = param_first(cmd);

		addr = param_int32(cmd, parm_index++);
		pImageDDRAddress = (unsigned char*)addr;

		len = param_int32(cmd, parm_index++);

		partitionname = param_str(cmd, parm_index++);

		pr_info("burn DDR address 0x%x  with len = 0x%x to flash area %s\n",
			addr, len, partitionname);

		CMD_ASSERT(CA_EncryptDDRImageAndBurnFlashNameByLen(pImageDDRAddress, 
			   len, partitionname), ret);
		pr_info("BurnFlashName success\n");
		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}

CMD({&cw_ca, NULL}, {&CA_BurnFlashNameByLen_show, NULL},
    {&cw_addr, 0, NULL},
    {&cw_length, 0, NULL},
    {&cw_partionname, NULL, CA_BurnFlashNameByLen})

int CA_DecryptFlashImage2DDR(unsigned char* pPartionName) __attribute__ ((weak));
static int CA_DecryptFlashImage2DDRTest(struct cmd_t *cmd)
{
	if (CA_DecryptFlashImage2DDR) {
		int ret = 0;
		int parm_index = 0;
		char *partitionname = NULL;

		parm_index = param_first(cmd);

		partitionname = param_str(cmd, parm_index++);

		pr_info("CA_DecryptFlashImage2DDR %s\n", partitionname);
		CMD_ASSERT(CA_DecryptFlashImage2DDR((unsigned char*)partitionname),
			   ret);
		pr_info("CA_DecryptFlashImage2DDR success\n");

		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}

CMD({&cw_ca, NULL}, {&CA_DecryptFlashImage2DDR_show, NULL},
    {&cw_partionname, NULL, CA_DecryptFlashImage2DDRTest})

int CA_FlashAuthenticateByName(unsigned char* pPartionName,unsigned int *ImgInDDRAddress) __attribute__ ((weak));
static int CA_FlashAuthenticateDebug(struct cmd_t *cmd)
{
	if (CA_FlashAuthenticateByName) {
		int ret = 0;
		int parm_index = 0;
		char *partitionname = NULL;

		parm_index = param_first(cmd);

		partitionname = param_str(cmd, parm_index++);    

		pr_info("CA_FlashAuthenticateByName %s\n",partitionname);
		CMD_ASSERT(CA_FlashAuthenticateByName((unsigned char*)partitionname,NULL),ret);
		pr_info("CA_FlashAuthenticateByName success\n");

		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}
CMD({&cw_ca, NULL}, {&CA_FlashAuthenticateDebug_show, NULL},
    {&cw_partionname, NULL, CA_FlashAuthenticateDebug})

int CA_FlashAuthenticateByAddr(unsigned int PartionAddr,int bisBootargsArea,unsigned int *ImgInDDRAddress) __attribute__ ((weak));
static int CA_FlashAuthenticateAddrDebug(struct cmd_t *cmd)
{
	if (CA_FlashAuthenticateByAddr) {
		int ret = 0;
		unsigned int addr = 0;
		int parm_index = 0;

		parm_index = param_first(cmd);

		addr = param_int32(cmd, parm_index++);

		pr_info("CA_FlashAuthenticateByAddr 0x%X\n",addr);
		CMD_ASSERT(CA_FlashAuthenticateByAddr(addr,0,&addr),ret);
		pr_info("verify success\n");

		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}
CMD({&cw_ca, NULL}, {&CA_FlashAuthenticateAddr_show, NULL},
    {&cw_addr, 0, CA_FlashAuthenticateAddrDebug})

int CA_AuthenticateBootParaBak(void) __attribute__ ((weak));
static int CA_AuthenticateBootParaDebug(struct cmd_t *cmd)
{
	if (CA_AuthenticateBootParaBak) {
		int ret = 0;

		ret = CA_AuthenticateBootParaBak();
		pr_info("verify success\n"); 

		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}
CMD({&cw_ca, NULL}, {&CA_AuthenticateBootParaDebug_show, NULL, CA_AuthenticateBootParaDebug})

int CA_CommonVerify_Signature(unsigned char *PartitionImagenName, unsigned char *PartitionSignName, unsigned int offset) __attribute__ ((weak));
static int CA_CommonVerify_FlashVerify_ImageSignature(struct cmd_t *cmd)
{
	if (CA_CommonVerify_Signature) {
		int ret = 0;
		int parm_index = 0;
		char *imgepartionname = NULL;
		char *signpartionname = NULL;
		unsigned int offset = 0;

		parm_index = param_first(cmd);
		imgepartionname = param_str(cmd, parm_index++);
		signpartionname = param_str(cmd, parm_index++);
		offset = (unsigned int)param_int32(cmd, parm_index++);
		CMD_ASSERT(CA_CommonVerify_Signature((unsigned char*)imgepartionname, 
						     (unsigned char*)signpartionname, offset), 
			   ret);

		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}

CMD({&cw_ca, NULL}, {&CA_common_verify_image_signature_show, NULL}, 
    {&cw_partionname, NULL},
    {&cw_partionname, NULL},
    {&cw_size, 0, CA_CommonVerify_FlashVerify_ImageSignature})

int CA_CommonVerify_Parse_SignCommand(struct cmd_t *cmd) __attribute__ ((weak));
CMD({&cw_ca, NULL}, {&CA_common_verify_signature_check_show, NULL, CA_CommonVerify_Parse_SignCommand})

int CA_CommonVerify_EncryptImage(unsigned char *PartitionImagenName, unsigned char *PartitionSignName, unsigned int offset) __attribute__ ((weak));
static int CA_CommonVerify_EncryptCommand(struct cmd_t *cmd)
{
	if (CA_CommonVerify_EncryptImage) {
		int ret = 0;
		int parm_index = 0;
		char *imgepartionname = NULL;
		char *signpartionname = NULL;

		parm_index = param_first(cmd);

		imgepartionname = param_str(cmd, parm_index++);
		signpartionname = param_str(cmd, parm_index++);

		CMD_ASSERT(CA_CommonVerify_EncryptImage((unsigned char*)imgepartionname, 
							(unsigned char*)signpartionname, 0),
			   ret);

		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}
CMD({&cw_ca, NULL}, {&CA_common_verify_encryptimage_show, NULL}, 
    {&cw_partionname, NULL},
    {&cw_partionname, NULL, CA_CommonVerify_EncryptCommand})

int CA_CommonVerify_BootPara_Test(void) __attribute__ ((weak));
static int CA_CommonVerify_BootParaDebug(struct cmd_t *cmd)
{
	if (CA_CommonVerify_BootPara_Test) {
		int ret = 0;
		ret = CA_CommonVerify_BootPara_Test();
		pr_info("verify success\n"); 
		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}

CMD({&cw_ca, NULL}, {&CA_common_verify_bootargs_show, NULL, CA_CommonVerify_BootParaDebug})

int Hash_Test(unsigned int g_AuthAlg, unsigned int address, unsigned int lengh) __attribute__ ((weak)); 
static int Hash_test(struct cmd_t *cmd)
{
	if (Hash_Test) {
		int ret = 0;
		unsigned int address;
		unsigned int Lengh;
		unsigned int g_AuthAlg = 0;
		int parm_index = 0;

		parm_index = param_first(cmd);
		g_AuthAlg = param_int32(cmd, parm_index++);
		address = param_int32(cmd, parm_index++);
		Lengh = param_int32(cmd, parm_index++);

		Hash_Test(g_AuthAlg, address, Lengh);
		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}

CMD({&cw_ca, NULL}, {&CA_Hash_Test_show, NULL},
    {&cw_flag, 0, NULL},
    {&cw_addr, 0, NULL},
    {&cw_size, 0, Hash_test})

int CA_EncryptBoot(void) __attribute__ ((weak));
static int CA_EncryptBoot_Test(struct cmd_t *cmd)
{
	if (CA_EncryptBoot) {
		CA_EncryptBoot();
		return 0;
	}

	pr_error("Not support this command.\n");
	return -1;
}
CMD({&cw_ca, NULL}, {&CA_encryptboot_show, NULL, CA_EncryptBoot_Test})

int CA_flash_write(char *pPartionName, unsigned long long offset, unsigned int bytes, unsigned char * buf) __attribute__ ((weak));
static int CA_nand_Test(struct cmd_t *cmd)
{
	if (CA_flash_write) {
		int parm_index = 0;
		char *pPartionName = 0;
		unsigned int FlashWriteSize = 0x00;
		unsigned char *pImageDDRAddress = NULL;
		unsigned int addr = 0X82000000;

		pImageDDRAddress = (unsigned char*)addr;

		pr_info("CA_nand_Test  xxxxx \n");
		parm_index = param_first(cmd);
		pPartionName = param_str(cmd, parm_index++);
		FlashWriteSize = param_int32(cmd, parm_index++);
		pr_info("1111 FlashWriteSize:0x%x, pImageDDRAddress:0x%x\n", 
			FlashWriteSize, pImageDDRAddress);
		CA_flash_write(pPartionName, 0ULL, 
			       FlashWriteSize, pImageDDRAddress); 
		return 0;
	}

	pr_error("Not support this command.\n");
	return -1;

}
CMD({&cw_ca, NULL}, {&CA_flashtest_show, NULL}, 
    {&cw_partionname, NULL},
    {&cw_size, 0,  CA_nand_Test})

int CA_AuthenticateLogoData(unsigned char *u8BaseparamFlashName, unsigned char *u8LogoFlashName) __attribute__ ((weak));

static int CA_DisplayLogoTest(struct cmd_t *cmd)
{
	if (CA_AuthenticateLogoData) {
		int ret = 0;
		int parm_index = 0;
		char *u8BaseparamFlashName = NULL;
		char *u8LogoFlashName = NULL;

		parm_index = param_first(cmd);

		u8BaseparamFlashName = param_str(cmd, parm_index++);
		u8LogoFlashName = param_str(cmd, parm_index++);

		pr_info("CA_AuthenticateLogoData baseparam:%s, "
			"logo:%s\n",u8BaseparamFlashName, u8LogoFlashName);
		CMD_ASSERT(CA_AuthenticateLogoData((unsigned char*)u8BaseparamFlashName, 
			   (unsigned char*)u8LogoFlashName),ret);
		pr_info("CA_AuthenticateLogoData success\n");

		return ret;
	}

	pr_error("Not support this command.\n");
	return -1;
}

CMD({&cw_ca, NULL}, {&CA_logotest_show, NULL}, 
    {&cw_partionname, NULL}, 
    {&cw_partionname, NULL, CA_DisplayLogoTest})
