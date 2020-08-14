/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv2bO4a86Ra1cP5w/R608AW9MyvBFsCSDo8GzQMC4Jhtu/zShyENpjFzntvB+
qu9VLf8XLp5Xa8ajq+PRch68NzO6N++JmckXDuwjeFcT855BxhwyurbQ7NR4ojDvZeoL6jC7
e7tHzbXti20q2qKQWAwLZ/GQe21+g8r9v1NUYXrGYbD5OUMw1y5FyTLbJPsK9Q==*/
/*--------------------------------------------------------------------------------------------------------------------------*/

struct file *kfile_open(const char *filename, int flags, int mode);

void kfile_close(struct file *filp);

int kfile_read(char *buf, unsigned int len, struct file *filp);

int kfile_write(char *buf, int len, struct file *filp);

int kfile_seek(loff_t offset, int origin, struct file *filp);

void dump(unsigned char data[], int len);

