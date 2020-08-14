1.目录结构：
1) adapter/：需要应用程序适配的一些接口
	int fv_is_first_enter(int *first);
		这个接口用于判定是否是第一次上电
	void fv_clear_first_enter(void);
		这个接口用于清除第一次上电标志		
	void fv_factory_reset(void);
		这个接口用于恢复出厂设置
	#define MAX_VERSION_LEN (64)
		这个宏定义了版本号字段最大长度
	void fv_get_sdk_version(char *buf, int *sz);
		这个接口用于获取sdk版本号
	void fv_get_app_version(char *buf, int *sz);
		这个接口用于获取app版本号
	void fv_get_plat_version(char *buf, int *sz);
		这个接口用于获取平台版本号
	
2) configs/: 需要客户适配的一些配置
	#define VIDEO_FILE_NAME "video.ts"
		码流名字
	#define DDR_DETECT_DRV	"/root/ddr.ko"
		ddr大小检测驱动
	#define CONFIG_FILE_NAME "/root/config.ini"	
		配置文件存放路径(含文件名)
	#define UDISK_MOUNT_POINT "/mnt"
		U盘挂载点
	#define FV_CONF_LANG_ZH
		打开这个宏时，界面显示中文，默认是中文界面，注释掉之后界面上lable显示为英文
	
	#define IR_VOL_PLUS_KEY	              (0x3ec1ff00)
		音量增大键键值
	#define IR_VOL_DEC_KEY	              (0x6699ff00)
		音量减小键键值
	#define IR_RESET_FACTORY_KEY	       (0x639cff00)
		恢复出厂设置键键值

	#define KEYLED_TYPE (0)
		前面板类型
		
	#define MAX_TUNER_NR	1
		单板所接tuner最大个数
	#define TUNER_SECTION	"tuner"
		配置文件里tuner section关键字
	#define	FREQ_KEY	"freq" 
		tuner锁频频点配置
	#define SYMBOL_RATE_BAND_KEY	"symbol_rate_or_band"
		符号率或者带宽配置,是符号率还是带宽取决于tuner类型配置
	#define MODE_TYPE_POLAR_KEY	"mode_type_or_polar"
		qam调制方式或极性配置,取决于tuner类型
	#define REVERSE_KEY	"reverse"
		频谱倒置处理方式,当tuner类型配置成CAB一类时有效
	
2.使用介绍	
	
1) 部署到单板
	/root
    	├── config.ini     ；来自 configs/config.ini
    	├── ddr.ko         ; 来自 drivers/ddr.ko
    	├── hifv           ; 来自 out/hifv
    	├── res        
    	│   └── DroidSansFallbackLegacy.ttf ; 来自 sample/res/font/DroidSansFallbackLegacy.ttf
    	
    	如果要验证音视频播放功能，还需要将本地音视频文件改名为video.ts并拷贝到u盘的第一个fat分区上。   
	
2)使用：
	如果是可执行文件hifv，执行hifv即可。如果需要强制进入厂测，执行hifv 1。
