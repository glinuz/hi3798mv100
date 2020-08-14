#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>

int open_serial(const char *dev_name)
{
	int fd;
	fd = open(dev_name, O_RDWR);
	if (-1 == fd){ 
		perror("");
		exit(1);
	}

	return fd;
}

int set_speed(int fd)
{
	int ret = 0;
	struct  termios opt;

	tcgetattr(fd, &opt);
	tcflush(fd, TCIOFLUSH);
	cfsetispeed(&opt,B115200);
	cfsetospeed(&opt,B115200);

	ret = tcsetattr(fd,TCSANOW,&opt);
	if(ret <0) {
		perror("");
	}
	tcflush(fd,TCIOFLUSH);

	return ret;
}

int set_serial_rowmode(int fd)
{
	int ret = 0;
	struct termios options; 

	tcgetattr(fd, &options);

	tcflush(fd, TCIOFLUSH);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSTOPB;
	options.c_oflag  &= ~OPOST;
	

	tcflush(fd,TCIFLUSH); 
	options.c_cc[VTIME] = 128;
	options.c_cc[VMIN] = 1;


	cfmakeraw(&options);

	options.c_lflag |= ECHO;

	ret = tcsetattr(fd,TCSANOW,&options);
	if (ret < 0) { 
		perror(""); 
	} 

	return ret;
}

#define MAX_BUFF_SIZE (20)

int main(int argc, char *argv[])
{
	int fd = -1;

	fd = open_serial("/dev/ttyAMA0");
	if(fd <0 )
		printf("open tty1 error!\n");
		
	set_speed(fd);
	set_serial_rowmode(fd);

    	char buf[MAX_BUFF_SIZE];	
	read(fd, buf,MAX_BUFF_SIZE); 
	write(fd,buf,MAX_BUFF_SIZE);
	
	close(fd);
	return 0;
}
