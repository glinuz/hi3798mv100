/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.2.26
 *
******************************************************************************/

#ifndef RULE_ATTR_H
#define RULE_ATTR_H

#ifndef __USE_FILE_OFFSET64
int ra_init_file(const char *rulefile, FILE *flwarning, FILE *flattrpath,
		 FILE *flnode, FILE *flmatch);

int ra_init_file_debuglevel(const char *rulefile, int debuglevel);

int ra_init_data(const char *rulelist, int sz_rulelist, FILE *flwarning,
		 FILE *flattrpath, FILE *flnode, FILE *flmatch);

int ra_free(void);

int ra_sys_lstat(const char *path, struct stat *buf);

int ra_set_rootdir(const char *path);

int ra_host_attr(const char *path, void **attr);

int ra_target_attr(const char *path, int sz_path, void **attr);

int ra_attr_cap(void *attr, void *data);

int ra_attr_stat(void *attr, struct stat *buf);

int ra_attr_stat_cap(void *attr, short *gid, short *uid, unsigned short *mode,
		     unsigned int permitted[2]);

int ra_target_stat(const char *path, int sz_path, struct stat *buf);
#else
#define ra_init_file                    ra_init_file_offset64
#define ra_init_file_debuglevel         ra_init_file_debuglevel_offset64
#define ra_init_data                    ra_init_data_offset64
#define ra_free                         ra_free_offset64
#define ra_sys_lstat                    ra_sys_lstat_offset64
#define ra_set_rootdir                  ra_set_rootdir_offset64
#define ra_host_attr                    ra_host_attr_offset64
#define ra_target_attr                  ra_target_attr_offset64
#define ra_attr_cap                     ra_attr_cap_offset64
#define ra_attr_stat                    ra_attr_stat_offset64
#define ra_attr_stat_cap                ra_attr_stat_cap_offset64
#define ra_target_stat                  ra_target_stat_offset64
#endif

#endif /* RULE_ATTR_H */
