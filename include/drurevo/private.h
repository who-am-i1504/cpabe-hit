/*
 * @Author: white john
 * @Date: 2020-12-21 15:40:10
 * @LastEditTime: 2020-12-21 21:52:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/drurevo/private.h
 */
#ifndef DRUREVO_PRIVARTE_H
#define DRUREVO_PRIVARTE_H
#include <pbc.h>
#include <glib.h>
#include "../policy.h"

typedef struct {
    char ** pairing_desc;
    pairing_t p;
    element_t g;
    element_t g_a;
    element_t g_beta;
    element_t g_beta_invert;
    element_t egg_alpha;
} dru_pub_t;

typedef struct {
    element_t a;
    element_t alpha;
    element_t beta;
    element_t gamma;
} dru_msk_t;

typedef struct {
    element_t alpha_i;
    element_t y;
    element_t vx;
} dru_attr_msk_t;

typedef struct {
    char *attribute;
    element_t egg_alpha_i;
    element_t gy;
    element_t pk1;
    element_t pk2;
} dru_attr_pk_t;

typedef struct {
    char * aid;
    GHashTable* attr_pk;
    GHashTable* attr_msk;
} dru_auth_key_t;

typedef struct {
    element_t kx;
    element_t hgid;
} dru_prv_item_t;

typedef struct {
    GPtrArray* attributes;
    element_t K;
    element_t L;
    GHashTable* attr_kx;
} dru_prv_t;

typedef struct {
    element_t C2;
    element_t C3;
    element_t C4;
    element_t C5;
} dru_cph_item_t;

typedef struct {
    char *policy;
    bswabe_policy_t* p;
    element_t C0;
    element_t C1;
} dru_cph_t;

#endif