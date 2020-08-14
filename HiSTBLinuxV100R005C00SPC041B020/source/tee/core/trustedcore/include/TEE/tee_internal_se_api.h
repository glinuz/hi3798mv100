#ifndef __TEE_INTERNAL_SE_API_H
#define __TEE_INTERNAL_SE_API_H

#include "tee_internal_api.h"

typedef struct _list_object {
	struct _list_object *pstPrev;
	struct _list_object *pstNext;
} list_object;

struct __TEE_SEServiceHandle;
struct __TEE_SEReaderHandle;
struct __TEE_SESessionHandle;
struct __TEE_SEChannelHandle;

typedef struct __TEE_SEServiceHandle *TEE_SEServiceHandle;
typedef struct __TEE_SEReaderHandle *TEE_SEReaderHandle;
typedef struct __TEE_SESessionHandle *TEE_SESessionHandle;
typedef struct __TEE_SEChannelHandle *TEE_SEChannelHandle;

#define ATR_LEN_MAX 32
#define AID_LEN_MIN 5
#define AID_LEN_MAX 16

#define SE_READER_COUNT 1

typedef struct __TEE_SEReaderProperties {
	bool sePresent;            // true if an SE is present in the reader
	bool teeOnly;              // true if this reader is only accessible via the TEE
	bool selectResponseEnable; // true if the response to a SELECT is available in the TEE
} TEE_SEReaderProperties;

typedef struct __TEE_SEAID {
	uint8_t *buffer;    // the value of the applet’s AID
	size_t bufferLen;   // length of the applet’s AID
} TEE_SEAID;

struct __TEE_SEServiceHandle {
	TEE_MUTEX_HANDLE se_mutex;
};

struct __TEE_SEReaderHandle {
	unsigned int id;
	TEE_SEReaderProperties property;
	bool basic_channel_locked;
	unsigned short session_count;
	unsigned short atr_len;
	char atr[ATR_LEN_MAX];
	const char *name;
	list_object session_head;
};

struct __TEE_SESessionHandle {
	char state;  // 0 - closed, 1 - open, -1 - invalid
	unsigned char channel_count;
	short reserved;
	TEE_SEReaderHandle reader;
	list_object list;
	list_object channel_head;
};

struct __TEE_SEChannelHandle {
	TEE_SEAID se_aid;
	bool basic_channel;
	char reserved;
	unsigned short resp_len;
	char *resp_buffer;
	TEE_SESessionHandle session;
	list_object list;
};

void init_tee_se_api(void);
TEE_Result TEE_SEServiceOpen(TEE_SEServiceHandle *seServiceHandle);
void TEE_SEServiceClose(TEE_SEServiceHandle seServiceHandle);
TEE_Result TEE_SEServiceGetReaders(TEE_SEServiceHandle seServiceHandle,
				   TEE_SEReaderHandle *seReaderHandleList,
				   size_t *seReaderHandleListLen);
void TEE_SEReaderGetProperties(TEE_SEReaderHandle seReaderHandle,
			       TEE_SEReaderProperties *readerProperties);
TEE_Result TEE_SEReaderGetName(TEE_SEReaderHandle seReaderHandle,
			       char *readerName, size_t *readerNameLen);
TEE_Result TEE_SEReaderOpenSession(TEE_SEReaderHandle seReaderHandle,
				   TEE_SESessionHandle *seSessionHandle);
void TEE_SEReaderCloseSessions(TEE_SEReaderHandle seReaderHandle);
TEE_Result TEE_SESessionGetATR(TEE_SESessionHandle seSessionHandle,
			       void *atr, size_t *atrLen);
TEE_Result TEE_SESessionIsClosed(TEE_SESessionHandle seSessionHandle);
void TEE_SESessionClose(TEE_SESessionHandle seSessionHandle);
void TEE_SESessionCloseChannels(TEE_SESessionHandle seSessionHandle);
TEE_Result TEE_SESessionOpenBasicChannel(TEE_SESessionHandle seSessionHandle,
		TEE_SEAID *seAID, TEE_SEChannelHandle *seChannelHandle);
TEE_Result TEE_SESessionOpenLogicalChannel(TEE_SESessionHandle seSessionHandle,
		TEE_SEAID *seAID, TEE_SEChannelHandle *seChannelHandle);
void TEE_SEChannelClose(TEE_SEChannelHandle seChannelHandle);
TEE_Result TEE_SEChannelSelectNext(TEE_SEChannelHandle seChannelHandle);
TEE_Result TEE_SEChannelGetSelectResponse(TEE_SEChannelHandle seChannelHandle,
		void *response, size_t *responseLen);
TEE_Result TEE_SEChannelTransmit(TEE_SEChannelHandle seChannelHandle,
				 void *command, size_t commandLen,
				 void *response, size_t *responseLen);

#endif
