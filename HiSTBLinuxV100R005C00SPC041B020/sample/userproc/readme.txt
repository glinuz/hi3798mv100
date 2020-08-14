Purpose: to demonstrate the user proc.
Command: ./sample_userproc
Note:
	Invoke API 'HI_PROC_AddDir' and 'HI_PROC_RemoveDir' to create or remove a proc directory, the dir will be added in /proc/hisi/, for multi-process supporting, its name will be ${NAME_INPUT}_${PID}.
	Invoke API 'HI_PROC_AddEntry' and 'HI_PROC_RemoveEntry' to create or remove a proc entry.
	You can register a call back function 'HI_PROC_SHOW_FN' to support read command as 'cat /proc/hisi/your_dir_pid/your_entry',
	and register function 'HI_PROC_CMD_FN' to support write command as 'echo params [...] > /proc/hisi/your_dir_pid/your_entry'.
	Invoke API 'HI_PROC_Printf' in your 'HI_PROC_SHOW_FN' to print info to proc entry.
	You can invoke it repeatedly, but the total amount of data must be less than 4KB.