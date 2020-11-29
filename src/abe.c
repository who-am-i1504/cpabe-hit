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

    element_mul(
        (*pp) -> egg_alpha,(*pp) -> g, (*pp) -> g);

    element_pow_zn((*pp) -> egg_alpha,
        (*pp) -> egg_alpha, (*msk) -> alpha);
}

int keygen(prv_sk ** prv, msk_p *msk, pub_p * pp, char ** attributes)
{
    *prv = malloc(sizeof(prv_sk));

    element_t r;
    element_t k3temp;
    
    element_init_Zr(r,            pp -> p);
    element_init_GT((*prv) -> k0, pp -> p);
    element_init_GT((*prv) -> k1, pp -> p);
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

    while(*attributes)
    {
        char *attr = strdup((*attributes) ++);

        element_t e_attr;
        element_t r_i;
        element_t k2i;
        element_t k3i;

        element_init_G1(k2i, pp -> p);
        element_init_G1(k3i, pp -> p);
        element_init_Zr(r_i, pp -> p);
        element_init_G1(e_attr, pp -> p);

        element_random(r_i);
        element_from_hash(e_attr, attr, strlen(attr));

        element_pow_zn(k2i, pp -> g, r_i);
        
        element_pow_zn(k3i, pp -> u, e_attr);
        element_mul(k3i, k3i, pp -> h);
        element_pow_zn(k3i, k3i, r_i);
        element_mul(k3i, k3i, k3temp);

        g_hash_table_insert((*prv) -> k2s, attributes, k2i);
        g_hash_table_insert((*prv) -> k3s, attributes, k3i);

        element_clear(e_attr);
        element_clear(r_i);
    }

    element_clear(k3temp);
    element_clear(r);

    return 0;
}


int encrypt(csp_s ** cs, pub_p *pp, t_node* node, GPtrArray * matrix, char ** thos, element_t m)
{

    *cs = malloc(sizeof(csp_s));
    element_t s;
    element_t session_key;

    element_init_Zr(s, pp -> p);
    element_init_GT(session_key, pp -> p);
    element_init_GT((*cs) -> header.c0, pp -> p);

    element_random(s);
    element_pow_zn(session_key, pp -> egg_alpha, s);
    element_pow_zn((*cs) -> header.c0, pp -> g, s);

    GHashTable * map;
    map = secret_sharing(pp -> p, s, node);
    if (map == NULL)
    {
        element_clear(s);
        element_clear(session_key);
        free(*cs);
        return 1;
    }
    GHashTable * c1s;
    GHashTable * c2s;
    GHashTable * c3s;

    (*cs) -> header.c1s = g_hash_table_new(g_str_hash, g_str_equal);
    (*cs) -> header.c2s = g_hash_table_new(g_str_hash, g_str_equal);
    (*cs) -> header.c3s = g_hash_table_new(g_str_hash, g_str_equal);

    c1s = (*cs) -> header.c1s;
    c2s = (*cs) -> header.c2s;
    c3s = (*cs) -> header.c3s;


    GList * list = g_hash_table_get_keys(map);

    for (GList *p = list; p != NULL; p = p -> next)
    {
        element_t * element_rho = map_string_to_group(pp -> p, p -> data, Zr);
        if (element_rho == NULL)
        {
            g_list_free(list);
            g_hash_table_destroy(map);
            element_clear(s);
            element_clear(session_key);
            free(*cs);
            return 1;
        }
        element_t ti;
        element_init_Zr(ti, pp -> p);
        element_random(ti);

        element_t before, after;
        element_init_GT(before, pp -> p);
        element_init_GT(after, pp -> p);
        element_pow_zn(before, *((element_t *)g_hash_table_lookup(map, p -> data)));

        element_pow_zn(after, pp -> v, ti);
        element_mul(before, before, after);

        element_t * res;
        res = malloc(sizeof(element_t));

        element_init_GT(*res, pp -> p);
        element_set(*res, before);

        g_hash_table_insert(c1s, p -> data, res);


        element_pow_zn(before, pp -> u, element_rho);
        element_neg(after, ti);
        element_pow_zn(after, pp -> h, after);

        res = malloc(sizeof(element_t));
        element_init_GT(*res, pp -> p);
        element_mul(*res, before, after);
        g_hash_table_insert(c2s, p -> data, res);

        res = malloc(sizeof(element_t));
        element_init_GT(*res, pp -> p);
        element_pow_zn(*res, pp -> g, ti);

        g_hash_table_insert(c3s, p -> data, res);

        element_clear(before);
        element_clear(after);
        element_clear(ti);
        element_clear(*element_rho);
        free(element_rho);
    }
    g_list_free(list);

    element_init_GT((*cs) -> c, pp -> p);

    element_mul((*cs) -> c, session_key, m);

    g_hash_table_destroy(map);
    element_clear(s);
    element_clear(session_key);

}


int decryption(pub_p *pp, prv_sk *prv, char *policy, csp_s *cs)
{
    
}