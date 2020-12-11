#include "decentral/core.h"

int global_setup(dcen_global_param ** pub, int lambda)
{
    *pub = malloc(sizeof(dcen_global_param));

    (*pub) -> pairing_desc = generate_a1_params(lambda);

    pairing_init_set_buf((*pub) -> p,
        (*pub) -> pairing_desc, strlen((*pub)->pairing_desc));
    element_init_G1((*pub) -> g, (*pub) -> p);
    element_random((*pub) -> g);
    return 0;
}

dcen_authority_keys* 
authority_setup(dcen_global_param* pub, 
                char *auth_id, 
                char ** attributes)
{
    dcen_authority_keys * auth_keys;
    element_t egg;
    element_t a, y;
    auth_keys = malloc(sizeof(dcen_authority_keys));
    
    /* Initial */
    element_init_GT(egg, pub -> p);
    element_init_Zr(a, pub -> p);
    element_init_Zr(y, pub -> p);
    auth_keys -> auth_id = strdup(auth_id);
    auth_keys -> pub_keys = g_hash_table_new(g_str_hash, g_str_equal);
    auth_keys -> secret_keys = g_hash_table_new(g_str_hash, g_str_equal);
    
    // e(g,g)
    pairing_apply(egg, pub -> g, pub -> g, pub -> p);

    while(*attributes)
    {
        char * attr;
        dcen_secret_t * secret;
        dcen_pub_t * pub_keys;

        secret = malloc(sizeof(dcen_secret_t));
        pub_keys = malloc(sizeof(dcen_pub_t));

        element_init_GT(pub_keys -> egg_a, pub -> p);
        element_init_G1(pub_keys -> gy, pub -> p);
        element_init_Zr(secret -> a, pub -> p);
        element_init_Zr(secret -> y, pub -> p);

        attr = strdup(*(attributes ++));
        element_random(a);
        element_random(y);

        element_set(secret -> a, a);
        element_set(secret -> y, y);

        // // e(g,g) ^ a
        element_pow_zn(pub_keys -> egg_a, egg, a);
        // g ^ y
        element_pow_zn(pub_keys -> gy, pub -> g, y);

        g_hash_table_insert(auth_keys -> pub_keys, attr, pub_keys);
        g_hash_table_insert(auth_keys -> secret_keys, attr, secret);
        // element_printf("%B\t%s\n", pub_keys -> gy, attr);
    }

    element_clear(egg);
    element_clear(a);
    element_clear(y);
    return auth_keys;

}

dcen_personal_single_key* 
keygen(char *uid, 
       char *attribute, 
       dcen_secret_t* sk, 
       dcen_global_param * pub)
{
    element_t h_gid;
    element_t a;
    element_t y;
    dcen_personal_single_key * per_keys;
    element_t *key;

    key = malloc(sizeof(element_t));
    per_keys = malloc(sizeof(dcen_personal_single_key));

    element_init_G1(h_gid, pub -> p);
    element_init_Zr(a, pub -> p);
    element_init_Zr(y, pub -> p);
    element_init_G1(*key, pub -> p);

    element_from_string(h_gid, uid);
    element_set(a, sk -> a);
    element_set(y, sk -> y);
    element_pow_zn(*key, pub -> g, a);
    element_pow_zn(h_gid, h_gid, y);
    element_mul(*key, *key, h_gid);

    per_keys -> key = key;
    per_keys -> attribute = strdup(attribute);

    element_clear(h_gid);
    element_clear(y);
    element_clear(a);

    return per_keys;

}

void
fill_policy_for_dcen( bswabe_policy_t* p, 
             dcen_global_param* pub, 
             element_t e,
             element_t w,
             dcen_cph_t* cph,
			 GHashTable* pks)
{
	int i;
	element_t r;
	element_t t;
    element_t wi;
    bswabe_polynomial_t *qw;
	
	element_init_same_as(r, e);
	element_init_same_as(t, e);
    element_init_same_as(wi, w);
    
	p->q = rand_poly(p->k - 1, e);
    qw = rand_poly(p->k - 1, w);
	if( p->children->len == 0 )
	{
        /*
        element_t * c1i;
	    c1i = malloc(sizeof(element_t));
	    p->cph = c1i;
	    element_init_Zr(*c1i, pub->p);
	    element_set(*c1i, p->q->coef[0]);
        element_printf("%s\t%B\n", p->attr, *c1i);
        /*/
        dcen_cph_list* c_list;
        p -> cph = malloc(sizeof(dcen_cph_list));
        c_list = p -> cph;
        element_t rx;
        element_t c1m, c3m;

        element_init_Zr(rx, pub -> p);
        element_init_GT(c1m, pub -> p);
        element_init_G1(c3m, pub -> p);
        element_init_GT(c_list -> c1, pub -> p);
        element_init_G1(c_list -> c2, pub -> p);
        element_init_G1(c_list -> c3, pub -> p);
        // random rx on ZP
        element_random(rx);
        // (e(g,g)^a) ^ rx 
        element_pow_zn(c1m, *((element_t *)
            (((dcen_pub_t *)g_hash_table_lookup(pks, p -> attr))->egg_a)), rx);
        
        // e(g,g)
        pairing_apply(c_list -> c1, pub -> g, pub -> g, pub -> p);
        // e(g,g)^lambda
        element_pow_zn(c_list -> c1, c_list -> c1, p->q->coef[0]);
        // (e(g,g)^lambda) * ((e(g,g)^a) ^ rx)
        element_mul(c_list -> c1, c_list -> c1, c1m);

        // (g ^ rx)
        element_pow_zn(c_list -> c2, pub -> g, rx);

        // g^wx
        element_pow_zn(c3m, pub -> g, qw->coef[0]);
        // gy^rx
        element_pow_zn(c_list -> c3, *((element_t *)
            (((dcen_pub_t *)g_hash_table_lookup(pks, p -> attr)) -> gy)), rx);
        // (g^wx) * (gy^rx) 
        element_mul(c_list -> c3, c_list -> c3, c3m);

        element_clear(rx);
        element_clear(c1m);
        element_clear(c3m);
	}
	else
		for( i = 0; i < p->children->len; i++ )
		{
			element_set_si(r, i + 1);
			eval_poly(t, p->q, r);
            eval_poly(wi, qw, r);
            // element_printf("wi is\t%ld\t%B\n", p, wi);
			fill_policy_for_dcen(g_ptr_array_index(p->children, i), pub, t, wi, cph, pks);
		}
    free_polynomial(qw);
	element_clear(r);
	element_clear(t);
	element_clear(wi);
	
}


dcen_cph_t *
encrypt(element_t m,
        char *policy, 
        dcen_global_param *pub,
        GHashTable *pks)
{
    dcen_cph_t * cph;
    element_t s, n, w;
    bswabe_policy_t * p;

    cph = malloc(sizeof(dcen_cph_t));


    element_init_GT(cph -> c0, pub -> p);
    element_init_Zr(s, pub -> p);
    element_init_Zr(w, pub -> p);
    element_init_GT(m, pub -> p);
    element_init_GT(n, pub -> p);

    p = parse_policy_postfix(policy);
    cph -> p = p;

    element_random(s);
    element_random(m);
    element_set0(w);

    pairing_apply(n, pub -> g, pub -> g, pub -> p);
    element_pow_zn(n, n, s);
    
    element_mul(cph -> c0, m, n);

    fill_policy_for_dcen(p, pub, s, w, cph, pks);

    element_clear(n);
    element_clear(s);

    return cph;
}

void dcen_dec_function(element_t r, 
                  element_t exp, 
                  bswabe_policy_t *p,
                  dcen_personal_keys * prv,
                  dcen_global_param * pub)
{
    /*
    element_t *c1;
	element_t m;
    c1 = p -> cph;
	element_init_Zr(m, pub -> p);
	element_mul(m, *c1, exp);
	element_add(r, r, m);
	element_clear(m);
	element_printf("dec:\t%s\t%B\t%B\n",p->attr, *c1, r);
    */
    dcen_cph_list *list;
    list = p -> cph;
    dcen_personal_single_key * person;
    element_t s, t, n;

    element_init_GT(s, pub -> p);
    element_init_GT(t, pub -> p);
    element_init_G1(n, pub -> p);

    person = g_hash_table_lookup(prv -> keys, p -> attr);

    element_from_string(n, prv -> uid);
    // element_printf("%s\t%B\n", prv->uid, n);
    
    pairing_apply(s, n, list -> c3, pub -> p);
    element_mul(s, list -> c1, s);
    
    pairing_apply(t, person -> key, list -> c2, pub -> p);

    element_invert(t, t);
    element_mul(s, s, t);
    element_pow_zn(s, s, exp);
    element_mul(r, r, s);

    element_clear(s);
    element_clear(n);
    element_clear(t);
}

int decrypt(element_t m, 
            dcen_cph_t * cph, 
            dcen_global_param * pub,
            dcen_personal_keys *kp)
{
    int res = 0;
    element_t t;
    GPtrArray * attributes;
    GList * attr_list;
    element_init_GT(t, pub -> p);
    element_init_GT(m, pub -> p);
    
    attributes = g_ptr_array_new();
    attr_list = g_hash_table_get_keys(kp->keys);
    
    for (GList* p = attr_list; p != NULL; p = p -> next)
        g_ptr_array_add(attributes, p -> data);
    g_list_free(attr_list);
    
    check_sat(cph -> p, attributes);
    if (!cph->p->satisfiable)
    {
        raise_error("cannot decrypt, attributes in key do not satisfy policy\n");
        res = 1;
        goto dcen_dec_destory;
    }
    
    pick_sat_min_leaves(cph -> p);
    
    
    dec_flatten(t, cph -> p, kp, pub, pub -> p, dcen_dec_function);

    element_invert(t,t);

    element_mul(m, cph -> c0, t);

dcen_dec_destory:
    element_clear(t);
    g_ptr_array_free(attributes, FALSE);
    return res;

}

void 
merge_hash_table(GHashTable *now, 
                      GHashTable * old)
{
    GList * list;
    list = g_hash_table_get_keys(old);
    for (GList * p = list; p != NULL; p = p -> next)
    {
        if (g_hash_table_contains(now, p -> data))continue;
        g_hash_table_insert(now, 
            p -> data, g_hash_table_lookup(old, p -> data));
    }
    g_list_free(list);
}