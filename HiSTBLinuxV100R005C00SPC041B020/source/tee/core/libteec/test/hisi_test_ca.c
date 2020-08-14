#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>


#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_app_load.h"

static const TEEC_UUID stb_test_uuid =
{
    0x0E0E0E0E, \
    0x0E0E, \
    0x0E0E, \
        { 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E }
};

#define TEST_SELF_TEST                   0
#define TEST_SEC_MMZ                     1
#define TEST_CIPHER                      2
#define TEST_VFMW                        3
#define TEST_CRYPTO                      4
#define TEST_CANAL                       5
#define TEST_CHECK_MEM_ACCESS_RIGHT      6000
#define TEST_TEE_PANIC				     6001

#define STB_TEST_ASSERT_PRINT(cond, fmt, arg...) \
        if (!(cond)) { \
            printf("[%s, %d]:" fmt "\n", __func__, __LINE__, ##arg); \
            while(1); \
        }

#define STB_TEST_ASSERT(cond) \
        STB_TEST_ASSERT_PRINT(cond, "%s", "assert");

#define stb_test_printf(fmt, arg...) \
    printf("STB Test: [%s, %d]:" fmt "\n", __func__, __LINE__, ##arg);


#define TEST_MEM_SIZE ((2<<20) - 1)
#define SEC_MMZ_ZONE "SEC-MMZ"

extern int HI_SEC_MMZ_TA2CA(unsigned long TAphyaddr, unsigned long CAphyaddr, unsigned long CopySize);

static int nonsecmmzdev = -1;
static TEEC_Context stb_test_context;
static TEEC_Session stb_test_session;
#if 0
void *alloc_testbuffer(size_t size, void **phyaddr)
{
    struct mmb_info mmi = {0};
    int ret = 0;

    nonsecmmzdev = open("/dev/mmz_userdev", O_RDWR);
    STB_TEST_ASSERT(nonsecmmzdev >= 0);

    mmi.size = TEST_MEM_SIZE;
    mmi.align = 0x100000;
    strncpy(mmi.mmb_name, "secmmzmmb", HIL_MMB_NAME_LEN - 1);
    mmi.mmb_name[HIL_MMB_NAME_LEN - 1] = '\0';

    ret = ioctl(nonsecmmzdev, IOC_MMB_ALLOC, &mmi);
    STB_TEST_ASSERT(!ret);
    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;
    ret = ioctl(nonsecmmzdev, IOC_MMB_USER_REMAP, &mmi);
    STB_TEST_ASSERT(!ret);

    close(nonsecmmzdev);
    if (phyaddr)
        *phyaddr = (void *)(mmi.phys_addr);

    return mmi.mapped;
}

void free_testbuffer(void*virtaddr, void *phyaddr)
{
    struct mmb_info mmi;
    int ret = 0;

    if (nonsecmmzdev == -1) {
        nonsecmmzdev = open("/dev/mmz_userdev", O_RDWR);
        STB_TEST_ASSERT(nonsecmmzdev >= 0);
    }


    mmi.phys_addr = (unsigned long)phyaddr;
    mmi.mapped = virtaddr;

    ioctl(nonsecmmzdev, IOC_MMB_USER_UNMAP, &mmi);

    /*call ioctl to free*/
    ioctl(nonsecmmzdev, IOC_MMB_FREE, &mmi);
}
int test_init_deinit(void)
{
    int i;
    int ret = 0;
    void *pa1 = NULL;
    void *pa2 = NULL;

    stb_test_printf("start\n");
    for (i = 0; i < 10; i++) {
        ret = HI_SEC_MMZ_Init();
        if (ret < 0)
            stb_test_printf("function return %d\n", ret);

        STB_TEST_ASSERT(!ret);

        HI_SEC_MMZ_DeInit();
    }

    ret = HI_SEC_MMZ_Init();
        if (ret < 0)
            stb_test_printf("function return %d\n", ret);

    pa1 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa1");
    pa2 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa2");

    STB_TEST_ASSERT(pa1);
    STB_TEST_ASSERT(pa2);

    ret = HI_SEC_MMZ_Delete((unsigned long)pa1);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    ret = HI_SEC_MMZ_Delete((unsigned long)pa2);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    HI_SEC_MMZ_DeInit();

    return 0;
}

int test_new_delete(void)
{
    int i;
    int ret = 0;
    void *pa1 = NULL;
    void *pa2 = NULL;

    stb_test_printf("start\n");
    ret = HI_SEC_MMZ_Init();
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);

    for (i = 0; i < 20; i++) {
        pa1 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa1");
        pa2 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa2");

        STB_TEST_ASSERT(pa1);
        STB_TEST_ASSERT(pa2);
        STB_TEST_ASSERT(pa1 != pa2);
        //stb_test_printf("pa1=%p, pa2=%p\n", pa1, pa2);

        ret = HI_SEC_MMZ_Delete((unsigned long)pa1);
        if (ret < 0)
            stb_test_printf("function return %d\n", ret);
        STB_TEST_ASSERT(!ret);

        ret = HI_SEC_MMZ_Delete((unsigned long)pa2);
        if (ret < 0)
            stb_test_printf("function return %d\n", ret);
        STB_TEST_ASSERT(!ret);
    }

    HI_SEC_MMZ_DeInit();

    return 0;
}

int test_catota(void)
{
    void *buf1  = NULL;
    void *buf1_phyaddr = NULL;
    void *buf2 = NULL;
    void *buf2_phyaddr = NULL;
    void *pa1  = NULL;
    int ret = 0;

    stb_test_printf("start\n");
    ret = HI_SEC_MMZ_Init();
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);

    STB_TEST_ASSERT(!ret);

    pa1 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa1");

    stb_test_printf("pa1 = %p\n", pa1);

    STB_TEST_ASSERT(pa1);

    buf1 = alloc_testbuffer(TEST_MEM_SIZE, &buf1_phyaddr);
    STB_TEST_ASSERT(buf1);
    buf2 = alloc_testbuffer(TEST_MEM_SIZE, &buf2_phyaddr);
    STB_TEST_ASSERT(buf2);

    memset(buf1, 0x5A, TEST_MEM_SIZE);
    memset(buf2, 0x5A, TEST_MEM_SIZE);

    ret = HI_SEC_MMZ_CA2TA((unsigned long)buf1_phyaddr, (unsigned long)pa1, TEST_MEM_SIZE);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);
//#define CONFIG_SEC_MMZ_TA2CA
#ifndef CONFIG_SEC_MMZ_TA2CA
    stb_test_printf("Warning: TA2CA is not supported, we cannot check the data!! If you want to check the data, define CONFIG_SEC_MMZ_TA2CA!!\n");

    goto out;
#else
    ret = HI_SEC_MMZ_TA2CA((unsigned long)pa1, (unsigned long)buf2_phyaddr, TEST_MEM_SIZE);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    ret = memcmp(buf1, buf2, TEST_MEM_SIZE);
    if (ret != 0)
        stb_test_printf("diff at %d\n", ret);
    STB_TEST_ASSERT(!ret);
#endif
out:
    ret = HI_SEC_MMZ_Delete((unsigned long)pa1);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    HI_SEC_MMZ_DeInit();

    free_testbuffer(buf1, buf1_phyaddr);
    free_testbuffer(buf2, buf2_phyaddr);
    return 0;
}

int test_ta2ta(void)
{
    void *pa1 = 0;
    void *pa2 = 0;
    void *buf1  = NULL;
    void *buf1_phyaddr = NULL;
    void *buf2 = NULL;
    void *buf2_phyaddr = NULL;
    int i;
    int ret = 0;

    stb_test_printf("start\n");
    ret = HI_SEC_MMZ_Init();
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);

    STB_TEST_ASSERT(!ret);

    pa1 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa1");
    pa2 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa2");

    stb_test_printf("pa1 = %p\n", pa1);
    stb_test_printf("pa2 = %p\n", pa2);

    STB_TEST_ASSERT(pa1);
    STB_TEST_ASSERT(pa2);

    buf1 = alloc_testbuffer(TEST_MEM_SIZE, &buf1_phyaddr);
    STB_TEST_ASSERT(buf1);
    buf2 = alloc_testbuffer(TEST_MEM_SIZE, &buf2_phyaddr);
    STB_TEST_ASSERT(buf2);

    memset(buf1, 0x5A, TEST_MEM_SIZE);

    ret = HI_SEC_MMZ_CA2TA((unsigned long)buf1_phyaddr, (unsigned long)pa1, TEST_MEM_SIZE);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    memset(buf2, 0xA5, TEST_MEM_SIZE);
    ret = HI_SEC_MMZ_CA2TA((unsigned long)buf2_phyaddr, (unsigned long)pa2, TEST_MEM_SIZE);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    ret = HI_SEC_MMZ_TA2TA((unsigned long)pa1, (unsigned long)pa2, TEST_MEM_SIZE);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

#ifndef CONFIG_SEC_MMZ_TA2CA
    stb_test_printf("Warning: TA2CA is not supported, we cannot check the data!! If you want to check the data, define CONFIG_SEC_MMZ_TA2CA!!\n");
    goto out;
#else
    memset(buf2, 0xFF, TEST_MEM_SIZE);
    ret = HI_SEC_MMZ_TA2CA((unsigned long)pa2, (unsigned long)buf2_phyaddr, TEST_MEM_SIZE);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    ret = memcmp(buf1, buf2, TEST_MEM_SIZE);
    if (ret != 0)
        stb_test_printf("diff at %d\n", ret);
    STB_TEST_ASSERT(!ret);
#endif
out:
    ret = HI_SEC_MMZ_Delete((unsigned long)pa1);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    ret = HI_SEC_MMZ_Delete((unsigned long)pa2);
    if (ret < 0)
        stb_test_printf("function return %d\n", ret);
    STB_TEST_ASSERT(!ret);

    HI_SEC_MMZ_DeInit();

    free_testbuffer(buf1, buf1_phyaddr);
    free_testbuffer(buf2, buf2_phyaddr);
    return 0;
}

int test_sec_world_api(uint32_t cmd)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t origin;
    int32_t ret = 0;

    result = TEEC_InitializeContext(NULL, &stb_test_context);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto error1;
    }

    result = TEEC_OpenSession(&stb_test_context, &stb_test_session, &stb_test_uuid,
                TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open stb_test_session failed\n");
        goto error2;
    }

    operation.started = 1;

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE,
                TEEC_NONE,
                TEEC_NONE,
                TEEC_NONE);

    result = TEEC_InvokeCommand(&stb_test_session, cmd, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        ret = ERR_IN_SEC_OS;
        goto error3;
    }

    TEEC_CloseSession(&stb_test_session);
    TEEC_FinalizeContext(&stb_test_context);
    return 0;
error3:
    TEEC_CloseSession(&stb_test_session);
error2:
    TEEC_FinalizeContext(&stb_test_context);
error1:
    return ret;
}
#endif

//added by fangjian 00208632
int test_tee_panic()
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t origin;
    int32_t ret = 0;
    
    char in[200] = "fangjian hello\n";
    char inout[120] = "123456789\n";
    
    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_3;
    }

    result = TEEC_OpenSession(&context, &session, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }
    
    result = TEEC_InvokeCommand(&session, TEST_TEE_PANIC, NULL, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }
    
cleanup_1:
    TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_3:
    if (result)
    {
        TEEC_Error("test_tee_panic case failed\n");
    }
    else
    {
        printf("test_tee_panic case pass!!\n");
    }
    
    return result;
}

//added by fangjian 00208632
int test_canal_short_buffer()
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t origin;
    int32_t ret = 0;
    
    char in[200] = "fangjian hello\n";
    char inout[120] = "123456789\n";
    
    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        ret = -1;
        goto cleanup_3;
    }

    result = TEEC_OpenSession(&context, &session, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        ret = -1;
        goto cleanup_2;
    }
    
    operation.started = 1;
    operation.params[0].tmpref.buffer = NULL; 
    operation.params[0].tmpref.size = 0;
    
    operation.params[1].tmpref.buffer = NULL; 
    operation.params[1].tmpref.size = 0;
    
    operation.params[2].tmpref.buffer = in; 
    operation.params[2].tmpref.size = sizeof(in);
    
    operation.params[3].tmpref.buffer = inout; 
    operation.params[3].tmpref.size = sizeof(inout);
    
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
                                            TEEC_MEMREF_TEMP_OUTPUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT);
    printf("[IN]operation.params[0].tmpref.size = %d\n", operation.params[0].tmpref.size);
    result = TEEC_InvokeCommand(&session, TEST_CANAL, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        if (result != TEEC_ERROR_SHORT_BUFFER)//check return_code == TEEC_ERROR_SHORT_BUFFER
        {
            //TEEC_Error("test_canal_short_buffer case failed\n");
            ret = -1;
            goto cleanup_1;
        }
    }
    printf("[OUT]operation.params[0].tmpref.size = %d\n", operation.params[0].tmpref.size);
    printf("[OUT]operation.params[3].tmpref.buffer=%s\n", operation.params[3].tmpref.buffer);
    
    if (operation.params[0].tmpref.size != sizeof(in))//check return_size == sizeof(in)
    {
        //TEEC_Error("test_canal_short_buffer case failed\n");
        ret = -1;
        goto cleanup_1;
    }
    
cleanup_1:
    TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_3:
    if (ret)
    {
        TEEC_Error("test_canal_short_buffer case failed\n");
    }
    else
    {
        printf("test_canal_short_buffer case pass!!\n");
    }
    
    return ret;
}

#define SESSSION_NUM 16
test_16_session(){

    TEEC_Debug("------%s------\n", __FUNCTION__);
    TEEC_Context context;
    TEEC_Session session[SESSSION_NUM];
    TEEC_Result result;
    TEEC_UUID svc_id = TEE_SERVICE_ECHO;
    uint32_t origin;
    int ret = 0;
    int i = 0;
    result = TEEC_InitializeContext(
               NULL,
               &context);

    if(result != TEEC_SUCCESS) {
        ret = -1;
        goto cleanup_1;
    }
    for(i = 0;i<SESSSION_NUM;i++){
        result = TEEC_OpenSession(
                    &context,
                    &session[i],
                    &stb_test_uuid,
                    TEEC_LOGIN_PUBLIC,
                    NULL,
                    NULL,
                    &origin);

        if(result != TEEC_SUCCESS) {
            ret = -1;
            TEEC_Error("open session failed, codes=0x%x, origin=0x%x\n", result, origin);
            goto cleanup_2;
        }
        
        stb_test_printf("session ***%d**** id 0x%x\n",i, session[i].session_id);
    }
    for(i = 0;i<SESSSION_NUM;i++){
        TEEC_CloseSession(&session[i]);
    }
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_1:
    return ret;
}


//added by fangjian 00208632 2015-08-03
int test_smc_switch_time_cost()
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t origin;
    int32_t ret = TEEC_SUCCESS;
    
    char in[200] = "fangjian hello\n";
    char inout[120] = "123456789\n";

    clock_t start, tmp;
    
    start = clock();
    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        ret = -1;
        goto cleanup_3;
    }
    tmp = clock() - start;
    printf("[time] TEEC_InitializeContext=%f s:\n", ((double)tmp) / CLOCKS_PER_SEC);

    start = clock();
    result = TEEC_OpenSession(&context, &session, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        ret = -1;
        goto cleanup_2;
    }
    tmp = clock() - start;
    printf("[time] TEEC_OpenSession=%f s:\n", ((double)tmp) / CLOCKS_PER_SEC);
    
    operation.started = 1;
    operation.params[0].tmpref.buffer = NULL; 
    operation.params[0].tmpref.size = 0;
    
    operation.params[1].tmpref.buffer = NULL; 
    operation.params[1].tmpref.size = 0;
    
    operation.params[2].tmpref.buffer = in; 
    operation.params[2].tmpref.size = sizeof(in);
    
    operation.params[3].tmpref.buffer = inout; 
    operation.params[3].tmpref.size = sizeof(inout);
    
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
                                            TEEC_MEMREF_TEMP_OUTPUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT);
    start = clock();
    result = TEEC_InvokeCommand(&session, TEST_CANAL, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        //TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        if (result != TEEC_ERROR_SHORT_BUFFER)//check return_code == TEEC_ERROR_SHORT_BUFFER
        {
            //TEEC_Error("test_canal_short_buffer case failed\n");
            ret = -1;
            goto cleanup_1;
        }
    }
    tmp = clock() - start;
    printf("[time] TEEC_InvokeCommand=%f s:\n", ((double)tmp) / CLOCKS_PER_SEC);
    
cleanup_1:
    start = clock();
    TEEC_CloseSession(&session);
    tmp = clock() - start;
    printf("[time] TEEC_CloseSession=%f s:\n", ((double)tmp) / CLOCKS_PER_SEC);
    
cleanup_2:
    start = clock();
    TEEC_FinalizeContext(&context);
    tmp = clock() - start;
    printf("[time] TEEC_FinalizeContext=%f s:\n", ((double)tmp) / CLOCKS_PER_SEC);

cleanup_3:
    
    return ret;
}


//added by fangjian 00208632 2015-08-18
#if 1
#define BIG_SIZE                    (1024 * 1024 * 4)
#define  OFFSET_SIZE             (1026)

#define CMD_TEE_CheckMemoryAccessRight  TEST_CHECK_MEM_ACCESS_RIGHT

#define TEE_MEMORY_ACCESS_READ                  0x1
#define TEE_MEMORY_ACCESS_WRITE                0x2
#define TEE_MEMORY_ACCESS_ANY_OWNER       0x4

#define ANY_OWNER_NOT_SET                             (0)
#define ANY_OWNER_NOT_SET_ACCESS_READ            TEE_MEMORY_ACCESS_READ
#define ANY_OWNER_NOT_SET_ACCESS_READ_WRITE    (TEE_MEMORY_ACCESS_READ | TEE_MEMORY_ACCESS_WRITE)
#define ANY_OWNER_NOT_SET_ACCESS_WRITE             TEE_MEMORY_ACCESS_WRITE
#define ANY_OWNER_SET                                           TEE_MEMORY_ACCESS_ANY_OWNER
#define ANY_OWNER_SET_ACCESS_READ                 (TEE_MEMORY_ACCESS_ANY_OWNER | TEE_MEMORY_ACCESS_READ)
#define ANY_OWNER_SET_ACCESS_READ_WRITE    (TEE_MEMORY_ACCESS_ANY_OWNER | TEE_MEMORY_ACCESS_READ | TEE_MEMORY_ACCESS_WRITE)
#define ANY_OWNER_SET_ACCESS_WRITE                 (TEE_MEMORY_ACCESS_ANY_OWNER | TEE_MEMORY_ACCESS_WRITE)


#define DO_FUNC_AND_PRINT(FUNC) do{ \
    result = FUNC(); \
    if (result){ \
        printf("STB Test: %s() failed\n", #FUNC); \
        count_error++; \
    } else{ \
        printf("STB Test: %s() passed\n", #FUNC); \
    } \
    cnt_total++; \
}while(0);


TEEC_Result Invoke_CheckMemoryAccessRight_BufferReadWrite_AccessRead_success()
{
    TEEC_Context context02;
    TEEC_Session session01;
    TEEC_Result result;
    uint32_t origin, i;
    TEEC_Operation operation;
    memset(&operation, 0, sizeof(operation));
    TEEC_SharedMemory SharedMem0;
    //InitializeContext
    result = TEEC_InitializeContext(NULL, &context02);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_1;
    }
    
    SharedMem0.buffer = NULL;
    SharedMem0.size = BIG_SIZE;
    SharedMem0.flags = TEEC_MEM_INOUT;//TEEC_MEMREF_PARTIAL_INOUT;//TEEC_MEM_INOUT;
    result = TEEC_AllocateSharedMemory(
                &context02,
                &SharedMem0);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("1 allocate failed\n");
        goto cleanup_3;
    }
    memset(SharedMem0.buffer, 0, BIG_SIZE);
    //OpenSession
    result = TEEC_OpenSession(&context02, &session01, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }
    
    operation.started = 1;
    //Invoke_CheckMemoryAccessRight 
    operation.paramTypes = TEEC_PARAM_TYPES(
    TEEC_VALUE_INPUT,
    TEEC_MEMREF_PARTIAL_INOUT,
    TEEC_NONE,
    TEEC_NONE);
    operation.params[0].value.a= ANY_OWNER_SET_ACCESS_READ;
    operation.params[1].memref.parent = &SharedMem0;
    operation.params[1].memref.offset = OFFSET_SIZE;
    operation.params[1].memref.size = BIG_SIZE - OFFSET_SIZE;
    result = TEEC_InvokeCommand(&session01, CMD_TEE_CheckMemoryAccessRight, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }

    //CloseSession
    TEEC_CloseSession(&session01);
cleanup_3:
    if(SharedMem0.buffer)
        TEEC_ReleaseSharedMemory(&SharedMem0);

cleanup_2:
    //FinalizeContext
    TEEC_FinalizeContext(&context02);
cleanup_1:
    return result;
}


TEEC_Result Invoke_CheckMemoryAccessRight_BufferReadWrite_AccessWrite_success()
{
    TEEC_Context context02;
    TEEC_Session session01;
    TEEC_Result result;
    uint32_t origin, i;
    TEEC_Operation operation;
    memset(&operation, 0, sizeof(operation));
    TEEC_SharedMemory SharedMem0;
    //TEEC_SharedMemory SharedMem1;
    //InitializeContext
    result = TEEC_InitializeContext(NULL, &context02);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_1;
    }
    //Allocate a shared memory
    SharedMem0.buffer = NULL;
    SharedMem0.size = BIG_SIZE;
    SharedMem0.flags = TEEC_MEM_INOUT;
    result = TEEC_AllocateSharedMemory(
                &context02,
                &SharedMem0);
    if(result != TEEC_SUCCESS) {
        TEEC_Error(" allocate failed\n");
        goto cleanup_3;
    }
    //OpenSession
    result = TEEC_OpenSession(&context02, &session01, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }
    operation.started = 1;
    //Invoke_CheckMemoryAccessRight 
    operation.paramTypes = TEEC_PARAM_TYPES(
    TEEC_VALUE_INPUT,
    TEEC_MEMREF_PARTIAL_INOUT,
     TEEC_NONE,
    TEEC_NONE);
    operation.params[0].value.a= ANY_OWNER_SET_ACCESS_WRITE;
    operation.params[1].memref.parent = &SharedMem0;
    operation.params[1].memref.offset = OFFSET_SIZE;
    operation.params[1].memref.size = BIG_SIZE - OFFSET_SIZE;
    result = TEEC_InvokeCommand(&session01, CMD_TEE_CheckMemoryAccessRight, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }

    //CloseSession
    TEEC_CloseSession(&session01);
cleanup_3:
    if(SharedMem0.buffer)
      TEEC_ReleaseSharedMemory(&SharedMem0);

cleanup_2:
    //FinalizeContext
    TEEC_FinalizeContext(&context02);
cleanup_1:
    return result;
}


TEEC_Result Invoke_CheckMemoryAccessRight_BufferRead_AccessRead_success()
{
    TEEC_Context context02;
    TEEC_Session session01;
    TEEC_Result result;
    uint32_t origin, i;
    TEEC_Operation operation;
    memset(&operation, 0, sizeof(operation));
    TEEC_SharedMemory SharedMem0;
    //TEEC_SharedMemory SharedMem1;
    //InitializeContext
    result = TEEC_InitializeContext(NULL, &context02);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_1;
    }
    //Allocate a shared memory
    SharedMem0.buffer = NULL;
    SharedMem0.size = BIG_SIZE;
    SharedMem0.flags = TEEC_MEM_INPUT;
    result = TEEC_AllocateSharedMemory(
                &context02,
                &SharedMem0);
    if(result != TEEC_SUCCESS) {
        TEEC_Error(" allocate failed\n");
        goto cleanup_3;
    }
    //OpenSession
    result = TEEC_OpenSession(&context02, &session01, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }
    operation.started = 1;
    //Invoke_CheckMemoryAccessRight 
    operation.paramTypes = TEEC_PARAM_TYPES(
    TEEC_VALUE_INPUT,
    TEEC_MEMREF_PARTIAL_INPUT,
     TEEC_NONE,
    TEEC_NONE);
    operation.params[0].value.a= ANY_OWNER_SET_ACCESS_READ;
    operation.params[1].memref.parent = &SharedMem0;
    operation.params[1].memref.offset = OFFSET_SIZE;
    operation.params[1].memref.size = BIG_SIZE - OFFSET_SIZE;
    result = TEEC_InvokeCommand(&session01, CMD_TEE_CheckMemoryAccessRight, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }

    //CloseSession
    TEEC_CloseSession(&session01);
cleanup_3:
    if(SharedMem0.buffer)
       TEEC_ReleaseSharedMemory(&SharedMem0);

cleanup_2:
    //FinalizeContext
    TEEC_FinalizeContext(&context02);
cleanup_1:
    return result;
}

TEEC_Result Invoke_CheckMemoryAccessRight_BufferWrite_AccessWrite_success()
{
    TEEC_Context context02;
    TEEC_Session session01;
    TEEC_Result result;
    uint32_t origin, i;
    TEEC_Operation operation;
    memset(&operation, 0, sizeof(operation));
    TEEC_SharedMemory SharedMem0;
    //TEEC_SharedMemory SharedMem1;
    //InitializeContext
    result = TEEC_InitializeContext(NULL, &context02);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_1;
    }
    //Allocate a shared memory
    SharedMem0.buffer = NULL;
    SharedMem0.size = BIG_SIZE;
    SharedMem0.flags = TEEC_MEM_OUTPUT;
    result = TEEC_AllocateSharedMemory(
                &context02,
                &SharedMem0);
    if(result != TEEC_SUCCESS) {
        TEEC_Error(" allocate failed\n");
        goto cleanup_3;
    }
    //OpenSession
    result = TEEC_OpenSession(&context02, &session01, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }
    operation.started = 1;
    //Invoke_CheckMemoryAccessRight 
        operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_MEMREF_PARTIAL_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a= ANY_OWNER_SET_ACCESS_WRITE;
    operation.params[1].memref.parent = &SharedMem0;
    operation.params[1].memref.offset = OFFSET_SIZE;
    operation.params[1].memref.size = BIG_SIZE - OFFSET_SIZE;
    result = TEEC_InvokeCommand(&session01, CMD_TEE_CheckMemoryAccessRight, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }

    //CloseSession
    TEEC_CloseSession(&session01);
cleanup_3:
    if(SharedMem0.buffer)
        TEEC_ReleaseSharedMemory(&SharedMem0);

cleanup_2:
    //FinalizeContext
    TEEC_FinalizeContext(&context02);
cleanup_1:
    return result;
}

TEEC_Result Invoke_CheckMemoryAccessRight_BufferRead_AccessWrite_error()
{
    TEEC_Context context02;
    TEEC_Session session01;
    TEEC_Result result;
    uint32_t origin, i;
    TEEC_Operation operation;
    memset(&operation, 0, sizeof(operation));
    TEEC_SharedMemory SharedMem0;
    //TEEC_SharedMemory SharedMem1;
    //InitializeContext
    result = TEEC_InitializeContext(NULL, &context02);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_1;
    }
    //Allocate a shared memory
    SharedMem0.buffer = NULL;
    SharedMem0.size = BIG_SIZE;
    SharedMem0.flags = TEEC_MEM_INPUT;
    result = TEEC_AllocateSharedMemory(
                &context02,
                &SharedMem0);
    if(result != TEEC_SUCCESS) {
        TEEC_Error(" allocate failed\n");
        goto cleanup_3;
    }
    //OpenSession
    result = TEEC_OpenSession(&context02, &session01, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }
    operation.started = 1;
    //Invoke_CheckMemoryAccessRight 
    operation.paramTypes = TEEC_PARAM_TYPES(
    TEEC_VALUE_INPUT,
    TEEC_MEMREF_PARTIAL_INPUT,
      TEEC_NONE,
    TEEC_NONE);
    operation.params[0].value.a= ANY_OWNER_SET_ACCESS_WRITE;
    operation.params[1].memref.parent = &SharedMem0;
    operation.params[1].memref.offset = OFFSET_SIZE;
    operation.params[1].memref.size = BIG_SIZE - OFFSET_SIZE;
    result = TEEC_InvokeCommand(&session01, CMD_TEE_CheckMemoryAccessRight, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        //TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        if (TEEC_ERROR_ACCESS_DENIED == result){
            result = TEEC_SUCCESS;
        }
    }
    else
    {
        result = -1;
    }
    //CloseSession
    TEEC_CloseSession(&session01);
cleanup_3:
    if(SharedMem0.buffer)
       TEEC_ReleaseSharedMemory(&SharedMem0);

cleanup_2:
    //FinalizeContext
    TEEC_FinalizeContext(&context02);
cleanup_1:
    return result;
}


TEEC_Result Invoke_CheckMemoryAccessRight_BufferShared_AccessAnyOwnerNotSet_error()
{
    TEEC_Context context02;
    TEEC_Session session01;
    TEEC_Result result;
    uint32_t origin, i;
    TEEC_Operation operation;
    memset(&operation, 0, sizeof(operation));
    TEEC_SharedMemory SharedMem0;
    //TEEC_SharedMemory SharedMem1;
    //InitializeContext
    result = TEEC_InitializeContext(NULL, &context02);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_1;
    }
    //Allocate a shared memory
    SharedMem0.buffer = NULL;
    SharedMem0.size = BIG_SIZE;
    SharedMem0.flags = TEEC_MEM_INPUT;
    result = TEEC_AllocateSharedMemory(
                &context02,
                &SharedMem0);
    if(result != TEEC_SUCCESS) {
        TEEC_Error(" allocate failed\n");
        goto cleanup_3;
    }
    //OpenSession
    result = TEEC_OpenSession(&context02, &session01, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }
    operation.started = 1;
    //Invoke_CheckMemoryAccessRight 
    operation.paramTypes = TEEC_PARAM_TYPES(
    TEEC_VALUE_INPUT,
    TEEC_MEMREF_PARTIAL_INPUT,
    TEEC_NONE,
    TEEC_NONE);
    operation.params[0].value.a= ANY_OWNER_NOT_SET;
    operation.params[1].memref.parent = &SharedMem0;
    operation.params[1].memref.offset = OFFSET_SIZE;
    operation.params[1].memref.size = BIG_SIZE - OFFSET_SIZE;
    result = TEEC_InvokeCommand(&session01, CMD_TEE_CheckMemoryAccessRight, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        //TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        if (TEEC_ERROR_ACCESS_DENIED == result){
            result = TEEC_SUCCESS;
        }
    }
    else
    {
        result = -1;
    }
    
    //CloseSession
    TEEC_CloseSession(&session01);
cleanup_3:
    if(SharedMem0.buffer)
        TEEC_ReleaseSharedMemory(&SharedMem0);

cleanup_2:
    //FinalizeContext
    TEEC_FinalizeContext(&context02);
cleanup_1:
    return result;
}

int Test_CheckMemoryAccessRights()
{
    TEEC_Result result = TEEC_SUCCESS;
    uint32_t count_error = 0;
    uint32_t cnt_total = 0;
    
    DO_FUNC_AND_PRINT(Invoke_CheckMemoryAccessRight_BufferReadWrite_AccessRead_success);
    DO_FUNC_AND_PRINT(Invoke_CheckMemoryAccessRight_BufferReadWrite_AccessWrite_success);
    DO_FUNC_AND_PRINT(Invoke_CheckMemoryAccessRight_BufferRead_AccessRead_success);
    DO_FUNC_AND_PRINT(Invoke_CheckMemoryAccessRight_BufferWrite_AccessWrite_success);
    DO_FUNC_AND_PRINT(Invoke_CheckMemoryAccessRight_BufferRead_AccessWrite_error);
    DO_FUNC_AND_PRINT(Invoke_CheckMemoryAccessRight_BufferShared_AccessAnyOwnerNotSet_error);

    printf("STB Test: %s: failed/total=%d/%d\n", __func__, count_error, cnt_total); 
    
    if (count_error){
        return -1;
    }
    
    return TEEC_SUCCESS;
}

#endif

int main(int argc, char *argv[])
{
    if (argc < 2)
        goto usage;

    /*if (!strcmp(argv[1], "secmmz")) {
        stb_test_printf("Sec MMZ testing start!\n");
        STB_TEST_ASSERT(!test_init_deinit());
        STB_TEST_ASSERT(!test_new_delete());
        STB_TEST_ASSERT(!test_catota());
        STB_TEST_ASSERT(!test_ta2ta());
        STB_TEST_ASSERT(!test_sec_world_api(TEST_SEC_MMZ));
        goto out;
    }*/

	/*
    if (!strcmp(argv[1], "crypto")) {
        stb_test_printf("Crypto API testing start!\n");
        STB_TEST_ASSERT(!test_sec_world_api(TEST_CRYPTO));
        goto out;
    }*/
	
	//added by fangjian00208632
	if (!strcmp(argv[1], "panic")) {
        stb_test_printf("TEE_Panic API testing start!\n");
        STB_TEST_ASSERT(!test_tee_panic());
        goto out;
    }
    
    //added by fangjian00208632
    if (!strcmp(argv[1], "canal")) {
        stb_test_printf("test_canal_short_buffer testing start!\n");
        STB_TEST_ASSERT(!test_canal_short_buffer());
        goto out;
    }

    //added by f00216599
    if (!strcmp(argv[1], "sess")) {
        stb_test_printf("test_16_sess testing start!\n");
        STB_TEST_ASSERT(!test_16_session());
        stb_test_printf("test_16_sess testing pass!\n");
        goto out;
    }
    
    //added by fangjian00208632
    if (!strcmp(argv[1], "smc_time")) {
        stb_test_printf("test_smc_switch_time_cost testing start!\n");
        STB_TEST_ASSERT(!test_smc_switch_time_cost());
        goto out;
    }

    //added by fangjian00208632 2015-08-18
    if (!strcmp(argv[1], "MemAccess")) {
        stb_test_printf("MemAccess testing start!\n");
        STB_TEST_ASSERT(!Test_CheckMemoryAccessRights());
        stb_test_printf("MemAccess testing passed!\n");
        goto out;
    }
    goto usage;
out:
    return 0;
usage:
    printf("Usage:\n");
    //printf("%s secmmz   -- test the sec mmz\n", argv[0]);
    printf("%s panic   -- test the TEE_Panic API\n", argv[0]);
    printf("%s canal       -- test the canal+ ERROR_SHORT_BUFFER\n", argv[0]);
    printf("%s sess       -- test the 16 sess\n", argv[0]);
    printf("%s smc_time   -- test the smc swtich time cost\n", argv[0]);
    printf("%s MemAccess   -- test the mem access right\n", argv[0]);
}
