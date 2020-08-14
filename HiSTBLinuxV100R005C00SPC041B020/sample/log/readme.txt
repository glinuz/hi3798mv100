Purpose: to demonstrate the usage of log print.
Command: ./sample_log
Note: 
	For modules listed in hi_module.h, invoke HI_MODULE_Register to register module, then you can use the macro like HI_ERR_PRINT to print your log.
	For user modules, put HI_MODULE_DECLARE in every file you want to use SDK log print, then you can use the macro like HI_MODULE_ERROR directlly.
	If you want to save the log to a file, invoke HI_SYS_SetLogPath to set a valid path.
	