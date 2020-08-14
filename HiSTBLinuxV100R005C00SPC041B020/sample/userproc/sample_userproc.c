#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_adp.h"

#define DIRNAME     "sample"
#define ENTRYNAME1   "help"
#define ENTRYNAME2   "testproc2"
#define ENTRYNAME3   "testproc3"
#define ENTRYNAME4   "testproc4"

#define SAMPLE_PRINT printf

HI_S32 Proc_Test_Show1(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    SAMPLE_PRINT("Print in process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    HI_PROC_Printf(pstBuf, "Print in process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    HI_PROC_Printf(pstBuf, "Invoke API \'HI_PROC_AddDir\' and \'HI_PROC_RemoveDir\' to create or remove a proc directory, the dir will be added in /proc/hisi/, for multi-process supporting, its name will be ${NAME_INPUT}_${PID}\n");
    HI_PROC_Printf(pstBuf, "invoke API \'HI_PROC_AddEntry\' and \'HI_PROC_RemoveEntry\' to create or remove a proc entry,\n");
    HI_PROC_Printf(pstBuf, "you can register a call back function \'HI_PROC_SHOW_FN\' to support read command as \'cat /proc/hisi/your_dir_pid/your_entry\',\n");
    HI_PROC_Printf(pstBuf, "and register function \'HI_PROC_CMD_FN\' to support write command as \'echo params [...] > /proc/hisi/your_dir_pid/your_entry\'.\n");
    HI_PROC_Printf(pstBuf, "Invoke API \'HI_PROC_Printf\' in your \'HI_PROC_SHOW_FN\' to print info to proc entry.\n");
    HI_PROC_Printf(pstBuf, "You can invoke it repeatedly, but the total amount of data must be less than 4KB. \n");
    return HI_SUCCESS;
}

HI_S32 Proc_Test_Show2(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    SAMPLE_PRINT("Print in process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    HI_PROC_Printf(pstBuf, "Print in process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    return HI_SUCCESS;
}

HI_S32 Proc_Test_Cmd1(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_S32 i;

    SAMPLE_PRINT("Print in process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    SAMPLE_PRINT("Argc: %d \n", u32Argc);
    for (i=0; i<u32Argc; i++)
    {
        SAMPLE_PRINT("\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }
    
    /* echo info to current teriminal */
    HI_PROC_Printf(pstBuf, "Get help info from %s.\n", __func__);
    return HI_SUCCESS;
}

HI_S32 Proc_Test_Cmd2(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_S32 i;

    SAMPLE_PRINT("Print in process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    SAMPLE_PRINT("Argc: %d \n", u32Argc);
    for (i=0; i<u32Argc; i++)
    {
        SAMPLE_PRINT("\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }
    
    return HI_SUCCESS;
}

HI_S32 main(HI_VOID)
{
    HI_S32 s32Ret;
    HI_PROC_ENTRY_S stEntry1;
    HI_PROC_ENTRY_S stEntry2;
    HI_PROC_ENTRY_S stEntry3;
    HI_PROC_ENTRY_S stEntry4;
    HI_CHAR aszInputCmd[16];

    HI_SYS_Init();
    HI_MODULE_Register(HI_ID_USR_START, "TestModule");

    /* Will be added at /proc/hisi/ directory */
    s32Ret = HI_PROC_AddDir(DIRNAME);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRINT("HI_PROC_AddDir(\"%s\") return %d\n", DIRNAME, s32Ret);
        goto sys_deinit;
    }
    SAMPLE_PRINT("Add proc dir /proc/hisi/"DIRNAME" successfully.\n");

    /* Will be added at /proc/hisi/${DIRNAME} directory */
    stEntry1.pszDirectory = DIRNAME;
    stEntry1.pszEntryName = ENTRYNAME1;
    stEntry1.pfnShowProc = Proc_Test_Show1;
    stEntry1.pfnCmdProc = Proc_Test_Cmd1;
    stEntry1.pPrivData = HI_NULL;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stEntry1);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRINT("HI_PROC_AddEntry(\"%s\") return %d\n", ENTRYNAME1, s32Ret);
        goto remove_dir;
    }
    SAMPLE_PRINT("Add proc entry /proc/hisi/"DIRNAME"/"ENTRYNAME1" successfully.\n");

    /* Will be added at /proc/hisi/${DIRNAME} directory */
    stEntry2.pszDirectory = DIRNAME;
    stEntry2.pszEntryName = ENTRYNAME2;
    stEntry2.pfnShowProc = Proc_Test_Show2;
    stEntry2.pfnCmdProc = Proc_Test_Cmd2;
    stEntry2.pPrivData = HI_NULL;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stEntry2);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRINT("HI_PROC_AddEntry(\"%s\") return %d\n", ENTRYNAME2, s32Ret);
        goto remove_entry1;
    }
    SAMPLE_PRINT("Add proc entry /proc/hisi/"DIRNAME"/"ENTRYNAME2" successfully.\n");

    /* Only support echo, will be added at /proc/hisi/${DIRNAME} directory */
    stEntry3.pszDirectory = DIRNAME;
    stEntry3.pszEntryName = ENTRYNAME3;
    stEntry3.pfnShowProc = HI_NULL;
    stEntry3.pfnCmdProc = Proc_Test_Cmd2;
    stEntry3.pPrivData = HI_NULL;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stEntry3);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRINT("HI_PROC_AddEntry(\"%s\") return %d\n", ENTRYNAME3, s32Ret);
        goto remove_entry2;
    }
    SAMPLE_PRINT("Add proc entry /proc/hisi/"DIRNAME"/"ENTRYNAME3" successfully.\n");

    /* Only support cat, Will be added at /proc/hisi/${DIRNAME} directory */
    stEntry4.pszDirectory = DIRNAME;
    stEntry4.pszEntryName = ENTRYNAME4;
    stEntry4.pfnShowProc = Proc_Test_Show2;
    stEntry4.pfnCmdProc = HI_NULL;
    stEntry4.pPrivData = HI_NULL;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stEntry4);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRINT("HI_PROC_AddEntry(\"%s\") return %d\n", ENTRYNAME4, s32Ret);
        goto remove_entry3;
    }
    SAMPLE_PRINT("Add proc entry /proc/hisi/"DIRNAME"/"ENTRYNAME4" successfully.\n");

    SAMPLE_PRINT("Telnet to board and try to control the proc entry:\n");
    SAMPLE_PRINT("    cat /proc/hisi/"DIRNAME"/"ENTRYNAME1"\n");
    SAMPLE_PRINT("    echo 1 2 3 > /proc/hisi/"DIRNAME"/"ENTRYNAME2"\n");
    SAMPLE_PRINT("Press q to quit.\n");

    while (1)
    {
        SAMPLE_GET_INPUTCMD(aszInputCmd);
        if ('q' == aszInputCmd[0])
        {
            break;
        }
        
        sleep(1);
    }

    HI_PROC_RemoveEntry(HI_ID_USR_START, &stEntry4);
    SAMPLE_PRINT("Remove proc entry /proc/hisi/"DIRNAME"/"ENTRYNAME4" successfully.\n");

remove_entry3:
    HI_PROC_RemoveEntry(HI_ID_USR_START, &stEntry3);
    SAMPLE_PRINT("Remove proc entry /proc/hisi/"DIRNAME"/"ENTRYNAME3" successfully.\n");

remove_entry2:
    HI_PROC_RemoveEntry(HI_ID_USR_START, &stEntry2);
    SAMPLE_PRINT("Remove proc entry /proc/hisi/"DIRNAME"/"ENTRYNAME2" successfully.\n");

remove_entry1:
    HI_PROC_RemoveEntry(HI_ID_USR_START, &stEntry1);
    SAMPLE_PRINT("Remove proc entry /proc/hisi/"DIRNAME"/"ENTRYNAME1" successfully.\n");

remove_dir:
    HI_PROC_RemoveDir(DIRNAME);
    SAMPLE_PRINT("Remove proc dir /proc/hisi/"DIRNAME" successfully.\n");

sys_deinit:
    HI_MODULE_UnRegister(HI_ID_USR_START);
    HI_SYS_DeInit();
    return 0;
}

