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
#include "decentral/core.h"
/*
int main()
{
    char *policy = "(sysadmin and hire_date < 946702800)" \
        " or (sysadmin and security_team and 2 of (executive_level >= 5, audit_group, strategy_team))";

    char *p = parse_policy_lang(policy);
    

    // printf("%s\n", p);
    // t_node * node = generate_tree(p);
    
    // GPtrArray * matrix;
    // // matrix = generate_access_array(node);
    // // print_matrix(matrix);
    // // char ** rhos = generate_rhos(node);
    // // print_rhos(rhos);
    // // printf("here\n");
    // // free(p);

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

    printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
    element_printf("The background M : %B\n", m);
    element_printf("The decryption M : %B\n", res);
    printf("%s \n", bswabe_error());

    // printf("%s\n", generate_a1_params(160));
    return 0;
    
}

/*/
int main()
{
    char *policy = "(sysadmin and hire_date)" \
        " or (sysadmin and security_team and 2 of (executive_level, audit_group, strategy_team))";

    char *p = parse_policy_lang(policy);
    
    char *attributes[] = {
        "sysadmin",
        "hire_date",
        "security_team",
        "executive_level",
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

    dcen_global_param * pub;
    global_setup(&pub, 160);

    GHashTable *publickeys;
    publickeys = g_hash_table_new(g_str_hash, g_str_equal);

    dcen_authority_keys *a1;
    char *a1_attributes[3] = {
        "sysadmin",
        "security_team",
        NULL
    };
    char *a2_attributes[5] = {
        "hire_date",
        "audit_group",
        "executive_level",
        "strategy_team",
        NULL
    };

    char **a1_attrs;
    char **a2_attrs;
    a1_attrs = malloc(sizeof(char *) * 3);
    a2_attrs = malloc(sizeof(char *) * 5);
    *(a1_attrs) = "sysadmin";
    *(a1_attrs + 1) = "security_team";
    *(a1_attrs + 2) = NULL;

    *(a2_attrs) = "hire_date";
    *(a2_attrs + 1) = "audit_group";
    *(a2_attrs + 2) = "strategy_team";
    *(a2_attrs + 3) = "executive_level";
    *(a2_attrs + 4) = NULL;



    a1 = authority_setup(pub, "a1", a1_attrs);
    dcen_authority_keys *a2;
    a2 = authority_setup(pub, "a2", a2_attrs);

    merge_hash_table(publickeys, a1 -> pub_keys);
    merge_hash_table(publickeys, a2 -> pub_keys);

    dcen_personal_keys pkeys;
    pkeys.uid = "user";
    pkeys.keys = g_hash_table_new(g_str_hash, g_str_equal);

    for (int i = 0; *(attrs + i); i ++)
    {
        dcen_personal_single_key * key;
        if (g_hash_table_contains(a1 -> secret_keys, *(attrs + i)))
            key = keygen(pkeys.uid, *(attrs + i), g_hash_table_lookup(a1 -> secret_keys, *(attrs + i)), pub);
        else
            key = keygen(pkeys.uid, *(attrs + i), g_hash_table_lookup(a2 -> secret_keys, *(attrs + i)), pub);
        g_hash_table_insert(pkeys.keys, *(attrs + i), key);
        
        // printf("%s\n", *(attrs + i));
    }

    element_t m, res;

    dcen_cph_t * cph;
    cph = encrypt(m, p, pub, publickeys);


    if (decrypt(res, cph, pub, &pkeys))printf("%s\n", bswabe_error());

    printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
    element_printf("The background M : %B\n", m);
    element_printf("The decryption M : %B\n", res);
    
    return 0;
}