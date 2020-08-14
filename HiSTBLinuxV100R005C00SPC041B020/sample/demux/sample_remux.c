
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <assert.h>

#include "hi_unf_common.h"
#include "hi_unf_demux.h"
#include "hi_mpi_demux.h"
#include "hi_adp_mpi.h"

#define BITS_PER_BYTE       8
#define BITS_PER_LONG              (sizeof(unsigned long) * BITS_PER_BYTE)
#define BIT_MASK(nr)              (1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)              ((nr) / BITS_PER_LONG)
#define DIV_ROUND_UP(n,d)      (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)   DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#define DECLARE_BITMAP(name,bits) \
                        unsigned long name[BITS_TO_LONGS(bits)]

static inline void bitmap_zero(unsigned long *dst, int nbits)
{
    int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
    memset(dst, 0, len);
}

static inline void bitmap_setbit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

    *p  |= mask;
}

static inline void bitmap_clrbit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

    *p &= ~mask;
}

static unsigned long __ffs(unsigned long word)
{
    int num = 0;

    if ((word & 0xffff) == 0) {
        num += 16;
        word >>= 16;
    }
    if ((word & 0xff) == 0) {
        num += 8;
        word >>= 8;
    }
    if ((word & 0xf) == 0) {
        num += 4;
        word >>= 4;
    }
    if ((word & 0x3) == 0) {
        num += 2;
        word >>= 2;
    }
    if ((word & 0x1) == 0)
        num += 1;
    return num;
}

#define ffz(x) __ffs( ~(x) )

unsigned long find_first_zero_bit(const unsigned long *addr, unsigned long size)
{
    const unsigned long *p = addr;
    unsigned long result = 0;
    unsigned long tmp;

    while (size & ~(BITS_PER_LONG-1)) {
        if (~(tmp = *(p++)))
            goto found;
        result += BITS_PER_LONG;
        size -= BITS_PER_LONG;
    }
    if (!size)
        return result;

    tmp = (*p) | (~0UL << size);
    if (tmp == ~0UL)    /* Are any bits zero? */
        return result + size;   /* Nope. */
found:
    return result + ffz(tmp);
}

unsigned long find_first_bit(const unsigned long *addr, unsigned long size)
{
    const unsigned long *p = addr;
    unsigned long result = 0;
    unsigned long tmp;

    while (size & ~(BITS_PER_LONG-1)) {
        if ((tmp = *(p++)))
            goto found;
        result += BITS_PER_LONG;
        size -= BITS_PER_LONG;
    }
    if (!size)
        return result;

    tmp = (*p) & (~0UL >> (BITS_PER_LONG - size));
    if (tmp == 0UL)     /* Are any bits set? */
        return result + size;   /* Nope. */
found:
    return result + __ffs(tmp);
}

#define BITOP_WORD(nr)      ((nr) / BITS_PER_LONG)
unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
                unsigned long offset)
{
    const unsigned long *p = addr + BITOP_WORD(offset);
    unsigned long result = offset & ~(BITS_PER_LONG-1);
    unsigned long tmp;

    if (offset >= size)
        return size;
    size -= result;
    offset %= BITS_PER_LONG;
    if (offset) {
        tmp = *(p++);
        tmp &= (~0UL << offset);
        if (size < BITS_PER_LONG)
            goto found_first;
        if (tmp)
            goto found_middle;
        size -= BITS_PER_LONG;
        result += BITS_PER_LONG;
    }
    while (size & ~(BITS_PER_LONG-1)) {
        if ((tmp = *(p++)))
            goto found_middle;
        result += BITS_PER_LONG;
        size -= BITS_PER_LONG;
    }
    if (!size)
        return result;
    tmp = *p;

found_first:
    tmp &= (~0UL >> (BITS_PER_LONG - size));
    if (tmp == 0UL)     /* Are any bits set? */
        return result + size;   /* Nope. */
found_middle:
    return result + __ffs(tmp);
}

#define for_each_set_bit(bit, addr, size) \
    for ((bit) = find_first_bit((addr), (size));        \
         (bit) < (size);                    \
         (bit) = find_next_bit((addr), (size), (bit) + 1))

#define DEMUX_ID_0 (0)
#define PORT_ID_0 (HI_UNF_DMX_PORT_RAM_0)

#define DEMUX_ID_1 (1)
#define PORT_ID_1 (HI_UNF_DMX_PORT_RAM_1)

#define PORT_ID_2 (HI_UNF_DMX_PORT_RAM_2)

#define TS_PAYLOAD_OFFSET (5)

FILE *g_TsFile_0 = HI_NULL, *g_TsFile_1 = HI_NULL;
HI_HANDLE g_TsBuf_0, g_TsBuf_1;
PMT_COMPACT_TBL    *g_ProgTbl_0 = HI_NULL, *g_ProgTbl_1 = HI_NULL;

struct TsThreadCtl
{
    HI_HANDLE TsBufHandle;
    FILE *TsFileHandle;
    HI_BOOL Stop;
    pthread_mutex_t Lock;
};

struct TsThreadCtl g_TsThreadCtl_0, g_TsThreadCtl_1;
pthread_t g_TsThd_0, g_TsThd_1;

struct CastTableCtl
{
    PMT_COMPACT_PROG *ProgInfo_0;
    PMT_COMPACT_PROG *ProgInfo_1;
    HI_BOOL Stop;
};

struct CastTableCtl g_CastTableCtl;
pthread_t g_CastTableThd;

HI_HANDLE g_hRmx;

struct TsRecCtl
{
    HI_HANDLE RecHandle;
    HI_BOOL Stop;
};

#define DEMUX_ID_2 (2)
struct TsRecCtl g_RecThreadCtl;
HI_HANDLE g_RecHandle;
pthread_t g_RecThd;
HI_CHAR TsFileName[128] = "rmx_allts_rec.ts";

#define PID_ARRAY_SIZE (0x1fff)
struct PidCtl
{
    pthread_mutex_t Lock;
    DECLARE_BITMAP(PidBitmap, PID_ARRAY_SIZE);
};

struct PidCtl g_PidCtl;

static HI_VOID InitPidCtl(HI_VOID)
{
    HI_U32 idx;

    pthread_mutex_init(&g_PidCtl.Lock, HI_NULL);

    bitmap_zero(g_PidCtl.PidBitmap, PID_ARRAY_SIZE);

    /* 0 ~ 0x20 is reserved range */
    for (idx = 0; idx < 0x20; idx ++)
    {
        bitmap_setbit(idx, g_PidCtl.PidBitmap);
    }
}

static HI_U32 mCrcTable[256];

static HI_VOID InitCrcTable(HI_VOID)
{
    int i ;
    HI_U32 poly = 0x04C11DB7;
    for (i = 0; i < 256; i++)
    {
        HI_U32 crc = i << 24;
        int j;
        for (j = 0; j < 8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80000000) ? (poly) : 0);
        }
        mCrcTable[i] = crc;
    }
}

static HI_U32 crc32(const HI_U8 *start, HI_U32 size)
{
    HI_U32 crc = 0xFFFFFFFF;
    const HI_U8 *p;
    for (p = start; p < start + size; ++p)
    {
        crc = (crc << 8) ^ mCrcTable[((crc >> 24) ^ *p) & 0xFF];
    }
    return crc;
}

HI_VOID TsThread(HI_VOID *args)
{
    struct TsThreadCtl *Ctl = (struct TsThreadCtl *)args;
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32                Readlen;
    HI_S32                Ret;

    while (!Ctl->Stop)
    {
        pthread_mutex_lock(&Ctl->Lock);

        Ret = HI_UNF_DMX_GetTSBuffer(Ctl->TsBufHandle, 188*10, &StreamBuf, 1000);
        if (Ret != HI_SUCCESS )
        {
            pthread_mutex_unlock(&Ctl->Lock);
            continue;
        }

        Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 188*10, Ctl->TsFileHandle);
        if(Readlen <= 0)
        {
            printf("read ts file end and rewind!\n");
            rewind(Ctl->TsFileHandle);
            pthread_mutex_unlock(&Ctl->Lock);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(Ctl->TsBufHandle, Readlen);
        if (Ret != HI_SUCCESS )
        {
            printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }

        usleep(1000);

        pthread_mutex_unlock(&Ctl->Lock);
    }

    Ret = HI_UNF_DMX_ResetTSBuffer(Ctl->TsBufHandle);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_DMX_ResetTSBuffer failed.\n");
    }

    return;
}

HI_S32 AddRemuxPidChannel(HI_HANDLE hRmx, HI_UNF_DMX_PORT_E PortId, HI_U32 Pid, HI_U32 RemapPid)
{
    HI_S32 ret  = HI_FAILURE;
    HI_HANDLE hPump;
    HI_MPI_RMX_PUMP_ATTR_S Attr;
    unsigned long mask, *p;

    ret = HI_MPI_RMX_GetPumpDefaultAttr(&Attr);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_RMX_GetPumpDefaultAttr failed.\n");
        goto out0;
    }

    assert(Pid < 0x1fff);

    pthread_mutex_lock(&g_PidCtl.Lock);

    mask = BIT_MASK(RemapPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(RemapPid);
    if (RemapPid < 0x1fff && *p & mask)
    {
        Attr.enPumpType = HI_MPI_RMX_PUMP_TYPE_REMAP_PID;
        Attr.enInPortId = PortId;
        Attr.u32Pid = Pid;
        Attr.u32RemapPid = RemapPid;
    }
    else
    {
        Attr.enPumpType = HI_MPI_RMX_PUMP_TYPE_PID;
        Attr.enInPortId = PortId;
        Attr.u32Pid = Pid;
    }

    pthread_mutex_unlock(&g_PidCtl.Lock);

    ret = HI_MPI_RMX_AddPump(hRmx, &Attr, &hPump);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_RMX_AddPump failed.\n");
        goto out0;
    }

    return HI_SUCCESS;

out0:
    return ret;
}

HI_S32 AddRemuxAllpassPort(HI_HANDLE hRmx, HI_UNF_DMX_PORT_E PortId)
{
    HI_S32 ret  = HI_FAILURE;
    HI_HANDLE hPump;
    HI_MPI_RMX_PUMP_ATTR_S Attr;

    ret = HI_MPI_RMX_GetPumpDefaultAttr(&Attr);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_RMX_GetPumpDefaultAttr failed.\n");
        goto out0;
    }

    Attr.enPumpType = HI_MPI_RMX_PUMP_TYPE_ALLPASS_PORT;
    Attr.enInPortId = PortId;

    ret = HI_MPI_RMX_AddPump(hRmx, &Attr, &hPump);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_RMX_AddPump failed.\n");
        goto out0;
    }

    return HI_SUCCESS;

out0:
    return ret;
}

static HI_S32 GenNewPat(PMT_COMPACT_PROG *ProgInfo_0, PMT_COMPACT_PROG *ProgInfo_1, HI_U8 *Pat/* in/out */)
{
    HI_U8 *Payload = &Pat[TS_PAYLOAD_OFFSET];
    HI_U32 TotalLen = 0, DescLen = 0, ProgramNumber = 1;
    HI_U32 Crc;
    unsigned long mask, *p;
    HI_U16 PmtPid_0, PmtPid_1;

    pthread_mutex_lock(&g_PidCtl.Lock);

    mask = BIT_MASK(ProgInfo_0->PmtRemapPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(ProgInfo_0->PmtRemapPid);
    if (ProgInfo_0->PmtRemapPid < 0x1fff && *p & mask)
    {
        PmtPid_0 = ProgInfo_0->PmtRemapPid;
    }
    else
    {
        PmtPid_0 = ProgInfo_0->PmtPid;
    }

    mask = BIT_MASK(ProgInfo_1->PmtRemapPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(ProgInfo_1->PmtRemapPid);
    if (ProgInfo_1->PmtRemapPid < 0x1fff && *p & mask)
    {
        PmtPid_1 = ProgInfo_1->PmtRemapPid;
    }
    else
    {
        PmtPid_1 = ProgInfo_1->PmtPid;
    }

    pthread_mutex_unlock(&g_PidCtl.Lock);

    Pat[0] = 0x47; /* sync_byte */
    Pat[1] = 0x40; /* transport_error_indicator, payload_unit_start_indicator, transport_priority, pid part 1 */
    Pat[2] = 0x0; /* pid part 2 */
    Pat[3] = 0x10; /* transport_scrambling_control , adaptation_field_control, continuity_counter */
    Pat[4] = 0x0; /* adaptation_field_data_bytes */

    Payload[0] = PAT_TABLE_ID; /* table_id */

    Payload[3] = 0; /*  transport_stream_id part 1 */
    Payload[4] = 1; /* transport_stream_id part 2 */

    Payload[5] = 0xc1; /* version_number and current_next_indicator */

    Payload[6] = 0; /* section_number */

    Payload[7] = 0; /* last_section_number */

    Payload[8] = 0; /* program_number part 1 */
    Payload[9] = ProgramNumber ++; /* program_number part 2 */
    Payload[10] = (PmtPid_0 >> 8) | 0xe0; /* pid part 1 */
    Payload[11] = PmtPid_0 & 0xff;
    DescLen += 4;

    Payload[12] = 0; /* program_number part 1 */
    Payload[13] = ProgramNumber ++; /* program_number part 2 */
    Payload[14] = (PmtPid_1 >> 8) | 0xe0; /* pid part 1 */
    Payload[15] = PmtPid_1 & 0xff;
    DescLen += 4;

    TotalLen = 8 /* section header len */ + DescLen + 1 /* crc token */;
    Payload[1] = ((TotalLen >> 8) & 0xf) | 0xb0;
    Payload[2] = TotalLen & 0xff;

    Crc = htonl(crc32(Payload, TotalLen - 1 /* crc token  */));
    memcpy(&Payload[TotalLen - 1], &Crc, 4);

    return HI_SUCCESS;
}

static HI_S32 GenNewPmt(HI_U32 ProgramNum, PMT_COMPACT_PROG *ProgInfo, HI_U8 *Pmt/* in/out */)
{
    HI_U8 *Payload = &Pmt[TS_PAYLOAD_OFFSET], *Ptr;
    HI_U32 TotalLen = 0, Len = 0, L1DescLen;
    HI_U32 Crc;
    unsigned long mask, *p;
    HI_U32 PmtPid, VidPid, AudPid;

    pthread_mutex_lock(&g_PidCtl.Lock);

    /* pmt */
    mask = BIT_MASK(ProgInfo->PmtRemapPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(ProgInfo->PmtRemapPid);
    if (ProgInfo->PmtRemapPid < 0x1fff && *p & mask)
    {
        PmtPid = ProgInfo->PmtRemapPid;
    }
    else
    {
        PmtPid = ProgInfo->PmtPid;
    }

    /* vid */
    mask = BIT_MASK(ProgInfo->VElementRemapPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(ProgInfo->VElementRemapPid);
    if (ProgInfo->VElementRemapPid < 0x1fff && *p & mask)
    {
        VidPid = ProgInfo->VElementRemapPid;
    }
    else
    {
        VidPid = ProgInfo->VElementPid;
    }

    /* aud */
    mask = BIT_MASK(ProgInfo->AElementRemapPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(ProgInfo->AElementRemapPid);
    if (ProgInfo->AElementRemapPid < 0x1fff && *p & mask)
    {
        AudPid = ProgInfo->AElementRemapPid;
    }
    else
    {
        AudPid = ProgInfo->AElementPid;
    }

    pthread_mutex_unlock(&g_PidCtl.Lock);

    Pmt[0] = 0x47; /* sync_byte */
    Pmt[1] = (PmtPid >> 8) |0x40; /* transport_error_indicator, payload_unit_start_indicator, transport_priority, pid part 1 */
    Pmt[2] = PmtPid & 0xff; /* pid part 2 */
    Pmt[3] = 0x10; /* transport_scrambling_control , adaptation_field_control, continuity_counter */
    Pmt[4] = 0x0; /* adaptation_field_data_bytes */

    memcpy(Payload, ProgInfo->u8PmtData, ProgInfo->u32PmtLen);

    /* update program number */
    Payload[3] = (ProgramNum >> 8) & 0xff;
    Payload[4] = ProgramNum & 0xff;

    /* update pid if need */
    TotalLen = ((Payload[1] << 8) | Payload[2]) & 0xfff;
    L1DescLen = (((Payload[10] & 0x0f) << 8) | Payload[11]);

    Len = TotalLen - 9 - L1DescLen;
    Ptr = &Payload[8 + 4 + L1DescLen];
    while(Len > 4)
    {
        HI_U32 Pid = (((Ptr[1] & 0x1f) << 8) | Ptr[2]);
        HI_U32 L2DescLen = (((Ptr[3] & 0x0f) << 8) | Ptr[4]);

        if (ProgInfo->VElementPid == Pid)
        {
                    Ptr[1] = (VidPid >> 8) & 0x1f;
                    Ptr[2] = VidPid & 0xff;
        }
        else if (ProgInfo->AElementPid == Pid)
        {
                    Ptr[1] = (AudPid >> 8) & 0x1f;
                    Ptr[2] = AudPid & 0xff;
        }

        Ptr += 5 + L2DescLen;
        Len -= 5 + L2DescLen;
    }

    /* update crc */
    Crc = htonl(crc32(Payload, TotalLen - 1 /* crc token  */));
    memcpy(&Payload[TotalLen - 1], &Crc, 4);

    return HI_SUCCESS;
}

HI_S32 AddRemuxNewTables(HI_VOID *args)
{
    HI_S32 ret = HI_FAILURE;
    PMT_COMPACT_PROG *ProgInfo_0 = ((struct CastTableCtl *)args)->ProgInfo_0;
    PMT_COMPACT_PROG *ProgInfo_1 = ((struct CastTableCtl *)args)->ProgInfo_1;
    HI_HANDLE Handle;
    HI_U8 Pat[188], Pmt_0[188], Pmt_1[188];
    HI_U32 PatCC = 0, Pmt0_CC = 0, Pmt1_CC = 0;

    memset(Pat, 0xff, sizeof(Pat));
    ret = GenNewPat(ProgInfo_0, ProgInfo_1, Pat);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    memset(Pmt_0, 0xff, sizeof(Pmt_0));
    ret = GenNewPmt(1, ProgInfo_0, Pmt_0);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    memset(Pmt_1, 0xff, sizeof(Pmt_1));
    ret = GenNewPmt(2, ProgInfo_1, Pmt_1);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = HI_UNF_DMX_CreateTSBuffer(PORT_ID_2, 0x200000, &Handle);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    while(!((struct CastTableCtl *)args)->Stop)
    {
        HI_UNF_STREAM_BUF_S   StreamBuf;

        /* pat */
        ret = HI_UNF_DMX_GetTSBuffer(Handle, sizeof(Pat), &StreamBuf, 0);
        if (HI_SUCCESS == ret)
        {
            memcpy(StreamBuf.pu8Data, Pat, sizeof(Pat));

            StreamBuf.pu8Data[3] = (StreamBuf.pu8Data[3] & 0xf0) | (PatCC ++ & 0xf);

            HI_UNF_DMX_PutTSBuffer(Handle, StreamBuf.u32Size);
        }

        /* pmt 0 */
        ret = HI_UNF_DMX_GetTSBuffer(Handle, sizeof(Pmt_0), &StreamBuf, 0);
        if (HI_SUCCESS == ret)
        {
            memcpy(StreamBuf.pu8Data, Pmt_0, sizeof(Pmt_0));

            StreamBuf.pu8Data[3] = (StreamBuf.pu8Data[3] & 0xf0) | (Pmt0_CC ++ & 0xf);

            HI_UNF_DMX_PutTSBuffer(Handle, StreamBuf.u32Size);
        }

        /* pmt 1 */
        ret = HI_UNF_DMX_GetTSBuffer(Handle, sizeof(Pmt_1), &StreamBuf, 0);
        if (HI_SUCCESS == ret)
        {
            memcpy(StreamBuf.pu8Data, Pmt_1, sizeof(Pmt_1));

            StreamBuf.pu8Data[3] = (StreamBuf.pu8Data[3] & 0xf0) | (Pmt1_CC ++ & 0xf);

            HI_UNF_DMX_PutTSBuffer(Handle, StreamBuf.u32Size);
        }

        usleep(50);  /* protocol restrict */
    }

    HI_UNF_DMX_DestroyTSBuffer(Handle);
out0:
    return ret;
}

HI_S32 PrepareRemux(PMT_COMPACT_PROG *ProgInfo)
{
    unsigned long mask, *p;

    ProgInfo->PmtRemapPid =0x1fff;
    ProgInfo->VElementRemapPid = 0x1fff;
    ProgInfo->AElementRemapPid = 0x1fff;

    pthread_mutex_lock(&g_PidCtl.Lock);

    /* pmt pid */
    mask = BIT_MASK(ProgInfo->PmtPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(ProgInfo->PmtPid);
    if (*p & mask)
    {
       ProgInfo->PmtRemapPid = find_first_zero_bit(g_PidCtl.PidBitmap, PID_ARRAY_SIZE);

       bitmap_setbit(ProgInfo->PmtRemapPid, g_PidCtl.PidBitmap);
    }
    else
    {
        bitmap_setbit(ProgInfo->PmtPid, g_PidCtl.PidBitmap);
    }

    /* vid pid */
    mask = BIT_MASK(ProgInfo->VElementPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(ProgInfo->VElementPid);
    if (*p & mask)
    {
       ProgInfo->VElementRemapPid = find_first_zero_bit(g_PidCtl.PidBitmap, PID_ARRAY_SIZE);

       bitmap_setbit(ProgInfo->VElementRemapPid, g_PidCtl.PidBitmap);
    }
    else
    {
        bitmap_setbit(ProgInfo->VElementPid, g_PidCtl.PidBitmap);
    }

    /* aud pid */
    mask = BIT_MASK(ProgInfo->AElementPid);
    p = ((unsigned long*)g_PidCtl.PidBitmap) + BIT_WORD(ProgInfo->AElementPid);
    if (*p & mask)
    {
       ProgInfo->AElementRemapPid = find_first_zero_bit(g_PidCtl.PidBitmap, PID_ARRAY_SIZE);

       bitmap_setbit(ProgInfo->AElementRemapPid, g_PidCtl.PidBitmap);
    }
    else
    {
        bitmap_setbit(ProgInfo->AElementPid, g_PidCtl.PidBitmap);
    }

    pthread_mutex_unlock(&g_PidCtl.Lock);

    return HI_SUCCESS;
}

HI_S32 StartRemux(PMT_COMPACT_PROG *ProgInfo_0, PMT_COMPACT_PROG *ProgInfo_1)
{
    HI_S32 ret = HI_FAILURE;
    HI_MPI_RMX_ATTR_S attr ;

    ret = HI_MPI_RMX_GetDefaultAttr(&attr);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_RMX_GetDefaultAttr failed.\n");
        goto out0;
    }

    ret = HI_MPI_RMX_Create(&attr, &g_hRmx);
    if (HI_SUCCESS != ret)
    {
        printf("create remux failed.\n");
        goto out0;
    }

    ret = AddRemuxAllpassPort(g_hRmx, PORT_ID_2);
    if (HI_SUCCESS != ret)
    {
        printf("AddRemuxAllpassPort failed.\n");
        goto out1;
    }

    ret = AddRemuxPidChannel(g_hRmx, PORT_ID_0, ProgInfo_0->VElementPid, ProgInfo_0->VElementRemapPid);
    if (HI_SUCCESS != ret)
    {
        printf("AddRemuxPidChannel failed.\n");
        goto out1;
    }

    ret = AddRemuxPidChannel(g_hRmx, PORT_ID_0, ProgInfo_0->AElementPid, ProgInfo_0->AElementRemapPid);
    if (HI_SUCCESS != ret)
    {
        printf("AddRemuxPidChannel failed.\n");
        goto out1;
    }

    ret = AddRemuxPidChannel(g_hRmx, PORT_ID_1, ProgInfo_1->VElementPid, ProgInfo_1->VElementRemapPid);
    if (HI_SUCCESS != ret)
    {
        printf("AddRemuxPidChannel ailed.\n");
        goto out1;
    }

    ret = AddRemuxPidChannel(g_hRmx, PORT_ID_1, ProgInfo_1->AElementPid, ProgInfo_1->AElementRemapPid);
    if (HI_SUCCESS != ret)
    {
        printf("AddRemuxPidChannel failed.\n");
        goto out1;
    }

    g_CastTableCtl.ProgInfo_0 = ProgInfo_0;
    g_CastTableCtl.ProgInfo_1 = ProgInfo_1;
    g_CastTableCtl.Stop = HI_FALSE;
    pthread_create(&g_CastTableThd, HI_NULL, (HI_VOID *)AddRemuxNewTables, (HI_VOID *)&g_CastTableCtl);

    pthread_mutex_lock(&g_TsThreadCtl_0.Lock);
    rewind(g_TsThreadCtl_0.TsFileHandle);
    HI_UNF_DMX_ResetTSBuffer(g_TsThreadCtl_0.TsBufHandle);
    pthread_mutex_unlock(&g_TsThreadCtl_0.Lock);

    pthread_mutex_lock(&g_TsThreadCtl_1.Lock);
    rewind(g_TsThreadCtl_1.TsFileHandle);
    HI_UNF_DMX_ResetTSBuffer(g_TsThreadCtl_1.TsBufHandle);
    pthread_mutex_unlock(&g_TsThreadCtl_1.Lock);

    ret = HI_MPI_RMX_Start(g_hRmx);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_RMX_Start failed.\n");
        goto out1;
    }

    return HI_SUCCESS;

out1:
    HI_MPI_RMX_Destroy(g_hRmx);
out0:
    return ret;
}

HI_VOID StopRemux(HI_VOID)
{
    HI_MPI_RMX_Destroy(g_hRmx);
}

HI_VOID* SaveRecDataThread(HI_VOID *arg)
{
    HI_S32      ret;
    FILE *RecFile = HI_NULL;
    struct TsRecCtl *TsRecCtl = (struct TsRecCtl *)arg;
    
    RecFile = fopen(TsFileName, "w");
    if (!RecFile)
    {
        perror("fopen error");
        return HI_NULL;
    }

    while (!TsRecCtl->Stop)
    {
        HI_UNF_DMX_REC_DATA_S RecData;

        ret = HI_UNF_DMX_AcquireRecData(TsRecCtl->RecHandle, &RecData, 100);
        if (HI_SUCCESS != ret)
        {
            if (HI_ERR_DMX_TIMEOUT == ret)
            {
                continue;
            }

            if (HI_ERR_DMX_NOAVAILABLE_DATA == ret)
            {
                continue;
            }

            assert(0);
        }
        
        if (RecData.u32Len != fwrite(RecData.pDataAddr, 1, RecData.u32Len, RecFile))
        {
            assert(0);
        }

        HI_UNF_DMX_ReleaseRecData(TsRecCtl->RecHandle, &RecData);
        
    }

    fclose(RecFile);

    return HI_NULL;
}

HI_S32 DmxStartRecord(HI_U32 DmxId)
{
    HI_S32 ret = HI_FAILURE;
    HI_UNF_DMX_REC_ATTR_S   RecAttr = {0};

    RecAttr.u32DmxId        = DmxId;
    RecAttr.u32RecBufSize   = 4 * 1024 * 1024;
    RecAttr.enRecType       = HI_UNF_DMX_REC_TYPE_ALL_PID;
    RecAttr.bDescramed      = HI_TRUE;
    RecAttr.enIndexType     = HI_UNF_DMX_REC_INDEX_TYPE_NONE;

    ret = HI_UNF_DMX_AttachTSPort(DmxId, HI_MPI_DMX_PORT_RMX_0);
    if (HI_SUCCESS != ret)
    {
        printf("HI_UNF_DMX_AttachTSPort failed 0x%x.\n", ret);
        goto out0;
    }

    ret = HI_UNF_DMX_CreateRecChn(&RecAttr, &g_RecHandle);
    if (HI_SUCCESS != ret)
    {
        printf("HI_UNF_DMX_CreateRecChn failed 0x%x\n", ret);
        goto out1;
    }

    printf("\nStart record remux stream[%s] now...\n\n", TsFileName);

    ret = HI_UNF_DMX_StartRecChn(g_RecHandle);
    if (HI_SUCCESS != ret)
    {
        printf("HI_UNF_DMX_StartRecChn failed 0x%x\n", ret);

        goto out2;
    }

    g_RecThreadCtl.RecHandle = g_RecHandle;
    g_RecThreadCtl.Stop = HI_FALSE;
    pthread_create(&g_RecThd, HI_NULL, (HI_VOID *)SaveRecDataThread, (HI_VOID*)&g_RecThreadCtl);

    return HI_SUCCESS;
    
out2:
    HI_UNF_DMX_DestroyRecChn(g_RecHandle);
out1:
    HI_UNF_DMX_DetachTSPort(DmxId);
out0:    
    return ret;
}

HI_VOID DmxStopRecord(HI_U32 DmxId)
{
    g_RecThreadCtl.Stop = HI_TRUE;
    pthread_join(g_RecThd, HI_NULL);

    HI_UNF_DMX_StopRecChn(g_RecHandle);
    HI_UNF_DMX_DelAllRecPid(g_RecHandle);
    HI_UNF_DMX_DestroyRecChn(g_RecHandle);

    HI_UNF_DMX_DetachTSPort(DmxId);
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32 ret = HI_FAILURE;
    HI_CHAR InputCmd[32];
    HI_U32 ProgNum_0, ProgNum_1;

    if (3 == argc)
    {
        g_TsFile_0 = fopen(argv[1], "rb");
        g_TsFile_1 = fopen(argv[2], "rb");

        if (!g_TsFile_0 || !g_TsFile_1)
        {
            printf("open file error!\n");
            return -1;
        }
    }
    else
    {
        printf("Usage: sample_remux file1 file2\n");

        return 0;
    }

    InitCrcTable();

    InitPidCtl();

    ret = HI_SYS_Init();
    if (HI_SUCCESS != ret)
    {
        printf("call HI_SYS_Init failed.\n");
        goto err0;
    }

    ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != ret)
    {
         printf("call HI_UNF_DMX_Init failed.\n");
         goto err1;
    }

    ret = HI_UNF_DMX_AttachTSPort(DEMUX_ID_0, PORT_ID_0);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_AttachTSPort failed.\n");
        goto err2;
    }

    ret = HI_UNF_DMX_CreateTSBuffer(PORT_ID_0, 0x200000, &g_TsBuf_0);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto err3;
    }

    g_TsThreadCtl_0.TsBufHandle = g_TsBuf_0;
    g_TsThreadCtl_0.TsFileHandle = g_TsFile_0;
    g_TsThreadCtl_0.Stop = HI_FALSE;
    pthread_mutex_init(&g_TsThreadCtl_0.Lock, HI_NULL);
    pthread_create(&g_TsThd_0, HI_NULL, (HI_VOID *)TsThread, (HI_VOID*)&g_TsThreadCtl_0);

    ret = HI_UNF_DMX_AttachTSPort(DEMUX_ID_1, PORT_ID_1);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_AttachTSPort failed.\n");
        goto err4;
    }

    ret = HI_UNF_DMX_CreateTSBuffer(PORT_ID_1, 0x200000, &g_TsBuf_1);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto err5;
    }

    g_TsThreadCtl_1.TsBufHandle = g_TsBuf_1;
    g_TsThreadCtl_1.TsFileHandle = g_TsFile_1;
    g_TsThreadCtl_1.Stop = HI_FALSE;
    pthread_mutex_init(&g_TsThreadCtl_1.Lock, HI_NULL);
    pthread_create(&g_TsThd_1, HI_NULL, (HI_VOID *)TsThread, (HI_VOID*)&g_TsThreadCtl_1);

    HIADP_Search_Init();

    ret = HIADP_Search_GetAllPmt(DEMUX_ID_0, &g_ProgTbl_0);
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Search_GetAllPmt failed\n");
        goto err6;
    }

    printf("\nPlease input the number of program to first remux pump: ");

    SAMPLE_GET_INPUTCMD(InputCmd);

    ProgNum_0 = (atoi(InputCmd) - 1) % g_ProgTbl_0->prog_num;

    ret = HIADP_Search_GetAllPmt(DEMUX_ID_1, &g_ProgTbl_1);
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Search_GetAllPmt failed\n");
        goto err6;
    }

    printf("\nPlease input the number of program to second remux pump: ");

    SAMPLE_GET_INPUTCMD(InputCmd);

    ProgNum_1 = (atoi(InputCmd) - 1) % g_ProgTbl_1->prog_num;

    PrepareRemux(g_ProgTbl_0->proginfo + ProgNum_0);

    PrepareRemux(g_ProgTbl_1->proginfo + ProgNum_1);

    StartRemux(g_ProgTbl_0->proginfo + ProgNum_0, g_ProgTbl_1->proginfo + ProgNum_1);

    DmxStartRecord(DEMUX_ID_2);

    while(1)
    {
        printf("please input the q to quit!\n");

        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            ret = HI_SUCCESS;
            break;
        }
    }

    DmxStopRecord(DEMUX_ID_2);

    StopRemux();

    HIADP_Search_FreeAllPmt(g_ProgTbl_0);
    HIADP_Search_FreeAllPmt(g_ProgTbl_1);
    HIADP_Search_DeInit();

    g_TsThreadCtl_0.Stop = HI_TRUE;
    pthread_join(g_TsThd_0, HI_NULL);

    g_TsThreadCtl_1.Stop = HI_TRUE;
    pthread_join(g_TsThd_1, HI_NULL);

    g_CastTableCtl.Stop = HI_TRUE;
    pthread_join(g_CastTableThd, HI_NULL);

err6:
    HI_UNF_DMX_DestroyTSBuffer(g_TsBuf_1);
err5:
    HI_UNF_DMX_DetachTSPort(DEMUX_ID_1);
err4:
    HI_UNF_DMX_DestroyTSBuffer(g_TsBuf_0);
err3:
    HI_UNF_DMX_DetachTSPort(DEMUX_ID_0);
err2:
    HI_UNF_DMX_DeInit();
err1:
    HI_SYS_DeInit();
err0:
    fclose(g_TsFile_0);
    fclose(g_TsFile_1);

    return ret;
}


