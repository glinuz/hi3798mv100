/**
 \file localplay_drm.h
 \brief 
 \author HiSilicon Technologies Co., Ltd.
 \version 1.0
 \author z00187490
 \date 2012-11-6
 */

#ifndef LOCALPLAY_DRM_H_


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * get the internal handle
 */
void* drm_get_handle();

/**
 * clear the internal drm handle
 */
void drm_clear_handle();
void drm_install();
void drm_cmd(const char* arg);

/**
 * return
 *  0 ok
 *  -1 right invalid
 *  1 acquiring right
 */
int drm_check_right_status(const char* path, const char* mime_type);

int drm_acquire_right_progress(const char* mime_type);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#define LOCALPLAY_DRM_H_
#endif /* LOCALPLAY_DRM_H_ */
