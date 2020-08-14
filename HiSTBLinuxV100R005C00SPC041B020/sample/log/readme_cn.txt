功能描述：
	演示如何使用SDK的日志打印。
命令格式：
	./sample_log
使用说明：
	对于hi_module.h中分配好的用户态模块，使用HI_MODULE_Register注册模块，然后使用HI_ERR_PRINT等宏来打印信息；
	对于没有分配的模块，在需要使用的每个文件中用HI_MODULE_DECLARE声明模块, 然后可以直接使用HI_MODULE_ERROR等宏来打印信息；
	使用HI_SYS_SetLogPath设置一个有效路径，可以将日志存储到文件。