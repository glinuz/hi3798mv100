.balign 4

.global resume_entry_start_32
resume_entry_start_32:
.incbin "./resume_entry_32bit/resume_entry_32bit.bin"
.global resume_entry_end_32
resume_entry_end_32:
