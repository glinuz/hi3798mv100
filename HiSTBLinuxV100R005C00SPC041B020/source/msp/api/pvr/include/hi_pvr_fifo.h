/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_fifo.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/24
  Description   :
  History       :
  1.Date        : 2008/04/24
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_PVR_H__
#define __HI_PVR_H__

#include "hi_type.h"

#if HI_OS_TYPE == HI_OS_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#elif HI_OS_TYPE == HI_OS_WIN32
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#endif


#include <fcntl.h>

#include "hi_type.h"
#include "hi_drv_pvr.h"
#include "hi_pvr_priv.h"
//#define PVR_USE_FILE_CACHE          /* enable file cache */
//#undef PVR_USE_FILE_CACHE

#ifdef PVR_USE_FILE_CACHE
#include "hi_pvr_file_cache.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */


#if HI_OS_TYPE == HI_OS_LINUX

#if 0
#define PVR_FOPEN_MODE_INDEX_BOTH   (O_CREAT | O_RDWR| O_LARGEFILE /*| O_APPEND*/ )
#define PVR_FOPEN_MODE_INDEX_WRITE  (O_CREAT | O_WRONLY| O_LARGEFILE /*| O_APPEND*/ )
#define PVR_FOPEN_MODE_INDEX_READ   (O_RDONLY | O_LARGEFILE )
#else
#define PVR_FOPEN_MODE_INDEX_BOTH   (O_CREAT | O_RDWR|  O_LARGEFILE/*| O_APPEND*/ )
#define PVR_FOPEN_MODE_INDEX_WRITE  (O_CREAT | O_RDWR| O_LARGEFILE /*| O_APPEND*/ )
#define PVR_FOPEN_MODE_INDEX_READ   (O_RDONLY  | O_LARGEFILE )
#endif

#define PVR_FOPEN_MODE_DATA_WRITE_ALL_TS   (O_CREAT | O_RDWR | O_LARGEFILE /*| O_APPEND*/ )

#ifdef HI_PVR_FIFO_DIO
#define PVR_FOPEN_MODE_DATA_BOTH    (O_CREAT | O_RDWR | O_DIRECT | O_LARGEFILE)
#define PVR_FOPEN_MODE_DATA_WRITE   (O_CREAT | O_WRONLY | O_DIRECT | O_LARGEFILE)
#define PVR_FOPEN_MODE_DATA_READ    (O_RDONLY | O_LARGEFILE)
#define PVR_DIO_ALIGN_SIZE          0x200
#else
#if 0
#define PVR_FOPEN_MODE_DATA_BOTH    (O_CREAT | O_RDWR | O_LARGEFILE /*| O_APPEND*/  )
#define PVR_FOPEN_MODE_DATA_WRITE   (O_CREAT | O_WRONLY | O_LARGEFILE /*| O_APPEND*/ )
#define PVR_FOPEN_MODE_DATA_READ    (O_RDONLY | O_LARGEFILE )
#else
#define PVR_FOPEN_MODE_DATA_BOTH    (O_CREAT | O_RDWR | O_LARGEFILE /*| O_APPEND*/  )
#define PVR_FOPEN_MODE_DATA_WRITE   (O_CREAT | O_RDWR | O_LARGEFILE /*| O_APPEND*/ )
#define PVR_FOPEN_MODE_DATA_READ    (O_RDWR  | O_LARGEFILE )
#endif
#endif

#elif HI_OS_TYPE == HI_OS_WIN32
typedef int ssize_t;
#define PVR_FOPEN_MODE_INDEX_BOTH   (O_CREAT | O_RDWR  | _O_BINARY)
#define PVR_FOPEN_MODE_INDEX_READ   (_O_RDONLY   | _O_BINARY)
#define PVR_FOPEN_MODE_DATA_BOTH    (O_CREAT | O_RDWR  | _O_BINARY)
#define PVR_FOPEN_MODE_DATA_READ    (_O_RDONLY   | _O_BINARY)
#endif


/* PVR file descriptor                                                      */
typedef int            PVR_FILE;
typedef int            PVR_FILE64;

/* invalid file description                                                 */
#define PVR_FILE_INVALID_FILE             (-1)

//#define PVR_CHECK_USB_FILE      /* enable USB file */
//#undef PVR_CHECK_USB_FILE

#ifdef SUPPORT_HIMOUNT
#undef PVR_CHECK_USB_FILE
#endif

#ifdef PVR_CHECK_USB_FILE
#define PVR_CHECK_USB_FILE_ALL  /*  enable force to check all file */
#undef PVR_CHECK_USB_FILE_ALL
extern HI_S32 PVR_CheckSingleUsbFile(const HI_CHAR * pszFileName);
extern HI_S32 PVR_CheckLastUsbFile(const HI_CHAR * pszFileName, HI_BOOL bCheck);
extern HI_S32 PVR_CheckUsbStillPlugIn(HI_VOID);
#endif

#if HI_OS_TYPE == HI_OS_LINUX

#ifndef _PVR_USE_MEM_FILE

//#define PVR_SET_MAXFILE_SIZE(maxSize)

#ifdef SUPPORT_HIMOUNT
extern int himount(const char*, const char*);
extern int hiunmount(const char* );
#endif



#ifndef PVR_USE_FILE_CACHE



#define pvr_open                            open
#define pvr_close                           close

#ifdef ANDROID
#define pvr_pread                           pread64
#define pvr_pwrite                          pwrite64
#define pvr_lseek                           lseek64
#else
#define pvr_pread                           pread
#define pvr_pwrite                          pwrite
#define pvr_lseek                           lseek
#endif
#define pvr_read                            read
#define pvr_write                           write
#define pvr_remove                          unlink
#define pvr_fsync                           fsync
#define pvr_truncate                        truncate
#ifdef SUPPORT_HIMOUNT
#define pvr_mount                           himount
#define pvr_umount                          hiunmount
#else
#define pvr_mount                           mount
#define pvr_umount                          umount
#endif
#define pvr_FILE                            FILE
#define pvr_fopen                           fopen
#define pvr_fclose                          fclose
#define pvr_fread                           fread
#define pvr_fwrite                          fwrite
#define pvr_fseek                           fseek
#define pvr_ftell                           ftell
#define pvr_fflush                          fflush
#define pvr_feof                            feof
#define pvr_ferror                          ferror



#define PVR_FC_open         pvr_open
#define PVR_FC_close        pvr_close
#define PVR_FC_pread        pvr_pread
#define PVR_FC_read         pvr_read
#define PVR_FC_pwrite       pvr_pwrite
#define PVR_FC_write        pvr_write
#define PVR_FC_lseek        pvr_lseek
#define PVR_FC_remove       pvr_remove
#define PVR_FC_fsync        pvr_fsync
#define PVR_FC_truncate     pvr_truncate
#define PVR_FC_fopen        pvr_fopen
#define PVR_FC_fclose       pvr_fclose
#define PVR_FC_fread        pvr_fread
#define PVR_FC_fwrite       pvr_fwrite
#define PVR_FC_fseek        pvr_fseek
#define PVR_FC_ftell        pvr_ftell
#define PVR_FC_fflush       pvr_fflush
#define PVR_FC_feof         pvr_feof
#define PVR_FC_ferror       pvr_ferror

#endif  /* #ifndef PVR_USE_FILE_CACHE */


/*-----------------------------
    filesystemtype:
        "vfat"
        "ntfs"
        "hifat"
        "hifat+ntfs"
------------------------------*/

#ifdef PVR_CHECK_USB_FILE_ALL
#define PVR_OPEN(filename, mode)            (PVR_CheckSingleUsbFile(filename),PVR_FC_open((filename), (mode), 0777))
#else
#define PVR_OPEN(filename, mode)            PVR_FC_open(filename, mode, 0777)
#endif
#ifdef ANDROID
#define PVR_READ(pMem, size, file, offset)  PVR_FC_pread(file, (void *)pMem, (size_t)(size), (off64_t)(offset))
#define PVR_WRITE(pMem, size, file, offset) PVR_FC_pwrite(file, pMem, size, (off64_t)(offset))
#define PVR_SEEK(file, offset, whence)      PVR_FC_lseek(file, (off64_t)(offset), whence)
#else
#define PVR_READ(pMem, size, file, offset)  PVR_FC_pread(file, (void *)pMem, (size_t)(size), (off_t)(offset))
#define PVR_WRITE(pMem, size, file, offset) PVR_FC_pwrite(file, pMem, size, (off_t)(offset))
#define PVR_SEEK(file, offset, whence)      PVR_FC_lseek(file, (offset), whence)
#endif
#define PVR_READ_C(file, pMem, size)        PVR_FC_read(file, (void *)(pMem), (size_t)(size))
#define PVR_WRITE_C(file, pMem, size)       PVR_FC_write(file, (void *)(pMem), (size_t)(size))
//#define PVR_SEEK64(file, offset, whence)    PVR_FC_lseek64(file, (HI_S64)(offset), whence)
#define PVR_CLOSE(file)                     PVR_FC_close(file)
#define PVR_REMOVE(pathname)                PVR_FC_remove(pathname)
#define PVR_FSYNC(file)                     PVR_FC_fsync(file)
#define PVR_TRUNCATE(path, len)             PVR_FC_truncate(path, len)


#define PVR_FS_FILE                         pvr_FILE
#define PVR_FS_FOPEN(path, mode)            PVR_FC_fopen((path), (mode))
#define PVR_FS_READ(ptr, size, nmemb, stream)   PVR_FC_fread((ptr), (size), (nmemb), (stream))
#define PVR_FS_WRITE(ptr, size, nmemb, stream)  PVR_FC_fwrite((ptr), (size), (nmemb), (stream))
#define PVR_FS_CLOSE(fp)                    PVR_FC_fclose(fp)
#define PVR_FS_SEEK(stream, offset, whence) PVR_FC_fseek((stream), (offset), (whence))
#define PVR_FS_TELL(stream)                 PVR_FC_ftell(stream)
#define PVR_FS_FFLUSH(stream)               PVR_FC_fflush(stream)
#define PVR_FS_EOF(stream)                  PVR_FC_feof(stream)
#define PVR_FS_ERROR(stream)                PVR_FC_ferror(stream)

#else   /* #ifndef _PVR_USE_MEM_FILE */

//#define PVR_SET_MAXFILE_SIZE(maxSize)   MemFile_SetMemSize((HI_U32)maxSize)
#define PVR_OPEN(filename, mode) MemFile_Open(filename, mode)
#define PVR_READ(pMem, size, file, offset) MemFile_Read(pMem, size, file, offset)
#define PVR_WRITE(pMem, size, file, offset) MemFile_Write(pMem, size, file, (HI_S32)offset)
#define PVR_SEEK(file, offset, whence) MemFile_Seek(file, offset, whence)
//#define PVR_SEEK64(file, offset, whence) MemFile_Seek(file, offset, whence)
#define PVR_CLOSE(file) MemFile_Close(file)
#define PVR_FSYNC(file)
#define PVR_MOUNT(source,target,filesystemtype,mountflags,data)
#define PVR_UMOUNT(target)

#endif  /* #ifndef _PVR_USE_MEM_FILE */

#elif HI_OS_TYPE == HI_OS_WIN32 /* #if HI_OS_TYPE == HI_OS_LINUX*/

#define PVR_SET_MAXFILE_SIZE(maxSize)
#define PVR_OPEN(filename, mode)          open(filename, mode, _S_IREAD | _S_IWRITE )

//extern ssize_t pread(PVR_FILE fd, HI_VOID *pBuf,  HI_U32 nBytes,  HI_S64 offset64);
//extern ssize_t pwrite(PVR_FILE fd, const HI_VOID *pBuf,  HI_U32 nBytes,  HI_S64 offset64);

#define PVR_READ(pMem, size, file, offset)  pread(file, pMem, (size_t)(size), (off64_t)(offset))
#define PVR_WRITE(pMem, size, file, offset) pwrite(file, pMem, (size_t)(size), (off64_t)(offset))
#define PVR_SEEK(file, offset, whence)      lseek(file, (HI_S32)(offset), whence)
//#define PVR_SEEK64(file, offset, whence)    _lseeki64(file, (HI_S64)(offset), whence)
#define PVR_CLOSE(file)                     close(file)
#define PVR_FSYNC(file)
#define PVR_MOUNT(source,target,filesystemtype,mountflags,data)
#define PVR_UMOUNT(target)

#endif /* HI_OS_TYPE == ... */

extern ssize_t PVR_READALL(void *buf, size_t count, PVR_FILE fd,  off64_t offset);


extern HI_BOOL PVR_CHECK_FILE_EXIST(const HI_CHAR *pszFileName);
extern HI_VOID PVR_REMOVE_FILE(const HI_CHAR *pszFileName);
extern HI_U64 PVR_FILE_GetFileSize(const HI_CHAR *pszFileName);
extern HI_VOID PVR_SET_MAXFILE_SIZE(PVR_FILE64 file, HI_U64 maxSize);


//////////////////////////////////////////////////////////////////
/****************************************************************/
/*                PVR FILE for >4G file @ FAT32                 */
/****************************************************************/
//////////////////////////////////////////////////////////////////
#define PVR_FILE_MAX_FILE          10 /* 5 for read + 5 for write */

#define PVR_FILE_FD_BASE           100

#define PVRFileGetPVRFd(fd)  (fd - PVR_FILE_FD_BASE)


#define PVRFileCheckPVRFd(pvrFd)  do\
    {\
        if ((HI_U32)pvrFd >= PVR_FILE_MAX_FILE)\
        {\
            return -1; \
        }\
    }while(0)

#define PVRFileCheckPVRFdOpen(pvrFd)  do\
    {\
        if (g_stPvrFiles[pvrFd].bOpened != HI_TRUE)\
        {\
            return -1; \
        }\
    }while(0)



extern HI_BOOL PVR_CHECK_FILE_EXIST64(const HI_CHAR *pszFileName);
extern HI_VOID PVR_REMOVE_FILE64(const HI_CHAR *pszFileName);
extern HI_U64  PVR_FILE_GetFileSize64(const HI_CHAR *pszFileName);

extern PVR_FILE64  PVR_OPEN64(const HI_CHAR *filename, int mode);
extern HI_S32  PVR_CLOSE64(PVR_FILE64 file);
extern HI_S32  PVR_FSYNC64(PVR_FILE64 file);
extern ssize_t PVR_PREAD64(HI_U8 *pMem, HI_U32 size, PVR_FILE64 file, HI_U64 offset);

extern ssize_t PVR_PWRITE64(const void *pMem,  HI_U32 size, PVR_FILE64 file, HI_U64 offset);



HI_S32 PVRFileGetOffsetFName(PVR_FILE64 file, HI_U64 offset, HI_CHAR *pRealName);

#if 0 /*obsolete APIs*/
extern int PVR_Mount(const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data);
extern int PVR_Umount(const char *target);
extern ssize_t PVR_WRITEALL(const void *buf, size_t count, PVR_FILE fd, off64_t offset);
extern ssize_t PVR_TS_PREAD64(HI_U8 *pMem, HI_U32 size, PVR_FILE64 file, HI_U64 offset);
extern HI_S32  PVR_GetTsFileFd(PVR_FILE64 file);
extern ssize_t PVR_READ64(HI_U8 *pMem, HI_U32 size, PVR_FILE64 file);
extern ssize_t PVR_WRITE64(const void *pMem,  HI_U32 size, PVR_FILE64 file);
extern ssize_t PVR_SEEK64(PVR_FILE64 file, HI_S64 offset, int whence);
HI_S32 PVRFileGetRealOffset(PVR_FILE64 file, HI_U64 *offset, HI_U64 *pu64OffsetLen);
#define PVR_MOUNT(source,target,filesystemtype,mountflags,data) PVR_Mount(source,target,filesystemtype,mountflags,data)
#define PVR_UMOUNT(target)                  PVR_Umount(target)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifdef __HI_PVR_H__ */

