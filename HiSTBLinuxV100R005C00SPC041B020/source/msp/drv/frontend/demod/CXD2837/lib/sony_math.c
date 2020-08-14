/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-03-28 01:40:32 #$
  File Revision : $Revision:: 4945 $
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Includes
------------------------------------------------------------------------------*/
#include "sony_math.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/

#define MAX_BIT_PRECISION        5
#define FRAC_BITMASK            0x1F                    /**< Depends upon MAX_BIT_PRECISION. */
#define LOG2_10_100X            332                     /**< log2 (10) */
#define LOG2_E_100X             144                     /**< log2 (e) */

/*------------------------------------------------------------------------------
 Statics
------------------------------------------------------------------------------*/
/**
 @brief Look up table for decimal portion of log2 calculation.
*/
static const uint8_t log2LookUp[] = {
    0, /* 0 */ 4,               /* 0.04439 */
    9, /* 0.08746 */ 13,        /* 0.12928 */
    17, /* 0.16993 */ 21,       /* 0.20945 */
    25, /* 0.24793 */ 29,       /* 0.28540 */
    32, /* 0.32193 */ 36,       /* 0.35755 */
    39, /* 0.39232 */ 43,       /* 0.42627 */
    46, /* 0.45943 */ 49,       /* 0.49185 */
    52, /* 0.52356 */ 55,       /* 0.55249 */
    58, /* 0.58496 */ 61,       /* 0.61471 */
    64, /* 0.64386 */ 67,       /* 0.67246 */
    70, /* 0.70044 */ 73,       /* 0.72792 */
    75, /* 0.75489 */ 78,       /* 0.78136 */
    81, /* 0.80736 */ 83,       /* 0.83289 */
    86, /* 0.85798 */ 88,       /* 0.88264 */
    91, /* 0.90689 */ 93,       /* 0.93074 */
    95, /* 0.95420 */ 98        /* 0.97728 */
};

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

uint32_t sony_math_log2 (uint32_t x)
{
    uint8_t count = 0;
    uint8_t index = 0;
    uint32_t xval = x;

    /* Get the MSB position. */
    for (x >>= 1; x > 0; x >>= 1) {
        count++;
    }

    x = count * 100;

    if (count > 0) {
        if (count <= MAX_BIT_PRECISION) {
            /* Mask the bottom bits. */
            index = (uint8_t) (xval << (MAX_BIT_PRECISION - count)) & FRAC_BITMASK;
            x += log2LookUp[index];
        }
        else {
            /* Mask the bits just below the radix. */
            index = (uint8_t) (xval >> (count - MAX_BIT_PRECISION)) & FRAC_BITMASK;
            x += log2LookUp[index];
        }
    }

    return (x);
}

uint32_t sony_math_log10 (uint32_t x)
{
    /* log10(x) = log2 (x) / log2 (10) */
    /* Note uses: logN (x) = logM (x) / logM (N) */
    return ((100 * sony_math_log2 (x) + LOG2_10_100X / 2) / LOG2_10_100X);
}

uint32_t sony_math_log (uint32_t x)
{
    /* ln (x) = log2 (x) / log2(e) */
    return ((100 * sony_math_log2 (x) + LOG2_E_100X / 2) / LOG2_E_100X);
}
