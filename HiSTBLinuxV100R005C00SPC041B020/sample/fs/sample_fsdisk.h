#ifndef  _SAMPLE_FSDISK_H_
#define  _SAMPLE_FSDISK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum length of path name */
#define MFS_MAXIMUM_PATH (256)

#define MFS_MAX_LONG_NAME_LEN (256)

/* File open type */
#define MFS_OPEN_FOR_READ (0)
#define MFS_OPEN_FOR_WRITE (1)

/* Seek options */
#define MFS_SEEK_BEGIN (0)
#define MFS_SEEK_END (1)
#define MFS_SEEK_FORWARD (2)
#define MFS_SEEK_BACK (3)

/* File system API return values.  */
#define MFS_SUCCESS (0x00)
#define MFS_ERROR (-1)

typedef struct MFS_DIRENTRY_INFO_STRUCT
{
    HI_U32 attributes; // Directory entry attributes.
    HI_U32 size;    // Size in bytes.

    HI_U32 year;
    HI_U32 month;
    HI_U32 day;
    HI_U32 hour;
    HI_U32 minute;
    HI_U32 second;

    HI_CHAR*  fxDirLongName;
    HI_CHAR*  fxDirLongFileDoubleName;
} MFS_DIRENTRY_INFO, *MFS_DIRENTRY_INFO_PTR;

HI_S32 FS_SystemDateSet        (HI_U32 year, HI_U32 month, HI_U32 day);
HI_S32 FS_SystemTimeSet        (HI_U32 hour, HI_U32 minute, HI_U32 second);
HI_S32 FS_SystemDateGet        (HI_U32 *year, HI_U32 *month, HI_U32 *day);
HI_S32 FS_SystemTimeGet        (HI_U32 *hour, HI_U32 *minute, HI_U32 *second);

HI_S32 FS_DirAttributesRead    (HI_CHAR *dirName, HI_U32 *attributes);
HI_S32 FS_DirAttributesSet     (HI_CHAR *dirName, HI_U32 attributes);
HI_S32 FS_DirCreate            (HI_CHAR *dirName);
HI_S32 FS_DirDelete            (HI_CHAR *dirName);
HI_S32 FS_DirRename            (HI_CHAR *oldDirName, HI_CHAR *newDirName);
HI_S32 FS_DirFirstEntryFind    (HI_CHAR *entryName);
HI_S32 FS_DirNextEntryFind     (HI_CHAR *entryName);
HI_S32 FS_DirNameTest          (HI_CHAR *dirName);
HI_S32 FS_DirInformationGet    (HI_CHAR *dirName, MFS_DIRENTRY_INFO *dirEntryInfo);
HI_S32 FS_DirDefaultSet        (HI_CHAR *newPathName);
HI_S32 FS_DirDefaultGet        (HI_CHAR **returnPathName);

HI_S32 FS_MediaFlush           (void);
HI_S32 FS_MediaSpaceAvailable  (HI_U32 *availableBytes);

HI_S32 FS_FileCreate           (HI_CHAR *fileName);
HI_S32 FS_FileDelete           (HI_CHAR *fileName);
HI_S32 FS_FileRename           (HI_CHAR *oldFileName, HI_CHAR *newFileName);
HI_S32 FS_FileAttributesSet    (HI_CHAR *fileName, HI_U32 attributes);
HI_S32 FS_FileAttributesRead   (HI_CHAR *fileName, HI_U32 *attributes);
HI_S32 FS_FileOpen             (HI_S32  *fileDesc, HI_CHAR *fileName, HI_U32 openType);
HI_S32 FS_FileClose            (HI_S32 fileDesc);
HI_S32 FS_FileRead             (HI_S32 fileDesc, void *buffer, HI_U32 requestSize, HI_U32 *actualSize);
HI_S32 FS_FileWrite            (HI_S32 fileDesc, void *buffer, HI_U32 size);
HI_S32 FS_FileRelativeSeek     (HI_S32 fileDesc, HI_U32 byteOffset, HI_U32 seekFrom);
HI_S32 FS_FileSeek             (HI_S32 fileDesc, HI_U32 byteOffset);
HI_S32 FS_FileTruncate         (HI_S32 fileDesc, HI_U32 size);

HI_S32    test_diskFs_list(void);

#ifdef __cplusplus
}
#endif

#endif
