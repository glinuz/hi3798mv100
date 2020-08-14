//#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "bitstream.h"
#include "compress.h"

/* marker */
#define  PIX_MARKER  0
#define  DIF_MARKER  1

/* line type */
#define  LINE_TYPE_HYBRID  0
#define  LINE_TYPE_VLC     1
#define  LINE_TYPE_FLC     2
#define  LINE_TYPE_INVALID 3

//extern FILE *fpLog;
//extern FILE *fpOut;
extern int  g_DiffCnt[512];

#define ABS(val)  (((val)<0)? (0 - (val)): (val))

static int VectEncTable[][4] = {
    {99,99,0x000, 2},  //00 escape
    { 0, 0,0x002, 3},  //010        
    { 1, 0,0x006, 4},  //0110       
    {-1, 0,0x007, 4},  //0111       
    { 2, 0,0x010, 5},  //10000      
    {-2, 0,0x011, 5},  //10001      
    {-3, 0,0x024, 6},  //100100     
    { 0,-1,0x025, 6},  //100101     
    { 0, 1,0x026, 6},  //100110     
    { 3, 0,0x027, 6},  //100111     
    {-1, 1,0x028, 6},  //101000     
    { 1,-1,0x029, 6},  //101001     
    { 0, 2,0x054, 7},  //1010100    
    { 0,-2,0x055, 7},  //1010101    
    {-2, 1,0x056, 7},  //1010110    
    {-1, 2,0x057, 7},  //1010111    
    { 4, 0,0x058, 7},  //1011000    
    { 2, 1,0x059, 7},  //1011001    
    {-2,-1,0x05A, 7},  //1011010    
    {-4, 0,0x05B, 7},  //1011011    
    { 2,-1,0x05C, 7},  //1011100    
    {-1,-1,0x05D, 7},  //1011101    
    { 1, 1,0x05E, 7},  //1011110    
    {-3, 1,0x0BE, 8},  //10111110   
    {-5, 0,0x0BF, 8},  //10111111   
    { 5, 0,0x0C0, 8},  //11000000   
    { 0, 3,0x0C1, 8},  //11000001   
    {-3,-1,0x0C2, 8},  //11000010   
    { 2,-2,0x0C3, 8},  //11000011   
    { 3, 1,0x0C4, 8},  //11000100   
    { 2, 2,0x0C5, 8},  //11000101   
    {-2, 2,0x0C6, 8},  //11000110   
    {-2,-2,0x0C7, 8},  //11000111   
    {-1,-2,0x0C8, 8},  //11001000   
    { 1, 2,0x0C9, 8},  //11001001   
    { 3,-1,0x0CA, 8},  //11001010   
    { 1,-2,0x0CB, 8},  //11001011   
    { 1, 4,0x198, 9},  //110011000  
    {-1,-4,0x199, 9},  //110011001  
    {-3,-3,0x19A, 9},  //110011010  
    {-4, 2,0x19B, 9},  //110011011  
    { 1,-4,0x19C, 9},  //110011100  
    { 0, 4,0x19D, 9},  //110011101  
    {-5,-1,0x19E, 9},  //110011110  
    { 5,-1,0x19F, 9},  //110011111  
    { 3,-3,0x1A0, 9},  //110100000  
    { 3, 3,0x1A1, 9},  //110100001  
    { 5, 1,0x1A2, 9},  //110100010  
    {-5, 1,0x1A3, 9},  //110100011  
    {-2,-3,0x1A4, 9},  //110100100  
    {-3, 3,0x1A5, 9},  //110100101  
    { 2, 3,0x1A6, 9},  //110100110  
    { 2,-3,0x1A7, 9},  //110100111  
    { 7, 0,0x1A8, 9},  //110101000  
    { 0,-4,0x1A9, 9},  //110101001  
    {-7, 0,0x1AA, 9},  //110101010  
    {-4, 1,0x1AB, 9},  //110101011  
    {-2, 3,0x1AC, 9},  //110101100  
    {-4,-1,0x1AD, 9},  //110101101  
    { 4,-1,0x1AE, 9},  //110101110  
    {-3,-2,0x1AF, 9},  //110101111  
    { 6, 0,0x1B0, 9},  //110110000  
    {-6, 0,0x1B1, 9},  //110110001  
    { 4, 1,0x1B2, 9},  //110110010  
    { 3, 2,0x1B3, 9},  //110110011  
    {-1,-3,0x1B4, 9},  //110110100  
    {-3, 2,0x1B5, 9},  //110110101  
    { 3,-2,0x1B6, 9},  //110110110  
    { 1, 3,0x1B7, 9},  //110110111  
    { 1,-3,0x1B8, 9},  //110111000  
    {-1, 3,0x1B9, 9},  //110111001  
    {-4,-2,0x1BA, 9},  //110111010  
    { 4, 2,0x1BB, 9},  //110111011  
    {-1, 4,0x1BC, 9},  //110111100  
    {-2, 4,0x1BD, 9},  //110111101  
    { 4,-2,0x1BE, 9},  //110111110  
    { 0,-3,0x1BF, 9},  //110111111  
    {-7,-1,0x380,10},  //1110000000 
    {-4, 5,0x381,10},  //1110000001 
    { 6, 2,0x382,10},  //1110000010 
    { 0, 6,0x383,10},  //1110000011 
    {-4,-5,0x384,10},  //1110000100 
    {-3, 6,0x385,10},  //1110000101 
    { 6, 3,0x386,10},  //1110000110 
    { 7,-1,0x387,10},  //1110000111 
    { 4, 6,0x388,10},  //1110001000 
    { 7, 1,0x389,10},  //1110001001 
    {-2,-6,0x38A,10},  //1110001010 
    {-1, 6,0x38B,10},  //1110001011 
    { 2,-5,0x38C,10},  //1110001100 
    {-5, 3,0x38D,10},  //1110001101 
    { 3, 5,0x38E,10},  //1110001110 
    { 6,-1,0x38F,10},  //1110001111 
    {-6,-2,0x390,10},  //1110010000 
    {-6, 2,0x391,10},  //1110010001 
    { 1, 7,0x392,10},  //1110010010 
    { 3, 6,0x393,10},  //1110010011 
    {-3,-6,0x394,10},  //1110010100 
    { 3,-5,0x395,10},  //1110010101 
    { 0,-7,0x396,10},  //1110010110 
    { 1,-5,0x397,10},  //1110010111 
    {-6,-3,0x398,10},  //1110011000 
    {-7,-2,0x399,10},  //1110011001 
    {-3,-5,0x39A,10},  //1110011010 
    { 5, 5,0x39B,10},  //1110011011 
    { 6, 1,0x39C,10},  //1110011100 
    { 3,-4,0x39D,10},  //1110011101 
    {-3, 5,0x39E,10},  //1110011110 
    {-2,-5,0x39F,10},  //1110011111 
    {-4,-4,0x3A0,10},  //1110100000 
    {-3,-4,0x3A1,10},  //1110100001 
    { 4, 4,0x3A2,10},  //1110100010 
    {-2, 5,0x3A3,10},  //1110100011 
    {-1,-5,0x3A4,10},  //1110100100 
    {-5,-3,0x3A5,10},  //1110100101 
    {-6,-1,0x3A6,10},  //1110100110 
    {-6, 1,0x3A7,10},  //1110100111 
    {-6, 3,0x3A8,10},  //1110101000 
    {-5, 5,0x3A9,10},  //1110101001 
    {-5,-4,0x3AA,10},  //1110101010 
    { 0,-6,0x3AB,10},  //1110101011 
    { 5, 3,0x3AC,10},  //1110101100 
    { 0,-5,0x3AD,10},  //1110101101 
    {-7, 1,0x3AE,10},  //1110101110 
    { 8, 0,0x3AF,10},  //1110101111 
    {-8, 0,0x3B0,10},  //1110110000 
    { 3, 4,0x3B1,10},  //1110110001 
    { 5,-2,0x3B2,10},  //1110110010 
    { 4,-3,0x3B3,10},  //1110110011 
    { 1, 5,0x3B4,10},  //1110110100 
    {-1, 5,0x3B5,10},  //1110110101 
    { 5, 2,0x3B6,10},  //1110110110 
    { 2, 5,0x3B7,10},  //1110110111 
    { 4, 5,0x3B8,10},  //1110111000 
    { 5,-3,0x3B9,10},  //1110111001 
    {-5, 4,0x3BA,10},  //1110111010 
    {-2, 6,0x3BB,10},  //1110111011 
    { 1,-6,0x3BC,10},  //1110111100 
    {-3, 4,0x3BD,10},  //1110111101 
    { 2,-4,0x3BE,10},  //1110111110 
    {-4,-3,0x3BF,10},  //1110111111 
    {-5,-2,0x3C0,10},  //1111000000 
    { 0, 5,0x3C1,10},  //1111000001 
    {-5, 2,0x3C2,10},  //1111000010 
    {-1,-6,0x3C3,10},  //1111000011 
    { 4,-4,0x3C4,10},  //1111000100 
    { 1, 6,0x3C5,10},  //1111000101 
    { 5, 4,0x3C6,10},  //1111000110 
    { 6,-2,0x3C7,10},  //1111000111 
    {-4, 3,0x3C8,10},  //1111001000 
    { 4, 3,0x3C9,10},  //1111001001 
    {-4, 4,0x3CA,10},  //1111001010 
    {-2,-4,0x3CB,10},  //1111001011 
    { 2, 4,0x3CC,10},  //1111001100 
    { 2, 6,0x3CD,10},  //1111001101 
    { 0, 7,0x3CE,10},  //1111001110 
    {-8, 2,0x79E,11},  //11110011110
    { 7, 6,0x79F,11},  //11110011111
    { 8,-1,0x7A0,11},  //11110100000
    { 5, 8,0x7A1,11},  //11110100001
    {-5,-8,0x7A2,11},  //11110100010
    { 8, 2,0x7A3,11},  //11110100011
    { 8, 3,0x7A4,11},  //11110100100
    { 8, 4,0x7A5,11},  //11110100101
    {-5,-7,0x7A6,11},  //11110100110
    {-3,-8,0x7A7,11},  //11110100111
    {-7,-6,0x7A8,11},  //11110101000
    { 4, 8,0x7A9,11},  //11110101001
    { 5,-6,0x7AA,11},  //11110101010
    { 1,-8,0x7AB,11},  //11110101011
    {-6,-7,0x7AC,11},  //11110101100
    {-6,-6,0x7AD,11},  //11110101101
    { 4,-6,0x7AE,11},  //11110101110
    { 3,-7,0x7AF,11},  //11110101111
    { 6, 7,0x7B0,11},  //11110110000
    { 0,-8,0x7B1,11},  //11110110001
    { 1, 8,0x7B2,11},  //11110110010
    { 2,-7,0x7B3,11},  //11110110011
    { 0, 8,0x7B4,11},  //11110110100
    {-5,-6,0x7B5,11},  //11110110101
    { 6, 5,0x7B6,11},  //11110110110
    { 6, 6,0x7B7,11},  //11110110111
    { 7,-2,0x7B8,11},  //11110111000
    {-6,-5,0x7B9,11},  //11110111001
    { 4, 7,0x7BA,11},  //11110111010
    { 5,-5,0x7BB,11},  //11110111011
    {-4, 7,0x7BC,11},  //11110111100
    {-3,-7,0x7BD,11},  //11110111101
    { 7, 4,0x7BE,11},  //11110111110
    { 7, 5,0x7BF,11},  //11110111111
    {-8,-3,0x7C0,11},  //11111000000
    { 8, 1,0x7C1,11},  //11111000001
    {-7,-4,0x7C2,11},  //11111000010
    {-3, 7,0x7C3,11},  //11111000011
    {-8,-2,0x7C4,11},  //11111000100
    {-5, 6,0x7C5,11},  //11111000101
    { 3, 8,0x7C6,11},  //11111000110
    {-4,-7,0x7C7,11},  //11111000111
    {-2, 8,0x7C8,11},  //11111001000
    {-7, 4,0x7C9,11},  //11111001001
    { 5, 7,0x7CA,11},  //11111001010
    { 6,-4,0x7CB,11},  //11111001011
    {-6, 5,0x7CC,11},  //11111001100
    { 2, 8,0x7CD,11},  //11111001101
    {-1, 8,0x7CE,11},  //11111001110
    { 6, 8,0x7CF,11},  //11111001111
    { 7,-3,0x7D0,11},  //11111010000
    { 8, 5,0x7D1,11},  //11111010001
    { 7, 7,0x7D2,11},  //11111010010
    { 8,-3,0x7D3,11},  //11111010011
    { 8, 6,0x7D4,11},  //11111010100
    { 4,-7,0x7D5,11},  //11111010101
    {-7,-5,0x7D6,11},  //11111010110
    {-3, 8,0x7D7,11},  //11111010111
    { 8,-2,0x7D8,11},  //11111011000
    { 3,-8,0x7D9,11},  //11111011001
    { 6,-5,0x7DA,11},  //11111011010
    {-8, 3,0x7DB,11},  //11111011011
    { 7,-4,0x7DC,11},  //11111011100
    {-8,-6,0x7DD,11},  //11111011101
    {-6,-8,0x7DE,11},  //11111011110
    {-7,-7,0x7DF,11},  //11111011111
    { 2,-8,0x7E0,11},  //11111100000
    {-2,-8,0x7E1,11},  //11111100001
    {-4,-8,0x7E2,11},  //11111100010
    {-8,-4,0x7E3,11},  //11111100011
    {-1, 7,0x7E4,11},  //11111100100
    {-8,-1,0x7E5,11},  //11111100101
    { 7, 2,0x7E6,11},  //11111100110
    { 7, 3,0x7E7,11},  //11111100111
    { 2,-6,0x7E8,11},  //11111101000
    { 1,-7,0x7E9,11},  //11111101001
    { 5,-4,0x7EA,11},  //11111101010
    {-4,-6,0x7EB,11},  //11111101011
    {-7, 2,0x7EC,11},  //11111101100
    {-7, 3,0x7ED,11},  //11111101101
    {-4, 6,0x7EE,11},  //11111101110
    {-6,-4,0x7EF,11},  //11111101111
    {-8, 1,0x7F0,11},  //11111110000
    {-6, 4,0x7F1,11},  //11111110001
    { 3, 7,0x7F2,11},  //11111110010
    { 4,-5,0x7F3,11},  //11111110011
    { 5, 6,0x7F4,11},  //11111110100
    { 6,-3,0x7F5,11},  //11111110101
    {-2,-7,0x7F6,11},  //11111110110
    {-5,-5,0x7F7,11},  //11111110111
    {-7,-3,0x7F8,11},  //11111111000
    {-2, 7,0x7F9,11},  //11111111001
    {-8,-5,0x7FA,11},  //11111111010
    { 2, 7,0x7FB,11},  //11111111011
    { 3,-6,0x7FC,11},  //11111111100
    { 6, 4,0x7FD,11},  //11111111101
    {-1,-7,0x7FE,11},  //11111111110
    {-1,-8,0x7FF,11}   //11111111111
};
#define MAX_CODE_LEN     11




/************************************************************************/
/*   put bits                                                           */
/************************************************************************/

typedef struct {
    unsigned int data;
    int  valid_bits;
    unsigned int *start_addr;
    int  max_size;
    unsigned int *write_addr;
} PBS;


void PBS_InitPbs(PBS *pPbs, unsigned int *pBuf, int BufSize )
{
    memset(pPbs, 0, sizeof(PBS));
    pPbs->start_addr = pBuf;
    pPbs->write_addr = pBuf;
    pPbs->max_size   = BufSize;
    
    return;
}

void PBS_PutBits(PBS *pPbs, unsigned int data, int BitsNum)
{
    unsigned int tmp;
    int BitsLeft;
    
    if (pPbs->valid_bits + BitsNum < 32)
    {
        pPbs->data |= (data << (32 - BitsNum - pPbs->valid_bits));
        pPbs->valid_bits += BitsNum;
    }
    else
    {
        /* 1. save the rest bits into 'tmp' */
        BitsLeft = pPbs->valid_bits + BitsNum - 32;
        tmp = data & ((1 << BitsLeft) - 1);
        
        /* 2. fill 32 bit data buf, then write it out */
        pPbs->data |= (data >> BitsLeft);
        *pPbs->write_addr++ = ENDIAN32(pPbs->data);
        
        /* 3. record the rest bits from 'tmp' */
        pPbs->data = (tmp << (32 - BitsLeft));
        pPbs->valid_bits = BitsLeft;
    }
}

void PBS_FlushBits(PBS *pPbs)
{
    if (pPbs->valid_bits > 0)
    {
        *pPbs->write_addr++ = ENDIAN32(pPbs->data);
        pPbs->data = 0;
        pPbs->valid_bits = 0;
    }
}

int PBS_GetDataSize(PBS *pPbs)
{
    return (pPbs->write_addr - pPbs->start_addr) * sizeof(unsigned int);
}



/************************************************************************/
/*  compressor / decompressor                                           */
/************************************************************************/
int  Enctab[2*CARE_RANGE+1][2*CARE_RANGE+1][2];   // (code,code_len)
//int  Singltab[2*CARE_RANGE+1][2];
int  EscCode[2];
int  Dectab[65536][3];  // (d0, d1, bit_len)

int compress_sgmt(unsigned char *p0, unsigned char *p1, int step, int SymbNum, int sgmt_size, unsigned int *multiple, unsigned short *pSgmtHead, unsigned char *pDst);

/* 初始化，主要是整理码表，使之便于编码/解码 */
void InitCompressor()
{
    int CodeNum;
    int i, j;
    int d0, d1;
    int Code, CodeLen, LeftBits;
    int Start, End;

    CodeNum = sizeof(VectEncTable) / (4*sizeof(int));

    /* 整理编码表，使之呈距阵排列，便于用查表编码 */
    memset(Enctab, 0, sizeof(Enctab));

    for (i = 0; i < CodeNum; i++)
    {
        d0 = VectEncTable[i][0];
        d1 = VectEncTable[i][1];
        Code    = VectEncTable[i][2];   // code
        CodeLen = VectEncTable[i][3];   // code length

        if (d0 == 99 && d1 == 99)
        {
            /* find escape code */
            EscCode[0] = Code;
            EscCode[1] = CodeLen;
        }
        else if (d0 != 99 && d1 == 99)
        {
            if( (d0 + CARE_RANGE) <(2*CARE_RANGE+1))
            {
//                Singltab[d0 + CARE_RANGE][0] = Code;
//                Singltab[d0 + CARE_RANGE][1] = CodeLen;
            }
        }
        else
        {
            if( (d0 + CARE_RANGE) <(2*CARE_RANGE+1))
            {
                Enctab[d0 + CARE_RANGE][d1 + CARE_RANGE][0] = Code;
                Enctab[d0 + CARE_RANGE][d1 + CARE_RANGE][1] = CodeLen;
            }
        }        
    }
    
    /* 整理解码表，使之可用查表法解变长码 */
    memset(Dectab, 0, sizeof(Dectab));

    for (i = 0; i < CodeNum; i++)
    {
        d0      = VectEncTable[i][0];
        d1      = VectEncTable[i][1];
        Code    = VectEncTable[i][2];   // code
        CodeLen = VectEncTable[i][3];   // code length

        LeftBits = MAX_CODE_LEN - CodeLen;  
        Start = Code << LeftBits;
        End   = Start + (1 << LeftBits) - 1;

        for (j = Start; j <= End; j++)
        {
            Dectab[j][0] = d0;
            Dectab[j][1] = d1;
            Dectab[j][2] = CodeLen;
        }
    }
    
    return;
}

int g_esc_cnt = 0;
SGMT_TYPE_E  GetSegmentType(unsigned char *p0, unsigned char *p1, int step, int SymbNum);

/* 压缩一帧 */
#if 0
int compress(unsigned char *pFrame, int Width, int Height, unsigned int *multiple, int sgmt_size, unsigned char *pData )
{
    int SymbNum, y;
    int total_size = 0;
    unsigned short *pSgmtHead;
    unsigned char *p0, *p1, *pDst;
    int i,j;
    int d0, d1;
    int size = 0;
	PBS pbs;
	int u32Wth;
    int u32LoopNum;
	int burstsize=0;
	SGMT_TYPE_E  eSgmtType;
    unsigned short  sgmt_len_idx, compressed_data_len, sgmt_type,  use_defalt_huff_table;
	int SgmtNum;
   unsigned int U32Len = 0;
   int compress_en;
   int stride_luma=768, stride_chrome=768;

    /* 生成信息区，信息区暂放置于帧头，用于描述各压缩段的起始地址/长度/模式等可能的信息 */
    pSgmtHead = (unsigned short *)(pData);

    /* 真正开始存放压缩数据的地址 */
    pDst      =  pData + Height * sizeof(unsigned short);
    total_size = (int)(pDst - pData);
    memset(pSgmtHead, 0, (int)(pDst - (unsigned char*)pSgmtHead));

    /* 分亮度/色度逐行压缩 */
    SymbNum = Width / 2;       /* 无论亮度还是色度，每行都是Width/2个符号 */

    /* 1. 压缩 */
    for (y = 0; y < Height; y++)
    {

        p0 = pFrame + y*Width;
        p1 = p0 + 1;
	size=0;

        u32Wth = SymbNum*2;
        
	PBS_InitPbs(&pbs, (int*)pDst, SymbNum * 2);  /* 2 pass已经保证了不膨胀 */

	/*判断是64/128/265一段*/
	if(sgmt_size==128)
	{
		if(u32Wth%128==0)
			u32LoopNum=u32Wth/128;
		else
			u32LoopNum=u32Wth/128+1;
	}
	else if(sgmt_size==64)
	{
		if(u32Wth%64==0)
			u32LoopNum=u32Wth/64;
		else
			u32LoopNum=u32Wth/64+1;
	}
	else if(sgmt_size==256)
	{
		if(u32Wth%256==0)
			u32LoopNum=u32Wth/256;
		else
			u32LoopNum=u32Wth/256+1;
	}

	for(j=0;j<u32LoopNum;j++)
		{
		burstsize=0;               //每段开始清burstsize

		if(sgmt_size==128)
		{
			if(u32Wth > 128)
			{
				if(j<(u32LoopNum - 1))
				{
					SgmtNum = 64;
				}
				else
				{
					SgmtNum = (u32Wth - (u32LoopNum-1)*128)/2;
				}
			}
		}
		if(sgmt_size==64)
		{
			if(u32Wth > 64)
			{
				if(j<(u32LoopNum - 1))
				{
					SgmtNum = 32;
				}
				else
				{
					SgmtNum = (u32Wth - (u32LoopNum-1)*64)/2;
				}
			}
		}
		if(sgmt_size==256)
		{
			if(u32Wth > 256)
			{
				if(j<(u32LoopNum - 1))
				{
					SgmtNum = 128;
				}
				else
				{
					SgmtNum = (u32Wth - (u32LoopNum-1)*256)/2;
				}
			}
		}

            eSgmtType = GetSegmentType(p0, p1, 2, SgmtNum);
                    
            //set compressor type
            PBS_PutBits(&pbs, eSgmtType, 1);
            
		burstsize++;

            if (eSgmtType == SGMT_TYPE_FLC)
            {
                for (i = 0; i < SgmtNum; i++)
                {
                    PBS_PutBits(&pbs, p0[0], 8);
                    PBS_PutBits(&pbs, p1[0], 8);

			burstsize+=16;

                    p0 += 2;
                    p1 += 2;
                }
            }
            else
            {
                /* 第一个符号: 保存原值 */
				
                PBS_PutBits(&pbs, p0[0], 8);
                PBS_PutBits(&pbs, p1[0], 8);
                    
		burstsize+=16;

                p0 += 2;
                p1 += 2;

                /* 从第二个符号开始: 保存偏差 or 原始值 */
                for (i = 1; i < SgmtNum; i++)
                {

			d0 = (int)p0[0] - (int)p0[-1];
			d1 = (int)p1[0] - (int)p1[-1];
                    
                    if ((d0 >= -CARE_RANGE && d0 <= CARE_RANGE) && (d1 >= -CARE_RANGE && d1 <= CARE_RANGE) &&   /* d0, d1在压缩编码的范围内 */
                        (Enctab[d0+CARE_RANGE][d1+CARE_RANGE][1] != 0))  /* 码表中确实有这个码 */ 
                    {
                        PBS_PutBits(&pbs, Enctab[d0+CARE_RANGE][d1+CARE_RANGE][0], Enctab[d0+CARE_RANGE][d1+CARE_RANGE][1]);
                    
			burstsize = burstsize + (UINT32)Enctab[d0+CARE_RANGE][d1+CARE_RANGE][1];
                    
                    }
                    else
                    {
                        PBS_PutBits(&pbs, EscCode[0], EscCode[1]);
                        
                        PBS_PutBits(&pbs, p0[0], 8);
                        PBS_PutBits(&pbs, p1[0], 8);

			burstsize=burstsize+18;
                    }

                    p0 += 2;
                    p1 += 2;
                }
		}

		/*最后一段不够64/128/256的话，这一段的burstsize就不统计了*/
		if((j!=(u32LoopNum-1)) || ((sgmt_size==128) && (u32Wth%128==0)) || ((sgmt_size==64) && (u32Wth%64==0)) || ((sgmt_size==256)&&(u32Wth%256==0)))
		{
			burstsize=(burstsize+7)/8;     //换成字节数

			if(burstsize%16==0)            //换成16字节的整数倍数
			{
				burstsize=burstsize/16;
			}
			else
			{
				burstsize=burstsize/16+1;
			}
			
			/*求出每段16字节的整数倍数的个数*/
			if(sgmt_size==128)
			{
				if(eSgmtType==0)
				{
					multiple[8]++;        
				}
				else 
				{
					if(burstsize==1)
						multiple[0]++;
					else if(burstsize==2)
						multiple[1]++;
					else if(burstsize==3)
						multiple[2]++;
					else if(burstsize==4)
						multiple[3]++;
					else if(burstsize==5)
						multiple[4]++;
					else if(burstsize==6)
						multiple[5]++;
					else if(burstsize==7)
						multiple[6]++;
					else if(burstsize==8)
						multiple[7]++;
					//else
					//	HI_PRINT("burstsize error\n");
				}
			}
			else if(sgmt_size==64)
			{
				if(eSgmtType==0)
				{
					multiple[4]++;
				}
				else 
				{
					if(burstsize==1)
						multiple[0]++;
					else if(burstsize==2)
						multiple[1]++;
					else if(burstsize==3)
						multiple[2]++;
					else if(burstsize==4)
						multiple[3]++;
					//else
					//	HI_PRINT("burstsize error\n");
				}
			}
			else if(sgmt_size==256)
			{
				if(eSgmtType==0)
				{
					multiple[16]++;
				}
				else 
				{
					if(burstsize==1)
						multiple[0]++;
					else if(burstsize==2)
						multiple[1]++;
					else if(burstsize==3)
						multiple[2]++;
					else if(burstsize==4)
						multiple[3]++;
					else if(burstsize==5)
						multiple[4]++;
					else if(burstsize==6)
						multiple[5]++;
					else if(burstsize==7)
						multiple[6]++;
					else if(burstsize==8)
						multiple[7]++;
					else if(burstsize==9)
						multiple[8]++;
					else if(burstsize==10)
						multiple[9]++;
					else if(burstsize==11)
						multiple[10]++;
					else if(burstsize==12)
						multiple[11]++;
					else if(burstsize==13)
						multiple[12]++;
					else if(burstsize==14)
						multiple[13]++;
					else if(burstsize==15)
						multiple[14]++;
                    else if(burstsize==16)
                        multiple[15]++;
                    //else
                    //HI_PRINT("burstsize error\n");
                }
            }
			}
		}
        
		PBS_FlushBits(&pbs);

		size = PBS_GetDataSize(&pbs);


	/* 生成segment header */
	
	compress_en=1;
		compressed_data_len   = (unsigned short)size;

		*pSgmtHead =   (compress_en << 15)  | ((compressed_data_len & 0x7fff) )  ;


        pSgmtHead++;
        pDst += stride_luma ;

	total_size += stride_luma;

}

    
    /* 2. 压缩色度 */
    /*色度头信息*/
    pData=pDst;
    /* 生成信息区，信息区暂放置于帧头，用于描述各压缩段的起始地址/长度/模式等可能的信息 */
    pSgmtHead = (unsigned short *)(pData);

    /* 真正开始存放压缩数据的地址 */
    pDst      =  pData + Height * sizeof(unsigned short);
    total_size += (int)(pDst - pData);

    memset(pSgmtHead, 0, (int)(pDst - (unsigned char*)pSgmtHead));
    for (y = 0; y < Height/2; y++)
    {
        p0 = pFrame + Width*Height + y*Width/2;
        p1 = pFrame + Width*Height*5/4 + y*Width/2;

        u32Wth = SymbNum*2;

	size=0;
        
	PBS_InitPbs(&pbs, (int*)pDst, SymbNum * 2);  /* 2 pass已经保证了不膨胀 */

	/*判断是64/128/265一段*/
	if(sgmt_size==128)
	{
		if(u32Wth%128==0)
			u32LoopNum=u32Wth/128;
		else
			u32LoopNum=u32Wth/128+1;
	}
	else if(sgmt_size==64)
	{
		if(u32Wth%64==0)
			u32LoopNum=u32Wth/64;
		else
			u32LoopNum=u32Wth/64+1;
	}
	else if(sgmt_size==256)
	{
		if(u32Wth%256==0)
			u32LoopNum=u32Wth/256;
		else
			u32LoopNum=u32Wth/256+1;
	}

	for(j=0;j<u32LoopNum;j++)
		{
		burstsize=0;               //每段开始清burstsize

		if(sgmt_size==128)
		{
			if(u32Wth > 128)
			{
				if(j<(u32LoopNum - 1))
				{
					SgmtNum = 64;
				}
				else
				{
					SgmtNum = (u32Wth - (u32LoopNum-1)*128)/2;
				}
			}
		}
		if(sgmt_size==64)
		{
			if(u32Wth > 64)
			{
				if(j<(u32LoopNum - 1))
				{
					SgmtNum = 32;
				}
				else
				{
					SgmtNum = (u32Wth - (u32LoopNum-1)*64)/2;
				}
			}
		}
		if(sgmt_size==256)
		{
			if(u32Wth > 256)
			{
				if(j<(u32LoopNum - 1))
				{
					SgmtNum = 128;
				}
				else
				{
					SgmtNum = (u32Wth - (u32LoopNum-1)*256)/2;
				}
			}
		}

            eSgmtType = GetSegmentType(p0, p1, 1, SgmtNum);
                    
            //set compressor type
            PBS_PutBits(&pbs, eSgmtType, 1);
            
		burstsize++;

            if (eSgmtType == SGMT_TYPE_FLC)
            {
                for (i = 0; i < SgmtNum; i++)
                {
                    PBS_PutBits(&pbs, p0[0], 8);
                    PBS_PutBits(&pbs, p1[0], 8);

			burstsize+=16;

                    p0 += 1;
                    p1 += 1;
                }
            }
            else
            {
                /* 第一个符号: 保存原值 */
				
                PBS_PutBits(&pbs, p0[0], 8);
                PBS_PutBits(&pbs, p1[0], 8);
                    
		burstsize+=16;

                p0 += 1;
                p1 += 1;

                /* 从第二个符号开始: 保存偏差 or 原始值 */
                for (i = 1; i < SgmtNum; i++)
                {

			d0 = (int)p0[0] - (int)p0[-1];
			d1 = (int)p1[0] - (int)p1[-1];
                    
                    if ((d0 >= -CARE_RANGE && d0 <= CARE_RANGE) && (d1 >= -CARE_RANGE && d1 <= CARE_RANGE) &&   /* d0, d1在压缩编码的范围内 */
                        (Enctab[d0+CARE_RANGE][d1+CARE_RANGE][1] != 0))  /* 码表中确实有这个码 */ 
                    {
                        PBS_PutBits(&pbs, Enctab[d0+CARE_RANGE][d1+CARE_RANGE][0], Enctab[d0+CARE_RANGE][d1+CARE_RANGE][1]);
                    
						burstsize = burstsize + (UINT32)Enctab[d0+CARE_RANGE][d1+CARE_RANGE][1];
                    
                    }
                    else
                    {
                        PBS_PutBits(&pbs, EscCode[0], EscCode[1]);
                        
                        PBS_PutBits(&pbs, p0[0], 8);
                        PBS_PutBits(&pbs, p1[0], 8);

						burstsize=burstsize+18;
                    }

                    p0 += 1;
                    p1 += 1;
                }
			}

			/*最后一段不够64/128/256的话，这一段的burstsize就不统计了*/
			if((j!=(u32LoopNum-1)) || ((sgmt_size==128) && (u32Wth%128==0)) || ((sgmt_size==64) && (u32Wth%64==0)) || ((sgmt_size==256)&&(u32Wth%256==0)))
			{
			burstsize=(burstsize+7)/8;     //换成字节数

			if(burstsize%16==0)            //换成16字节的整数倍数
			{
				burstsize=burstsize/16;
			}
			else
			{
				burstsize=burstsize/16+1;
			}
			
			/*求出每段16字节的整数倍数的个数*/
			if(sgmt_size==128)
			{
				if(eSgmtType==0)
				{
					multiple[8]++;        
				}
				else 
				{
					if(burstsize==1)
						multiple[0]++;
					else if(burstsize==2)
						multiple[1]++;
					else if(burstsize==3)
						multiple[2]++;
					else if(burstsize==4)
						multiple[3]++;
					else if(burstsize==5)
						multiple[4]++;
					else if(burstsize==6)
						multiple[5]++;
					else if(burstsize==7)
						multiple[6]++;
					else if(burstsize==8)
						multiple[7]++;
					//else
						//HI_PRINT("burstsize error\n");
				}
			}
			else if(sgmt_size==64)
			{
				if(eSgmtType==0)
				{
					multiple[4]++;
				}
				else 
				{
					if(burstsize==1)
						multiple[0]++;
					else if(burstsize==2)
						multiple[1]++;
					else if(burstsize==3)
						multiple[2]++;
					else if(burstsize==4)
						multiple[3]++;
					//else
						//HI_PRINT("burstsize error\n");
				}
			}
			else if(sgmt_size==256)
			{
				if(eSgmtType==0)
				{
					multiple[16]++;
				}
				else 
				{
					if(burstsize==1)
						multiple[0]++;
					else if(burstsize==2)
						multiple[1]++;
					else if(burstsize==3)
						multiple[2]++;
					else if(burstsize==4)
						multiple[3]++;
					else if(burstsize==5)
						multiple[4]++;
					else if(burstsize==6)
						multiple[5]++;
					else if(burstsize==7)
						multiple[6]++;
					else if(burstsize==8)
						multiple[7]++;
					else if(burstsize==9)
						multiple[8]++;
					else if(burstsize==10)
						multiple[9]++;
					else if(burstsize==11)
						multiple[10]++;
					else if(burstsize==12)
						multiple[11]++;
					else if(burstsize==13)
						multiple[12]++;
					else if(burstsize==14)
						multiple[13]++;
					else if(burstsize==15)
						multiple[14]++;
					else if(burstsize==16)
						multiple[15]++;
					//else
					//	HI_PRINT("burstsize error\n");
				}
			}
			}
		}
        
		PBS_FlushBits(&pbs);

	size = PBS_GetDataSize(&pbs);


	/* 生成segment header */
	
		compress_en=1;
		compressed_data_len   = (unsigned short)size;

		*pSgmtHead =   (compress_en << 15)  | ((compressed_data_len & 0x7fff) )  ;

		total_size += stride_chrome;

		pSgmtHead++;
		pDst += stride_chrome;
    }
    /*将色度无效头信息设成0*/
    for(y=0;y<Height/2;y++)
    {
		*pSgmtHead++=0;
    }

    return total_size;
}
#endif


SGMT_TYPE_E  GetSegmentType(unsigned char *p0, unsigned char *p1, int step, int SymbNum)
{
    int i;
    int d0, d1;
    SGMT_TYPE_E  eSgmtType = SGMT_TYPE_FLC;
    int  nBitsVlc, nBitsFlc;

    p0 += step;
    p1 += step;
    
    nBitsVlc = 16;

    /* 从第二个符号开始统计比特数 */
    for (i = 1; i < SymbNum; i++)
    {

		d0 = (int)p0[0] - (int)p0[-1];
        d1 = (int)p1[0] - (int)p1[-1];

        if ((d0 >= -CARE_RANGE && d0 <= CARE_RANGE) && (d1 >= -CARE_RANGE && d1 <= CARE_RANGE) &&   /* d0, d1在压缩编码的范围内 */
            (Enctab[d0+CARE_RANGE][d1+CARE_RANGE][1] != 0))  /* 码表中确实有这个码 */ 
        {
            nBitsVlc     += Enctab[d0+CARE_RANGE][d1+CARE_RANGE][1];
        }
        else
        {
            /* miss，如果vlc先放一个esc码然后保存原始值, 如果hybrid放1比特类型再放原始值 */
            nBitsVlc     += (EscCode[1] + 16);
        }
        
        p0 += step;
        p1 += step;
    }

    nBitsFlc = SymbNum * 16;

    if (nBitsVlc < nBitsFlc)
    {
        eSgmtType = SGMT_TYPE_VLC;
    }
    else
    {
        eSgmtType = SGMT_TYPE_FLC;
    }
    
    return eSgmtType;
}

extern int PrintDetailEn;
//extern FILE *fpLog;

int decompress(unsigned char *pData, int DataLen, int Width, int Height, int stride_luma, int stride_chrome, unsigned char *pFrame)
{
    int i;
    unsigned char *pStream = NULL;
    unsigned short *pSgmtHead;
    unsigned char *p0, *p1;
    //unsigned char *pStr = 0;

	int j,k;
	int  UncprSgmtSize = 0  ;
	int  marker, d0, d1;
	BS bs;
	//unsigned short compressed_data_len;
	unsigned short  compress_en;
	//int dataLength=0;
	//int totalData=0;
	int u32LoopNum;
	unsigned short SegHead;
	int step;

    //pstFrameInfo = (FRAME_INFO_HEADER_S*)pData;
    pSgmtHead    = (unsigned short*)(pData);
   pStream      = pData +  Height  * sizeof(unsigned short);
    /* 解压, 逐段解压，直到整个平面完全解压完毕 */
    p0 = pFrame;
    p1 = p0 + 1;
//HI_PRINT("line:%d\n",__LINE__);
    for (i = 0; i < Height; i++)
    {
		/* 解析段头，以及必要的准备 */
		SegHead=pSgmtHead[0];
		compress_en = (SegHead >> 15) & 1;
		//compressed_data_len = (SegHead >> 1) & 0x7fff;
		
	step=2;
	BsInit(&bs, pStream, Width*Height);
		
	if(Width%128 == 0)
		u32LoopNum=Width/128;
	else
		u32LoopNum=Width/128+1;
	
	if(compress_en==1)                 //改行是压缩后的数据
	{
		for(j=0;j<u32LoopNum;j++)
		{	
			if(j < (u32LoopNum - 1))
			{
				UncprSgmtSize = 128;
			}
			else
			{
				UncprSgmtSize = (Width - (u32LoopNum-1)*128);
			}

			marker = BsGet(&bs, 1);

			if(marker == 0)//SGMT_TYPE_FLC
			{
				for(k = 0; k < UncprSgmtSize/2; k++)
				{
					p0[0] = (unsigned char)BsGet(&bs,8);
					p1[0] = (unsigned char)BsGet(&bs,8);
					p0+=step;
					p1+=step;
					//HI_PRINT("line:%d,p0[0]:%d,p1[0]\n",__LINE__,p0[0],p1[0]);
				}
			}
			else              //VLC
			{
				p0[0] = (unsigned char)BsGet(&bs, 8);
				p1[0] = (unsigned char)BsGet(&bs, 8);
				
				p0 += step;
				p1 += step;

				for (k = 1; k < UncprSgmtSize/2; k++)
				{
					marker = BsShow(&bs, EscCode[1]);
					if (marker == EscCode[0])
					{
						BsSkip(&bs,EscCode[1]);
						p0[0] = (unsigned char)BsGet(&bs,8);
						p1[0] = (unsigned char)BsGet(&bs,8);

					}
					else
					{
						marker = BsShow(&bs, MAX_CODE_LEN);
						d0 = Dectab[marker][0];
						d1 = Dectab[marker][1];
						BsSkip(&bs, Dectab[marker][2]);

						//dataLength=Dectab[marker][2];
						//totalData+=dataLength;

						if (step == 2)
						{
							p0[0] = (unsigned char)(d0 + (int)p1[-1*step]);
							p1[0] = (unsigned char)(d1 + (int)p0[0]);
						}
						else
						{
							p0[0] = (unsigned char)(d0 + (int)p0[-1]);
							p1[0] = (unsigned char)(d1 + (int)p1[-1]);
						}
					}
					//HI_PRINT("line:%d,p0[0]:%d,p1[0]\n",__LINE__,p0[0],p1[0]);
					p0 += step;
					p1 += step;
				}
			}
		}
	}
	else                 //改行是未压缩的数据
	{
		for(j=0;j<Width/2;j++)
		{
			p0[0] = (unsigned char)BsGet(&bs,8);
			p1[0] = (unsigned char)BsGet(&bs,8);
			p0+=step;
			p1+=step;
		}
	}

	//fflush(fpLog);

		pSgmtHead++;

		pStream += stride_luma;
		
	p0 = p0  + ( stride_luma - Width );
	p1 = p1  + ( stride_luma - Width );
    }
//HI_PRINT("line:%d,p0[0]:%d,p1[0]:%d\n",__LINE__,p0[-2],p1[-2]);
//fwrite(pFrame, 1, Height*Width, fpOut);
	
	/* 解压色度，U,V两个平面一起解压 */
    
    p0 = pFrame + stride_luma * Height;                  //V色度
    p1 = pFrame + stride_luma * Height + 1;              //U色度
    pSgmtHead    = (unsigned short*)(pStream);
    pStream      = pStream +  Height  * sizeof(unsigned short);
    
    for (i = 0; i < Height/2; i++)
    {
	/* 解析色度段头，以及必要的准备 */
		SegHead=pSgmtHead[0];
		compress_en = (SegHead >> 15) & 1;
		//compressed_data_len = (SegHead >> 1) & 0x7fff;
		
		if(SegHead==0)
			continue;
		else
		{
			step=2;
		BsInit(&bs, pStream, Width*Height/2);
		
		if(Width%128 == 0)
			u32LoopNum=Width/128;
		else
			u32LoopNum=Width/128+1;
				
		if(compress_en==1)           //该行是压缩后的数据
		{
			for(j=0;j<u32LoopNum;j++)
			{
				if(j < (u32LoopNum - 1))
				{
					UncprSgmtSize = 128;
				}
				else
				{
					UncprSgmtSize = (Width - (u32LoopNum-1)*128);
				}
	
				marker = BsGet(&bs, 1);
	
				if(marker == 0)//SGMT_TYPE_FLC
				{
					for(k = 0; k < UncprSgmtSize/2; k++)
					{
						p0[0] = (unsigned char)BsGet(&bs,8);
						p1[0] = (unsigned char)BsGet(&bs,8);
						p0+=step;
						p1+=step;
					}
				}
				else              //VLC
				{
					p0[0] = (unsigned char)BsGet(&bs, 8);
					p1[0] = (unsigned char)BsGet(&bs, 8);
					
					p0 += step;
					p1 += step;
	
					for (k = 1; k < UncprSgmtSize/2; k++)
					{
						marker = BsShow(&bs, EscCode[1]);
						if (marker == EscCode[0])
						{
							BsSkip(&bs,EscCode[1]);
							p0[0] = (unsigned char)BsGet(&bs,8);
							p1[0] = (unsigned char)BsGet(&bs,8);
						}
						else
						{
							marker = BsShow(&bs, MAX_CODE_LEN);
							d0 = Dectab[marker][0];
							d1 = Dectab[marker][1];
							BsSkip(&bs, Dectab[marker][2]);
	
							//dataLength=Dectab[marker][2];
							//totalData+=dataLength;
	
							p0[0] = (unsigned char)(d0 + (int)p0[-2]);
							p1[0] = (unsigned char)(d1 + (int)p1[-2]);
						}
						p0 += step;
						p1 += step;
					}
				}
			}
		}
		else			//改行未压缩
		{
			for(j=0;j<Width/2;j++)
			{
				p0[0] = (unsigned char)BsGet(&bs,8);
				p1[0] = (unsigned char)BsGet(&bs,8);
				p0+=step;
				p1+=step;
			}
		}
	
		//fflush(fpLog);
		pSgmtHead++;

	pStream += stride_chrome;

		}

	p0 = p0  + ( stride_chrome - Width );
	p1 = p1  + ( stride_chrome - Width );
    }
    return (pStream - pData);
}
