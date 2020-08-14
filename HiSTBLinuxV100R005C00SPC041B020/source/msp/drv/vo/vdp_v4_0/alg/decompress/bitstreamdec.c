//--------
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#include "ttv_mode.h"


uint32 i_mask[33] = {
					 0x00,
                     0x01,      0x03,      0x07,      0x0f,
                     0x1f,      0x3f,      0x7f,      0xff,
                     0x1ff,     0x3ff,     0x7ff,     0xfff,
                     0x1fff,    0x3fff,    0x7fff,    0xffff,
                     0x1ffff,   0x3ffff,   0x7ffff,   0xfffff,
                     0x1fffff,  0x3fffff,  0x7fffff,  0xffffff,
                     0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
                     0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff
};

void BitStreamInitDec (BIT_STREAM_DEC * bs_ptr, uint8 * p_data, int i_data)
{
	bs_ptr->p_start		= p_data;
    bs_ptr->p			= p_data;
    bs_ptr->i_left		= 8;
	bs_ptr->total_bits	= 0;
}



uint32 BitStreamShowBits (int n_bits)
{
	uint32 i_cache;

	if ( (n_bits > 0))
    {
		i_cache = ((bit_stream_dec->p[3] << 24) + (bit_stream_dec->p[2] << 16) + 
						    (bit_stream_dec->p[1] << 8)  + bit_stream_dec->p[0]) >> (8 - bit_stream_dec->i_left);

		return (i_cache & i_mask[n_bits]);
	}

	return 0;
}


void BitStreamFlushBits (int n_bits)
{	
    bit_stream_dec->i_left -= n_bits;
	bit_stream_dec->total_bits += n_bits;
	bit_stream_dec->cur_mb_bit+= n_bits;
    while (bit_stream_dec->i_left <= 0)
    {
        bit_stream_dec->p++;
        bit_stream_dec->i_left += 8;
    }
}



uint32 BitStreamReadBits (int n_bits)
{
    int      i_shr;
    uint32   i_result = 0;
	int      i_pos = 0;


	bit_stream_dec->cur_mb_bit+= n_bits;

    while( n_bits > 0 )
    {


		if( ( i_shr = bit_stream_dec->i_left - n_bits ) >= 0 )
        {
            /* more in the buffer than requested */
			i_result |= ((*bit_stream_dec->p >> (8 - bit_stream_dec->i_left)) & i_mask[n_bits]) << i_pos;
			bit_stream_dec->i_left -= n_bits;
            if( bit_stream_dec->i_left == 0 )
            {
                bit_stream_dec->p++;
                bit_stream_dec->i_left = 8;
            }
            return( i_result );
		}
		else
		{
			/* less in the buffer than requested */
			i_result |= ((*bit_stream_dec->p >> (8 - bit_stream_dec->i_left)) & i_mask[bit_stream_dec->i_left]) << i_pos;

			i_pos   += bit_stream_dec->i_left;
			n_bits  -= bit_stream_dec->i_left;
			bit_stream_dec->p++;
			bit_stream_dec->i_left = 8;
		}

	}

	return( i_result );
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif