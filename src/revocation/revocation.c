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

/**
 * @description: generate the PP and MSK
 * @param {pub}
 * @param {msk}
 * @param {N}
 * @return
 */
void 
arcp_setup(arcp_pub_t **pub, 
           arcp_msk_t ** msk, 
           int N)
{
    element_t egg;
    element_t z;
    (*pub) = malloc(sizeof(arcp_pub_t));
    (*msk) = malloc(sizeof(arcp_msk_t));

    (*pub) -> pairing_desc = strdup(TYPE_A_PARAMS);
    (*pub) -> m = (int)sqrt((double)N);
    (*msk) -> m = (*pub) -> m;
    (*msk) -> ctr = 0;

    pairing_init_set_buf((*pub) -> p, 
        (*pub) -> pairing_desc, strlen((*pub) -> pairing_desc));
    
    element_init_G1((*pub) -> g, (*pub) -> p);
    element_init_G1((*pub) -> h, (*pub) -> p);
    element_init_G1((*pub) -> f, (*pub) -> p);
    element_init_G1((*pub) -> G, (*pub) -> p);
    element_init_G1((*pub) -> H, (*pub) -> p);
    element_init_GT(egg, (*pub) -> p);
    element_init_Zr(z, (*pub) -> p);

    element_random((*pub) -> g);
    element_random((*pub) -> h);
    element_random((*pub) -> f);
    element_random((*pub) -> G);
    element_random((*pub) -> H);

    pairing_apply(egg, (*pub) -> g, (*pub) -> g, (*pub) -> p);

    (*pub) -> egg_alpha = malloc(sizeof(element_t) * (*pub) -> m);
    (*pub) -> gr = malloc(sizeof(element_t) * (*pub) -> m);
    (*pub) -> gz = malloc(sizeof(element_t) * (*pub) -> m);
    (*pub) -> gc = malloc(sizeof(element_t) * (*pub) -> m);
    (*pub) -> fs = malloc(sizeof(element_t) * (*pub) -> m);
    (*msk) -> alpha = malloc(sizeof(element_t) * (*msk) -> m);
    (*msk) -> r = malloc(sizeof(element_t) * (*msk) -> m);
    (*msk) -> c = malloc(sizeof(element_t) * (*msk) -> m);

    for (int i = 0; i < (*pub) -> m; i++)
    {
        element_init_GT((*pub) -> egg_alpha[i], (*pub) -> p);

        element_init_G1((*pub) -> fs[i], (*pub) -> p);
        element_init_G1((*pub) -> gr[i], (*pub) -> p);
        element_init_G1((*pub) -> gz[i], (*pub) -> p);
        element_init_G1((*pub) -> gc[i], (*pub) -> p);

        element_init_Zr((*msk) -> alpha[i], (*pub) -> p);
        element_init_Zr((*msk) -> r[i], (*pub) -> p);
        element_init_Zr((*msk) -> c[i], (*pub) -> p);

        element_random(z);
        element_random((*msk) -> alpha[i]);
        element_random((*msk) -> r[i]);
        element_random((*msk) -> c[i]);
        element_random((*pub) -> fs[i]);

        element_pow_zn((*pub) -> egg_alpha[i], egg, (*msk) -> alpha[i]);
        element_pow_zn((*pub) -> gr[i], (*pub) -> g, (*msk) -> r[i]);
        element_pow_zn((*pub) -> gc[i], (*pub) -> g, (*msk) -> c[i]);
        element_pow_zn((*pub) -> gz[i], (*pub) -> g, z);

    }

    element_clear(egg);
    element_clear(z);
}

/**
 * @description: generate the private key of user
 * @param {pub} PP
 * @param {msk} MSK
 * @param {attributes} the attributes of user
 * @return {arcp_prv_t*} private key
 */
arcp_prv_t *
arcp_keygen(arcp_pub_t *pub,
            arcp_msk_t *msk,
            char **attributes)
{
    arcp_prv_t * prv;
    element_t delta_ij;
    element_t delta_ij_x;
    element_t mul;
    element_t attr_zr;
    prv = malloc(sizeof(arcp_prv_t));

    msk -> ctr ++;
    prv -> j = msk -> ctr % pub -> m;
    prv -> j = prv -> j == 0?pub -> m:prv->j;
    prv -> i = (msk -> ctr - prv -> j) / pub -> m + 1;

    element_init_Zr(delta_ij, pub -> p);
    element_init_Zr(delta_ij_x, pub -> p);
    element_init_Zr(attr_zr, pub -> p);

    element_init_G1(mul, pub -> p);
    element_init_G1(prv -> k0, pub -> p);
    element_init_G1(prv -> k1, pub -> p);
    element_init_G1(prv -> k2, pub -> p);

    prv -> attributes = g_ptr_array_new();
    element_random(delta_ij);

    // g ^ alpha_i
    element_pow_zn(prv -> k0, pub -> g, msk -> alpha[prv -> i - 1]);
    // G_i^c_j
    element_pow_zn(mul, pub -> gr[prv -> i - 1], msk -> c[prv -> j - 1]);
    // (g ^ alpha_i) * (G_i^c_j)
    element_mul(prv->k0, prv -> k0, mul);
    // f*f_j
    element_mul(mul, pub -> f, pub -> fs[prv -> j - 1]);
    // (f*f_j) ^ delta
    element_pow_zn(mul, mul, delta_ij);
    // ((g ^ alpha_i) * (G_i^c_j)) * ((f*f_j) ^ delta)
    element_mul(prv->k0, prv->k0, mul);

    // g^delta
    element_pow_zn(prv->k1, pub -> g, delta_ij);

    // Z_i ^ delta
    element_pow_zn(prv->k2, pub -> gz[prv -> i - 1], delta_ij);

    prv -> k3 = malloc(sizeof(element_t) * (pub -> m));
    prv -> ks = g_hash_table_new(g_str_hash, g_str_equal);

    for (int j = 0; j < pub -> m; j ++)
    {
        if (j + 1 == prv -> j)continue;

        element_init_G1(prv -> k3[j], pub -> p);
        // f_j' ^ delta
        element_pow_zn(prv -> k3[j], pub -> fs[j], delta_ij);
    }

    while(*attributes)
    {
        char *attr;
        arcp_prv_item_t *item;

        attr = strdup(*(attributes ++));
        g_ptr_array_add(prv -> attributes, attr);

        item = malloc(sizeof(arcp_prv_item_t));
        
        element_init_G1(item -> k4, pub -> p);
        element_init_G1(item -> k5, pub -> p);
        
        // delta_ij_x
        element_random(delta_ij_x);
        // rou(x)
        element_from_string(attr_zr, attr);

        //g^delta_ij_x
        element_pow_zn(item -> k4, pub -> g, delta_ij_x);

        // H ^ x
        element_pow_zn(item -> k5, pub -> H, attr_zr);
        // (H^x)*h
        element_mul(item->k5, item->k5, pub -> h);
        // ((H^x)*h)^delta_ij_x
        element_pow_zn(item -> k5, item -> k5, delta_ij_x);
        // element_invert(item -> k5, item -> k5);

        // G^delta
        element_pow_zn(mul, pub -> G, delta_ij);
        // (G^-delta)
        element_invert(mul, mul);
        // (((H^x)*h)^delta_ij_x) * (G^-delta)
        element_mul(item -> k5, item -> k5, mul);
        
        g_hash_table_insert(prv -> ks, attr, item);
    }
    element_clear(attr_zr);
    element_clear(delta_ij_x);
    element_clear(mul);
    element_clear(delta_ij);

    return prv;

}

void arcp_enc(bswabe_policy_t* p,
              arcp_pub_t *pub,
              arcp_cph_t* cph)
{
    arcp_cph_item *item;
    element_t xi, mul, pow;

    item = malloc(sizeof(arcp_cph_item));

    element_init_G1(item -> p0, pub -> p);
    element_init_G1(item -> p1, pub -> p);
    element_init_G1(item -> p2, pub -> p);
    element_init_G1(mul, pub -> p);
    
    element_init_Zr(xi, pub -> p);
    element_init_Zr(pow, pub -> p);

    element_from_string(pow, p -> attr);

    element_random(xi);

    element_pow_zn(item -> p2, pub -> g, xi);

    element_pow_zn(item -> p0, pub -> f, p -> q -> coef[0]);
    
    // element_printf("%B\n", item -> p0);
    element_pow_zn(mul, pub -> G, xi);
    element_mul(item -> p0, item -> p0, mul);

    element_pow_zn(item -> p1, pub -> H, pow);
    element_mul(item -> p1, item -> p1, pub -> h);
    element_neg(xi, xi);

    element_pow_zn(item -> p1, item -> p1, xi);

    p -> cph = item;
    element_clear(xi);
    element_clear(mul);
    element_clear(pow);
}

arcp_cph_t *
arcp_encrypt(arcp_pub_t *pub,
             element_t m,
             GArray * revo_list,
             char *policy,
             int user_index)
{
    arcp_cph_t * cph;
    element_t kappa, tau, rx, ry, rz, pi, c1, c2, c3, muls, s_back;
    bswabe_polynomial_t *s;
    bswabe_polynomial_t *t;
    bswabe_polynomial_t**wj;
    bswabe_polynomial_t**vi;
    bswabe_polynomial_t* vc;
    bswabe_polynomial_t* x1;
    bswabe_polynomial_t* x2;
    bswabe_polynomial_t* x3;
    bswabe_polynomial_t* xmid;
    bswabe_polynomial_t* cmid;
    bswabe_polynomial_t* x3mid;
    int i_min, j_min, sum;

    j_min = user_index % pub -> m;
    j_min = j_min == 0 ? pub -> m:j_min;
    i_min = (user_index - j_min)/ pub -> m;
    j_min --;
    
    /// random tau in Zr 
    element_init_Zr(tau, pub -> p);
    // rx
    element_init_Zr(rx, pub -> p);
    // ry
    element_init_Zr(ry, pub -> p);
    // rz
    element_init_Zr(rz, pub -> p);
    // pi
    element_init_Zr(pi, pub -> p);
    
    element_init_Zr(s_back, pub -> p);
    element_init_Zr(c1, pub -> p);
    element_init_Zr(c2, pub -> p);
    element_init_Zr(c3, pub -> p);
    // kappa
    element_init_Zr(kappa, pub -> p);

    element_init_G1(muls, pub -> p);
    element_init_GT(m, pub -> p);
    
    element_random(m);
    
    element_random(rx);
    element_random(ry);
    element_random(rz);

    element_random(pi);
    element_random(kappa);
    element_random(tau);

    wj = malloc(sizeof(bswabe_polynomial_t*) * pub -> m);
    vi = malloc(sizeof(bswabe_polynomial_t*) * pub -> m);

    x1 = vector_init_x1(rx, rz);
    x2 = vector_init_x2(ry, rz);
    x3 = vector_init_x3(rx, ry, rz);
    s = vector_init_random(pub -> m, pub -> p, Zr);
    t = vector_init_random(pub -> m, pub -> p, Zr);
    
    // vc = vector_init_random(3, pub -> p, Zr);
    {
        bswabe_polynomial_t *m1;
        bswabe_polynomial_t *m2;
        bswabe_polynomial_t *m3;
        m1 = vector_duplicate(x1);
        m2 = vector_duplicate(x2);
        m3 = vector_duplicate(x3);
        element_random(c1);
        element_random(c2);
        element_random(c3);
        vector_mul_element(m1, m1, c1);
        vector_mul_element(m2, m2, c2);
        vector_mul_element(m3, m3, c3);
        vector_add_out(m1, m1, m2);
        vector_add_out(m1, m1, m3);
        vc = m1;
        free_polynomial(m2);
        free_polynomial(m3);
    }

    for (int i = 0; i < pub -> m; i++)
    {
        bswabe_polynomial_t *m1;
        bswabe_polynomial_t *m2;
        bswabe_polynomial_t *m3;
        m1 = vector_duplicate(x1);
        m2 = vector_duplicate(x2);
        m3 = vector_duplicate(x3);
        element_random(c1);
        element_random(c2);
        element_random(c3);
        vector_mul_element(m1, m1, c1);
        vector_mul_element(m2, m2, c2);
        vector_mul_element(m3, m3, c3);
        vector_add_out(m1, m1, m2);
        vector_add_out(m1, m1, m3);
        wj[i] = m1;
        free_polynomial(m2);
        free_polynomial(m3);

    }
        // wj[i] = vector_init_random(3, pub -> p, Zr);
    
    for (int i = 0; i <= i_min; i ++)
        //vi[i] = vector_init_random(3, pub -> p, Zr);
    {
        bswabe_polynomial_t *m1;
        bswabe_polynomial_t *m2;
        bswabe_polynomial_t *m3;
        m1 = vector_duplicate(x1);
        m2 = vector_duplicate(x2);
        m3 = vector_duplicate(x3);
        element_random(c1);
        element_random(c2);
        element_random(c3);
        vector_mul_element(m1, m1, c1);
        vector_mul_element(m2, m2, c2);
        vector_mul_element(m3, m3, c3);
        vector_add_out(m1, m1, m2);
        vector_add_out(m1, m1, m3);
        vi[i] = m1;
        free_polynomial(m2);
        free_polynomial(m3);
    }

    for (int i = i_min + 1; i < pub -> m; i ++)
    {
        bswabe_polynomial_t* m1;
        bswabe_polynomial_t* m2;
        m1 = vector_duplicate(x1);
        m2 = vector_duplicate(x2);
        element_random(c1);
        element_random(c2); 
        vector_mul_element(m1, m1, c1);
        vector_mul_element(m2, m2, c2);
        vector_add_out(m1, m1, m2);
        free_polynomial(m2);
        vi[i] = m1;
    }

    cph = malloc(sizeof(arcp_cph_t));
    // g_array_sort(revo_list, g_int_);

    cph -> revo_list = g_hash_table_new(g_int_hash, g_int_equal);

    for (int i = 0; i < revo_list -> len; i++)
        g_hash_table_add(cph -> revo_list, &g_array_index(revo_list, int, i));

    cph -> r0 = malloc(sizeof(bswabe_polynomial_t*) * pub -> m);
    cph -> r1 = malloc(sizeof(bswabe_polynomial_t*) * pub -> m);
    cph -> q0 = malloc(sizeof(element_t) * pub -> m);
    cph -> q1 = malloc(sizeof(element_t) * pub -> m);
    cph -> q2 = malloc(sizeof(element_t) * pub -> m);
    cph -> t = malloc(sizeof(element_t) * pub -> m);
    cph -> c0 = malloc(sizeof(bswabe_polynomial_t *) * pub -> m);
    cph -> c1 = malloc(sizeof(bswabe_polynomial_t *) * pub -> m);

    cph -> policy = parse_policy_postfix(policy);
    for (int i = 0; i < pub -> m; i++)
    {
        xmid = vector_duplicate(vi[i]);
        element_init_G1(cph -> q0[i], pub -> p);
        element_init_G1(cph -> q1[i], pub -> p);
        element_init_G1(cph -> q2[i], pub -> p);
        element_init_GT(cph -> t[i], pub -> p);
        // g^s_i
        element_pow_zn(cph -> q0[i], pub -> g, s->coef[i]);
        // f
        element_set(cph -> q1[i], pub -> f);
        
        for(int j = 1; j <= pub -> m; j ++)
            if ((sum = j + i * pub -> m) && !g_hash_table_contains(cph -> revo_list, &sum))
                // f*f_j
                element_mul(cph -> q1[i], cph -> q1[i], pub -> fs[j - 1]);
        // (f * f_j) ^ s_i
        element_pow_zn(cph -> q1[i], cph -> q1[i], s -> coef[i]);
        // Z_i ^ t_i
        element_pow_zn(muls, pub -> gz[i], t -> coef[i]);
        // g^t_i
        element_pow_zn(cph -> q2[i], pub -> g, t -> coef[i]);

        if (i < i_min)
        {
            // g ^ vector vi
            cph -> r0[i] = vector_pow_base_new(xmid, pub -> g);
            // kappa * vector vi
            vector_mul_element(xmid, xmid, kappa);
            // g ^ (kappa * vector vi)
            cph -> r1[i] = vector_pow_base_new(xmid, pub -> g);

            element_random(s_back);
            // E_i ^ s-_i
            element_pow_zn(cph -> t[i], pub -> egg_alpha[i], s_back);
        }
        else
        {
            // vi ^ s_i
            vector_mul_element(xmid, xmid, s -> coef[i]);
            // G_i ^ (vi ^ s_i)
            cph -> r0[i] = vector_pow_base_new(xmid, pub -> gr[i]);
            // kappa ^ (vi ^ s_i)
            vector_mul_element(xmid, xmid, kappa);
            // G_i ^ (kappa ^ (vi ^ s_i))
            cph -> r1[i] = vector_pow_base_new(xmid, pub -> gr[i]);
            
            //  vi . vc
            vector_inner_mul_out(s_back, vi[i], vc);
            // s_back = (vi[i] * vc) * tau
            element_mul(s_back, s_back , tau);
            // (g^s_i) ^ ((vi[i] * vc) * tau)
            element_pow_zn(cph -> q0[i], cph -> q0[i], s_back);

            // ((f * f_j) ^ s_i) ^ ((vi[i] * vc) * tau)
            element_pow_zn(cph -> q1[i], cph -> q1[i], s_back);
            // ((vi[i] * vc) * tau) * s_i
            element_mul(s_back, s_back, s -> coef[i]);
            element_pow_zn(cph -> t[i], pub -> egg_alpha[i], s_back);
            element_mul(cph -> t[i], cph -> t[i], m);
        }
        // 
        element_mul(cph -> q1[i], cph -> q1[i], muls);
        element_pow_zn(muls, pub -> f, pi);
        element_mul(cph -> q1[i], cph -> q1[i], muls);
        free_polynomial(xmid);
    }
    
    for (int j = 0; j < pub -> m; j ++)
    {
        xmid = vector_duplicate(wj[j]);
        vector_mul_element(xmid, xmid, kappa);
        cmid = vector_pow_base_new(xmid, pub -> g);
        free_polynomial(xmid);
        
        xmid = vector_duplicate(vc);

        cph -> c1[j] = vector_pow_base_new(wj[j], pub -> g);

        if (j < j_min)
        {
            element_random(s_back);
            x3mid = vector_duplicate(x3);
            vector_mul_element(x3mid, x3mid, s_back);
            vector_add_out(xmid, xmid, x3mid);
            free_polynomial(x3mid);
        }
        vector_mul_element(xmid, xmid, tau);
        cph -> c0[j] = vector_pow_base_new(xmid, pub -> gc[j]);
        vector_mul_out(cph -> c0[j], cph -> c0[j], cmid);

        free_polynomial(xmid);
        free_polynomial(cmid);
    }
    
    fill_policy(cph -> policy, pub, pi, cph, arcp_enc);

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
    for (int i = 0; i < pub -> m; i ++)
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

    // element_printf("%B\n", cph -> q0[0]);
    return cph;
}

void arcp_dec(element_t r,
              element_t exp,
              bswabe_policy_t* p,
              arcp_prv_t* prv,
              arcp_pub_t* pub)
{
    element_t s, t;
    arcp_cph_item* item;
    arcp_prv_item_t *prv_item;

    item = p -> cph;
    prv_item = g_hash_table_lookup(prv->ks, p -> attr);

    element_init_GT(s, pub -> p);
    element_init_GT(t, pub -> p);

    pairing_apply(s, prv -> k1, item -> p0, pub -> p);
    pairing_apply(t, prv_item -> k4, item -> p1, pub -> p);
    element_mul(s, s, t);
    pairing_apply(t, prv_item -> k5, item -> p2, pub -> p);
    element_mul(s, s, t);
    element_pow_zn(s, s, exp);
    element_mul(r, r, s);

    element_clear(s);
    element_clear(t);

}

int
arcp_decrypt(arcp_pub_t *pub, 
             arcp_cph_t *cph, 
             arcp_prv_t* prv, 
             element_t m)
{
    int res = 0;
    int sum;
    sum = (prv -> i - 1) * pub -> m + prv -> j;

    element_t dp;
    element_t di;
    element_t s, t, kij;

    if (g_hash_table_contains(cph -> revo_list, &sum))
    {
        raise_error("you had been revoked");
        return 1;
    }

    check_sat(cph -> policy, prv -> attributes);

    if (!cph -> policy -> satisfiable)
    {
        raise_error("your attributes can staify the cph.");
        return 1;
    }

    pick_sat_min_leaves(cph -> policy);

    element_init_GT(dp, pub -> p);
    element_init_GT(di, pub -> p);
    element_init_GT(s, pub -> p);
    element_init_GT(t, pub -> p);
    element_init_G1(kij, pub -> p);
    element_init_GT(m, pub -> p);

    dec_flatten(dp, cph -> policy, prv, pub, pub -> p, arcp_dec);
    
    element_set(kij, prv -> k0);
    for (int j = 0; j < pub -> m; j ++)
        if ((j + 1 != prv -> j) && 
            (sum = (prv -> i - 1) * (pub -> m) + j + 1) && 
            !g_hash_table_contains(cph -> revo_list, &sum))
            element_mul(kij, kij, prv -> k3[j]);
    // e(k''_ij, q''_i)
    pairing_apply(s, prv -> k2, cph -> q2[prv -> i - 1], pub -> p);
    // e(k-_ij, q_i)
    pairing_apply(t, kij, cph -> q0[prv -> i - 1], pub -> p);
    // e(k''_ij, q''_i)*e(k-_ij, q_i)
    element_mul(s, s, t);
    // e(k'_ij, Q'_i)
    pairing_apply(t, prv -> k1, cph -> q1[prv -> i - 1], pub -> p);
    // 1 / e(k'_ij, Q'_i)
    element_invert(t, t);
    //  e(k''_ij, q''_i)*e(k-_ij, q_i) / e(k'_ij, Q'_i)
    element_mul(s, s, t);
    // element_set1(s);
    // e_3(R'_i, C'_j)
    vector_e_mul(t, cph -> r1[prv -> i - 1], cph -> c1[prv -> j - 1], pub -> p);

    // (e(k''_ij, q''_i)*e(k-_ij, q_i) / e(k'_ij, Q'_i)) * e_3(R'_i, C'_j)
    element_mul(s, s, t);
    // e_3(R_i, C_j)
    vector_e_mul(t, cph -> r0[prv -> i - 1], cph -> c0[prv -> j - 1], pub -> p);
    // 1 / e_3(R_i, C_j)
    element_invert(t,t);
    // (e(k''_ij, q''_i)*e(k-_ij, q_i) / e(k'_ij, Q'_i)) * e_3(R'_i, C'_j) / e_3(R_i, C_j)
    element_mul(di, s, t);

    element_mul(dp, dp, di);
    element_div(m, cph -> t[prv -> i - 1], dp);

    element_clear(dp);
    element_clear(di);
    element_clear(s);
    element_clear(t);
    element_clear(kij);

arcp_destory_dec:

    return res;
}