#ifndef TEE_GET_APP_CERT_H_
#define TEE_GET_APP_CERT_H_

int tee_get_app_cert(int ca_pid, int ca_uid, uint32_t* len, uint8_t *buffer, uint32_t* type);

#endif
