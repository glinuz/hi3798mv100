#ifndef __SE_HAL_H
#define __SE_HAL_H

/*
 * reader_id: SE read_id(no used now)
 * atr: SE's atr buffer
 * atr_len: atr buffer's len
 *
 * return: 0 - success
 *         <0 - fails
 */
int scard_connect(int reader_id, void *atr, unsigned int *atr_len);

/*
 * reader_id: SE read_id(no used now)
 *
 * return: 0 - success
 *         <0 - fails
 */
int scard_disconnect(int reader_id);

/*
 * reader_id: SE read_id(no used now)
 * cmd: transfered command to SE
 * cmd_len: command's len
 * rsp: response buffer of this command
 * rsp: response buffer's len
 *
 * return: 0 - success
 *         <0 - fails
 */
int scard_transmit(int reader_id, unsigned char *cmd, unsigned int cmd_len,
		   unsigned char *rsp, unsigned int *rsp_len);

#endif
