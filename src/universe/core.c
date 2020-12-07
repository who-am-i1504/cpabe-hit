#include "universe/core.h"

#define TYPE_A_PARAMS \
    "type a\n" \
    "q 81869981414486565817042987620009425916711137248094272342132238763687306328559\n" \
    "r 604462909877683331530751\n" \
    "h 135442522736512392410054892783912140655846630328108560\n" \
    "exp1 46\n" \
    "exp2 79\n" \
    "sign0 -1\n" \
    "sign1 1\n"

void
bswabe_setup( bswabe_pub_t** pub, bswabe_msk_t** msk )
{
	element_t alpha;

	/* initialize */
 
	*pub = malloc(sizeof(bswabe_pub_t));
	*msk = malloc(sizeof(bswabe_msk_t));

	(*pub)->pairing_desc = strdup(TYPE_A_PARAMS);
	pairing_init_set_buf((*pub)->p, (*pub)->pairing_desc, strlen((*pub)->pairing_desc));

	element_init_G1((*pub) -> g, (*pub) -> p);
	element_init_G1((*pub) -> u, (*pub) -> p);
	element_init_G1((*pub) -> h, (*pub) -> p);
	element_init_G1((*pub) -> w, (*pub) -> p);
	element_init_G1((*pub) -> v, (*pub) -> p);
	element_init_GT((*pub) -> egg_alpha, (*pub) -> p);
    element_init_Zr((*msk) -> alpha, (*pub) -> p); 

	/* compute */

 	element_random((*msk)->alpha);
	element_random((*pub)->g);
	element_random((*pub)->u);
	element_random((*pub)->h);
	element_random((*pub)->w);
	element_random((*pub)->v);
	
	pairing_apply((*pub) -> egg_alpha, (*pub) -> g, (*pub) -> g, (*pub) -> p);
	element_pow_zn((*pub) -> egg_alpha, (*pub) -> egg_alpha, (*msk) -> alpha);
}

bswabe_prv_t* bswabe_keygen( bswabe_pub_t* pub,
							 bswabe_msk_t* msk,
							 char** attributes )
{
	bswabe_prv_t* prv;
    element_t r;
    element_t r_inv;
	element_t mid;
	/* initialize */

	prv = malloc(sizeof(bswabe_prv_t));

    element_init_Zr(r, pub -> p);
	element_init_G1(mid, pub -> p);
    element_init_G1(prv -> k0, pub -> p);
    element_init_G1(prv -> k1, pub -> p);
    element_init_Zr(r_inv, pub -> p);
    prv -> k2s = g_hash_table_new(g_str_hash, g_str_equal);
    prv -> k3s = g_hash_table_new(g_str_hash, g_str_equal);
	prv->comps = g_ptr_array_new();

	/* compute */

	// random element r on ZP
    element_random(r);
	// g ^ alpha
    element_pow_zn(prv -> k0, pub -> g, msk -> alpha);
	// w ^ r
    element_pow_zn(mid, pub -> w, r);
	// (g ^ alpha) * (w ^ r)
    element_mul(prv->k0, prv->k0, mid);
	// g ^ r
    element_pow_zn(prv -> k1, pub -> g, r);
    element_neg(r_inv, r);

	while( *attributes )
	{
		char *attr;
        element_t Ai;
        element_t * k2i;
        element_t * k3i;
        element_t m_k3;
		element_t n_k3;
        element_t rp;
        
        k2i = malloc(sizeof(element_t));
        k3i = malloc(sizeof(element_t));

        element_init_G1(*k2i, pub -> p);
        element_init_G1(*k3i, pub -> p);
        element_init_G1(m_k3, pub -> p);
        element_init_G1(n_k3, pub -> p);
        element_init_Zr(rp, pub -> p);
        element_init_Zr(Ai, pub -> p);
        
        attr = strdup(*(attributes ++));
		// printf("%s \t %s\n", attr, *attributes);
        element_from_string(Ai, attr);
        element_random(rp);

		// g ^ ri
        element_pow_zn(*k2i, pub -> g, rp);

		// u ^ Ai -> Ai
        element_pow_zn(m_k3, pub -> u, Ai);
		// (u ^ Ai) * h -> Ai
        element_mul(m_k3, m_k3, pub -> h);
		// ((u ^ Ai) * h) ^ rp -> Ai
        element_pow_zn(m_k3, m_k3, rp);
		// v ^ -r
        element_pow_zn(n_k3, pub -> v, r_inv);
		// (((u ^ Ai) * h) ^ rp) * (v ^ -ri)
        element_mul(*k3i, n_k3, m_k3);
        
        element_clear(rp);
        element_clear(Ai);
        element_clear(m_k3);
        element_clear(n_k3);

        g_hash_table_insert(prv -> k2s, attr, k2i);
        g_hash_table_insert(prv -> k3s, attr, k3i);
        g_ptr_array_add(prv -> comps, attr);

		// element_printf("%B\t%B\n", *k2i, *k3i);
	}

    element_clear(r);
    element_clear(r_inv);

	return prv;
}

void enc_func( bswabe_policy_t* p, 
               bswabe_pub_t* pub, 
			   bswabe_cph_t* cph )
{
	/*
	element_t * c1i;
	p->cph = cph;
	c1i = malloc(sizeof(element_t));
	element_init_Zr(*c1i, pub->p);
	element_set(*c1i, p->q->coef[0]);
	g_hash_table_insert(cph -> c1, p -> attr, c1i);
	element_printf("%s\t%B\n", p -> attr, p->q->coef[0]);
	*/
	
	element_t r;
	element_t t;
	element_t h;
	element_t m;
	element_t n;
	element_t h_a;
	
	element_t * c1i;
	element_t * c2i;
	element_t * c3i;
	
	c1i = malloc(sizeof(element_t));
	c2i = malloc(sizeof(element_t));
	c3i = malloc(sizeof(element_t));
	element_init_G1(*c1i, pub -> p);
	element_init_G1(*c2i, pub -> p);
	element_init_G1(*c3i, pub -> p);
	element_init_Zr(r, pub->p);
	element_init_Zr(t, pub->p);
	element_init_G1(h, pub->p);
	element_init_Zr(h_a, pub->p);
	element_init_G1(m, pub -> p);
	element_init_G1(n, pub -> p);
	element_from_string(h_a, p -> attr);
	
	element_random(t);
	element_pow_zn(*c3i, pub -> g, t);
	
	element_pow_zn(m, pub -> w, p->q->coef[0]);
	element_pow_zn(n, pub -> v, t);
	element_mul(*c1i, m, n);
	element_pow_zn(m, pub -> u, h_a);
	element_mul(m, m, pub -> h);
	element_neg(r, t);
	element_pow_zn(*c2i, m, r);

	g_hash_table_insert(cph -> c1, p -> attr, c1i);
	g_hash_table_insert(cph -> c2, p -> attr, c2i);
	g_hash_table_insert(cph -> c3, p -> attr, c3i);
	p->cph = cph;

	element_clear(r);
	element_clear(t);
	element_clear(h);
	element_clear(h_a);
	element_clear(m);
	element_clear(n);
}

bswabe_cph_t*
bswabe_enc( bswabe_pub_t* pub, element_t m, char* policy )
{
	bswabe_cph_t* cph;
 	element_t s;

	/* initialize */

	cph = malloc(sizeof(bswabe_cph_t));

	element_init_Zr(s, pub->p);
	element_init_GT(m, pub->p);
	element_init_GT(cph->cs, pub->p);
	element_init_G1(cph->c0,  pub->p);
	cph->p = parse_policy_postfix(policy);
	cph->c1 = g_hash_table_new(g_str_hash, g_str_equal);
	cph->c2 = g_hash_table_new(g_str_hash, g_str_equal);
	cph->c3 = g_hash_table_new(g_str_hash, g_str_equal);

	/* compute */

 	element_random(m);
	// secret s
 	element_random(s);
	// element_printf("%B\n", s);
    // (e(g,g) ^ alpha) ^ s
	element_pow_zn(cph->cs, pub->egg_alpha, s);
	// m * ((e(g,g) ^ alpha) ^ s)
	element_mul(cph->cs, m, cph->cs);

	// g ^ s
	element_pow_zn(cph->c0, pub->g, s);
	fill_policy(cph->p, pub, s, cph, enc_func);


	element_clear(s);

	return cph;
}

void
dec_leaf_flatten( element_t r,
				  element_t exp,
				  bswabe_policy_t* p, 
				  bswabe_prv_t* prv, 
				  bswabe_pub_t* pub )
{
	/*element_t *c1;
	element_t m;
	element_init_Zr(m, pub -> p);
	c1 = g_hash_table_lookup(((bswabe_cph_t*)(p -> cph)) -> c1, p -> attr);
	element_mul(m, *c1, exp);
	element_add(r, r, m);
	element_clear(m);
	element_printf("%B\n", r);
	/*/
	element_t s;
	element_t t;
	element_t *k2;
	element_t *k3;
	element_t *c1;
	element_t *c2;
	element_t *c3;


	k2 = g_hash_table_lookup(prv -> k2s, p -> attr);
	k3 = g_hash_table_lookup(prv -> k3s, p -> attr);
	c1 = g_hash_table_lookup(((bswabe_cph_t*)(p -> cph)) -> c1, p -> attr);
	c2 = g_hash_table_lookup(((bswabe_cph_t*)(p -> cph)) -> c2, p -> attr);
	c3 = g_hash_table_lookup(((bswabe_cph_t*)(p -> cph)) -> c3, p -> attr);
	

	element_init_GT(s, pub->p);
	element_init_GT(t, pub->p);

	pairing_apply(s, *c1, prv -> k1, pub -> p);
	pairing_apply(t, *c2, *k2, pub -> p);

	element_mul(s, s, t);

	pairing_apply(t, *c3, *k3, pub -> p);
	element_mul(s, s, t);

	element_pow_zn(s, s, exp);
	element_mul(r, r, s);

	element_clear(s);
	element_clear(t);
}


int
bswabe_dec( bswabe_pub_t* pub, 
			bswabe_prv_t* prv, 
			bswabe_cph_t* cph, 
			element_t m )
{
	int ret = 0;
	element_t t;

	element_init_GT(m, pub->p);
	element_init_GT(t, pub->p);

	check_sat(cph->p, prv->comps);
	if( !cph->p->satisfiable )
	{
		raise_error("cannot decrypt, attributes in key do not satisfy policy\n");
		ret = 1;
		goto bswabe_dec_destory;
	}

	pick_sat_min_leaves(cph->p);
	dec_flatten(t, cph->p, prv, pub, pub -> p, dec_leaf_flatten);
	

	element_t session_key;
	element_init_GT(session_key, pub -> p);
	pairing_apply(session_key, cph -> c0, prv -> k0, pub -> p);
	
	element_invert(t, t);
	element_mul(session_key, session_key, t);

	element_invert(session_key, session_key);
	element_mul(m, cph -> cs, session_key);
	
bswabe_dec_destory:
	// element_clear(m);
	element_clear(t);
	return ret;
}
