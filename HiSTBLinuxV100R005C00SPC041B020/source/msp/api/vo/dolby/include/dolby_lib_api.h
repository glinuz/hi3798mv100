/******************************************************************************

  Copyright (C), 2001-2015, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_vo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/10/13
  Description   :
  History       :
  1.Date        : 2015/10/13
    Author      : t00177539
    Modification: Created file

*******************************************************************************/
#include <rpu_ext_config.h>
#include <rpu_api_common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "KdmTypeFxp_hisi.h"


/*-----------------------------------------------------------------------------+
 |                            golbal Test define
 +----------------------------------------------------------------------------*/
//#define METADATA_TEST_READ_BIN_WRITE_BIN
//#define METADATA_TEST_WRITWE_BIN

//#define DOLBY_REG_SELF_TEST
//#define DOLBY_REG_TEST_WRITE_BIN 



/*-----------------------------------------------------------------------------+
 |                            golbal API
 +----------------------------------------------------------------------------*/

typedef enum {
    false = 0,
    true = 1,
}bool;

/*-----------------------------------------------------------------------------+
 |                            Metadata Parser API
 +----------------------------------------------------------------------------*/
#define METADATA_NULL_ADDR              0x80000001
#define METADATA_MALLOC_FAILURE         0x80000002
#define METADATA_OPENFILE_FAILURE       0x80000003
#define METADATA_INIT_FAILURE           0x80000004
#define METADATA_PARSE_FAILURE          0x80000005
extern int metadata_parser_init(void);
extern int metadata_parser_deinit(void);
extern int metadata_parser_process(char *p_rpu_buf, uint32_t rpu_len,
        rpu_ext_config_fixpt_main_t *p_composer_data,
        dm_metadata_t *p_dm_data, char *p_md_bin);

int set_medata_debug_dir(char *p_input_rpu_name, char *p_out_dir);

/*-----------------------------------------------------------------------------+
 |                            Metadata update over hdmi API
 +----------------------------------------------------------------------------*/
extern void bypass_metadata_update (dm_metadata_base_t        *p_dm_md,
                                    dm_metadata_base_t        *p_dm_md_hdmi, 
                                    signal_color_space_e  output_color_space,
                                    signal_range_e        output_signal_range,
                                    bool                  f_graphic_on,
                                    uint32_t             *p_length);


/*-----------------------------------------------------------------------------+
 |                            Dolby register  API
 +----------------------------------------------------------------------------*/
typedef enum {
    display_out_sdr_normal,
    display_out_sdr_authentication,
    display_out_hdr10,
    display_out_dolby_ipt,
    display_out_dolby_yuv,
}display_out_type_t;

typedef struct target_info_s{
    uint32_t w,h;
    int16_t Contrast, Brightness;
    display_out_type_t tgt_type;
}target_info_t;

typedef enum {
    src_video_sdr,
    src_video_hdr10,
    src_video_dolby_yuv,
    src_video_dolby_ipt,
}src_video_type_t;

typedef struct src_videoinfo_s{
    uint32_t w,h;
    uint32_t bits;
    src_video_type_t video_type;
}src_videoinfo_t;

typedef struct src_grcinfo_s{
    uint32_t w,h;
}src_grcinfo_t;

typedef struct dolby_vision_info_s{
	char *pbuild_time;
    char *pversion;
    char *puser_name;
    uint32_t user_id[4];
}dolby_vision_info_t;

extern int reg_init(void);
extern int reg_deinit(void);

/*! @brief Commits the 
 *  @param[in]  *p_src_dm_metadata   Source DM Metadata                      
 *  @param[in]  *p_src_comp_metadata Source Composer Metadata                
 *  @param[in]  video_info           VideoFrame info
 *  @param[out] *p_dst_comp_reg      Generic Register Mapping for Composer.  
 *  @param[out] *p_dst_dm_reg        Generic Register Mapping for DM.        
 *  @return
 *      @li 0       success
 *      @li <0      error
 */
extern int commit_reg(dm_metadata_t *p_src_dm_metadata, DmKsFxp_t *p_dst_dm_reg,
        src_videoinfo_t video_info);

/* w,h must equal with video_info.w&h */
extern int set_display_config(target_info_t tgt_info);

extern int set_reg_debug_dir(char *p_dm_in_file, char *p_out_dir);

extern int set_grc_config(src_grcinfo_t grc_info);
extern int get_dolby_vision_info(dolby_vision_info_t *pdolby_vision_info);


typedef int (* FN_DOLBY_metadata_parser_init)(void);
typedef int (* FN_DOLBY_metadata_parser_deinit)(void);
typedef int (* FN_DOLBY_metadata_parser_process)(char *p_rpu_buf, uint32_t rpu_len,
                                    rpu_ext_config_fixpt_main_t *p_composer_data,
                                    dm_metadata_t *p_dm_data, char *p_md_bin);
typedef void (* FN_DOLBY_bypass_metadata_update)(dm_metadata_base_t  *p_dm_md,
                                    dm_metadata_base_t        *p_dm_md_hdmi, 
                                    signal_color_space_e  output_color_space,
                                    signal_range_e        output_signal_range,
                                    bool                  f_graphic_on,
                                    uint32_t             *p_length);


typedef int (* FN_DOLBY_reg_init)(void);
typedef int (* FN_DOLBY_reg_deinit)(void);
typedef int (* FN_DOLBY_commit_reg)(dm_metadata_t *p_src_dm_metadata, 
                                    DmKsFxp_t *p_dst_dm_reg,
                                    src_videoinfo_t video_info);

typedef int (* FN_DOLBY_set_display_config)(target_info_t tgt_info);
typedef int (* FN_DOLBY_set_grc_config)(src_grcinfo_t grc_info);
typedef int (* FN_DOLBY_get_dolby_vision_info)(dolby_vision_info_t *pdolby_vision_info);

typedef struct
{
    FN_DOLBY_metadata_parser_init      pfnMetadata_parser_init;
    FN_DOLBY_metadata_parser_deinit    pfnMetadata_parser_deinit;
    FN_DOLBY_metadata_parser_process   pfnMetadata_parser_process;

    FN_DOLBY_bypass_metadata_update    pfnBypass_metadata_update;
    
    FN_DOLBY_reg_init                  pfnReg_init;
    FN_DOLBY_reg_deinit                pfnReg_deinit;
    FN_DOLBY_commit_reg                pfnCommit_reg;
    FN_DOLBY_set_display_config        pfnSet_display_config;
    FN_DOLBY_set_grc_config            pfnSet_grc_config;
    FN_DOLBY_get_dolby_vision_info     pfnget_dolby_vision_info;
}DOLBY_FUNC_ENTRY_S;




