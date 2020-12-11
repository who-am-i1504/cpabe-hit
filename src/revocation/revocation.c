#include "revocation/core.h"

#ifndef TYPE_A_PARAMS
#define TYPE_A_PARAMS "type a\n" \
    "q 81869981414486565817042987620009425916711137248094272342132238763687306328559\n" \
    "r 604462909877683331530751\n" \
    "h 135442522736512392410054892783912140655846630328108560\n" \
    "exp1 46\n" \
    "exp2 79\n" \
    "sign0 -1\n" \
    "sign1 1\n"

#endif

#ifndef FGADR_ATTRIBUTE_NOTHING_KEY
#define FGADR_ATTRIBUTE_NOTHING_KEY "attribute-revocation-list-no-user"
#endif

void fgadr_setup(fgadr_pub_t ** pub, fgadr_msk_t ** msk)
{
    (*pub) = malloc(sizeof(fgadr_pub_t));
    (*msk) = malloc(sizeof(fgadr_msk_t));
    (*pub) -> pairing_desc = strdup(TYPE_A_PARAMS);
    pairing_init_set_buf((*pub) -> p, 
        (*pub) -> pairing_desc, strlen((*pub) -> pairing_desc));

    element_init_GT((*pub) -> egg_beta, (*pub) -> p);
    element_init_G1((*pub) -> g, (*pub) -> p);
    element_init_G1((*pub) -> v_beta, (*pub) -> p);
    element_init_G1((*msk) -> h, (*pub) -> p);
    element_init_G1((*msk) -> v, (*pub) -> p);
    element_init_Zr((*msk) -> alpha, (*pub) -> p);
    element_init_Zr((*msk) -> beta, (*pub) -> p);

    element_random((*pub) -> g);
    element_random((*msk) -> h);
    element_random((*msk) -> v);
    element_random((*msk) -> alpha);
    element_random((*msk) -> beta);

    pairing_apply((*pub) -> egg_beta, (*pub) -> g, (*pub) -> g, (*pub) -> p);
    element_pow_zn((*pub) -> egg_beta, (*pub) -> egg_beta, (*msk) -> beta);

    element_pow_zn((*pub) -> v_beta, (*msk) -> v, (*msk) -> beta);

    (*pub) -> attr_pks = g_hash_table_new(g_str_hash, g_str_equal);
    (*msk) -> id = g_hash_table_new(g_str_hash, g_str_equal);
    (*msk) -> attr_sks = g_hash_table_new(g_str_hash, g_str_equal);
}

void fgadr_attr_pair(fgadr_pub_t *pub, 
                     fgadr_msk_t * msk, 
                     char *attribute)
{
    if (attribute == NULL 
        || !strlen(attribute) 
        || g_hash_table_contains(pub -> attr_pks, attribute))
        return;
    element_t * t;
    element_t *gt;
    char * attr;
    attr = strdup(attribute);
    t = malloc(sizeof(element_t));
    gt = malloc(sizeof(element_t));

    element_init_G1(*gt, pub -> p);
    element_init_Zr(*t, pub -> p);

    element_random(*t);
    element_pow_zn(*gt, pub -> g, *t);


    g_hash_table_insert(pub -> attr_pks, attr, gt);
    g_hash_table_insert(msk -> attr_sks, attr, t);

}


fgadr_auth_sk *
fgadr_keygen_auth(fgadr_pub_t * pub,
                  element_t h,
                  element_t t,
                  element_t alpha,
                  element_t lambda,
                  char *attribute)
{
    fgadr_auth_sk * auth_sk;
    element_t alpha_div_t;
    element_t h_alpha_t;

    auth_sk = malloc(sizeof(fgadr_auth_sk));

    auth_sk -> attribute = strdup(attribute);
    element_init_G1(auth_sk -> s, pub -> p);
    element_init_G1(auth_sk -> w, pub -> p);
    element_init_Zr(alpha_div_t, pub -> p);
    element_init_G1(h_alpha_t, pub -> p);

    element_invert(alpha_div_t, t);
    element_mul(alpha_div_t, alpha_div_t, alpha);
    element_pow_zn(h_alpha_t, h, alpha_div_t);
    element_pow_zn(auth_sk -> s, pub -> g, alpha_div_t);
    element_pow_zn(auth_sk -> s, auth_sk -> s, lambda);

    element_mul(alpha_div_t, alpha, lambda);
    element_pow_zn(auth_sk -> w, pub -> g, alpha_div_t);

    element_clear(h_alpha_t);
    element_clear(alpha_div_t);

    return auth_sk;
}


void 
fgadr_keygen_user(fgadr_user_sk * prv,
                  fgadr_pub_t * pub,
                  fgadr_msk_t * msk,
                  fgadr_auth_sk *auth_sk)
{
    element_t beta_inv;
    
    element_init_Zr(beta_inv, pub -> p);
    element_neg(beta_inv, msk -> beta);

    element_pow_zn(prv -> w_star, prv -> w_star, beta_inv);

    element_clear(beta_inv);
}

void 
fgadr_revocation(fgadr_auth_t* auth,
                 fgadr_msk_t* msk,
                 fgadr_pub_t * pub, 
                 element_t lambda, 
                 char* id, 
                 char *attribute)
{
    if (auth -> revo_list == NULL)auth->revo_list = g_hash_table_new(g_str_hash, g_str_equal);
    fgadr_rev_item * item;
    fgadr_attr_pk_sk * attr;
    element_t h_alpha;
    element_t v_beta_t;
    element_t g_alpha_lambda;
    GHashTable * attr_rev_list;
    if (!g_hash_table_contains(auth -> revo_list, attribute))
    {
        auth->revo_list = g_hash_table_new(g_str_hash, g_str_equal);
    }

    if (!g_hash_table_contains(auth-> revo_list, attribute))
    {
        g_hash_table_insert(auth->revo_list, 
            attribute, g_hash_table_new(g_str_hash, g_str_equal));
    }
    attr_rev_list = g_hash_table_lookup(auth->revo_list, attribute);

    if (g_hash_table_contains(attr_rev_list, id))
    {
        raise_error("user %s has been revocated.", id);
        return ;
    }

    if (!g_hash_table_size(attr_rev_list))
    {
        element_t * base;
        element_t mid;
        element_init_G1(mid, pub -> p);
        base = malloc(sizeof(element_t));
        element_init_G1(*base, pub -> p);

        element_pow_zn(mid, msk -> h, msk ->alpha);
        element_pow_zn(*base, pub -> v_beta, 
            *(element_t *)g_hash_table_lookup(msk -> attr_sks, attribute));

        g_hash_table_insert(attr_rev_list, FGADR_ATTRIBUTE_NOTHING_KEY, base);
        element_clear(mid);

    }

    if (attribute != NULL)
    {

        item = malloc(sizeof(fgadr_rev_item));
        attr = g_hash_table_lookup(auth -> attr_pair, attribute);
    
        if (lambda ->data == NULL)
        {
            element_init_Zr(lambda, pub -> p);
            element_set0(lambda);
        }
        element_init_same_as(item -> lambda, lambda);
        element_init_G1(item -> list_rev, pub -> p);
        element_init_G1(h_alpha, pub -> p);
        element_init_G1(v_beta_t, pub -> p);
        element_init_G1(g_alpha_lambda, pub -> p);
    
        element_set(item -> lambda, lambda);
    
        element_pow_zn(h_alpha, auth -> h, auth -> alpha);
    
        element_pow_zn(v_beta_t, pub -> v_beta, attr -> t);
    
        element_pow_zn(g_alpha_lambda, pub -> g, auth -> alpha);
        element_pow_zn(g_alpha_lambda, g_alpha_lambda, lambda);
    
        element_mul(item -> list_rev, g_alpha_lambda, v_beta_t);
        element_mul(item -> list_rev, item->list_rev, h_alpha);
    
        element_clear(lambda);
        element_clear(h_alpha);
        element_clear(v_beta_t);
        element_clear(g_alpha_lambda);
        g_hash_table_insert(auth -> revo_list, attribute, item);
    }
    
    
}

bswabe_polynomial_t* 
random_sum_to_q(int number, element_t sum)
{
    element_t zero;
    bswabe_polynomial_t *res;
    res = malloc(sizeof(bswabe_polynomial_t));
    res->deg = number;
    res->coef = malloc(sizeof(element_t) * number);
    element_init_same_as(zero, sum);
    element_set0(zero);
    for (int i = 0; i < number - 1; i ++)
    {
        element_init_same_as(res -> coef[i], sum);
        element_random(res -> coef[i]);
        element_add(zero, zero, res -> coef[i]);
    }
    element_init_same_as(res -> coef[number - 1], sum);
    element_sub(res -> coef[number - 1], sum, zero);
    element_clear(zero);
    return res;
}

void 
gfadr_policy_enc(bswabe_policy_t *p, 
                 fgadr_pub_t * pub, 
                 fgadr_cph_mid_t * params)
{
    GHashTable * list;
    int size;
    bswabe_polynomial_t *u_list;
    element_t *L;
    GList * ls;
    bswabe_polynomial_t* c2;
    bswabe_polynomial_t* c3;
    element_t *T;
    fgadr_cs_other *cs;

    T = g_hash_table_lookup(pub -> attr_pks, p -> attr);

    cs = malloc(sizeof(fgadr_cs_other));
    c2 = malloc(sizeof(bswabe_polynomial_t));
    c3 = malloc(sizeof(bswabe_polynomial_t));

    list = g_hash_table_lookup(params -> revolist, p -> attr);
    if (g_hash_table_size(list) == 1)
    {
        c2 ->deg = 1;
        c3 ->deg = 1;
        c2 -> coef = malloc(sizeof(element_t));
        c3 -> coef = malloc(sizeof(element_t));
        element_init_G1(*(c2 -> coef), pub -> p);
        element_init_G1(*(c3 -> coef), pub -> p);

        element_pow_zn(*(c2 -> coef), *T, p -> q -> coef[0]);
        element_pow_zn(*(c2 -> coef), *(c2 -> coef), params -> r);
        element_pow_zn(*(c3 -> coef), 
            *(element_t*)(g_hash_table_lookup
            (list, FGADR_ATTRIBUTE_NOTHING_KEY)), p -> q -> coef[0]);
        element_pow_zn(*(c3 -> coef), *(c3 -> coef), params -> r);
    }
    else
    {
        size = g_hash_table_size(list);
        c2 -> deg = size;
        c2 -> coef = malloc(sizeof(element_t) * size);
        c3 -> deg = size;
        c3 -> coef = malloc(sizeof(element_t) * size);
        u_list = random_sum_to_q(size, p -> q -> coef[0]);
        ls = g_hash_table_get_keys(list);
        int i = 0;
        for (GList * lh = ls; lh != NULL; lh = lh -> next)
        {
            if (!strcpm(FGADR_ATTRIBUTE_NOTHING_KEY, lh -> data))continue;
            fgadr_rev_item* item;
            item = g_hash_table_lookup(list, lh -> data);
            element_init_G1(c2 -> coef[i], pub -> p);
            element_init_G1(c3 -> coef[i], pub -> p);

            element_pow_zn(c2 ->coef[i], *T, u_list->coef[i]);
            element_pow_zn(c2 -> coef[i], c2->coef[i], params -> r);

            element_pow_zn(c3 ->coef[i], item -> list_rev, u_list->coef[i]);
            element_pow_zn(c3 -> coef[i], c3->coef[i], params -> r);
            
            i ++;
        }

        g_list_free(ls);
        free_polynomial(u_list);
    }

    cs -> c2 = c2;
    cs -> c3 = c3;

    p -> cph = cs;
}

fgadr_cph_t *
gfadr_encrypt(fgadr_pub_t * pub, 
              GHashTable *revo_list, 
              bswabe_policy_t *policy,
              element_t m)
{
    fgadr_cph_t *cph;
    element_t s,r;
    fgadr_cph_mid_t* params;

    params = malloc(sizeof(fgadr_cph_mid_t));
    cph = malloc(sizeof(fgadr_cph_t));

    params -> revolist = revo_list;
    params -> cph = cph;
    element_init_Zr(s, pub -> p);
    element_init_Zr(params -> r, pub -> p);
    element_init_GT(cph -> c0, pub -> p);
    element_init_G1(cph -> c1, pub -> p);


    element_random(s);
    element_set1(r);

    element_pow_zn(cph -> c0, pub -> egg_beta, s);
    element_pow_zn(cph -> c0, cph -> c0, params -> r);

    element_mul(cph -> c0, m, cph -> c0);

    element_pow_zn(cph -> c1, pub -> g, s);
    element_pow_zn(cph -> c1, cph -> c1, params -> r);


    fill_policy(policy, pub, s, params, gfadr_policy_enc);


}


void
fgadr_reencrypt_enc(bswabe_policy_t* p,
                    fgadr_pub_t* pub,
                    GHashTable* revo_list)
{
    if (p->attr == NULL)
    {
        for (int i = 0; i < p -> children ->len; i ++)
        {
            fgadr_reencrypt_enc(p -> children -> pdata[i], pub, revo_list);
        }
    }
    else
    {
        fgadr_cs_other *cs;
        GHashTable * list;
        list = g_hash_table_lookup(revo_list, p -> attr);
        cs = p -> cph;

        if (cs -> c2 -> deg == 1 && g_hash_table_size(list) != 1)
        {
            // R 由空转非空
            element_t u;
            element_t * c2;
            element_t * c3;
            element_t mid;
            c2 = cs -> c2 -> coef;
            c3 = cs -> c3 -> coef;
            
            cs -> c2 -> deg = g_hash_table_size(list);
            cs -> c3 -> deg = g_hash_table_size(list);

            cs -> c2 = realloc(cs -> c2, g_hash_table_size(list));
            cs -> c3 = realloc(cs -> c3, g_hash_table_size(list));

            element_init_same_as(cs -> c2 -> coef[1], *c2);
            element_init_same_as(cs -> c3 -> coef[1], *c3);
            element_init_G1(mid, pub -> p);
            element_init_Zr(u, pub -> p);

            element_random(u);

            element_pow_zn(cs -> c2 -> coef[1], 
                *(element_t *)(g_hash_table_lookup(pub -> attr_pks, p -> attr)), u);
            element_pow_zn(cs -> c3 -> coef[1], 
                *(element_t *)(g_hash_table_lookup(list, FGADR_ATTRIBUTE_NOTHING_KEY)), u);
            
            element_invert(mid, cs -> c2 -> coef[1]);
            element_mul(*c2, *c2, mid);
            element_invert(mid, cs -> c3 -> coef[1]);
            element_mul(*c3, *c3, mid);
        }
        else if (cs -> c2 -> deg != 1 && cs -> c2 -> deg + 1 != g_hash_table_size(list))
        {
            // R 由非空变化
            element_t u;
            int deg;

            deg = cs -> c2 -> deg;
            cs -> c2 -> deg *= 2;
            cs -> c3 -> deg *= 2;

            

            cs -> c2 -> coef = realloc(cs -> c2 -> coef, sizeof(element_t) * (deg * 2));
            cs -> c3 -> coef = realloc(cs -> c3 -> coef, sizeof(element_t) * (deg * 2));

            element_init_Zr(u, pub -> p);

            element_random(u);
        }
        
    }
}

void 
fgadr_reencrypt(fgadr_cph_t * cph,
                fgadr_pub_t * pub,
                GHashTable *revo_list)
{
    fgadr_reencrypt_enc(cph -> policy, pub, revo_list);
}