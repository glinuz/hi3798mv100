	.section .image,#alloc
	.globl	input_data
input_data:
	.incbin   "image_data.lzma"
	.globl    input_data_end
input_data_end:
