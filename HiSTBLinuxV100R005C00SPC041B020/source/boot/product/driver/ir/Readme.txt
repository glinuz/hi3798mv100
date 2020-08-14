Hisilicon ir driver in fastboot3.0.
Copyright (c) 2013 Hisilicon Co., Ltd.

When your application want to use infrared code in fastboot, 
please include hi_unf_ir.h which have been place in ../include/hi_unf_ir.h.

The usage of interfaces in hi_unf_ir.h is the same with hi_unf_ecs.h,
please refer to the api document for details.

File cmd_ir.c is a sample usage of this ir driver in fastboot3.0.

Usage of cmd_ir:
Step 1: Build you SDK and burn fastboot-burn.bin into board.
Step 2: Stop at fastboot command line after board boot up.
Step 3: Type "ir start" in fastboot cmdline to open ir driver.
Step 4: Type "ir read 200 200" to get the keys will be sent out 
	by some RC.
	- The first number 200 means you want to fetch 200 keys
	parsed by ir driver.
	- The second number 200 means you want to wait at most 200 ms
	while get one key from ir driver. 

Commands supported by this driver:
1. ir start: Open the ir driver.
2. ir stop: Close the ir driver.
3. ir read @cnt @timeout_ms: Try to get @cnt keys, 
	and wait a key at most @timeout_ms time.
	While reading keys, you can interrupt read by press Ctrl+c.
4. ir status @protocol: Get the enable/disable status of protocol 
	specified by @protocol.
5. ir enable @protocol: Enable protocol specified by @protocol.
6. ir disable @protocol: Disable protocol specified by @protocol.
7. ir set up 0|1: To set report up event(1) or not report (0).
8. ir set hold 0|1: To set report hold event(1) or not report(0).
9. ir set hi @ms: To set hold event report interval, unit millisecond.

In debug mode, the two command list blow is supported.
7. ir set dbg on|off: Turn on debug message or turn off debug message.
8. ir set info on|off: Turn on info message or turn off info message.

Notes:
	You can type "?ir" in fastboot cmdline to get some help infomations
	about how to use the ir command.
