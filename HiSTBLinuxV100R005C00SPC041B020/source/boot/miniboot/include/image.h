/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "compile.h"

/*
 * CPU Architecture Codes (supported by Linux)
 */
#define IH_ARCH_INVALID		0	/* Invalid CPU	*/
#define IH_ARCH_ALPHA		1	/* Alpha	*/
#define IH_ARCH_ARM		2	/* ARM		*/
#define IH_ARCH_ARM64		22	/* ARM64	*/


#define IH_NMLEN		32	/* Image Name Length		*/
/*
 * Legacy format image header,
 * all data in network byte order (aka natural aka bigendian).
 */
typedef struct image_header {
	uint32_t	ih_magic;	/* Image Header Magic Number	*/
	uint32_t	ih_hcrc;	/* Image Header CRC Checksum	*/
	uint32_t	ih_time;	/* Image Creation Timestamp	*/
	uint32_t	ih_size;	/* Image Data Size		*/
	uint32_t	ih_load;	/* Data	 Load  Address		*/
	uint32_t	ih_ep;		/* Entry Point Address		*/
	uint32_t	ih_dcrc;	/* Image Data CRC Checksum	*/
	uint8_t		ih_os;		/* Operating System		*/
	uint8_t		ih_arch;	/* CPU architecture		*/
	uint8_t		ih_type;	/* Image Type			*/
	uint8_t		ih_comp;	/* Compression Type		*/
	uint8_t		ih_name[IH_NMLEN];	/* Image Name		*/
} image_header_t;

/*******************************************************************/
/* Legacy format specific code (prefixed with image_) */
/*******************************************************************/
static inline uint32_t get_image_header_size (void)
{
	return (sizeof (image_header_t));
}

#define image_get_hdr_l(f) \
	static inline uint32_t image_get_##f(const image_header_t *hdr) \
	{ \
		return SWAP32(hdr->ih_##f); \
	}
image_get_hdr_l (magic);	/* image_get_magic */
image_get_hdr_l (hcrc);		/* image_get_hcrc */
image_get_hdr_l (time);		/* image_get_time */
image_get_hdr_l (size);		/* image_get_size */
image_get_hdr_l (load);		/* image_get_load */
image_get_hdr_l (ep);		/* image_get_ep */
image_get_hdr_l (dcrc);		/* image_get_dcrc */

#define image_get_hdr_b(f) \
	static inline uint8_t image_get_##f(const image_header_t *hdr) \
	{ \
		return hdr->ih_##f; \
	}
image_get_hdr_b (os);		/* image_get_os */
image_get_hdr_b (arch);		/* image_get_arch */
image_get_hdr_b (type);		/* image_get_type */
image_get_hdr_b (comp);		/* image_get_comp */

static inline char *image_get_name (const image_header_t *hdr)
{
	return (char *)hdr->ih_name;
}

static inline uint32_t image_get_data_size (const image_header_t *hdr)
{
	return image_get_size (hdr);
}

/**
 * image_get_data - get image payload start address
 * @hdr: image header
 *
 * image_get_data() returns address of the image payload. For single
 * component images it is image data start. For multi component
 * images it points to the null terminated table of sub-images sizes.
 *
 * returns:
 *     image payload data start address
 */
 
static inline int check_image_arch (const image_header_t *hdr, uint8_t arch)
{
	return (hdr->ih_arch == arch) ? 0 : -1;
}

static inline unsigned long get_image_data (const image_header_t *hdr)
{
	return ((unsigned long)hdr + get_image_header_size());
}

static inline unsigned long get_image_ep (const image_header_t *hdr)
{
	return SWAP32(hdr->ih_ep);
}

static inline uint32_t get_image_size (const image_header_t *hdr)
{
	return (SWAP32(hdr->ih_size) + get_image_header_size ());
}

static inline unsigned long get_image_end (const image_header_t *hdr)
{
	return ((unsigned long)hdr + get_image_size (hdr));
}

#endif	/* __IMAGE_H__ */
