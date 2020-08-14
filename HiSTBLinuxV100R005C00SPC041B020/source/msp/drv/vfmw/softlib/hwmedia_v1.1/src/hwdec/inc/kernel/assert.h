
#ifndef _LINUX_ASSERT_H
#define _LINUX_ASSERT_H

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define assert(expr) do { \
        if (!(expr)) \
            printk("!!ASSERTION FAILED: [%s:%d] \"" #expr "\"\n", __FILE__, __LINE__); \
    } while (0)
#else
#define assert(expr)
#endif

#endif /* _LINUX_ASSERT_H */
