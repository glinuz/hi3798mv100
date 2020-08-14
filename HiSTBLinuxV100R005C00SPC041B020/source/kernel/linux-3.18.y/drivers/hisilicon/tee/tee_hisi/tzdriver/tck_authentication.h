/*
 * file: tck_authentication.h
 * author: wangdedong
 * create date: 2016/8/4
 * description: The file excutes the ca's identification check.
 * It will send the xml id info to the tzdriver, and then transmit
 * to the secure os.
 */
#ifndef __TCK_AUTHENTICATION__
#define __TCK_AUTHENTICATION__
/* This buffer used for calculate the task hash */
extern unsigned char g_ca_auth_hash_buf[MAX_SHA_256_SZ];
#endif /*__TCK_AUTHENTICATION__*/
