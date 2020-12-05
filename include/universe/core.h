#ifndef UNIVERSE_CORE_H
#define UNIVERSE_CORE_H


#include "private.h"
#ifndef BSWABE_DEBUG
#define NDEBUG
#endif

void bswabe_setup( bswabe_pub_t** pub, bswabe_msk_t** msk );
bswabe_prv_t* bswabe_keygen( bswabe_pub_t* pub, bswabe_msk_t* msk, char** attributes );
bswabe_cph_t* bswabe_enc( bswabe_pub_t* pub, element_t m, char* policy);
void lagrange_coef( element_t r, GArray* s, int i );
void dec_leaf_flatten( element_t r, element_t exp, bswabe_policy_t* p, bswabe_prv_t* prv, bswabe_pub_t* pub );
int bswabe_dec( bswabe_pub_t* pub, bswabe_prv_t* prv, bswabe_cph_t* cph, element_t m );
#endif
