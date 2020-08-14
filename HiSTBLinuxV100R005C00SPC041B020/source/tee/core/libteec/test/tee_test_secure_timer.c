#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "test_hello.h"
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

 /* example uuid */
static const TEEC_UUID EXAMPLE_uuid =
{
    0x19b39980, 0x2487, 0x7b84,
    { 0xf4, 0x1a, 0xbc, 0x89, 0x22, 0x62, 0xbb, 0x3d }
};

extern TEEC_Result TEEC_EXT_TST (TEEC_Session* session, TEEC_Operation *operation);

#if 1
static int create_timer_test(TEEC_Session *session, uint32_t time_seconds, uint32_t type)
{
    uint32_t cmd = CMD_ID_CREATE_ONE_TIMER;
    TEEC_Operation operation = {0};
    uint32_t origin;
    TEEC_Result ret;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = time_seconds;
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);

    return ret;
}

static int get_sys_time_test(TEEC_Session *session, uint32_t time_seconds, uint32_t type)
{
    uint32_t cmd = 6;
    TEEC_Operation operation = {0};
    uint32_t origin;
    TEEC_Result ret;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = time_seconds;
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);

    printf("System time is operation.params[1].value.a is %d\n", operation.params[1].value.a);

    return ret;
}


static int destory_timer_test(TEEC_Session *session, uint32_t time_seconds, uint32_t type, int tst_arg)
{
    uint32_t cmd = CMD_ID_CREATE_ONE_TIMER;
    TEEC_Operation operation = {0};
    uint32_t origin;
    TEEC_Result ret;
    int32_t timer_id;

    if (time_seconds <= 3) {
        printf("test case not correct, You should enlarge time seconds\n");
    }

    /*1. First Create Timer*/
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = time_seconds;
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);

    timer_id = operation.params[1].value.a;
    printf("timer id is %d\n", timer_id);

    /*2. sleep a few seconds before time eclipse*/
    printf("start to delay 3 seconds\n");
    sleep(3);
    printf("after delay\n");

    /*3. destory timer*/
    cmd = CMD_ID_DESTORY_EXIST_TIMER;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    if (tst_arg == CMD_ID_DESTORY_NON_EXIST_TIMER) {
        operation.params[0].value.b = 0xFFFFEEEE;
    }
    else
        operation.params[0].value.b = type;

    printf("timer type is %d\n", operation.params[0].value.b);

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (tst_arg == CMD_ID_DESTORY_NON_EXIST_TIMER) {
        if (ret != TEEC_SUCCESS)
            printf("Suceed  to execute destory non exist timer:cmd id is %x, ret is 0x%x\n", cmd, ret);
        ret = TEEC_SUCCESS;
    }
    else {
        if (ret != TEEC_SUCCESS)
            printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);
    }
    return ret;
}


static int destory_timer_test_once(TEEC_Session *session, uint32_t time_seconds, uint32_t type)
{
    uint32_t cmd = CMD_ID_CREATE_ONE_TIMER;
    TEEC_Operation operation = {0};
    uint32_t origin;
    TEEC_Result ret;
    int32_t timer_id;

    if (time_seconds <= 3) {
        printf("test case not correct, You should enlarge time seconds\n");
    }

    /*3. destory timer*/
    cmd = CMD_ID_DESTORY_EXIST_TIMER;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.b = type;

    printf("timer type is %d\n", operation.params[0].value.b);

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);
    return ret;
}

static int get_timer_expiretime_once_test(TEEC_Session *session, uint32_t time_seconds, uint32_t type)
{
    uint32_t cmd = CMD_ID_CREATE_ONE_TIMER;
    TEEC_Operation operation = {0};
    uint32_t origin;
    TEEC_Result ret;
    int32_t timer_id;

    if (time_seconds <= 3) {
        printf("test case not correct, You should enlarge time seconds\n");
    }

    /*3. get expire timer*/
    cmd = CMD_ID_GET_TIMER_EXPIRE_TIME;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);
    else
        printf("succeed: search time_seconds is %d\n", operation.params[1].value.a);
    return ret;
}

static int get_timer_expiretime_test(TEEC_Session *session, uint32_t time_seconds, uint32_t type)
{
    uint32_t cmd = CMD_ID_CREATE_ONE_TIMER;
    TEEC_Operation operation = {0};
    uint32_t origin;
    TEEC_Result ret;
    int32_t timer_id;

    if (time_seconds <= 3) {
        printf("test case not correct, You should enlarge time seconds\n");
    }

    /*1. First Create Timer*/
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = time_seconds;
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);

    timer_id = operation.params[1].value.a;
    printf("timer id is %d\n", timer_id);

    /*2. sleep a few seconds before time eclipse*/
    printf("start to delay %d seconds\n", 3);
    sleep(3);
    printf("after delay\n");

    /*3. get expire timer*/
    cmd = CMD_ID_GET_TIMER_EXPIRE_TIME;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);
    else
        printf("succeed: search time_seconds is %d\n", operation.params[1].value.a);
    return ret;
}

static int get_timer_remaintime_once_test(TEEC_Session *session, uint32_t time_seconds, uint32_t type)
{
    uint32_t cmd = CMD_ID_CREATE_ONE_TIMER;
    TEEC_Operation operation = {0};
    uint32_t origin;
    TEEC_Result ret;
    int32_t timer_id;

    if (time_seconds <= 3) {
        printf("test case not correct, You should enlarge time seconds\n");
    }

    /*3. get expire timer*/
    cmd = CMD_ID_GET_TIMER_REMAIN_TIME;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);
    else
        printf("succeed: search remain time_seconds is %d\n", operation.params[1].value.a);
    return ret;
}

static int get_timer_remaintime_test(TEEC_Session *session, uint32_t time_seconds, uint32_t type)
{
    uint32_t cmd = CMD_ID_CREATE_ONE_TIMER;
    TEEC_Operation operation = {0};
    uint32_t origin;
    TEEC_Result ret;
    int32_t timer_id;

    if (time_seconds <= 3) {
        printf("test case not correct, You should enlarge time seconds\n");
    }

    /*1. First Create Timer*/
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = time_seconds;
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);

    timer_id = operation.params[1].value.a;
    printf("timer id is %d\n", timer_id);

    /*2. sleep a few seconds before time eclipse*/
    printf("start to delay 3 seconds\n");
    sleep(3);
    printf("after delay\n");

    /*3. get expire timer*/
    cmd = CMD_ID_GET_TIMER_REMAIN_TIME;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.b = type;

    ret = TEEC_InvokeCommand(session, cmd, &operation, &origin);
    if (ret != TEEC_SUCCESS)
        printf("failed to invoke cmd:cmd id is %x, ret is 0x%x\n", cmd, ret);
    else
        printf("succeed: search remain time_seconds is %d\n", operation.params[1].value.a);
    return ret;
}

static int register_callback_func(TEEC_Session *session)
{
    TEEC_Operation operation = {0};
    int32_t ret;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INOUT,
        TEEC_VALUE_INOUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = TST_CMD_ID_01;

    ret = TEEC_EXT_TST(session, &operation);
    if (ret != TEEC_SUCCESS)
        printf("Failed to execute TST 01 ret=0x%x\n", ret);

    return ret;
}
static int get_timer_type(TEEC_Session *session)
{
    TEEC_Operation operation = {0};
    int32_t ret;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INOUT,
        TEEC_VALUE_INOUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = TST_CMD_ID_02;

    ret = TEEC_EXT_TST(session, &operation);
    if (ret != TEEC_SUCCESS)
        printf("Failed to execute TST 02 ret=0x%x\n", ret);

    return operation.params[1].value.a;
}
#if 0
int get_rtc0_time(void)
{
    int fd, retval;

    struct rtc_time rtc_tm;

    time_t timep;

    struct tm *p;
    fd = open("/dev/rtc0", O_RDONLY);

    if (fd == -1) {

            perror("/dev/rtc");

            exit(errno);

    }
    /* Read the RTC time/date */

    retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);

    if (retval == -1) {

            perror("ioctl");

            exit(errno);

    }

    close(fd);

    fprintf(stderr, "RTC date/time: %d/%d/%d %02d:%02d:%02d\n",

            rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900,

            rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

    time(&timep);

    p = gmtime(&timep);

    fprintf(stderr, "OS date/time(UTC): %d/%d/%d %02d:%02d:%02d\n",

            p->tm_mday, p->tm_mon + 1, p->tm_year + 1900,

            p->tm_hour, p->tm_min, p->tm_sec);



    p = localtime(&timep);

    fprintf(stderr, "OS date/time(Local): %d/%d/%d %02d:%02d:%02d\n",

            p->tm_mday, p->tm_mon + 1, p->tm_year + 1900,

            p->tm_hour, p->tm_min, p->tm_sec);



    return
}
#endif
#endif
void kill_test(int task_pid)
{
    int pid;
    int status, ret;
    printf("send signal to process (%d) \n", task_pid);
    ret = kill(task_pid,SIGTERM);
    if (ret<0)
        printf("ret is %d, strerror(errno) is %s\n", ret, strerror(errno));
    printf("kill result is ret 0x%x\n", ret);
    /*wait(&status);
    if(WIFSIGNALED(status))
        printf("chile process receive signal %d\n", WTERMSIG(status));
    */
    printf("kill finish\n");
}

static void timer_ut_usage(void) {
    printf("You can use timer_ut #param1 #param2 #param3 to test TrustedCore timer mechanism:e.g. timer_ut 0 12 12\n");
    printf("param1 is as belows:\n");
    printf("      0:create timer and register callback func, fork anther thread to wait timer use up\n");
    printf("      2:create timer and delete the timer event\n");
    printf("      3:delete the non-exist timer event\n");
    printf("      4:create timer and get the total time that be set\n");
    printf("      5:create timer delay 3 seconds then get the remain time\n");
    printf("      8:create one timer\n");
    printf("      9:get timer total time\n");
    printf("      10:get timer remain time\n");
    printf("      11:destory timer\n");
    printf("      12:get secure time in TEE\n");
    printf("param2 is the time value(seconds), e.g. 12 represent we create a 12s timer event ,after 12s, it will response and call the callback func.\n");
    printf("param3 is the timer type, which will pass to callback func and send it to Skytone framework\n");
}

int main(int argc, char **argv)
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t origin;
    int32_t cmd_id;
    int32_t seconds;
    int32_t type;
    int pid;
    clock_t start,finish;
    double time;

    if (argc != 4) {
        printf("input error\n");
        timer_ut_usage();
        return -1;
    }

    cmd_id = atoi(argv[1]);
    seconds = atoi(argv[2]);
    type = atoi(argv[3]);
    printf("cmd_id is %d, seconds is %d,type is %d\n", cmd_id, seconds, type);
    if (cmd_id == 0xff) {
        printf("Test for learning\n");
        kill_test(seconds);
        return 0;
    }
#if 1
    result = TEEC_InitializeContext(NULL, &context);

    if(result != TEEC_SUCCESS) {
       printf("teec initial failed\n");
       goto cleanup_1;
    }
	printf("Succeed to InitializeContext\n");
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
						TEEC_NONE,
						TEEC_NONE,
						TEEC_MEMREF_TEMP_INPUT,
						TEEC_MEMREF_TEMP_INPUT);
    result = TEEC_OpenSession(&context, &session, &EXAMPLE_uuid,
                TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);

    if(result != TEEC_SUCCESS) {
        printf("teec open session failed\n");
        goto cleanup_2;
    }
	printf("Succeed to OpenSession\n");
    //result = TEEC_InvokeCommand(&session, cmd, NULL, &origin);
    switch(cmd_id)
    {
        case CMD_ID_CREATE_ONE_TIMER:
            pid = fork();
            if (pid < 0) {
                printf("failed to fork another process pid =0x%x\n", pid);
            }
            else if (pid == 0) {
                printf("step into child process\n");
                while(1) {
                    usleep(200*1000);
                    if (type == get_timer_type(&session)) {
                        finish = clock();
                        time=(double)(finish-start)/CLOCKS_PER_SEC;
                        printf("timer time length is %f\n", time);
                        goto cleanup_1;
                        break;
                    }
                    //printf("son process running\n");
                }
            }
            else {
                result = register_callback_func(&session);
                if (result != TEEC_SUCCESS) {
                    printf("failed to register call back func\n");
                }
                else {
                    printf("succeed to register callback func\n");
                    start = clock();
                    result = create_timer_test(&session, seconds, type);
                }

            }
            break;
        case CMD_ID_CREATE_ONE_TIMER_WITHOUT_CALLBACK:
            result = create_timer_test(&session, seconds, type);
            break;
        case CMD_ID_DESTORY_EXIST_TIMER:
            result = register_callback_func(&session);
            result = destory_timer_test(&session, seconds, type, CMD_ID_DESTORY_EXIST_TIMER);
            if (result != TEEC_SUCCESS) {
                printf("failed to destory exist timer\n");
            }
            else {
                printf("succeed to destory exist timer\n");
            }
            break;
        case CMD_ID_DESTORY_NON_EXIST_TIMER:
            result = register_callback_func(&session);
            result = destory_timer_test(&session, seconds, type, CMD_ID_DESTORY_NON_EXIST_TIMER);
            if (result != TEEC_SUCCESS) {
                printf("failed to execute destory non exist timer\n");
            }
            else {
                printf("succeed to execute destory non exist timer\n");
            }
            break;
        case CMD_ID_GET_TIMER_EXPIRE_TIME:
            result = register_callback_func(&session);
            result = get_timer_expiretime_test(&session, seconds, type);
            if (result != TEEC_SUCCESS) {
                printf("failed to get_timer_expiretime_test exist timer\n");
            }
            else {
                printf("succeed to get_timer_expiretime_test exist timer\n");
            }
            break;
        case CMD_ID_GET_TIMER_REMAIN_TIME:
            result = register_callback_func(&session);
            result = get_timer_remaintime_test(&session, seconds, type);
            if (result != TEEC_SUCCESS) {
                printf("failed to get_timer_remaintime_test exist timer\n");
            }
            else {
                printf("succeed to get_timer_remaintime_test exist timer\n");
            }
            break;
        case CMD_ID_NO_CALLBACK:
            result = create_timer_test(&session, seconds, type);
            break;
        case CMD_ID_CREATE_ONE_TIMER_ONCE:
            result = register_callback_func(&session);
            if (result != TEEC_SUCCESS) {
                printf("failed to register call back func\n");
            }
            result = create_timer_test(&session, seconds, type);
            break;
        case CMD_ID_GET_ONE_TIMER_EXPIRE_ONCE:
            result = get_timer_expiretime_once_test(&session, seconds, type);
            break;
        case CMD_ID_GET_ONE_TIMER_REMAIN_ONCE:
            result = get_timer_remaintime_once_test(&session, seconds, type);
            break;
        case CMD_ID_DESTORY_EXIST_TIMER_ONCE:
            result = destory_timer_test_once(&session, seconds, type);
            break;
        case CMD_ID_GET_SYS_TIME:
            get_sys_time_test(&session, seconds, type);
            break;
        default:
            break;
    }


    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }
	printf("Succeed to InvokeCommand\n");

    TEEC_CloseSession(&session);
    printf("Succeed to Close Sesion\n");

    //sleep(seconds+3);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_1:
    printf("pid %d exit\n", getpid());
    //while(1);
    return 0;
    #endif
}
