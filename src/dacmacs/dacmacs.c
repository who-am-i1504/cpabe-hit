/*
 * @Author: your name
 * @Date: 2020-12-22 15:33:44
 * @LastEditTime: 2020-12-23 20:05:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/dacmacs/dacmacs.c
 */

#include "dacmacs/core.h"

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
dacamacs_casetup(dacmacs_capub_t** pub,
                 dacmacs_camsk_t** msk)
{
    (*pub) = malloc(sizeof(dacmacs_capub_t));
    (*msk) = malloc(sizeof(dacmacs_camsk_t));

    (*pub)->pairing_desc = strdup(TYPE_A_PARAMS);

    pairing_init_set_buf((*pub)->p, 
        (*pub)->pairing_desc, strlen((*pub)->pairing_desc));
    
    element_init_G1((*pub)->g, (*pub)->p);
    element_init_G1((*pub)->ga, (*pub)->p);
    element_init_Zr((*msk)->a, (*pub)->p);

    element_random((*msk)->a);
    element_random((*pub)->g);

    element_pow_zn((*pub)->ga, (*pub)->g, (*msk)->a);

}

// element_t u;

dacmacs_user_t*
dacmacs_user_register(dacmacs_capub_t* pub,
                      char *uid,
                      GPtrArray* attributes)
{
    dacmacs_user_t* prv;
    dacmacs_user_cert_t* cert;

    prv = malloc(sizeof(dacmacs_user_t));
    cert = malloc(sizeof(dacmacs_user_cert_t));

    prv->cert = cert;

    cert->uid = strdup(uid);

    prv->sks = NULL;

    prv->attributes = g_ptr_array_new();

    element_init_Zr(cert->u, pub->p);
    element_init_Zr(prv->gsk, pub->p);
    element_init_G1(prv->gpk, pub->p);
    element_init_G1(cert->g_z_invert, pub->p);

    element_random(cert->u);
    element_random(prv->gsk);

    // if (!strcmp(uid, "user1"))
    // {
    //     element_init_Zr(u, pub->p);
    //     element_set(u, cert->u);
    // }

    element_pow_zn(prv->gpk, pub->g, cert->u);

    element_pow_zn(cert->g_z_invert, pub->g, prv->gsk);
    element_invert(prv->gsk, prv->gsk);

    for (int i = 0; i < attributes->len; i ++)
        g_ptr_array_add(prv->attributes, 
            strdup(g_ptr_array_index(attributes, i)));

    return prv;
}

dacmacs_auth_t*
dacmacs_aasetup(dacmacs_capub_t* pub,
                GPtrArray* attributes,
                char * aid)
{
    dacmacs_auth_t* auth;
    dacmacs_auth_msk_t* pmsk;
    dacmacs_auth_pub_t* pk;
    
    auth = malloc(sizeof(dacmacs_auth_t));

    auth->attributes = g_ptr_array_new();
    pmsk = malloc(sizeof(dacmacs_auth_msk_t));
    pk = malloc(sizeof(dacmacs_auth_pub_t));
    auth->auth_msk = pmsk;
    auth->auth_pk = pk;
    auth->attr_pks = g_hash_table_new(g_str_hash, g_str_equal);
    auth->attr_msks = g_hash_table_new(g_str_hash, g_str_equal);
    auth->aid = strdup(aid);

    element_init_GT(pk->egg_alpha, pub->p);
    element_init_G1(pk->g_beta_gamma, pub->p);
    element_init_G1(pk->g_beta_invert, pub->p);
    element_init_Zr(pmsk->alpha, pub->p);
    element_init_Zr(pmsk->beta, pub->p);
    element_init_Zr(pmsk->gamma, pub->p);

    element_random(pmsk->alpha);
    element_random(pmsk->beta);
    element_random(pmsk->gamma);

    element_pairing(pk->egg_alpha, pub->g, pub->g);
    element_pow_zn(pk->egg_alpha, pk->egg_alpha, pmsk->alpha);
    element_pow_zn(pk->g_beta_gamma, pub->g, pmsk->beta);
    element_pow_zn(pk->g_beta_gamma, pk->g_beta_gamma, pmsk->gamma);
    element_pow_zn(pk->g_beta_invert, pub->g, pmsk->beta);
    element_invert(pmsk->beta, pmsk->beta);

    for (int i = 0; i < attributes->len; i++)
    {
        char *attr;
        dacmacs_attr_pk_t *pkx;
        dacmacs_attr_msk_t *vx;
        element_t mid;
        
        attr = g_ptr_array_index(attributes, i);

        if (g_hash_table_contains(auth->attr_pks, attr))continue;

        pkx = malloc(sizeof(dacmacs_attr_pk_t));
        vx = malloc(sizeof(dacmacs_attr_msk_t));

        attr = strdup(attr);

        pkx->version = 0;
        vx->version = 0;

        element_init_G1(pkx->pk, pub->p);
        element_init_G1(mid, pub->p);
        
        element_init_Zr(vx->vx, pub->p);
        element_init_Zr(vx->auk, pub->p);
        element_init_Zr(vx->kuk, pub->p);
        element_init_Zr(vx->cuk, pub->p);

        element_set1(vx->auk);
        element_set1(vx->kuk);
        element_set1(vx->cuk);

        element_random(vx->vx);

        element_from_string(pkx->pk, attr);

        element_pow_zn(mid, pub->g, vx->vx);

        element_mul(pkx->pk, pkx->pk, mid);

        element_pow_zn(pkx->pk, pkx->pk, pmsk->gamma);

        g_hash_table_insert(auth->attr_pks, attr, pkx);
        g_hash_table_insert(auth->attr_msks, attr, vx);

        g_ptr_array_add(auth->attributes, attr);

        element_clear(mid);
    }

    return auth;
}

dacmacs_prv_item_t *
dacmacs_keygen(dacmacs_auth_t* auth,
               dacmacs_capub_t* pub,
               dacmacs_user_cert_t* cert,
               GPtrArray *attributes)
{
    dacmacs_prv_item_t* prv;
    element_t t;

    prv = malloc(sizeof(dacmacs_prv_item_t));

    prv->attributes = g_ptr_array_new();
    prv->kx = g_hash_table_new(g_str_hash, g_str_equal);
    prv->aid = strdup(auth->aid);
    
    element_init_G1(prv->K, pub->p);
    element_init_G1(prv->L, pub->p);
    element_init_G1(prv->R, pub->p);
    element_init_Zr(t, pub->p);

    element_invert(t, auth->auth_msk->beta);

    element_pow_zn(prv->K, pub->ga, t);
    
    element_random(t);

    element_pow3_zn(prv->K, cert->g_z_invert, 
        auth->auth_msk->alpha, pub->ga, cert->u, prv->K, t);
    
    element_pow_zn(prv->L, cert->g_z_invert, auth->auth_msk->beta);
    element_pow_zn(prv->L, prv->L, t);

    element_pow_zn(prv->R, pub->ga, t);

    element_mul(t, auth->auth_msk->beta, cert->u);

    for (int i = 0; i < attributes->len; i++)
    {
        dacmacs_prv_kx_t* kx;
        dacmacs_attr_pk_t* attr_pk;
        char *attr;

        attr = g_ptr_array_index(attributes, i);

        if (g_hash_table_contains(prv->kx, attr))continue;
        if (!g_hash_table_contains(auth->attr_pks, attr))continue;
        
        attr = strdup(attr);
        attr_pk = g_hash_table_lookup(auth->attr_pks, attr);

        kx = malloc(sizeof(dacmacs_prv_kx_t));

        kx->version = attr_pk->version;
        element_init_G1(kx->kx, pub->p);
        element_pow2_zn(kx->kx, prv->L, 
            auth->auth_msk->gamma, attr_pk->pk, t);
        
        g_hash_table_insert(prv->kx, attr, kx);
        g_ptr_array_add(prv->attributes, attr);
    }

    element_clear(t);

    return prv;
}

void
dacmacs_enc_detail(bswabe_policy_t* p,
                   dacmacs_capub_t* pub,
                   GPtrArray* params)
{
    GPtrArray* auth_pks;   
    GHashTable* auth_list;
    dacmacs_auth_item_t* auth;
    dacmacs_cph_item_t* item;
    dacmacs_attr_pk_t* attr_pk;
    element_t ri;

    auth_pks = g_ptr_array_index(params, 0);
    auth_list = g_ptr_array_index(params, 1);

    item = malloc(sizeof(dacmacs_cph_item_t));

    for (int i = 0; i < auth_pks->len; i ++)
    {
        auth = g_ptr_array_index(auth_pks, i);
        if (g_hash_table_contains(auth->attr_pks, p->attr))break;
    }

    if (!g_hash_table_contains(auth_list, auth->aid))
        g_hash_table_insert(auth_list, auth->aid, auth);
    
    attr_pk = g_hash_table_lookup(auth->attr_pks, p->attr);
    
    item->version = attr_pk->version;

    element_init_G1(item->C3, pub->p);
    element_init_G1(item->D1, pub->p);
    element_init_G1(item->D2, pub->p);

    element_init_Zr(ri, pub->p);
    
    element_random(ri);

    element_pow2_zn(item->C3, pub->ga, p->q->coef[0], attr_pk->pk, ri);
    
    element_pow_zn(item->D2, auth->pks->g_beta_gamma, ri);

    element_neg(ri, ri);

    element_pow_zn(item->D1, auth->pks->g_beta_invert, ri);

    element_clear(ri);

    p->cph = item;
}

dacmacs_cph_t*
dacmacs_encrypt(dacmacs_capub_t* pub,
                GPtrArray* attr_pks,
                element_t m,
                char *policy)
{
    dacmacs_cph_t* cph;
    GPtrArray* params;
    element_t s;
    GHashTable* auth_list;
    GList* list;
    dacmacs_auth_item_t* item;

    params = g_ptr_array_new();
    auth_list = g_hash_table_new(g_str_hash, g_str_equal);
    g_ptr_array_add(params, attr_pks);
    g_ptr_array_add(params, auth_list);
    cph = malloc(sizeof(dacmacs_cph_t));

    cph->aids = g_ptr_array_new();
    cph->policy = strdup(policy);

    cph->p = parse_policy_postfix(cph->policy);
    
    element_init_GT(cph->C0, pub->p);
    element_init_GT(m, pub->p);
    element_init_G1(cph->C1, pub->p);
    cph->C2 = g_hash_table_new(g_str_hash, g_str_equal);
    // element_init_G1(cph->C2, pub->p);
    
    element_init_Zr(s, pub->p);

    element_random(s);

    element_pow_zn(cph->C1, pub->g, s);

    fill_policy(cph->p, pub, s, params, dacmacs_enc_detail);

    element_set1(cph->C0);
    // element_set1(cph->C2);

    list = g_hash_table_get_values(auth_list);

    for (GList* p = list; p != NULL; p = p->next)
    {
        item = p->data;
        element_t *c2;
        c2 = malloc(sizeof(element_t));
        element_init_G1(*c2, pub->p);
        g_ptr_array_add(cph->aids, strdup(item->aid));
        
        element_mul(cph->C0, cph->C0, item->pks->egg_alpha);
        
        element_pow_zn(*c2, item->pks->g_beta_invert, s);
        g_hash_table_insert(cph->C2, item->aid, c2);
    }

    element_pow_zn(cph->C0, cph->C0, s);
    
    element_random(m);
    element_mul(cph->C0, cph->C0, m);

    // element_pow_zn(cph->C2, cph->C2, s);
    
    element_clear(s);
    
    g_list_free(list);
    g_hash_table_destroy(auth_list);
    g_ptr_array_free(params, FALSE);
    return cph;
}

int
dacmacs_dec_detail(element_t r,
                   element_t exp,
                   bswabe_policy_t* p,
                   dacmacs_user_t* prv,
                   dacmacs_capub_t* pub)
{
    element_t s, t;
    dacmacs_prv_item_t* prv_item;
    dacmacs_cph_item_t* item;
    dacmacs_prv_kx_t* kx;
    
    GList *list;

    list = g_hash_table_get_values(prv->sks);

    for (GList* q = list; q != NULL; q = q->next)
    {
        prv_item = q->data;
        if (g_hash_table_contains(prv_item->kx, p->attr))break;
    }

    item = p->cph;
    kx = g_hash_table_lookup(prv_item->kx, p->attr);

    if (kx->version != item->version)
    {
        raise_error("Your sk's version can't satify the cph's version\n" \
                    "Please update the cph or your sk.\n");
        g_list_free(list);
        return 1;
    }

    element_init_GT(s, pub->p);
    element_init_GT(t, pub->p);

    element_pairing(s, item->C3, prv->gpk);
    element_pairing(t, item->D1, kx->kx);
    
    element_mul(s, s, t);

    element_pairing(t, item->D2, prv_item->L);

    element_mul(s, s, t);

    element_pow_zn(s, s, exp);

    element_mul(r, r, s);

    element_clear(s);
    element_clear(t);

    g_list_free(list);
    return 0;
}

int
dacmacs_decrypt(dacmacs_cph_t* cph,
                dacmacs_capub_t* pub,
                dacmacs_user_t* prv,
                element_t m)
{
    int res;
    element_t dp, di, pow, mul1, mul2;
    GList* list;
    dacmacs_prv_item_t* prv_item;
    
    res = 0;

    check_sat(cph->p, prv->attributes);

    pick_sat_min_leaves(cph->p);

    if (!cph->p->satisfiable)
    {
        raise_error("your attributes can't satify the policy.");
        return 1;
    }
    element_init_GT(dp, pub->p);
    element_init_GT(di, pub->p);
    element_init_Zr(pow, pub->p);
    element_init_GT(mul1, pub->p);
    element_init_GT(mul2, pub->p);
    element_init_GT(m, pub->p);
    
    if (dec_flatten(dp, cph->p, prv, pub, pub->p, dacmacs_dec_detail))
    {
        res = 1;
        goto dacmacs_destory;
    };

    element_invert(dp, dp);

    element_set1(di);

    for (int i = 0; i < cph->aids->len; i ++)
    {
        char *aid;
        aid = g_ptr_array_index(cph->aids, i);

        prv_item = g_hash_table_lookup(prv->sks, aid);

        element_pairing(mul1, cph->C1, prv_item->K);
        
        element_pairing(mul2, prv_item->R, 
            *((element_t *)g_hash_table_lookup(cph->C2, aid)));
        
        element_invert(mul2, mul2);
        
        element_mul(mul1, mul1, mul2);
        element_mul(mul1, mul1, dp);
        element_mul(di, di, mul1);
    }
    
    element_pow_zn(di, di, prv->gsk);

    element_invert(di, di);

    element_mul(m, cph->C0, di);

dacmacs_destory:

    element_clear(dp);
    element_clear(di);
    element_clear(mul1);
    element_clear(mul2);
    element_clear(pow);

    return res;
}

void
dacmacs_ukeygen(dacmacs_auth_msk_t* auth_msk,
                dacmacs_capub_t* pub,
                dacmacs_attr_pk_t* attr_pks,
                dacmacs_attr_msk_t* attr_msks,
                char* attribute)
{
    element_t vx1, pkm;

    
    attr_pks->version ++;
    attr_msks->version = attr_pks->version;
    element_init_Zr(vx1, pub->p);
    element_init_G1(pkm, pub->p);
    
    element_random(vx1);

    while(!element_cmp(vx1, attr_msks->vx))
    {
        element_random(vx1);
    }

    element_sub(attr_msks->auk, vx1, attr_msks->vx);
    element_mul(attr_msks->auk, attr_msks->auk, auth_msk->gamma);

    element_set(attr_msks->vx, vx1);
    
    element_pow_zn(pkm, pub->g, attr_msks->auk);

    element_mul(attr_pks->pk, attr_pks->pk, pkm);

    element_mul(attr_msks->kuk, attr_msks->auk, auth_msk->beta);
    element_invert(attr_msks->cuk, auth_msk->gamma);
    element_mul(attr_msks->cuk, attr_msks->cuk, auth_msk->beta);
    element_mul(attr_msks->cuk, attr_msks->cuk, attr_msks->auk);

    element_clear(vx1);
    element_clear(pkm);
}

void
dacmacs_secret_key_update(dacmacs_prv_item_t *prv_item,
                          dacmacs_capub_t* pub,
                          dacmacs_user_cert_t* cert,
                          dacmacs_attr_msk_t* attr_msk,
                          char *attribute)
{
    dacmacs_prv_kx_t* kx;
    element_t kukj;

    kx = g_hash_table_lookup(prv_item->kx, attribute);
    element_init_G1(kukj, pub->p);

    element_pow_zn(kukj, pub->g, cert->u);
    element_pow_zn(kukj, kukj, attr_msk->kuk);

    kx->version = attr_msk->version;
    
    element_mul(kx->kx, kx->kx, kukj);

    element_clear(kukj);
}

void
dacmacs_cph_update(dacmacs_cph_t* cph,
                   dacmacs_capub_t* pub,
                   element_t cuk,
                   int version,
                   char *attribute)
{
    GQueue* queue;
    bswabe_policy_t* cur;
    dacmacs_cph_item_t* item;
    element_t D_CUK;
    
    queue = g_queue_new();
    element_init_G1(D_CUK, pub->p);
    g_queue_push_tail(queue, cph->p);

    while(!g_queue_is_empty(queue))
    {
        cur = g_queue_pop_head(queue);

        if (cur->children->len)
            for (int i = 0; i < cur->children->len; i++)
                g_queue_push_tail(queue, 
                    g_ptr_array_index(cur->children, i));
        else
            if (!strcmp(cur->attr, attribute))
            {
                item = cur->cph;
                item->version = version;
                element_pow_zn(D_CUK, item->D2, cuk);
                element_mul(item->C3, item->C3, D_CUK);
            }
    }
    element_clear(D_CUK);
    
    g_queue_clear(queue);
}