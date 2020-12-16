#ifndef REVOCATION_H
#define REVOCATION_H
#include <math.h>
#include "policy.h"
#include <pbc.h>
#include <glib.h>
#include <stdarg.h>

typedef enum
{
    G1, Zr, GT
}element_type;

typedef struct {
    char *pairing_desc;
    pairing_t p;
    element_t g;        // g
    element_t h;        // h
    element_t f;        // f
    element_t G;        // G
    element_t H;        // H
    element_t *fs;      // f1,f2,f3, ... , fm
    element_t *egg_alpha;   // E
    element_t *gr;      // Gi
    element_t *gz;      // Zi
    element_t *gc;      // Hj
    int m;
} arcp_pub_t;

typedef struct {
    element_t *alpha;   // alpha1, alpha2, ..., alpham
    element_t *r;       // r1, r2, ..., rm
    element_t *c;       // c1, c2, ..., cm
    int m;
    int ctr;
} arcp_msk_t;

typedef struct {
    element_t k4;   
    element_t k5;
} arcp_prv_item_t;

typedef struct {
    int i,j;        // (i, j)
    GPtrArray * attributes; // S
    element_t k0;           // K_ij
    element_t k1;           // k'_ij
    element_t k2;           // k''_ij
    element_t *k3;          
    GHashTable *ks;

} arcp_prv_t;

typedef struct {
    element_t p0;
    element_t p1;
    element_t p2;
} arcp_cph_item;

typedef struct {
    bswabe_policy_t * policy;
    bswabe_polynomial_t** r0;
    bswabe_polynomial_t** r1;
    element_t* q0;
    element_t* q1;
    element_t* q2;
    element_t* t;
    bswabe_polynomial_t** c0;
    bswabe_polynomial_t** c1;
    GHashTable * revo_list;
}arcp_cph_t;

#endif