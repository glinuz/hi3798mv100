/*
 * =====================================================================================
 *         Author:  zhanchonglong(zKF48694)
 *           Date:  03/27/2012
 *    Description:  The file is to use locolplay to play VOB or M2TS, so it can gapless play 
 *  Function List: The main function:
                    playM3U9Main
                    hasBrothersFile
                    hroughWholeDerectory
                    DvdBdGaplessPlay
                    putFilenameToM3U9File
                    createListPoint
                    insertFilenameToList
                    freeList
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#define HI_SUCCESS       0
#define HI_FAILURE       (-1)

struct filename_list
{
    char *pfile_name;
    char* pfile_path;
    int file_size;
    struct filename_list* next;
};
enum FLAGSTYPE
{
    FROM_FILE = 0,
    FROM_DRECTORY,
};
enum FORMATTYPE
{
    FORMAT_VOB = 0,
    FORMAT_M2TS,
};

static struct filename_list* createListPoint(char *filename, char * filepath)
{
    struct filename_list * list_point;
    list_point = (struct filename_list*) malloc(sizeof(struct filename_list));
    if(NULL == list_point)
    {
        printf("struct filename_list malloc failed!\n");
        return NULL;
    }
    memset(list_point, 0, sizeof(struct filename_list));
    list_point->file_size = strlen(filename) + 1; 
    list_point->pfile_name = (char *)malloc(list_point->file_size* sizeof(char));
    if(list_point->pfile_name == NULL)
    {
        printf("one filename to list  malloc failed\n");
        free(list_point);
        return NULL;
    }
    list_point->pfile_path = (char *)malloc((strlen(filepath) + 1)* sizeof(char));
    if(list_point->pfile_path == NULL)
    {
        printf("one filepath to list  malloc failed\n");
        free(list_point);
        free(list_point->pfile_name);
        return NULL;
    }
    memset(list_point->pfile_name, 0, list_point->file_size);
    memset(list_point->pfile_path, 0, strlen(filepath) + 1);

    memcpy(list_point->pfile_name, filename, strlen(filename));
    memcpy(list_point->pfile_path, filepath, strlen(filepath));
    list_point->next = NULL;
    return list_point;
}
static int insertFilenameToList(struct filename_list** header, char *filename, char* filepath,int flags)
{
    if(!filename)
    {
        printf("now you read input file is wrong!");
        return HI_FAILURE;
    }
    if(!(*header))
    {
        *header = createListPoint(filename, filepath);
        if(*header == NULL)
        {
            printf("createListPoint failed!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        struct filename_list * new_tmp = NULL; 
        struct filename_list * tmp1    = *header;
        struct filename_list * tmp2    = NULL;
        while(tmp1 && (memcmp(tmp1->pfile_name, filename, strlen(filename)) < 0))
        {
            tmp2 = tmp1;
            tmp1 = tmp1->next;
        }
        if( NULL == tmp1)
        {
            new_tmp = createListPoint(filename, filepath);
            if(!new_tmp)
            {
                printf(" [%d] create file list point failed\n",__LINE__);
                return HI_FAILURE;
            }
            tmp2->next = new_tmp; 
        }
        else if(tmp1 == *header)
        {
            if(flags == FROM_DRECTORY)
            {
                new_tmp = createListPoint(filename, filepath);
                if(!new_tmp)
                {
                    printf(" [%d] create file list point failed\n",__LINE__);
                    return HI_FAILURE;
                }
                new_tmp->next = tmp1;
                *header = new_tmp;
            }
        }
        else
        {
            new_tmp = createListPoint(filename, filepath);
            if(!new_tmp)
            {
                printf(" [%d] create file list point failed\n",__LINE__);
                return HI_FAILURE;
            }
            tmp2->next = new_tmp; 
            new_tmp->next = tmp1;
        }
    }
    return HI_SUCCESS;
}
static int freeList(struct filename_list** header)
{
    if(!*header)
    {
        printf("now your list is already free!\n");
        return HI_SUCCESS;
    }
    struct filename_list* tmp = *header;
    struct filename_list* p = NULL;

    while(tmp)
    {
        p = tmp->next;
        free(tmp->pfile_name);
        tmp->pfile_name = NULL;
        free(tmp->pfile_path);
        tmp->pfile_path = NULL;
        free(tmp);
        tmp = p;
    }
    p = NULL;
    *header = NULL;
    tmp = NULL;
    return HI_SUCCESS;

}
static int putFilenameToM3U9File(struct filename_list **header)
{
    //creat m3u9 file, and save the brother files in it 
    int fd;
    fd = open("/data/01.m3u9", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0777);
    if(-1 == fd)
    {
        printf("creat m3u9 file failed!");
        return HI_FAILURE;
    }
    
    const char * m3u9_header = "#HISIPLAY\n#HISIPLAY_START_GAPLESS\n";
    const char * m3u9_tail = "#HISIPLAY_ENDLIST\n";
    const char * stream_info = "#HISIPLAY_STREAM\n";
    struct filename_list* tmp = NULL;
    tmp = *header;

    write(fd, m3u9_header, strlen(m3u9_header));
    while(tmp)
    {
        //printf("tmp->pfile_path = %s\n",tmp->pfile_name);
        write(fd, stream_info, strlen(stream_info));
        write(fd, tmp->pfile_path, strlen(tmp->pfile_path));
        write(fd, "\n", 1);
        tmp=tmp->next;
    }
    write(fd, m3u9_tail, strlen(m3u9_tail));
    close(fd);
    return HI_SUCCESS;
}
static int DvdBdGaplessPlay(int formattype ,char * path_name,struct filename_list **header, int flags)
{
    //do dvd bd gapless play
    DIR* dp;
    struct dirent *dirp;
    regex_t reg;   
    int ret = 0;
    int num = 0;
    regmatch_t pm[3];   
    char cwd[2048];

    if(FORMAT_VOB == formattype )
    {
        const char* pattern="vts_[0-9][1-9]_[1-9].vob";
        ret = regcomp( &reg, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB );   
    }
    if(FORMAT_M2TS == formattype )
    {
        const char* pattern1="[0-9][0-9][0-9][0-9][0-9].m2ts";   
        ret = regcomp( &reg, pattern1, REG_EXTENDED | REG_ICASE | REG_NOSUB );
    }
    if(ret != 0)
    {
        printf("register regular expression failed\n");
        return HI_FAILURE;
    }

    getcwd(cwd, sizeof(cwd));
    if(NULL == (dp = opendir(path_name)))
    {
        printf("open dir failed!\n");
        regfree( &reg); 
        return HI_FAILURE;
        
    }
    chdir(path_name); 
    while((dirp = readdir(dp)) != NULL)
    {
        /*
         * Get all files in the directory, then judge the file name and path of
         * the file name is the brother file, to vob and m2ts
         * vts_01_1.VOB  vts_01_2.vob or vts_02_1.vob
         */
        struct stat stat_file;
        if(stat(dirp->d_name, &stat_file) < 0)
        {
            printf("[%d]get filename stat failed\n",__LINE__);
            regfree(&reg); 
            chdir("..");
            closedir(dp);
            chdir(cwd); 
            return HI_FAILURE;
        }
        //printf("dirp->d_name = %s\n",dirp->d_name);
        if(S_ISREG(stat_file.st_mode))
        {
            ret = regexec( &reg, dirp->d_name , 3, pm, 0);   
            if(0 == ret)
            {   
                char path_buf[1024] = {0};

                memset(path_buf, 0, 1024);
                if(!getcwd(path_buf, 1024))
                {
                    printf("getcwd failed\n");
                    chdir(cwd);
                    return HI_FAILURE;
                }

                strncat(path_buf, "/", 1);
                strncat(path_buf, dirp->d_name, strlen(dirp->d_name));
                //printf("path_buf = %s",path_buf);
                
                if( HI_FAILURE == insertFilenameToList(header,dirp->d_name,path_buf, flags))
                {
                    printf("[%s][%s][%d] failed\n",__FILE__,__func__,__LINE__);
                    regfree( &reg); 
                    chdir("..");
                    closedir(dp);
                    chdir(cwd);
                    return HI_FAILURE;
                }
                num ++;
                //printf("there is one match!\n");
            }
        }
    }
    chdir("..");
    if(0 == num)
    {
        //no match
        printf("there is no one match!\n");
        regfree( &reg); 
        closedir(dp);
        chdir(cwd);
        return HI_FAILURE;
    }
    regfree( &reg); 
    closedir(dp);
    chdir(cwd);
    return HI_SUCCESS; 
}
static int openDirectory(char * path, struct filename_list **header)
{
    DIR* dp;
    struct dirent *dirp;
    int num = 0;
    int ret = 0;
    if(NULL == (dp = opendir(path)))
    {
        printf("[%d]open dir failed!\n",__LINE__);
        return HI_FAILURE;
    }
    chdir(path); 
    while((dirp = readdir(dp)) != NULL)
    {
        struct stat stat_file;
        if(stat(dirp->d_name, &stat_file) < 0)
        {
            printf("[%d] get filename or derectory stat failed\n",__LINE__);
            chdir(".."); 
            closedir(dp);
            return HI_FAILURE;
        }
        if(S_ISDIR(stat_file.st_mode))
        {
            if(0 == strncasecmp(dirp->d_name,"VIDEO_TS", 8))
            {
                //do DVD VIDEO_TS gapless play
                ret = DvdBdGaplessPlay(FORMAT_VOB, dirp->d_name, header, FROM_DRECTORY);
                if(HI_FAILURE == ret)
                {
                    chdir(".."); 
                    closedir(dp);
                    return HI_FAILURE;
                }
                num ++;
            }
            if(0 == strncasecmp(dirp->d_name,"STREAM", 6))
            {
                //do BD'STREAM gapless play
                ret = DvdBdGaplessPlay(FORMAT_M2TS, dirp->d_name, header, FROM_DRECTORY);
                if(HI_FAILURE == ret)
                {
                    chdir(".."); 
                    closedir(dp);
                    return HI_FAILURE;
                }
                num++;
            }
            if(0 == strncasecmp(dirp->d_name, "BDMV", 4))
            {
                //in  STREAM  parent derectory gapless play
                if(HI_FAILURE == openDirectory(dirp->d_name, header))
                {
                    printf("[%s][%d][%s]failed\n",__FILE__,__LINE__,__func__);
                    chdir(".."); 
                    closedir(dp);
                    return HI_FAILURE;
                }
                num++;
            }
        }
    }
    chdir(".."); 
    if( 0 == num)
    {
        closedir(dp);
        return HI_FAILURE;
    }
    closedir(dp);
    return HI_SUCCESS;
}
static int throughWholeDerectory(char * path)
{
    // judge it's  DVD or BD derectory
    struct filename_list *header = NULL;
    char * p1 = NULL;
    int ret = 0;

    p1 = path + strlen(path);
    //judge the input derectory like /mnt/sda/sda1/VIDEO_TS/
    if(*(p1 - 1) == '/')
    {
        p1 = p1 - 2;
    }
    while(*p1 != '/')
    {
        p1--;
    }

    if(0 == strncasecmp(p1 + 1, "VIDEO_TS", 8))
    {
        //do DVD VIDEO_TS gapless play
        ret = DvdBdGaplessPlay(FORMAT_VOB, path, &header, FROM_DRECTORY);
        if(HI_FAILURE == ret)
        {
            printf("[%d] DvdBdGaplessPlay failed\n",__LINE__);
            return HI_FAILURE;
        }
    }
    else if(0 == strncasecmp(p1 + 1, "STREAM", 6))
    {
        //do BD STREAM gapless play
        //judge it's M2TS derectory
        ret = DvdBdGaplessPlay(FORMAT_M2TS, path, &header, FROM_DRECTORY);
        if(HI_FAILURE == ret)
        {
            printf("[%d] DvdBdGaplessPlay failed\n",__LINE__);
            return HI_FAILURE;
        }
    }
    else
    {
        if(HI_FAILURE== openDirectory(path, &header))
        {
            printf("[%s][%d][%s]failed\n",__FILE__,__LINE__,__func__);
            return HI_FAILURE;
        }
    }

    ret = putFilenameToM3U9File(&header);
    if(HI_FAILURE == ret)
    {
        printf("creat m3u9 file failed\n");
        freeList(&header);
        return HI_FAILURE;
    }
    freeList(&header);
    return HI_SUCCESS;
}

//like: /mnt/sda/sda1/VIDEO_TS/VTS_01_1.VOB
static int hasBrothersFile(char *path)
{
    char * p1 = NULL;
    char * p = NULL;
    char * path_name = NULL;

    // get the derectory where the file 
    path_name = (char*)malloc((strlen(path) + 1)*sizeof(char));
    if(NULL == path_name)
    {
        printf("malloc failed!");
        return HI_FAILURE;
    }
    memset(path_name, 0, strlen(path) + 1);

    p1 = strrchr(path, '/');
    if(NULL == p1)
    {
       printf("Find characters failed!\n");
       free(path_name); 
       return HI_FAILURE;
    }

    p = strrchr(path, '.');
    if(NULL == p)
    {
       printf("Find characters failed!\n");
       free(path_name); 
       return HI_FAILURE;
    }
    strncpy(path_name, path, p1 - path);

    struct filename_list *header = NULL;
    if( HI_FAILURE == insertFilenameToList(&header, p1 + 1 ,path, FROM_FILE))
    {
        printf("[%s][%s %d]failed!\n",__FILE__,__func__,__LINE__);
        free(path_name); 
        freeList(&header);
        return HI_FAILURE;
    }
    if( 0 == strncasecmp( p + 1, "vob", 3))
    {
        DvdBdGaplessPlay(FORMAT_VOB,path_name,&header,FROM_FILE);
    }
    else if(0 == strncasecmp( p + 1, "m2ts", 4))
    {
        DvdBdGaplessPlay(FORMAT_M2TS,path_name,&header,FROM_FILE);
    }
    else
    {
        //printf("Now this format is  not surport!\n");
        free(path_name); 
        freeList(&header);
        return HI_FAILURE;
    }
    
    //if brother file , creat m3u9 file, and save the brother files in it, successfully backto Main 
    int ret = 0;
    ret = putFilenameToM3U9File(&header);
    if(HI_FAILURE == ret)
    {
        printf("Create m3u9 file failed\n");
        free(path_name); 
        freeList(&header);
        return HI_FAILURE;
    }
    free(path_name); 
    freeList(&header);
    return HI_SUCCESS;
}
int playM3U9Main(char * path)
{
    //judge your input is file or derectory ?
    struct stat stat_file;
    if(stat(path, &stat_file) < 0)
    {
        //printf("playM3U9Main stat wrong!\n");
        return HI_FAILURE;
    }
    
    if(S_ISREG(stat_file.st_mode))
    {
        //go to regulater
        //printf("regulator\n");
        //if file  open the file located derectory
        if(HI_FAILURE == hasBrothersFile(path))
        {
            return HI_FAILURE;
            //do something
        }
    }
    if(S_ISDIR(stat_file.st_mode))
    {
        //go to directory 
        //printf("directory\n");
        if(HI_FAILURE == throughWholeDerectory(path))
        {
            return HI_FAILURE;
            printf("failed\n");
        }

    }
    return HI_SUCCESS;
}
