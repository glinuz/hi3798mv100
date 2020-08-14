/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_lib_mtx.c
*
* @brief supporting library functions for TPG
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/

//#include <math.h>

#include "si_lib_mtx_api.h"
#include "si_lib_malloc_api.h"
#include "platform_api.h"
#include "si_lib_obj_api.h"


/***** Register Module name **************************************************/

//SII_LIB_OBJ_MODULE_DEF(lib_mtx);

/***** local macro definitions ***********************************************/

#define   dArray(pMtx,row,col)   (((float*)&pMtx->pArr)[(row)*pMtx->Col+(col)])
#define   dIsNaNf(flt)           (((0x7f800000 - ((*(uint32_t*)&(flt)) & 0x7fffffff)) >> 31) ? (TRUE) : (FALSE)) /* Check if float number */

/***** local prototypes ******************************************************/

static SiiLibMtx_t* sCreate( SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, const char* pStr );
static void      sDelete( SiiLibMtx_t* pMtx );
//static void      sIdentity( SiiLibMtx_t* pMtx );
//static void      sAdd( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
static void      sMultiply( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS1, SiiLibMtx_t* pMtxS2 );
//static void      sInverse( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
//static void      sAdjugate( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
//static void      sTranspose( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
//static bool_t    sIsSquare( SiiLibMtx_t* pMtx );
static void      sCopy( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
//static void      sScaler( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS, float fScaler );
static void      sZero( SiiLibMtx_t* pMtx );
//static float     sDeterminant( SiiLibMtx_t* pMtx );
//static float     sCofactor( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col );
//static void      sCofactors( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
//static void      sMinor( SiiLibMtx_t* pMtxD, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, SiiLibMtx_t* pMtxS );
//static void      sGetSkewSymmetricMatrix( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
//static void      sCheckSquare( SiiLibMtx_t* pMtx );
static void      sCheckSameSize( SiiLibMtx_t* pMtx1, SiiLibMtx_t* pMtx2 );
//static void      sCheckMinor( SiiLibMtx_t* pMtx1, SiiLibMtx_t* pMtx2 );
static void      sCheckMultiply( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS1, SiiLibMtx_t* pMtxS2 );

/***** public functions ******************************************************/

SiiLibMtx_t* SiiLibMtxCreate( SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, const char* pStr )
{
	return sCreate(Row, Col, pStr);
}

void SiiLibMtxDelete( SiiLibMtx_t* pMtx )
{
	sDelete(pMtx);
}

void SiiLibMtxLoad( SiiLibMtx_t* pMtx, const float *pArr )
{
	SiiLibMtxIdx_t r, c;

	SII_PLATFORM_DEBUG_ASSERT(pMtx);
	for( r=0; r<pMtx->Row; r++ )
		for( c=0; c<pMtx->Col; c++ )
			dArray(pMtx, r, c) = *(pArr++);	
}

#if 0 // not used
void SiiLibMtxCopy( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	sCopy(pMtxD, pMtxS);
}
#endif

void SiiLibMtxSetCoef( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, float Value )
{
	dArray(pMtx, Row, Col) = Value;
}

float SiiLibMtxGetCoef( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col )
{
	return dArray(pMtx, Row, Col);
}

#if 0 // not used
void SiiLibMtxZero( SiiLibMtx_t* pMtx )
{
	sZero(pMtx);
}

void SiiLibMtxIdentity( SiiLibMtx_t* pMtx )
{
	sIdentity(pMtx);
}

void SiiLibMtxInverse( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	sInverse(pMtxD, pMtxS);
}

void SiiLibMtxTranspose( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	sTranspose(pMtxD, pMtxS);
}

void SiiLibMtxMinor( SiiLibMtx_t* pMtxD, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, SiiLibMtx_t* pMtxS )
{
	sMinor(pMtxD, Row, Col, pMtxS);
}

float SiiLibMtxCofactor( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col )
{
	return sCofactor(pMtx, Row, Col);
}

void SiiLibMtxCofactors( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	sCofactors(pMtxD, pMtxS);
}

void SiiLibMtxAdjugate( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	sAdjugate(pMtxD, pMtxS);
}

void SiiLibMtxScaler( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS, float fScaler )
{
	sScaler(pMtxD, pMtxS, fScaler);
}

float SiiLibMtxDeterminant( SiiLibMtx_t* pMtx )
{
	return sDeterminant(pMtx);
}

void SiiLibMtxAdd( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	sAdd(pMtxD, pMtxS);
}
#endif

void SiiLibMtxMultiply( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS1, SiiLibMtx_t* pMtxS2 )
{
	sMultiply(pMtxD, pMtxS1, pMtxS2);
}

#if 0 // not used
void SiiLibMtxClip( SiiLibMtx_t* pMtx, float fHigh, float fLow )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtx);

	/* Clip all values */
	for( r=0; r<pMtx->Row; r++ )
	{
		for( c=0; c<pMtx->Col; c++ )
		{
			if     ( fHigh < dArray(pMtx, r, c) )
				dArray(pMtx, r, c) = fHigh;
			else if( fLow > dArray(pMtx, r, c) )
				dArray(pMtx, r, c) = fLow;
		}
	}
}

void SiiLibMtxRound( SiiLibMtx_t* pMtx )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtx);

	/* Round all values */
	for( r=0; r<pMtx->Row; r++ )
	{
		for( c=0; c<pMtx->Col; c++ )
		{
			dArray(pMtx, r, c) = 1.0;
		}
	}
}

void SiiLibMtxCrossProduct( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS1, SiiLibMtx_t* pMtxS2 ) /* Also known as Vector Product */
{
	/* The adjugate matrix is the transpose of the matrix of cofactors */
	SiiLibMtx_t* pMtxT;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS1);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS2);

	/* Check same size */
	sCheckSameSize(pMtxD, pMtxS1);
	sCheckSameSize(pMtxD, pMtxS2);

	/* Create temporarily matrix */
	pMtxT = sCreate(pMtxS1->Row, pMtxS1->Col, "Temp");

	/* Get Skew Symmetric Matrix */
	sGetSkewSymmetricMatrix(pMtxT, pMtxS1);

	/* Calculate Cross product */
	sMultiply(pMtxD, pMtxT, pMtxS2);

	/* Delete temporarily matrix */
	sDelete(pMtxT);
}

void SiiLibMtxDump( SiiLibMtx_t* pMtx )
{
	SiiLibMtxIdx_t c, r;

	SII_PLATFORM_DEBUG_ASSERT(pMtx);
	SII_PRINTF("M:%s\n", pMtx->pStr);
	for( r=0; r<pMtx->Row; r++ )
	{
		SII_PRINTF("  | ");
		{
			for( c=0; c<pMtx->Col; c++ )
				SII_PRINTF("%+f ", dArray(pMtx, r, c));
		}
		SII_PRINTF("|\n");
	}
}

void SiiLibMtxXYZ2xyz( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);

	/* Check same size */
	sCheckSameSize(pMtxD, pMtxS);

	for( c=0; c<pMtxS->Col; c++ )
	{
		float fTot = 0.0;

		for( r=0; r<pMtxS->Row; r++ )
		{
			fTot += dArray(pMtxS, r, c); 
		}

		for( r=0; r<pMtxS->Row; r++ )
		{
			dArray(pMtxD, r, c) = dArray(pMtxS, r, c) / fTot;
		}
	}
}

void SiiLibMtxVect2Diag( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t r;

	SII_PLATFORM_DEBUG_ASSERT(pMtxD);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(1 == pMtxS->Col);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS->Row == pMtxD->Row);

	sZero(pMtxD);
	for( r=0; r<pMtxS->Row; r++ )
	{
		dArray(pMtxD, r, r) = dArray(pMtxS, r, 0);
	}
}

void SiiLibMtxDiag2Vect( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t r;

	SII_PLATFORM_DEBUG_ASSERT(pMtxD);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(1 == pMtxD->Col);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS->Row == pMtxD->Row);

	for( r=0; r<pMtxS->Row; r++ )
	{
		dArray(pMtxD, r, 0) = dArray(pMtxS, r, r);
	}
}

void SiiLibMtxXYZ2xyY( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);

	/* Check same size */
	sCheckSameSize(pMtxD, pMtxS);

	for( c=0; c<pMtxS->Col; c++ )
	{
		float fTot = 0.0;
		float fY   = 0.0;

		for( r=0; r<pMtxS->Row; r++ )
		{
			fTot += dArray(pMtxS, r, c); 
		}

		fY = dArray(pMtxS, pMtxS->Row-2, c);
		for( r=0; r<(pMtxS->Row-1); r++ )
		{
			dArray(pMtxD, r, c) = dArray(pMtxS, r, c) / fTot;
		}
		dArray(pMtxD, r, c) = fY;
	}
}

void SiiLibMtxxyY2XYZ( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtx_t*   pMtxT;
	SiiLibMtxIdx_t c;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);

	/* Check same size */
	sCheckSameSize(pMtxD, pMtxS);

	pMtxT = sCreate(pMtxS->Row, pMtxS->Col, "T1");

	for( c=0; c<pMtxS->Col; c++ )
	{
		dArray(pMtxT, 0, c) = (dArray(pMtxS, 0, c)*dArray(pMtxS, 2, c)) / dArray(pMtxS, 1, c);
		dArray(pMtxT, 1, c) = dArray(pMtxS, 2, c);
		dArray(pMtxT, 2, c) = ((1.0-dArray(pMtxS, 0, c)-dArray(pMtxS, 1, c))*dArray(pMtxS, 2, c)) / dArray(pMtxS, 1, c);
	}

	sCopy(pMtxD, pMtxT);
	sDelete(pMtxT);
}

void SiiLibMtxGamma( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS, float Gamma )
{
	SiiLibMtxIdx_t c, r;

	for( c=0; c<pMtxS->Col; c++ )
	{
		for( r=0; r<pMtxS->Row; r++ )
		{
			dArray(pMtxD, r, c) = pow(dArray(pMtxS, r, c), Gamma);
		}
	}
}

void SiiLibMtxScaleRow( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, float fScaler )
{
	SiiLibMtxIdx_t c;

	SII_PLATFORM_DEBUG_ASSERT(pMtx);
	SII_PLATFORM_DEBUG_ASSERT(Row<pMtx->Row);

	for( c=0; c<pMtx->Col; c++ )
	{
		dArray(pMtx, Row, c) *= fScaler;
	}
}
#endif

/***** local functions *******************************************************/

static SiiLibMtx_t* sCreate( SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, const char* pStr )
{
	SiiLibMtx_t* pMtx;
	uint16_t  Size = sizeof(SiiLibMtx_t) + sizeof(float)*Col*Row;

	pMtx = (SiiLibMtx_t*)SiiLibMallocCreate(Size);
	SII_PLATFORM_DEBUG_ASSERT(pMtx);

	pMtx->Col   = Col;
	pMtx->Row   = Row;
	pMtx->pStr  = pStr;

	/* Zero Matrix */
	sZero(pMtx);

	return pMtx;
}

static void sDelete( SiiLibMtx_t* pMtx )
{
	SII_PLATFORM_DEBUG_ASSERT(pMtx);
	SiiLibMallocDelete(pMtx);
}

#if 0 // not used
static void sIdentity( SiiLibMtx_t* pMtx )
{
	SiiLibMtxIdx_t r;

	/* Check for Valid matrix */
	SII_PLATFORM_DEBUG_ASSERT(pMtx);

	/* Check for square matrix */
	sCheckSquare(pMtx);

	sZero(pMtx);
	for( r=0; r<pMtx->Row; r++ )
		dArray(pMtx, r, r) = 1.0;
}

static void sAdd( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);

	if( 1 == pMtxS->Col )
	{
		/* Check if number of rows are the same */
		SII_PLATFORM_DEBUG_ASSERT(pMtxS->Row == pMtxD->Row);

		/* Add Input Vectot to each Matrix column */
		for( c=0; c<pMtxD->Col; c++ )
			for( r=0; r<pMtxD->Row; r++ )
				dArray(pMtxD, r, c) += dArray(pMtxS, r, 0);
	}
	else
	{
		/* Check same size */
		sCheckSameSize(pMtxD, pMtxS);

		/* Add Matrix 1:1 */
		for( c=0; c<pMtxD->Col; c++ )
			for( r=0; r<pMtxD->Row; r++ )
				dArray(pMtxD, r, c) += dArray(pMtxS, r, c);
	}
}
#endif

static void sMultiply( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS1, SiiLibMtx_t* pMtxS2 )
{
	SiiLibMtx_t*   pMtxT;
	SiiLibMtxIdx_t c, r, s;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS1);
	SII_PLATFORM_DEBUG_ASSERT(pMtxS2);

	pMtxT = sCreate(pMtxS2->Row, pMtxS2->Col, "T");

	/* Check if Matrices can be multiplied */
	sCheckMultiply(pMtxT, pMtxS1, pMtxS2);

	/* Multiply Destination Matrix by Source Matrix */
	for( r=0; r<pMtxS1->Row; r++ )
	{
		for( c=0; c<pMtxS2->Col; c++ )
		{
			register float fAdd = 0.0;

			for( s=0; s<pMtxS2->Row; s++ )
				fAdd += dArray(pMtxS1, r, s) * dArray(pMtxS2, s, c); 

			dArray(pMtxT, r, c) = fAdd;
		}
	}
	sCopy(pMtxD, pMtxT);
	sDelete(pMtxT);
}

#if 0 // not used
static void sInverse( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	float fDet;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);

	/* Check for square matrices */
	sCheckSquare(pMtxS);
	sCheckSquare(pMtxD);

	/* Find determinant value */
	fDet = sDeterminant(pMtxS);

	/* Find Adjugate matrix */
	sAdjugate(pMtxD, pMtxS);

	/* Scale Adjugate matrix with 1/det */
	sScaler(pMtxD, pMtxD, 1.0 / fDet);
}

static void sAdjugate( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	/* The adjugate matrix is the transpose of the matrix of cofactors */
	SiiLibMtx_t* pMtxT;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);

	/* Check for square matrices */
	sCheckSquare(pMtxS);

	/* Check same size */
	sCheckSameSize(pMtxD, pMtxS);

	/* Create temporarily matrix */
	pMtxT = sCreate(pMtxS->Row, pMtxS->Col, "Tadj");

	/* Calculate Cofactor Matrix */    
	sCofactors(pMtxT, pMtxS);

	/* Find Transpose Matrix */    
	sTranspose(pMtxD, pMtxT);

	/* Delete temporarily matrix */
	sDelete(pMtxT);
}

static void sTranspose( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t c, r;
	SiiLibMtx_t*   pMtxT;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);

	/* Create temporarily matrix */
	pMtxT = sCreate(pMtxD->Row, pMtxD->Col, "Ttrp");

	/* Find Transpose Matrix */
	for( r=0; r<pMtxD->Row; r++ )
		for( c=0; c<pMtxD->Col; c++ )
			dArray(pMtxT, c, r) = dArray(pMtxS, r, c);

	/* Copy temp matrix to destination matrix */
	sCopy(pMtxD, pMtxT);

	/* Delete temporarily matrix */
	sDelete(pMtxT);
}

static bool_t sIsSquare( SiiLibMtx_t* pMtx )
{
	return (pMtx->Row == pMtx->Col) ? (TRUE) : (FALSE);
}
#endif

static void sCopy( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);

	/* Check same size */
	sCheckSameSize(pMtxD, pMtxS);

	/* Copy Matrix content */
	for( r=0; r<pMtxS->Row; r++ )
		for( c=0; c<pMtxS->Col; c++ )
			dArray(pMtxD, r, c) = dArray(pMtxS, r, c);
}

#if 0 // not used
static void sScaler( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS, float fScaler )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);

	for( r=0; r<pMtxS->Row; r++ )
		for( c=0; c<pMtxS->Col; c++ )
			dArray(pMtxD, r, c) = dArray(pMtxS, r, c) * fScaler;
}
#endif

static void sZero( SiiLibMtx_t* pMtx )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrix */
	SII_PLATFORM_DEBUG_ASSERT(pMtx);

	/* Zero all matrix values */
	for( r=0; r<pMtx->Row; r++ )
		for( c=0; c<pMtx->Col; c++ )
			dArray(pMtx, r, c) = 0.0;
}

#if 0 // not used
static float sDeterminant( SiiLibMtx_t* pMtx )
{
	SiiLibMtx_t*    pMtxT;
	SiiLibMtxIdx_t  Order = pMtx->Col;
	float        fDet  = 1.0;
	SiiLibMtxIdx_t  i, j, k;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtx);

	/* Check for square matrices */
	sCheckSquare(pMtx);

	pMtxT = sCreate(pMtx->Row, pMtx->Col, "T");
	sCopy(pMtxT, pMtx);
	for( i=0; i<Order; i++ )
	{
		for( j=0; j<Order; j++ )
		{
			float fMul = dArray(pMtxT, j, i) / dArray(pMtxT, i, i);

			if( i != j )
			{
				for( k=0; k<Order; k++ )
				{
					dArray(pMtxT, j, k) = dArray(pMtxT, j, k) - dArray(pMtxT, i, k)*fMul;
				}
			}
		}
	}

	for( i=0; i<Order; i++ )
	{
		fDet = fDet * dArray(pMtxT, i, i);
	}

	sDelete(pMtxT);
	return (dIsNaNf(fDet)) ? (0.0) : (fDet);
}

static float sCofactor( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col )
{
	SiiLibMtx_t* pMtxT;
	float     fCofac;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtx);

	/* Check for square matrices */
	sCheckSquare(pMtx);

	pMtxT = sCreate(pMtx->Row-1, pMtx->Col-1, "T");
	sMinor(pMtxT, Row, Col, pMtx);
	fCofac = sDeterminant(pMtxT);
	if( (Row+Col)&1 )
		fCofac *= -1.0;
	sDelete(pMtxT);
	return fCofac;
}

static void sCofactors( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t c, r;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);

	/* Check same size */
	sCheckSameSize(pMtxD, pMtxS);

	/* Calculate Matrix of Cofactors */    
	for( r=0; r<pMtxD->Row; r++ )
		for( c=0; c<pMtxD->Col; c++ )
			dArray(pMtxD, r, c) = sCofactor(pMtxS, r, c);
}

static void sMinor( SiiLibMtx_t* pMtxD, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, SiiLibMtx_t* pMtxS )
{
	SiiLibMtxIdx_t r1, c1;
	SiiLibMtxIdx_t r2, c2;

	/* Check for Valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(pMtxS);
	SII_PLATFORM_DEBUG_ASSERT(pMtxD);

	/* Check size of destination Matrix */
	sCheckMinor(pMtxD, pMtxS);

	for( r1=0, r2=0; r1<pMtxS->Row; r1++ )
	{
		if( r1 == Row ) continue;

		for( c1=0, c2=0; c1<pMtxS->Col; c1++ )
		{
			if( c1 == Col ) continue;

			dArray(pMtxD, r2, c2) = dArray(pMtxS, r1, c1);
			c2++;
		}
		r2++;
	}
}

static void sGetSkewSymmetricMatrix( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS )
{
	/* For now just 3x3 only */
	SII_PLATFORM_DEBUG_ASSERT(3==pMtxD->Col);
	SII_PLATFORM_DEBUG_ASSERT(3==pMtxD->Row);
	SII_PLATFORM_DEBUG_ASSERT(1==pMtxS->Col);
	SII_PLATFORM_DEBUG_ASSERT(3==pMtxS->Row);

	dArray(pMtxD, 1, 0) = +dArray(pMtxS, 2, 0);
	dArray(pMtxD, 0, 1) = -dArray(pMtxS, 2, 0);

	dArray(pMtxD, 2, 0) = -dArray(pMtxS, 1, 0);
	dArray(pMtxD, 0, 2) = +dArray(pMtxS, 1, 0);

	dArray(pMtxD, 2, 1) = +dArray(pMtxS, 0, 0);
	dArray(pMtxD, 1, 2) = -dArray(pMtxS, 0, 0);
}

static void sCheckSquare( SiiLibMtx_t* pMtx )
{
	if( !sIsSquare(pMtx) )
	{
		//DEBUG_PRINT(MSG_ALWAYS, "ERROR: [%s] is not a Square Matrix.\n", pMtx->pStr);
		SII_PLATFORM_DEBUG_ASSERT(0);
	}
}

static void sCheckMinor( SiiLibMtx_t* pMtx1, SiiLibMtx_t* pMtx2 )
{
	if( (pMtx1->Row != (pMtx2->Row-1)) || (pMtx1->Col != (pMtx2->Col-1)) )
	{
		//DEBUG_PRINT(MSG_ALWAYS, "ERROR: [%s] has the wrong size in order to be a minor of [%s].\n", pMtx1->pStr, pMtx2->pStr);
		SII_PLATFORM_DEBUG_ASSERT(0);
	}
}
#endif

static void sCheckSameSize( SiiLibMtx_t* pMtx1, SiiLibMtx_t* pMtx2 )
{
	if( (pMtx1->Row != pMtx2->Row) || (pMtx1->Col != pMtx2->Col) )
	{
		//DEBUG_PRINT(MSG_ALWAYS, "ERROR: [%s] and [%s] are not the same size.\n", pMtx1->pStr, pMtx2->pStr);
		SII_PLATFORM_DEBUG_ASSERT(0);
	}
}

static void sCheckMultiply( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS1, SiiLibMtx_t* pMtxS2 )
{
	if( pMtxS1->Col != pMtxS2->Row )
	{
		//DEBUG_PRINT(MSG_ALWAYS, "ERROR: [%s] can not be multiplied by [%s].\n", pMtxS1->pStr, pMtxS2->pStr);
		SII_PLATFORM_DEBUG_ASSERT(0);
	}
	else if( (pMtxD->Col != pMtxS2->Col) || (pMtxD->Row != pMtxS1->Row) )
	{
		//DEBUG_PRINT(MSG_ALWAYS, "ERROR: Result of multiplication doesn't fit into [%s].\n", pMtxD->pStr);
		SII_PLATFORM_DEBUG_ASSERT(0);
	}
}

/***** end of file ***********************************************************/
