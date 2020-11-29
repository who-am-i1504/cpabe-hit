#include "pairing_util.h"

void access_tree_node_secret_sharing(pairing_t p, element_t root_secret, t_node *node, GHashTable *map)
{
    if (node -> attr == NULL)
    {
        for (int i = 0; i < node -> children -> len; i ++)
        {
            element_t* e;
            element_t x;
            element_init_Zr(x, p);
            element_set_si(x, i + 1);
            e = evalute(p, x, 
                node -> k, generate_coef(p, node->k - 1, root_secret));
            access_tree_node_secret_sharing(p,
                *e, (t_node *)(node -> children -> pdata[i]), map);
            element_clear(x);
            element_clear(*e);
            free(e);
        }
    }
    else
    {
        element_t *e;
        e = malloc(sizeof(element_t));

        element_init_GT(*e, p);
        element_set(*e, root_secret);
        g_hash_table_add(node -> attr, e);
    }
    
}

GHashTable* secret_sharing(pairing_t p, element_t secret, t_node * root)
{
    GHashTable *shared_elements_map;
    shared_elements_map = g_hash_table_new(g_str_hash, g_str_equal);
    // for (int i = 0; i < )
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

    element_random(*res);

    return res;
}