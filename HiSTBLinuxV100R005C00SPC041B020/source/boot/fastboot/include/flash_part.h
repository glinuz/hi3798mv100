/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*
******************************************************************************/

#ifndef FLASH_PARTH
#define FLASH_PARTH
/******************************************************************************/

int find_flash_part(char *mtdparts, char *media_name, char *ptn_name,
                    uint64_t *start, uint64_t *length);
int flash_part_init(char *cmdline_string);

/******************************************************************************/
#endif /* FLASH_PARTH */



