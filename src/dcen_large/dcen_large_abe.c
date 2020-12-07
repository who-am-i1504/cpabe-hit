#include "dcen_large/core.h"

void 
global_setup(dcen_large_pub_t ** pub, char *pairing_desc)
{
    (*pub) = malloc(sizeof(dcen_large_pub_t));

    (*pub) -> pairing_desc = strdup(pairing_desc);

    pairing_init_set_buf((*pub)->p, 
        (*pub) -> pairing_desc, strlen((*pub)->pairing_desc));
    element_init_G1((*pub)->g, (*pub)->p);
    element_random((*pub)->g);
}

dcen_large_authority_keys *
auth_setup(dcen_large_pub_t * pub, 
           char *attribute)
{
    dcen_large_authority_keys *auth_keys;

    auth_keys = malloc(sizeof(dcen_large_authority_keys));

    auth_keys -> attribute = strdup(attribute);

    auth_keys->pub_key = malloc(sizeof(dcen_large_public_key));
    auth_keys->secret_key = malloc(sizeof(dcen_large_secret_key));

    element_init_GT(auth_keys->pub_key->egg_alpha, pub -> p);
    element_init_G1(auth_keys->pub_key->gy, pub -> p);
    element_init_Zr(auth_keys->secret_key->alpha, pub -> p);
    element_init_Zr(auth_keys->secret_key->y, pub -> p);
    
    element_random(auth_keys->secret_key->alpha);
    element_random(auth_keys->secret_key->y);

    pairing_apply(auth_keys->pub_key->egg_alpha, pub -> g, pub -> g, pub -> p);

    element_pow_zn(auth_keys->pub_key->egg_alpha, 
        auth_keys->pub_key->egg_alpha, auth_keys->secret_key->alpha);
    element_pow_zn(
        auth_keys->pub_key->gy, pub -> g, auth_keys->secret_key->y);
}

dcen_large_person_key *
keygen( dcen_large_public_key* pub, 
        char *gid, 
        char *attribute, 
        dcen_large_secret_key* keys)
{
    element_t hgid;
    element_t fu;
     
    dcen_large_person_key * person_key;
    person_key = malloc(sizeof(dcen_large_person_key));

    
}
