#ifndef __SME_PROPERTY_H__
#define __SME_PROPERTY_H__

#include "osal_type.h"

class SmeProperty
{
public:
typedef enum _tagValType
{
   E_VAL_TYPE_UINT8 = 1,
   E_VAL_TYPE_UINT16,
   E_VAL_TYPE_UINT32,
   E_VAL_TYPE_UINT64,
   E_VAL_TYPE_STRING,
   E_VAL_TYPE_BUTTOM,
}E_VAL_TYPE;


    class ConstStringKey
{
public:
        static const  V_CHAR *m_spszKeyLog;/*sme.prop.key.Log*/
        static const  V_CHAR *m_spszKeyPlgPath;/*sme.prop.key.plugin.path*/
        static const  V_CHAR *m_spszKeyStartTime;/*sme.prop.key.starttime*/
        static const  V_CHAR *m_spszKeyFps;/*sme.prop.key.fps*/
        static const  V_CHAR *m_spszKeyCurrFps;/*sme.prop.key.curr.fps*/
        static const  V_CHAR *m_spszKeyDemuxFps;/*sme.prop.key.demux.fps*/
        static const  V_CHAR *m_spszKeyDecodeFps;/*sme.prop.key.decode.fps*/
        static const  V_CHAR *m_spszKeyStreamBitrate;/*sme.prop.key.stream.bitrate*/
    };

    SmeProperty();
    SmeProperty(IN const V_CHAR* pszKey);
    SmeProperty(IN const SmeProperty& objOrg);
    virtual ~SmeProperty();

    V_VOID SetUint8Val(IN V_UINT8 u8Val);
    V_UINT8 GetUint8Val() const;
    V_VOID SetUint16Val(IN V_UINT16 u16Val);
    V_UINT16 GetUint16Val() const;
    V_VOID SetUint32Val(IN V_UINT32 u32Val);
    V_UINT32 GetUint32Val() const;
    V_VOID SetUint64Val(IN V_UINT64 u64Val);
    V_UINT64 GetUint64Val() const;
    V_VOID SetStringVal(IN const V_CHAR* pszVal);
    const V_CHAR* GetStringVal() const;
    V_VOID SetData(IN const V_UINT8* pu8Data, IN V_UINT32 u32Len);
    const V_UINT8* GetData() const;
    V_UINT32 GetDataLen() const;
    E_VAL_TYPE GetValType()const;
    const V_CHAR* GetStringKey()const;
    V_VOID SetValType(IN E_VAL_TYPE eValType);

private:
    const V_CHAR* m_pszKey;
    E_VAL_TYPE m_eValType;
    union{
        V_UINT8 u8Val;
        V_UINT16 u16Val;
        V_UINT32 u32Val;
        V_UINT64 u64Val;
        V_CHAR* pszVal;
    }m_unVal;

    V_UINT32 m_u32Len;
    V_UINT8* m_pu8Data;
};

#endif //__SME_PROPERTY_H__
