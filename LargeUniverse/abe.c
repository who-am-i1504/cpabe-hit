#include <pbc/pbc.h>
#include <glib-2.0/glib.h>
#include <string.h>
#include "./abe.h"

#define TYPE_A_PARAMS 
    "type a\n"
    "q 81869981414486565817042987620009425916711137248094272342132238763687306328559\n"
    "r 604462909877683331530751\n"
    "h 135442522736512392410054892783912140655846630328108560\n"
    "exp1 46\n"
    "exp2 79\n"
    "sign0 -1\n"
    "sign1 1\n"

void setup(pub_p **pp, msk_p ** msk)
{
    *pp = malloc(sizeof(pub_p);
    *msk = malloc(sizeof(msk_p));

    (*pp) -> pairing_desc = strdup (TYPE_A_PARAMS);

    (*pp) -> p = pairing_init_set_buf((*pp) -> p, 
        (*pp) -> pairing_desc, strlen((*pp) -> pairing_desc));
    


    element_init_G1((*pp) -> g, (*pp) -> p);
    element_init_G1((*pp) -> u, (*pp) -> p);
    element_init_G1((*pp) -> h, (*pp) -> p);
    element_init_G1((*pp) -> w, (*pp) -> p);
    element_init_G1((*pp) -> v, (*pp) -> p);
    element_init_GT((*pp) -> egg_alpha, (*pp) -> p);

    element_init_Zr((*msk) -> alpha, (*pp) -> p);

    element_random((*msk) -> alpha);

    element_random((*pp) -> g);
    element_random((*pp) -> u);
    element_random((*pp) -> h);
    element_random((*pp) -> w);
    element_random((*pp) -> v);

    element_mul(
        (*pp) -> egg_alpha,(*pp) -> g, (*pp) -> g);

    element_pow_zn((*pp) -> egg_alpha,
        (*pp) -> egg_alpha, (*msk) -> alpha);
}

int keygen(prv_sk ** prv, msk_p *msk, pub_p * pp, char ** attributes)
{
    *prv = malloc(sizeof(prv_sk));

    element_t r;
    
    element_init_Zr(r,            (*pp) -> p);
    element_init_GT((*prv) -> k0, (*pp) -> p);
    element_init_GT((*prv) -> k1. (*pp) -> p);

    element_random(r);
    element_pow_zn((*prv) -> k1, (*pp) -> w, r);
    element_pow_zn((*prv) -> k0, (*pp) -> g, (*msk)-> alpha);
    element_mul((*prv) -> k0, (*prv) -> k0, (*prv) -> k1);

    element_pow_zn((*prv) -> k1, (*pp) -> g, r);

    (*prv) -> k2s = g_hash_table_new(g_str_hash, g_str_equal);
    (*prv) -> k3s = g_hash_table_new(g_str_hash, g_str_equal);

    while(*attributes)
    {
        char *attr = strdup((*attribute) ++);

        element_t e_attr;
        element_t r_i;
        element_t k2i;
        element_t k3i;

        element_init_G1(k2i, (*pp) -> p);
        element_init_G1(k3i, (*pp) -> p);
        element_init_Zr(r_i, (*pp) -> p);
        element_init_G1(e_attr, (*pp) -> p);

        element_random(r_i);
        element_from_hash(e_attr, attr, strlen(attr));

        element_pow_zn()




    }




    element_clear(r);
}