#ifndef _HELLO_Android_H_
#define _HELLO_ANDROID_H_
#include <linux/cdev.h>
#include <linux/semaphore.h>
#define HELLO_DEVICE_NODE_NAME "tzdriver"
#define HELLO_DEVICE_FILE_NAME "tzdriver"
#define HELLO_DEVICE_PROC_NAME "tzdriver"
#define HELLO_DEVICE_CLASS_NAME "tzdriver"
struct tz_dev {
 int val;
 struct semaphore sem;
 struct cdev dev;
};

typedef struct xRegSMC

{

  unsigned int APICODE;
  unsigned int SMCID;
  unsigned int r1;
  unsigned int r2;
  unsigned int r3;
  unsigned int r4;
  unsigned int r5;
  unsigned int r6;
  unsigned int CMD;

} sRegSMC_t;

#endif
