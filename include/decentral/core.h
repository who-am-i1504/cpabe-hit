#ifndef DECENTRAL_CORE_H
#define DECENTRAL_CORE_H
#include "private.h"
#include <openssl/bn.h>
#ifndef NUM_PRIMES
#define NUM_PRIMES 3
#endif
char * generate_a1_params(int lambda);
int global_setup(dcen_global_param **pub, int lambda);
dcen_authority_keys* authority_setup(dcen_global_param* pub, char *auth_id, char ** attributes);
dcen_personal_single_key* keygen(char *uid, char *attribute, dcen_secret_t* sk, dcen_global_param * pub);
void fill_policy_for_dcen( bswabe_policy_t* p, dcen_global_param* pub, element_t s, element_t w, dcen_cph_t* cph, GHashTable* pks);
dcen_cph_t * encrypt(element_t m, char *policy, dcen_global_param *pub, GHashTable *pks);
void dcen_dec_function(element_t r, element_t exp, bswabe_policy_t *p, dcen_personal_keys * prv, dcen_global_param * pub);
int decrypt(element_t m, dcen_cph_t * cph, dcen_global_param * pub, dcen_personal_keys *kp);
void merge_hash_table(GHashTable *now, GHashTable * old);
#endif