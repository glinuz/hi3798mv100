#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
//#include <sys/time.h>
//#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee_client_api.h"
#include "tee_client_id.h"


 /* example uuid */
static const TEEC_UUID stb_test_uuid =
{
    0x0E0E0E0E, \
    0x0E0E, \
    0x0E0E, \
        { 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E }
};

#define  	TEST_TZ_OTP_INIT        27 
#define 	TEST_TZ_OTP_DEINIT      28
#define 	TEST_TZ_OTP_SetReset    29
#define 	TEST_TZ_OTP_GetChipID   30
#define 	TEST_TZ_OTP_ReadWord    31
#define 	TEST_TZ_OTP_ReadByte    32
#define 	TEST_TZ_OTP_WriteWord   33
#define 	TEST_TZ_OTP_WriteByte   34

static int otp_read_by_byte(TEEC_Session session, 
                                 unsigned int address,
                                 unsigned int length)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    unsigned char * pu8Buff = NULL;
    unsigned int * pu32 = NULL;
    unsigned int i;

    if((address >= 0x800)||(length == 0)||((address+length)>0x800))
    {
        printf("invalid parameters, address=0x%x length=0x%x.\n", address, length);
        return -1;
    }
    
    pu8Buff = malloc(length);
    if(pu8Buff == NULL)
    {
        printf("malloc buffer failed, length=0x%x.\n", length);
        return -1;
    }
    memset(pu8Buff, 0, length);
    operation.started = 1;
    operation.params[0].value.a = address; 
    operation.params[1].tmpref.buffer = pu8Buff;
    operation.params[1].tmpref.size = length;
    operation.params[2].value.a = length;   //count

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    result = TEEC_InvokeCommand(&session, TEST_TZ_OTP_ReadByte, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        free(pu8Buff);
        return -1;
    }
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    i=0;
    pu32 = (unsigned int *)pu8Buff;
    while(1)
    {
        if((i%0x10)==0)
        {
            printf("\n%04x: ", address+i);
        }
        printf("%08x ", *pu32);
        i=i+4;
        pu32++;
        if(i>=length)
            break;
    }
    printf("\n");
    free(pu8Buff);
    return 0;
}

static int otp_read_by_word(TEEC_Session session, 
                                 unsigned int address,
                                 unsigned int length)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    unsigned char * pu8Buff = NULL;
    unsigned int * pu32 = NULL;
    unsigned int i;

    if((address >= 0x800)||(length == 0)||((length%4)!=0)||((address+length)>0x800))
    {
        printf("invalid parameters, address=0x%x length=0x%x.\n", address, length);
        return -1;
    }
    
    pu8Buff = malloc(length);
    if(pu8Buff == NULL)
    {
        printf("malloc buffer failed, length=0x%x.\n", length);
        return -1;
    }
    memset(pu8Buff, 0, length);
    operation.started = 1;
    operation.params[0].value.a = address; 
    operation.params[1].tmpref.buffer = pu8Buff;
    operation.params[1].tmpref.size = length;
    operation.params[2].value.a = length/sizeof(unsigned int);  //count

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_OTP_ReadWord, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        free(pu8Buff);
        return -1;
    }

    i=0;
    pu32 = (unsigned int *)pu8Buff;
    while(1)
    {
        if((i%0x10)==0)
        {
            printf("\n%04x: ", address+i);
        }
        printf("%08x ", *pu32);
        i=i+4;
        pu32++;
        if(i>=length)
            break;
    }
    printf("\n");
    free(pu8Buff);
    return 0;
}


static int otp_write_by_byte(TEEC_Session session, 
                                 unsigned int address,
                                 unsigned int length,
                                 unsigned int value)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    unsigned char * pu8Buff = NULL;
    unsigned int * pu32 = NULL;
    unsigned int i;

    if((address >= 0x800)||(length == 0)||((address+length)>0x800))
    {
        printf("invalid parameters, address=0x%x length=0x%x.\n", address, length);
        return -1;
    }
    
    pu8Buff = malloc(length);
    if(pu8Buff == NULL)
    {
        printf("malloc buffer failed, length=0x%x.\n", length);
        return -1;
    }
    memset(pu8Buff, (unsigned char)value, length);
    operation.started = 1;
    operation.params[0].value.a = address; 
    operation.params[1].tmpref.buffer = pu8Buff;
    operation.params[1].tmpref.size = length;
    operation.params[2].value.a = length;   //count

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_OTP_WriteByte, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        free(pu8Buff);
        return -1;
    }

#if 0
    i=0;
    pu32 = (unsigned int *)pu8Buff;
    while(1)
    {
        if((i%0x10)==0)
        {
            printf("\n%04x: ", address+i);
        }
        printf("%08x ", *pu32);
        i=i+4;
        pu32++;
        if(i>=length)
            break;
    }
    printf("\n");
#endif    
    free(pu8Buff);
    return 0;
}


static int otp_write_by_word(TEEC_Session session, 
                                 unsigned int address,
                                 unsigned int length,
                                 unsigned int value)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    unsigned char * pu8Buff = NULL;
    unsigned int * pu32 = NULL;
    unsigned int i;

    if((address >= 0x800)||(length == 0)||((length%4)!=0)||((address+length)>0x800))
    {
        printf("invalid parameters, address=0x%x length=0x%x.\n", address, length);
        return -1;
    }
    
    pu8Buff = malloc(length);
    if(pu8Buff == NULL)
    {
        printf("malloc buffer failed, length=0x%x.\n", length);
        return -1;
    }
    pu32 = (unsigned int *)pu8Buff;
    for(i=0;i<length/4;i++)
    {
        *pu32 = value;
        pu32++;
    }
    operation.started = 1;
    operation.params[0].value.a = address; 
    operation.params[1].tmpref.buffer = pu8Buff;
    operation.params[1].tmpref.size = length;
    operation.params[2].value.a = length/sizeof(unsigned int);   //count

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_OTP_WriteWord, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        free(pu8Buff);
        return -1;
    }
#if 0
    i=0;
    pu32 = (unsigned int *)pu8Buff;
    while(1)
    {
        if((i%0x10)==0)
        {
            printf("\n%04x: ", address+i);
        }
        printf("%08x ", *pu32);
        i=i+4;
        pu32++;
        if(i>=length)
            break;
    }
    printf("\n");
#endif    
    free(pu8Buff);
    return 0;
}


static int otp_get_chipid(TEEC_Session session)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    uint8_t chipid[8];
    uint32_t i;

    memset(chipid, 0x0, 8);
    operation.started = 1;
    operation.params[0].value.a = 0; 
    operation.params[1].tmpref.buffer = (void *)chipid;
    operation.params[1].tmpref.size = sizeof(chipid);
    operation.params[2].value.a = sizeof(chipid);

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_OTP_GetChipID, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }
    printf("ChipId:0x");
    for (i = 0; i < 8; i++)
    {
        printf("%02x", chipid[i]);
    }
    printf("\n");

    return 0;
}

static int otp_reset(TEEC_Session session)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    unsigned int chipid = 0;

    operation.started = 1;
    operation.params[0].value.a = 0; 
    operation.params[1].tmpref.buffer = (void *)&chipid;
    operation.params[1].tmpref.size = sizeof(chipid);
    operation.params[2].value.a = sizeof(chipid);

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_OTP_SetReset, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}

static int otp_init(TEEC_Session session)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    unsigned int chipid = 0;

    operation.started = 1;
    operation.params[0].value.a = 0; 
    operation.params[1].tmpref.buffer = (void *)&chipid;
    operation.params[1].tmpref.size = sizeof(chipid);
    operation.params[2].value.a = sizeof(chipid);

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_OTP_INIT, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}

static int otp_deinit(TEEC_Session session)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    unsigned int chipid = 0;

    operation.started = 1;
    operation.params[0].value.a = 0; 
    operation.params[1].tmpref.buffer = (void *)&chipid;
    operation.params[1].tmpref.size = sizeof(chipid);
    operation.params[2].value.a = sizeof(chipid);

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_OTP_DEINIT, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}

int main(int argc,char *argv[])
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    unsigned int mode = 0;
    unsigned int address = 0;
    unsigned int length = 0;
    unsigned int value = 0;

    usleep(100*1000);
    
    if (argc <= 1)
    {
        printf("Usage: teec_otp mode [address] [length] [value]\n"
               "       mode: 0--read by bytes\n"
               "             1--read by words\n"
               "             2--write by bytes\n"
               "             3--write by words\n"
               "             4--get chipid\n"
               "             5--reset otp\n"
               "       length: unit is bytes\n");
        printf("Example:./teec_otp 0 0 0x100\n");
        return -1;
    }
        
    if (argc >= 5)
    {
        value = strtol(argv[4],NULL,0);
        printf("value = 0x%x\n",value);
    }
        
    if (argc >= 4)
    {
        length = strtol(argv[3],NULL,0);
        printf("length = 0x%x\n",length);
    }

    if (argc >= 3)
    {
        address = strtol(argv[2],NULL,0);
        printf("address = 0x%x\n",address);
    }

    if (argc >= 2)
    {
        mode = strtol(argv[1],NULL,0);
        printf("mode = %d\n",mode);
    }

    usleep(100*1000);

    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        printf("teec initial failed\n");
        goto cleanup_1;
    }
   
    result = TEEC_OpenSession(&context, &session, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        printf("teec open session failed\n");
        goto cleanup_2;
    }

    switch(mode)
    {
        case 0: //read by byte
            otp_read_by_byte(session, address, length);
            break;
        case 1: //read by word
            otp_read_by_word(session, address, length);
            break;
        case 2: //write by byte
            otp_write_by_byte(session, address, length, value);
            break;
        case 3: //write by word
            otp_write_by_word(session, address, length, value);
            break;
        case 4: //get chipid
            otp_get_chipid(session);
            break;
        case 5: //otp reset
            otp_reset(session);
            break;
        default:
            printf("invalid mode=%d\n", mode);
            break;
    }

    usleep(100*1000);
    
    TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_1:
    return 0;
}
