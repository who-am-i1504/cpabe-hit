/*
 * @Author: Qinggang Wu
 * @Date: 2020-12-17 16:58:50
 * @LastEditTime: 2020-12-20 15:03:32
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


void global_setup(drevo_pub_t **pub, int AA_Num)
{
    *pub = malloc(sizeof(drevo_pub_t));

    (*pub) -> pairing_desc = strdup(TYPE_A_PARAMS);

    pairing_init_set_buf((*pub) -> p, 
        (*pub) -> pairing_desc, strlen((*pub) -> pairing_desc));
    
    element_init_G1((*pub) -> g, (*pub) -> p);
    element_init_GT((*pub) -> egg, (*pub) -> p);

    element_random((*pub) -> g);

    element_pairing((*pub) -> egg, (*pub) -> g, (*pub) -> g);
}

void aasetup(drevo_apk_t** apk, 
             drevo_ask_t** ask, 
             drevo_pub_t* pub,
             char *aid)
{
    (*apk) = malloc(sizeof(drevo_apk_t));
    (*ask) = malloc(sizeof(drevo_apk_t));

    element_init_Zr((*ask) -> alpha, pub -> p);
    element_init_Zr((*ask) -> y, pub -> p);
    element_init_GT((*apk) -> egg_alpha, pub -> p);
    element_init_G1((*apk) -> gy, pub -> p);

    element_random((*ask) -> alpha);
    element_random((*ask) -> y);

    element_pow_zn((*apk) -> egg_alpha, pub -> egg, (*ask) -> alpha);
    element_pow_zn((*apk) -> gy, pub -> g, (*ask) -> y);
}

drevo_prv_t *
drevo_init_prv(int gid)
{
    drevo_prv_t* prv;
    prv = malloc(sizeof(drevo_prv_t));

    prv -> attributes = g_ptr_array_new();

    prv -> sk = g_hash_table_new(g_str_hash, g_str_equal);

    prv -> gid = gid;
    return prv;
}

void 
attr_key_gen(drevo_prv_t* prv,
             drevo_pub_t* pub,
             drevo_ask_t* ask,
             int gid,
             char *attribute)
{
    element_t t, hgid, fattr;
    
    element_init_Zr(t, pub -> p);
    element_init_G1(hgid, pub -> p);
    element_init_G1(fattr, pub -> p);
    element_set_si(hgid, gid);

    while(*attribute)
    {
        drevo_prv_item_t* item;
        char *attr;

        attr = strdup(*(attribute ++));

        item = malloc(sizeof(drevo_prv_item_t));

        element_init_G1(item -> k0, pub -> p);
        element_init_G1(item -> k1, pub -> p);
        

        element_random(t);

        element_from_string(fattr, attr);

        element_pow3_zn(item -> k0, pub -> g, ask -> alpha, hgid, ask -> y, fattr, t);

        element_pow_zn(item -> k1, pub -> g, t);

        g_hash_table_insert(prv -> sk, attr, item);
        g_ptr_array_add(prv -> attributes, attr);
    }
    element_clear(t);
    element_clear(hgid);
    element_clear(fattr);

}

drevo_kek_tree_t* 
kek_tree(char *aid, 
         char *attribute, 
         int user_num, 
         drevo_pub_t* pub)
{
    drevo_kek_tree_t* treex;
    treex = malloc(sizeof(drevo_kek_tree_t));
    


}

void
drevo_fill_policy( bswabe_policy_t* p, 
                   drevo_pub_t* pub, 
                   element_t e,
                   element_t z,
                   GHashTable* apk_list)
{
	int i;
	element_t r;
	element_t t;
	element_t h;
    bswabe_polynomial_t* zi;
	
	element_init_same_as(r, e);
	element_init_same_as(t, e);
	element_init_same_as(h, z);

	p->q = rand_poly(p->k - 1, e);
    zi = rand_poly(p->k - 1, z);
	if( p->children->len == 0 )
	{
		element_t ri, fattr;
        drevo_cph_item_t * item;
        drevo_apk_t * apk;
        
        item = malloc(sizeof(drevo_cph_item_t));

        apk = g_hash_table_lookup(apk_list, p -> attr);

        element_init_GT(item -> C1, pub -> p);
        element_init_G1(item -> C2, pub -> p);
        element_init_G1(item -> C3, pub -> p);
        element_init_G1(item -> C4, pub -> p);
        element_init_G1(fattr, pub -> p);

        element_init_Zr(ri, pub -> p);

        element_from_string(fattr, p -> attr);
        element_random(ri);

        element_pow2_zn(item -> C1, 
            pub -> egg, p->q->coef[0], apk -> egg_alpha, ri);
        
        element_pow2_zn(item -> C3, 
            apk -> gy, ri, pub -> g, zi -> coef[0]);
        
        element_pow_zn(item -> C4, fattr, ri);

        element_pow_zn(item -> C2, pub -> g, ri);
        element_invert(item -> C2, item -> C2);
        p -> cph = item;
        element_clear(ri);
        element_clear(fattr);
	}
	else
		for( i = 0; i < p->children->len; i++ )
		{
			element_set_si(r, i + 1);
			eval_poly(t, p->q, r);
            eval_poly(h, zi, r);
			drevo_fill_policy(g_ptr_array_index(p->children, i), pub, t, h, apk_list);
		}
	free_polynomial(p->q);
	free_polynomial(zi);
	element_clear(r);
	element_clear(t);
	element_clear(h);
	
}

drevo_cph_t*
drevo_encrypt(drevo_pub_t* pub,
              GHashTable* apk_list,
              char *policy,
              element_t m)
{
    drevo_cph_t * cph;
    element_t s, z;
    
    cph = malloc(sizeof(drevo_cph_t));

    cph -> policy_desc = strdup(policy);

    cph -> policy - parse_policy_postfix(policy);

    element_init_GT(m, pub -> p);
    element_init_GT(cph -> C0, pub -> p);

    element_init_Zr(s, pub -> p);
    element_init_Zr(z, pub -> p);

    element_random(s);
    element_random(m);
    element_pow_zn(cph -> C0, pub -> egg, s);
    element_mul(cph -> C0, cph -> C0, m);

    element_set0(z);

    drevo_fill_policy(cph -> policy, pub, s, z, apk_list);

    element_clear(s);
    element_clear(z);

    return cph;
}

void 
drevo_reencrypt_policy(bswabe_policy_t* p, 
                       GHashTable* agk)
{
    if( p->children->len == 0 )
    {
        drevo_cph_item_t* item;
        item = p -> cph;

        element_pow_zn(item -> C4, item -> C4, 
            *((element_t *)g_hash_table_lookup(agk, p -> attr)));
    }
    else
    {
        for (int i = 0; i < p->children->len; i++)
            drevo_reencrypt(g_ptr_array_index(p->children, i), agk);
    }
    
}

void
drevo_reencrypt(drevo_cph_t *cph,
                GHashTable * agk)
{
    drevo_reencrypt_policy(cph -> policy, agk);
}

void drevo_dec(element_t r,
               element_t exp,
               bswabe_policy_t* p,
               drevo_mid_value * mid,
               drevo_pub_t* pub)
{
    drevo_prv_t* prv;
    element_t s, t, hgid, agk_invert, km;
    drevo_cph_item_t* item;
    drevo_prv_item_t* prv_item;
    GHashTable * agk;
    
    prv = mid -> prv;
    agk = mid -> agk;

    item = p -> cph;
    prv_item = g_hash_table_lookup(prv->sk, p -> attr);
    
    element_init_GT(s, pub -> p);
    element_init_GT(t, pub -> p);
    element_init_G1(hgid, pub -> p);
    element_init_Zr(agk_invert, pub -> p);
    element_init_G1(km, pub -> p);

    element_pairing(s, prv_item -> k0, item -> C2);
    element_mul(s, item -> C1, s);
    
    element_set_si(hgid, prv -> gid);
    element_pairing(t, hgid, item -> C3);
    element_mul(s, s, t);

    element_invert(agk_invert, 
        *((element_t *)g_hash_table_lookup(agk, p -> attr)));
    
    element_pow_zn(km, prv_item -> k1, agk_invert);

    element_pairing(t, km, item -> C4);

    element_mul(s, s, t);

    element_pow_zn(s, s, exp);
    element_mul(r, r, s);

    element_clear(s);
    element_clear(t);
    element_clear(hgid);

}


int 
drevo_decrypt(drevo_cph_t* cph, 
              drevo_pub_t* pub,
              GHashTable* agk,
              drevo_prv_t* prv,
              element_t m)
{
    int res;
    element_t egg_s;
    GPtrArray* attributes;
    GList* prv;
    drevo_mid_value mid;

    res = 0;
    
    attributes = g_ptr_array_new();
    prv = g_hash_table_get_keys(prv->sk);

    for (GList* p = prv; p != NULL; p = p -> next)
        g_ptr_array_add(attributes, p -> data);
        

    element_init_GT(m, pub -> p);
    element_init_GT(egg_s, pub -> p);
    
    check_sat(cph -> policy, attributes);

    pick_sat_min_leaves(cph -> policy);

    if (!cph -> policy -> satisfiable)
    {
        raise_error("your attributes can't satify the policy.");
        res = 1;
        goto drevo_destory;
    }
    
    mid.pub = pub;
    mid.agk = agk;
    mid.prv = prv;
    dec_flatten(egg_s, cph -> policy, 
        &mid, pub, pub -> p, drevo_dec);

    element_div(m, cph -> C0, egg_s);

    element_clear(egg_s);

drevo_destory:
    g_list_free(prv);
    g_ptr_array_free(attributes, FALSE);

    return res;
}

void 
drevo_modify(bswabe_policy_t* p, 
             element_t z)
{
    if (!p -> children -> len)
    {
        drevo_cph_item_t *item;
        item = p -> cph;
        element_pow_zn(item -> C1, item -> C1, z);
        element_pow_zn(item -> C3, item -> C3, z);
    }
    else
        for (int i = 0; i < p -> children -> len; i ++)
            drevo_modify(g_ptr_array_index(p -> children, i), z);
}

void 
drevo_genTK(drevo_tk_t** tk, 
            drevo_rk_t** rk,
            drevo_prv_t* prv,
            drevo_pub_t* pub,
            GHashTable* agk)
{
    GList* attrs;
    element_t km, z, mid;

    (*tk) = malloc(sizeof(drevo_tk_t));
    (*rk) = malloc(sizeof(drevo_rk_t));

    (*tk) -> attributes = g_ptr_array_copy(prv -> attributes, strdup, NULL);
    
    (*tk) -> gid = prv -> gid;
    (*tk) -> tk = g_hash_table_new(g_str_hash, g_str_equal);
    
    element_init_Zr((*rk) -> z, pub -> p);
    element_init_Zr(z, pub -> p);
    element_init_Zr(mid, pub -> p);

    element_random((*rk) -> z);
    element_invert(z, (*rk) -> z);

    attrs = g_hash_table_get_keys(prv -> sk);
    element_init_G1(km, pub -> p);
    for (GList* p = attrs; p != NULL; p = p -> next)
    {
        drevo_prv_item_t* item;
        drevo_prv_item_t* prv_item;

        item = malloc(sizeof(drevo_prv_item_t));

        prv_item = g_hash_table_lookup(prv->sk, p -> data);
        element_init_as(item -> k0, prv_item -> k0);
        element_init_as(item -> k1, prv_item -> k0);

        element_set(km, prv_item -> k0);

        element_pow_zn(item -> k0, km, z);
        element_invert(mid, *((element_t *)g_hash_table_lookup(agk, p -> data)));

        element_pow_zn(km, km, mid);

        element_pow_zn(item -> k1, km, z);

        g_hash_table_insert((*tk) -> tk, p -> data, item);

    }

    element_clear(km);
    element_clear(z);
    element_clear(mid);
}


void drevo_transform_dec(element_t r,
               element_t exp,
               bswabe_policy_t* p,
               drevo_tk_t * prv,
               drevo_pub_t* pub)
{
    element_t s, t, hgid;
    drevo_cph_item_t* item;
    drevo_prv_item_t* prv_item;
    
    item = p -> cph;
    prv_item = g_hash_table_lookup(prv->tk, p -> attr);
    
    element_init_GT(s, pub -> p);
    element_init_GT(t, pub -> p);
    element_init_G1(hgid, pub -> p);

    element_pairing(s, prv_item -> k0, item -> C2);
    element_mul(s, item -> C1, s);
    
    element_set_si(hgid, prv -> gid);
    element_pairing(t, hgid, item -> C3);
    element_mul(s, s, t);

    element_pairing(t, prv_item -> k1, item -> C4);

    element_mul(s, s, t);

    element_pow_zn(s, s, exp);
    element_mul(r, r, s);

    element_clear(s);
    element_clear(t);
    element_clear(hgid);

}

drevo_cph_out_t*
drevo_transform(drevo_cph_t* cph,
                drevo_pub_t* pub,
                drevo_tk_t* tk)
{
    drevo_cph_out_t* out;

    out = malloc(sizeof(drevo_cph_out_t));

    out -> policy = strdup(cph -> policy_desc);

    element_init_same_as(out -> C0, cph -> C0);

    element_init_GT(out -> egg_s_z, pub -> p);

    element_set(out -> C0, cph -> C0);

    check_sat(cph -> policy, tk -> attributes);

    pick_sat_min_leaves(cph -> policy);
    
    dec_flatten(out -> egg_s_z, cph -> policy, tk, pub, pub -> p, drevo_transform_dec);
    
    return out;
}

void 
revo_decrypt_out(drevo_cph_out_t* out,
                 drevo_rk_t* rk,
                 element_t m)
{
    element_init_same_as(m, out -> egg_s_z);
    element_pow_zn(m, out -> egg_s_z, rk -> z);
    element_div(m, out -> C0, m);
}

void
revo_keyupdate(drevo_prv_t *prv, 
               char *attribute, 
               element_t agku,
               GHashTable * agk)
{
    GList* list;
    element_t pow;
    drevo_prv_item_t* item;
    
    element_init_same_as(pow, agku);
    list = g_hash_table_get_keys(prv -> sk);

    for (GList* p = list; p!=NULL; p = p -> next)
    {
        item = g_hash_table_lookup(prv -> sk, p -> data);
        if (!strcmp(p -> data, attribute))
        {
            element_invert(pow, agku);            
        }
        else
        {
            element_invert(pow, 
                *((element_t*)g_hash_table_lookup(agk, p -> data)));
        }

        element_pow_zn(item -> k1, item -> k1, pow);
    }
    element_clear(pow);
    g_list_free(list);
}


void
drevo_ctupdate_policy( bswabe_policy_t* p, 
                       drevo_pub_t* pub, 
                       element_t e,
                       element_t z,
                       drevo_mid_value1* apk_list)
{
	int i;
	element_t r;
	element_t t;
	element_t h;
    bswabe_polynomial_t* zi;
	
	element_init_same_as(r, e);
	element_init_same_as(t, e);
	element_init_same_as(h, z);

	p->q = rand_poly(p->k - 1, e);
    zi = rand_poly(p->k - 1, z);
	if( p->children->len == 0 )
	{
        drevo_cph_item_t* item;
		element_t ri, mGT, mG1;
        drevo_apk_t* apk;

        item = p -> cph;
        apk = g_hash_table_lookup(apk_list -> apk, p -> attr);
        
        element_init_Zr(ri, pub -> p);
        element_init_GT(mGT, pub -> p);
        element_init_G1(mG1, pub -> p);
        element_random(ri);      
        element_pow2_zn(mGT, pub -> egg, 
            p -> q -> coef[0], apk -> egg_alpha, ri);
        element_mul(item -> C1, item -> C1, mGT);

        element_pow_zn(mG1, pub -> g, ri);
        element_invert(mG1, mG1);

        element_mul(item -> C2, item -> C2, mG1);

        element_pow2_zn(mG1, apk -> gy, ri, pub -> g, zi -> coef[i]);

        element_mul(item ->C3, item -> C3, mG1);

        element_from_string(mG1, p -> attr);
        element_pow_zn(mG1, mG1, ri);
        element_mul(item -> C4, item -> C4, mG1);

        if (!strcmp(p -> attr, apk_list -> attribute))
            element_pow_zn(item -> C4, item -> C4, apk_list -> agku);
        else
            element_pow_zn(item -> C4, item -> C4, 
                *((element_t *)g_hash_table_lookup(apk_list -> agk, p -> attr)));
        element_clear(ri);
        element_clear(mGT);
        element_clear(mG1);
        
	}
	else
		for( i = 0; i < p->children->len; i++ )
		{
			element_set_si(r, i + 1);
			eval_poly(t, p->q, r);
            eval_poly(h, zi, r);
			drevo_fill_policy(g_ptr_array_index(p->children, i), pub, t, h, apk_list);
		}
	free_polynomial(p->q);
	free_polynomial(zi);
	element_clear(r);
	element_clear(t);
	element_clear(h);
	
}

void 
drevo_ctupdate(drevo_cph_t* cph, 
               drevo_pub_t* pub,
               char *attribute,
               element_t agku,
               GHashTable* agk,
               GHashTable* apk)
{
    element_t s, z, mid;
    drevo_mid_value1 value;
    
    value.pub = pub;
    value.agk = agk;
    value.agk = &agku;
    value.apk = apk;
    element_init_Zr(s, pub -> p);
    element_init_Zr(z, pub -> p);
    element_init_GT(mid, pub -> p);
    element_random(s);
    element_set0(z);
    element_pow_zn(mid, pub -> egg, s);
    element_mul(cph -> C0, cph -> C0, mid);
    drevo_ctupdate_policy(cph -> policy, pub, s, z, &value);
    element_clear(s);
    element_clear(z);
}