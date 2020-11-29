#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lsss.h"
#include <glib.h>
#include "policy_lang.h"

// string_policy_format test
/*
int main()
{
    GString * res = string_policy_format("\\(\\)");
    printf("here\n");
    printf("%s \n", res -> str);
    g_string_free(res, 1);
    return 0;
}*/


// int main()
// {
//     t_node * p = generate_tree("A and B and (C or D)");

//     t_node * c = p;
    
//     return 0;
// }

int main()
{
    char *policy = "(sysadmin and (hire_date < 946702800 or security_team)) or (business_staff and 2 of (executive_level >= 5, audit_group, strategy_team))";

    char *p = parse_policy_lang(policy);

    printf("%s\n", p);
    t_node * node = generate_tree(p);
    
    GPtrArray * matrix;
    matrix = generate_access_array(node);
    print_matrix(matrix);
    char ** rhos = generate_rhos(node);
    print_rhos(rhos);
    printf("here\n");
    free(p);

    return 0;
    
}