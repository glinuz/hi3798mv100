#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_KIRIN_PLATFORM)
#if (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3650)
#include "ecies_wrapped_key_austin.h"
#elif (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI6250)
#include "ecies_wrapped_key_dallas.h"
#elif (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3660 || TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3670)
#include "ecies_wrapped_key_chicago.h"
#else
char encrypted_key[] = {0};

const char wrapped_rsa_priv_p[] = {0};

const char wrapped_rsa_priv_q[] = {0};

const char wrapped_rsa_priv_dq[] = {0};

const char wrapped_rsa_priv_dp[] = {0};

const char wrapped_rsa_priv_qinv[] = {0};
#endif
#else
char encrypted_key[] = {0};

const char wrapped_rsa_priv_p[] = {0};

const char wrapped_rsa_priv_q[] = {0};

const char wrapped_rsa_priv_dq[] = {0};

const char wrapped_rsa_priv_dp[] = {0};

const char wrapped_rsa_priv_qinv[] = {0};
#endif
void get_ptr_ecies(const char **p1, const char **p2,
		const char **p3, const char **p4,
		const char **p5, const char **p6)
{
    *p1 = encrypted_key;
    *p2 = wrapped_rsa_priv_p;
    *p3 = wrapped_rsa_priv_q;
    *p4 = wrapped_rsa_priv_dq;
    *p5 = wrapped_rsa_priv_dp;
    *p6 = wrapped_rsa_priv_qinv;
}
