#include "vfmw_osal.h"

#define VFMW_MAX_PROC_NAME_SIZE 16

typedef int (*vfmw_proc_read_fn)(struct seq_file *, void *);
typedef int (*vfmw_proc_write_fn)(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

typedef struct
{
    HI_CHAR proc_name[VFMW_MAX_PROC_NAME_SIZE];

    vfmw_proc_read_fn  read;
    vfmw_proc_write_fn write;
} vfmw_proc_func;

void VID_STD_Enum2Str(VID_STD_E eVidStd, char *strVidStd);

int vfmw_read_proc_info(struct seq_file *p, void *v);
int vfmw_read_proc(struct seq_file *p, void *v);
int vfmw_read_proc_dbg(struct seq_file *p, void *v);
int vfmw_read_proc_prn(struct seq_file *p, void *v);
int vfmw_read_proc_scd(struct seq_file *p, void *v);
int vfmw_read_proc_chn(struct seq_file *p, void *v);
int vfmw_read_proc_dec(struct seq_file *p, void *v);

ssize_t vfmw_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

#ifdef VFMW_DNR_SUPPORT
int vfmw_read_proc_dnr(struct seq_file *p, void *v);
#endif

#ifdef VFMW_BTL_SUPPORT
int vfmw_read_proc_btl(struct seq_file *p, void *v);
#endif

#ifdef VFMW_SCD_LOWDLY_SUPPORT
int vfmw_read_proc_lwd(struct seq_file *p, void *v);
#endif

int str2val(char *str, unsigned int *data);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
int vfmw_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data);
#else
int vfmw_write_proc(struct file *file, const char __user *buffer, size_t count, loff_t *pos);
#endif

int vfmw_proc_open(struct inode *inode, struct file *file);

HI_S32 vfmw_proc_create(HI_CHAR *proc_name, vfmw_proc_read_fn read, vfmw_proc_write_fn write);
HI_VOID vfmw_proc_destroy(HI_CHAR *proc_name);

int  vfmw_proc_init(void);
void vfmw_proc_exit(void);

static const struct file_operations vfmw_proc_fops =
{
    .owner   = THIS_MODULE,
    .open    = vfmw_proc_open,
    .read    = seq_read,
    .write   = vfmw_proc_write,
    .llseek  = seq_lseek,
    .release = single_release,
};

