#ifndef LSSS_H
#define LSSS_H

#include <string.h>
#include <pbc.h>
#include <glib.h>
#include "access.h"

void raise_error(char* fmt, ...);
GString * string_policy_format(char * policy);
t_node * generate_tree(char *policy);
static GArray * array_add_element(GPtrArray * matrix, int *col, t_node * node);
GPtrArray * generate_access_array(t_node *policy);
int visit(GPtrArray * matrix, int i, int j);
void print_matrix(GPtrArray * matrix);
char ** generate_rhos(t_node * policy);
void print_rhos(char **rhos);

element_t* evaluate(pairing_t p, element_t x, int degree, element_t *coef);
element_t* generate_coef(pairing_t p, int degree, element_t zero_value);
#endif