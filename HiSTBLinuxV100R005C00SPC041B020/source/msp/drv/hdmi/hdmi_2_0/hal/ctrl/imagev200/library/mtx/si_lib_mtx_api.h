/*************************************************************************
* si_lib_mtx_api.h
*************************************************************************/
#ifndef __SI_LIB_MTX_API_H__
#define __SI_LIB_MTX_API_H__

#include "si_datatypes.h"

/************************************************************************/
/*  Type Definitions                                                    */
/************************************************************************/
typedef uint8_t SiiLibMtxIdx_t;

typedef struct
{
	SiiLibMtxIdx_t  Row;
	SiiLibMtxIdx_t  Col;
	const char*     pStr;
	float*          pArr;
} SiiLibMtx_t;

/************************************************************************/
/*	Function Prototype                                                  */
/************************************************************************/
SiiLibMtx_t* SiiLibMtxCreate( SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, const char* pStr );
void         SiiLibMtxDelete( SiiLibMtx_t* pMtx );
void         SiiLibMtxLoad( SiiLibMtx_t* pMtx, const float pArr[] );
void         SiiLibMtxCopy( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxSetCoef( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, float Value );
float        SiiLibMtxGetCoef( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col );
#if 0
void         SiiLibMtxZero( SiiLibMtx_t* pMtx );
void         SiiLibMtxIdentity( SiiLibMtx_t* pMtx );
void         SiiLibMtxInverse( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxTranspose( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxMinor( SiiLibMtx_t* pMtxD, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col, SiiLibMtx_t* pMtxS );
float        SiiLibMtxCofactor( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, SiiLibMtxIdx_t Col );
void         SiiLibMtxCofactors( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxAdjugate( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxScaler( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS, float fScaler );
float        SiiLibMtxDeterminant( SiiLibMtx_t* pMtx );
void         SiiLibMtxAdd( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
#endif
void         SiiLibMtxMultiply( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS1, SiiLibMtx_t* pMtxS2 );
#if 0
void         SiiLibMtxClip( SiiLibMtx_t* pMtx, float fHigh, float fLow );
void         SiiLibMtxCrossProduct( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS1, SiiLibMtx_t* pMtxS2 ); /* Also known as Vector Product */
void         SiiLibMtxDump( SiiLibMtx_t* pMtx );

void         SiiLibMtxXYZ2xyz( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxVect2Diag( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxDiag2Vect( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxXYZ2xyY( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxxyY2XYZ( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS );
void         SiiLibMtxGamma( SiiLibMtx_t* pMtxD, SiiLibMtx_t* pMtxS, float Gamma );
void         SiiLibMtxScaleRow( SiiLibMtx_t* pMtx, SiiLibMtxIdx_t Row, float fScaler );
#endif
#endif /* __SI_LIB_MTX_API_H__ */
