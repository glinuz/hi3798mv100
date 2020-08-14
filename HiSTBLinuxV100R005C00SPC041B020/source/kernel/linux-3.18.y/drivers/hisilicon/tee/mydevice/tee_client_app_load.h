/**
 * @file tee_client_app_load.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义非安全客户端接口\n
 */

#include "tee_client_type.h"

#ifndef _TEE_CLIENT_APP_LOAD_H_
#define _TEE_CLIENT_APP_LOAD_H_

#define MAX_FILE_PATH_LEN 20
#define MAX_FILE_NAME_LEN 40
#define MAX_FILE_EXT_LEN  6

#define MAX_IMAGE_LEN 0x800000      /* max image len */
#define MAX_SHARE_BUF_LEN 0x400000  /* max share buf len */
#define LOAD_IMAGE_FLAG_OFFSET 0x4
#define SEND_IMAGE_LEN (MAX_SHARE_BUF_LEN - LOAD_IMAGE_FLAG_OFFSET)

typedef struct{
    uint32_t context_len;         /* manifest_crypto_len + cipher_bin_len */
    uint32_t manifest_crypto_len; /* manifest crypto len */
    uint32_t manifest_plain_len;  /* manfiest str + manifest binary */
    uint32_t manifest_str_len;    /* manifest str len*/
    uint32_t cipher_bin_len;      /* cipher elf len */
}TEEC_image_head;

/*
 * Function:       TEEC_StartApp
 * Description:    This function starts an app
 * Parameters:     context: a pointer to an initialized TEE Context.
 *                 srv_uuid: a pointer to the secure service application UUID.
 * Return:         > 0: application load success
 *                 == 0: no need to load application
 *                 < 0: erro
 */
int32_t TEEC_StartApp(
        TEEC_Context* context,
        const TEEC_UUID *srv_uuid);

#endif
/**
 * History: \n
 * 2013-5-12 l00202565: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */
