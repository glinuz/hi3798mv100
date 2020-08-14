1.Directory structure£º
1) adapter/£ºThis interface adapter required
	int fv_is_first_enter(int *first);
		This interface is used to determine whether it is first powered up
	void fv_clear_first_enter(void);
		This interface is used to clear the first power-on flag		
	void fv_factory_reset(void);
		This interface is used to restore the factory settings
	#define MAX_VERSION_LEN (64)
		This macro defines the maximum length of the version number
	void fv_get_sdk_version(char *buf, int *sz);
		This interface is used to obtain sdk version number
	void fv_get_app_version(char *buf, int *sz);
		This interface is used to obtain app version number
	void fv_get_plat_version(char *buf, int *sz);
		This interface is used to obtain platform version number
	
2) configs/: This configurations adapter required
	#define VIDEO_FILE_NAME "video.ts"
		video name
	#define DDR_DETECT_DRV	"/root/ddr.ko"
		ddr size detect module
	#define CONFIG_FILE_NAME "/root/config.ini"
		config file path 
	#define UDISK_MOUNT_POINT "/mnt"
		Udisk mount point
	#define FV_CONF_LANG_ZH
		default defined and display chinese,otherwise dislay english
		
	#define IR_VOL_PLUS_KEY	              (0x3ec1ff00)
		volume up key
	#define IR_VOL_DEC_KEY	              (0x6699ff00)
		volume down key
	#define IR_RESET_FACTORY_KEY	       (0x639cff00)
		restore factory settings key
	
	#define KEYLED_TYPE (0)
		keyled type
	
	#define MAX_TUNER_NR	1
		max tuner connect number
	#define TUNER_SECTION	"tuner"
		tuner section keyword in config file
	#define	FREQ_KEY	"freq" 
		tuner frequency setting keyword in config file
	#define SYMBOL_RATE_BAND_KEY	"symbol_rate_or_band"
		symbol setting keyword in config file
	#define MODE_TYPE_POLAR_KEY	"mode_type_or_polar"
		qam modulation setting keyword in config file
	#define REVERSE_KEY	"reverse"
		spectrum invert setting keyword in config file
	
2.Userguide	
	
1) Deploy
	/root
    	©À©¤©¤ config.ini     £»from configs/config.ini
    	©À©¤©¤ ddr.ko         ; from drivers/ddr.ko
    	©À©¤©¤ hifv           ; from out/hifv
    	©À©¤©¤ res        
    	©¦   ©¸©¤©¤ DroidSansFallbackLegacy.ttf ; from sample/res/font/DroidSansFallbackLegacy.ttf 
   	If you want to verify audio and video play feature, 
   	please rename the local audio and video files as video.ts and 
   	copy it to the u disk first fat partition.
2)Execute£º
	#./hifv OR ./hifv 1;  the latter is manual mode.
