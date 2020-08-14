
(1)
int open_serial(const char *dev_name)
This function opens a seirial device.It returns a descripter that you can use to access the COM port,for instance,"/dev/ttyAMA0".

Parameters
dev_name   Pointer to a null-terminated string that specifies the name of the COM port to open. 

(2)
int set_speed(int fd)
This function sets the input and output baudrate of a COM port to 115200,and flushes both data received but not read, and data written but not transmitted.
It returns success if any of the requested changes counld be successfully carried out.
return value 0  indicates success.
return value -1 indicates failure.

Parameters
fd  Descripter of the COM port the property of which would be changed.

(3)
int set_serial_rowmode(int fd)
This function sets the data bits of a COM port to 8,disables parity generation on output and parity checking for input ,and sets one stop bit.
It sets timeout in deciseconds for non-canonical read to 128 and minimum number of characters for non-canonical read to 1,and echos input characters.
return value 0  indicates success.
return value -1 indicates failure.

Parameters
fd  Descripter of the COM port the property of which would be changed.

===============================================================
Example Code

The following example demonstrates the use of the read function.

char buff[10];//Buffer for the data to read. 
int length = 1;//Length of the data to read.
int ret = read(fd, buf,length);
//the ret value indiates the count of data read.
if(ret == length)
{
	//It indicates that the the read operation is successfully carried out.
}
--------------------------------------------------------------
The following example demonstrates the use of the write function.

char buf[10] = "123456789";// Buffer containing the data to be transmitted to the COM port.
write(fd,buf,10);//It sends 10 bytes to the the COM port.


