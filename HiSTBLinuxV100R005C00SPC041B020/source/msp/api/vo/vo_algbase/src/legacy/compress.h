#define  CARE_RANGE  8

#define  PIX_MARKER  0
#define  DIF_MARKER  1

/* segment length index */
#define  SGMT_LEN_IDX_64      0
#define  SGMT_LEN_IDX_128     1
#define  SGMT_LEN_IDX_256     2
#define  SGMT_LEN_IDX_ROW     3

/*the number of bytes in one segment*/
#define  SGMT_SIZE            128

typedef struct 
{
    unsigned short  LumaSgmtNum;
    unsigned short  ChromSgmtNum;
} FRAME_INFO_HEADER_S;

typedef enum
{
    SGMT_TYPE_FLC,
    SGMT_TYPE_VLC
} SGMT_TYPE_E;


void InitCompressor();
int compress(unsigned char *pFrame, int Width, int Height,unsigned int *multiple, int sgmt_size, unsigned char *pData );
int decompress(unsigned char *pData, int DataLen, int Width, int Height, int stride_luma, int stride_chrome, unsigned char *pFrame);
