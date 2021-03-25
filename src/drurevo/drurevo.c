/*
 * @Author: your name
 * @Date: 2020-12-21 16:44:54
 * @LastEditTime: 2021-01-22 17:10:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/drurevo/drurevo.c
 */

#include "drurevo/core.h"

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
getij(int *i, 
      int *j, 
      int id, 
      int m)
{
    // printf("%d\t%d\t\n", id, m);
    (*j) = id % m;
    // printf("%d\t%d\t\n", id, m);
    (*j) = (*j) == 0? m:(*j);

    (*i) = (id - (*j)) / m + 1;
}

void
drur_ca_rusetup(drur_capub_t* pub,
                drur_camsk_t* msk)
{
    drur_rupub_t* rupub;
    drur_rumsk_t* rumsk;
    element_t egg, z;

    rupub = pub->rupub;
    rumsk = msk->rumsk;

    element_init_G1(rupub->h, pub->p);
    element_init_G1(rupub->G, pub->p);
    element_init_G1(rupub->H, pub->p);
    element_init_G1(rupub->f, pub->p);

    element_init_GT(egg, pub->p);

    element_init_Zr(z, pub->p);

    element_random(rupub->h);
    element_random(rupub->G);
    element_random(rupub->H);
    element_random(rupub->f);

    element_pairing(egg, pub->g, pub->g);

    rupub->fs = malloc(sizeof(element_t)*rupub->m);
    rupub->EI = malloc(sizeof(element_t)*rupub->m);
    rupub->GI = malloc(sizeof(element_t)*rupub->m);
    rupub->ZI = malloc(sizeof(element_t)*rupub->m);
    rupub->HJ = malloc(sizeof(element_t)*rupub->m);

    rumsk->alpha = malloc(sizeof(element_t)*rumsk->m);
    rumsk->r = malloc(sizeof(element_t)*rumsk->m);
    rumsk->c = malloc(sizeof(element_t)*rumsk->m);

    for (int i = 0; i < rupub->m; i++)
    {
        element_init_G1(rupub->fs[i], pub->p);
        element_init_G1(rupub->GI[i], pub->p);
        element_init_G1(rupub->ZI[i], pub->p);
        element_init_G1(rupub->HJ[i], pub->p);
        element_init_GT(rupub->EI[i], pub->p);

        element_init_Zr(rumsk->alpha[i], pub->p);
        element_init_Zr(rumsk->r[i], pub->p);
        element_init_Zr(rumsk->c[i], pub->p);

        element_random(rupub->fs[i]);

        element_random(rumsk->alpha[i]);
        element_random(rumsk->r[i]);
        element_random(rumsk->c[i]);

        element_random(z);

        element_pow_zn(rupub->EI[i], egg, rumsk->alpha[i]);

        element_pow_zn(rupub->GI[i], pub->g, rumsk->r[i]);
        element_pow_zn(rupub->ZI[i], pub->g, z);
        element_pow_zn(rupub->HJ[i], pub->g, rumsk->c[i]);
    }

    element_clear(egg);
    element_clear(z);
}

void
drur_casetup(drur_capub_t** pub, 
             drur_camsk_t** msk,
             int N,
             int lambda)
{
    drur_rupub_t* rupub;
    drur_rumsk_t* rumsk;
    
    (*pub) = malloc(sizeof(drur_capub_t));
    (*msk) = malloc(sizeof(drur_camsk_t));
    rupub = malloc(sizeof(drur_rupub_t));
    rumsk = malloc(sizeof(drur_rumsk_t));

    (*pub)->rupub = rupub;
    (*msk)->rumsk = rumsk;

    rupub->m = (int)sqrt((double)N) + 1;
    rupub->ctr = 0;
    rumsk->m = rupub->m;
    // rumsk->ctr = 0;

    (*pub)->pairing_desc = generate_a1_params(lambda);

    pairing_init_set_buf((*pub)->p, 
        (*pub)->pairing_desc, strlen((*pub)->pairing_desc));
    
    element_init_G1((*pub)->g, (*pub)->p);
    element_init_G1((*pub)->ga, (*pub)->p);

    element_init_Zr((*msk)->a, (*pub)->p);

    element_random((*msk)->a);
    element_random((*pub)->g);

    element_pow_zn((*pub)->ga, (*pub)->g, (*msk)->a);

    drur_ca_rusetup(*pub, *msk);

}

drur_user_t*
drur_user_register(drur_capub_t* pub,
                   char *uid,
                   GPtrArray* attributes)
{
    drur_user_t* user;
    drur_user_cert_t* cert;

    user = malloc(sizeof(drur_user_t));
    cert = malloc(sizeof(drur_user_cert_t));

    user->cert = cert;
    user->sks = NULL;
    user->cask = NULL;
    user->attributes = g_ptr_array_new();

    cert->uid = strdup(uid);
    element_init_G1(cert->g_z_invert, pub->p);
    element_init_G1(user->gpk, pub->p);
    
    element_init_Zr(cert->u, pub->p);
    element_init_Zr(user->gsk, pub->p);

    element_random(cert->u);
    element_random(user->gsk);

    element_pow_zn(user->gpk, pub->g, cert->u);
    element_pow_zn(cert->g_z_invert, pub->g, user->gsk);

    element_invert(user->gsk, user->gsk);

    for (int i = 0; i < attributes->len; i++)
        g_ptr_array_add(user->attributes, 
            strdup(g_ptr_array_index(attributes, i)));

    cert->id = ++ pub->rupub->ctr;

    return user;
}

drur_auth_t *
drur_aasetup(drur_capub_t* pub,
             GPtrArray* attributes,
             char* aid)
{
    drur_auth_t* auth;
    drur_auth_msk_t* pmsk;
    drur_auth_pub_t* pk;

    auth = malloc(sizeof(drur_auth_t));

    auth->attributes = g_ptr_array_new();
    pmsk = malloc(sizeof(drur_auth_msk_t));
    pk = malloc(sizeof(drur_auth_pub_t));

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

    for (int i = 0; i < attributes->len; i ++)
    {
        char *attr;
        drur_attr_pk_t* pkx;
        drur_attr_msk_t* vx;
        element_t mid;

        attr = g_ptr_array_index(attributes, i);

        if (g_hash_table_contains(auth->attr_pks, attr))continue;

        pkx = malloc(sizeof(drur_attr_pk_t));
        vx = malloc(sizeof(drur_attr_msk_t));

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
        element_mul(pkx->pk, pkx->pk, pmsk->gamma);
        
        g_hash_table_insert(auth->attr_pks, attr, pkx);
        g_hash_table_insert(auth->attr_msks, attr, vx);

        g_ptr_array_add(auth->attributes, attr);

        element_clear(mid);
    }

    return auth;
}

void
drur_add_attribute(drur_attr_pk_t** pkx, 
                   drur_attr_msk_t** vx, 
                   drur_capub_t* pub,
                   drur_auth_msk_t* pmsk,
                   char* attr)
{
    element_t mid;
    (*pkx) = malloc(sizeof(drur_attr_pk_t));
    (*vx) = malloc(sizeof(drur_attr_msk_t));
    (*pkx)->version = 0;
    (*vx)->version = 0;
    element_init_G1((*pkx)->pk, pub->p);
    element_init_G1(mid, pub->p);
    element_init_Zr((*vx)->vx, pub->p);
    element_init_Zr((*vx)->auk, pub->p);
    element_init_Zr((*vx)->kuk, pub->p);
    element_init_Zr((*vx)->cuk, pub->p);
    
    element_set1((*vx)->auk);
    element_set1((*vx)->kuk);
    element_set1((*vx)->cuk);
    element_random((*vx)->vx);
    element_from_string((*pkx)->pk, attr);
    element_pow_zn(mid, pub->g, (*vx)->vx);
    element_mul((*pkx)->pk, (*pkx)->pk, pmsk->gamma);
    
    element_clear(mid);
}

drur_ru_sk_t* 
drur_cakeygen(drur_capub_t* pub,
              drur_camsk_t* msk,
              GPtrArray* attributes,
              int userid)
{
    drur_ru_sk_t* prv;
    int i,j;
    element_t s, sx, dx;
    
    getij(&i, &j, userid, pub->rupub->m);

    prv = malloc(sizeof(drur_ru_sk_t));

    prv->K_j = malloc(sizeof(element_t) * pub->rupub->m);
    prv->m = pub->rupub->m;
    prv->Kijx = g_hash_table_new(g_str_hash, g_str_equal);
    prv->attributes = g_ptr_array_new();
    
    element_init_G1(prv->K0, pub->p);
    element_init_G1(prv->K1, pub->p);
    element_init_G1(prv->K2, pub->p);

    element_init_Zr(s, pub->p);
    element_init_Zr(sx, pub->p);
    element_init_Zr(dx, pub->p);

    element_random(s);
    
    element_set(prv->K0, pub->rupub->f);

    element_mul(prv->K0, prv->K0, pub->rupub->fs[j - 1]);

    element_pow3_zn(prv->K0, pub->g, msk->rumsk->alpha[i - 1],
        pub->rupub->GI[i-1], msk->rumsk->c[j - 1], prv->K0, s);
    
    element_pow_zn(prv->K1, pub->g, s);

    element_pow_zn(prv->K2, pub->rupub->ZI[i-1], s);

    for (int k = 0; k < pub->rupub->m; k ++)
    {
        element_init_G1(prv->K_j[k], pub->p);
        if (k == j - 1)
            continue;

        element_pow_zn(prv->K_j[k], pub->rupub->fs[k], s);
    }

    for (int k = 0; k < attributes->len; k ++)
    {
        char* attr;
        element_t* kjx;

        attr = g_ptr_array_index(attributes, k);
        attr = strdup(attr);
        kjx = malloc(sizeof(element_t)*2);
        
        element_init_G1(*kjx, pub->p);
        element_init_G1(*(kjx + 1), pub->p);

        element_random(sx);

        element_pow_zn(*kjx, pub->g, sx);
        
        element_from_string(dx, attr);
        element_pow_zn(*(kjx + 1), pub->rupub->H, dx);

        element_mul(*(kjx + 1), *(kjx + 1), pub->rupub->h);

        element_neg(dx, s);
        element_pow2_zn(*(kjx + 1), 
            *(kjx + 1), sx, pub->rupub->G, dx);
        
        g_hash_table_insert(prv->Kijx, attr, kjx);
        g_ptr_array_add(prv->attributes, attr);
    }

    element_clear(sx);
    element_clear(dx);
    element_clear(s);
    return prv;
}

drur_prv_item_t*
drur_aakeygen(drur_auth_t* auth,
              drur_capub_t* pub,
              drur_user_cert_t* cert,
              GPtrArray* attributes)
{
    drur_prv_item_t* prv;
    element_t t;

    prv = malloc(sizeof(drur_prv_item_t));
    
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
        drur_prv_kx_t* kx;
        drur_attr_pk_t* attr_pk;
        char *attr;

        attr = g_ptr_array_index(attributes, i);

        if (g_hash_table_contains(prv->kx, attr))continue;
        if (!g_hash_table_contains(auth->attr_pks, attr))continue;
        
        attr = strdup(attr);
        attr_pk = g_hash_table_lookup(auth->attr_pks, attr);

        kx = malloc(sizeof(drur_prv_kx_t));

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
drur_fill_policy(bswabe_policy_t* p,
                 drur_capub_t* pub,
                 element_t s1,
                 element_t s2,
                 GPtrArray* params)
{
    int i;
    bswabe_polynomial_t* ws;
	element_t r;
	element_t t;
    element_t h;
	
	element_init_same_as(r, s1);
	element_init_same_as(t, s1);
    element_init_same_as(h, s1);

	p->q = rand_poly(p->k - 1, s1);
    ws = rand_poly(p->k - 1, s2);

	if( p->children->len == 0 )
	{
		GPtrArray* auth_pks;   
        GHashTable* auth_list;
        drur_auth_item_t* auth;
        drur_cph_item_t* item;
        drur_attr_pk_t* attr_pk;
        element_t ri, xi;
    
        auth_pks = g_ptr_array_index(params, 0);
        auth_list = g_ptr_array_index(params, 1);
    
        item = malloc(sizeof(drur_cph_item_t));
    
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
        element_init_G1(item->P0, pub->p);
        element_init_G1(item->P1, pub->p);
        element_init_G1(item->P2, pub->p);
        
        element_init_Zr(ri, pub->p);
        element_init_Zr(xi, pub->p);
        
        element_from_string(xi, p->attr);
        element_pow_zn(item->P1, pub->rupub->H, xi);
        element_mul(item->P1, item->P1, pub->rupub->h);

        element_random(ri);
        element_random(xi);

        element_pow2_zn(item->P0, pub->rupub->f, ws->coef[0],
            pub->rupub->G, xi);
        element_pow_zn(item->P2, pub->g, xi);
        element_neg(xi, xi);

        element_pow_zn(item->P1, item->P1, xi);
    
        element_pow2_zn(item->C3, pub->ga, p->q->coef[0], attr_pk->pk, ri);
        element_pow_zn(item->D2, auth->pks->g_beta_gamma, ri);
        element_neg(ri, ri);
        element_pow_zn(item->D1, auth->pks->g_beta_invert, ri);
    
        element_clear(ri);
        element_clear(xi);
    
        p->cph = item;
	}
	else
		for( i = 0; i < p->children->len; i++ )
		{
			element_set_si(r, i + 1);
			eval_poly(t, p->q, r);
            eval_poly(h, ws, r);
			drur_fill_policy(g_ptr_array_index(p->children, i), pub, t, h, params);
		}
	free_polynomial(p->q);
    free_polynomial(ws);
	element_clear(r);
	element_clear(t);
}

drur_cph_t*
drur_encrypt(drur_capub_t* pub,
             GPtrArray* attr_pks,
             element_t m,
             char* policy,
             GArray* revo_list,
             int userindex)
{
    drur_cph_t* cph;
    drur_rucph_t* ru_cph;
    GPtrArray* params;
    GHashTable* auth_list;
    GList* list;
    drur_auth_item_t* item;
    int i_min, j_min, sum;
    element_t kappa, tau, rx, 
        ry, rz, pi, c1, c2, c3, muls, s_back, npi, egg_alpha;
    bswabe_polynomial_t *s, *t, **wj, **vi, 
        *vc, *x1, *x2, *x3, *xmid, *cmid, *x3mid;

    getij(&i_min, &j_min, userindex, pub->rupub->m);
    i_min --;
    j_min --;

    element_init_Zr(tau, pub->p);
    element_init_Zr(rx, pub->p);
    element_init_Zr(ry, pub->p);
    element_init_Zr(rz, pub->p);
    element_init_Zr(pi, pub->p);
    element_init_Zr(s_back, pub->p);
    element_init_Zr(c1, pub->p);
    element_init_Zr(c2, pub->p);
    element_init_Zr(c3, pub->p);
    element_init_Zr(kappa, pub->p);
    element_init_Zr(npi, pub->p);

    element_init_G1(muls, pub->p);
    element_init_GT(m, pub->p);
    element_init_GT(egg_alpha, pub->p);

    element_random(m);

    element_random(rx);
    element_random(ry);
    element_random(rz);

    element_random(pi);
    element_random(npi);
    element_random(kappa);
    element_random(tau);

    wj = malloc(sizeof(bswabe_policy_t*)*pub->rupub->m);
    vi = malloc(sizeof(bswabe_policy_t*)*pub->rupub->m);

    x1 = vector_init_x1(rx, rz);
    x2 = vector_init_x2(ry, rz);
    x3 = vector_init_x3(rx, ry, rz);
    
    s = vector_init_random(pub->rupub->m, pub->p, Zr);
    t = vector_init_random(pub->rupub->m, pub->p, Zr);

    vc = random_vector_by_x1_x2_x3(x1, x2, x3, c1, c2, c3);

    for (int i = 0; i < pub->rupub->m; i ++)
        wj[i] = random_vector_by_x1_x2_x3(x1, x2, x3, c1, c2, c3);
    

    for (int i = 0; i < pub->rupub->m; i ++)
        if (i <= i_min)
            vi[i] = random_vector_by_x1_x2_x3(x1, x2, x3, c1, c2, c3);
        else
            vi[i] = random_vector_by_x1_x2(x1, x2, c1, c2);
    
    cph = malloc(sizeof(drur_cph_t));
    ru_cph = malloc(sizeof(drur_rucph_t));
    cph->ru_cph = ru_cph;
    ru_cph->m = pub->rupub->m;

    params = g_ptr_array_new();
    auth_list = g_hash_table_new(g_str_hash, g_str_equal);
    g_ptr_array_add(params, attr_pks);
    g_ptr_array_add(params, auth_list);
    cph->aids = g_ptr_array_new();
    cph->policy = strdup(policy);

    cph->p = parse_policy_postfix(cph->policy);
    
    element_init_G1(cph->C1, pub->p);
    
    cph->C2 = g_hash_table_new(g_str_hash, g_str_equal);
    
    drur_fill_policy(cph->p, pub, npi, pi, params);

    element_pow_zn(cph->C1, pub->g, npi);
    
    element_set1(egg_alpha);

    list = g_hash_table_get_values(auth_list);

    for (GList* p = list; p != NULL; p = p->next)
    {
        item = p->data; 
        element_t *C2;
        C2 = malloc(sizeof(element_t));

        element_init_G1(*C2, pub->p);
        g_ptr_array_add(cph->aids, strdup(item->aid));
        // printf("%s\n", item->aid);
        
        element_pow_zn(*C2, item->pks->g_beta_invert, npi);
        element_mul(egg_alpha, egg_alpha, item->pks->egg_alpha);
        g_hash_table_insert(cph->C2, item->aid, C2);
    }

    element_pow_zn(egg_alpha, egg_alpha, npi);

    ru_cph->revo_list = g_hash_table_new(g_int_hash, g_int_equal);
    ru_cph->revo_arr = g_array_copy(revo_list);
    
    for (int i = 0; i < ru_cph->revo_arr->len; i++)
        g_hash_table_add(ru_cph->revo_list, &(g_array_index(ru_cph->revo_arr, int, i)));
    
    ru_cph->R0 = malloc(sizeof(bswabe_polynomial_t*)*pub->rupub->m);
    ru_cph->R1 = malloc(sizeof(bswabe_polynomial_t*)*pub->rupub->m);
    ru_cph->Q0 = malloc(sizeof(element_t)*pub->rupub->m);
    ru_cph->Q1 = malloc(sizeof(element_t)*pub->rupub->m);
    ru_cph->Q2 = malloc(sizeof(element_t)*pub->rupub->m);
    ru_cph->T = malloc(sizeof(element_t)*pub->rupub->m);
    ru_cph->C0 = malloc(sizeof(bswabe_polynomial_t*)*pub->rupub->m);
    ru_cph->C1 = malloc(sizeof(bswabe_polynomial_t*)*pub->rupub->m);
    
    for (int i = 0; i < pub->rupub->m; i ++)
    {
        xmid = vector_duplicate(vi[i]);
        element_init_G1(ru_cph->Q0[i], pub->p);
        element_init_G1(ru_cph->Q1[i], pub->p);
        element_init_G1(ru_cph->Q2[i], pub->p);
        element_init_GT(ru_cph->T[i], pub->p);
        
        element_pow_zn(ru_cph->Q0[i], pub->g, s->coef[i]);

        element_set(ru_cph->Q1[i], pub->rupub->f);

        for (int j = 1; j <= pub->rupub->m; j ++)
            if ((sum = j + i * pub->rupub->m) 
                && !g_hash_table_contains(ru_cph->revo_list, &sum))

                element_mul(ru_cph->Q1[i], ru_cph->Q1[i], pub->rupub->fs[j - 1]);
        
        element_pow_zn(ru_cph->Q2[i], pub->g, t->coef[i]);

        if (i < i_min)
        {
            ru_cph->R0[i] = vector_pow_base_new(xmid, pub->g);
            vector_mul_element(xmid, xmid, kappa);

            ru_cph->R1[i] = vector_pow_base_new(xmid, pub->g);
            
            element_random(s_back);
            
            element_pow3_zn(ru_cph->Q1[i], ru_cph->Q1[i], s->coef[i],
                pub->rupub->ZI[i], t->coef[i], pub->rupub->f, pi);

            element_pow_zn(ru_cph->T[i], pub->rupub->EI[i], s_back);

        }
        else
        {
            vector_mul_element(xmid, xmid, s->coef[i]);
            ru_cph->R0[i] = 
                vector_pow_base_new(xmid, pub->rupub->GI[i]);
            
            vector_mul_element(xmid, xmid, kappa);

            ru_cph->R1[i] = 
                vector_pow_base_new(xmid, pub->rupub->GI[i]);
            
            vector_inner_mul_out(s_back, vi[i], vc);
            element_mul(s_back, s_back, tau);
            element_pow_zn(ru_cph->Q0[i], ru_cph->Q0[i], s_back);

            element_mul(s_back, s_back, s->coef[i]);            
            element_pow3_zn(ru_cph->Q1[i], ru_cph->Q1[i], s_back, 
                pub->rupub->ZI[i], t->coef[i], pub->rupub->f, pi);
            // 此处需进行改变
            element_pow_zn(ru_cph->T[i], pub->rupub->EI, s_back);
            element_mul(ru_cph->T[i], ru_cph->T[i], m);
        }
        element_mul(ru_cph->T[i], ru_cph->T[i], egg_alpha);
        free_polynomial(xmid);
    }

    for (int j = 0; j < pub->rupub->m; j ++)
    {
        xmid = vector_duplicate(wj[j]);

        vector_mul_element(xmid, xmid, kappa);
        cmid = vector_pow_base_new(xmid, pub->g);
        free_polynomial(xmid);
        
        xmid = vector_duplicate(vc);
        
        ru_cph->C1[j] = vector_pow_base_new(wj[j], pub->g);
        
        if (j < j_min)
        {
            element_random(s_back);
            x3mid = vector_duplicate(x3);
            vector_mul_element(x3mid, x3mid, s_back);
            vector_add_out(xmid, xmid, x3mid);
            free_polynomial(x3mid);
        }

        vector_mul_element(xmid, xmid, tau);

        ru_cph->C0[j] = vector_pow_base_new(xmid, pub->rupub->HJ[j]);
        vector_mul_out(ru_cph->C0[j], ru_cph->C0[j], cmid);

        free_polynomial(xmid);
        free_polynomial(cmid);
    }
    element_clear(rx);
    element_clear(ry);
    element_clear(rz);
    element_clear(pi);
    element_clear(s_back);
    element_clear(c1);
    element_clear(c2);
    element_clear(c3);
    element_clear(kappa);
    element_clear(muls);
    for (int i = 0; i < pub->rupub->m; i ++)
    {
        free_polynomial(wj[i]);
        free_polynomial(vi[i]);
    }
    free(wj);
    free(vi);

    free_polynomial(vc);
    free_polynomial(x1);
    free_polynomial(x2);
    free_polynomial(x3);
    free_polynomial(s);
    free_polynomial(t);

    return cph;
}

int
drur_dec_detail_aa(element_t r,
                element_t exp,
                bswabe_policy_t* p,
                drur_user_t* prv,
                drur_capub_t* pub)
{
    element_t s, t;
    drur_prv_item_t* prv_item;
    drur_cph_item_t* item;
    drur_prv_kx_t* kx;
    element_t *kjx;

    GList* list;
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
drur_dec_detail_ca(element_t r,
                element_t exp,
                bswabe_policy_t* p,
                drur_user_t* prv,
                drur_capub_t* pub)
{
    element_t s, t;
    drur_cph_item_t* item;
    element_t *kjx;

    GList* list;
    list = g_hash_table_get_values(prv->sks);

    element_init_GT(s, pub->p);
    element_init_GT(t, pub->p);

    item = p->cph;
    kjx = g_hash_table_lookup(prv->cask->Kijx, p->attr);
    element_pairing(s, prv->cask->K1, item->P0);
    // element_printf("%B\t%B\n", *kjx, item->P1);
    element_pairing(t, *kjx, item->P1);
    element_mul(s, s, t);
    
    element_pairing(t, *(kjx + 1), item->P2);
    element_mul(s, s, t);

    element_pow_zn(s, s, exp);
    element_mul(r, r, s);

    element_clear(s);
    element_clear(t);
    return 0;
}


int
drur_decrypt(drur_cph_t* cph,
             drur_capub_t* pub,
             drur_user_t* prv,
             element_t m)
{
    int res, ui, uj, sum;
    element_t s, t, dp, di, mul1, mul2, kij;
    drur_prv_item_t* prv_item;

    if (g_hash_table_contains(cph->ru_cph->revo_list, &prv->cert->id))
    {
        raise_error("you had been revoked");
        return 1;
    }
    
    getij(&ui, &uj, prv->cert->id, pub->rupub->m);

    res = 0;
    check_sat(cph->p, prv->attributes);
    if (!cph->p->satisfiable)
    {
        raise_error("your attributes can't satify the policy.");
        return 1;
    }
    pick_sat_min_leaves(cph->p);
    element_init_GT(dp, pub->p);
    element_init_GT(di, pub->p);
    element_init_GT(s, pub->p);
    element_init_GT(t, pub->p);
    element_init_GT(mul1, pub->p);
    element_init_GT(mul2, pub->p);
    element_init_GT(m, pub->p);

    element_init_G1(kij, pub->p);

    if (dec_flatten(s, cph->p, prv, pub, pub->p, drur_dec_detail_aa)
        || dec_flatten(dp, cph->p, prv, pub, pub->p, drur_dec_detail_ca))
    {
        res = 1;
        goto drur_destory;
    }

    element_invert(s, s);
    // element_pow_zn(egg_a_u_s, egg_a_u_s, prv->cert->u);
    
    element_set1(t);
    
    for (int i = 0; i < cph->aids->len; i ++)
    {
        char *aid;
        aid = g_ptr_array_index(cph->aids, i);

        prv_item = g_hash_table_lookup(prv->sks, aid);
        
        element_pairing(mul1, cph->C1, prv_item->K);
        element_pairing(mul2, prv_item->R,
            *((element_t*)g_hash_table_lookup(cph->C2, aid)));
        element_invert(mul2, mul2);
        element_mul(mul1, mul1, mul2);
        element_mul(mul1, mul1, s);
        element_mul(t, t, mul1);
    }

    element_pow_zn(t, t, prv->gsk);
    element_invert(t, t);

    element_set(kij, prv->cask->K0);
    for (int j = 0; j < pub->rupub->m; j ++)
        if (j + 1 != uj && 
            (sum = (ui-1)*pub->rupub->m + j + 1) && 
            !g_hash_table_contains(cph->ru_cph->revo_list, &sum))
            element_mul(kij, kij, prv->cask->K_j[j]);
    
    element_pairing(mul1, prv->cask->K2, cph->ru_cph->Q2[ui-1]);
    element_pairing(mul2, kij, cph->ru_cph->Q0[ui-1]);

    element_mul(s, mul1, mul2);

    element_pairing(mul1, prv->cask->K1, cph->ru_cph->Q1[ui-1]);
    
    element_invert(mul1, mul1);
    element_mul(s, s, mul1);
    vector_e_mul(mul1, cph->ru_cph->R1[ui - 1], cph->ru_cph->C1[uj - 1], pub->p);
    vector_e_mul(mul2, cph->ru_cph->R0[ui - 1], cph->ru_cph->C0[uj - 1], pub->p);
    
    element_div(mul1, mul1, mul2);
    element_mul(di, s, mul1);

    element_mul(di, di, dp);

    element_div(dp, cph->ru_cph->T[ui-1], di);
    element_mul(m, dp, t);
    
drur_destory:

    element_clear(s);
    element_clear(t);
    element_clear(mul1);
    element_clear(mul2);
    element_clear(di);
    element_clear(dp);
    element_clear(kij);

    return res;
}

void
drur_ukeygen(drur_auth_msk_t* auth_msk,
             drur_capub_t* pub,
             drur_attr_pk_t* attr_pks,
             drur_attr_msk_t* attr_msks,
             char* attribtue)
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
drur_secret_key_update(drur_prv_item_t* prv_item,
                       drur_capub_t* pub,
                       drur_user_cert_t* cert,
                       drur_attr_msk_t* attr_msk,
                       char *attribute)
{
    drur_prv_kx_t* kx;
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
drur_cph_update(drur_cph_t* cph,
                drur_capub_t* pub,
                element_t cuk,
                int version,
                char *attribute)
{
    GQueue* queue;
    bswabe_policy_t* cur;
    drur_cph_item_t* item;
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
                if (item->version < version)continue;
                item->version = version;
                // element_printf("%B\t%B\n", item->C3, cuk);
                element_pow_zn(D_CUK, item->D2, cuk);
                element_mul(item->C3, item->C3, D_CUK);
            }
    }
    element_clear(D_CUK);
    
    g_queue_clear(queue);
}

void
drur_realse_rupub(drur_rupub_t* pub)
{
    if (pub == NULL)return;
    element_clear(pub->h);
    element_clear(pub->G);
    element_clear(pub->H);
    element_clear(pub->f);
    for (int i = 0; i < pub->m; i ++)
    {
        element_clear(pub->fs[i]);
        element_clear(pub->EI[i]);
        element_clear(pub->GI[i]);
        element_clear(pub->ZI[i]);
        element_clear(pub->HJ[i]);
    }
    free(pub->fs);
    free(pub->EI);
    free(pub->GI);
    free(pub->ZI);
    free(pub->HJ);
    free(pub);
}

void 
drur_realse_capub(drur_capub_t* pub)
{
    if (pub == NULL)return;
    drur_realse_rupub(pub->rupub);
    free(pub->pairing_desc);
    element_clear(pub->g);
    element_clear(pub->ga);
    pairing_clear(pub->p);
    free(pub);
}

void
drur_realse_rumsk(drur_rumsk_t* msk)
{
    if (msk == NULL)return;
    for (int i = 0; i < msk->m; i ++)
    {
        element_clear(msk->alpha[i]);
        element_clear(msk->r[i]);
        element_clear(msk->c[i]);
    }
    free(msk->alpha);
    free(msk->r);
    free(msk->c);
    free(msk);
}

void
drur_realse_camsk(drur_camsk_t* msk)
{
    if (msk == NULL)return;
    element_clear(msk->a);
    drur_realse_rumsk(msk->rumsk);
    free(msk);
}

void
drur_realse_user_cert(drur_user_cert_t* cert)
{
    if (cert == NULL)return;
    element_clear(cert->u);
    element_clear(cert->g_z_invert);
    free(cert->uid);
    free(cert);
}

void
realse_element_arr(element_t * tar)
{
    if (tar == NULL)return;
    element_clear(tar[0]);
    free(tar);
}

void
realse_element_arr_by_size(element_t * tar, int n)
{
    if (tar == NULL)return;
    for (int i = 0; i < n; i ++)
        element_clear(tar[i]);
    free(tar);
}

void
realse_hash_table(GHashTable* hash, func f)
{
    if (hash == NULL)return;
    GList* list;
    list = g_hash_table_get_values(hash);
    for (GList* p = list; p != NULL; p = list)
    {
        list = p -> next;
        f(p->data);
    }
    g_list_free(list);
    g_hash_table_destroy(hash);
}

void
drur_realse_ru_sk(drur_ru_sk_t* sk)
{
    if (sk == NULL)return;
    realse_element_arr_by_size(sk->K_j, sk->m);
    element_clear(sk->K0);
    element_clear(sk->K1);
    element_clear(sk->K2);
    GList* list;
    list = g_hash_table_get_values(sk->Kijx);
    for (GList* p = list; p != NULL; p = p->next)
    {
        element_t *cur;
        cur = p->data;
        element_clear(cur[0]);
        element_clear(cur[1]);
        free(cur);
    }
    g_list_free(list);
    g_hash_table_destroy(sk->Kijx);
    for (int i = 0; i < sk->attributes->len; i ++)
        free(g_ptr_array_index(sk->attributes, i));
    g_ptr_array_free(sk->attributes, 1);
    free(sk);
}

void
drur_realse_prv_kx(drur_prv_kx_t* kx)
{
    if (kx == NULL)return;
    element_clear(kx->kx);
    free(kx);
}

void
drur_realse_prv_item(drur_prv_item_t* item)
{
    if (item == NULL)return;
    free(item->aid);
    element_clear(item->K);
    element_clear(item->L);
    element_clear(item->R);
    realse_hash_table(item->kx, drur_realse_prv_kx);
    for (int i = 0; i < item->attributes->len; i ++)
        free(g_ptr_array_index(item->attributes, i));
    g_ptr_array_free(item->attributes, TRUE);
    free(item);
}

void
drur_realse_user(drur_user_t* user)
{
    if (user == NULL)return;
    element_clear(user->gpk);
    element_clear(user->gsk);
    drur_realse_user_cert(user->cert);
    realse_hash_table(user->sks, drur_realse_prv_item);
    drur_realse_ru_sk(user->cask);
    for (int i = 0; i < user->attributes->len; i ++)
        free(g_ptr_array_index(user->attributes, i));
    for (int i = 0; i < user->aids->len; i ++)
        free(g_ptr_array_index(user->aids, i));
    g_ptr_array_free(user->attributes, TRUE);
    g_ptr_array_free(user->aids, TRUE);
    free(user);
}

void
drur_realse_auth_pub(drur_auth_pub_t* pub)
{
    if (pub == NULL)return;
    element_clear(pub->egg_alpha);
    element_clear(pub->g_beta_gamma);
    element_clear(pub->g_beta_invert);
    free(pub);
}

void 
drur_realse_auth_msk(drur_auth_msk_t* msk)
{
    if (msk == NULL)return;
    element_clear(msk->alpha);
    element_clear(msk->beta);
    element_clear(msk->gamma);
    free(msk);
}

void
drur_realse_attr_pk(drur_attr_pk_t* pk)
{
    if (pk == NULL)return;
    element_clear(pk->pk);
    free(pk);
}

void
drur_realse_attr_msk(drur_attr_msk_t* msk)
{
    if (msk == NULL)return;
    element_clear(msk->vx);
    element_clear(msk->auk);
    element_clear(msk->kuk);
    element_clear(msk->cuk);
    free(msk);
}

void
drur_realse_auth(drur_auth_t * auth)
{
    if (auth == NULL)return;
    free(auth->aid);
    drur_realse_auth_pub(auth->auth_pk);
    drur_realse_auth_msk(auth->auth_msk);
    realse_hash_table(auth->attr_pks, drur_realse_attr_pk);
    realse_hash_table(auth->attr_msks, drur_realse_attr_msk);
    for (int i = 0; i < auth->attributes->len; i ++)
        free(g_ptr_array_index(auth->attributes, i));
    g_ptr_array_free(auth->attributes, TRUE);
    free(auth);
}

void
drur_realse_auth_item(drur_auth_item_t* auth)
{
    if (auth == NULL)return;
    free(auth->aid);
    drur_realse_auth_pub(auth->pks);
    realse_hash_table(auth->attr_pks, drur_realse_attr_pk);
    for (int i = 0; i < auth->attributes->len; i ++)
        free(g_ptr_array_index(auth->attributes, i));
    g_ptr_array_free(auth->attributes, TRUE);
    free(auth);
}

void
drur_realse_cph_item(drur_cph_item_t* item)
{
    if (item == NULL)return;
    element_clear(item->C3);
    element_clear(item->D1);
    element_clear(item->D2);
    element_clear(item->P0);
    element_clear(item->P1);
    element_clear(item->P2);
    free(item);
}

void 
drur_realse_rucph(drur_rucph_t* cph)
{
    if (cph == NULL)return;
    for (int i = 0; i < cph->m; i ++)
    {
        free_polynomial(cph->R0[i]);
        free_polynomial(cph->R1[i]);
        free_polynomial(cph->C0[i]);
        free_polynomial(cph->C1[i]);
    }
    free(cph->R0);
    free(cph->R1);
    free(cph->C0);
    free(cph->C1);
    g_array_free(cph->revo_arr, TRUE);
    realse_element_arr_by_size(cph->Q0, cph->m);
    realse_element_arr_by_size(cph->Q1, cph->m);
    realse_element_arr_by_size(cph->Q2, cph->m);
    realse_element_arr_by_size(cph->T,  cph->m);
    g_hash_table_destroy(cph->revo_list);
    free(cph);
}

void
drur_realse_cph(drur_cph_t * cph)
{
    if (cph == NULL)return;
    element_clear(cph->C1);
    free(cph->policy);
    bswabe_policy_free(cph->p, drur_realse_cph_item);
    realse_hash_table(cph->C2, realse_element_arr);
    drur_realse_rucph(cph->ru_cph);
    for (int i = 0; i < cph->aids->len; i ++)
        free(g_ptr_array_index(cph->aids, i));
    g_ptr_array_free(cph->aids, TRUE);
}

