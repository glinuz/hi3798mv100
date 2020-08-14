/*efuse*/
#define SW_SYSCALL_EFUSE_READ       0xc0ec
#define SW_SYSCALL_TEE_HAL_GET_DIEID		0xc0ed
#define SW_SYSCALL_EFUSE_WRITE      0xc0f0
#define SW_SYSCALL_P61_FAC_TEST      0xd0fe
#define SW_SYSCALL_DRIVER_TEST      0xd0ff
#ifdef TEE_SUPPORT_TUI
#define SW_TP_GET_DATA  								0xe302
#define SW_TP_SET_KB_RECT                               0xe310
#define SW_TP_SET_SIMU_DATA                             0xe311
#define SW_SYSCALL_TUI_INIT_SDRIVER                     0xc2f3
#endif
#define SW_SYSCALL_SCARD_CONNECT                        0xe108
#define SW_SYSCALL_SCARD_DISCONNECT                     0xe109
#define SW_SYSCALL_SCARD_TRANSMIT                       0xe10a

/* display */
#define SW_SYSCALL_FB_SEC_CFG                           0xe300
#define SW_SYSCALL_FB_SEC_DISPLAY                       0xe301
#define SW_SYSCALL_FB_ACTIVE_FLAG                       0xe303
#define SW_SYSCALL_FB_GETINFO                           0xe304
#define SW_SYSCALL_FB_RELEASE_FLAG                      0xe305
#define SW_SYSCALL_FB_SEC_SET                           0xe306

/*root check*/
#define SW_SYSCALL_IS_DEVICE_ROOTED		0xe400

/* get share memory info with fastboot */
#define SW_SYSCALL_GET_SHAREMEM_INFO    0xe410

/*vsim*/
#define SW_SYSCALL_GET_VSIM_SHAREMEM		0xe500
#define  SW_SYSCALL_FP_COMMAND_INFO                                0xe600
#define  SW_SYSCALL_FP_SPI_TRANSACTION                             0xe601
#define  SW_SYSCALL_FP_SPI_FULL_DUPLEX_WITH_SPEED_TRANSACTION      0xe602
#define  SW_SYSCALL_FP_SPI_HALF_DUPLEX_WITH_SPEED_TRANSACTION      0xe603
#define  SW_SYSCALL_SRE_GET_RTC_TIME                               0xe604
#define  SW_SYSCALL_SRE_SET_RTC_TIME                               0xe605

/*vcodec*/
#define SW_SYSCALL_SECURE_TEE_GETID				0x9000
#define SW_SYSCALL_SECURE_TEE_RELEASEID			0x9004
#define SW_SYSCALL_SECURE_TEE_SENDMESSAGE		0x9008
#define SW_SYSCALL_SECURE_TEE_MMAP				0x9009
#define SW_SYSCALL_SECURE_TEE_UNMAP				0x900a
#define SW_SYSCALL_SECURE_ISSEUCREMEM			0x900b
#define SW_SYSCALL_BSP_MODEM_CALL              	0xe700

#ifdef TEE_SUPPORT_HIVCODEC
/*hi_vcodec*/
#define SW_SYSCALL_SEC_VDEC_INIT                0xb000
#define SW_SYSCALL_SEC_VDEC_EXIT                0xb004
#define SW_SYSCALL_SEC_VDEC_CONTROL             0xb008
#define SW_SYSCALL_SEC_VDEC_SUSPEND             0xb00c
#define SW_SYSCALL_SEC_VDEC_RESUME              0xb010
#define SW_SYSCALL_SEC_VDEC_RUNPROCESS          0xb014
#define SW_SYSCALL_SEC_VDEC_READPROC            0xb018
#define SW_SYSCALL_SEC_VDEC_WRITEPROC           0xb01c
#define SW_SYSCALL_SEC_VDEC_GETCHANIMAGE        0xb020
#define SW_SYSCALL_SEC_VDEC_RELEASECHANIMAGE    0xb024
#endif

#ifdef FEATURE_IRIS
/*iris*/
#define  SW_SYSCALL_IRIS_TEE_MMAP                            0xe610
#define  SW_SYSCALL_IRIS_TEE_UNMAP                           0xe614
#define  SW_SYSCALL_IRIS_TEE_ISSECUREMEMORY                  0xe618
#endif