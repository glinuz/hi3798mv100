/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*
******************************************************************************/

#ifndef USB_LOGIFH
#define USB_LOGIFH
/******************************************************************************/

int usb_detect(void);

long usb_get_file_size(const char *filename);

int usb_file_check(const char *filename);

long usb_file_read(const char *filename, void *buffer, 
		   unsigned long pos, unsigned long length);

int usb_set_negotiation_timeout(int timeout);

/******************************************************************************/
#endif /* USB_LOGIFH */
