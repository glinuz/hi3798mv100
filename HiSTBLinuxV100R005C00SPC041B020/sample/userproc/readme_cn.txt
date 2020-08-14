功能描述：
	演示userproc的接口如何使用。
命令格式: 
	./sample_userproc
使用说明:
	调用'HI_PROC_AddDir'和'HI_PROC_RemoveDir'分别用以创建和删除proc目录，此目录将会被创建到/proc/hisi目录下以${NAME_INPUT}_${PID}的方式命名。
	调用'HI_PROC_AddEntry' 和'HI_PROC_RemoveEntry'分别用以创建和删除proc文件。
	通过注册'HI_PROC_SHOW_FN'回调就能够支持通过'cat /proc/hisi/your_dir_pid/your_entry'在回调函数中通过'HI_PROC_Printf'输出信息反馈给用户。
	通过注册'HI_PROC_CMD_FN'回调就能够支持通过'echo params [...] > /proc/hisi/your_dir_pid/your_entry'在回调函数中处理用户输入的信息。
	userproc最大支持在‘HI_PROC_SHOW_FN’中通过'HI_PROC_Printf'输出4KB的日志信息。