/*
 * Copyright (C) 2004-2012 Free Software Foundation, Inc.
 * Copyright (C) 2013 Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * GnuTLS is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GnuTLS is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Written by Nikos Mavrogiannopoulos <nmav@gnutls.org>.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <certtool-cfg.h>
#include <gnutls/x509.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>
#include <time.h>
#include <parse-datetime.h>
#include <autoopts/options.h>
#include <intprops.h>

/* for inet_pton */
#include <sys/types.h>

#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#elif HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif
#include <arpa/inet.h>

/* Gnulib portability files. */
#include <getpass.h>
#include "certtool-common.h"

extern int batch;

#define MAX_ENTRIES 128
#define MAX_POLICIES 8

typedef struct _cfg_ctx {
	char *organization;
	char *unit;
	char *locality;
	char *state;
	char *dn;
	char *cn;
	char *uid;
	char *challenge_password;
	char *pkcs9_email;
	char *country;
	char *policy_oid[MAX_POLICIES];
	char *policy_txt[MAX_POLICIES];
	char *policy_url[MAX_POLICIES];
	char **dc;
	char **dns_name;
	char **uri;
	char **ip_addr;
	char **email;
	char **dn_oid;
	char *crl_dist_points;
	char *password;
	char *pkcs12_key_name;
	char *expiration_date;
	char *activation_date;
	int serial;
	int expiration_days;
	int ca;
	int path_len;
	int tls_www_client;
	int tls_www_server;
	int signing_key;
	int encryption_key;
	int cert_sign_key;
	int crl_sign_key;
	int code_sign_key;
	int ocsp_sign_key;
	int time_stamping_key;
	int ipsec_ike_key;
	char **key_purpose_oids;
	int crl_next_update;
	int crl_number;
	int crq_extensions;
	char *proxy_policy_language;
	char **ocsp_uris;
	char **ca_issuers_uris;
} cfg_ctx;

cfg_ctx cfg;

void cfg_init(void)
{
	memset(&cfg, 0, sizeof(cfg));
	cfg.path_len = -1;
	cfg.serial = -1;
}

#define READ_MULTI_LINE(name, s_name) \
  val = optionGetValue(pov, name); \
  if (val != NULL && val->valType == OPARG_TYPE_STRING) \
  { \
    if (s_name == NULL) { \
      i = 0; \
      s_name = malloc(sizeof(char*)*MAX_ENTRIES); \
      do { \
        if (val && !strcmp(val->pzName, name)==0) \
          continue; \
        s_name[i] = strdup(val->v.strVal); \
        i++; \
          if (i>=MAX_ENTRIES) \
            break; \
      } while((val = optionNextValue(pov, val)) != NULL); \
      s_name[i] = NULL; \
    } \
  }

#define READ_MULTI_LINE_TOKENIZED(name, s_name) \
  val = optionGetValue(pov, name); \
  if (val != NULL && val->valType == OPARG_TYPE_STRING) \
  { \
    char str[512]; \
    char * p; \
    if (s_name == NULL) { \
      i = 0; \
      s_name = malloc(sizeof(char*)*MAX_ENTRIES); \
      do { \
        if (val && !strcmp(val->pzName, name)==0) \
          continue; \
        strncpy(str, val->v.strVal, sizeof(str)-1); \
        str[sizeof(str)-1] = 0; \
        if ((p=strchr(str, ' ')) == NULL && (p=strchr(str, '\t')) == NULL) { \
          fprintf(stderr, "Error parsing %s\n", name); \
          exit(1); \
        } \
        p[0] = 0; \
        p++; \
        s_name[i] = strdup(str); \
        while(*p==' ' || *p == '\t') p++; \
        if (p[0] == 0) { \
          fprintf(stderr, "Error (2) parsing %s\n", name); \
          exit(1); \
        } \
        s_name[i+1] = strdup(p); \
        i+=2; \
        if (i>=MAX_ENTRIES) \
          break; \
      } while((val = optionNextValue(pov, val)) != NULL); \
      s_name[i] = NULL; \
    } \
  }

#define READ_BOOLEAN(name, s_name) \
  val = optionGetValue(pov, name); \
  if (val != NULL) \
    { \
      s_name = 1; \
    }

#define READ_NUMERIC(name, s_name) \
  val = optionGetValue(pov, name); \
  if (val != NULL) \
    { \
      if (val->valType == OPARG_TYPE_NUMERIC) \
        s_name = val->v.longVal; \
      else if (val->valType == OPARG_TYPE_STRING) \
        s_name = atoi(val->v.strVal); \
    }

int template_parse(const char *template)
{
	/* Parsing return code */
	int ret;
	unsigned int i;
	tOptionValue const *pov;
	const tOptionValue *val;
	char tmpstr[256];

	pov = configFileLoad(template);
	if (pov == NULL) {
		perror("configFileLoad");
		fprintf(stderr, "Error loading template: %s\n", template);
		exit(1);
	}

	/* Option variables */
	val = optionGetValue(pov, "organization");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.organization = strdup(val->v.strVal);

	val = optionGetValue(pov, "unit");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.unit = strdup(val->v.strVal);

	val = optionGetValue(pov, "locality");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.locality = strdup(val->v.strVal);

	val = optionGetValue(pov, "state");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.state = strdup(val->v.strVal);

	val = optionGetValue(pov, "dn");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.dn = strdup(val->v.strVal);

	val = optionGetValue(pov, "cn");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.cn = strdup(val->v.strVal);

	val = optionGetValue(pov, "uid");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.uid = strdup(val->v.strVal);

	val = optionGetValue(pov, "challenge_password");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.challenge_password = strdup(val->v.strVal);

	val = optionGetValue(pov, "password");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.password = strdup(val->v.strVal);

	val = optionGetValue(pov, "pkcs9_email");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.pkcs9_email = strdup(val->v.strVal);

	val = optionGetValue(pov, "country");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.country = strdup(val->v.strVal);

	val = optionGetValue(pov, "expiration_date");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.expiration_date = strdup(val->v.strVal);

	val = optionGetValue(pov, "activation_date");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.activation_date = strdup(val->v.strVal);

	for (i = 0; i < MAX_POLICIES; i++) {
		snprintf(tmpstr, sizeof(tmpstr), "policy%d", i + 1);
		val = optionGetValue(pov, tmpstr);
		if (val != NULL && val->valType == OPARG_TYPE_STRING)
			cfg.policy_oid[i] = strdup(val->v.strVal);

		if (cfg.policy_oid[i] != NULL) {
			snprintf(tmpstr, sizeof(tmpstr), "policy%d_url",
				 i + 1);
			val = optionGetValue(pov, tmpstr);
			if (val != NULL
			    && val->valType == OPARG_TYPE_STRING)
				cfg.policy_url[i] = strdup(val->v.strVal);

			snprintf(tmpstr, sizeof(tmpstr), "policy%d_txt",
				 i + 1);
			val = optionGetValue(pov, tmpstr);
			if (val != NULL
			    && val->valType == OPARG_TYPE_STRING) {
				cfg.policy_txt[i] = strdup(val->v.strVal);
			}
		}
	}

	READ_MULTI_LINE("dc", cfg.dc);
	READ_MULTI_LINE("dns_name", cfg.dns_name);
	READ_MULTI_LINE("uri", cfg.uri);

	READ_MULTI_LINE("ip_address", cfg.ip_addr);
	READ_MULTI_LINE("email", cfg.email);
	READ_MULTI_LINE("key_purpose_oid", cfg.key_purpose_oids);

	READ_MULTI_LINE_TOKENIZED("dn_oid", cfg.dn_oid);

	val = optionGetValue(pov, "crl_dist_points");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.crl_dist_points = strdup(val->v.strVal);

	val = optionGetValue(pov, "pkcs12_key_name");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.pkcs12_key_name = strdup(val->v.strVal);


	READ_NUMERIC("serial", cfg.serial);
	READ_NUMERIC("expiration_days", cfg.expiration_days);
	READ_NUMERIC("crl_next_update", cfg.crl_next_update);
	READ_NUMERIC("crl_number", cfg.crl_number);
	READ_NUMERIC("path_len", cfg.path_len);

	val = optionGetValue(pov, "proxy_policy_language");
	if (val != NULL && val->valType == OPARG_TYPE_STRING)
		cfg.proxy_policy_language = strdup(val->v.strVal);

	READ_MULTI_LINE("ocsp_uri", cfg.ocsp_uris);
	READ_MULTI_LINE("ca_issuers_uri", cfg.ca_issuers_uris);

	READ_BOOLEAN("ca", cfg.ca);
	READ_BOOLEAN("honor_crq_extensions", cfg.crq_extensions);
	READ_BOOLEAN("tls_www_client", cfg.tls_www_client);
	READ_BOOLEAN("tls_www_server", cfg.tls_www_server);
	READ_BOOLEAN("signing_key", cfg.signing_key);
	READ_BOOLEAN("encryption_key", cfg.encryption_key);
	READ_BOOLEAN("cert_signing_key", cfg.cert_sign_key);
	READ_BOOLEAN("crl_signing_key", cfg.crl_sign_key);
	READ_BOOLEAN("code_signing_key", cfg.code_sign_key);
	READ_BOOLEAN("ocsp_signing_key", cfg.ocsp_sign_key);
	READ_BOOLEAN("time_stamping_key", cfg.time_stamping_key);
	READ_BOOLEAN("ipsec_ike_key", cfg.ipsec_ike_key);

	optionUnloadNested(pov);

	return 0;
}

#define IS_NEWLINE(x) ((x[0] == '\n') || (x[0] == '\r'))

void
read_crt_set(gnutls_x509_crt_t crt, const char *input_str, const char *oid)
{
	char input[128];
	int ret;

	fputs(input_str, stderr);
	if (fgets(input, sizeof(input), stdin) == NULL)
		return;

	if (IS_NEWLINE(input))
		return;

	ret =
	    gnutls_x509_crt_set_dn_by_oid(crt, oid, 0, input,
					  strlen(input) - 1);
	if (ret < 0) {
		fprintf(stderr, "set_dn: %s\n", gnutls_strerror(ret));
		exit(1);
	}
}

void
read_crq_set(gnutls_x509_crq_t crq, const char *input_str, const char *oid)
{
	char input[128];
	int ret;

	fputs(input_str, stderr);
	if (fgets(input, sizeof(input), stdin) == NULL)
		return;

	if (IS_NEWLINE(input))
		return;

	ret =
	    gnutls_x509_crq_set_dn_by_oid(crq, oid, 0, input,
					  strlen(input) - 1);
	if (ret < 0) {
		fprintf(stderr, "set_dn: %s\n", gnutls_strerror(ret));
		exit(1);
	}
}

/* The input_str should contain %d or %u to print the default.
 */
static int read_int_with_default(const char *input_str, int def)
{
	char *endptr;
	long l, len;
	static char input[128];

	fprintf(stderr, input_str, def);
	if (fgets(input, sizeof(input), stdin) == NULL)
		return def;

	if (IS_NEWLINE(input))
		return def;

	len = strlen(input);

	l = strtol(input, &endptr, 0);

	if (*endptr != '\0' && *endptr != '\r' && *endptr != '\n') {
		fprintf(stderr, "Trailing garbage ignored: `%s'\n",
			endptr);
		return 0;
	}

	if (l <= INT_MIN || l >= INT_MAX) {
		fprintf(stderr, "Integer out of range: `%s'\n", input);
		return 0;
	}

	if (input == endptr)
		l = def;

	return (int) l;
}

int read_int(const char *input_str)
{
	return read_int_with_default(input_str, 0);
}

const char *read_str(const char *input_str)
{
	static char input[128];
	int len;

	fputs(input_str, stderr);
	if (fgets(input, sizeof(input), stdin) == NULL)
		return NULL;

	if (IS_NEWLINE(input))
		return NULL;

	len = strlen(input);
	if ((len > 0) && (input[len - 1] == '\n'))
		input[len - 1] = 0;
	if (input[0] == 0)
		return NULL;

	return input;
}

/* Default is:
 * def: 0 -> no
 * def: 1 -> yes
 */
int read_yesno(const char *input_str, int def)
{
	char input[128];

      restart:
	fputs(input_str, stderr);
	if (fgets(input, sizeof(input), stdin) == NULL)
		return def;

	if (IS_NEWLINE(input))
		return def;

	if (input[0] == 'y' || input[0] == 'Y')
		return 1;
	else if (input[0] == 'n' || input[0] == 'N')
		return 0;
	else
		goto restart;
}


/* Wrapper functions for non-interactive mode.
 */
const char *get_pass(void)
{
	if (batch)
		return cfg.password;
	else
		return getpass("Enter password: ");
}

const char *get_confirmed_pass(bool empty_ok)
{
	if (batch)
		return cfg.password;
	else {
		const char *pass = NULL;
		char *copy = NULL;

		do {
			if (pass)
				fprintf(stderr,
					"Password missmatch, try again.\n");

			free(copy);

			pass = getpass("Enter password: ");
			copy = strdup(pass);
			pass = getpass("Confirm password: ");
		}
		while (strcmp(pass, copy) != 0
		       && !(empty_ok && *pass == '\0'));

		free(copy);

		return pass;
	}
}

const char *get_challenge_pass(void)
{
	if (batch)
		return cfg.challenge_password;
	else
		return getpass("Enter a challenge password: ");
}

const char *get_crl_dist_point_url(void)
{
	if (batch)
		return cfg.crl_dist_points;
	else
		return
		    read_str
		    ("Enter the URI of the CRL distribution point: ");
}

void get_country_crt_set(gnutls_x509_crt_t crt)
{
	int ret;

	if (batch) {
		if (!cfg.country)
			return;
		ret =
		    gnutls_x509_crt_set_dn_by_oid(crt,
						  GNUTLS_OID_X520_COUNTRY_NAME,
						  0, cfg.country,
						  strlen(cfg.country));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crt_set(crt, "Country name (2 chars): ",
			     GNUTLS_OID_X520_COUNTRY_NAME);
	}

}

void get_organization_crt_set(gnutls_x509_crt_t crt)
{
	int ret;

	if (batch) {
		if (!cfg.organization)
			return;

		ret =
		    gnutls_x509_crt_set_dn_by_oid(crt,
						  GNUTLS_OID_X520_ORGANIZATION_NAME,
						  0, cfg.organization,
						  strlen(cfg.
							 organization));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crt_set(crt, "Organization name: ",
			     GNUTLS_OID_X520_ORGANIZATION_NAME);
	}

}

void get_unit_crt_set(gnutls_x509_crt_t crt)
{
	int ret;

	if (batch) {
		if (!cfg.unit)
			return;

		ret =
		    gnutls_x509_crt_set_dn_by_oid(crt,
						  GNUTLS_OID_X520_ORGANIZATIONAL_UNIT_NAME,
						  0, cfg.unit,
						  strlen(cfg.unit));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crt_set(crt, "Organizational unit name: ",
			     GNUTLS_OID_X520_ORGANIZATIONAL_UNIT_NAME);
	}

}

void get_state_crt_set(gnutls_x509_crt_t crt)
{
	int ret;

	if (batch) {
		if (!cfg.state)
			return;
		ret =
		    gnutls_x509_crt_set_dn_by_oid(crt,
						  GNUTLS_OID_X520_STATE_OR_PROVINCE_NAME,
						  0, cfg.state,
						  strlen(cfg.state));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crt_set(crt, "State or province name: ",
			     GNUTLS_OID_X520_STATE_OR_PROVINCE_NAME);
	}

}

void get_locality_crt_set(gnutls_x509_crt_t crt)
{
	int ret;

	if (batch) {
		if (!cfg.locality)
			return;
		ret =
		    gnutls_x509_crt_set_dn_by_oid(crt,
						  GNUTLS_OID_X520_LOCALITY_NAME,
						  0, cfg.locality,
						  strlen(cfg.locality));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crt_set(crt, "Locality name: ",
			     GNUTLS_OID_X520_LOCALITY_NAME);
	}

}

void get_cn_crt_set(gnutls_x509_crt_t crt)
{
	int ret;

	if (batch) {
		if (!cfg.cn)
			return;
		ret =
		    gnutls_x509_crt_set_dn_by_oid(crt,
						  GNUTLS_OID_X520_COMMON_NAME,
						  0, cfg.cn,
						  strlen(cfg.cn));
		if (ret < 0) {
			fprintf(stderr, "set_dn_by_oid: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crt_set(crt, "Common name: ",
			     GNUTLS_OID_X520_COMMON_NAME);
	}

}

void get_dn_crt_set(gnutls_x509_crt_t crt)
{
	int ret;
	const char *err;

	if (batch) {
		if (!cfg.dn)
			return;
		ret = gnutls_x509_crt_set_dn(crt, cfg.dn, &err);
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s at: %s\n",
				gnutls_strerror(ret), err);
			exit(1);
		}
	}
}

void get_uid_crt_set(gnutls_x509_crt_t crt)
{
	int ret;

	if (batch) {
		if (!cfg.uid)
			return;
		ret =
		    gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_LDAP_UID,
						  0, cfg.uid,
						  strlen(cfg.uid));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crt_set(crt, "UID: ", GNUTLS_OID_LDAP_UID);
	}

}

void get_oid_crt_set(gnutls_x509_crt_t crt)
{
	int ret, i;

	if (batch) {
		if (!cfg.dn_oid)
			return;
		for (i = 0; cfg.dn_oid[i] != NULL; i += 2) {
			if (cfg.dn_oid[i + 1] == NULL) {
				fprintf(stderr,
					"dn_oid: %s does not have an argument.\n",
					cfg.dn_oid[i]);
				exit(1);
			}
			ret =
			    gnutls_x509_crt_set_dn_by_oid(crt,
							  cfg.dn_oid[i], 0,
							  cfg.dn_oid[i +
								     1],
							  strlen(cfg.
								 dn_oid[i +
									1]));

			if (ret < 0) {
				fprintf(stderr, "set_dn_oid: %s\n",
					gnutls_strerror(ret));
				exit(1);
			}
		}
	}
}

void get_key_purpose_set(int type, void *crt)
{
	int ret, i;

	if (batch) {
		if (!cfg.key_purpose_oids)
			return;
		for (i = 0; cfg.key_purpose_oids[i] != NULL; i++) {
			if (type == TYPE_CRT)
				ret =
				    gnutls_x509_crt_set_key_purpose_oid
				    (crt, cfg.key_purpose_oids[i], 0);
			else
				ret =
				    gnutls_x509_crq_set_key_purpose_oid
				    (crt, cfg.key_purpose_oids[i], 0);

			if (ret < 0) {
				fprintf(stderr,
					"set_key_purpose_oid (%s): %s\n",
					cfg.key_purpose_oids[i],
					gnutls_strerror(ret));
				exit(1);
			}
		}
	}
}

void get_ocsp_issuer_set(gnutls_x509_crt_t crt)
{
	int ret, i;
	gnutls_datum_t uri;

	if (batch) {
		if (!cfg.ocsp_uris)
			return;
		for (i = 0; cfg.ocsp_uris[i] != NULL; i++) {
			uri.data = cfg.ocsp_uris[i];
			uri.size = strlen(cfg.ocsp_uris[i]);
			ret =
			    gnutls_x509_crt_set_authority_info_access(crt,
								      GNUTLS_IA_OCSP_URI,
								      &uri);
			if (ret < 0) {
				fprintf(stderr, "set OCSP URI (%s): %s\n",
					cfg.ocsp_uris[i],
					gnutls_strerror(ret));
				exit(1);
			}
		}
	}
}

void get_ca_issuers_set(gnutls_x509_crt_t crt)
{
	int ret, i;
	gnutls_datum_t uri;

	if (batch) {
		if (!cfg.ca_issuers_uris)
			return;
		for (i = 0; cfg.ca_issuers_uris[i] != NULL; i++) {
			uri.data = cfg.ca_issuers_uris[i];
			uri.size = strlen(cfg.ca_issuers_uris[i]);
			ret =
			    gnutls_x509_crt_set_authority_info_access(crt,
								      GNUTLS_IA_CAISSUERS_URI,
								      &uri);
			if (ret < 0) {
				fprintf(stderr,
					"set CA ISSUERS URI (%s): %s\n",
					cfg.ca_issuers_uris[i],
					gnutls_strerror(ret));
				exit(1);
			}
		}
	}
}


void get_pkcs9_email_crt_set(gnutls_x509_crt_t crt)
{
	int ret;

	if (batch) {
		if (!cfg.pkcs9_email)
			return;
		ret =
		    gnutls_x509_crt_set_dn_by_oid(crt,
						  GNUTLS_OID_PKCS9_EMAIL,
						  0, cfg.pkcs9_email,
						  strlen(cfg.pkcs9_email));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crt_set(crt, "E-mail: ", GNUTLS_OID_PKCS9_EMAIL);
	}

}

int get_serial(void)
{
	int default_serial = time(NULL);

	if (batch) {
		if (cfg.serial < 0)
			return default_serial;
		return cfg.serial;
	} else {
		return read_int_with_default
		    ("Enter the certificate's serial number in decimal (default: %u): ",
		     default_serial);
	}
}

static
time_t get_date(const char* date)
{
	time_t t;
	struct timespec r;
	
	if (date==NULL || parse_datetime(&r, date, NULL) == 0) {
	        fprintf(stderr, "Cannot parse date: %s\n", date);
	        exit(1);
        }
        
        return r.tv_sec;
}

time_t get_activation_date()
{

	if (batch && cfg.activation_date != NULL) {
       		return get_date(cfg.activation_date);
	}
	
	return time(NULL);
}

static
time_t days_to_secs(int days)
{
time_t secs = days;
time_t now = time(NULL);

       	if (secs != (time_t)-1) {
      	        if (INT_MULTIPLY_OVERFLOW(secs, 24*60*60)) {
       	                secs = -1;
	        } else {
    	                secs *= 24*60*60;
      	        }
        }
                                
        if (secs != (time_t)-1) {
                if (INT_ADD_OVERFLOW(secs, now)) {
                        secs = -1;
                } else {
                        secs += now;
                }
        }
        
        return secs;
}

time_t get_expiration_date()
{
	if (batch) {
		if (cfg.expiration_date == NULL) {
		        time_t secs, now;
		        
		        now = time(NULL);

        		if (cfg.expiration_days == 0 || cfg.expiration_days < -2)
        		        secs = days_to_secs(365);
                        else {
                                secs = days_to_secs(cfg.expiration_days);
                        }

			return secs;
		} else
			return get_date(cfg.expiration_date);
	} else {
		int days;

		do {
			days =
			    read_int
			    ("The certificate will expire in (days): ");
		}
		while (days == 0);
		return days_to_secs(days);
	}
}

int get_ca_status(void)
{
	if (batch) {
		return cfg.ca;
	} else {
		return
		    read_yesno
		    ("Does the certificate belong to an authority? (y/N): ",
		     0);
	}
}

int get_crq_extensions_status(void)
{
	if (batch) {
		return cfg.crq_extensions;
	} else {
		return
		    read_yesno
		    ("Do you want to honour the extensions from the request? (y/N): ",
		     0);
	}
}

int get_crl_number(void)
{
	if (batch) {
		return cfg.crl_number;
	} else {
		return read_int_with_default("CRL Number: ", 1);
	}
}

int get_path_len(void)
{
	if (batch) {
		return cfg.path_len;
	} else {
		return read_int_with_default
		    ("Path length constraint (decimal, %d for no constraint): ",
		     -1);
	}
}

const char *get_pkcs12_key_name(void)
{
	const char *name;

	if (batch) {
		if (!cfg.pkcs12_key_name)
			return "Anonymous";
		return cfg.pkcs12_key_name;
	} else {
		do {
			name = read_str("Enter a name for the key: ");
		}
		while (name == NULL);
	}
	return name;
}

int get_tls_client_status(void)
{
	if (batch) {
		return cfg.tls_www_client;
	} else {
		return
		    read_yesno
		    ("Is this a TLS web client certificate? (y/N): ", 0);
	}
}

int get_tls_server_status(void)
{
	if (batch) {
		return cfg.tls_www_server;
	} else {
		return
		    read_yesno
		    ("Is this a TLS web server certificate? (y/N): ", 0);
	}
}

/* convert a printable IP to binary */
static int string_to_ip(unsigned char *ip, const char *str)
{
	int len = strlen(str);
	int ret;

#if HAVE_IPV6
	if (strchr(str, ':') != NULL || len > 16) {	/* IPv6 */
		ret = inet_pton(AF_INET6, str, ip);
		if (ret <= 0) {
			fprintf(stderr, "Error in IPv6 address %s\n", str);
			exit(1);
		}

		/* To be done */
		return 16;
	} else
#endif
	{			/* IPv4 */
		ret = inet_pton(AF_INET, str, ip);
		if (ret <= 0) {
			fprintf(stderr, "Error in IPv4 address %s\n", str);
			exit(1);
		}

		return 4;
	}

}

void get_ip_addr_set(int type, void *crt)
{
	int ret = 0, i;
	unsigned char ip[16];
	int len;

	if (batch) {
		if (!cfg.ip_addr)
			return;

		for (i = 0; cfg.ip_addr[i] != NULL; i++) {
			len = string_to_ip(ip, cfg.ip_addr[i]);
			if (len <= 0) {
				fprintf(stderr,
					"Error parsing address: %s\n",
					cfg.ip_addr[i]);
				exit(1);
			}

			if (type == TYPE_CRT)
				ret =
				    gnutls_x509_crt_set_subject_alt_name
				    (crt, GNUTLS_SAN_IPADDRESS, ip, len,
				     GNUTLS_FSAN_APPEND);
			else
				ret =
				    gnutls_x509_crq_set_subject_alt_name
				    (crt, GNUTLS_SAN_IPADDRESS, ip, len,
				     GNUTLS_FSAN_APPEND);

			if (ret < 0)
				break;
		}
	} else {
		const char *p;

		p = read_str
		    ("Enter the IP address of the subject of the certificate: ");
		if (!p)
			return;

		len = string_to_ip(ip, p);
		if (len <= 0) {
			fprintf(stderr, "Error parsing address: %s\n", p);
			exit(1);
		}

		if (type == TYPE_CRT)
			ret =
			    gnutls_x509_crt_set_subject_alt_name(crt,
								 GNUTLS_SAN_IPADDRESS,
								 ip, len,
								 GNUTLS_FSAN_APPEND);
		else
			ret =
			    gnutls_x509_crq_set_subject_alt_name(crt,
								 GNUTLS_SAN_IPADDRESS,
								 ip, len,
								 GNUTLS_FSAN_APPEND);
	}

	if (ret < 0) {
		fprintf(stderr, "set_subject_alt_name: %s\n",
			gnutls_strerror(ret));
		exit(1);
	}
}

void get_email_set(int type, void *crt)
{
	int ret = 0, i;

	if (batch) {
		if (!cfg.email)
			return;

		for (i = 0; cfg.email[i] != NULL; i++) {
			if (type == TYPE_CRT)
				ret =
				    gnutls_x509_crt_set_subject_alt_name
				    (crt, GNUTLS_SAN_RFC822NAME,
				     cfg.email[i], strlen(cfg.email[i]),
				     GNUTLS_FSAN_APPEND);
			else
				ret =
				    gnutls_x509_crq_set_subject_alt_name
				    (crt, GNUTLS_SAN_RFC822NAME,
				     cfg.email[i], strlen(cfg.email[i]),
				     GNUTLS_FSAN_APPEND);

			if (ret < 0)
				break;
		}
	} else {
		const char *p;

		p = read_str
		    ("Enter the e-mail of the subject of the certificate: ");
		if (!p)
			return;

		if (type == TYPE_CRT)
			ret =
			    gnutls_x509_crt_set_subject_alt_name(crt,
								 GNUTLS_SAN_RFC822NAME,
								 p,
								 strlen(p),
								 GNUTLS_FSAN_APPEND);
		else
			ret =
			    gnutls_x509_crq_set_subject_alt_name(crt,
								 GNUTLS_SAN_RFC822NAME,
								 p,
								 strlen(p),
								 GNUTLS_FSAN_APPEND);
	}

	if (ret < 0) {
		fprintf(stderr, "set_subject_alt_name: %s\n",
			gnutls_strerror(ret));
		exit(1);
	}
}


void get_dc_set(int type, void *crt)
{
	int ret = 0, i;

	if (batch) {
		if (!cfg.dc)
			return;

		for (i = 0; cfg.dc[i] != NULL; i++) {
			if (type == TYPE_CRT)
				ret =
				    gnutls_x509_crt_set_dn_by_oid(crt,
								  GNUTLS_OID_LDAP_DC,
								  0,
								  cfg.
								  dc[i],
								  strlen
								  (cfg.
								   dc[i]));
			else
				ret =
				    gnutls_x509_crq_set_dn_by_oid(crt,
								  GNUTLS_OID_LDAP_DC,
								  0,
								  cfg.
								  dc[i],
								  strlen
								  (cfg.
								   dc[i]));

			if (ret < 0)
				break;
		}
	} else {
		const char *p;

		do {
			p = read_str
			    ("Enter the subject's domain component (DC): ");
			if (!p)
				return;

			if (type == TYPE_CRT)
				ret =
				    gnutls_x509_crt_set_dn_by_oid(crt,
								  GNUTLS_OID_LDAP_DC,
								  0, p,
								  strlen
								  (p));
			else
				ret =
				    gnutls_x509_crq_set_dn_by_oid(crt,
								  GNUTLS_OID_LDAP_DC,
								  0, p,
								  strlen
								  (p));
		}
		while (p != NULL);
	}

	if (ret < 0) {
		fprintf(stderr, "set_dn_by_oid: %s\n",
			gnutls_strerror(ret));
		exit(1);
	}
}

void get_dns_name_set(int type, void *crt)
{
	int ret = 0, i;

	if (batch) {
		if (!cfg.dns_name)
			return;

		for (i = 0; cfg.dns_name[i] != NULL; i++) {
			if (type == TYPE_CRT)
				ret =
				    gnutls_x509_crt_set_subject_alt_name
				    (crt, GNUTLS_SAN_DNSNAME,
				     cfg.dns_name[i],
				     strlen(cfg.dns_name[i]),
				     GNUTLS_FSAN_APPEND);
			else
				ret =
				    gnutls_x509_crq_set_subject_alt_name
				    (crt, GNUTLS_SAN_DNSNAME,
				     cfg.dns_name[i],
				     strlen(cfg.dns_name[i]),
				     GNUTLS_FSAN_APPEND);

			if (ret < 0)
				break;
		}
	} else {
		const char *p;

		do {
			p = read_str
			    ("Enter a dnsName of the subject of the certificate: ");
			if (!p)
				return;

			if (type == TYPE_CRT)
				ret = gnutls_x509_crt_set_subject_alt_name
				    (crt, GNUTLS_SAN_DNSNAME, p, strlen(p),
				     GNUTLS_FSAN_APPEND);
			else
				ret = gnutls_x509_crq_set_subject_alt_name
				    (crt, GNUTLS_SAN_DNSNAME, p, strlen(p),
				     GNUTLS_FSAN_APPEND);
		}
		while (p);
	}

	if (ret < 0) {
		fprintf(stderr, "set_subject_alt_name: %s\n",
			gnutls_strerror(ret));
		exit(1);
	}
}

void get_policy_set(gnutls_x509_crt_t crt)
{
	int ret = 0, i;
	gnutls_x509_policy_st policy;

	if (batch) {
		// if (!cfg.policy_oid)
		// 	return;

		for (i = 0; cfg.policy_oid[i] != NULL; i++) {
			memset(&policy, 0, sizeof(policy));
			policy.oid = cfg.policy_oid[i];

			if (cfg.policy_txt[i] != NULL) {
				policy.qualifier[policy.qualifiers].type =
				    GNUTLS_X509_QUALIFIER_NOTICE;
				policy.qualifier[policy.qualifiers].data =
				    cfg.policy_txt[i];
				policy.qualifier[policy.qualifiers].size =
				    strlen(cfg.policy_txt[i]);
				policy.qualifiers++;
			}

			if (cfg.policy_url[i] != NULL) {
				policy.qualifier[policy.qualifiers].type =
				    GNUTLS_X509_QUALIFIER_URI;
				policy.qualifier[policy.qualifiers].data =
				    cfg.policy_url[i];
				policy.qualifier[policy.qualifiers].size =
				    strlen(cfg.policy_url[i]);
				policy.qualifiers++;
			}

			ret = gnutls_x509_crt_set_policy(crt, &policy, 0);
			if (ret < 0)
				break;
		}
	}

	if (ret < 0) {
		fprintf(stderr, "set_policy: %s\n", gnutls_strerror(ret));
		exit(1);
	}
}

void get_uri_set(int type, void *crt)
{
	int ret = 0, i;

	if (batch) {
		if (!cfg.uri)
			return;

		for (i = 0; cfg.uri[i] != NULL; i++) {
			if (type == TYPE_CRT)
				ret =
				    gnutls_x509_crt_set_subject_alt_name
				    (crt, GNUTLS_SAN_URI, cfg.uri[i],
				     strlen(cfg.uri[i]),
				     GNUTLS_FSAN_APPEND);
			else
				ret =
				    gnutls_x509_crq_set_subject_alt_name
				    (crt, GNUTLS_SAN_URI, cfg.uri[i],
				     strlen(cfg.uri[i]),
				     GNUTLS_FSAN_APPEND);

			if (ret < 0)
				break;
		}
	} else {
		const char *p;

		do {
			p = read_str
			    ("Enter a URI of the subject of the certificate: ");
			if (!p)
				return;

			if (type == TYPE_CRT)
				ret = gnutls_x509_crt_set_subject_alt_name
				    (crt, GNUTLS_SAN_URI, p, strlen(p),
				     GNUTLS_FSAN_APPEND);
			else
				ret = gnutls_x509_crq_set_subject_alt_name
				    (crt, GNUTLS_SAN_URI, p, strlen(p),
				     GNUTLS_FSAN_APPEND);
		}
		while (p);
	}

	if (ret < 0) {
		fprintf(stderr, "set_subject_alt_name: %s\n",
			gnutls_strerror(ret));
		exit(1);
	}
}



int get_sign_status(int server)
{
	const char *msg;

	if (batch) {
		return cfg.signing_key;
	} else {
		if (server)
			msg =
			    "Will the certificate be used for signing (DHE and RSA-EXPORT ciphersuites)? (Y/n): ";
		else
			msg =
			    "Will the certificate be used for signing (required for TLS)? (Y/n): ";
		return read_yesno(msg, 1);
	}
}

int get_encrypt_status(int server)
{
	const char *msg;

	if (batch) {
		return cfg.encryption_key;
	} else {
		if (server)
			msg =
			    "Will the certificate be used for encryption (RSA ciphersuites)? (Y/n): ";
		else
			msg =
			    "Will the certificate be used for encryption (not required for TLS)? (Y/n): ";
		return read_yesno(msg, 1);
	}
}

int get_cert_sign_status(void)
{
	if (batch) {
		return cfg.cert_sign_key;
	} else {
		return
		    read_yesno
		    ("Will the certificate be used to sign other certificates? (y/N): ",
		     0);
	}
}

int get_crl_sign_status(void)
{
	if (batch) {
		return cfg.crl_sign_key;
	} else {
		return
		    read_yesno
		    ("Will the certificate be used to sign CRLs? (y/N): ",
		     0);
	}
}

int get_code_sign_status(void)
{
	if (batch) {
		return cfg.code_sign_key;
	} else {
		return
		    read_yesno
		    ("Will the certificate be used to sign code? (y/N): ",
		     0);
	}
}

int get_ocsp_sign_status(void)
{
	if (batch) {
		return cfg.ocsp_sign_key;
	} else {
		return
		    read_yesno
		    ("Will the certificate be used to sign OCSP requests? (y/N): ",
		     0);
	}
}

int get_time_stamp_status(void)
{
	if (batch) {
		return cfg.time_stamping_key;
	} else {
		return
		    read_yesno
		    ("Will the certificate be used for time stamping? (y/N): ",
		     0);
	}
}

int get_ipsec_ike_status(void)
{
	if (batch) {
		return cfg.ipsec_ike_key;
	} else {
		return
		    read_yesno
		    ("Will the certificate be used for IPsec IKE operations? (y/N): ",
		     0);
	}
}

int get_crl_next_update(void)
{
	int days;

	if (batch) {
		if (cfg.crl_next_update <= 0)
			return 365;
		else
			return cfg.crl_next_update;
	} else {
		do {
			days =
			    read_int
			    ("The next CRL will be issued in (days): ");
		}
		while (days == 0);
		return days;
	}
}

const char *get_proxy_policy(char **policy, size_t * policylen)
{
	const char *ret;

	if (batch) {
		ret = cfg.proxy_policy_language;
		if (!ret)
			ret = "1.3.6.1.5.5.7.21.1";
	} else {
		do {
			ret =
			    read_str
			    ("Enter the OID of the proxy policy language: ");
		}
		while (ret == NULL);
	}

	*policy = NULL;
	*policylen = 0;

	if (strcmp(ret, "1.3.6.1.5.5.7.21.1") != 0 &&
	    strcmp(ret, "1.3.6.1.5.5.7.21.2") != 0) {
		fprintf(stderr,
			"Reading non-standard proxy policy not supported.\n");
	}

	return ret;
}

/* CRQ stuff.
 */
void get_country_crq_set(gnutls_x509_crq_t crq)
{
	int ret;

	if (batch) {
		if (!cfg.country)
			return;
		ret =
		    gnutls_x509_crq_set_dn_by_oid(crq,
						  GNUTLS_OID_X520_COUNTRY_NAME,
						  0, cfg.country,
						  strlen(cfg.country));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crq_set(crq, "Country name (2 chars): ",
			     GNUTLS_OID_X520_COUNTRY_NAME);
	}

}

void get_organization_crq_set(gnutls_x509_crq_t crq)
{
	int ret;

	if (batch) {
		if (!cfg.organization)
			return;

		ret =
		    gnutls_x509_crq_set_dn_by_oid(crq,
						  GNUTLS_OID_X520_ORGANIZATION_NAME,
						  0, cfg.organization,
						  strlen(cfg.
							 organization));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crq_set(crq, "Organization name: ",
			     GNUTLS_OID_X520_ORGANIZATION_NAME);
	}

}

void get_unit_crq_set(gnutls_x509_crq_t crq)
{
	int ret;

	if (batch) {
		if (!cfg.unit)
			return;

		ret =
		    gnutls_x509_crq_set_dn_by_oid(crq,
						  GNUTLS_OID_X520_ORGANIZATIONAL_UNIT_NAME,
						  0, cfg.unit,
						  strlen(cfg.unit));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crq_set(crq, "Organizational unit name: ",
			     GNUTLS_OID_X520_ORGANIZATIONAL_UNIT_NAME);
	}

}

void get_state_crq_set(gnutls_x509_crq_t crq)
{
	int ret;

	if (batch) {
		if (!cfg.state)
			return;
		ret =
		    gnutls_x509_crq_set_dn_by_oid(crq,
						  GNUTLS_OID_X520_STATE_OR_PROVINCE_NAME,
						  0, cfg.state,
						  strlen(cfg.state));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crq_set(crq, "State or province name: ",
			     GNUTLS_OID_X520_STATE_OR_PROVINCE_NAME);
	}

}

void get_locality_crq_set(gnutls_x509_crq_t crq)
{
	int ret;

	if (batch) {
		if (!cfg.locality)
			return;
		ret =
		    gnutls_x509_crq_set_dn_by_oid(crq,
						  GNUTLS_OID_X520_LOCALITY_NAME,
						  0, cfg.locality,
						  strlen(cfg.locality));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crq_set(crq, "Locality name: ",
			     GNUTLS_OID_X520_LOCALITY_NAME);
	}

}

void get_dn_crq_set(gnutls_x509_crq_t crq)
{
	int ret;
	const char *err;

	if (batch) {
		if (!cfg.dn)
			return;
		ret = gnutls_x509_crq_set_dn(crq, cfg.dn, &err);
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s at: %s\n",
				gnutls_strerror(ret), err);
			exit(1);
		}
	}
}

void get_cn_crq_set(gnutls_x509_crq_t crq)
{
	int ret;

	if (batch) {
		if (!cfg.cn)
			return;
		ret =
		    gnutls_x509_crq_set_dn_by_oid(crq,
						  GNUTLS_OID_X520_COMMON_NAME,
						  0, cfg.cn,
						  strlen(cfg.cn));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crq_set(crq, "Common name: ",
			     GNUTLS_OID_X520_COMMON_NAME);
	}

}

void get_uid_crq_set(gnutls_x509_crq_t crq)
{
	int ret;

	if (batch) {
		if (!cfg.uid)
			return;
		ret =
		    gnutls_x509_crq_set_dn_by_oid(crq, GNUTLS_OID_LDAP_UID,
						  0, cfg.uid,
						  strlen(cfg.uid));
		if (ret < 0) {
			fprintf(stderr, "set_dn: %s\n",
				gnutls_strerror(ret));
			exit(1);
		}
	} else {
		read_crq_set(crq, "UID: ", GNUTLS_OID_LDAP_UID);
	}

}

void get_oid_crq_set(gnutls_x509_crq_t crq)
{
	int ret, i;

	if (batch) {
		if (!cfg.dn_oid)
			return;
		for (i = 0; cfg.dn_oid[i] != NULL; i += 2) {
			if (cfg.dn_oid[i + 1] == NULL) {
				fprintf(stderr,
					"dn_oid: %s does not have an argument.\n",
					cfg.dn_oid[i]);
				exit(1);
			}
			ret =
			    gnutls_x509_crq_set_dn_by_oid(crq,
							  cfg.dn_oid[i], 0,
							  cfg.dn_oid[i +
								     1],
							  strlen(cfg.
								 dn_oid[i +
									1]));

			if (ret < 0) {
				fprintf(stderr, "set_dn_oid: %s\n",
					gnutls_strerror(ret));
				exit(1);
			}
		}
	}

}
