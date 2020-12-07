#ifndef DECENTRAL_PRIVATE_H
#define DECENTRAL_PRIVATE_H
#include <pbc.h>
#include <glib.h>
#include "policy.h"
#define MAX_PARAMS_LENGTH 2046

typedef struct {
    char *pairing_desc;
    pairing_t p;
    element_t g;
} dcen_global_param;

typedef struct {
    element_t egg_a;
    element_t gy;
} dcen_pub_t;

typedef struct {
    element_t a;
    element_t y;
} dcen_secret_t;

typedef struct {
    char *auth_id;
    GHashTable * pub_keys;
    GHashTable* secret_keys;
} dcen_authority_keys;

typedef struct {
    char *attribute;
    element_t* key;
} dcen_personal_single_key;

typedef struct {
    char* uid;
    GHashTable* keys;
} dcen_personal_keys;

typedef struct {
    element_t c1;
    element_t c2;
    element_t c3;
} dcen_cph_list;

typedef struct {
    bswabe_policy_t * p;
    element_t c0;
} dcen_cph_t;


#endif