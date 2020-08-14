/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCO4ir2/uQ7BFDYEYESXdzFFpPTRP1CkEuTQUs7
G00Y4bEQHXqeukQXzDlltu5030K80Q8fgmX6afxoLRrvo21fSVuK65jTI+NOH3M0IroGVyIH
QY8PNaWteTmWlvWaXZawdu6faYR36mZ7j4IdGN7JgMORW/YbNwJhQvSaUruFZQ==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/************************************************************************
* Copyright(C), 2008-2013, Huawei Tech. Co., Ltd.
* All rights reserved.
*
* $Id: imedia_ini.c,v 1.138 2010/01/12 09:39:22 songxiaogang Exp $
*
* Description：
*
* Reference Doc：
*
* Modification：//见文件尾
*
*************************************************************************/

#include "iMedia_error.h"
#include "iMedia_util.h"
#ifndef __KERNEL__
#include <stdio.h>
#endif
//#if !defined(_MSC_VER)
//#undef  stricmp
//#undef  strnicmp
//#define stricmp  strcasecmp
//#define strnicmp strncasecmp
//#endif

/* a simple .ini file parser and container for Android
 * no sections support. see android/utils/ini.h for
 * more details on the supported file format.
 */
typedef struct STRU_INI_PAIR
{
    char  *key;
    char  *value;
} STRU_INI_PAIR;

typedef struct STRU_INI_FILE
{
    INT32 iNumPairs;
    INT32 iMaxPairs;
    STRU_INI_PAIR  *pstPairs;
} STRU_INI_FILE;

static INT32 iniFile_addPair(STRU_INI_FILE *pstIni, const char  *key, int  keyLen, const char  *value, int valueLen)
{
    STRU_INI_PAIR  *pair;

    // 检测空间是否足够
    if (pstIni->iNumPairs >= pstIni->iMaxPairs)
    {
        INT32 oldMax = pstIni->iMaxPairs;
        INT32 newMax = oldMax + (oldMax >> 1) + 4;
        STRU_INI_PAIR *newPairs = NULL;

        if (newMax < 64)
        { newMax = 64; }

        // 分配新的内存
        newPairs = IMedia_Malloc(newMax * sizeof(STRU_INI_PAIR));

        if (NULL == newPairs)
        {
            return IMEDIA_RET_MEM_MALLOC_FAIL;
        }

        // 释放以前的内存
        if (NULL != pstIni->pstPairs)
        {
            IMedia_Free(pstIni->pstPairs);
        }

        pstIni->pstPairs  = newPairs;
        pstIni->iMaxPairs = newMax;
    }

    pair = pstIni->pstPairs + pstIni->iNumPairs;

    pair->key = IMedia_Malloc(keyLen + valueLen + 2);
    memcpy(pair->key, key, keyLen);
    pair->key[keyLen] = 0;

    pair->value = pair->key + keyLen + 1;
    memcpy(pair->value, value, valueLen);
    pair->value[valueLen] = 0;

    pstIni->iNumPairs += 1;

    return IMEDIA_RET_SUCCESS;
}
/*
static void iniFile_getPair(STRU_INI_FILE* pstIni, int index, const char*  *pKey, const char*  *pValue)
{
    const char*  key   = NULL;
    const char*  value = NULL;

    if (pstIni && index >= 0 && index < pstIni->iNumPairs)
    {
        key   = pstIni->pstPairs[index].key;
        value = pstIni->pstPairs[index].value;
    }

    *pKey   = key;
    *pValue = value;
}*/

/* NOTE: we avoid using <ctype.h> functions to avoid locale-specific
 *       behaviour that can be the source of strange bugs.
 */

static const char *skipSpaces(const char *p)
{
    while (*p == ' ' || *p == '\t')
    { p ++; }

    return p;
}

static const char *skipToEOL(const char  *p)
{
    while (*p && (*p != '\n' && *p != '\r'))
    { p ++; }

    if (*p)
    {
        p ++;

        if (p[-1] == '\r' && p[0] == '\n')
        { p ++; }
    }

    return p;
}

static int isKeyStartChar(int  c)
{
    return ((unsigned)(c - 'a') < 26 ||
            (unsigned)(c - 'A') < 26 ||
            c == '_');
}

static int isKeyChar(int  c)
{
    return isKeyStartChar(c) || ((unsigned)(c - '0') < 10) || (c == '.');
}

// 从内存初始化Ini文件
EXPORT_API INT32  IMedia_Ini_OpenFromMemory(const char *pszText, IMEDIA_INI_FILE *pIni)
{

    STRU_INI_FILE *pstIniFile = NULL;
    const char  *p = pszText;
    int          lineno = 0;

    if (NULL == pIni)
    {
        return IMEDIA_RET_PARAM_NULL;
    }

    pstIniFile = IMedia_Malloc(sizeof(STRU_INI_FILE));

    if (NULL == pstIniFile)
    {
        return IMEDIA_RET_MEM_MALLOC_FAIL;
    }

    memset(pstIniFile, 0, sizeof(STRU_INI_FILE));

    while (*p)
    {
        const char  *key;
        int          keyLen;
        const char  *value;
        int          valueLen;

        lineno++;

        /* skip leading whitespace */
        p = skipSpaces(p);

        /* skip comments and empty lines */
        if (*p == 0 || *p == ';' || *p == '#' || *p == '\n' || *p == '\r')
        {
            p = skipToEOL(p);
            continue;
        }

        /* check the key name */
        key = p++;

        if (!isKeyStartChar(*key))
        {
            p = skipToEOL(p);
            IMEDIA_LOG(IMEDIA_WARNING, "%d: key name doesn't start with valid character. line ignored", lineno);
            continue;
        }

        while (isKeyChar(*p))
        { p++; }

        keyLen = p - key;
        p      = skipSpaces(p);

        /* check the equal */
        if (*p != '=')
        {
            IMEDIA_LOG(IMEDIA_WARNING, "%d: missing expected assignment operator (=). line ignored", lineno);
            p = skipToEOL(p);
            continue;
        }

        p++;

        /* skip spaces before the value */
        p     = skipSpaces(p);
        value = p;

        /* find the value */
        while (*p && (*p != ';' && *p != '#' && *p != '\n' && *p != '\r'))
        { p++; }

        /* remove trailing spaces */
        while (p > value && (p[-1] == ' ' || p[-1] == '\t'))
        { p--; }

        valueLen = p - value;

        (void)iniFile_addPair(pstIniFile, key, keyLen, value, valueLen);
        IMEDIA_LOG(IMEDIA_DEBUG, "%d: KEY='%.*s' VALUE='%.*s'", lineno, keyLen, key, valueLen, value);

        p = skipToEOL(p);
    }

    *pIni = (IMEDIA_INI_FILE)pstIniFile;

    return IMEDIA_RET_SUCCESS;
}

EXPORT_API INT32  IMedia_Ini_OpenFromFile(const char *pszFileName, IMEDIA_INI_FILE *pIni)
{
    INT32 ret = 0;
    FILE *fp = NULL;
    char *text = NULL;
    int  size = 0;

    if (NULL == pIni)
    {
        return IMEDIA_RET_PARAM_NULL;
    }

    fp = fopen(pszFileName, "rt");

    if (NULL == fp)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "could not open file: %s err: %s", pszFileName, strerror(errno));
        return IMEDIA_RET_FILE_OPEN_FAIL;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* avoid reading a very large file that was passed by mistake
     * this threshold is quite liberal.
     */
#define  MAX_INI_FILE_SIZE  655360

    if (size < 0 || size > MAX_INI_FILE_SIZE)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "hardware configuration file '%s' too large (%ld bytes)", pszFileName, size);
        goto EXIT;
    }

    /* read the file, add a sentinel at the end of it */
    text = IMedia_Malloc(size + 1);
    fread(text, 1, size, fp);
    text[size] = 0;

    ret = IMedia_Ini_OpenFromMemory(text, pIni);
    IMedia_Free(text);

EXIT:
    fclose(fp);

    return ret;
}

EXPORT_API void IMedia_Ini_Close(IMEDIA_INI_FILE ini)
{
    INT32 i = 0;
    STRU_INI_FILE *p = (STRU_INI_FILE *)ini;

    for (i = 0; i < p->iNumPairs; i++)
    {
        IMedia_Free(p->pstPairs[i].key);
        p->pstPairs[i].key   = NULL;
        p->pstPairs[i].value = NULL;
    }

    IMedia_Free(p->pstPairs);
    IMedia_Free(p);
}

EXPORT_API const char *IMedia_Ini_GetValue(IMEDIA_INI_FILE ini, const char *key)
{
    STRU_INI_FILE *p = (STRU_INI_FILE *)ini;

    if (p && key)
    {
        INT32 i;

        // search from end
        for (i = p->iNumPairs - 1; i >= 0; i--)
        {
            if (!stricmp(p->pstPairs[i].key, key))
            { return p->pstPairs[i].value; }
        }
    }

    return NULL;
}

EXPORT_API int IMedia_Ini_GetInteger(IMEDIA_INI_FILE ini, const char *key, INT32 defaultValue)
{
    const char  *valueStr = IMedia_Ini_GetValue(ini, key);
    int          value    = defaultValue;

    if (valueStr != NULL)
    {
        char  *end;
        long   l = strtol(valueStr, &end, 10);

        if (end != NULL && end[0] == 0 && (int)l == l)
        { value = l; }
    }

    return value;
}

EXPORT_API double IMedia_Ini_GetDouble(IMEDIA_INI_FILE ini, const char *key, double defaultValue)
{
    const char  *valueStr = IMedia_Ini_GetValue(ini, key);
    double       value    = defaultValue;

    if (valueStr != NULL)
    {
        char   *end;
        double  d = strtod(valueStr, &end);

        if (end != NULL && end[0] == 0)
        { value = d; }
    }

    return value;
}

EXPORT_API INT32  IMedia_Ini_GetBoolean(IMEDIA_INI_FILE ini, const char *key, const char *defaultValue)
{
    const char  *value  = IMedia_Ini_GetValue(ini, key);

    if (!value)
    { value = defaultValue; }

    if (!strncmp(value, "1", 2)    ||
        !stricmp(value, "yes")  ||
        !stricmp(value, "true"))
    {
        return 1;
    }
    else
    { return 0; }
}

/************************************************************************
* $Log$
************************************************************************/
