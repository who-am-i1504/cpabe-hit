#include "abe.h"

#define TYPE_A_PARAMS \
    "type a\n" \
    "q 81869981414486565817042987620009425916711137248094272342132238763687306328559\n" \
    "r 604462909877683331530751\n" \
    "h 135442522736512392410054892783912140655846630328108560\n" \
    "exp1 46\n" \
    "exp2 79\n" \
    "sign0 -1\n" \
    "sign1 1\n"

void setup(pub_p **pp, msk_p ** msk)
{
    *pp = malloc(sizeof(pub_p));
    *msk = malloc(sizeof(msk_p));

    (*pp) -> pairing_desc = strdup (TYPE_A_PARAMS);

    pairing_init_set_buf((*pp) -> p, 
        (*pp) -> pairing_desc, strlen((*pp) -> pairing_desc));
    


    element_init_G1((*pp) -> g, (*pp) -> p);
    element_init_G1((*pp) -> u, (*pp) -> p);
    element_init_G1((*pp) -> h, (*pp) -> p);
    element_init_G1((*pp) -> w, (*pp) -> p);
    element_init_G1((*pp) -> v, (*pp) -> p);
    element_init_GT((*pp) -> egg_alpha, (*pp) -> p);

    element_init_Zr((*msk) -> alpha, (*pp) -> p);

    element_random((*msk) -> alpha);

    element_random((*pp) -> g);
    element_random((*pp) -> u);
    element_random((*pp) -> h);
    element_random((*pp) -> w);
    element_random((*pp) -> v);

    pairing_apply(
        (*pp) -> egg_alpha, (*pp) -> g, (*pp) -> g, (*pp) -> p);

    element_pow_zn((*pp) -> egg_alpha,
        (*pp) -> egg_alpha, (*msk) -> alpha);
}

int keygen(prv_sk ** prv, msk_p *msk, pub_p * pp, char ** attributes)
{
    *prv = malloc(sizeof(prv_sk));

    element_t r;
    element_t k3temp;
    
    element_init_Zr(r,            pp -> p);
    element_init_G1((*prv) -> k0, pp -> p);
    element_init_G1((*prv) -> k1, pp -> p);
    element_init_G1(k3temp, pp -> p);

    element_random(r);
    element_pow_zn((*prv) -> k1, pp -> w, r);
    element_pow_zn((*prv) -> k0, pp -> g, msk-> alpha);
    element_mul((*prv) -> k0, (*prv) -> k0, (*prv) -> k1);

    element_pow_zn((*prv) -> k1, pp -> g, r);

    element_neg(k3temp, r);
    element_pow_zn(k3temp, pp -> v, k3temp);

    (*prv) -> k2s = g_hash_table_new(g_str_hash, g_str_equal);
    (*prv) -> k3s = g_hash_table_new(g_str_hash, g_str_equal);

    GPtrArray * mid;
    mid = g_ptr_array_new();

    while(*attributes)
    {
        char *attr = strdup(*(attributes ++));
        g_ptr_array_add(mid, attr);
        element_t e_attr;
        element_t r_i;
        element_t *k2i;
        element_t *k3i;

        k2i = malloc(sizeof(element_t));
        k3i = malloc(sizeof(element_t));

        element_init_G1(*k2i, pp -> p);
        element_init_G1(*k3i, pp -> p);
        element_init_Zr(r_i, pp -> p);
        element_init_G1(e_attr, pp -> p);

        element_random(r_i);
        element_from_hash(e_attr, attr, strlen(attr));

        element_pow_zn(*k2i, pp -> g, r_i);
        
        element_pow_zn(*k3i, pp -> u, e_attr);
        element_mul(*k3i, *k3i, pp -> h);
        element_pow_zn(*k3i, *k3i, r_i);
        element_mul(*k3i, *k3i, k3temp);

        g_hash_table_insert((*prv) -> k2s, strdup(attr), k2i);
        g_hash_table_insert((*prv) -> k3s, strdup(attr), k3i);

        element_clear(e_attr);
        element_clear(r_i);
    }

    (*prv) -> attribute = malloc(sizeof(char *) * ((mid -> len) + 1));
    for (int i = 0; i < mid -> len; i ++)
    {
        *((*prv) -> attribute + i) = mid -> pdata[i];
    }
    *(((*prv) -> attribute) + (mid -> len)) = NULL;
    g_ptr_array_free(mid, FALSE);
    element_clear(k3temp);
    element_clear(r);

    return 0;
}

int encrypt(csp_s **cs, pub_p * pp, char *policy, element_t m)
{
    int res = 0;
    element_t s;
    element_t session_key;
    t_node *node = generate_tree(policy);
    GPtrArray *matrix = generate_access_array(node);
    char **rhos = generate_rhos(node);

    *cs = malloc(sizeof(csp_s));
    // (*cs) -> header = malloc(sizeof(hsp_s));

    element_init_Zr(s, pp -> p);
    element_init_GT(session_key, pp -> p);
    element_init_G1((*cs) -> c0, pp -> p);
    element_init_GT((*cs) -> c, pp -> p);

    element_random(s);
    element_pow_zn(session_key, pp -> egg_alpha, s);
    element_pow_zn((*cs) -> c0, pp -> g, s);

    GHashTable * map;
    map = secret_sharing(pp -> p, s, node);
    if (map == NULL)
    {
        element_clear(s);
        element_clear(session_key);
        free(*cs);
        res = 1;
        goto destory;
    }
    GHashTable * c1s = NULL;
    GHashTable * c2s = NULL;
    GHashTable * c3s = NULL;

    (*cs) -> c1s = g_hash_table_new(g_str_hash, g_str_equal);
    (*cs) -> c2s = g_hash_table_new(g_str_hash, g_str_equal);
    (*cs) -> c3s = g_hash_table_new(g_str_hash, g_str_equal);

    c1s = (*cs) -> c1s;
    c2s = (*cs) -> c2s;
    c3s = (*cs) -> c3s;


    GList * list = g_hash_table_get_keys(map);

    for (GList *p = list; p != NULL; p = p -> next)
    {
        // 将属性值映射到群上
        element_t * element_rho = map_string_to_group(pp -> p, p -> data, Zr);
        if (element_rho == NULL)
        {
            g_list_free(list);
            g_hash_table_destroy(map);
            element_clear(s);
            element_clear(session_key);
            free(*cs);
            res = 1;
            goto destory;
        }
        element_t ti;

        element_init_Zr(ti, pp -> p);
        element_random(ti);

        element_t * res;
        element_t before, after;

        element_init_G1(before, pp -> p);
        element_init_G1(after, pp -> p);

        // 计算 (w ^ attribute) * (v ^ ti)
        element_pow_zn(before, 
            pp -> w, *((element_t *)g_hash_table_lookup(map, p -> data)));
        
        element_pow_zn(after, pp -> v, ti);
        element_mul(before, before, after);
        
        res = malloc(sizeof(element_t));

        element_init_G1(*res, pp -> p);
        element_set(*res, before);

        g_hash_table_insert(c1s, strdup(p -> data), res);

        // printf("insert into c1s is :\t%s\t%ld\n", strdup(p -> data), g_hash_table_lookup((*cs) -> c1s, p -> data));
        // element_printf("encryption before : \t%B\n", *res);

        res = malloc(sizeof(element_t));
        element_init_G1(*res, pp -> p);

        // 计算 (u ^ element_rho * h) ^ (-ti)
        element_pow_zn(before, pp -> u, element_rho);
        element_mul(before, before, pp -> h);
        // element_printf("encryption before : \t%B\n", before);
        element_t neg_t_i;
        element_init_Zr(neg_t_i, pp -> p);
        element_neg(neg_t_i, ti);
        element_pow_zn(*res, before, neg_t_i);

        g_hash_table_insert(c2s, strdup(p -> data), res);
        // printf("insert into c2s is :\t%s\n", p -> data);
        // element_printf("encryption before : \t%B\n", *res);

        res = malloc(sizeof(element_t));
        element_init_G1(*res, pp -> p);
        element_pow_zn(*res, pp -> g, ti);
        // element_printf("encryption before : \t%B\n", ti);

        g_hash_table_insert(c3s, strdup(p -> data), res);
        // printf("insert into c3s is :\t%s\n", p -> data);
        // element_printf("encryption before : \t%B\n", *res);

        element_clear(neg_t_i);
        element_clear(before);
        element_clear(after);
        element_clear(ti);
        element_clear(*element_rho);
        free(element_rho);
    }
    g_list_free(list);

    element_mul((*cs) -> c, session_key, m);
    // element_printf("encryption m : \t%B\n", m);
    // element_printf("encryption session_key : \t%B\n", session_key);
    // element_printf("encryption cs -> c : \t%B\n", (*cs) -> c);
    // element_printf("encryption cs -> header.c0 : \t%B\n", (*cs) -> header.c0);
    (*cs) -> policy = strdup(policy);
    g_hash_table_destroy(map);
    element_clear(s);
    element_clear(session_key);

destory:
    for (int i = 0; i < matrix -> len; i ++)
    {
        g_array_free(matrix -> pdata[i], TRUE);
    }
    g_ptr_array_free(matrix, TRUE);
    for (char **p = rhos; p != NULL; p ++)
    {
        free(*p);
    }
    free(rhos);
    // printf("insert into c1s is :\t%s\t%ld\n", "audit_group", g_hash_table_lookup((*cs) -> c1s, "audit_group"));
    free_tree_node(node);

    return res;
}

int decryption(element_t **m, pub_p *pp, prv_sk *prv, csp_s *cs)
{
    GPtrArray *matrix;
    t_node * node;
    char **rhos;
    int res = 0;
    char * policy;

    *m = malloc(sizeof(element_t));

    policy = cs -> policy;

    node = generate_tree(policy);
    if (node == NULL)
    {
        res = 1;
        goto dec_destory;
    }
    matrix = generate_access_array(node);
    if (matrix == NULL)
    {
        res = 1;
        goto dec_destory;
    }
    rhos = generate_rhos(node);
    if (rhos == NULL)
    {
        res = 1;
        goto dec_destory;
    }

    GHashTable * omegas_map = reconstruct_omegas(pp -> p, prv->attribute, node);
    if (omegas_map == NULL)
    {
        res = 1;
        goto dec_destory;
    }

    element_t session_key;
    element_init_GT(session_key, pp -> p);
    pairing_apply(session_key, cs -> c0, prv -> k0, pp -> p);
    
    // element_printf("encryption before : \t%B\n", session_key);
    element_t A;
    element_init_GT(A, pp -> p);
    element_set1(A);

    GList * key_set = g_hash_table_get_keys(omegas_map);

    for (GList * p = key_set; p != NULL; p = p->next)
    {
        // printf("%s\t%ld\n", p -> data, g_hash_table_lookup(cs -> c1s, p -> data));
        
        element_t * c1;
        element_t * c2;
        element_t * k2;
        element_t * c3;
        element_t * k3;

        c1 = g_hash_table_lookup(cs -> c1s, p -> data);
        c2 = g_hash_table_lookup(cs -> c2s, p -> data);
        k2 = g_hash_table_lookup(prv -> k2s, p -> data);
        c3 = g_hash_table_lookup(cs -> c3s, p -> data);
        k3 = g_hash_table_lookup(prv -> k3s, p -> data);
        printf("%s\n", p -> data);

        element_t * lambda = g_hash_table_lookup(omegas_map, p -> data);

        element_t mid1;
        element_t mid2;

        element_init_GT(mid1, pp -> p);
        element_init_GT(mid2, pp -> p);

        pairing_apply(mid1, *c1, prv -> k1, pp -> p);
        // element_printf("encryption *c2 and k2 :\t%B\t%B\n", *c2, *k2);
        pairing_apply(mid2, *c2, *k2, pp -> p);

        element_mul(mid1, mid1, mid2);

        pairing_apply(mid2, *c3, *k3, pp -> p);

        element_mul(mid1, mid1, mid2);
        element_pow_zn(mid1, mid1, *lambda);

        element_mul(A, A, mid1);

        element_clear(mid1);
        element_clear(mid2);
    }

    g_list_free(key_set);
    element_div(session_key, session_key, A);

    element_init_GT(**m, pp -> p);
    // element_printf("%B\n", session_key);
    element_div(**m, cs -> c, session_key);
    element_printf("%B\n", *m);

    element_clear(A);
    element_clear(session_key);

dec_destory:
    if (res)free(*m);
    if (key_set)
    {
        free_hash_value_element(omegas_map);
    }
    if (matrix != NULL)
    {
        for (int i = 0; i < matrix -> len; i ++)
        {
            g_array_free(matrix -> pdata[i], TRUE);
        }
        g_ptr_array_free(matrix, TRUE);
    }
    if (rhos)
    {
        for (char **p = rhos; p != NULL; p ++)
        {
            free(*p);
        }
        free(rhos);
    }
    if (node)
        free_tree_node(node);
    return res;
}