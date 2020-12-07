#ifndef DCEN_LARGE_H
#define DCEN_LARGE_H
#include <pbc.h>
#include <glib.h>
#include "policy.h"

typedef struct {
    char * pairing_desc;
    pairing_t p;
    element_t g;
} dcen_large_pub_t;

typedef struct {
    element_t egg_alpha;
    element_t gy;
} dcen_large_public_key;

typedef struct {
    element_t alpha;
    element_t y;
} dcen_large_secret_key;

typedef struct {
    char *attribute;
    dcen_large_public_key * pub_key;
    dcen_large_secret_key * secret_key;
} dcen_large_authority_keys;

typedef struct {
    element_t kgid_u;
    element_t kgid_t;
} dcen_large_person_key;

typedef struct {
    element_t c1;
    element_t c2;
    element_t c3;
    element_t c4;
} dcen_large_clist;

typedef struct {
    element_t c0;
    bswabe_policy_t *p;
} dcen_large_cph_t;

#endif