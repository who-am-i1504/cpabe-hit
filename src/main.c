// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <glib.h>
// #include "policy_lang.h"
// #include "abe.h"

// // string_policy_format test
// /*
// int main()
// {
//     GString * res = string_policy_format("\\(\\)");
//     printf("here\n");
//     printf("%s \n", res -> str);
//     g_string_free(res, 1);
//     return 0;
// }*/


// // int main()
// // {
// //     t_node * p = generate_tree("A and B and (C or D)");

// //     t_node * c = p;
    
// //     return 0;
// // }

// int main()
// {
//     char *policy = "(sysadmin and hire_date < 946702800)" \
//         " or (security_team and 2 of (executive_level >= 5, audit_group, strategy_team))";

//     char *p = parse_policy_lang(policy);

//     // printf("%s\n", p);
//     // t_node * node = generate_tree(p);
    
//     // GPtrArray * matrix;
//     // matrix = generate_access_array(node);
//     // print_matrix(matrix);
//     // char ** rhos = generate_rhos(node);
//     // print_rhos(rhos);
//     // printf("here\n");
//     // free(p);

//     char *attributes[] = {
//         "sysadmin",
//         "hire_date = 2800",
//         "security_team",
//         "executive_level = 100",
//         "audit_group",
//         NULL
//     };
//     GSList* alist;
//     char **attrs;
//     alist = 0;
//     for (int i = 0; i < 4; i ++)
//     {
//         // printf("\t%s\n", attributes[i]);
//         parse_attribute(&alist, *(attributes + i));
//     }
//     int j = 0;
    
//     attrs = malloc(sizeof(char *) * (g_slist_length(alist) + 1));
//     for (GSList *a = alist; a!=NULL; a = a ->next)
//     {
//         *(attrs + j) = a -> data;
//         // printf("%s\n", *(attrs + j));
//         j ++;
//     }
//     *(attrs + j) = NULL;

//     pub_p * pp;
//     msk_p * msk;

//     setup(&pp, &msk);

//     prv_sk * prv;
//     keygen(&prv, msk, pp, attrs);

//     csp_s* cs;
//     element_t m;

//     element_init_GT(m, pp -> p);
//     element_random(m);

//     encrypt(&cs, pp, p, m);

//     element_t *res;
//     decryption(&res, pp, prv, cs);

//     printf("%d\n", element_cmp(m, res));
//     element_printf("The background M : %B\n", m);
//     element_printf("The decryption M : %B\n", res);
//     return 0;
    
// }
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "policy_lang.h"
#include "universe/core.h"

int main()
{
    char *policy = "(sysadmin and hire_date < 946702800)" \
        " or (sysadmin and security_team and 2 of (executive_level >= 5, audit_group, strategy_team))";

    char *p = parse_policy_lang(policy);
    

    // printf("%s\n", p);
    // t_node * node = generate_tree(p);
    
    // GPtrArray * matrix;
    // matrix = generate_access_array(node);
    // print_matrix(matrix);
    // char ** rhos = generate_rhos(node);
    // print_rhos(rhos);
    // printf("here\n");
    // free(p);

    char *attributes[] = {
        "sysadmin",
        "hire_date = 2800",
        "security_team",
        "executive_level = 100",
        "audit_group",
        NULL
    };
    GSList* alist;
    char **attrs;
    alist = 0;
    for (int i = 0; i < 5; i ++)
    {
        // printf("\t%s\n", attributes[i]);
        parse_attribute(&alist, *(attributes + i));
    }
    int j = 0;
    
    attrs = malloc(sizeof(char *) * (g_slist_length(alist) + 1));
    for (GSList *a = alist; a!=NULL; a = a ->next)
    {
        *(attrs + j) = a -> data;
        // printf("%s\n", *(attrs + j));
        j ++;
    }
    *(attrs + j) = NULL;

    bswabe_pub_t* pub;
    bswabe_msk_t* msk;

    bswabe_setup(&pub, &msk);

    bswabe_prv_t * prv;
    prv = bswabe_keygen(pub, msk, attrs);

    bswabe_cph_t* cph;
    element_t m;
    cph = bswabe_enc(pub, m, p);
    element_t res;
    bswabe_dec(pub, prv, cph, res);

    printf("%d\n", element_cmp(m, res));
    element_printf("The background M : %B\n", m);
    element_printf("The decryption M : %B\n", res);
    printf("%s \n", bswabe_error());
    return 0;
    
}