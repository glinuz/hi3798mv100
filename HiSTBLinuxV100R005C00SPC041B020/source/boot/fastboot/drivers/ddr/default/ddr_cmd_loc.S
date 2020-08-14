/* DDR Training command codes location. Copy from DDR to SRAM to train DDR. */

.section .image,#alloc

.globl ddr_training_cmd_start
ddr_training_cmd_start:
.incbin "cmd_bin/ddr_cmd.bin"
.globl ddr_training_cmd_end
ddr_training_cmd_end:
