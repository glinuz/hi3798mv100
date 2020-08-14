功能描述: 演示如何调用I2C函数实现i2c器件和芯片之间的通信
命令格式:
	从i2c器件读取数据: 
	Usage: i2c_read  i2c_channel  device_addr  register_addr  register_addr_len  read_bytes_number

	向i2c器件写入数据: 
	Usage: i2c_write  i2c_channel  device_addr  register_addr  register_addr_len  write_bytes_number  byte0 [... byten] 

参数说明: 参见 Usage.

注意事项: 请确保i2c器件与芯片之间的硬件连接的正确性.