#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#endif

#include "hi_module.h"
#include "hi_drv_mem.h"
#include "hi_db.h"


/***********************************************************************
the db struct:

DB :   | dbcheck | datalen | table 0 | ... ... | table n |

TABLE: | name | datasize | key 0 | ... ...| key n |

KEY:   | name | valuelen | value |
************************************************************************/

#ifdef __KERNEL__
#define DB_MALLOC(size)     HI_VMALLOC(HI_ID_PDM, size)
#define DB_FREE(p)          HI_VFREE(HI_ID_PDM, p)
#else
#define DB_MALLOC(size)     malloc(size)
#define DB_FREE(p)          free(p)
#endif


#define HI_ERR_DB(format, arg...)
#define HI_INFO_DB(format, arg...)


static HI_CHAR g_DBCheck[HI_DB_CHECK_LEN] = {"###"};


HI_S32 HI_DB_Create(HI_DB_S *pstDB)
{
    if(HI_NULL == pstDB)
    {
        HI_ERR_DB("ERR: para is null\n");
        return HI_FAILURE;
    }

    pstDB->pData = DB_MALLOC(HI_DB_MAX_SIZE);
    if(HI_NULL == pstDB->pData)
    {
        HI_ERR_DB("ERR: malloc error\n");
        return HI_FAILURE;
    }

    memset(pstDB->pData, 0x00, HI_DB_MAX_SIZE);

    memcpy(pstDB->pData, (HI_U8 *)g_DBCheck, sizeof(g_DBCheck));

    pstDB->u32DataLen = HI_DB_CHECK_LEN + sizeof(HI_U32);
    memcpy(pstDB->pData + HI_DB_CHECK_LEN, (HI_U8 *)(&(pstDB->u32DataLen)), sizeof(HI_U32));

    return HI_SUCCESS;
}

HI_S32 HI_DB_Destroy(HI_DB_S *pstDB)
{    
    if(HI_NULL == pstDB)
    {
        HI_ERR_DB("ERR: para is null\n");
        return HI_FAILURE;
    }

    if(pstDB->u32DataLen > HI_DB_MAX_SIZE)
    {
        HI_ERR_DB("ERR: data len too large\n");
        return HI_FAILURE;
    }
    
    pstDB->u32DataLen = 0;

    if(HI_NULL != pstDB->pData)
    {
        DB_FREE(pstDB->pData);
        pstDB->pData = HI_NULL;
    }
    
    return HI_SUCCESS;
}

HI_S32 HI_DB_GetDBFromMem(HI_U8 *pDBMem, HI_DB_S *pstDB)
{
    HI_CHAR DBCheck[HI_DB_CHECK_LEN] = {0};
    
    if(HI_NULL == pDBMem || HI_NULL == pstDB)
    {
        HI_ERR_DB("ERR: para is null\n");
        return HI_FAILURE;
    }
    
    memcpy(DBCheck, (HI_CHAR *)pDBMem, HI_DB_CHECK_LEN);

    if(0 != strncmp(DBCheck, g_DBCheck, strlen(g_DBCheck)))
    {
        HI_ERR_DB("ERR: this is not a db\n");
        return HI_FAILURE;
    }
    
    pstDB->u32DataLen = *(HI_U32 *)(pDBMem + HI_DB_CHECK_LEN);
    if(pstDB->u32DataLen > HI_DB_MAX_SIZE)
    {
        HI_ERR_DB("ERR: db size is error\n");
        return HI_FAILURE;    
    }
    
    pstDB->pData = pDBMem;

    return HI_SUCCESS;
}

HI_S32 HI_DB_InsertTable(HI_DB_S *pstDB, HI_DB_TABLE_S *pstTable)
{
    HI_U32  NeedLen = 0;
    
    if(HI_NULL == pstDB || HI_NULL == pstTable)
    {
        HI_ERR_DB("ERR: para is null\n");
        return HI_FAILURE;
    }

    NeedLen = HI_DB_MAX_NAME_LEN + sizeof(HI_U32);
    if(pstDB->u32DataLen + NeedLen > HI_DB_MAX_SIZE)
    {
        HI_ERR_DB("ERR: db is full\n");
        return HI_FAILURE;
    }  

    pstTable->pstDB = pstDB;

    memcpy((pstDB->pData +  pstDB->u32DataLen), pstTable->Name, HI_DB_MAX_NAME_LEN);
    pstDB->u32DataLen += HI_DB_MAX_NAME_LEN;

    pstTable->u32DataSize = 0;
    memcpy((pstDB->pData +  pstDB->u32DataLen), (HI_U8 *)(&(pstTable->u32DataSize)), sizeof(HI_U32));
    pstDB->u32DataLen += sizeof(HI_U32);

    /*we need to update db datalen*/
    memcpy(pstDB->pData + HI_DB_CHECK_LEN, (HI_U8 *)(&(pstDB->u32DataLen)), sizeof(HI_U32));
    
    pstTable->pData = pstDB->pData + pstDB->u32DataLen;

    return HI_SUCCESS;
}

HI_S32 HI_DB_DeleteTable(HI_DB_S *pstDB, HI_CHAR TableName[])
{   
    HI_ERR_DB("ERR: delete table is not support\n");
    return HI_FAILURE;
}

HI_S32 HI_DB_GetTableByName(HI_DB_S *pstDB, HI_CHAR TableName[], HI_DB_TABLE_S *pstTable)
{
    HI_DB_TABLE_S       stTable;
    HI_U8               *pDBStr;

    if(HI_NULL == pstDB || HI_NULL == pstTable || HI_NULL == TableName)
    {
        HI_ERR_DB("ERR: para is null\n");
        return HI_FAILURE;
    }
    
    pDBStr = pstDB->pData + HI_DB_CHECK_LEN + sizeof(HI_U32);

    while((HI_U32)(pDBStr - pstDB->pData) < pstDB->u32DataLen)
    {
        stTable.pstDB = pstDB;

		memset(stTable.Name, 0, sizeof(stTable.Name));
        memcpy(stTable.Name, pDBStr, HI_DB_MAX_NAME_LEN - 1);
        pDBStr += HI_DB_MAX_NAME_LEN;
        
        stTable.u32DataSize = *((HI_U32 *)(pDBStr));
        pDBStr += sizeof(HI_U32);

        stTable.pData = pDBStr;

        /*find the table*/
        if( 0 == strncmp(stTable.Name, TableName, strlen(TableName)) )
        {
            *pstTable = stTable;
            break;
        }
        else
        {
            pDBStr += stTable.u32DataSize;
        }
    }

    if((HI_U32)(pDBStr - pstDB->pData) >= pstDB->u32DataLen)
    {
        HI_ERR_DB("ERR: can not find table %s\n", TableName);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 HI_DB_InsertKey(HI_DB_TABLE_S *pstTable, HI_DB_KEY_S *pstKey)
{
    HI_U32  NeedLen = 0;
    
    if(HI_NULL == pstTable || HI_NULL == pstKey)
    {
        HI_ERR_DB("ERR: para is null\n");
        return HI_FAILURE;
    }

    NeedLen = HI_DB_MAX_NAME_LEN + sizeof(HI_U32) + pstKey->u32ValueSize;
    if(pstTable->pstDB->u32DataLen + NeedLen > HI_DB_MAX_SIZE)
    {
        HI_ERR_DB("ERR: db is full\n");
        return HI_FAILURE;
    }


    memcpy((pstTable->pData + pstTable->u32DataSize), pstKey->Name, HI_DB_MAX_NAME_LEN);
    pstTable->u32DataSize += HI_DB_MAX_NAME_LEN;

    memcpy((pstTable->pData + pstTable->u32DataSize), (HI_U8 *)(&(pstKey->u32ValueSize)), sizeof(HI_U32));
    pstTable->u32DataSize += sizeof(HI_U32);

    memcpy((pstTable->pData +  pstTable->u32DataSize), (HI_U8 *)(pstKey->pValue), pstKey->u32ValueSize);
    pstTable->u32DataSize += pstKey->u32ValueSize;  

    /*we need update table datasize*/
    memcpy(pstTable->pData - sizeof(HI_U32), (HI_U8 *)(&(pstTable->u32DataSize)), sizeof(HI_U32));

    /*we need update db datalen*/
    pstTable->pstDB->u32DataLen += NeedLen;
    memcpy(pstTable->pstDB->pData + HI_DB_CHECK_LEN, (HI_U8 *)(&(pstTable->pstDB->u32DataLen)), sizeof(HI_U32));

    return HI_SUCCESS;
}

HI_S32 HI_DB_DeleteKey(HI_DB_TABLE_S *pstTable, HI_CHAR KeyName[])
{
    HI_ERR_DB("ERR: delete key is not support\n");
    return HI_FAILURE;
}

HI_S32 HI_DB_GetKeyByName(HI_DB_TABLE_S *pstTable, HI_CHAR KeyName[], HI_DB_KEY_S *pstKey)
{
    HI_DB_KEY_S         stKey;
    HI_U8               *pTableStr;

    if(HI_NULL == pstTable || HI_NULL == pstKey || HI_NULL == KeyName)
    {
        HI_ERR_DB("ERR: para is null\n");
        return HI_FAILURE;
    }
    
    pTableStr = pstTable->pData;

    while((HI_U32)(pTableStr - pstTable->pData) < pstTable->u32DataSize)
    {
    	memset(stKey.Name, 0, sizeof(stKey.Name));
        memcpy(stKey.Name, pTableStr, HI_DB_MAX_NAME_LEN - 1);
        pTableStr += HI_DB_MAX_NAME_LEN;
        
        stKey.u32ValueSize = *((HI_U32 *)pTableStr);
        pTableStr += sizeof(HI_U32);

        stKey.pValue= pTableStr;

        /*find the key*/
        if( 0 == strncmp(stKey.Name, KeyName, strlen(KeyName)) )
        {
            *pstKey = stKey;
            break;
        }
        else
        {
            pTableStr += stKey.u32ValueSize;
        }
    }

    if((HI_U32)(pTableStr - pstTable->pData) >= pstTable->u32DataSize)
    {
        HI_INFO_DB("ERR: can not find key %s\n", KeyName);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}
