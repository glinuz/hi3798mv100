#ifndef __COMMON_MEM_DRV_PRIVATE_H__
#define __COMMON_MEM_DRV_PRIVATE_H__

#ifdef __cplusplus
extern "C" {
#endif

extern HI_U8* kmem_file_name;
extern HI_U32 kmem_line_number;

#define KMEM_FILE_LINE kmem_file_name = __FILE__, kmem_line_number = __LINE__

HI_VOID*    hi_kzalloc(HI_U32 module_id, HI_U32 size, HI_S32 flags);
HI_VOID*    hi_kmalloc(HI_U32 module_id, HI_U32 size, HI_S32 flags);
HI_VOID     hi_kfree(HI_U32 module_id, HI_VOID *ptr);

HI_VOID*    hi_vmalloc(HI_U32 module_id, HI_U32 size);
HI_VOID     hi_vfree(HI_U32 module_id, HI_VOID *ptr);

#define hi_kzalloc(module_id, size, flags)    (KMEM_FILE_LINE, hi_kzalloc(module_id, size, flags))
#define hi_kmalloc(module_id, size, flags)    (KMEM_FILE_LINE, hi_kmalloc(module_id, size, flags))
#define hi_kfree(  module_id,  addr)          (KMEM_FILE_LINE, hi_kfree(  module_id, addr))
#define hi_vmalloc(module_id, size)           (KMEM_FILE_LINE, hi_vmalloc(module_id, size))
#define hi_vfree(  module_id, addr)           (KMEM_FILE_LINE, hi_vfree(  module_id, addr))

#ifdef __cplusplus
}
#endif

#endif
