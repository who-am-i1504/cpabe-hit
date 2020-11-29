#ifndef ABE_H
#define ABE_H
#include <pbc.h>
#include <glib.h>
#include <string.h>
#include "access.h"
#include "pairing_util.h"

struct public_keys_ser_parameter{
    char * pairing_desc;
    pairing_t p;
    element_t g;
    element_t u;
    element_t h;
    element_t w;
    element_t v;
    element_t egg_alpha;
};

typedef struct public_keys_ser_parameter pub_p;

struct master_secret_key_ser_parameter {
    element_t alpha;
};

typedef struct master_secret_key_ser_parameter msk_p;

struct secret_key_ser_parameter {
    element_t k0;
    element_t k1;
    
    GHashTable * k2s;
    GHashTable * k3s;
};

typedef struct secret_key_ser_parameter prv_sk;

struct header_ser_parameter {
    char ** rhos;
    element_t c0;
    GHashTable * c1s;
    GHashTable * c2s;
    GHashTable * c3s;    
};

typedef struct header_ser_parameter hsp_s;

struct ciphertext_ser_parameter{
    hsp_s header;
    element_t c;
};

typedef struct ciphertext_ser_parameter csp_s;


void setup(pub_p **pp, msk_p ** msk);
int keygen(prv_sk ** prv, msk_p *msk, pub_p * pp, char ** attributes);
int encrypt(csp_s ** cs, pub_p *pp, t_node* node, GPtrArray * matrix, char ** thos, element_t m);


#endif