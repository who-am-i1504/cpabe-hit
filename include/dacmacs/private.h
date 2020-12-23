/*
 * @Author: your name
 * @Date: 2020-12-22 15:32:50
 * @LastEditTime: 2020-12-23 19:31:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/dacmacs/private.h
 */
#ifndef DACMACS_PRIVATE_H
#define DACMACS_PRIVATE_H
#include <pbc.h>
#include <glib.h>
#include "policy.h"

typedef struct {
    char* pairing_desc;
    pairing_t p;
    element_t g;
    element_t ga;
} dacmacs_capub_t;

typedef struct {
    element_t a;
} dacmacs_camsk_t;

typedef struct {
    char * uid;
    element_t u;
    element_t g_z_invert;
} dacmacs_user_cert_t;

typedef struct {
    element_t gpk;
    element_t gsk;
    dacmacs_user_cert_t* cert;
    GHashTable* sks;
    GPtrArray* attributes;
} dacmacs_user_t;

typedef struct {
    element_t egg_alpha;
    element_t g_beta_invert;
    element_t g_beta_gamma;
} dacmacs_auth_pub_t;

typedef struct {
    element_t alpha;
    element_t beta;
    element_t gamma;
} dacmacs_auth_msk_t;

typedef struct {
    int version;
    element_t pk;
} dacmacs_attr_pk_t;

typedef struct {
    int version;
    element_t vx;
    element_t auk;
    element_t kuk;
    element_t cuk;
} dacmacs_attr_msk_t;

typedef struct {
    char * aid;
    dacmacs_auth_pub_t* auth_pk;
    dacmacs_auth_msk_t* auth_msk;
    GHashTable* attr_pks;
    GHashTable* attr_msks;
    GPtrArray* attributes;
} dacmacs_auth_t;

typedef struct {
    char *aid;
    dacmacs_auth_pub_t* pks;
    GHashTable* attr_pks;
    GPtrArray* attributes;
} dacmacs_auth_item_t;

typedef struct {
    int version;
    element_t kx;
} dacmacs_prv_kx_t;

typedef struct {
    char *aid;
    element_t K;
    element_t L;
    element_t R;
    GHashTable* kx;
    GPtrArray* attributes;
} dacmacs_prv_item_t;

typedef struct {
    int version;
    element_t C3;
    element_t D1;
    element_t D2;
} dacmacs_cph_item_t;

typedef struct {
    element_t C0;
    element_t C1;
    GHashTable* C2;
    bswabe_policy_t* p;
    GPtrArray* aids;
    char * policy;
} dacmacs_cph_t;

#endif //DACMACS_PRIVATE_H
