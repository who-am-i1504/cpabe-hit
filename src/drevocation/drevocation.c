/*
 * @Author: Qinggang Wu
 * @Date: 2020-12-17 16:58:50
 * @LastEditTime: 2020-12-20 21:57:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/drevocation/drevocation.c
 */
#include "drevocation/core.h"

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

void 
drevo_setup(drevo_pub_t** pub, 
            drevo_msk_t** msk)
{
    element_t beta_invert;
    (*pub) = malloc(sizeof(drevo_pub_t));
    (*msk) = malloc(sizeof(drevo_msk_t));

    (*pub)->pairing_desc = strdup(TYPE_A_PARAMS);

    pairing_init_set_buf((*pub)->p, 
        (*pub)->pairing_desc, strlen((*pub)->pairing_desc));
    
    element_init_G1((*pub)->g, (*pub)->p);
    element_init_G1((*pub)->g_a, (*pub)->p);
    element_init_G1((*pub)->g_beta, (*pub)->p);
    element_init_G1((*pub)->g_beta_invert, (*pub)->p);
    element_init_GT((*pub)->egg_alpha, (*pub)->p);
    element_init_Zr((*msk)->a, (*pub)->p);
    element_init_Zr((*msk)->alpha, (*pub)->p);
    element_init_Zr((*msk)->beta, (*pub)->p);
    element_init_Zr((*msk)->gamma, (*pub)->p);
    element_init_Zr(beta_invert, (*pub)->p);

    element_random((*pub)->g);
    element_random((*msk)->a);
    element_random((*msk)->alpha);
    element_random((*msk)->beta);
    element_random((*msk)->gamma);

    element_invert(beta_invert, (*msk)->beta);

    element_pow_zn((*pub)->g_a, (*pub)->g, (*msk)->a);
    element_pow_zn((*pub)->g_beta, (*pub)->g, (*msk)->beta);
    element_pow_zn((*pub)->g_beta_invert, (*pub)->g, beta_invert);

    element_pairing((*pub)->egg_alpha, (*pub)->g, (*pub)->g);
    element_pow_zn((*pub)->egg_alpha, (*pub)->egg_alpha, (*msk)->alpha);
    
    element_clear(beta_invert);

    (*msk)->Vx = g_hash_table_new(g_str_hash, g_str_equal);
}

drevo_pkx_t*
drevo_pk_setup(drevo_pub_t* pub, 
               drevo_msk_t* msk,
               char *attribute)
{
    element_t *vx;
    drevo_pkx_t* pkx;
    pkx = malloc(sizeof(drevo_pkx_t));
    pkx->attribute = strdup(attribute);

    vx = malloc(sizeof(element_t));

    element_init_G1(pkx->pk1, pub->p);
    element_init_G1(pkx->pk2, pub->p);
    element_init_Zr(*vx, pub->p);

    element_from_string(pkx->pk1, pkx->attribute);
    element_random(*vx);

    element_pow_zn(pkx->pk1, pkx->pk1, *vx);

    element_pow_zn(pkx->pk2, pkx->pk1, msk->gamma);

    g_hash_table_insert(msk->Vx, attribute, vx);
    return pkx;
}

drevo_prv_t*
drevo_keygen(drevo_pub_t* pub,
             drevo_msk_t* msk,
             GHashTable* pkxs,
             char **attributes)
{
    drevo_prv_t* prv;
    element_t t, tm;

    prv = malloc(sizeof(drevo_prv_t));

    prv->attrs = g_ptr_array_new();
    prv->Kx = g_hash_table_new(g_str_hash, g_str_equal);

    element_init_Zr(t, pub->p);
    element_init_Zr(tm, pub->p);
    element_init_G1(prv->K, pub->p);
    element_init_G1(prv->L, pub->p);

    element_random(t);
    element_mul(tm, t, msk->a);

    element_pow_zn(prv->L, pub->g, t);

    element_pow2_zn(prv->K, 
        pub->g_beta_invert, msk->alpha, pub->g_beta_invert, tm);
    
    element_mul(tm, t, msk -> beta);
    
    while(*attributes)
    {
        char *attr;
        element_t *kx;
        drevo_pkx_t* pkx;

        attr = strdup(*(attributes ++));
        kx = malloc(sizeof(element_t));
        pkx = g_hash_table_lookup(pkxs, attr);

        element_init_G1(*kx, pub->p);
        element_pow2_zn(*kx, pub->g_beta, tm, pkx->pk1, tm);

        g_hash_table_insert(prv->Kx, attr, kx);

        g_ptr_array_add(prv->attrs, attr);
    }

    element_clear(t);
    element_clear(tm);
    return prv;
    
}

void 
drevo_enc_detail(bswabe_policy_t* p,
                 drevo_pub_t* pub,
                 GHashTable* pkxs)
{
    drevo_cph_item_t* item;
    drevo_pkx_t* pkx;
    element_t r, r_neg;
    
    pkx = g_hash_table_lookup(pkxs, p->attr);
    item = malloc(sizeof(drevo_cph_item_t));
    p->cph = item;

    element_init_Zr(r, pub->p);
    element_init_Zr(r_neg, pub->p);

    element_init_G1(item->C1, pub->p);
    element_init_G1(item->D1, pub->p);
    element_init_G1(item->D2, pub->p);

    element_random(r);
    element_neg(r_neg, r);
    
    element_pow3_zn(item->C1, pub->g_a, 
        p->q->coef[0], pub->g_beta, r_neg, pkx->pk1, r_neg);
    
    element_pow_zn(item->D1, pkx->pk2, r);

    element_pow_zn(item->D2, pub->g_beta_invert, r);

    element_clear(r);
    element_clear(r_neg);
}

drevo_cph_t*
drevo_encrypt(drevo_pub_t* pub,
              element_t m,
              GHashTable* pkxs,
              char *policy)
{
    drevo_cph_t* cph;
    element_t s;

    cph = malloc(sizeof(drevo_cph_t));

    element_init_GT(m, pub->p);
    element_init_GT(cph->C, pub->p);
    element_init_G1(cph->C1, pub->p);

    element_init_Zr(s, pub->p);

    element_random(m);
    element_random(s);

    element_pow_zn(cph->C, pub->egg_alpha, s);
    element_mul(cph->C, cph->C, m);

    element_pow_zn(cph->C1, pub->g_beta, s);

    cph->p = parse_policy_postfix(policy);
    cph->policy = strdup(policy);

    fill_policy(cph->p, pub, s, pkxs, drevo_enc_detail);

    element_clear(s);
    return cph;
}

void
drevo_dec_detail(element_t r,
                 element_t exp,
                 bswabe_policy_t* p,
                 drevo_prv_t* prv,
                 drevo_pub_t* pub)
{
    element_t s, t;
    drevo_cph_item_t* item;
    element_t *kx;

    item = p->cph;
    kx = g_hash_table_lookup(prv->Kx, p->attr);

    element_init_GT(s, pub->p);
    element_init_GT(t, pub->p);

    element_pairing(s, item->C1, prv->L);
    element_pairing(t, item->D2, *kx);

    element_mul(s, s, t);

    element_pow_zn(s, s, exp);

    element_mul(r, r, s);

    element_clear(s);
    element_clear(t);
}


int
drevo_decrypt(drevo_pub_t* pub,
              drevo_cph_t* cph,
              drevo_prv_t* prv,
              element_t m)
{
    int res;
    element_t dp, di;
    res = 0;

    check_sat(cph->p, prv->attrs);

    pick_sat_min_leaves(cph->p);

    if (!cph->p->satisfiable)
    {
        raise_error("your attributes can't satify the access policy.\n");
        res = 1;
        goto drevo_destory;
    }

    element_init_GT(dp, pub->p); 
    element_init_GT(m, pub->p); 
    element_init_GT(di, pub->p); 

    dec_flatten(dp, cph->p, prv, pub, pub->p, drevo_dec_detail);
    
    element_pairing(di, cph->C1, prv->K);
    
    element_invert(dp, dp);

    element_mul(dp, dp, di);

    element_invert(dp, dp);

    element_mul(m, dp, cph->C);

    element_clear(dp);
    element_clear(di);

drevo_destory:

    return res;
}


void
drevo_ukeygen(drevo_uk_t** uk, 
              drevo_msk_t* msk,
              element_t vx)
{
    element_t vx_new, vxm;

    (*uk) = malloc(sizeof(drevo_uk_t));

    element_init_same_as((*uk)->uk1, vx);
    element_init_same_as((*uk)->uk2, vx);
    element_init_same_as(vx_new, vx);
    element_init_same_as(vxm, vx);

    element_random(vx_new);

    element_div((*uk)->uk1, vx_new, vx);
    
    element_sub(vxm, vx, vx_new);
    
    element_div((*uk)->uk2, vxm, vx);
    element_div((*uk)->uk2, (*uk)->uk2, msk->gamma);

    element_set(vx, vx_new);

    element_clear(vx_new);
    element_clear(vxm);

}

void 
drevo_update_pkx(drevo_pkx_t* pkx, 
                 element_t vx,
                 drevo_uk_t* uk)
{
    element_pow_zn(pkx->pk1, pkx->pk1, uk->uk1);
    element_pow_zn(pkx->pk2, pkx->pk2, uk->uk1);
}

void
drevo_update_prv_key(drevo_prv_t* prv,
                     drevo_msk_t* msk,
                     drevo_uk_t* uk,
                     char *attribute)
{
    element_t *kx;
    element_t L_beta_2;
    
    element_init_same_as(L_beta_2, prv->L);

    element_pow_zn(L_beta_2, prv->L, msk->beta);
    element_pow_zn(L_beta_2, L_beta_2, msk->beta);

    kx = g_hash_table_lookup(prv->Kx, attribute);

    element_div(*kx, *kx, L_beta_2);
    element_pow_zn(*kx, *kx, uk->uk1);

    element_mul(*kx, *kx, L_beta_2);

    element_clear(L_beta_2);
}

void
drevo_cph_update(drevo_cph_t* cph,
                 drevo_uk_t* uk,
                 drevo_pub_t* pub,
                 char * attribute)
{
    GQueue* queue;
    element_t mid;
    drevo_cph_item_t* item;
    queue = g_queue_new();

    g_queue_push_tail(queue, cph->p);
    element_init_G1(mid, pub->p);
    while(!g_queue_is_empty(queue))
    {
        bswabe_policy_t* root;
        root = g_queue_pop_head(queue);
        for (int i = 0; i < root->children->len; i++)
            g_queue_push_tail(queue, g_ptr_array_index(root->children, i));
        if (!root->children->len && !strcmp(attribute, root->attr))
        {
            item = root->cph;
            element_pow_zn(mid, item->D1, uk->uk2);
            element_mul(item->C1, item->C1, mid);
            element_pow_zn(item->D1, item->D1, uk->uk1);
        }
    }

    element_clear(mid);
    g_queue_clear(queue);
}