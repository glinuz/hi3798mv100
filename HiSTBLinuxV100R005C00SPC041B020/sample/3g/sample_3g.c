/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : 3g_sample.c
Version       : Initial Draft
Author        : Hisilicon OSDRV group
Created       : 2014-06-04
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_3g_intf.h"

#define MAX_CMD_RETRIES        5
/* Macro */
#define MAX_ARGC       10
#define MAX_PAMRM_SIZE 30

/* Variable */
static HI_CHAR args[MAX_ARGC][MAX_PAMRM_SIZE];

static int max_operator_num = MAX_OPERATOR_NUM;
static HI_3G_CARD_S *card = NULL;
static HI_3G_OPERATOR_S operator[MAX_OPERATOR_NUM];
static HI_3G_CARD_OPERATOR_S currentoperator;
static char *card_status[] =
{
    "HI_3G_CARD_STATUS_UNAVAILABLE",
    "HI_3G_CARD_STATUS_SIM_PIN",
    "HI_3G_CARD_STATUS_SIM_PUK",
    "HI_3G_CARD_STATUS_SIM_PIN2",
    "HI_3G_CARD_STATUS_SIM_PUK2",
    "HI_3G_CARD_STATUS_DISCONNECTED",
    "HI_3G_CARD_STATUS_CONNECTED",
};

static HI_U32 g3_parse_arg(HI_U32 argc)
{
    HI_U32 i;
    int ret = 0xab;
    int card_num = 0;
    int operator_num = 0;
    char *argoption[] = {NULL};
    int ix = 0;
    int issr;
    int ber;
    HI_3G_CARD_COPS_MODE_E register_mode;
    HI_3G_CARD_COPS_OP_FORMAT_E register_format =
                    HI_3G_CARD_COPS_OP_FORMAT_LONG_STRING;
    HI_3G_CARD_STATE_E status = HI_3G_CARD_STATUS_UNAVAILABLE;
    HI_3G_PDP_S pdp;

    printf("input parameter Num:%d ", argc);
    for (i = 0 ; i < argc; i++)
    {
        printf("argv[%d]:%s, ", i, args[i]);
    }
    printf("\n");

    /* Parse 3g input command */
    if ( (0 == strcmp("help", (char *)args[0])) || (0 == strcmp("h", (char *)args[0])) )
    {
        printf("List all testtool command\n");
        printf("help          list all command we provide\n");
        printf("q             quit sample test\n");
        printf("scan          scan card\n");
        printf("init          init card\n");
        printf("deinit        deinit card\n");
        printf("getcard       get card status\n");
        printf("getallop      get all operators\n");
        printf("getcurop      get current operator\n");
        printf("regop         register operator\n");
        printf("getapn        get apn\n");
        printf("setapn        set apn\n");
        printf("connect       connect\n");
        printf("disconnect    disconnect\n");
        printf("getqua        get quality\n");
        printf("auto          auto connect\n");

        return 0;
    }

    if (0 == strcmp("scan", (char *)args[0]))
    {
        ret = HI_3G_ScanCard(card, 1, &card_num);
    }
    else if (0 == strcmp("init", (char *)args[0]))
    {
        ret = HI_3G_InitCard(card);
    }
    else if (0 == strcmp("deinit", (char *)args[0]))
    {
        ret = HI_3G_DeInitCard(card);
    }
    else if (0 == strcmp("getcard", (char *)args[0]))
    {
        ret = HI_3G_GetCardStatus(card, &status);
        if (0 == ret)
        {
            printf("card status:%d, %s.\n", status, card_status[status]);
        }
    }
    else if (0 == strcmp("getallop", (char *)args[0]))
    {
        ret = HI_3G_GetAllOperators (card,
                         operator,
                         max_operator_num,
                         &operator_num);

        if (ret == 0)
        {
            printf("get %d operators successfully:\n", operator_num);
            for (ix = 0; ix < operator_num; ix++)
            {
                printf("operator[%d]:\n"
                    "   netmode=%s\n"
                    "   long string=%s\n"
                    "   short string=%s\n"
                    "   numeric string=%s\n"
                    "   rat=%s\n",
                    ix,
                    operator[ix].anetmode,
                    operator[ix].alongoperator,
                    operator[ix].ashortoperator,
                    operator[ix].anumericoperator,
                    operator[ix].arat);
            }
        }
    }
    else if (0 == strcmp("getcurop", (char *)args[0]))
    {
        ret = HI_3G_GetCurrentOperator(card, &currentoperator);
        if (0 == ret)
        {
            printf("current operator:\n"
                "   netmode :%s\n"
                "   operformat: %s\n"
                "   operator: %s\n"
                "   rat: %s\n",
                currentoperator.anetmode,
                currentoperator.aformatoperator,
                currentoperator.aoperatorinfo,
                currentoperator.arat);
        }
    }
    else if (0 == strcmp("regop", (char *)args[0]))
    {
        if ((argc < 2) || (argc > 4))
        {
            printf("Usage: regop mode format op_name\n"
                "    mode:\n"
                "        0 - auto\n"
                "        1 - manual\n"
                "        2 - de-register\n"
                "        3 - set operator format\n"
                "        4 - if manual fail, change to auto\n");
            printf("    format:\n"
                "        0 - long string\n"
                "        1 - short string\n"
                "        2 - numeric string\n");
            return 0;
        }
        register_mode = atoi(args[1]);
        register_format = atoi(args[2]);
        printf("reg mode: %d; format: %d; name: [%s].\n", register_mode,
            register_format, args[3]);
        ret = HI_3G_RegisterOperator(card,
                         register_mode,
                         register_format,
                         args[3]);
    }
    else if (0 == strcmp("getapn", (char *)args[0]))
    {
        ret = HI_3G_GetApn(card, &pdp);
        if (ret == 0)
            printf("get current pdp successfully:\n"
                 "  cid:             %s\n"
                 "  pdp_type:        %s\n"
                 "  apn:             %s\n"
                 "  pdp_ipaddr:      %s\n"
                 "  d_comp:          %s\n"
                 "  h_comp:          %s\n",
                 pdp.acid,
                 pdp.apdptype,
                 pdp.aapn,
                 pdp.apdpipaddr,
                 pdp.adcomp,
                 pdp.ahcomp);
    }
    else if (0 == strcmp("setapn", (char *)args[0]))
    {
        if (argc != 2)
        {
            printf("Usage: setapn apn\n");
            return 0;
        }
        printf("apn: [%s]\n", args[1]);
        ret = HI_3G_SetApn(card, args[1]);
    }
    else if (0 == strcmp("connect", (char *)args[0]))
    {
        if (argc != 4)
        {
            printf("Usage: connect username password telephone\n");
            return 0;
        }
        printf("username: [%s]; pass: [%s]; tele: [%s].\n", args[1], args[2], args[3]);
        ret = HI_3G_ConnectCard(card, args[1], args[2], args[3], 0, argoption);
    }
    else if (0 == strcmp("disconnect", (char *)args[0]))
    {
        ret = HI_3G_DisConnectCard(card);
    }
    else if (0 == strcmp("getqua", (char *)args[0]))
    {
        ret = HI_3G_GetQuality(card, &issr, &ber);
        if (ret == 0)
            printf("get card quality successfully\n"
                "   issr: %d,"
                "   ber: %d\n",
                issr, ber);
    }
    else if (0 == strcmp("auto", (char *)args[0]))
    {
        if (argc != 5)
        {
            printf("Usage: auto username password telephone apn\n");
            printf("   eg: auto '' '' *99# 3gnet\n");
            return 0;
        }
        printf("username: [%s]; pass: [%s]; tele: [%s]; apn [%s].\n", args[1],
            args[2], args[3], args[4]);

        /* scan card */
        ret = HI_3G_ScanCard(card, 1, &card_num);
        if(0 != ret)
        {
            printf("auto scan card fail.\n");
            goto OUT;
        }

        /* init card */
        ret = HI_3G_InitCard(card);
        if(0 != ret)
        {
            printf("auto init card fail.\n");
            goto OUT;
        }

        /* get card status */
        ret = HI_3G_GetCardStatus(card, &status);
        if(0 != ret)
        {
            printf("auto get card status fail.\n");
            goto OUT;
        }
        if (status != HI_3G_CARD_STATUS_DISCONNECTED)
        {
            printf("auto get card status [%d][%s] is not ready.\n",
                status, card_status[status]);
            goto OUT;
        }

        /* set apn */
        ret = HI_3G_SetApn(card, args[4]);
        if(0 != ret)
        {
            printf("auto set apn fail.\n");
            goto OUT;
        }

        /* connect card */
        ix = 0;
        do {
            printf("auto connect times: %d.\n", ix);
            ret = HI_3G_ConnectCard(card, args[1], args[2], args[3], 0, argoption);
            if (ret == 0)
                break;
            sleep(1);
        } while (++ix < MAX_CMD_RETRIES);

        if(0 != ret)
        {
            printf("auto connect card fail.\n");
        }
    }

OUT:
    if (ret == 0xab) return 0;

    if (ret == 0)
        printf(" Ok %s Success.\n", args[0]);
    else
        printf("***ERR %s fail.\n", args[0]);

    return ret;
}


/*adjust input parameter*/
static HI_U8 g3_adjust_string(HI_CHAR *ptr)
{
    HI_U32 i;

    /* Search the first charater char */
    while(*ptr==' ' && *ptr++ != '\0')
    {
        ;
    }

    /* change to little case charater */
    for(i=strlen((char *)ptr); i>0; i--)
    {
        if ((*(ptr+i-1) >= 'A') && (*(ptr+i-1) <= 'Z'))
        {
            *(ptr+i-1) = (*(ptr+i-1) - 'A') + 'a';
        }
    }

    for(i=strlen((char *)ptr);i>0;i--)
    {
        if (*(ptr+i-1) == 0x0a || *(ptr+i-1) == ' ')
        {
            *(ptr+i-1) = '\0';
        }
        else
        {
            break;
        }
    }

    for(i=0; i<MAX_ARGC; i++)
    {
        memset(args[i], 0, MAX_PAMRM_SIZE);
    }
    /* fill args[][] with input string */
    for(i=0; i<MAX_ARGC; i++)
    {
        HI_U8 j = 0;
        while(*ptr==' ' && *ptr++ != '\0')
        ;

        while((*ptr !=' ') && (*ptr!='\0'))
        {
            args[i][j++] = *ptr++;
        }

        args[i][j] = '\0';
        if ('\0' == *ptr)
        {
            i++;
            break;
        }
        args[i][j] = '\0';
    }

    return i;
}

HI_U32 g3_test_cmd(HI_CHAR * u8String)
{
    HI_U8 argc = 0;

    if (NULL == u8String)
    {
        return HI_FAILURE;
    }
    memset(args, 0, sizeof(args));
    argc = g3_adjust_string(u8String);

    if ( (0 == strcmp("q", (char *)args[0])) || (0 == strcmp("Q", (char *)args[0]))
      || (0 == strcmp("quit", (char *)args[0])) || (0 == strcmp("QUIT", (char *)args[0])) )
    {
        printf("quit the program, use extran interface to quit\n");
        return HI_FAILURE;
    }
    g3_parse_arg(argc);

    return HI_SUCCESS;
}

int main(void)
{
    HI_CHAR InputCmd[32];

    card = (HI_3G_CARD_S *)malloc(sizeof(HI_3G_CARD_S));
    if (card == NULL) {
        perror("malloc memory for cards failed!\n");
        return -1;
    }

    printf("please input 'h' to get help or 'q' to quit!\n");

    while (1) {
        printf("3g >");
        memset(InputCmd, 0, 30);
        fgets((char *)InputCmd, 30, stdin);
        if ('q' == InputCmd[0])
        {
            printf("to quit!\n");
            break;
        }

        g3_test_cmd(InputCmd);
    }

    free(card);
    return 0;
}
