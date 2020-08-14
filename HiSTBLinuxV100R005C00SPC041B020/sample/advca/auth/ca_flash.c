#include <stdio.h>
#include <stdlib.h>
#include <cutils/log.h>

#include "ca_flash.h"

#define SAMPLE_PRINTF       printf

HI_S32 CA_flash_GetPartitionSize(HI_CHAR *pPartionName, HI_U32 *size)
{    
    HI_S32 ret = 0;
    HI_HANDLE flashhandle;
    HI_Flash_InterInfo_S FlashInfo;
    
    flashhandle = HI_Flash_OpenByName(pPartionName);
    if (HI_INVALID_HANDLE == flashhandle)
    {        
        SAMPLE_PRINTF("HI_Flash_Open error\n");        
        return -1;    
    } 
    
    ret = HI_Flash_GetInfo(flashhandle, &FlashInfo); 
    if (HI_FAILURE == ret)    
    {      
        SAMPLE_PRINTF("HI_Flash_GetInfo partion %s error\n", pPartionName); 
        HI_Flash_Close(flashhandle);        
        return -1;    
    }
    
    *size = (HI_U32)FlashInfo.PartSize;
    HI_Flash_Close(flashhandle);

    return 0;
}

HI_S32 CA_flash_read(HI_CHAR *pPartionName, HI_U64 offset, HI_U32 bytes, HI_U8 * buf, HI_Flash_InterInfo_S *flashInfo)
{    
    HI_S32 ret = 0;    
    HI_HANDLE flashhandle;
    
    flashhandle = HI_Flash_OpenByName(pPartionName);
    if (HI_INVALID_HANDLE == flashhandle)    
    {        
        SAMPLE_PRINTF("HI_Flash_Open error\n");   
        return -1;    
    }
    
    if (HI_NULL != flashInfo) 
    {       
        ret = HI_Flash_GetInfo(flashhandle, flashInfo);     
        if (HI_FAILURE == ret)       
        {         
            SAMPLE_PRINTF("HI_Flash_GetInfo partion %s error\n", pPartionName);   
            HI_Flash_Close(flashhandle);           
            return -1;   
        }   
    }    
    
    ret = HI_Flash_Read(flashhandle, offset, buf, bytes, HI_FLASH_RW_FLAG_RAW);  
    if (HI_FAILURE == ret || HI_FLASH_END_DUETO_BADBLOCK == ret)
    {     
        SAMPLE_PRINTF("HI_Flash_Read partion %s error\n", pPartionName); 
        HI_Flash_Close(flashhandle);     
        return -1;    
    } 
    
    HI_Flash_Close(flashhandle);  
    
    return 0;
}
