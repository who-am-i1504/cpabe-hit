
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
#include "drurevo/core.h"

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

    while(1)
    {

    drur_capub_t* pub;
    drur_camsk_t* msk;
    drur_casetup(&pub, &msk, 100, 160);

    drur_auth_t *a1;
    a1 = drur_aasetup(pub, a1_attrs, "a1");
    drur_auth_t *a2;
    a2 = drur_aasetup(pub, a2_attrs, "a2");
    
    drur_user_t* user1;
    user1 = drur_user_register(pub, "user1", user_attributes);
    user1->sks = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(user1->sks, a1->aid,
        drur_aakeygen(a1, pub, user1->cert, user1->attributes));

    g_hash_table_insert(user1->sks, a2->aid, 
        drur_aakeygen(a2, pub, user1->cert, user1->attributes));
    
    user1->cask = drur_cakeygen(pub, msk, user1->attributes, user1->cert->id);

    drur_user_t* user2;
    user2 = drur_user_register(pub, "user2", user_attributes);
    user2->sks = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(user2->sks, a1->aid, 
        drur_aakeygen(a1, pub, user2->cert, user2->attributes));

    g_hash_table_insert(user2->sks, a2->aid,
        drur_aakeygen(a2, pub, user2->cert, user2->attributes));
    
    user2->cask = drur_cakeygen(pub, msk, user2->attributes, user2->cert->id);

    drur_user_t* user3;
    user3 = drur_user_register(pub, "user3", user_attributes);
    user3->sks = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(user3->sks, a1->aid, 
        drur_aakeygen(a1, pub, user3->cert, user3->attributes));

    g_hash_table_insert(user3->sks, a2->aid,
        drur_aakeygen(a2, pub, user3->cert, user3->attributes));
    
    user3->cask = drur_cakeygen(pub, msk, user3->attributes, user3->cert->id);
    
    drur_auth_item_t auth_item1, auth_item2;

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
    drur_cph_t* cph;

    GArray * revo_list;
    revo_list = g_array_new(0, 1, sizeof(int));
    int i = 2;
    g_array_append_val(revo_list, i);

    cph = drur_encrypt(pub, auth_pks, m, p, revo_list, 1);
    // g_array_free(revo_list, TRUE);
    element_t res;

    if (drur_decrypt(cph, pub, user1, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
    }
    element_clear(res);
    drur_attr_msk_t* sys_msk;
    
    sys_msk = g_hash_table_lookup(a1->attr_msks, "sysadmin");
    drur_ukeygen(a1->auth_msk, 
        pub, g_hash_table_lookup(a1->attr_pks, "sysadmin"), 
        g_hash_table_lookup(a1->attr_msks, "sysadmin"), "sysadmin");
    
    drur_cph_update(cph, pub, 
        sys_msk->cuk, sys_msk->version, "sysadmin");

    if (drur_decrypt(cph, pub, user1, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
        element_clear(res);
    }
    
    drur_secret_key_update(
        g_hash_table_lookup(user2->sks, "a1"), pub, user2->cert, sys_msk, "sysadmin");

    drur_secret_key_update(
        g_hash_table_lookup(user3->sks, "a1"), pub, user3->cert, sys_msk, "sysadmin");

    if (drur_decrypt(cph, pub, user2, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);
        element_clear(res);
    }
    
    if (drur_decrypt(cph, pub, user3, res))
    {
        printf("%s\n", bswabe_error());
    }
    else
    {
        printf("比较原数据与解密后数据是否相同：%s\n", element_cmp(m, res)?"FALSE":"TRUE");
        element_printf("The background M : %B\n", m);
        element_printf("The decryption M : %B\n", res);    
        element_clear(res);
    }
    
    element_clear(m);
    drur_realse_auth(a1);
    drur_realse_auth(a2);
    drur_realse_user(user1);
    printf("user1\n");
    drur_realse_user(user2);
    printf("user2\n");
    drur_realse_user(user3);
    printf("user3\n");
    g_ptr_array_free(auth_pks, 0);
    printf("auth_pks\n");
    drur_realse_camsk(msk);
    printf("msk\n");
    drur_realse_capub(pub);
    printf("pub\n");
    }

    
    return 0;
}