#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> /*for errno*/
#include <fcntl.h>
#ifndef WITH_DEVCHIP_PLATFORM
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/bio.h>
#include <openssl/crypto.h>
#endif
#ifdef _ANDROID_PLATFORM_HISI
#include <cutils/log.h>
#include <cutils/properties.h>
#endif
#include "tee_client_storage_api.h"
#include "tee_client_type.h"

/* #define TEEC_DEBUG */
#include "tee_client_api.h"

#include "tee_ca_daemon.h"
#include "securec.h"

/* #define tlogd */
#include "tee_log.h"

#define SHELL_UID (2000)

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "teecd"
#ifndef WITH_DEVCHIP_PLATFORM
static const xmlChar **appCerts;
#endif
static int appCertsSize;
#define MAX_PATH_LENGTH      256

#ifndef WITH_DEVCHIP_PLATFORM
/*
 * May it be moved to common lib module
 */
static unsigned int convert_str2dex(char byte)
{
	if (byte >= '0' && byte <= '9')
		return (unsigned int)(byte - '0'); /*lint !e571 */
	if (byte >= 'A' && byte <= 'F')
		return (unsigned int)((byte - 'A') + 10);/*lint !e571 */
	if (byte >= 'a' && byte <= 'f')
		return (unsigned int)((byte - 'a') + 10);/*lint !e571 */
	return 0;
}


/*
 * May it be moved to common lib module
 */
char hexToByte(char c)
{
	if ((c >= '0') && (c <= '9'))
		return (c - '0');
	else if ((c >= 'A') && (c <= 'F'))
		return ((c - 'A') + 10);
	else if ((c >= 'a') && (c <= 'f'))
		return ((c - 'a') + 10);
	else
		return 0;
}

/*
 * May it be moved to common lib module
 */
int strToByte(char *result, char *str)
{
	unsigned int i;
	char n1, n2;
	unsigned int len = strlen(str);

	for (i = 0; i < len / 2; i++) {
		n1 = hexToByte(str[2 * i]); /*lint !e679 */
		n2 = hexToByte(str[2 * i + 1]); /*lint !e679 */
		result[i] = (char)(((unsigned char)n1) <<
				4 | (unsigned char)n2);
	}
	return (int)(len / 2);
}

int getInstalledAppCerts(xmlDocPtr doc)
{
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;
	xmlNodeSetPtr nodes;
	xmlNodePtr cur;
	const xmlChar *tmp;

	int size;
	int i, index;

	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		tloge("unable to create new XPath context\n");
		xmlXPathFreeContext(xpathCtx);
		return -1;
	}

	xpathObj = xmlXPathEvalExpression(
			(const xmlChar *)"//package/sigs/cert[@key]",
			xpathCtx);
	if (xpathObj == NULL) {
		tloge("unable to evaluate xpath expression\n");
		xmlXPathFreeContext(xpathCtx);
		return -1;
	}

	nodes = xpathObj->nodesetval;
	size = (nodes) ? nodes->nodeNr : 0;
	if (size <= 0) {
		tloge("size = %d", size);
		xmlXPathFreeContext(xpathCtx);
		return -1;
	}

	if (appCerts != NULL && appCertsSize > 0) {
		for (i = 0; i < appCertsSize; i++)
			xmlFree((xmlChar *)appCerts[i]);
		free(appCerts);
	}

	appCertsSize = size;
	appCerts = malloc(sizeof(char *) * (size_t)size);
	if (appCerts == NULL) {
		xmlXPathFreeContext(xpathCtx);
		return -1;
	}

	for (i = 0; i < size; i++) {
		if (NULL == nodes) {
			tloge("cannot find struct xpathObjUid->nodesetval\n");
			return -1;
		}
		cur = nodes->nodeTab[i];
		tmp = xmlGetProp(cur, (const xmlChar *)"index");
		if (NULL == tmp) {
			xmlXPathFreeContext(xpathCtx);
			return -1;
		}
		index = atoi((char const *)tmp);
		xmlFree((void *)tmp);

		tmp = xmlGetProp(cur, (const xmlChar *)"key");
		if (index < appCertsSize)
			appCerts[index] = tmp;
	}

	xmlXPathFreeContext(xpathCtx);
	return 0;
}

int getCertsForPackage(xmlDocPtr doc,
				const char *pkgName,
				unsigned int uid,
				const xmlChar **certs)
{
	xmlXPathContextPtr xpathCtx, xpathCtxUid;
	xmlXPathObjectPtr xpathObj, xpathObjUid;
	xmlNodeSetPtr nodes, nodesUid;
	xmlNodePtr cur, curUid;
	const xmlChar *tmp;
	const xmlChar *tmpForUid;
	const int EVAL_LEN = 1024;
	char eval[EVAL_LEN] = {0};

	int size;
	int ret;
	bool cert_match = false;
	int sizeOfCertForUid;
	int index, i, indexForUid = 0, j;

	/* com.android.shell has the same cert with framework
	*  So when malicious c program fake is cmdline to
	*  com.huawei.hidisk pacage name. it also can get the system cert.
	*  Then the malicious can use storage_task in trustedcore.
	*  To stop the attack, when UID is SHELL, just return -1,
	*  then pass the uid to the TrustedCore.
	*/
	if (uid == SHELL_UID) {
		tloge("uid is shell UID, just return -1\n");
		return -1;
	}

	/*1. use package name to find cert index*/
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		tloge("unable to create new XPath context\n");
		return -1;
	}

	if (strlen("//package[@name=]/sigs/cert") + strlen(pkgName) >=
		(sizeof(eval) - 1)) {
		tloge("pkgName len is %zu\n", strlen(pkgName));
		xmlXPathFreeContext(xpathCtx);
		return -1;
	}
	ret = snprintf_s(eval, EVAL_LEN - 1, EVAL_LEN - 1,
			"//package[@name='%s']/sigs/cert", pkgName);
	if (ret > EVAL_LEN - 1 || ret < 0) {
		tloge("getCertsForPackage snprintf_s err\n");
		return ret;
	}
	eval[sizeof(eval) - 1] = '\0';
	xpathObj = xmlXPathEvalExpression((xmlChar const *)eval, xpathCtx); /*[false alarm]: ignore*/
	if (xpathObj == NULL) {
		/*cannot find package name*/
		tloge("unable to evaluate xpath expression\n");
		xmlXPathFreeContext(xpathCtx);
		return -1;
	}

	nodes = xpathObj->nodesetval;
	size = (nodes) ? nodes->nodeNr : 0;
	if (size <= 0) {
		tloge("cannot find package:%s\n", pkgName);
		xmlXPathFreeContext(xpathCtx);
		xmlXPathFreeObject(xpathObj);
		return -1;
	}
	/*2. use userId to find cert index*/
	xpathCtxUid = xmlXPathNewContext(doc);
	if (xpathCtxUid == NULL) {
		tloge("unable to create new xpathCtxUid context\n");
		xmlXPathFreeContext(xpathCtx);
		xmlXPathFreeObject(xpathObj);
		return -1;
	}

	if (uid > 100000)
		uid = uid % 100000;
	ret = snprintf_s(eval, EVAL_LEN - 1, EVAL_LEN - 1,
			"//package[@userId='%d' or @sharedUserId='%d']/sigs/cert",
			uid, uid);
	if (ret > EVAL_LEN - 1 || ret < 0) {
		tloge("getCertsForPackage snprintf_s err\n");
		return ret;
	}
	xpathObjUid = xmlXPathEvalExpression(
			(xmlChar const *)eval,
			xpathCtxUid);/*[false alarm]: ignore*/
	if (xpathObjUid == NULL) {
		/*cannot find uid or shareUid*/
		tloge("unable to evaluate xpathObjUid expression\n");
		xmlXPathFreeContext(xpathCtxUid);
		xmlXPathFreeContext(xpathCtx);
		xmlXPathFreeObject(xpathObj);
		return -1;
	}

	nodesUid = xpathObjUid->nodesetval;
	sizeOfCertForUid = (nodesUid) ? nodesUid->nodeNr : 0;
	if (sizeOfCertForUid <= 0) {
		tloge("cannot find sizeOfCertForUid uid:%d\n", sizeOfCertForUid);
		xmlXPathFreeContext(xpathCtx);
		xmlXPathFreeObject(xpathObj);
		xmlXPathFreeContext(xpathCtxUid);
		xmlXPathFreeObject(xpathObjUid);
		return -1;
	}

	tlogd("package %s has %d certs %d certsOfUid\n",
		pkgName, size, sizeOfCertForUid);

	getInstalledAppCerts(doc);

	for (i = 0; i < size; i++) {
		if (NULL == nodes) {
			tloge("cannot find struct xpathObjUid->nodesetval\n");
			return -1;
		}
		cur = nodes->nodeTab[i];
		tmp = xmlGetProp(cur, (const xmlChar *)"index");
		/* cid 336201 */
		if (!tmp) {
			tloge("cannot find cur in xml\n");
			certs[i] = NULL;
			xmlXPathFreeContext(xpathCtx);
			xmlXPathFreeObject(xpathObj);
			xmlXPathFreeContext(xpathCtxUid);
			xmlXPathFreeObject(xpathObjUid);
			return -1;
		}
		tlogd("index:%s \n", tmp);
		index = atoi((char const *)tmp);
		xmlFree((void *)tmp);
		cert_match = false;

		/*3. check if uid and package name get the same cert index */
		for (j = 0; j < sizeOfCertForUid; j++) {
			if (NULL == nodesUid) {
				tloge("cannot find struct xpathObjUid->nodesetval\n");
				return -1;
			}
			curUid = nodesUid->nodeTab[j];
			tmpForUid = xmlGetProp(curUid, (const xmlChar *)"index");
			tlogd("index for uid:%s \n", tmpForUid);
			indexForUid = atoi((char const *)tmpForUid);
			xmlFree((void *)tmpForUid);
			if (indexForUid == index) {
				cert_match = true;
				tlogd("indexForUid %d, index %d are the same\n",
						indexForUid, index);
				break;
			}
		}

		if (!cert_match) {
			tlogd("indexForUid %d, index %d are not the same\n",
				indexForUid, index);
			certs[i] = NULL;
			xmlXPathFreeContext(xpathCtx);
			xmlXPathFreeObject(xpathObj);
			xmlXPathFreeContext(xpathCtxUid);
			xmlXPathFreeObject(xpathObjUid);
			return -1;
		}

		if (index < appCertsSize)
			certs[i] = appCerts[index];
		else
			certs[i] = NULL;
	}

	/* indicate  the end of data; */
	certs[i] = NULL;

	xmlXPathFreeContext(xpathCtx);
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtxUid);
	xmlXPathFreeObject(xpathObjUid);
	return size;

}

static int tee_encode_app_cert(X509 *cert,
			uint32_t *len,
			char *buf,
			unsigned int buf_len)
{
	char *modulus;
	unsigned char *modulus_temp = NULL;
	unsigned int modulus_size = 0;
	char *exponent;
	unsigned char *exponent_temp = NULL;
	unsigned int exponent_size = 0;
	unsigned char *publickeys_temp = NULL;
	unsigned int malloc_size = 0;
	unsigned int i;
	int ret = -1;
	unsigned int public_key_len;

	*len = 0;
	if (X509_get_pubkey(cert) == NULL) {
		tloge("get pubkey is null");
		return -1;
	}
	modulus = (char*)BN_bn2hex(X509_get_pubkey(cert)->pkey.rsa->n);
	exponent = (char*)BN_bn2hex(X509_get_pubkey(cert)->pkey.rsa->e);
	if (modulus == NULL || exponent == NULL) {
		tloge("mod or exp is null");
		goto clean_up;
	}
	tlogd("------exponent %s, modulus %s\n", exponent, modulus);
	tlogd("------exponent size %zu, modulus size %zu\n",
		strlen(exponent), strlen(modulus));
	modulus_size = strlen(modulus) / 2;
	/* cid 339564 */
	if (!modulus_size) {
		tloge("modulus size is (0) not correct\n");
		goto clean_up;
	}
	exponent_size = strlen(exponent) / 2;
	/* cid 345960 */
	if (!exponent_size) {
		tloge("exponent_size is (0) not correct\n");
		goto clean_up;
	}
	/* cid 345960 */
	/* cid 344584 */
	malloc_size = modulus_size + exponent_size;
	if (malloc_size <= modulus_size || malloc_size <= exponent_size) {
		tloge("modulus_size+exponent_size buffer overflow\n");
		goto clean_up;
	}
	malloc_size += 8;
	if (malloc_size <= (modulus_size + exponent_size) || malloc_size <= 8) {
		tloge("malloc_size+8 buffer overflow\n");
		goto clean_up;
	}
	publickeys_temp =
		(unsigned char *)malloc(modulus_size + exponent_size + 8);
	if (!publickeys_temp) {
		tloge("*publickeys malloc failed\n");
		goto clean_up;
	}
	ret = memset_s(publickeys_temp, (modulus_size + exponent_size + 8), 0,
		(modulus_size + exponent_size + 8));
	if (ret != EOK) {
		tloge("memset_s erro!\n");
		goto clean_up;
	}

	modulus_temp = (unsigned char *)malloc(modulus_size);
	if (!modulus_temp) {
		tloge("modulus_temp malloc failed\n");
		ret = -1;
		goto clean_up;
	}
	ret = memset_s(modulus_temp, modulus_size, 0, modulus_size);
	if (ret != EOK) {
		tloge("memset_s erro!\n");
		goto clean_up;
	}
	exponent_temp = (unsigned char *)malloc(exponent_size);
	if (!exponent_temp) {
		tloge("exponent_temp malloc failed\n");
		ret = -1;
		goto clean_up;
	}
	ret = memset_s(exponent_temp, exponent_size, 0, exponent_size);
	if (ret != EOK) {
		tloge("memset_s erro!\n");
		goto clean_up;
	}
	for (i = 0; i < modulus_size; i++) {
		*(modulus_temp + i) = convert_str2dex(*(modulus + i * 2)) << /*lint !e679 */
			4 | convert_str2dex(*(modulus + i * 2 + 1));/*lint !e679 */
	}
	for (i = 0; i < exponent_size; i++) {
		*(exponent_temp + i) = convert_str2dex(*(exponent + i * 2)) << /*lint !e679 */
			4 | convert_str2dex(*(exponent + i * 2 + 1)); /*lint !e679 */
	}

	/* The apk certificate format is as follows:
	 * modulus_size(4bytes)||modulus buffer||exponent size||exponent buffer
	 */

	/* modulus */
	*(unsigned int *)publickeys_temp = modulus_size;/*lint !e826 */
	ret = memcpy_s(publickeys_temp + sizeof(modulus_size), modulus_size,
		modulus_temp, modulus_size);
	if (ret != EOK) {
		tloge("memcpy_s erro!\n");
		goto clean_up;
	}

	/* exponent */
	*(unsigned int *)(publickeys_temp + sizeof(modulus_size) + modulus_size)
		= exponent_size; /*lint !e826 */
	ret = memcpy_s(publickeys_temp +
		sizeof(modulus_size) +
		sizeof(exponent_size) +
		modulus_size,
		exponent_size,
		exponent_temp,
		exponent_size);
	if (ret != EOK) {
		tloge("memcpy_s erro!\n");
		goto clean_up;
	}

	public_key_len = modulus_size + exponent_size +
			sizeof(modulus_size) + sizeof(exponent_size);
	/* cid 344584 */
	if ((sizeof(public_key_len) + public_key_len) < buf_len) {
		ret = memcpy_s(buf, sizeof(public_key_len), &public_key_len,
			sizeof(public_key_len));
		if (ret != EOK) {
			tloge("memcpy_s erro!\n");
			goto clean_up;
		}
		buf += sizeof(public_key_len);
		*len += sizeof(public_key_len);
		tlogd("public_key_len = %d\n", public_key_len);

		ret = memcpy_s(buf, buf_len - /*[false alarm]: processed */
				sizeof(public_key_len),
				publickeys_temp,
				modulus_size +
				exponent_size +
				sizeof(modulus_size) +
				sizeof(exponent_size));  /*[false alarm]: processed */
		if (ret != EOK) {
			tloge("memcpy_s erro!\n");
			goto clean_up;
		}
		*len += modulus_size + exponent_size +
			sizeof(modulus_size) + sizeof(exponent_size);
	} else {
		tloge("public_key_len(%d)+sizeof(public_key_len) larger than 4k\n",
			public_key_len);
		ret = -1;
		goto clean_up;
	}
	tlogd("total_size = %d ", *len);

	ret = 0;
clean_up:
	if (modulus)
		OPENSSL_free(modulus);
	if (exponent)
		OPENSSL_free(exponent);
	if (publickeys_temp)
		free(publickeys_temp);
	if (modulus_temp)
		free(modulus_temp);
	if (exponent_temp)
		free(exponent_temp);
	return ret;
}
#endif

int tee_get_app_cert(unsigned int ca_pid, unsigned int ca_uid, uint32_t *len,
		     uint8_t *buffer, uint32_t *type)
{
	char *pkgName;
#ifndef WITH_DEVCHIP_PLATFORM
	xmlDocPtr doc;
	const xmlChar *certs[16] = {0};
#endif
	char buf[2048] = {0};
	int ret;
#ifndef WITH_DEVCHIP_PLATFORM
	X509 *cert = NULL;
#endif
	const unsigned char *temp = NULL;
	char path[MAX_PATH_LENGTH] = {0};
	FILE *fd;
	int pubkey_len;
	uint32_t num, path_len;
	char *path_temp;
	uint32_t input_len = *len;
	int i = 0;
	uint32_t cert_len = 0;

	*type = CA_CERT_NATIVE;
#ifndef WITH_DEVCHIP_PLATFORM
	tloge("CA TYPE is<%d>\n", *type);
#endif
	/* cid 333805 */
	ret = snprintf_s(path,
			 MAX_PATH_LENGTH - 1, MAX_PATH_LENGTH - 1,
			 "/proc/%d/cmdline",
			 ca_pid);
	if (ret > MAX_PATH_LENGTH - 1 || ret < 0) {
		tloge("tee_get_app_cert snprintf_s err\n");
		return ret;
	}
	fd = fopen(path, "r");  /*[false alarm]: processed */
	if (fd == NULL) {
		tloge("fopen is error: %s\n", strerror(errno));
		return -1;
	}
	ret = memset_s(path, MAX_PATH_LENGTH, 0x00, MAX_PATH_LENGTH);
	if (ret != EOK) {
		tloge("memset_s erro!\n");
		fclose(fd);  /*[false alarm]: processed */
		return ret;
	}
	path_temp = fgets(path, MAX_PATH_LENGTH - 1, fd);
	fclose(fd);
	if (!path_temp) {
		tloge("Bad pacakge name\n");
		return -1;
	}

	pkgName = path;
	*len = 0;
	num = strlen(path);
	tlogd("num is %d", num);
	tlogd("len is %d, packagename is %s\n", *len, pkgName);
	path_len = num;
	if (sizeof(path_len) > input_len) {
		tloge("buffer overflow\n");
		return -10;
	}
	ret = memcpy_s(buffer, input_len, &path_len, sizeof(path_len));
	if (ret != EOK) {
		tloge("memcpy_s erro!\n");
		return ret;
	}
	buffer += sizeof(path_len);
	*len += sizeof(path_len);
	input_len -= sizeof(path_len);

	if (num > input_len) {
		tloge("buffer overflow!\n");
		return -11;
	}
	ret = memcpy_s(buffer, input_len, path, num);
	if (ret != EOK) {
		tloge("memcpy_s erro!\n");
		return ret;
	}
	buffer += num;
	*len += num;
	input_len -= num;

#ifndef WITH_DEVCHIP_PLATFORM
	doc = xmlParseFile("/data/system/packages.xml");
	if (doc == NULL) {
		pubkey_len = 0;
		if (sizeof(pubkey_len) > input_len) {
			tloge("buffer overflow!!\n");
			return -12;
		}
		ret = memcpy_s(buffer, input_len,
			&pubkey_len, sizeof(pubkey_len)); /*[false alarm]: processed */
		if (ret != EOK) {
			tloge("memcpy_s erro!\n");
			return ret;
		}
		buffer += sizeof(pubkey_len);
		*len += sizeof(pubkey_len);
		input_len -= num;
		tloge("Error: unable to parse xml file\n");
		/* We return success because we still have valid uid data */
		return 0; /*lint !e438 */
	}

	if (getCertsForPackage(doc, pkgName, ca_uid, certs) < 0) {
		/*If get public key failed,pass 0,
		then in kernel get uid for ca*/
		pubkey_len = 0;
		if (sizeof(pubkey_len) > input_len) {
			tloge("buffer overflow!!!\n");
			return -13;
		}
		ret = memcpy_s(buffer, input_len,
				&pubkey_len, sizeof(pubkey_len)); /*[false alarm]: processed */
		if (ret != EOK) {
			tloge("memcpy_s erro!\n");
			return ret;
		}
		buffer += sizeof(pubkey_len);
		*len += sizeof(pubkey_len);
		input_len -= num;
		tloge("get certs for app failed\n");
		xmlFreeDoc(doc);
		/* We return success because we still have valid uid data */
		return 0; /*lint !e438 */
	}

	*type = CA_CERT_APK;
	tlogd("get certs :\n");
	while (certs[i]) {
		ret = strToByte(buf, (char *)certs[i]);
		temp = (unsigned char *)buf;
		/* tlogd("%s", temp); */

		cert = d2i_X509(NULL, &temp, (long)ret);
		if (!cert) {
			tloge("unable to parse certificate in memory\n");
			return -1;
		}

		tee_encode_app_cert(cert, &cert_len,
				(char *)buffer, (BUF_MAX_SIZE - (*len)));
		X509_free(cert);
		i++;
	}
	tlogd("get certs end\n");
	*len += cert_len;

	xmlFreeDoc(doc);
#endif
	return 0;
}
