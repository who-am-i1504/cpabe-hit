#ifndef REVOCATION_H
#define REVOCATION_H
#include "policy.h"
#include <pbc.h>
#include <glib.h>
typedef struct {
    char *pairing_desc;
    pairing_t p;
    element_t g;
    element_t egg;

} arpu_pub_t;

typedef struct {
    element_t egg_alpha;
    element_t gy;
} arpu_auth_apk_t;

typedef struct {
    element_t alpha;
    element_t y;
} arpu_auth_ask_t;

typedef struct {
     GHashTable * k0;
     GHashTable * k1;
} arpu_prv_t;

#endif