#include "tee_symbol.h"
#include "sre_syscalls_ext.h"
#ifdef TEE_SUPPORT_TUI
#include "tui_api.h"
#include "tui_obj.h"
#include "tui_env.h"
#include "penpaint_api.h"
#include "tee_tui_api.h"
#endif
#if ((TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3650) || (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI6250))
EXPORT_SYMBOL(__SECURE_TEE_GetId);
EXPORT_SYMBOL(__SECURE_TEE_ReleaseId);
EXPORT_SYMBOL(__SECURE_TEE_SendMessage);
#endif

#ifdef FEATURE_IRIS
EXPORT_SYMBOL(__iris_tee_mmap);
EXPORT_SYMBOL(__iris_tee_unmap);
EXPORT_SYMBOL(__iris_tee_isSecureMemory);
#endif

EXPORT_SYMBOL(__SECURE_TEE_Mmap);
EXPORT_SYMBOL(__SECURE_TEE_Unmap);
EXPORT_SYMBOL(__SECURE_ISSecureMemory);

EXPORT_SYMBOL(__bsp_efuse_read);
EXPORT_SYMBOL(__bsp_efuse_write);
EXPORT_SYMBOL(__bsp_modem_call);
EXPORT_SYMBOL(__tee_hal_get_dieid);

EXPORT_SYMBOL(__hisi_fb_cfg_sec);
EXPORT_SYMBOL(__hisi_wait_vactive_flag);
EXPORT_SYMBOL(__hisi_wait_release_flag);
EXPORT_SYMBOL(__hisi_fb_alpha_set);
EXPORT_SYMBOL(__hisi_get_disp_info);

EXPORT_SYMBOL(__scard_connect);
EXPORT_SYMBOL(__scard_disconnect);
EXPORT_SYMBOL(__scard_transmit);
#ifdef TEE_SUPPORT_TUI

// tee tui api
EXPORT_SYMBOL(tui_init);
EXPORT_SYMBOL(tui_release);
EXPORT_SYMBOL(poll_tui_event);
EXPORT_SYMBOL(tui_show);
EXPORT_SYMBOL(tui_get_tpdata);
EXPORT_SYMBOL(init_keyboard);

//png
EXPORT_SYMBOL(tui_draw_picture);
EXPORT_SYMBOL(tui_draw_picture_ext);
EXPORT_SYMBOL(tui_draw_pic);
EXPORT_SYMBOL(tui_draw_pic_ext);
EXPORT_SYMBOL(tui_draw_pic_ext_scale);

//ui func
EXPORT_SYMBOL(ui_draw_rect);
EXPORT_SYMBOL(ui_draw_round_rect);
EXPORT_SYMBOL(ui_draw_line);
EXPORT_SYMBOL(ui_draw_point);

EXPORT_SYMBOL(ui_system_open);
EXPORT_SYMBOL(ui_system_close);
EXPORT_SYMBOL(ui_open);
EXPORT_SYMBOL(ui_close);
EXPORT_SYMBOL(ui_show);
EXPORT_SYMBOL(ui_bitmap);


EXPORT_SYMBOL(get_win_info);
EXPORT_SYMBOL(env_init);
EXPORT_SYMBOL(env_attach_ui);
EXPORT_SYMBOL(env_attach_IM);
EXPORT_SYMBOL(env_attach_IM_keyboard);
EXPORT_SYMBOL(env_attach_SI);
EXPORT_SYMBOL(env_status_init);
EXPORT_SYMBOL(env_showup);
EXPORT_SYMBOL(env_release);
EXPORT_SYMBOL(env_event_loop);
EXPORT_SYMBOL(env_keyboard_loop);
EXPORT_SYMBOL(env_set_focus);
EXPORT_SYMBOL(env_event_loop_quit);
EXPORT_SYMBOL(keyboard_font_init);
EXPORT_SYMBOL(keyboard_font_exit);
EXPORT_SYMBOL(font_init);
EXPORT_SYMBOL(font_close);
EXPORT_SYMBOL(ui_set);



EXPORT_SYMBOL(create_pen);
EXPORT_SYMBOL(destroy_pen);
EXPORT_SYMBOL(tui_draw_string);
EXPORT_SYMBOL(tui_draw_string_ext);
EXPORT_SYMBOL(tui_check_text_format);

EXPORT_SYMBOL(TEE_TUIInitSession);
EXPORT_SYMBOL(TEE_TUIDisplayScreen);
EXPORT_SYMBOL(TEE_TUICloseSession);
EXPORT_SYMBOL(TEE_TUIGetScreenInfo);
EXPORT_SYMBOL(TEE_TUICheckTextFormat);
EXPORT_SYMBOL(TEE_TUISetInfo);
#endif

EXPORT_SYMBOL(__driver_dep_test);
EXPORT_SYMBOL(__driver_p61_factory_test);
EXPORT_SYMBOL(__driver_fingerprint_command);
EXPORT_SYMBOL(__driver_spi_full_duplex);
EXPORT_SYMBOL(__driver_spi_full_duplex_with_speed);
EXPORT_SYMBOL(__driver_spi_half_duplex_with_speed);

EXPORT_SYMBOL(__get_target_product);

EXPORT_SYMBOL(ss_crypto_use_cbc);

/*skytone version api*/
EXPORT_SYMBOL(TEE_EXT_Get_Skytone_Version);

/*rtc config api*/
EXPORT_SYMBOL(__sre_get_rtc_time);
EXPORT_SYMBOL(__sre_set_rtc_time);
#ifdef TEE_SUPPORT_HIVCODEC
EXPORT_SYMBOL(__SEC_VDEC_Init);
EXPORT_SYMBOL(__SEC_VDEC_Exit);
EXPORT_SYMBOL(__SEC_VDEC_Control);
EXPORT_SYMBOL(__SEC_VDEC_Suspend);
EXPORT_SYMBOL(__SEC_VDEC_Resume);
EXPORT_SYMBOL(__SEC_VDEC_RunProcess);
EXPORT_SYMBOL(__SEC_VDEC_ReadProc);
EXPORT_SYMBOL(__SEC_VDEC_WriteProc);
EXPORT_SYMBOL(__SEC_VDEC_GetChanImage);
EXPORT_SYMBOL(__SEC_VDEC_ReleaseChanImage);
#endif
