/*
 * @Author: your name
 * @Date: 2020-12-17 09:30:46
 * @LastEditTime: 2020-12-20 20:12:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/drevocation/private.h
 */
#ifndef DREVOCATION_H
#define DREVOCATION_H
#include <math.h>
#include "policy.h"
#include <pbc.h>
#include <glib.h>
#include <stdarg.h>

typedef struct {
    char * pairing_desc;
    pairing_t p;
    element_t g;
    element_t g_a;
    element_t g_beta;
    element_t g_beta_invert;
    element_t egg_alpha;
} drevo_pub_t;

typedef struct {
    element_t alpha;
    element_t beta;
    element_t gamma;
    element_t a;
    GHashTable* Vx;
} drevo_msk_t;

typedef struct {
    char *attribute;
    element_t pk1;
    element_t pk2;
} drevo_pkx_t;

typedef struct {
    GPtrArray* attrs;
    element_t K;
    element_t L;
    GHashTable* Kx;
} drevo_prv_t;

typedef struct {
    element_t C1;
    element_t D1;
    element_t D2;
} drevo_cph_item_t;

typedef struct {
    element_t C;
    element_t C1;
    char *policy;
    bswabe_policy_t* p;
} drevo_cph_t;


typedef struct {
    element_t uk1;
    element_t uk2;
} drevo_uk_t;

#endif