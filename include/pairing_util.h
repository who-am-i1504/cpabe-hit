#ifndef PAIRING_UTIL_H
#define PAIRING_UTIL_H

#include <pbc.h>
#include <glib.h>
#include "access.h"
#include "lsss.h"

enum PairingGroupType
{
    Zr,
    G1,
    G2,
    GT
};

int element_destory(element_t * e, int degree);
element_t* evaluate(pairing_t p, element_t x, int degree, element_t *coef);
element_t* generate_coef(pairing_t p, int degree, element_t zero_value);

void access_tree_node_secret_sharing(pairing_t p, element_t roor_secret, t_node *node, GHashTable *map);
GHashTable * secret_sharing(pairing_t p, element_t secret, t_node * root);

element_t * map_string_to_group(pairing_t p, char *message, int type);
element_t * calcoef(pairing_t p, GArray * set, int index);
#endif