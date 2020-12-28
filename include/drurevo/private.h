/*
 * @Author: white john
 * @Date: 2020-12-21 15:40:10
 * @LastEditTime: 2020-12-27 16:35:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/drurevo/private.h
 */
#ifndef DRUREVO_PRIVARTE_H
#define DRUREVO_PRIVARTE_H
#include <pbc.h>
#include <glib.h>
#include "vector.h"
#include "policy.h"

typedef struct {
    int m;
    int ctr;
    element_t h;
    element_t G;
    element_t H;
    element_t f;
    element_t* fs;
    element_t* EI;
    element_t* GI;
    element_t* ZI;
    element_t* HJ;
} drur_rupub_t;

typedef struct {
    char* pairing_desc;
    pairing_t p;
    element_t g;
    element_t ga;
    drur_rupub_t* rupub;
} drur_capub_t;

typedef struct {
    int m;
    element_t *alpha;
    element_t *r;
    element_t *c;
    // int ctr;
} drur_rumsk_t;

typedef struct {
    element_t a;
    drur_rumsk_t* rumsk;
} drur_camsk_t;

typedef struct {
    int id;
    char *uid;
    element_t u;
    element_t g_z_invert;
} drur_user_cert_t;

typedef struct {
    int m;
    element_t K0;
    element_t K1;
    element_t K2;
    element_t *K_j;
    GHashTable* Kijx;
    GPtrArray* attributes;
} drur_ru_sk_t;

typedef struct {
    element_t gpk;
    element_t gsk;
    drur_user_cert_t* cert;
    GHashTable* sks;
    GPtrArray* aids;
    drur_ru_sk_t* cask;
    GPtrArray* attributes;
} drur_user_t;

typedef struct {
    element_t egg_alpha;
    element_t g_beta_invert;
    element_t g_beta_gamma;
} drur_auth_pub_t;

typedef struct {
    element_t alpha;
    element_t beta;
    element_t gamma;
} drur_auth_msk_t;

typedef struct {
    int version;
    element_t pk;
} drur_attr_pk_t;

typedef struct {
    int version;
    element_t vx;
    element_t auk;
    element_t kuk;
    element_t cuk;
} drur_attr_msk_t;

typedef struct {
    char* aid;
    drur_auth_pub_t* auth_pk;
    drur_auth_msk_t* auth_msk;
    GHashTable* attr_pks;
    GHashTable* attr_msks;
    GPtrArray* attributes;
} drur_auth_t;

typedef struct {
    char *aid;
    drur_auth_pub_t* pks;
    GHashTable* attr_pks;
    GPtrArray* attributes;
} drur_auth_item_t;

typedef struct {
    int version;
    element_t kx;
} drur_prv_kx_t;

typedef struct {
    char *aid;
    element_t K;
    element_t L;
    element_t R;
    GHashTable* kx;
    GPtrArray* attributes;
} drur_prv_item_t;

typedef struct {
    int version;
    element_t C3;
    element_t D1;
    element_t D2;
    element_t P0;
    element_t P1;
    element_t P2;
} drur_cph_item_t;

typedef struct {
    int m;
    bswabe_polynomial_t** R0;
    bswabe_polynomial_t** R1;
    element_t *Q0;
    element_t *Q1;
    element_t *Q2;
    element_t *T;
    bswabe_polynomial_t** C0;
    bswabe_polynomial_t** C1;
    GHashTable* revo_list;
    GArray *revo_arr;
} drur_rucph_t;

typedef struct {
    element_t C1;
    GHashTable* C2;
    bswabe_policy_t* p;
    GPtrArray* aids;
    char *policy;
    drur_rucph_t* ru_cph;
} drur_cph_t;

#endif