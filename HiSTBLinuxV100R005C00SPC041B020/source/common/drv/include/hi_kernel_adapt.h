
#define HI_DECLARE_MUTEX(x) DEFINE_SEMAPHORE(x)

#define HI_INIT_MUTEX(x)    sema_init(x, 1)
