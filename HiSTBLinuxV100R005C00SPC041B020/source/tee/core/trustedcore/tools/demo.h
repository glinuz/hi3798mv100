int bsp_efuse_write(const unsigned int *buf,
		    const unsigned int group,
		    const unsigned int size);

int bsp_efuse_read(unsigned int *buf,
		   const unsigned int group,
		   const unsigned int size);
