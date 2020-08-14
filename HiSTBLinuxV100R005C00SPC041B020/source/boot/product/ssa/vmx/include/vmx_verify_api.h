#ifndef __VMX_VERIFY_API_H__
#define __VMX_VERIFY_API_H__

/*signature:	
    is the data array stored as signature value for the image. It is not necessarily an encrypted signature 
src:	
    is the data array of size len. The decrypted image is available there after decryption 	
    for VMX Third-Party Signing Solution:		
        for bootloader, decrypted image will be loaded to 'src', re-encrypted image will be loaded to 'tmp'.		
        for apploader or recovery, re-encrypted image will be loaded to 'src', 'tmp' can be freed after verifySignature returns.
tmp:	
    is the data array of size len (not overlapping with src array) only used temporarily src and tmp might be, depending on some chipset features, restricted to some values, e.g. beginning of a long word address or something like that. 
len:	
    is the length of the data array, it must be a multiple of 16bytes (padded at the end) 
maxLen:	
    defines the max-size of application, it must be greater or equal to len. This length value must be equal to the size of the file that has been used for signing. In most cases this value will be equal to len, as the compile file will be signed. 
mode:	
    gives the mode the verification is done, for flash application (0), to be programmed into the flash before the flash is mounted on the PCB in production or after re-encryption, or for loader (1) to be used by serial or OTA 	for VMX Third-Party Signing Solution, 'mode' is deprecated.
errorCode:	
    is a pointer to a value that tells the reason for not having a valid image  	
    1 means no valid signature found - reboot  	
    2 means store src part to flash and reboot after it 
return value: 	
    0 indicates do not start the application 	
    1 indicates the application can be started 	
    If this function returns the result 1, the application provided in the data-array src can be started. 
    If not, the application must not be started. 
    In case the image should be individually stored into the flash, the image should be stored first 
    and the box should be rebooted after that. If the verification has been called as part of an upgrade process, 
    the signature must be stored as well. Normally this will only happen if the image has been loaded into the flash externally, 
    as all loaders should also use this verification function before storing the image into flash. 
    Therefore, only the individualized image will be stored into flash in this case. 
    It is highly recommended to do the re-encryption in the fab already, as a failure in updating the image in flash could lead to a none-recoverable problem. 	
    In case the return value is 0 and error code is 1, the image and signature are not valid or do not match. 	
    This library implementing this function is never provided to the set top box manufacturer. 
    It is added by Verimatrix when the boot-loader is built and signed. 

Please refer to <ViewRight_STB_DVB_Advanced_Security_Bootloader_vxxx.pdf> from VMX for the detailed information.
*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

int verifySignature( unsigned char* signature, 
					unsigned char	* src, 
					unsigned char* tmp, 
					unsigned int len, 
					unsigned int maxLen, 
					unsigned char mode, 
					unsigned char *errorCode );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__VMX_VERIFY_API_H__*/
