#ifndef __TEE_CONFIG_H_
#define __TEE_CONFIG_H_
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * HELLO_WORLD TA
 * 79b77788-9789-4a7a-a2be-b60155eef5f3
 */
#define TEE_SERVICE_HELLOWORLD \
{ \
    0x79b77788, \
    0x9789, \
    0x4a7a, \
    { \
        0xa2, 0xbe, 0xb6, 0x01, 0x55, 0xee, 0xf5, 0xf3 \
    } \
}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 *  KERNEL_MEM_USAGE TA
 * 04040404-9789-4a7a-a2be-b60155eef5f3
 */
#define TEE_SERVICE_KERNELMEMUSAGE \
{ \
    0x04040404, \
    0x9789, \
    0x4a7a, \
    { \
        0xa2, 0xbe, 0xb6, 0x01, 0x55, 0xee, 0xf5, 0xf3 \
    } \
}


/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * TUI_DEMO TA
 * 8a613138-5288-e6ac-3f3f-fb4790f7b1fa
 */
#define TEE_SERVICE_TUI_DEMO \
{ \
    0x8a613138, \
    0x5288, \
    0xe6ac, \
    { \
        0x3f, 0x3f, 0xfb, 0x47, 0x90, 0xf7, 0xb1, 0xfa \
    } \
}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * TIMER_UT TA
    19B39980-2487-7B84-F41A-BC892262BB3D
 */
#define TEE_SERVICE_TIMER_UT \
{ \
    0x19B39980, \
    0x2487, \
    0x7B84, \
    { \
        0xf4, 0x1a, 0xbc, 0x89, 0x22, 0x62, 0xbb, 0x3d \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * PERMISSIONCONTROL_UT TA
 * F1AE5991-F36A-84A4-EE9F-234B37FBBE69
 */
#define TEE_SERVICE_PERMCTRL_UT \
{ \
    0xF1AE5991, \
    0xF36A, \
    0x84A4, \
    { \
        0xee, 0x9f, 0x23, 0x4b, 0x37, 0xfb, 0xbe, 0x69 \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * FingerPrint TA
 * a32b3d00-cb57-11e3-9c1a-0800200c9a66.sec
 */
#define TEE_SERVICE_FINGERPRINT \
{ \
    0xa32b3d00, \
    0xcb57, \
    0x11e3, \
    { \
        0x9c, 0x1a, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 \
    } \
}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * FingerPrint Coating check TA
 * e5c6a727-c219-aa13-3e14-444d853a200a.sec
 */

#define TEE_FINGERPRINT_COATING_CHECK  \
{ \
    0xe5c6a727, \
    0xc219, \
    0xaa13, \
    { \
        0x3e, 0x14, 0x44, 0x4d, 0x85, 0x3a, 0x20, 0x0a   \
    } \
}


/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * FingerPrint Sensor check TA
 * use this TA to check the fp senor:fpc or other vendor
 * fd1bbfb2-9a62-4b27-8fdb-a503529076af.sec
 */

#define TEE_FINGERPRINT_SENSOR_CHECK  \
{ \
    0xfd1bbfb2, \
    0x9a62, \
    0x4b27, \
    { \
        0x8f, 0xdb, 0xa5, 0x03, 0x52, 0x90, 0x76, 0xaf   \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * SignTool TA:used to store huawei VIP key
 * 9b17660b-8968-4eed-917e-dd32379bd548
 */
#define TEE_SERVICE_SIGNTOOL \
{ \
    0x9b17660b, \
    0x8968, \
    0x4eed, \
    { \
        0x91, 0x7e, 0xdd, 0x32, 0x37, 0x9b, 0xd5, 0x48 \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * product line RPMB key write TA
 * 6c8cf255-ca98-439e-a98e-ade64022ecb6
 */
#define TEE_SERVICE_RPMBKEY \
{ \
    0x6c8cf255, \
    0xca98, \
    0x439e, \
    { \
        0xa9, 0x8e, 0xad, 0xe6, 0x40, 0x22, 0xec, 0xb6 \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * product line spi bus check
 * 868ccafb-794b-46c6-b5c4-9f1462de4e02
 */
#define TEE_SERVICE_HARDWARECHECK \
    { \
    0x868ccafb, \
	0x794b, \
	0x46c6, \
    { \
        0xb5, 0xc4, 0x9f, 0x14, 0x62, 0xde, 0x4e, 0x02 \
	    } \
	}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * Huawei wallet TA:used to check spi in product line
 * 4ae7ba51-2810-4cee-abbe-a42307b4ace3
 */
#define TEE_SERVICE_HUAWEIWALLET \
{ \
    0x4ae7ba51, \
    0x2810, \
    0x4cee, \
    { \
        0xab, 0xbe, 0xa4, 0x23, 0x07, 0xb4, 0xac, 0xe3 \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * SKYTONE TA
 * abe89147-cd61-f43f-71c4-1a317e405312.sec
 */
#define TEE_SERVICE_SKYTONE \
{ \
    0xabe89147, \
    0xcd61, \
    0xf43f, \
    { \
        0x71, 0xc4, 0x1a, 0x31, 0x7e, 0x40, 0x53, 0x12 \
    } \
}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * SKYTONE TA
 * 3a8bddbc-7151-11e5-9d70-feff819cdc9f.sec
 */
#define TEE_SERVICE_SKYTONE_UI \
    { \
    0x3a8bddbc, \
	0x7151, \
	0x11e5, \
    { \
        0x9d, 0x70, 0xfe, 0xff, 0x81, 0x9c, 0xdc, 0x9f \
	    } \
	}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * crypto sms TA:used to encrypt sms in carrara
 * 79b77788-9789-4a7a-a2be-b60155eef5f4
 */
#define TEE_SERVICE_CRYPTOSMS \
{ \
    0x79b77788, \
    0x9789, \
    0x4a7a, \
    { \
        0xa2, 0xbe, 0xb6, 0x01, 0x55, 0xee, 0xf5, 0xf4 \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * alipay TA from zhifubao:used for zhifubao
 * 66302DF4-31F2-1782-042F-A55C7466DA21
 */
#define TEE_SERVICE_ALIPAY \
{ \
    0x66302DF4, \
    0x31F2, \
    0x1782, \
    { \
        0x04, 0x2F, 0xA5, 0x5C, 0x74, 0x66, 0xDA, 0x21 \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * wechat TA from tencent:used for wechar pay
 * B7C9A7FD-851E-7761-07DF-8AB7C0B02787
 */
#define TEE_SERVICE_WECHAT \
{ \
    0xB7C9A7FD, \
    0x851E, \
    0x7761, \
    { \
        0x07, 0xdf, 0x8a, 0xb7, 0xc0, 0xb0, 0x27, 0x87 \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * antitheft TA from huawei:used for device antitheft
 * b4b71581-add2-e89f-d536-f35436dc7973
 */

#define TEE_SERVICE_Antitheft \
{ \
    0xB4B71581, \
    0xADD2, \
    0xE89F, \
    { \
        0xD5, 0x36, 0xF3, 0x54, 0x36, 0xDC, 0x79, 0x73   \
    } \
}
/**
* @ingroup  TEE_CONFIG_DATA
*
* widevine TA
* a3d05777-b0ec-43b0-8887-a7f93697830a a3d05777-b0ec-43b0-8887-a7f93697830a.sec
*/
#define TEE_SERVICE_WIDEVINE_DRM \
{ \
	0xA3D05777, \
	0xB0EC, \
	0x43B0, \
	{ \
		0x88, 0x87, 0xA7, 0xF9, 0x36, 0x97, 0x83, 0x0A \
	} \
}
/* @ingroup  TEE_CONFIG_DATA
*
* FIDO TA from huawei
* 883890ba-3ef8-4f0b-9c02-f5874acbf2ff
*/

#define TEE_SERVICE_FIDO \
    { \
	0x883890ba, \
	0x3ef8, \
	0x4f0b, \
    { \
        0x9c, 0x02, 0xf5, 0x87, 0x4a, 0xcb, 0xf2, 0xff   \
    } \
}

/* @ingroup  TEE_CONFIG_IFAA
*
* IFAA TA from huawei
* 993e26b8-0273-408e-98d3-60c997c37121
*/
#define TEE_SERVICE_IFAA \
    { \
	0x993e26b8, \
	0x0273, \
	0x408e, \
    { \
        0x98, 0xd3, 0x60, 0xc9, 0x97, 0xc3, 0x71, 0x21   \
    } \
}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * sensor info TA from huawei to check fingerprint sensor type
 * fd1bbfb2-9a62-4b27-8fdb-a503529076af
 */

#define TEE_SERVICE_SENSORINFO \
    { \
        0xfd1bbfb2, \
        0x9a62, \
        0x4b27, \
    { \
        0x8f, 0xdb, 0xa5, 0x03, 0x52, 0x90, 0x76, 0xaf   \
    } \
}
/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * mdpp ta
 * 41d21d0f-9757-41f1-8706-b3baa35fe82f
 */

#define TEE_SERVICE_MDPP \
    { \
        0x41d21d0f, \
        0x9757, \
        0x41f1, \
    { \
        0x87, 0x06, 0xb3, 0xba, 0xa3, 0x5f, 0xe8, 0x2f   \
    } \
}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * mmz ta
 * 814e69b9-7646-4467-a94c-6b136c16b5fb
 */

#define TEE_SERVICE_MMZ \
    { \
        0x814e69b9, \
        0x7646, \
        0x4467, \
    { \
        0xa9, 0x4c, 0x6b, 0x13, 0x6c, 0x16, 0xb5, 0xfb   \
    } \
}

/**
 * @ingroup  TEE_CONFIG_DATA
 *
 * endorsement ta
 * 90925dbc-b85b-4ae3-a813-2dc9b4266da9
 */

#define TEE_SERVICE_ENDORSEMENT \
    { \
        0x90925dbc, \
        0xb85b, \
        0x4ae3, \
    { \
        0xa8, 0x13, 0x2d, 0xc9, 0xb4, 0x26, 0x6d, 0xa9   \
    } \
}

/**
 * @ingroup  TEE_SERVICE_IRIS
 *
 * iris ta
 * d28e1250-d4df-496d-9fcc-2181c3a2f4fa
 */

#define TEE_SERVICE_IRIS \
    { \
        0xd28e1250, \
        0xd4df, \
        0x496d, \
    { \
        0x9f, 0xcc, 0x21, 0x81, 0xc3, 0xa2, 0xf4, 0xfa   \
    } \
}

#endif
