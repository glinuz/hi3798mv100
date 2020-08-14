/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : sme_property.cpp
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2014-04-09
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  日    期   :
  作    者   :
  修改内容   : 创建文件
*-------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*  -----------------------------------------------------------------------------
*
*******************************************************************************/
#include <string.h>
#include "sme_property.h"
#include "osal_type.h"
#include "sme_log.h"
#include "sme_macro.h"
#include "osal_mem.h"

//lint -e1732
/*lint -e838*/ // initial
//lint -e1055  //Symbol 'strlen' undeclared, assumed to return int


SmeProperty::SmeProperty(IN const SmeProperty& objOrg)
:m_pszKey(NULL)
,m_eValType(E_VAL_TYPE_BUTTOM)
,m_u32Len(0)
,m_pu8Data(NULL)

{
    E_SME_MEM_SECURE_RET eErr = E_SME_MEM_OK;
    m_pszKey = objOrg.GetStringKey();
    m_eValType = objOrg.GetValType();
    m_unVal.u64Val = 0;
    switch(m_eValType)
    {
        case E_VAL_TYPE_UINT8:
            m_unVal.u8Val = objOrg.GetUint8Val();
            break;
        case E_VAL_TYPE_UINT16:
            m_unVal.u16Val = objOrg.GetUint16Val();
            break;
        case E_VAL_TYPE_UINT32:
            m_unVal.u32Val = objOrg.GetUint32Val();
            break;
        case E_VAL_TYPE_UINT64:
            m_unVal.u64Val = objOrg.GetUint64Val();
            break;
        case E_VAL_TYPE_STRING:
            if(NULL == objOrg.GetStringVal())
            {
                break;
            }
            m_unVal.pszVal = new V_CHAR[(V_UINT32)(strlen(objOrg.GetStringVal()) + 1)];
            if(NULL == m_unVal.pszVal)
            {
                ICS_LOGE("m_unVal.pszVal : new Failed");
                break;
            }
            eErr = VOS_Memcpy_S(m_unVal.pszVal, (V_UINT32)(strlen(objOrg.GetStringVal()) + 1),
                objOrg.GetStringVal(), (V_UINT32)(strlen(objOrg.GetStringVal()) + 1));
            if(E_SME_MEM_OK != eErr)
            {
                ICS_LOGE("VOS_Memcpy_S m_unVal.pszVal : new Failed(%d)", eErr);
            }
            break;
        default:
            ICS_LOGE("unknown Type");
            break;
    }

    m_u32Len = objOrg.GetDataLen();
    if((m_u32Len > 0) && (NULL != objOrg.GetData()))
    {
       //copy data;
       //失败的话，需要日志表明，及清空len及其他成员。
       m_pu8Data = new V_UINT8[m_u32Len];
       if(NULL == m_pu8Data)
       {
            if(E_VAL_TYPE_STRING == m_eValType)
            {
                UTILS_MSAFEDELA(m_unVal.pszVal);
            }
            ICS_LOGE("m_pu8Data : new Failed");
            m_u32Len = 0;
       }
       else

       {
            eErr = VOS_Memcpy_S(m_pu8Data, m_u32Len,
                objOrg.GetData(), m_u32Len);
            if(E_SME_MEM_OK != eErr)
            {
                ICS_LOGE("VOS_Memcpy_S m_pu8Data : new Failed(%d)", eErr);
            }
       }
    }

}
SmeProperty::SmeProperty(IN const V_CHAR* pszKey)
:m_pszKey(NULL)
,m_eValType(E_VAL_TYPE_BUTTOM)
,m_u32Len(0)
,m_pu8Data(NULL)
{
    m_unVal.u64Val = 0;
    m_pszKey = pszKey;
}
SmeProperty::SmeProperty()
:m_pszKey(NULL)
,m_eValType(E_VAL_TYPE_BUTTOM)
,m_u32Len(0)
,m_pu8Data(NULL)
{
    m_unVal.u64Val = 0;
}
SmeProperty::~SmeProperty()
{

        m_pszKey = NULL;


    UTILS_MSAFEDELA(m_pu8Data);
    m_u32Len = 0;

    if(E_VAL_TYPE_STRING == m_eValType)
    {
        UTILS_MSAFEDELA(m_unVal.pszVal);
    }

    m_unVal.u64Val = 0;
    m_eValType = E_VAL_TYPE_BUTTOM;
}

const V_CHAR* SmeProperty::ConstStringKey::m_spszKeyLog = "sme.prop.key.Log";
const V_CHAR* SmeProperty::ConstStringKey::m_spszKeyPlgPath = "sme.prop.key.plugin.path";
const V_CHAR* SmeProperty::ConstStringKey::m_spszKeyStartTime = "sme.prop.key.starttime";
const V_CHAR* SmeProperty::ConstStringKey::m_spszKeyFps = "sme.prop.key.fps";
const V_CHAR* SmeProperty::ConstStringKey::m_spszKeyCurrFps = "sme.prop.key.curr.fps";
const V_CHAR* SmeProperty::ConstStringKey::m_spszKeyDemuxFps = "sme.prop.key.demux.fps";
const V_CHAR* SmeProperty::ConstStringKey::m_spszKeyDecodeFps = "sme.prop.key.decode.fps";
const V_CHAR* SmeProperty::ConstStringKey::m_spszKeyStreamBitrate = "sme.prop.key.stream.bitrate";

V_VOID SmeProperty::SetUint8Val(V_UINT8 u8Val)
{
    ICS_LOGD("SetUint8Val:%d", u8Val);

    //如果之前是字符串才释放
    if(E_VAL_TYPE_STRING == m_eValType)
    {
        UTILS_MSAFEDELA(m_unVal.pszVal);
        m_eValType = E_VAL_TYPE_BUTTOM;
    }

    m_unVal.u64Val = 0;
    m_unVal.u8Val = u8Val;
    m_eValType = E_VAL_TYPE_UINT8;
}

V_UINT8 SmeProperty::GetUint8Val() const
{
    ICS_LOGD("GetUint8Val:%d", m_unVal.u8Val);
    UTILS_MLOGE_RET_VAL_IF(E_VAL_TYPE_UINT8 != m_eValType, 0,
        ("Invalid Opertion, mType:%u", m_eValType));

        return m_unVal.u8Val;
    }


V_VOID SmeProperty::SetUint16Val(V_UINT16 u16Val)
{
    ICS_LOGD("SetUint16Val:%d", u16Val);

    //如果之前是字符串才释放
    if(E_VAL_TYPE_STRING == m_eValType)
    {
        UTILS_MSAFEDELA(m_unVal.pszVal);
        m_eValType = E_VAL_TYPE_BUTTOM;
    }

    m_unVal.u64Val = 0;
    m_unVal.u16Val = u16Val;
    m_eValType = E_VAL_TYPE_UINT16;
}

V_UINT16 SmeProperty::GetUint16Val() const
{
    ICS_LOGD("GetUint16Val:%d", m_unVal.u16Val);
    UTILS_MLOGE_RET_VAL_IF(E_VAL_TYPE_UINT16 != m_eValType, 0,
        ("Invalid Opertion, mType:%u", m_eValType));

        return m_unVal.u16Val;
    }



V_VOID SmeProperty::SetUint32Val(V_UINT32 u32Val)
{
    ICS_LOGD("SetUint32Val:%d", u32Val);

    //如果之前是字符串才释放
    if(E_VAL_TYPE_STRING == m_eValType)
    {
        UTILS_MSAFEDELA(m_unVal.pszVal);
        m_eValType = E_VAL_TYPE_BUTTOM;
    }

    m_unVal.u64Val = 0;
    m_unVal.u32Val = u32Val;
    m_eValType = E_VAL_TYPE_UINT32;
}

V_UINT32 SmeProperty::GetUint32Val() const
{
    ICS_LOGD("GetUint32Val:%d", m_unVal.u32Val);
    UTILS_MLOGE_RET_VAL_IF(E_VAL_TYPE_UINT32 != m_eValType, 0,
        ("Invalid Opertion, mType:%u", m_eValType));

        return m_unVal.u32Val;
    }



V_VOID SmeProperty::SetUint64Val(V_UINT64 u64Val)
{
    ICS_LOGD("SetUint64Val:%lld", u64Val);

    //如果之前是字符串才释放
    if(E_VAL_TYPE_STRING == m_eValType)
    {
        UTILS_MSAFEDELA(m_unVal.pszVal);
        m_eValType = E_VAL_TYPE_BUTTOM;
    }

    m_unVal.u64Val = 0;
    m_unVal.u64Val = u64Val;
    m_eValType = E_VAL_TYPE_UINT64;
}

V_UINT64 SmeProperty::GetUint64Val() const
{
    ICS_LOGD("GetUint64Val:%lld", m_unVal.u64Val);
    UTILS_MLOGE_RET_VAL_IF(E_VAL_TYPE_UINT64 != m_eValType, 0,
        ("Invalid Opertion, mType:%u", m_eValType));

         return m_unVal.u64Val;
    }


V_VOID SmeProperty::SetStringVal( const V_CHAR* pszVal)
{
    E_SME_MEM_SECURE_RET eErr = E_SME_MEM_OK;

    if(NULL == pszVal)
    {
        m_eValType = E_VAL_TYPE_BUTTOM;
        ICS_LOGE("pszVal : NULL");
        return ;
    }
    ICS_LOGD("SetStringVal:%s", pszVal);

    //如果之前是字符串才释放
    if(E_VAL_TYPE_STRING == m_eValType)
    {
        UTILS_MSAFEDELA(m_unVal.pszVal);
        m_eValType = E_VAL_TYPE_BUTTOM;
    }

    m_unVal.pszVal = new V_CHAR[(V_UINT32)(strlen(pszVal) + 1)];
    if(NULL == m_unVal.pszVal)
    {
        ICS_LOGE("m_unVal.pszVal : new Failed");
        return ;
    }

    eErr = VOS_Memcpy_S(m_unVal.pszVal, (V_UINT32)strlen(pszVal) + 1,
        pszVal, (V_UINT32)strlen(pszVal) + 1);
    if(E_SME_MEM_OK != eErr)
    {
        ICS_LOGE("VOS_Memcpy_S m_unVal.pszVal : new Failed(%d)", eErr);
    }

    m_eValType = E_VAL_TYPE_STRING;

    return ;
}

const V_CHAR* SmeProperty::GetStringVal() const
{
    UTILS_MLOGE_RET_VAL_IF(E_VAL_TYPE_STRING != m_eValType, NULL,
        ("Invalid Opertion, mType:%u", m_eValType));

         return m_unVal.pszVal;
    }


V_VOID SmeProperty::SetData(const V_UINT8* pu8Data, V_UINT32 u32Len)
{
    if((u32Len > 0) && (NULL != pu8Data))
    {
        m_u32Len = u32Len;
        UTILS_MSAFEDELA(m_pu8Data);
        //copy data;
        //失败的话，需要日志表明，及清空len及其他成员。

        m_pu8Data = new V_UINT8[m_u32Len];
        if(NULL == m_pu8Data)
        {
            if(E_VAL_TYPE_STRING == m_eValType)
            {
                UTILS_MSAFEDELA(m_unVal.pszVal);
            }
            ICS_LOGE("m_pu8Data : new Failed");
            m_u32Len = 0;
        }
        else
        {
            E_SME_MEM_SECURE_RET eErr = E_SME_MEM_OK;
            eErr = VOS_Memcpy_S(m_pu8Data, m_u32Len, pu8Data, m_u32Len);
            if(E_SME_MEM_OK != eErr)
            {
                ICS_LOGE("VOS_Memcpy_S m_pu8Data : new Failed(%d)", eErr);
            }
        }
    }

    return ;
}
V_VOID SmeProperty::SetValType(IN E_VAL_TYPE eValType)
{
    m_eValType = eValType;
    return;
}

const V_UINT8* SmeProperty::GetData() const
{
   return m_pu8Data;
}

V_UINT32 SmeProperty::GetDataLen() const
{
   return m_u32Len;
}

SmeProperty::E_VAL_TYPE SmeProperty::GetValType()const
{
    return m_eValType;
}

const V_CHAR* SmeProperty::GetStringKey()const
{
    return m_pszKey;
}
