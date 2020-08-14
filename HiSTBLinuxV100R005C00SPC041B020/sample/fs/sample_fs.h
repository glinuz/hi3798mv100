#ifndef __TEST_MAIN_H__
#define __TEST_MAIN_H__

#define GREENPRINT "\033[1;32m"
#define BLUEPRINT "\033[1;34m"
#define REDPRINT "\033[1;31m"
#define NONEPRINT "\033[0m"

/**********  micro defines ***********/
#define FAIL_HERE \
    do \
    {\
        {\
            printf("!!Fail here: %d @ %s\n", __LINE__, __FUNCTION__); \
            return -1; \
        } \
    } while (0)

#define SHOW_LIST(TestCase) \
    do {\
        int num = sizeof(TestCase) / sizeof(TestCase[0]); \
        int i = 0; \
        printf(GREENPRINT); \
        printf("\n********************%s***********************\n", # TestCase); \
        while (i < num)\
        {\
            printf("* %d : %s\n", i + 1, TestCase[i].name); \
            i++; \
        } \
        printf("******************************************************\n"); \
        printf(NONEPRINT); \
        printf("please input [ 1 - %d]: \n", num); \
    } while (0)

#define SHOW_WELCOME() \
    do {\
        printf(GREENPRINT); \
        printf("***************************************************************\n"); \
        printf("*           Welcome to Disk File System Test Demo ...                                  \n"); \
        printf("*                   [ Version:  1.0.0 ]                                                              \n"); \
        printf("***************************************************************\n"); \
        printf(NONEPRINT); \
    } while (0)

typedef int (*test_func)(void);

typedef struct hiCaseItem_t
{
    char      name[200];
    test_func func;
} CaseItem_S;

void GetUsrInput(void);

int  test_diskPeformance_list(void);
int  test_fdisk_list(void);
int  test_diskFs_list(void);
int  test_diskManage_list(void);

#endif
