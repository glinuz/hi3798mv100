#ifndef __TEE_FS_H
#define __TEE_FS_H

#include "tee_internal_api.h"

//CRYPTO_USE_SW_ENGINE for use sw_crypto_engine
//#define CRYPTO_USE_SW_ENGINE

/* Macros for access() */
#define R_OK  4     // Read
#define W_OK  2     // Write
#define F_OK  0     // Existence

#define TRANS_BUFF_SIZE AGENT_BUFF_SIZE    //transfer buffer size,should be equal to teecd's buffer size
#define HASH_LEN   32
/*DIR_LEN is for mutiple sec storage partition and dir,e.g. sec_storage/dirA/file1.txt*/
#define DIR_LEN 64
#define HASH_NAME_BUFF_LEN  (2*HASH_LEN + 1 + DIR_LEN)
#define FD_MAX  1024    //max opend files per process at same time
#define SFS_PARTITION_PERSISTENT "sec_storage/"
#define SFS_PARTITION_TRANSIENT "sec_storage_data/"
#define FILE_NAME_INVALID_STR "../"  //file name path must not contain ../

TEE_Result tee_fs_init(void *control);
void tee_fs_exit(void);

void dump_current_fd_map(void);
TEE_Result check_file_name(const char *name);

void fs_set_serr(uint32_t ns_errno);
TEE_Result fs_get_serr(void);

/*******************************************************************************
* Function    : pretend a filesystem opt "fopen"
* Description : null
* Input       : name - file name, include path
                flag - open mode
* Output      : open mode
* Return      : -1:opt failed, >0:file handler
*******************************************************************************/
int32_t fopen(const char *name, uint32_t flags);

/*******************************************************************************
* Function    : pretend a filesystem opt "fclose"
* Description : null
* Input       : file handler
* Output      : null
* Return      : -1:failed, =0:success
*******************************************************************************/
int32_t fclose(int32_t fd);

/*******************************************************************************
* Function    : pretend a filesystem opt "fread"
* Description : null
* Input       : size - read size
                fd - file handler
                error - 0:success, -1:error
* Output      : out_buf - buffer to store read file content
* Return      : number of element successfully read, less than count means error
                occurs
*******************************************************************************/
/*
 * Notice: total buffer is TRANS_BUFF_SIZE - sizeof(struct sec_storage_t),
 *         if write content is more than it, it need to write for N times.
 */
uint32_t fread(void *out_buf, uint32_t count, int32_t fd, int32_t *error);

/*******************************************************************************
* Function    : pretend a filesystem opt "fwrite"
* Description : null
* Input       : content - content buffer
                size - element size
                count - element count
                fd - file handler
* Output      : null
* Return      : number of element successfully write, less than count means error
                occurs
*******************************************************************************/
uint32_t fwrite(const void *content, uint32_t count, int32_t fd);

/*******************************************************************************
* Function    : pretend a filesystem opt "fseek"
* Description : null
* Input       : fd - file handler
                offset - bytes to the position specified by whence
                whence - SEEK_SET, SEEK_CUR, or SEEK_END, the offset is relative
                         to the start of the file, the current position indicator,
                         or end-of-file, respectively
* Output      : null
* Return      : -1:failed, =0:success
*******************************************************************************/
int32_t fseek(int32_t fd, int32_t offset, uint32_t whence);

/*******************************************************************************
* Function    : pretend a filesystem opt "fremove"
* Description : null
* Input       : file path
* Output      : null
* Return      : -1:failed, =0:success
*******************************************************************************/
int32_t fremove(const char *r_pth);

/*******************************************************************************
* Function    : pretend a filesystem opt "ftruncate"
* Description : null
* Input       : name - file name
              : len - new file length
* Output      : null
* Return      : -1:failed, =0:success
*******************************************************************************/
int32_t ftruncate(const char *name, uint32_t len);

/*******************************************************************************
* Function    : pretend a filesystem opt "frename"
* Description : null
* Input       : old_name - old file name
              : new_name - new file name
* Output      : null
* Return      : -1:failed, =0:success
*******************************************************************************/
int32_t frename(const char *old_name, const char *new_name);

/*******************************************************************************
* Function    : pretend a filesystem opt "fcreate"
* Description : null
* Input       : name - file name, include path
                flag - open mode
* Output      : null
* Return      : -1:opt failed, >0:file handler
*******************************************************************************/
int32_t fcreate(const char *name, uint32_t flag);

/*******************************************************************************
* Function    : pretend a filesystem opt "fcreate"
* Description : comfirm return value is success before use output value
* Input       : fd - file handler
* Output      : pos - current file pointer position
                len - file length
* Return      : -1:opt failed, >0:file handler
*******************************************************************************/
int32_t finfo(int32_t fd, uint32_t *pos, uint32_t *len);

/*******************************************************************************
* Function    : pretend a filesystem opt "access"
* Description : check if caller can access file with mode
* Input       : name - file name
* Output      : mode - access mode, R_OK:read, W_OK:write, F_OK:exist
* Return      : 0:access ok, <0:fail
*******************************************************************************/
int32_t faccess(const char *name, int mode);

/*******************************************************************************
* Function    : pretend a filesystem opt "access"
                difference from faccess: it can detect all path file
* Description : check if caller can access file with mode
* Input       : name - file path
* Output      : mode - access mode, R_OK:read, W_OK:write, F_OK:exist
* Return      : 0:access ok, <0:fail
*******************************************************************************/
int32_t faccess2(const char *name, int mode);

/*******************************************************************************
* Function    : pretend a filesystem opt "statfs"
* Description : get disk remain size of secure storage
* Input       : null
* Output      : secure_remain - disk remain size in kbytes of "/sec_storage",
                                can be NULL
                data_secure_remain - disk remain size in kbytes of
                                     "/data/sec_storage_data", can be NULL
* Return      : 0:opt success, <0:fail
*******************************************************************************/
int32_t TEE_EXT_DiskUsage(uint32_t *secure_remain, uint32_t *data_secure_remain);

int32_t fcopy(const char *from_path, const char *to_path);
int32_t fsync(int32_t fd);

uint32_t getFsAgentBufferMetaSize();
#endif
