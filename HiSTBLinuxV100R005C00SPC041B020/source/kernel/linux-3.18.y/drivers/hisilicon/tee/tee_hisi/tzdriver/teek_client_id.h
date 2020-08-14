/**
 * @file tee_client_id.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：安全世界提供的安全服务与命令定义头文件\n
 */
#ifndef _TEE_CLIENT_ID_H_
#define _TEE_CLIENT_ID_H_

/**
 *  * @ingroup  TEE_ID
 *   *
 *    * secboot靠
 *     */
#define TEE_SERVICE_SECBOOT \
{ \
	0x08080808, \
	0x0808, \
	0x0808, \
	{ \
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 \
	} \
}

/*e7ed1f64-4687-41da-96dc-cbe4f27c838f*/
#define TEE_SERVICE_ANTIROOT \
{ \
	0xE7ED1F64, \
	0x4687, \
	0x41DA, \
	{ \
		0x96, 0xDC, 0xCB, 0xE4, 0xF2, 0x7C, 0x83, 0x8F \
	} \
}

/**
 * @ingroup  TEE_COMMON_DATA
 *
 * 安全服务secboot支持的命令ID
 */
enum SVC_SECBOOT_CMD_ID {
	SECBOOT_CMD_ID_INVALID = 0x0,	/**< Secboot Task 无效ID*/
	SECBOOT_CMD_ID_COPY_VRL,	/**< Secboot Task 拷贝VRL*/
	SECBOOT_CMD_ID_COPY_DATA,	/**< Secboot Task 拷贝镜像*/
	SECBOOT_CMD_ID_VERIFY_DATA,	/**< Secboot Task 验证*/
	SECBOOT_CMD_ID_RESET_IMAGE,	/**< Secboot Task 复位SoC请求*/
	SECBOOT_CMD_ID_COPY_VRL_TYPE,  /**< Secboot Task 拷贝VRL，并传递SoC Type*/
	SECBOOT_CMD_ID_COPY_DATA_TYPE,	/**< Secboot Task 拷贝镜像,并传递SoC Type*/
	SECBOOT_CMD_ID_VERIFY_DATA_TYPE, /**< Secboot Task 校验，并传递SoC Type，校验成功解复位SoC*/
	 /**< Secboot Task原地校验，并传递SoC Type,校验成功解复位SoC*/
	SECBOOT_CMD_ID_VERIFY_DATA_TYPE_LOCAL,
	SECBOOT_CMD_ID_BSP_MODEM_CALL,           /**< Secboot Task 执行对应函数*/
	SECBOOT_CMD_ID_BSP_MODULE_VERIFY,        /**< Secboot Task modem module校验函数*/
};

/**
 * @ingroup TEE_COMMON_DATA
 *
 * 安全服务secboot支持的镜像类型
 */
enum SVC_SECBOOT_IMG_TYPE {
	MODEM,
	HIFI,
	DSP,
	XDSP,
	TAS,
	WAS,
	CAS,
	MODEM_DTB,
    ISP,
	SOC_MAX
};
/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全服务Global支持的命令ID
 */
enum SVC_GLOBAL_CMD_ID {
	GLOBAL_CMD_ID_INVALID = 0x0,	/**< 安全服务Global，无效的命令*/
	GLOBAL_CMD_ID_BOOT_ACK,	   /**< 安全服务Global，启动应答*/
	GLOBAL_CMD_ID_OPEN_SESSION,    /**< 安全服务Global，打开Session*/
	GLOBAL_CMD_ID_CLOSE_SESSION,	/**< 安全服务Global，关闭Session*/
	GLOBAL_CMD_ID_LOAD_SECURE_APP,
	/**< 安全服务Global，动态加载安全应用*/
	GLOBAL_CMD_ID_NEED_LOAD_APP,/**< 安全服务Global，判断是否是需要加载安全应用*/
	GLOBAL_CMD_ID_REGISTER_AGENT,
	/**< 安全服务Global，注册代理*/
	GLOBAL_CMD_ID_UNREGISTER_AGENT,
	/**< 安全服务Global，注销代理*/
	GLOBAL_CMD_ID_REGISTER_NOTIFY_MEMORY,
	/**< 安全服务Global，注册异步调用缓冲区*/
	GLOBAL_CMD_ID_UNREGISTER_NOTIFY_MEMORY,
	 /**< 安全服务Global，注销异步调用缓冲区*/
	GLOBAL_CMD_ID_INIT_CONTENT_PATH,  /**< 安全服务Global，初始化content path*/
	GLOBAL_CMD_ID_TERMINATE_CONTENT_PATH,
	 /**< 安全服务Global，释放content path*/
	GLOBAL_CMD_ID_ALLOC_EXCEPTION_MEM,
	/**< 安全服务Global，分配异常信息保存空间*/
	GLOBAL_CMD_ID_TEE_TIME,	    /**< 安全服务Global，获取安全OS的时间 */
	GLOBAL_CMD_ID_TEE_INFO,	   /** tlogcat 靠靠OS  靠 */
	GLOBAL_CMD_ID_KILL_TASK,   /** Global Task Kill session */
	GLOBAL_CMD_ID_SET_CA_HASH,   /*set ca hash info*/
	GLOBAL_CMD_ID_MAX,    /**< 安全服务Global，未支持的命令*/
};

#endif
/**
 * History: \n
 * 2013-4-12 h00211444: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
