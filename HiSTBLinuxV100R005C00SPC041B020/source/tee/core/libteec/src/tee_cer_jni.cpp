#include <jni.h>
#include <cutils/log.h>
#include <stddef.h>
#include <fcntl.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/bio.h>
#include "securec.h"

//#define LOG_NDK
#define LOG_TAG     "testServiceJNI"
#define JAVA_CLASS_TO_REGIST_METHOD        "com/example/test_service/MainActivity"
#define PACKAGEMANAGER_GET_SIGNATURES       64
#define MAX_PATH_LENGTH      256

#define LOGE ALOGE
#define LOGV

extern "C"     int get_app_cert(unsigned char **publickeys, int *total_size, int *cer_num);

/* TODO:
 * the dump_data() fun may be move to log module, and rewrite it,
 * because the pdata may overread!!!
 */
/*
void dump_data(char *info, unsigned char * data, unsigned int len)
{
    char  buf[100] = {0};
    int i, j, k;
    char *pbuf = buf;
    char *pdata = (char *)data;

	if (NULL == info || NULL == data) {
		LOGV("Parameter fault!");
		return;
	}
	if (len > 1024) {
    {
        len = 1024;
    };

    k = len / 32;

    for (j = 0; j < k; j++)
    {
        for (i = 0; i < 32; i++)
        {
            sprintf(pbuf, "%02X", *pdata++);
            pbuf += 2;
        }

        buf[64] = 0;
        LOGV("%s : %s", info, buf);
        pbuf = buf;
    }

    k = len % 32;

    if (k == 0)
    {
        return;
    }

    for (i = 0; i < k; i++)
    {
        sprintf(pbuf , "%02X", *pdata++);
        pbuf += 2;
    }

    *pbuf = 0;
    LOGV("%s : %s", info, buf);
}
*/

// UserHandle.getUserId(myUid())
jint get_user_id(JNIEnv *jniEnv)
{
	jint ret = -1;
	jclass clazz;
	jmethodID method;

	if (NULL == jniEnv) {
		LOGV("Parameter fault!");
		return ret;
	}
	clazz = jniEnv->FindClass("android/os/UserHandle");

	if (clazz == NULL) {
		LOGE("find class: android/os/UserHandle failed!");
		return ret;
	}

	method = jniEnv->GetStaticMethodID(clazz, "getUserId", "(I)I");
	if (method == NULL) {
		LOGE("find methodID: getUserId  \"(I)I\" failed!");
		return ret;
	}

	// invoke ActivityThread.getPackageManager();
	ret = jniEnv->CallStaticIntMethod(clazz, method, getuid());
	LOGV("get_user_id = %d", ret);

	return ret;
}

void check_exception(JNIEnv *env)
{
	if (NULL == env) {
		LOGV("Parameter fault!");
		return;
	}
//    jthrowable throwable;
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		LOGV("exception occured!");
	}
}

int get_jni_env(JNIEnv **jniEnv)
{
	int ret = -1;
	int size = 0;
	JavaVM **buffer;

	if (NULL == jniEnv) {
		LOGV("Parameter fault!");
		return;
	}
	JNI_GetCreatedJavaVMs(NULL, 0, &size);           // 1. just get the required array length
	LOGV("JNI_GetCreatedJavaVMs , size = %d", size);
	if (!size) {
		LOGE("JNI_GetCreatedJavaVMs failed");
		return ret;
	}

	buffer = new JavaVM*[size];
	JNI_GetCreatedJavaVMs(buffer, size, &size);     // 2. get the data
	if (buffer[0] != NULL) {
		if (buffer[0]->GetEnv((void **) jniEnv, JNI_VERSION_1_4) == JNI_OK) { // get jniEnv
			ret = 0;
		} else {
			LOGE("GetEnv failed!");
		}
	}

	delete [] buffer;
	return ret;
}
static unsigned int convert_str2dex(char byte)
{
	if (byte >= '0' && byte <= '9')
		return (byte - '0');
	if (byte >= 'A' && byte <= 'F')
		return (byte - 'A' + 10);
	if (byte >= 'a' && byte <= 'f')
		return (byte - 'a' + 10);
	return 0;
}
/**
 * 1.get current package name,
 * 2.get package manger, call static function ActivityThread.getPackageManger()
 * 3.get package info, call object function pm.getPackageInfo()
 * 4.get signature array object, call object access function;
 * 5.get all signature data , and dump to log.
 *
 * note:Only one cer is supported now,multi cers should be implimented
 */
//JNIEXPORT jint JNICALL get_app_cert(JNIEnv *e, jobject thiz)
int get_app_cert(unsigned char **publickeys, int *total_size, int *cer_num)
{
	LOGV("%s", __func__);
	char path[MAX_PATH_LENGTH] = {0};
	int fd;
	jint ret = -1;

	JNIEnv *jniEnv = NULL;
	jsize size;
	jclass clazz;
	jmethodID method;
	jobject   object;
	jfieldID  field;
	jobjectArray  object_array;
	jbyteArray sig_data;

	unsigned char *data = NULL;
	const unsigned char *temp = NULL;
	X509 *cert = NULL;
	char *modulus = NULL;
	unsigned char *modulus_temp = NULL;
	int modulus_size = 0;
	char *exponent = NULL;
	unsigned char *exponent_temp = NULL;
	int exponent_size = 0;
	unsigned char *publickeys_temp = NULL;

	if (NULL == publickeys || NULL == total_size || NULL == cer_num) {
		LOGV("Parameter fault!");
		return ret;
	}
	// step 1, get package name
	ret = snprintf_s(path, MAX_PATH_LENGTH, "/proc/%d/cmdline", getpid());
	if (ret != EOK) {
		TEEC_Error("snprintf_s erro!\n");
		return ret;
	}
	fd = open(path, O_RDONLY);
	ret = memset_s(path, MAX_PATH_LENGTH, 0x00, MAX_PATH_LENGTH);
	if (ret != EOK) {
		TEEC_Error("memset_s erro!\n");
		return ret;
	}
	read(fd, path, MAX_PATH_LENGTH);
	close(fd);

	LOGV("package name = %s", path);

	if (get_jni_env(&jniEnv)) {
		LOGE("get java vm env fail\n");
		return -1;
	}

	// step 2, get package manager
	clazz = jniEnv->FindClass("android/app/ActivityThread");

	if (clazz == NULL) {
		LOGE("find class: android/app/ActivityThread failed!");
		goto clean_up;
	}

	method = jniEnv->GetStaticMethodID(clazz, "getPackageManager", "()Landroid/content/pm/IPackageManager;");
	if (method == NULL) {
		LOGE("find methodID: getPackageManager  \"()Landroid/content/pm/IPackageManager\"; failed!");
		goto clean_up;
	}

	// invoke ActivityThread.getPackageManager();
	object = jniEnv->CallStaticObjectMethod(clazz, method);
	if (object == NULL) {
		LOGE("call methodID: getPackageManager  failed!");
		goto clean_up;
	}

	// step 3, get Package Info
	clazz = jniEnv->FindClass("android/content/pm/IPackageManager");
	if (clazz == NULL) {
		LOGE("find class: android/content/pm/IPackageManager failed!");
		goto clean_up;
	}

	method = jniEnv->GetMethodID(clazz, "getPackageInfo", "(Ljava/lang/String;II)Landroid/content/pm/PackageInfo;");
	if (method == NULL) {
		LOGE("find methodID: getPackageInfo  \"(Ljava/lang/String;II)Landroid/content/pm/PackageInfo;\"; failed!");
		goto clean_up;
	}

	// invoke PackageManager.getPackageInfo(String packageName, int flags, int userId);
	object = jniEnv->CallObjectMethod(object, method, jniEnv->NewStringUTF(path), PACKAGEMANAGER_GET_SIGNATURES, get_user_id(jniEnv));

	if (object == NULL) {
		LOGE("call methodID: getPackageInfo  failed!");
		goto clean_up;
	}

	// step 4, get signature array object
	clazz = jniEnv->FindClass("android/content/pm/PackageInfo");
	if (clazz == NULL) {
		LOGE("find class: android/content/pm/PackageInfo failed!");
		goto clean_up;
	}

	field = jniEnv->GetFieldID(clazz, "signatures", "[Landroid/content/pm/Signature;");
	if (field == NULL) {
		LOGE("get field: signatures failed!");
		goto clean_up;
	}

	//access field signatures
	object_array = (jobjectArray)jniEnv->GetObjectField(object, field);
	if (object_array == NULL) {
		LOGE("get signatures failed!");
		goto clean_up;
	}

	size = jniEnv->GetArrayLength(object_array);
	LOGV("signatures size = %d ", size);
	*cer_num = size;

	// TODO get signature, we just get the first signature for test!
	object = jniEnv->GetObjectArrayElement(object_array, 0);
	if (object == NULL) {
		LOGE("get signature failed!");
		goto clean_up;
	}

	// step 5 , get signature and dump to log
	clazz = jniEnv->FindClass("android/content/pm/Signature");
	if (clazz == NULL) {
		LOGE("find class: android/content/pm/Signature failed!");
		goto clean_up;
	}

	method = jniEnv->GetMethodID(clazz, "toByteArray", "()[B");
	if (method == NULL) {
		LOGE("find methodID: toByteArray  \"()[B;\"; failed!");
		goto clean_up;
	}

	// invoke Signature.toByteArray()
	sig_data = (jbyteArray)jniEnv->CallObjectMethod(object, method);
	if (sig_data == NULL) {
		LOGE("get signature data fail!");
		goto clean_up;
	}

	size = jniEnv->GetArrayLength(sig_data);
	LOGV("signature data size = %d ", size);

	data = new unsigned char [size];
	if (data == NULL) {
		LOGE("out of memory!");
		goto clean_up;
	}

	jniEnv->GetByteArrayRegion(sig_data, 0, size, (jbyte *)data);

	//dump_data("sig_data:", data, size);

	//get exponent and modulus from certificate start
	//buffer send to SecureWorld: modulus_size(4 bytes) + modulus + exponent_size(4 bytes) + exponent
	temp = data;
	cert = d2i_X509(NULL, &temp, size);
	if (!cert) {
		LOGE("unable to parse certificate in memory\n");
		goto clean_up;
	}
	modulus = BN_bn2hex(X509_get_pubkey(cert)->pkey.rsa->n);
	exponent = BN_bn2hex(X509_get_pubkey(cert)->pkey.rsa->e);
	LOGV("------exponent %s, modulus %s\n", exponent, modulus);
	LOGV("------exponent size %d, modulus size %d\n", strlen(exponent), strlen(modulus));
	modulus_size = strlen(modulus) / 2;
	exponent_size = strlen(exponent) / 2;
	publickeys_temp = (unsigned char *)malloc((modulus_size + exponent_size + 8) * (*cer_num));
	if (!publickeys_temp) {
		LOGE("*publickeys malloc failed\n");
		goto clean_up;
	}
	ret = memset_s(publickeys_temp, (modulus_size + exponent_size + 8) * (*cer_num),
		 0, (modulus_size + exponent_size + 8) * (*cer_num));
	if (ret != EOK) {
		TEEC_Error("memset_s erro!\n");
		return ret;
	}
	modulus_temp = (unsigned char *)malloc(modulus_size);
	if (!modulus_temp) {
		LOGE("modulus_temp malloc failed\n");
		goto clean_up;
	}
	ret = memset_s(modulus_temp, modulus_size, 0, modulus_size);
	if (ret != EOK) {
		TEEC_Error("memset_s erro!\n");
		return ret;
	}
	exponent_temp = (unsigned char *)malloc(exponent_size);
	if (!exponent_temp) {
		LOGE("exponent_temp malloc failed\n");
		goto clean_up;
	}
	ret = memset_s(exponent_temp, exponent_size, 0, exponent_size);
	if (ret != EOK) {
		TEEC_Error("memset_s erro!\n");
		return ret;
	}

	for (int i = 0; i < modulus_size; i++) {
		*(modulus_temp + i) = convert_str2dex(*(modulus + i * 2)) << 4 | convert_str2dex(*(modulus + i * 2 + 1));
	}
	for (int i = 0; i < exponent_size; i++) {
		*(exponent_temp + i) = convert_str2dex(*(exponent + i * 2)) << 4 | convert_str2dex(*(exponent + i * 2 + 1));
	}
	//dump_data("n:", modulus_temp, modulus_size);
	//dump_data("e:", exponent_temp, exponent_size);
	*(unsigned int *)publickeys_temp = modulus_size;
	ret = memcpy_s(publickeys_temp + 4, modulus_size, modulus_temp, modulus_size);
	if (ret != EOK) {
		TEEC_Error("memcpy_s erro!\n");
		return ret;
	}
	*(unsigned int *)(publickeys_temp + 4 + modulus_size) = exponent_size;
	ret = memcpy_s(publickeys_temp + 8 + modulus_size, exponent_size, exponent_temp, exponent_size);
	if (ret != EOK) {
		TEEC_Error("memcpy_s erro!\n");
		return ret;
	}
	//get exponent and modulus from certificate end
	*total_size = modulus_size + exponent_size + 8;
	*publickeys = publickeys_temp;
	LOGV("total_size = %d ", *total_size);

	check_exception(jniEnv);

	LOGV("a long way to go!");
	ret = 0;
clean_up:
	if (data)
		delete [] data;
	if (modulus)
		OPENSSL_free(modulus);
	if (exponent)
		OPENSSL_free(exponent);
	if (modulus_temp)
		free(modulus_temp);
	if (exponent_temp)
		free(exponent_temp);
	return ret;
}
