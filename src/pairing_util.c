#include "pairing_util.h"

int element_destory(element_t * e, int degree)
{
    for (int i = 0 ; i < degree; i ++)
    {
        element_clear(*(e + i));
    }
    free(e);
}

element_t* evaluate(pairing_t p, element_t x, int degree, element_t* coef)
{
    element_t *result;
    element_t temp;
    result = malloc(sizeof(element_t));
    element_init_Zr(*result, p);
    element_init_Zr(temp, p);

    element_set0(result);
    element_set1(temp);

    for (int i = 0; i < degree; i ++)
    {
        for (int j = 0; j < i; j ++)
        {
            // element_printf("evaluate1 temp x is : \t%B\t%B\n", temp, x);
            element_mul(temp, temp, x);
        }
        // element_printf("evaluate2 temp coef + i is : \t%B\t%B\n", temp, *(coef + i));
        element_mul(temp, temp, *(coef + i));
        element_add(*result, *result, temp);
        element_set1(temp);
    }

    element_clear(temp);

    return result;
}

element_t *generate_coef(pairing_t p, int degree, element_t zero_value)
{
    element_t *res;

    res = (element_t *)malloc(sizeof(element_t) * (degree));

    element_init_Zr(*res, p);
    element_set(*res, zero_value);
    // element_printf("labdas is : %B\t%B\n", *res, zero_value);
    int i;
    for (i = 1; i < degree; i ++)
    {
        element_init_Zr(*(res + i), p);
        element_random(*(res + i));
        // element_printf("labdas is : %B\n", *(res + i));
    }

    return res;
}

void access_tree_node_secret_sharing(pairing_t p, element_t root_secret, t_node *node, GHashTable *map)
{
    if (node -> attr == NULL)
    {
        element_t *mid;
        mid = generate_coef(p, node->k, root_secret);
        for (int i = 0; i < node -> children -> len; i ++)
        {
            element_t* e;
            element_t x;
            // element_printf("root_secret is : %B\n", root_secret);
            element_init_Zr(x, p);
            element_set_si(x, i + 1);
            e = evaluate(p, x, 
                node -> k, mid);
            // element_printf("root_children_secret is : %B\n", *e);
            
            access_tree_node_secret_sharing(p,
                *e, (t_node *)(node -> children -> pdata[i]), map);
            element_clear(x);
            element_clear(*e);
            free(e);
        }
        element_destory(mid, node -> k);
    }
    else
    {
        element_t *e;
        e = malloc(sizeof(element_t));

        element_init_Zr(*e, p);
        element_set(*e, root_secret);
        g_hash_table_insert(map, node -> attr, e);
        // printf("\t%s\t", node -> attr);
        // element_printf("%B\n", *e);
        // element_printf("labdas is : %B\n", root_secret);
    }
    
}

GHashTable* secret_sharing(pairing_t p, element_t secret, t_node * root)
{
    GHashTable *shared_elements_map;
    shared_elements_map = g_hash_table_new(g_str_hash, g_str_equal);
    // for (int i = 0; i < )
    // element_printf("secret is : %B\n", secret);
    access_tree_node_secret_sharing(p, secret, root, shared_elements_map);
    return shared_elements_map;
}

element_t * map_string_to_group(pairing_t p, char *message, int type)
{
    element_t *res;
    res = malloc(sizeof(element_t));
    
    switch(type)
    {
        case Zr:element_init_Zr(*res, p);break;
        case G1:element_init_G1(*res, p);break;
        case G2:element_init_G2(*res, p);break;
        case GT:element_init_GT(*res, p);break;
        default:
            free(res);
            return NULL;
    }

    // element_random(*res);
    char *hash_code;
    hash(&hash_code, message);
    element_from_hash(*res, hash_code, strlen(hash_code));
    free(hash_code);

    return res;
}

element_t * calcoef(pairing_t p, GArray * set, int index)
{
    element_t * element_sets;
    element_sets = malloc(sizeof(element_t) * (set -> len));
    for (int i = 0; i < set -> len; i ++)
    {
        element_init_Zr(*(element_sets + i), p);
        element_set_si(*(element_sets + i), g_array_index(set, int, i));
    }

    element_t element_index;
    element_init_Zr(element_index, p);
    element_set_si(element_index, index);

    element_t * res;
    res = malloc(sizeof(element_t));
    element_init_Zr(*res, p);
    element_set1(*res);

    for (int i = 0; i < set -> len; i ++)
    {
        if (g_array_index(set, int, i) == index)continue;
        element_t member;
        element_t denominator;
        element_t zero;

        element_init_Zr(zero, p);
        element_init_Zr(member, p);
        element_init_Zr(denominator, p);

        element_set0(zero);
        element_sub(member, zero, *(element_sets + i));

        element_sub(denominator, element_index, *(element_sets + i));

        element_mul(*res, res, member);
        element_invert(denominator, denominator);
        element_mul(*res, *res, denominator);

        element_clear(member);
        element_clear(denominator);
        element_clear(zero);

    }

    element_clear(element_index);
    for (int i = 0; i < set -> len; i ++)
        element_clear(*(element_sets + i));

    free(element_sets);

    return res;
}