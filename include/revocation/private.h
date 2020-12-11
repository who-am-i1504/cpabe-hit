#ifndef REVOCATION_H
#define REVOCATION_H
#include "policy.h"
#include <pbc.h>
#include <glib.h>

typedef struct {
    char * pairing_desc;
    pairing_t p;
    element_t g;
    element_t v_beta;
    element_t egg_beta;
    GHashTable* attr_pks;
}fgadr_pub_t;


typedef struct {
    element_t alpha;
    element_t beta;
    element_t v;
    element_t h;
    GHashTable * id;
    GHashTable* attr_sks;
}fgadr_msk_t;


typedef struct {
    char * attribute;
    element_t s;
    element_t w;
} fgadr_auth_sk;

typedef struct {
    element_t lamda;
    element_t beta;
    element_t w_star;
    GHashTable * sk_attrs;
} fgadr_user_sk;

typedef struct {
    element_t lambda;
    element_t list_rev;
} fgadr_rev_item;

typedef struct {
    element_t t;
    element_t gt;
} fgadr_attr_pk_sk;

typedef struct {
    element_t h;
    element_t alpha;
    GHashTable * pks;
    GHashTable * mk;
    GHashTable *revo_list;
    GHashTable *attr_pair;
} fgadr_auth_t;

typedef struct {
    element_t c0;
    element_t c1;
    bswabe_policy_t *policy;
    char *p;
    GHashTable * crev;
    GHashTable * crev_nul;
} fgadr_cph_t;

typedef struct {
    bswabe_polynomial_t* c2;
    bswabe_polynomial_t* c3;
} fgadr_cs_other;

typedef struct {
    element_t r;
    fgadr_cph_t* cph;
    GHashTable * revolist;
} fgadr_cph_mid_t;

#endif