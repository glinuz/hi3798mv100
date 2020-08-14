/*=================================================

Open MAX   Component: Video Decoder
Protocal definition for openMAX decoder component.
File:    omx_codec_type.h
Author:  yangyichang 00226912
Date:    16, 03, 2013.

=================================================*/

#ifndef OMX_Codec_Type_h
#define OMX_Codec_Type_h

#include "OMX_VideoExt.h"

/* CodecType Relative */
static const struct codec_info codec_trans_list[] = {

    {OMX_ROLE_VIDEO_DECODER_AVC,      OMX_VIDEO_CodingAVC,       STD_H264},
    {OMX_ROLE_VIDEO_DECODER_MPEG1,    OMX_VIDEO_CodingMPEG1,     STD_MPEG2},
    {OMX_ROLE_VIDEO_DECODER_MPEG2,    OMX_VIDEO_CodingMPEG2,     STD_MPEG2},
    {OMX_ROLE_VIDEO_DECODER_MPEG4,    OMX_VIDEO_CodingMPEG4,     STD_MPEG4},
    {OMX_ROLE_VIDEO_DECODER_H263,     OMX_VIDEO_CodingH263,      STD_H263},
    {OMX_ROLE_VIDEO_DECODER_SORENSON, OMX_VIDEO_CodingSorenson,  STD_SORENSON},
    {OMX_ROLE_VIDEO_DECODER_AVS,      OMX_VIDEO_CodingAVS,       STD_AVS},
    {OMX_ROLE_VIDEO_DECODER_VC1,      OMX_VIDEO_CodingVC1,       STD_VC1},
    {OMX_ROLE_VIDEO_DECODER_WMV,      OMX_VIDEO_CodingWMV,       STD_VC1},
    {OMX_ROLE_VIDEO_DECODER_DIVX3,    OMX_VIDEO_CodingDIVX3,     STD_DIVX3},
    {OMX_ROLE_VIDEO_DECODER_VP6,      OMX_VIDEO_CodingVP6,       STD_VP6},
    {OMX_ROLE_VIDEO_DECODER_VP8,      OMX_VIDEO_CodingVP8,       STD_VP8},
    {OMX_ROLE_VIDEO_DECODER_HEVC,     OMX_VIDEO_CodingHEVC,      STD_HEVC},
    {OMX_ROLE_VIDEO_DECODER_MVC,      OMX_VIDEO_CodingMVC,       STD_MVC},
  #ifdef REAL8_SUPPORT
    {OMX_ROLE_VIDEO_DECODER_RV,       OMX_VIDEO_CodingRV,        STD_REAL8},
  #endif
  #ifdef REAL9_SUPPORT
    {OMX_ROLE_VIDEO_DECODER_RV,       OMX_VIDEO_CodingRV,        STD_REAL9},
  #endif
};


static const struct codec_profile_level avc_profile_level_list[] ={

    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel1},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel1b},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel11},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel12},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel13},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel2},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel21},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel22},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel3},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel31},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel32},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel4},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel41},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel42},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel5},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel51},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel1},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel1b},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel11},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel12},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel13},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel2},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel21},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel22},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel3},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel31},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel32},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel4},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel41},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel42},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel5},
    {OMX_VIDEO_AVCProfileMain,     OMX_VIDEO_AVCLevel51},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel1},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel1b},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel11},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel12},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel13},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel2},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel21},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel22},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel3},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel31},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel32},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel4},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel41},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel42},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel5},
    {OMX_VIDEO_AVCProfileHigh,     OMX_VIDEO_AVCLevel51},

};


static const struct codec_profile_level mpeg4_profile_level_list[] = {

    {OMX_VIDEO_MPEG4ProfileSimple,         OMX_VIDEO_MPEG4Level0},
    {OMX_VIDEO_MPEG4ProfileSimple,         OMX_VIDEO_MPEG4Level0b},
    {OMX_VIDEO_MPEG4ProfileSimple,         OMX_VIDEO_MPEG4Level1},
    {OMX_VIDEO_MPEG4ProfileSimple,         OMX_VIDEO_MPEG4Level2},
    {OMX_VIDEO_MPEG4ProfileSimple,         OMX_VIDEO_MPEG4Level3},
    {OMX_VIDEO_MPEG4ProfileSimple,         OMX_VIDEO_MPEG4Level4},
    {OMX_VIDEO_MPEG4ProfileSimple,         OMX_VIDEO_MPEG4Level4a},
    {OMX_VIDEO_MPEG4ProfileSimple,         OMX_VIDEO_MPEG4Level5},
    {OMX_VIDEO_MPEG4ProfileAdvancedSimple, OMX_VIDEO_MPEG4Level0},
    {OMX_VIDEO_MPEG4ProfileAdvancedSimple, OMX_VIDEO_MPEG4Level0b},
    {OMX_VIDEO_MPEG4ProfileAdvancedSimple, OMX_VIDEO_MPEG4Level1},
    {OMX_VIDEO_MPEG4ProfileAdvancedSimple, OMX_VIDEO_MPEG4Level2},
    {OMX_VIDEO_MPEG4ProfileAdvancedSimple, OMX_VIDEO_MPEG4Level3},
    {OMX_VIDEO_MPEG4ProfileAdvancedSimple, OMX_VIDEO_MPEG4Level4},
    {OMX_VIDEO_MPEG4ProfileAdvancedSimple, OMX_VIDEO_MPEG4Level4a},
    {OMX_VIDEO_MPEG4ProfileAdvancedSimple, OMX_VIDEO_MPEG4Level5},

};


static const struct codec_profile_level mpeg2_profile_level_list[] = {

    { OMX_VIDEO_MPEG2ProfileSimple,  OMX_VIDEO_MPEG2LevelLL},
    { OMX_VIDEO_MPEG2ProfileSimple,  OMX_VIDEO_MPEG2LevelML},
    { OMX_VIDEO_MPEG2ProfileSimple,  OMX_VIDEO_MPEG2LevelH14},
    { OMX_VIDEO_MPEG2ProfileSimple,  OMX_VIDEO_MPEG2LevelHL},
    { OMX_VIDEO_MPEG2ProfileMain,    OMX_VIDEO_MPEG2LevelLL},
    { OMX_VIDEO_MPEG2ProfileMain,    OMX_VIDEO_MPEG2LevelML},
    { OMX_VIDEO_MPEG2ProfileMain,    OMX_VIDEO_MPEG2LevelH14},
    { OMX_VIDEO_MPEG2ProfileMain,    OMX_VIDEO_MPEG2LevelHL},
    { OMX_VIDEO_MPEG2ProfileHigh,    OMX_VIDEO_MPEG2LevelLL},
    { OMX_VIDEO_MPEG2ProfileHigh,    OMX_VIDEO_MPEG2LevelML},
    { OMX_VIDEO_MPEG2ProfileHigh,    OMX_VIDEO_MPEG2LevelH14},
    { OMX_VIDEO_MPEG2ProfileHigh,    OMX_VIDEO_MPEG2LevelHL},

};


static const struct codec_profile_level h263_profile_level_list[] = {

    { OMX_VIDEO_H263ProfileBaseline,           OMX_VIDEO_H263Level10},
    { OMX_VIDEO_H263ProfileBaseline,           OMX_VIDEO_H263Level20},
    { OMX_VIDEO_H263ProfileBaseline,           OMX_VIDEO_H263Level30},
    { OMX_VIDEO_H263ProfileBaseline,           OMX_VIDEO_H263Level40},
    { OMX_VIDEO_H263ProfileBaseline,           OMX_VIDEO_H263Level45},
    { OMX_VIDEO_H263ProfileBaseline,           OMX_VIDEO_H263Level50},
    { OMX_VIDEO_H263ProfileBaseline,           OMX_VIDEO_H263Level60},
    { OMX_VIDEO_H263ProfileBaseline,           OMX_VIDEO_H263Level70},
    { OMX_VIDEO_H263ProfileBackwardCompatible, OMX_VIDEO_H263Level10},
    { OMX_VIDEO_H263ProfileBackwardCompatible, OMX_VIDEO_H263Level20},
    { OMX_VIDEO_H263ProfileBackwardCompatible, OMX_VIDEO_H263Level30},
    { OMX_VIDEO_H263ProfileBackwardCompatible, OMX_VIDEO_H263Level40},
    { OMX_VIDEO_H263ProfileBackwardCompatible, OMX_VIDEO_H263Level45},
    { OMX_VIDEO_H263ProfileBackwardCompatible, OMX_VIDEO_H263Level50},
    { OMX_VIDEO_H263ProfileBackwardCompatible, OMX_VIDEO_H263Level60},
    { OMX_VIDEO_H263ProfileBackwardCompatible, OMX_VIDEO_H263Level70},
    { OMX_VIDEO_H263ProfileISWV2,              OMX_VIDEO_H263Level10},
    { OMX_VIDEO_H263ProfileISWV2,              OMX_VIDEO_H263Level20},
    { OMX_VIDEO_H263ProfileISWV2,              OMX_VIDEO_H263Level30},
    { OMX_VIDEO_H263ProfileISWV2,              OMX_VIDEO_H263Level40},
    { OMX_VIDEO_H263ProfileISWV2,              OMX_VIDEO_H263Level45},
    { OMX_VIDEO_H263ProfileISWV2,              OMX_VIDEO_H263Level50},
    { OMX_VIDEO_H263ProfileISWV2,              OMX_VIDEO_H263Level60},
    { OMX_VIDEO_H263ProfileISWV2,              OMX_VIDEO_H263Level70},

};


static const struct codec_profile_level vc1_profile_level_list[] = {

    { OMX_VIDEO_VC1ProfileSimple,              OMX_VIDEO_VC1LevelLow},
    { OMX_VIDEO_VC1ProfileSimple,              OMX_VIDEO_VC1LevelMedium},
    { OMX_VIDEO_VC1ProfileSimple,              OMX_VIDEO_VC1LevelHigh},
    { OMX_VIDEO_VC1ProfileSimple,              OMX_VIDEO_VC1Level0},
    { OMX_VIDEO_VC1ProfileSimple,              OMX_VIDEO_VC1Level1},
    { OMX_VIDEO_VC1ProfileSimple,              OMX_VIDEO_VC1Level2},
    { OMX_VIDEO_VC1ProfileSimple,              OMX_VIDEO_VC1Level3},
    { OMX_VIDEO_VC1ProfileSimple,              OMX_VIDEO_VC1Level4},
    { OMX_VIDEO_VC1ProfileMain,                OMX_VIDEO_VC1LevelLow},
    { OMX_VIDEO_VC1ProfileMain,                OMX_VIDEO_VC1LevelMedium},
    { OMX_VIDEO_VC1ProfileMain,                OMX_VIDEO_VC1LevelHigh},
    { OMX_VIDEO_VC1ProfileMain,                OMX_VIDEO_VC1Level0},
    { OMX_VIDEO_VC1ProfileMain,                OMX_VIDEO_VC1Level1},
    { OMX_VIDEO_VC1ProfileMain,                OMX_VIDEO_VC1Level2},
    { OMX_VIDEO_VC1ProfileMain,                OMX_VIDEO_VC1Level3},
    { OMX_VIDEO_VC1ProfileMain,                OMX_VIDEO_VC1Level4},
    { OMX_VIDEO_VC1ProfileAdvanced,            OMX_VIDEO_VC1LevelLow},
    { OMX_VIDEO_VC1ProfileAdvanced,            OMX_VIDEO_VC1LevelMedium},
    { OMX_VIDEO_VC1ProfileAdvanced,            OMX_VIDEO_VC1LevelHigh},
    { OMX_VIDEO_VC1ProfileAdvanced,            OMX_VIDEO_VC1Level0},
    { OMX_VIDEO_VC1ProfileAdvanced,            OMX_VIDEO_VC1Level1},
    { OMX_VIDEO_VC1ProfileAdvanced,            OMX_VIDEO_VC1Level2},
    { OMX_VIDEO_VC1ProfileAdvanced,            OMX_VIDEO_VC1Level3},
    { OMX_VIDEO_VC1ProfileAdvanced,            OMX_VIDEO_VC1Level4},

};


static const struct codec_profile_level vp8_profile_level_list[] = {

    { OMX_VIDEO_VP8ProfileMain,            OMX_VIDEO_VP8Level_Version0},
    { OMX_VIDEO_VP8ProfileMain,            OMX_VIDEO_VP8Level_Version1},
    { OMX_VIDEO_VP8ProfileMain,            OMX_VIDEO_VP8Level_Version2},
    { OMX_VIDEO_VP8ProfileMain,            OMX_VIDEO_VP8Level_Version3},

};


static const struct codec_profile_level wmv_profile_level_list[] = {

    { OMX_VIDEO_WMVProfileSimple,            OMX_VIDEO_WMVLevelLow},
    { OMX_VIDEO_WMVProfileSimple,            OMX_VIDEO_WMVLevelMedium},
    { OMX_VIDEO_WMVProfileSimple,            OMX_VIDEO_WMVLevelHigh},
    { OMX_VIDEO_WMVProfileSimple,            OMX_VIDEO_WMVLevelL0},
    { OMX_VIDEO_WMVProfileSimple,            OMX_VIDEO_WMVLevelL1},
    { OMX_VIDEO_WMVProfileSimple,            OMX_VIDEO_WMVLevelL2},
    { OMX_VIDEO_WMVProfileSimple,            OMX_VIDEO_WMVLevelL3},
    { OMX_VIDEO_WMVProfileSimple,            OMX_VIDEO_WMVLevelL4},
    { OMX_VIDEO_WMVProfileMain,              OMX_VIDEO_WMVLevelLow},
    { OMX_VIDEO_WMVProfileMain,              OMX_VIDEO_WMVLevelMedium},
    { OMX_VIDEO_WMVProfileMain,              OMX_VIDEO_WMVLevelHigh},
    { OMX_VIDEO_WMVProfileMain,              OMX_VIDEO_WMVLevelL0},
    { OMX_VIDEO_WMVProfileMain,              OMX_VIDEO_WMVLevelL1},
    { OMX_VIDEO_WMVProfileMain,              OMX_VIDEO_WMVLevelL2},
    { OMX_VIDEO_WMVProfileMain,              OMX_VIDEO_WMVLevelL3},
    { OMX_VIDEO_WMVProfileMain,              OMX_VIDEO_WMVLevelL4},
    { OMX_VIDEO_WMVProfileAdvanced,          OMX_VIDEO_WMVLevelLow},
    { OMX_VIDEO_WMVProfileAdvanced,          OMX_VIDEO_WMVLevelMedium},
    { OMX_VIDEO_WMVProfileAdvanced,          OMX_VIDEO_WMVLevelHigh},
    { OMX_VIDEO_WMVProfileAdvanced,          OMX_VIDEO_WMVLevelL0},
    { OMX_VIDEO_WMVProfileAdvanced,          OMX_VIDEO_WMVLevelL1},
    { OMX_VIDEO_WMVProfileAdvanced,          OMX_VIDEO_WMVLevelL2},
    { OMX_VIDEO_WMVProfileAdvanced,          OMX_VIDEO_WMVLevelL3},
    { OMX_VIDEO_WMVProfileAdvanced,          OMX_VIDEO_WMVLevelL4},

};


/* CodecType Relative */
/*fix here*/
static const struct codec_profile_level hevc_profile_level_list[] ={
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel1},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel1},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel2},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel2},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel21},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel21},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel3},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel3},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel31},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel31},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel4},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel4},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel41},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel41},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel5},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel5},
};
static const struct codec_profile_level mvc_profile_level_list[] ={
    { 0,            0},
};
static const struct codec_profile_level mpeg1_profile_level_list[] = {
    { 0,            0},
};
static const struct codec_profile_level divx3_profile_level_list[] = {
    { 0,            0},
};
static const struct codec_profile_level vp6_profile_level_list[] = {
    { 0,            0},
};
static const struct codec_profile_level avs_profile_level_list[] = {
    { 0,            0},
};
static const struct codec_profile_level sorenson_profile_level_list[] = {
    { 0,            0},
};
static const struct codec_profile_level rv_profile_level_list[] = {
    { 0,            0},
};
static const struct codec_profile_level mjpeg_profile_level_list[] = {
    { 0,            0},
};


#endif

