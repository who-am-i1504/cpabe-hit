/*
 * @Author: your name
 * @Date: 2020-12-21 16:44:54
 * @LastEditTime: 2020-12-21 22:04:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/drurevo/drurevo.c
 */

#include "drurevo/core.h"

void
drurevo_setup(dru_pub_t **pub, 
              dru_msk_t** msk)
{
    (*pub) = malloc(sizeof(dru_pub_t));
    (*msk) = malloc(sizeof(dru_msk_t));

    (*pub) -> pairing_desc = generate_a1_params(160);

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
    
    element_random((*msk)->a);
    element_random((*msk)->alpha);
    element_random((*msk)->beta);
    element_random((*msk)->gamma);

    element_random((*pub)->g);

    element_pairing((*pub)->egg_alpha, (*pub)->g, (*pub)->g);
    element_pow_zn((*pub)->egg_alpha, (*pub)->egg_alpha, (*msk)->alpha);

    element_pow_zn((*pub)->g_a, (*pub)->g, (*msk)->a);

    element_pow_zn((*pub)->g_beta_invert, (*pub)->g, (*msk)->beta);
    element_invert((*msk)->beta, (*msk)->beta);

    element_pow_zn((*pub)->g_beta, (*pub)->g, (*msk)->beta);
    
}

void
dru_attr_gen(dru_pub_t* pub,
             dru_msk_t* msk,
             GHashTable* pks,
             GHashTable* pmsks,
             char * attribute)
{
    dru_attr_pk_t* pk;
    dru_attr_msk_t* pmsk;

    pk = malloc(sizeof(dru_attr_pk_t));
    pmsk = malloc(sizeof(dru_attr_msk_t));

    pk->attribute = strdup(attribute);

    element_init_Zr(pmsk->alpha_i, pub->p);
    element_init_Zr(pmsk->y, pub->p);
    element_init_Zr(pmsk->vx, pub->p);

    element_init_GT(pk->egg_alpha_i, pub->p);
    element_init_G1(pk->gy, pub->p);
    element_init_G1(pk->pk1, pub->p);
    element_init_G1(pk->pk2, pub->p);

    element_random(pmsk->alpha_i);
    element_random(pmsk->y);
    element_random(pmsk->vx);

    element_pairing(pk->egg_alpha_i, pub->g, pub->g);

    element_pow_zn(pk->egg_alpha_i, pk->egg_alpha_i, pmsk->alpha_i);

    element_pow_zn(pk->gy, pub->g, pmsk->y);

    element_from_string(pk->pk1, attribute);

    element_pow_zn(pk->pk1, pk->pk1, pmsk->vx);

    element_pow_zn(pk->pk2, pk->pk2, msk->gamma);

    g_hash_table_insert(pks, pk->attribute, pk);
    g_hash_table_insert(pmsks, pk->attribute, pmsk);
}

dru_auth_key_t* 
dru_auth_setup(dru_pub_t* pub,
               dru_msk_t* msk,
               GPtrArray* attributes,
               char* auth_id)
{
    dru_auth_key_t* auth;
    auth = malloc(sizeof(dru_auth_key_t));

    auth->aid = strdup(auth_id);
    auth->attr_pk = g_hash_table_new(g_str_hash, g_str_equal);
    auth->attr_msk = g_hash_table_new(g_str_hash, g_str_equal);

    for (int i = 0; i < attributes->len; i ++)
    {
        dru_attr_gen(pub, msk, auth->attr_pk, 
            auth->attr_msk, g_ptr_array_index(attributes, i));
    }
    
    return auth;
}

GHashTable* 
dru_auth_prv_keygen(element_t apk,
                    element_t t,
                    dru_pub_t* pub,
                    GHashTable* pkxs,
                    char* gid,
                    GPtrArray* attributes)
{
    dru_prv_item_t* item;
    element_t t_beta;
    item = malloc(sizeof(dru_prv_item_t));

    element_init_same_as(t_beta, t);
    element_init_G1(item->kx, pub->p);
    element_init_G1(item->hgid, pub->p);

    element_init_

    
    
}

dru_prv_t*
dru_keygen(dru_pub_t* pub,
           dru_msk_t* msk,
           char* gid,
           char** attributes,
           element_t t)
{
    dru_prv_t* prv;
    
    prv = malloc(sizeof(dru_prv_t));

    prv->attr_kx = g_hash_table_new(g_str_hash, g_str_equal);
    prv->attributes = g_ptr_array_new();
    
    element_init_G1(prv->K, pub->p);
    element_init_G1(prv->L, pub->p);

    element_init_Zr(t, pub->p);
    element_random(t);

    element_pow_zn(prv->K, pub->g_beta_invert, t);

    element_pow2_zn(prv->K, pub->g_beta_invert, msk->alpha, prv->K, msk->a);

    element_pow_zn(prv->L, pub->g, t);
    
    element_mul(t, t, msk->beta);

    while(*attributes)
        g_ptr_array_add(prv->attributes, strdup(*(attributes++)));

    return prv;

}

void 
dru_fill_policy(bswabe_policy_t* p,
                element_t s1,
                element_t s2,
                dru_pub_t* pub,
                GHashTable* pkxs)
{
    int i;
	element_t r;
	element_t t1;
    element_t t2;
    bswabe_polynomial_t* sw2;
	
	element_init_same_as(r, s1);
	element_init_same_as(t1, s1);
	element_init_same_as(t2, s1);

	p->q = rand_poly(p->k - 1, s1);
    sw2 = rand_poly(p->k-1, s2);

	if( p->children->len == 0 )
	{
		
	}
	else
		for( i = 0; i < p->children->len; i++ )
		{
			element_set_si(r, i + 1);
			eval_poly(t1, p->q, r);
            eval_poly(t2, sw2, r);
			dru_fill_policy(g_ptr_array_index(p->children, i), t1, t2, pub, pkxs);
		}
	free_polynomial(p->q);
    free_polynomial(sw2);
	element_clear(r);
	element_clear(t1);
	element_clear(t2);
}

dru_cph_t *
dru_encrypt(dru_pub_t* pub,
            dru_auth_pk_t* pk,
            element_t m,
            char* policy)
{
    dru_cph_t *cph;
    element_t s1, s2;
    cph = malloc(sizeof(dru_cph_t));

    cph->policy = strudp(policy);
    cph->p = parse_policy_postfix(cph->policy);

    element_init_G1(s1, pub->p);
    element_init_G1(s2, pub->p);
    element_init_GT(cph->C0, pub->p);
    element_init_GT(m, pub->p);

    element_random(m);
    element_random(s1);
    element_random(s2);

    element_pow_zn(cph->C0, pub->egg_alpha, s1);
    element_pow_zn(cph->C0, cph->C0, s2);

    element_mul(cph->C0, m, cph->C0);

    element_mul(cph->C1, pub->g_beta, s1);

    
    
}