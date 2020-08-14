Purpose: to demonstrate how to communicate with i2c device by calling function of I2C module .

Command: 
	read:
	Usage: i2c_read  i2c_channel  device_addr  register_addr  register_addr_len  read_bytes_number
	write:
	Usage: i2c_write  i2c_channel  device_addr  register_addr  register_addr_len  write_bytes_number  byte0 [... byten]
        
Parameter: see Usage.

Notice: to insure that the hardward connection between i2c device and chipset is correctly. 