#include "revocation/core.h"

#ifndef TYPE_A_PARAMS
#define TYPE_A_PARAMS "type a\n" \
    "q 81869981414486565817042987620009425916711137248094272342132238763687306328559\n" \
    "r 604462909877683331530751\n" \
    "h 135442522736512392410054892783912140655846630328108560\n" \
    "exp1 46\n" \
    "exp2 79\n" \
    "sign0 -1\n" \
    "sign1 1\n"

#endif

#ifndef FGADR_ATTRIBUTE_NOTHING_KEY
#define FGADR_ATTRIBUTE_NOTHING_KEY "attribute-revocation-list-no-user"
#endif

arpu_pub_t *
global_setup(int lambda)
{
    arpu_pub_t * pub;
    pub = malloc(sizeof(arpu_pub_t));

    pub -> pairing_desc = strdup(TYPE_A_PARAMS);

    pairing_init_set_buf(pub -> p, pub -> pairing_desc, sizeof(pub -> pairing_desc));

    element_init_G1(pub -> g, pub -> p);
    element_init_GT(pub -> egg, pub -> p);

    element_random(pub -> g);
    pairing_apply(pub -> egg, pub -> g, pub -> g, pub -> p);

    return pub;
}

void 
aa_setup(arpu_auth_apk_t** pk, 
         arpu_auth_ask_t** sk, 
         arpu_pub_t * pub)
{
    (*pk) = malloc(sizeof(arpu_auth_apk_t));
    (*sk) = malloc(sizeof(arpu_auth_ask_t));

    element_init_Zr((*sk) -> alpha, pub -> p);
    element_init_Zr((*sk) -> y, pub -> p);
    element_init_GT((*pk) -> egg_alpha, pub -> p);
    element_init_G1((*pk) -> gy, pub -> p);

    element_random((*sk) -> alpha);
    element_random((*sk) -> y);

    element_pow_zn((*pk) -> egg_alpha, pub -> egg, (*sk) -> alpha);
    element_pow_zn((*pk) -> gy, pub -> g, (*sk) -> y);
}

void 
attr_keygen(arpu_prv_t *prv, 
            arpu_pub_t * pub,
            char *userid,
            char **user_attr,
            GHashTable* auth_attr)
{
    element_t *k0;
    element_t *k1;
    element_t t;
    element_t mul;

    element_init_Zr(t, pub -> p);
    element_init_G1(mul, pub -> p);
    
    while(*user_attr)
    {
        char *attr;
        attr = strdup(*(user_attr));

        if (g_hash_table_contains(auth_attr, attr))
        {
            arpu_auth_ask_t *ask;
            k0 = malloc(sizeof(element_t));
            k1 = malloc(sizeof(element_t));
            ask = g_hash_table_lookup(auth_attr, attr);

            element_init_G1(*k0, pub -> p);
            element_init_G1(*k1, pub -> p);

            element_random(t);

            element_pow_zn(*k1, pub -> g, t);

            element_pow_zn(*k0, pub -> g, ask -> alpha);
            element_from_string(mul, userid);
            element_pow_zn(mul, mul, ask -> y);
            element_mul(*k0, *k0, mul);

            element_from_string(mul, attr);
            element_pow_zn(mul, mul, t);
            element_mul(*k0, *k0, mul);

            g_hash_table_insert(prv -> k0, attr, k0);
            g_hash_table_insert(prv -> k1, attr, k1);

        }
        else free(attr);
    }
    element_clear(t);
    element_clear(mul);
}


