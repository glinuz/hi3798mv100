/*
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


///
/// @file
/// @brief Defines common macros, functions and data structures for all Availink products. This file also declare functions for ensuring the SDK can work in both little endian and big endian target platforms.
/// 
//$Revision: 255 $ 
//$Date: 2008-02-06 10:37:01 -0500 (Wed, 06 Feb 2008) $
// 
#ifndef avl_h_h
	#define avl_h_h

	#include "bspdatadef.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	#define AVL_min(x,y) (((x) < (y)) ? (x) : (y))
	#define AVL_max(x,y) (((x) < (y)) ? (y) : (x))
	#define AVL_floor(a) (((a) == (int)(a))? ((int)(a)) : (((a) < 0)? ((int)((a)-1)) : ((int)(a))))
	#define AVL_ceil(a)  (((a) == (int)(a))? ((int)(a)) : (((a) < 0)? ((int)(a)) : ((int)((a)+1))))
	#define AVL_abs(a)  (((a)>0) ? (a) : (-(a)))
	#define AVL_abssub(a,b) ((a>=b)?(a-b):(b-a))
	#define AVL_CONSTANT_10_TO_THE_9TH 1000000000

	#ifdef AVL_CPLUSPLUS
		#define AVL_NULL    0
	#else
		#define AVL_NULL    ((void *)0)
	#endif

	/// Availink's version structure.
	struct AVL_VerInfo {
		AVL_uchar m_Major;	///< The major version
		AVL_uchar m_Minor;	///< The minor version
		AVL_uint16 m_Build;	///< The build version
	};

	/// Stores an unsigned 64-bit integer
	struct AVL_uint64
	{
		AVL_uint32 m_HighWord;	///< The most significant 32-bits of the unsigned 64-bit integer
		AVL_uint32 m_LowWord;	///< The least significant 32-bits of the unsigned 64-bit integer
	};

	/// Chunk two bytes \a uidata in to \a pBuff.
	/// 
	/// @param uidata The input 2 bytes data.
	/// @param pBuff The destination buffer, at least 2 bytes length.
	/// 
	/// @remarks This function is used to eliminates the big endian and little endian problem.
	void Chunk16(AVL_uint16 uidata, AVL_puchar pBuff);

	/// Composes a ::AVL_uint16 from two bytes in a AVL_uchar array.
	/// 
	/// @param pBuff  The buffer has at least 2 bytes data.
	/// 
	/// @return AVL_uint16
	/// @remarks This function is used to eliminates the big endian and little endian problem.
	AVL_uint16 DeChunk16(const AVL_puchar pBuff);

	/// Chunk four bytes \a uidata in to \a pBuff.
	/// 
	/// @param uidata The input 3 bytes data.
	/// @param pBuff The destination buffer, at least 3 bytes length.
	/// 
	/// @remarks This function is used to eliminates the big endian and little endian problem.
	void Chunk32(AVL_uint32 uidata, AVL_puchar pBuff);

	/// Composes a ::AVL_uint16 from four bytes in a AVL_uchar array.
	/// 
	/// @param pBuff  The buffer has at least 4 bytes data.
	/// 
	/// @return AVL_uint32
	/// @remarks This function is used to eliminates the big endian and little endian problem.
	AVL_uint32 DeChunk32(const AVL_puchar pBuff);

	/// Chunk 3 byte of \a uiaddr into the \a pBuff
	/// 
	/// @param uiaddr The address. Only the three LSB bytes will be used.
	/// @param pBuff The destination buffer, at lease three bytes length.
	/// 
	/// @remarks This function is used to eliminates the big endian and little endian problem.
	void ChunkAddr(AVL_uint32 uiaddr, AVL_puchar pBuff);

	/// Adds a 32-bit unsigned integer to a 64-bit unsigned integer.  Stores the result in a 64-bit unsigned integer.
	///
	/// @param pSum Contains the 64-bit addend.  Also carries back the resulting sum.
	/// @param uiAddend Contains the 32-bit addend.
	///
	/// @remarks This function is an 'internal' function. Availink does not recommend that the user call it directly.
	void Add32To64(struct AVL_uint64 *pSum, AVL_uint32 uiAddend);

	/// Divides two 64-bit unsigned integers.  Stores the result in a 64-bit unsigned integer.
	///
	/// @param divisor Contains the 64-bit divisor. 
	/// @param dividend Contains the 64-bit dividend.
	///
	/// @remarks This function is an 'internal' function. Availink does not recommend that the user call it directly.
	AVL_uint32 Divide64(struct AVL_uint64 divisor, struct AVL_uint64 dividend);

	/// Multiplies two 32-bit unsigned integers.  Stores the result in a 64-bit unsigned integer.
	///
	/// @param pDst Carries back the 64-bit product of the multiplication.
	/// @param m1 Contains one of the 32-bit factors to be used in the multiplication.
	/// @param m2 Contains the other 32-bit factor to be used in the multiplication.
	///
	/// @remarks This function is an 'internal' function. Availink does not recommend that the user call it directly.
	void Multiply32(struct AVL_uint64 *pDst, AVL_uint32 m1, AVL_uint32 m2);

	/// Shifts a 32-bit unsigned integer left by 16 bits and then adds the result to a 64-bit unsigned integer.  Stores the sum in a 64-bit unsigned integer.
	///
	/// @param pDst Contains the 64-bit addend.  Also carries back the resulting sum.
	/// @param a Contains the 32-bit input which is shifted and added to the other addend.
	///
	/// @remarks This function is an 'internal' function. Availink does not recommend that the user call it directly.
	void AddScaled32To64(struct AVL_uint64 *pDst, AVL_uint32 a);

	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
