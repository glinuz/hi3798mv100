#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/smp.h>
#include <linux/kthread.h>
#include <linux/mutex.h>

//#define TEEC_DEBUG
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_type.h"
#include "tzDriver.h"
#include "tc_ns_client.h"
//#include "sec_mmz.h"

/*
#define SMC_ID(typ,svc,fun)                     \
    (uint32_t)( ((typ << SMC_TP_SHIFT) & SMC_TP_MASK) | \
                ((svc << SVC_ID_SHIFT) & SVC_ID_MASK) | \
                ((fun << FUN_ID_SHIFT) & FUN_ID_MASK)   \
               )
*/
#define SMC_ID(typ,svc,fun)                 3

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

static DEFINE_MUTEX(scm_lock);
sRegSMC_t sSMCArgs;


static void sendSMC(sRegSMC_t* psArgs)
{
//  PUSH  {r4-r7}
//  LDMIA r0, {r0-r7}
//  POP   {r4-r7}
}

struct otz_smc_cdata {
        int cmd_addr;
        int ret_val;
};
static struct otz_smc_cdata otz_smc_cd[NR_CPUS];
 u32 smc(u32 cmd_addr)
{
    int context_id;
    register u32 r0 asm("r0") = 1;
    register u32 r1 asm("r1") = (u32)&context_id;
    register u32 r2 asm("r2") = cmd_addr;
    do {
        asm volatile(
            __asmeq("%0", "r0")
            __asmeq("%1", "r0")
            __asmeq("%2", "r1")
            __asmeq("%3", "r2")
 #if GCC_VERSION >= 40600//for hisi linux:by fangjian00208632
            ".arch_extension sec\n"
 #endif
            "smc    #0  @ switch to secure world\n"
            : "=r" (r0)
            : "r" (r0), "r" (r1), "r" (r2)
            : "r3");
    } while (r0 == 1);
    return r0;
}

/*主设备和从设备号变量*/
static int tz_major = 0;
static int tz_minor = 0;

/*设备类别和设备变量*/
static struct class * tz_class = NULL;
static struct tz_dev* tz_dev = NULL;

/*传统的设备文件操作方法*/
static int     tz_open(struct inode* inode, struct file* filp);
static int     tz_release(struct inode* inode, struct file* filp);
static ssize_t tz_read(struct file* filp, char __user *buf, size_t count, loff_t* f_pos);
static ssize_t tz_write(struct file* filp, const char __user *buf, size_t count, loff_t* f_pos);

/*设备文件操作方法表*/
static struct file_operations tz_fops = {
    .owner   = THIS_MODULE,
    .open    = tz_open,
    .release = tz_release,
    .read    = tz_read,
    .write   = tz_write,
};


/*访问设置属性方法*/
static ssize_t tz_val_show (struct device* dev, struct device_attribute* attr, char* buf);
static ssize_t tz_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count);

/*定义设备属性*/
static DEVICE_ATTR(val, S_IRUGO | S_IWUSR, tz_val_show, tz_val_store);

/*定义传统的设备文件访问方法，主要是定义tz_open、tz_release、tz_read 和tz_write 这四个打开、释放、读和写设备文件的方法：*/
/*打开设备方法*/

static int tz_open(struct inode* inode, struct file* filp) {
    struct tz_dev* dev;
    printk("Tz Driver OPEN FUNC!!!!");
    /*将自定义设备结构体保存在文件指针的私有数据域中，以便访问设备时拿来用*/
    dev = container_of(inode->i_cdev, struct tz_dev, dev);
    filp->private_data = dev;

    return 0;
}


/*设备文件释放时调用，空实现*/
static int tz_release(struct inode* inode, struct file* filp) {
    return 0;
}

/*读取设备的寄存器val 的值*/
static ssize_t tz_read(struct file* filp, char __user *buf, size_t count, loff_t* f_pos) {
    ssize_t err = 0;
    struct tz_dev* dev = filp->private_data;

    /*同步访问*/
    if(down_interruptible(&(dev->sem))) {
        return -ERESTARTSYS;
    }

    if(count < sizeof(dev->val)) {
        goto out;
    }

    /*将寄存器val 的值拷贝到用户提供的缓冲区*/
    if(copy_to_user(buf, &(dev->val), sizeof(dev->val))) {
        err = -EFAULT;
        goto out;
    }

    err = sizeof(dev->val);

    printk("read before smc\n");
    sendSMC(&sSMCArgs);

out:
    up(&(dev->sem));
    return err;
}

static void my_tasklet_func(unsigned long data)
{
    printk("this is tasklet handler\n");
    printk("need_resched %u, preempt count %u\n", current_thread_info()->flags, current_thread_info()->preempt_count);
    smc(1);
}

static long secondary_otz_smc_handler(void *info)
{
        struct otz_smc_cdata *cd = (struct otz_smc_cdata *)info;

        rmb();


        cd->ret_val = smc(cd->cmd_addr);
        wmb();
	 return 0;

}

static u32 post_smc(int cpu_id,u32 cmd_addr)
{
    struct otz_smc_cdata *cd = &otz_smc_cd[cpu_id];


            cd->cmd_addr = cmd_addr;
            cd->ret_val  = 0;
            wmb();

    //        smp_call_function_single(0, secondary_otz_smc_handler,
      //              (void *)cd, 1);
               work_on_cpu(0,secondary_otz_smc_handler,(void *)cd);
            rmb();


            return cd->ret_val;



}
static void smc_call(u32 cmd_addr)
{

    int cpu_id = smp_processor_id();

        if (cpu_id != 0) {
                    mb();
                return           post_smc(cpu_id, cmd_addr); /* post it to primary */
        } else {
                return              post_smc(cpu_id,cmd_addr); /* called directly on primary core */
        }
}


int hdcp_test_write(void)
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    TEEC_Operation operation;
    TEEC_UUID svc_id = {
        0x79b77788, 0x9789, 0x4a7a,
        { 0xa2, 0xbe, 0xb6, 0x01, 0x55, 0xee, 0xf5, 0xf3 }
    };
    //TEE_SERVICE_ECHO;

    int err=0;
    unsigned int dev_id;

    printk("%s, %d\n", __func__, __LINE__);
    result = TEEC_InitializeContext(
               NULL,
               &context,
               &dev_id
               );
    
    if(result != TEEC_SUCCESS) {
        goto cleanup_1;
    }
    result = TEEC_OpenSession(
                &context,
                &session,
                &svc_id,
                TEEC_LOGIN_PUBLIC,
                NULL,
                NULL,
                NULL,
                dev_id);

    if(result != TEEC_SUCCESS) {
        goto cleanup_2;
    }

    memset(&operation, 0x00, sizeof(operation));
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = 0xFFFF;
    operation.params[0].value.b = 0xFFFD;
    result = TEEC_InvokeCommand(
                 &session,
                 ECHO_CMD_ID_SEND_CMD,
                 &operation,
                 NULL,
                 dev_id);

    if (result == TEEC_SUCCESS) {
        printk("[0] : %x %x\n", operation.params[0].value.a, operation.params[0].value.b);
    } else {
        printk("invoke failed\n");
        goto cleanup_3;
    }

    printk("hdcp key write test pass\n");
cleanup_3:
    TEEC_CloseSession(&session,dev_id);
cleanup_2:
    TEEC_FinalizeContext(&context,dev_id);
cleanup_1:
    if (err)
        while(1);
    return 0;

    //TEEC_Encode(&cli_context, *destination, session->session_id,
     //   GLOBAL_CMD_ID_OPEN_SESSION, cli_login, operation);
    #if 0
    struct file test_file;
    unsigned long arg;
    TC_NS_ClientOpen_from_kernel(NULL, &test_file);
    TC_NS_ClientIoctl_from_kernel(&test_file, TC_NS_CLIENT_IOCTL_SES_OPEN_REQ,  arg);
    TC_NS_ClientIoctl_from_kernel(&test_file, TC_NS_CLIENT_IOCTL_SEND_CMD_REQ,  arg);
    TC_NS_ClientIoctl_from_kernel(&test_file,TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ,  arg);
    TC_NS_ClientClose_from_kernel(NULL, &test_file);
    #endif
}
  // DECLARE_TASKLET(my_tasklet,my_tasklet_func,0);

#if 0
int cp_test()
{
    void * pa1 = 0;
    void * pa2 = 0;
    int i;
    char str1[1000];

    //for(i = 0; i < 1; i++)
    {    
        pa1 = HI_SEC_MMZ_New(1 << 20, "test", "aaa1"); 
        pa2 = HI_SEC_MMZ_New(1 << 20, "test", "aaa2"); 

        printk("pa1 = 0x%x\n", pa1);
        printk("pa2 = 0x%x\n", pa2);
    }

    printk("%s, %d\n", __func__, __LINE__);
    HI_SEC_MMZ_CA2TA(2<<20, pa1, 100);
    printk("%s, %d\n", __func__, __LINE__);
    HI_SEC_MMZ_CA2TA((2<<20) - 1, pa1, 100);
    printk("%s, %d\n", __func__, __LINE__);
    //HI_SEC_MMZ_TA2CA(pa1,(2<<20) - 1, 100);
    printk("%s, %d\n", __func__, __LINE__);
    HI_SEC_MMZ_TA2TA(pa1, pa2, 100);
    printk ("%s, %d\n", __func__, __LINE__);
    
    HI_SEC_MMZ_Delete(pa1);
    HI_SEC_MMZ_Delete(pa2);
    return 0;
}
#endif

/*写设备的寄存器值val*/
static ssize_t tz_write(struct file* filp, const char __user *buf, size_t count, loff_t* f_pos) {
//    struct tz_dev* dev = filp->private_data;
    ssize_t err = 0;

    unsigned int cpu = smp_processor_id();
        int i=2000;

    /*同步访问*/
//    if(down_interruptible(&(dev->sem))) {
//        return -ERESTARTSYS;
//    }
/*
    if(count != sizeof(dev->val)) {
        goto out;
    }
*/
    /*将用户提供的缓冲区的值写到设备寄存器去*/
    /*if(copy_from_user(&sSMCArgs, buf, sizeof(sRegSMC_t))) {
        err = -EFAULT;
        goto out;
    }
    */
    printk("Tz driver Write[ before SMC send].\n");
    printk("cpu%u call\n",cpu);
    //cp_test();
    //hdcp_test_write();
/*
    tee_test_hdcp_storekey
    asm("nop");
    asm("nop");
    asm("nop");
    asm("smc #0");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
  */
//    tasklet_hi_schedule(&my_tasklet);
    //set_tsk_need_resched(current);
   // printk("need_resched %u, preempt count %u\n", current_thread_info()->flags, current_thread_info()->preempt_count);
  // while(1)
  //    printk("-----------scm call----------\n");
    //mutex_lock(&scm_lock);
     //smp_cross_call(cpumask_of(0), 9);
    //smc_call(1);
    //mutex_unlock(&scm_lock);
    //smp_cross_call(cpumask_of(0), 9);
   //   smc(1);
   //  asm("smc 2");
    printk("cpu%u call end \n",cpu);
    printk("SMC call done!\n");
/*
    sSMCArgs.CMD = 1;//DRV_TZ_SMC_API;
    switch(sSMCArgs.APICODE){
      case 1:// DRV_TZ_OPEN_DEVICE:
      //  sendSMC(&sSMCArgs);
      break;
      default:
      // Should never reach here
      tmp = -1;//TZ_ERROR_ILLEGAL_STATE;
      break;
    }
*/
    //err = sizeof(dev->val);

//out:
//    up(&(dev->sem));
    return err;
}

/*定义通过devfs 文件系统访问方法，这里把设备的寄存器val 看成是设备的一个属性，通过读写这个属性来对设备进行访问，
  主要是实现tz_val_show 和tz_val_store 两个方法，同时定义了两个内部使用的访问val 值的方法__tz_get_val 和
  __tz_set_val：*/
/*读取寄存器val 的值到缓冲区buf 中，内部使用*/

static ssize_t __tz_get_val(struct tz_dev* dev, char* buf) {
    int val = 0;

    /*同步访问*/
    if(down_interruptible(&(dev->sem))) {
        return -ERESTARTSYS;
    }

    val = dev->val;
    up(&(dev->sem));

    return ;
    //snprintk(buf, PAGE_SIZE, "%d\n", val);
}

/*把缓冲区buf 的值写到设备寄存器val 中去，内部使用*/
static ssize_t __tz_set_val(struct tz_dev* dev, const char* buf, size_t count) {
    int val = 0;

     /*将字符串转换成数字*/
    val = simple_strtol(buf, NULL, 10);

    /*同步访问*/
    if(down_interruptible(&(dev->sem))) {
        return -ERESTARTSYS;
    }

    dev->val = val;
    up(&(dev->sem));

    return count;
}

/*读取设备属性val*/
static ssize_t tz_val_show(struct device* dev, struct device_attribute* attr, char* buf) {
    struct tz_dev* hdev = (struct tz_dev*)dev_get_drvdata(dev);

    return __tz_get_val(hdev, buf);
}

/*写设备属性val*/
static ssize_t tz_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) {
    struct tz_dev* hdev = (struct tz_dev*)dev_get_drvdata(dev);

    return __tz_set_val(hdev, buf, count);
}


/*最后，定义模块加载和卸载方法，这里只要是执行设备注册和初始化操作：*/
/*初始化设备*/
static int __tz_setup_dev(struct tz_dev* dev) {
    int err;
    dev_t devno = MKDEV(tz_major, tz_minor);

    memset(dev, 0, sizeof(struct tz_dev));

    cdev_init(&(dev->dev), &tz_fops);
    dev->dev.owner = THIS_MODULE;
    dev->dev.ops = &tz_fops;

    /*注册字符设备*/
    err = cdev_add(&(dev->dev),devno, 1);
    if(err) {
       return err;
    }

    /*初始化信号量和寄存器val 的值*/
    //init_MUTEX(&(dev->sem));
	sema_init(&(dev->sem), 1);
    dev->val = 0;

    return 0;
}

/*模块加载方法*/
static int __init tz_init(void){
//printk("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHtz\n");
//return 0;
    int err = -1;
    dev_t dev = 0;
    struct device* temp = NULL;

    /*动态分配主设备和从设备号*/
    err = alloc_chrdev_region(&dev, 0, 1, HELLO_DEVICE_NODE_NAME);
    if(err < 0) {
        printk(KERN_ALERT"Failed to alloc char dev region.\n");
        goto fail;
    }

    tz_major = MAJOR(dev);
    tz_minor = MINOR(dev);

     /*分配helo 设备结构体变量*/
    tz_dev = kmalloc(sizeof(struct tz_dev), GFP_KERNEL);
    if(!tz_dev) {
        err = -ENOMEM;
        printk(KERN_ALERT"Failed to alloc tz_dev.\n");
        goto unregister;
    }

    /*初始化设备*/
    err = __tz_setup_dev(tz_dev);
    if(err) {
        printk(KERN_ALERT"Failed to setup dev: %d.\n", err);
        goto cleanup;
    }

     /*在/sys/class/目录下创建设备类别目录tz*/
    tz_class = class_create(THIS_MODULE, HELLO_DEVICE_CLASS_NAME);
    if(IS_ERR(tz_class)) {
        err = PTR_ERR(tz_class);
        printk(KERN_ALERT"Failed to create tz class.\n");
        goto destroy_cdev;
    }

    /*在/dev/目录和/sys/class/tz 目录下分别创建设备文件tz*/
    temp = device_create(tz_class, NULL, dev, "%s", HELLO_DEVICE_FILE_NAME);
    if(IS_ERR(temp)) {
        err = PTR_ERR(temp);
        printk(KERN_ALERT"Failed to create tz device.");
        goto destroy_class;
    }

    /*在/sys/class/tz/tz 目录下创建属性文件val*/
    err = device_create_file(temp, &dev_attr_val);
    if(err < 0) {
        printk(KERN_ALERT"Failed to create attribute val.");
        goto destroy_device;
    }

    dev_set_drvdata(temp, tz_dev);

    /*创建/proc/tz 文件*/
    //tz_create_proc();

    TEEC_Debug("TZ device initialized.\n");
    /*added by zhtao*/
    //cp_test(); 
    //hdcp_test_write();
    return 0;

destroy_device:
    device_destroy(tz_class, dev);

destroy_class:
    class_destroy(tz_class);

destroy_cdev:
    cdev_del(&(tz_dev->dev));

cleanup:
    kfree(tz_dev);

unregister:
    unregister_chrdev_region(MKDEV(tz_major, tz_minor), 1);

fail:
    return err;
}
EXPORT_SYMBOL(hdcp_test_write);
//EXPORT_SYMBOL(cp_test);

/*模块卸载方法*/
static void __exit tz_exit(void)
{
    TEEC_Debug("TZ device removed.\n");
#if 0
    dev_t devno = MKDEV(tz_major, tz_minor);

    printk(KERN_ALERT"Destroy tz device.\n");

    /*删除/proc/tz 文件*/
    //tz_remove_proc();

    /*销毁设备类别和设备*/
    if(tz_class) {
        device_destroy(tz_class, MKDEV(tz_major, tz_minor));
        class_destroy(tz_class);
    }

    /*删除字符设备和释放设备内存*/
    if(tz_dev) {
        cdev_del(&(tz_dev->dev));
        kfree(tz_dev);
    }

    /*释放设备号*/
    unregister_chrdev_region(devno, 1);
#endif
}

EXPORT_SYMBOL(tz_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TrustZone Driver");

//late_initcall(tz_init);
module_init(tz_init);
module_exit(tz_exit);
