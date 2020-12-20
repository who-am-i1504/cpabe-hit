#ifndef REVOCATION_H
#define REVOCATION_H
#include <math.h>
#include "policy.h"
#include <pbc.h>
#include <glib.h>
#include <stdarg.h>

typedef struct {
    char *pairing_desc;
    pairing_t p;
    element_t g;
    element_t egg;
    int AAs;
} drevo_pub_t;

typedef struct {
    element_t alpha;
    element_t y;
} drevo_ask_t;

typedef struct {
    element_t egg_alpha;
    element_t gy;
} drevo_apk_t;

typedef struct {
    element_t k0;
    element_t k1;
} drevo_prv_item_t;

typedef struct {
    int gid;
    GHashTable * sk;
    GPtrArray* attributes;
} drevo_prv_t;

typedef struct {
    element_t kek_j;
    int index;
} drevo_kek_node_t;

typedef struct {
    GNode * tree;
    char *attribute;
    char *aid;
} drevo_kek_tree_t;

typedef struct {
    element_t C1;
    element_t C2;
    element_t C3;
    element_t C4;

} drevo_cph_item_t;

typedef struct {
    char* policy_desc;
    bswabe_policy_t * policy;
    element_t C0;
} drevo_cph_t;

typedef struct {
    char *attr;
    int gid;
    GHashTable* tk;
    GPtrArray* attributes;
} drevo_tk_t;

typedef struct {
    element_t z;
} drevo_rk_t;

typedef struct {
    drevo_pub_t* pub;
    GHashTable *agk;
    drevo_prv_t* prv;
} drevo_mid_value;

typedef struct {
    drevo_pub_t* pub;
    GHashTable *agk;
    char* attribute;
    element_t* agku;
    GHashTable* apk;
} drevo_mid_value1;

typedef struct {
    char* policy;
    element_t C0;
    element_t egg_s_z;
} drevo_cph_out_t;

#endif