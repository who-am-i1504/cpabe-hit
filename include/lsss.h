#ifndef LSSS_H
#define LSSS_H

#include <string.h>
#include <pbc.h>
#include <glib.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include "access.h"
#include "pairing_util.h"

void raise_error(char* fmt, ...);
GString * string_policy_format(char * policy);
t_node * generate_tree(char *policy);
static GArray * array_add_element(GPtrArray * matrix, int *col, t_node * node);
GPtrArray * generate_access_array(t_node *policy);
int visit(GPtrArray * matrix, int i, int j);
void print_matrix(GPtrArray * matrix);
char ** generate_rhos(t_node * policy);
void print_rhos(char **rhos);


void free_tree_node(t_node * root);

st_node * pack_t_node(t_node * root, st_node * parent, int index);
void free_st_node (st_node * root);
int satify_node(st_node *root, char **attributes);
void calcoefficients(pairing_t p, GHashTable * map, st_node *root);
GHashTable * calcoeficient(pairing_t p, st_node * root, char ** attributes);
GHashTable * reconstruct_omegas(pairing_t p, char ** attributes, t_node *root);
int hash(char **res, char *attr);
#endif