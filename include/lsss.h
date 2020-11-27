#include <glib.h>
#include "abe.h"
GString * string_policy_format(char * policy);
t_node * generate_tree(char *policy);
GArray * generate_access_array(char *policy);