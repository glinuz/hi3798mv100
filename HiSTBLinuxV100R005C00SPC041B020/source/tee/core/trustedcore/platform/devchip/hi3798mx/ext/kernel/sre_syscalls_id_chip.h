// vfmw begin
#ifdef WITH_HISI_VFMW
#define SW_SYSCALL_VFMW_INIT                0xc84c
#define SW_SYSCALL_VFMW_EXIT                0xc850
#define SW_SYSCALL_VFMW_RESUME              0xc854
#define SW_SYSCALL_VFMW_SUSPEND             0xc858
#define SW_SYSCALL_VFMW_CONTROL             0xc85c
#define SW_SYSCALL_VFMW_THREAD              0xc860
#define SW_SYSCALL_VFMW_GET_IMAGE           0xc864
#define SW_SYSCALL_VFMW_RELEASE_IMAGE       0xc868
#define SW_SYSCALL_VFMW_READ_PROC           0xc86c
#define SW_SYSCALL_VFMW_WRITE_PROC          0xc870
#endif

// vfmw end
/* SEC MMZ */
#define SW_SYSCALL_MMZ_NEW                              0xc900
#define SW_SYSCALL_MMZ_DEL                              0xc901
#define SW_SYSCALL_MMZ_MALLOC                           0xc902
#define SW_SYSCALL_MMZ_FREE                             0xc903
#define SW_SYSCALL_MMZ_MAP                              0xc904
#define SW_SYSCALL_MMZ_UNMAP                            0xc905
#define SW_SYSCALL_MMZ_FLUSH                            0xc906
#define SW_SYSCALL_MEM_COPY                             0xc908
#define SW_SYSCALL_TEE_ISSECMMZ				            0xc909
/* SEC MEM */
#define SW_SYSCALL_TEE_ISSECMEM                         0xc920
#define SW_SYSCALL_TEE_ISNONSECMEM                      0xc924
#define SW_SYSCALL_DEBUG                                0xc928

#if (defined(WITH_HISI_CIPHER))
#define SW_SYSCALL_CIPHER_IOCTL				0xc800
#define SW_SYSCALL_CIPHER_PBKDF2			0xc804
#endif

#ifdef WITH_HISI_OTP
#define SW_SYSCALL_OTP_INIT				    0xc601
#define SW_SYSCALL_OTP_DeInit				0xc602
#define SW_SYSCALL_OTP_Reset				0xc603
#define SW_SYSCALL_OTP_GetChipID			0xc604
#define SW_SYSCALL_OTP_ReadWord				0xc605
#define SW_SYSCALL_OTP_ReadByte				0xc606
#define SW_SYSCALL_OTP_WriteWord			0xc607
#define SW_SYSCALL_OTP_WriteByte			0xc608
#define SW_SYSCALL_OTP_Test				    0xc609
#endif

#ifdef WITH_HISI_ADVCA

#define SW_SYSCALL_ADVCA_IOCTL              0xc707
#endif

/* smmu   */
#ifdef WITH_HISI_SMMU
#define SW_SYSCALL_HISI_SEC_MAPTOSMMU           0xc300
#define SW_SYSCALL_HISI_SEC_UNMAPFROMSMMU               0xc304
#define SW_SYSCALL_HISI_SEC_MAPTOSMMU_AND_SETFLAG               0xc308
#define SW_SYSCALL_HISI_SEC_UNMAPFROMSMMU_AND_CLRFLG            0xc30c
#define SW_SYSCALL_HISI_SEC_ALLOC               0xc310
#define SW_SYSCALL_HISI_SEC_FREE                0xc314
#define SW_SYSCALL_HISI_SEC_MAP_TO_CPU          0xc318
#define SW_SYSCALL_HISI_SEC_UNMAP_FROM_CPU              0xc31c
#define SW_SYSCALL_HISI_SEC_MAP_TO_SEC_SMMU             0xc320
#define SW_SYSCALL_HISI_SEC_UNMAP_FROM_SEC_SMMU         0xc324
#define SW_SYSCALL_HISI_NONSEC_MEM_MAP_TO_SEC_CPU               0xc328
#define SW_SYSCALL_HISI_NOSEC_MEM_UNMAP_FROM_SEC_CPU            0xc32c
#define SW_SYSCALL_HISI_NOSEC_MEM_MAP_TO_SEC_SMMU               0xc330
#define SW_SYSCALL_HISI_NOSEC_MEM_UNMAP_FROM_SEC_SMMU           0xc334
#define SW_SYSCALL_AGENT_CLOSED         0xc338
#define SW_SYSCALL_SEC_IOCTL            0xc33c
#define SW_SYSCALL_HISI_MEM_FLUSH               0xc340
#endif

#ifdef WITH_HISI_VDP
#define SW_SYSCALL_VDP_DRV_IOCTL        0xca00
#endif

#ifdef WITH_HISI_DMX 
#define SW_SYSCALL_DEMUX_NEW_DESC                                  0xd800
#define SW_SYSCALL_DEMUX_DEL_DESC                                  0xd804
#define SW_SYSCALL_DEMUX_ATTACH_DESC                               0xd808
#define SW_SYSCALL_DEMUX_DETACH_DESC                               0xd80C
#define SW_SYSCALL_DEMUX_GET_DESC_ATTR                             0xd810
#define SW_SYSCALL_DEMUX_SET_DESC_ATTR                             0xd814
#define SW_SYSCALL_DEMUX_SET_DESC_EVEN                             0xd818
#define SW_SYSCALL_DEMUX_SET_DESC_ODD                              0xd81c
#define SW_SYSCALL_DEMUX_SET_DESC_EVEN_IV                          0xd820
#define SW_SYSCALL_DEMUX_SET_DESC_ODD_IV                           0xd824
#define SW_SYSCALL_DEMUX_GET_KEYID                                 0xd828

#define SW_SYSCALL_DEMUX_NEW_SC                                    0xd82c
#define SW_SYSCALL_DEMUX_DEL_SC                                    0xd830
#define SW_SYSCALL_DEMUX_GET_SC_ATTR                               0xd834
#define SW_SYSCALL_DEMUX_SET_SC_ATTR                               0xd838
#define SW_SYSCALL_DEMUX_SET_SC_EVEN                               0xd83c
#define SW_SYSCALL_DEMUX_SET_SC_ODD                                0xd840
#define SW_SYSCALL_DEMUX_ATTACH_SC                                 0xd844
#define SW_SYSCALL_DEMUX_DETACH_SC                                 0xd848
#define SW_SYSCALL_DEMUX_GET_SC_KEYID                              0xd84c

#define SW_SYSCALL_DEMUX_GET_CHNID                                 0xd850

#define SW_SYSCALL_DEMUX_REG_CHAN                                  0xd854
#define SW_SYSCALL_DEMUX_UNREG_CHAN                                0xd858
#define SW_SYSCALL_DEMUX_REG_OQ                                    0xd85c
#define SW_SYSCALL_DEMUX_UNREG_OQ                                  0xd860
#define SW_SYSCALL_DEMUX_REG_RAM_PORT                              0xd864
#define SW_SYSCALL_DEMUX_UNREG_RAM_PORT                            0xd866
#define SW_SYSCALL_DEMUX_LOCK_CHAN                                 0xd86c
#define SW_SYSCALL_DEMUX_UNLOCK_CHAN                               0xd870
#define SW_SYSCALL_DEMUX_REG_VID_SECBUF                            0xd874
#define SW_SYSCALL_DEMUX_UNREG_VID_SECBUF                          0xd878
#define SW_SYSCALL_DEMUX_REG_AUD_SECBUF                            0xd87c
#define SW_SYSCALL_DEMUX_UNREG_AUD_SECBUF                          0xd880
#define SW_SYSCALL_DEMUX_FIXUP_AUD_SECBUF                          0xd884
#define SW_SYSCALL_DEMUX_PARSER_PES_HEADER                         0xd888
#define SW_SYSCALL_DEMUX_PARSER_DISP_CONTROL                       0xd88c
#define SW_SYSCALL_DEMUX_GET_PES_HEADER_LEN                        0xd890
#define SW_SYSCALL_DEMUX_REG_REC_SECBUF                            0xd894
#define SW_SYSCALL_DEMUX_UNREG_REC_SECBUF                          0xd898
#define SW_SYSCALL_DEMUX_PARSER_SC_DATA                            0xd89c
#define SW_SYSCALL_DEMUX_PARSER_FILTER_SC_DATA                     0xd8a0
#endif

#ifdef WITH_HISI_PVR
#define SW_SYSCALL_PVR_OPEN_CHANNEL                             0xd900
#define SW_SYSCALL_PVR_CLOSE_CHANNEL                            0xd904
#define SW_SYSCALL_PVR_PROC_TSDATA                              0xd908
#define SW_SYSCALL_PVR_GET_TEE_STATE                            0xd90c
#define SW_SYSCALL_PVR_COPY_TO_TEE                              0xd910
#define SW_SYSCALL_PVR_COPY_TO_REE                              0xd914
#define SW_SYSCALL_PVR_DeInit                                   0xd918
#define SW_SYSCALL_PVR_GET_OUTPUTBUF                            0xd91c
#define SW_SYSCALL_PVR_COPY_TO_REE_TEST                         0xd920
#define SW_SYSCALL_PVR_DEMUX_REG_BUFFER                         0xd924
#define SW_SYSCALL_PVR_DEMUX_UNREG_BUFFER                       0xd928
#define SW_SYSCALL_PVR_CA_GET_ADDRINFO                          0xd928
#endif

#ifdef WITH_HISI_SEC_HDMI
#define SW_SYSCALL_HI_DRV_HDMI_SETSRM                           0x8000
#define SW_SYSCALL_HI_DRV_HDMI_SETHDCPSTRATEGY                  0x8004
#define SW_SYSCALL_HI_DRV_HDMI_GETHDCPSTRATEGY                  0x8008
#define SW_SYSCALL_HI_DRV_HDMI_GETHDCPSTATUS                    0x800c
#define SW_SYSCALL_HI_DRV_HDMI_SETHDCPMUTE                      0x8010
#define SW_SYSCALL_HI_DRV_HDMI_HDCP22_INIT                      0x8014
#define SW_SYSCALL_HI_DRV_HDMI_TEEPROC                          0x8018
#define SW_SYSCALL_HI_DRV_HDMI_SETHDCPCAPS                      0x801c
#define SW_SYSCALL_HI_DRV_HDMI_GETHDCPCAPS                      0x8020
#define SW_SYSCALL_HI_DRV_HDMI_INIT                             0x8024
#define SW_SYSCALL_HI_DRV_HDMI_DEINIT                           0x8028
#define SW_SYSCALL_HI_DRV_HDMI_START                            0x802c
#define SW_SYSCALL_HI_DRV_HDMI_STOP                             0x8030
#define SW_SYSCALL_HI_DRV_HDMI_IOCTRL                           0x8034
#endif
#ifdef WITH_STB_BEIDOU
#define SW_SYSCALL_BEIDOU_GET_SW_VER      0x1000
#define SW_SYSCALL_BEIDOU_GET_POS_PARAM   0x1004
#define SW_SYSCALL_BEIDOU_CALC_DISTANCE   0x1008
#endif

#ifdef  WITH_STB_I2C_DRIVER

#define SW_SYSCALL_I2C_INIT                        0xc100
#define SW_SYSCALL_I2C_SET_SPEED                   0xc104
#define SW_SYSCALL_I2C_WRITE                       0xc108
#define SW_SYSCALL_I2C_READ                        0xc10c
#define SW_SYSCALL_I2C_DEINIT                      0xc120
#endif
