#include <config.h>

.section .image,#alloc
.align 5
.global merge_image_start
merge_image_start:

.incbin CFG_MERGE_IMAGE_FILE
merge_image_end:

.global merge_image_length
merge_image_length:
.word merge_image_end - merge_image_start

