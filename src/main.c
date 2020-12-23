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
#include "revocation/core.h"
#include "drevocation/core.h"
#include "dacmacs/core.h"
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

/*

// 测试多授权属性加密代码
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
*/
/*
int main()
{

    char *policy = "(sysadmin and hire_date)" \
        " or (sysadmin and security_team and 2 of (executive_level, audit_group, strategy_team))";

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



    arcp_pub_t * pub;
    arcp_msk_t * msk;

    arcp_setup(&pub, &msk, 100);

    arcp_prv_t *prv1;
    prv1 = arcp_keygen(pub, msk, attrs);
    // printf("%d\t%d\n", prv1 -> i, prv1 -> j);
    arcp_prv_t *prv2;
    prv2 = arcp_keygen(pub, msk, attrs);
    // printf("%d\t%d\n", prv2 -> i, prv2 -> j);
    arcp_prv_t *prv3;
    prv3 = arcp_keygen(pub, msk, attrs);
    // printf("%d\t%d\n", prv3 -> i, prv3 -> j);

    arcp_cph_t * cph;
    element_t m;
    GArray * revo_list;
    revo_list = g_array_new(0, 1, sizeof(int));
    int i = 2;
    g_array_append_val(revo_list, i);


    cph = arcp_encrypt(pub, m, revo_list, p, 1);
    // element_printf("%B\n", m);

    element_t res;

    // printf("here\n");
    // arcp_reencrypt(pub, cph, 3);
    if (arcp_decrypt(pub, cph, prv1, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
    }
    
    // element_printf("%B\n", res);

    return 0;
}*/
/*
int main()
{
    char *policy = "(sysadmin and hire_date)" \
        " or (sysadmin and security_team and 2 of (executive_level, audit_group, strategy_team))";

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

    char *attributes_all[] ={
        "sysadmin",
        "hire_date",
        "security_team",
        "executive_level",
        "audit_group",
        "strategy_team",
        NULL
    };
    drevo_pub_t *pub;
    drevo_msk_t* msk;
    drevo_setup(&pub, &msk);

    GHashTable* pkxs;
    pkxs = g_hash_table_new(g_str_hash, g_str_equal);

    for (int i = 0; i < 6; i++)
    {
        drevo_pkx_t* pk;
        pk = drevo_pk_setup(pub, msk, attributes_all[i]);
        g_hash_table_insert(pkxs, attributes_all[i], pk);
    }

    drevo_prv_t* prv1;
    drevo_prv_t* prv2;

    prv1 = drevo_keygen(pub, msk, pkxs, attrs);
    prv2 = drevo_keygen(pub, msk, pkxs, attrs);

    drevo_cph_t* cph;
    element_t m;

    cph = drevo_encrypt(pub, m, pkxs, p);
    element_printf("%B\n", m);

    element_t res;

    if (drevo_decrypt(pub, cph, prv1, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
    }

    drevo_uk_t* uk;

    drevo_ukeygen(&uk, msk,
        *(element_t *)g_hash_table_lookup(msk -> Vx, "sysadmin"));
    
    drevo_cph_update(cph, uk, pub, "sysadmin");

    drevo_update_prv_key(prv2, msk, uk, "sysadmin");
    
    if (drevo_decrypt(pub, cph, prv1, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
    }

    if (drevo_decrypt(pub, cph, prv2, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
    }

    return 0;
}*/

int main()
{
    char *policy = "(sysadmin and hire_date)" \
        " or (sysadmin and security_team and 2 of (executive_level, audit_group, strategy_team))";

    char *p = parse_policy_lang(policy);
    GPtrArray* user_attributes;
    user_attributes = g_ptr_array_new();
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
        g_ptr_array_add(user_attributes, a->data);
        // printf("%s\n", *(attrs + j));
        j ++;
    }
    *(attrs + j) = NULL;

    dacmacs_capub_t* pub;
    dacmacs_camsk_t* msk;
    dacamacs_casetup(&pub, &msk);

    GHashTable *publickeys;
    publickeys = g_hash_table_new(g_str_hash, g_str_equal);

    dacmacs_auth_t *a1;
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

    GPtrArray *a1_attrs;
    GPtrArray *a2_attrs;
    a1_attrs = g_ptr_array_new();
    a2_attrs = g_ptr_array_new();
    g_ptr_array_add(a1_attrs, "sysadmin");
    g_ptr_array_add(a1_attrs, "security_team");

    g_ptr_array_add(a2_attrs, "hire_date");
    g_ptr_array_add(a2_attrs, "audit_group");
    g_ptr_array_add(a2_attrs, "strategy_team");
    g_ptr_array_add(a2_attrs, "executive_level");


    a1 = dacmacs_aasetup(pub, a1_attrs, "a1");
    dacmacs_auth_t *a2;
    a2 = dacmacs_aasetup(pub, a2_attrs, "a2");
    
    dacmacs_user_t* user1;
    user1 = dacmacs_user_register(pub, "user1", user_attributes);
    user1->sks = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(user1->sks, a1->aid,
        dacmacs_keygen(a1, pub, user1->cert, user1->attributes));

    g_hash_table_insert(user1->sks, a2->aid, 
        dacmacs_keygen(a2, pub, user1->cert, user1->attributes));

    dacmacs_user_t* user2;
    user2 = dacmacs_user_register(pub, "user2", user_attributes);
    user2->sks = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(user2->sks, a1->aid, 
        dacmacs_keygen(a1, pub, user2->cert, user2->attributes));

    g_hash_table_insert(user2->sks, a2->aid,
        dacmacs_keygen(a2, pub, user2->cert, user2->attributes));
    
    dacmacs_auth_item_t auth_item1, auth_item2;

    auth_item1.pks = a1->auth_pk;
    auth_item1.attr_pks = a1->attr_pks;
    auth_item1.attributes = a1->attributes;
    auth_item1.aid = a1->aid;

    auth_item2.pks = a2->auth_pk;
    auth_item2.attr_pks = a2->attr_pks;
    auth_item2.attributes = a2->attributes;
    auth_item2.aid = a2->aid;


    GPtrArray* auth_pks;

    auth_pks = g_ptr_array_new();

    g_ptr_array_add(auth_pks, &auth_item1);
    g_ptr_array_add(auth_pks, &auth_item2);

    element_t m;
    dacmacs_cph_t* cph;
    cph = dacmacs_encrypt(pub, auth_pks, m, p);

    element_t res;

    if (dacmacs_decrypt(cph, pub, user1, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
    }
    dacmacs_attr_msk_t* sys_msk;
    sys_msk = g_hash_table_lookup(a1->attr_msks, "sysadmin");
    dacmacs_ukeygen(a1->auth_msk, 
        pub, g_hash_table_lookup(a1->attr_pks, "sysadmin"), 
        g_hash_table_lookup(a1->attr_msks, "sysadmin"), "sysadmin");
    
    dacmacs_cph_update(cph, pub, 
        sys_msk->cuk, sys_msk->version, "sysadmin");

    if (dacmacs_decrypt(cph, pub, user1, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
    }

    dacmacs_secret_key_update(
        g_hash_table_lookup(user2->sks, "a1"), pub, user2->cert, sys_msk, "sysadmin");

    if (dacmacs_decrypt(cph, pub, user2, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
    }
    
    return 0;
}