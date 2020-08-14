#ifndef _UTILS_BENCHMARK_H_
#define _UTILS_BENCHMARK_H_


#include "sme_player_type.h"
#include "sme_player_inf.h"
#include "sme_proc.h"


class SmeBenchMark
{
private:
    ST_PROC_COMMON_INFO                           m_stCommonInfo;
    ST_SMEDEMUX_PROC_INFO                         m_stSmeDemuxInfo;
    ST_VIDEODEC_PROC_INFO                         m_stVdecInfo;
    ST_AUDIODEC_PROC_INFO                         m_stAdecInfo;
    ST_VIDEOSINK_PROC_INFO                        m_stVsinkInfo;
    ST_AUDIOSINK_PROC_INFO                        m_stAsinkInfo;
    ST_SMEHWTSDEMUX_PROC_INFO                     m_stTsDemuxInfo;

private :
    V_INT32 SME_CreateProcDir();
public:
    V_VOID*                                       m_pPlayBinHandler;
    V_VOID*                                       m_pImplHandler;
    ST_SME_PLAYER_CONTROL_PARA                    m_stPlayerControlPara;
    E_PLAYER_TYPE                                 m_ePlayerType;

SmeBenchMark();
    virtual  ~ SmeBenchMark();
    V_INT32 SME_CreateBenchMark(V_VOID* pPlaybinHandle, V_VOID *pImplHandle, E_PLAYER_TYPE ePlayerType);
    V_VOID SME_DestroyBenchMark() const;
};



#endif //_UTILS_BENCHMARK_H_
