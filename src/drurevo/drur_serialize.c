/*
 * @Author: your name
 * @Date: 2020-12-26 20:52:39
 * @LastEditTime: 2021-02-24 20:08:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/drurevo/drur_seriealize.c
 */
#include "drurevo/serialize.h"

void
drur_serialize_rupub(GByteArray* b,
                     drur_rupub_t* pub)
{
    serialize_uint32(b, (u_int32_t)pub->m);
    serialize_uint32(b, (u_int32_t)pub->ctr);

    serialize_element(b, pub->h);
    serialize_element(b, pub->G);
    serialize_element(b, pub->H);
    serialize_element(b, pub->f);
    
    for (int i = 0; i < pub->m; i ++)
        serialize_element(b, pub->fs[i]);
    for (int i = 0; i < pub->m; i ++)
        serialize_element(b, pub->EI[i]);
    for (int i = 0; i < pub->m; i ++)
        serialize_element(b, pub->GI[i]);
    for (int i = 0; i < pub->m; i ++)
        serialize_element(b, pub->ZI[i]);
    for (int i = 0; i < pub->m; i ++)
        serialize_element(b, pub->HJ[i]);
}

drur_rupub_t*
drur_unserialize_rupub(drur_capub_t* pub,
                       GByteArray* b,
                       int *offset)
{
    int i;
    int n;
    drur_rupub_t* rupub;

    rupub = malloc(sizeof(drur_rupub_t));
    rupub->m = (int) unserialize_uint32(b, offset);
    rupub->ctr = (int) unserialize_uint32(b, offset);
    
    rupub->fs = malloc(sizeof(element_t)*rupub->m);
    rupub->EI = malloc(sizeof(element_t)*rupub->m);
    rupub->GI = malloc(sizeof(element_t)*rupub->m);
    rupub->ZI = malloc(sizeof(element_t)*rupub->m);
    rupub->HJ = malloc(sizeof(element_t)*rupub->m);

    element_init_G1(rupub->h, pub->p);
    element_init_G1(rupub->G, pub->p);
    element_init_G1(rupub->H, pub->p);
    element_init_G1(rupub->f, pub->p);

    for (int i = 0; i < rupub->m; i ++)
    {
        element_init_G1(rupub->fs[i], pub->p);
        element_init_G1(rupub->GI[i], pub->p);
        element_init_G1(rupub->ZI[i], pub->p);
        element_init_G1(rupub->HJ[i], pub->p);
        element_init_GT(rupub->EI[i], pub->p);
    }

    if (unserialize_element(b, offset, rupub->h))
        goto rupub_error;
    if (unserialize_element(b, offset, rupub->G))
        goto rupub_error;
    if (unserialize_element(b, offset, rupub->H))
        goto rupub_error;
    if (unserialize_element(b, offset, rupub->f))
        goto rupub_error;

    for (int i = 0; i < rupub->m; i ++)
        if (unserialize_element(b, offset, rupub->fs[i]))
            goto rupub_error;
    for (int i = 0; i < rupub->m; i ++)
        if (unserialize_element(b, offset, rupub->EI[i]))
            goto rupub_error;
    for (int i = 0; i < rupub->m; i ++)
        if (unserialize_element(b, offset, rupub->GI[i]))
            goto rupub_error;
    for (int i = 0; i < rupub->m; i ++)
        if (unserialize_element(b, offset, rupub->ZI[i]))
            goto rupub_error;
    for (int i = 0; i < rupub->m; i ++)
        if (unserialize_element(b, offset, rupub->HJ[i]))
            goto rupub_error;
    
    return rupub;

rupub_error:
    element_clear(rupub->h);
    element_clear(rupub->G);
    element_clear(rupub->H);
    element_clear(rupub->f);
    for (int i = 0; i < rupub->m; i ++)
    {
        element_clear(rupub->fs[i]);
        element_clear(rupub->GI[i]);
        element_clear(rupub->ZI[i]);
        element_clear(rupub->HJ[i]);
        element_clear(rupub->EI[i]);
    }
    free(rupub->fs);
    free(rupub->EI);
    free(rupub->GI);
    free(rupub->ZI);
    free(rupub->HJ);
    return NULL;
}

void
drur_serialize_capub(GByteArray* b, 
                     drur_capub_t* pub)
{
    serialize_string(b, pub->pairing_desc);
    serialize_element(b, pub->g);
    serialize_element(b, pub->ga);
    drur_serialize_rupub(b, pub->rupub);
}

drur_capub_t*
drur_unserialize_capub(GByteArray* b, int* offset)
{
    drur_capub_t* pub;
    
    pub = malloc(sizeof(drur_capub_t));

    pub->pairing_desc = unserialize_string(b, offset);
    if (pub->pairing_desc == NULL)
    {
        free(pub);
        return NULL;
    }
    pairing_init_set_buf(pub->p, 
        pub->pairing_desc, strlen(pub->pairing_desc));
    
    element_init_G1(pub->g, pub->p);
    element_init_G1(pub->ga, pub->p);
    if (unserialize_element(b, offset, pub->g) || 
        unserialize_element(b, offset, pub->ga))
        goto capub_error;
    pub->rupub = drur_unserialize_rupub(pub, b, offset);
    if (!pub->rupub)goto capub_error;
    return pub;
capub_error:
    free(pub->pairing_desc);
    element_clear(pub->g);
    element_clear(pub->ga);
    free(pub);
    return NULL;
}

void
drur_serialize_rumsk(GByteArray* b, drur_rumsk_t* msk)
{
    serialize_uint32(b, (uint32_t)msk->m);
    for (int i = 0; i < msk->m; i++)
        serialize_element(b, msk->alpha[i]);
    for (int i = 0; i < msk->m; i++)
        serialize_element(b, msk->r[i]);
    for (int i = 0; i < msk->m; i++)
        serialize_element(b, msk->c[i]);
}

drur_rumsk_t*
drur_unserialize_rumsk(drur_capub_t* pub,
                       GByteArray* b, 
                       int *offset)
{
    drur_rumsk_t* msk;
    msk = malloc(sizeof(drur_rumsk_t));

    msk->m = (int)unserialize_uint32(b, offset);

    msk->alpha = malloc(sizeof(element_t)*msk->m);
    msk->r = malloc(sizeof(element_t)*msk->m);
    msk->c = malloc(sizeof(element_t)*msk->m);

    for (int i = 0; i < msk->m; i ++)
    {
        element_init_Zr(msk->alpha[i], pub->p);
        element_init_Zr(msk->r[i], pub->p);
        element_init_Zr(msk->c[i], pub->p);
    }

    for (int i = 0; i < msk->m; i ++)
        if (unserialize_element(b, offset, msk->alpha[i]))
            goto rumsk_error;
    for (int i = 0; i < msk->m; i ++)
        if (unserialize_element(b, offset, msk->r[i]))
            goto rumsk_error;
    for (int i = 0; i < msk->m; i ++)
        if (unserialize_element(b, offset, msk->c[i]))
            goto rumsk_error;

    return msk;
rumsk_error:
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
    return NULL;
}

void
drur_serialize_camsk(GByteArray* b, drur_camsk_t* msk)
{
    serialize_element(b, msk->a);
    drur_serialize_rumsk(b, msk->rumsk);
}

drur_camsk_t*
drur_unserialize_camsk(drur_capub_t* pub,
                       GByteArray* b, 
                       int *offset)
{
    drur_camsk_t* msk;

    msk = malloc(sizeof(drur_camsk_t));

    element_init_Zr(msk->a, pub->p);

    unserialize_element(b, offset, msk->a);

    msk->rumsk = drur_unserialize_rumsk(pub, b, offset);

    return msk;
camsk_error:
    element_clear(msk->a);
    free(msk);
}

void
drur_serialize_cert(GByteArray* b,
                    drur_user_cert_t* cert)
{
    serialize_uint32(b, (uint32_t)cert->id);
    serialize_string(b, cert->uid);
    serialize_element(b, cert->u);
    serialize_element(b, cert->g_z_invert);
}

drur_user_cert_t*
drur_unserialize_cert(drur_capub_t* pub,
                      GByteArray* b, 
                      int *offset)
{
    drur_user_cert_t* cert;
    cert = malloc(sizeof(drur_user_cert_t));

    element_init_Zr(cert->u, pub->p); 
    element_init_G1(cert->g_z_invert, pub->p); 

    cert->id = unserialize_uint32(b, offset);
    cert->uid = unserialize_string(b, offset);
    if (!cert->uid) goto cert_error;

    if (unserialize_element(b, offset, cert->u) || 
        unserialize_element(b, offset, cert->g_z_invert))
    {
        free(cert->uid);
        goto cert_error;
    }
    return cert;
cert_error:
    element_clear(cert->u);
    element_clear(cert->g_z_invert);
    free(cert);
    return NULL;
}

void 
drur_serialize_prv_kx(GByteArray* b, drur_prv_kx_t* kx)
{
    serialize_uint32(b, (uint32_t)kx->version);
    serialize_element(b, kx->kx);
}

drur_prv_kx_t* 
drur_unserialize_prv_kx(drur_capub_t* pub,
                        GByteArray* b,
                        int *offset)
{
    drur_prv_kx_t* kx;
    kx = malloc(sizeof(drur_prv_kx_t));

    kx->version = unserialize_uint32(b, offset);
    element_init_G1(kx->kx, pub->p);
    if (unserialize_element(b, offset, kx->kx))
    {
        element_clear(kx->kx);
        free(kx);
        return NULL;   
    }
    return kx;
}

void
drur_serialize_ru_sk(GByteArray* b,
                     drur_ru_sk_t* sk)
{
    serialize_uint32(b, (uint32_t)sk->m);
    serialize_element(b, sk->K0);
    serialize_element(b, sk->K1);
    serialize_element(b, sk->K2);
    for (int i = 0; i < sk->m; i ++)
        serialize_element(b, sk->K_j[i]);
    serialize_uint32(b, (uint32_t)sk->attributes->len);
    for (int i = 0; i < sk->attributes->len; i ++)
    {
        char* attr;
        element_t *kijx;
        attr = g_ptr_array_index(sk->attributes, i);
        serialize_string(b, attr);
        kijx = g_hash_table_lookup(sk->Kijx, attr);
        serialize_element(b, kijx[0]);
        serialize_element(b, kijx[1]);     
    }
}

drur_ru_sk_t*
drur_unserialize_ru_sk(drur_capub_t* pub,
                       GByteArray* b,
                       int *offset)
{
    drur_ru_sk_t* sk;
    int len;
    sk = malloc(sizeof(drur_ru_sk_t));

    sk->m = unserialize_uint32(b, offset);
    element_init_G1(sk->K0, pub->p);
    element_init_G1(sk->K1, pub->p);
    element_init_G1(sk->K2, pub->p);
    sk->K_j = malloc(sizeof(element_t)*sk->m);

    sk->attributes = g_ptr_array_new();
    sk->Kijx = g_hash_table_new(g_str_hash, g_str_equal);
    for (int i = 0; i < sk->m; i ++)
        element_init_G1(sk->K_j[i], pub->p);

    if(unserialize_element(b, offset, sk->K0) ||
        unserialize_element(b, offset, sk->K1) ||
        unserialize_element(b, offset, sk->K2))
        goto ru_sk_destory;

    for (int i = 0; i < sk->m; i ++)
        element_init_G1(sk->K_j[i], pub->p);
    
    len = unserialize_uint32(b, offset);
    for (int i = 0; i < len; i ++)
    {
        char *attr;
        element_t* kijx;
        attr = unserialize_string(b, offset);
        kijx = malloc(sizeof(element_t)*2);
        element_init_G1(kijx[0], pub->p);
        element_init_G1(kijx[1], pub->p);
        
        unserialize_element(b, offset, kijx[0]);
        unserialize_element(b, offset, kijx[1]);

        g_ptr_array_add(sk->attributes, attr);
        g_hash_table_insert(sk->Kijx, attr, kijx);
    }
    return sk;
ru_sk_destory:
    
    element_clear(sk->K0);
    element_clear(sk->K1);
    element_clear(sk->K2);

    for (int i = 0; i < sk->m; i ++)
        element_clear(sk->K_j[i]);
    
    free(sk->K_j);
    for (int i = 0; i < sk->attributes->len; i ++)
    {
        char * p;
        element_t *kijx;

        p = g_ptr_array_index(sk->attributes, i);
        
        kijx = g_hash_table_lookup(sk->Kijx, p);
        
        element_clear(kijx[0]);
        element_clear(kijx[1]);

        free(kijx);
        free(p);
    }

    g_ptr_array_free(sk->attributes, 1);
    g_hash_table_destroy(sk->Kijx);
    
    free(sk);
    
    return NULL;
}

void
drur_serialize_prv_item(GByteArray* b, drur_prv_item_t* item)
{
    serialize_string(b, item->aid);
    serialize_element(b, item->K);
    serialize_element(b, item->L);
    serialize_element(b, item->R);
    serialize_uint32(b, (uint32_t)item->attributes->len);
    for (int i = 0; i < item->attributes->len; i ++)
    {
        char * attr;
        attr = g_ptr_array_index(item->attributes, i);
        serialize_string(b, attr);
        drur_serialize_prv_kx(b, g_hash_table_lookup(item->kx, attr));
    }
}

drur_prv_item_t*
drur_unserialize_prv_item(drur_capub_t* pub,
                          GByteArray* b,
                          int *offset)
{
    drur_prv_item_t* item;
    int len;

    item = malloc(sizeof(drur_prv_item_t));
    
    element_init_G1(item->K, pub->p);
    element_init_G1(item->L, pub->p);
    element_init_G1(item->R, pub->p);

    item->kx = g_hash_table_new(g_str_hash, g_str_equal);
    item->attributes = g_ptr_array_new();

    item->aid = unserialize_string(b, offset);
    if (item ->aid == NULL || 
        unserialize_element(b, offset, item->K) ||
        unserialize_element(b, offset, item->L) ||
        unserialize_element(b, offset, item->R))
    {
        goto prv_item_destory;
    }
    len = unserialize_uint32(b, offset);
    for (int i = 0; i < len; i ++)
    {
        drur_prv_kx_t* kx;
        char *attr;
        attr = unserialize_string(b, offset);
        if (attr == NULL)goto prv_item_destory;

        kx = drur_unserialize_prv_kx(pub, b, offset);
        if (kx == NULL)
        {
            free(attr);
            goto prv_item_destory;
        }
        // element_printf("C3:\t%B\n", kx->kx);
        g_ptr_array_add(item->attributes, attr);
        g_hash_table_insert(item->kx, attr, kx);
    }
    return item;
prv_item_destory:

    element_clear(item->K);
    element_clear(item->L);
    element_clear(item->R);
    if (item->aid != NULL)
        free(item->aid);

    for (int i = 0; i < item->attributes->len; i ++)
    {
        char *p;
        drur_prv_kx_t *kx;

        p = g_ptr_array_index(item->attributes, i);
        kx = g_hash_table_lookup(item->kx, p);
        drur_realse_prv_kx(kx);
        free(p);
    }
    g_ptr_array_free(item->attributes, 1);
    g_hash_table_destroy(item->kx);

    free(item);

    return NULL;
}


void
drur_serialize_user(GByteArray* b, drur_user_t* user)
{
    serialize_element(b, user->gpk);
    serialize_element(b, user->gsk);
    drur_serialize_cert(b, user->cert);
    drur_serialize_ru_sk(b, user->cask);
    serialize_uint32(b, (uint32_t)user->attributes->len);

    for (int i = 0; i < user->attributes->len; i ++)
    {
        char* attr;
        attr = g_ptr_array_index(user->attributes, i);
        serialize_string(b, attr);
    }
    serialize_uint32(b, (uint32_t)user->aids->len);
    for (int i = 0; i < user->aids->len; i ++)
    {
        char *aid;
        aid = g_ptr_array_index(user->aids, i);
        serialize_string(b, aid);
        drur_serialize_prv_item(b, g_hash_table_lookup(user->sks, aid));
    }
}

drur_user_t*
drur_unserialize_user(drur_capub_t* pub,
                      GByteArray* b,
                      int *offset)
{
    drur_user_t* user;
    int len;
    user = malloc(sizeof(drur_user_t));
    element_init_G1(user->gpk, pub->p);
    element_init_Zr(user->gsk, pub->p);
    user->attributes = g_ptr_array_new();
    user->aids = g_ptr_array_new();
    user->sks = g_hash_table_new(g_str_hash, g_str_equal);
    
    if (unserialize_element(b, offset, user->gpk) ||
        unserialize_element(b, offset, user->gsk))
        goto user_destory;
    
    user->cert = drur_unserialize_cert(pub, b, offset);

    if (user->cert == NULL) goto user_destory;

    user->cask = drur_unserialize_ru_sk(pub, b, offset);

    if (user->cask == NULL) 
    {
        drur_realse_user_cert(user->cert);
        goto user_destory;
    }

    len = unserialize_uint32(b, offset);
    for (int i = 0; i < len; i ++)
    {
        char *attr;
        attr = unserialize_string(b, offset);
        if (attr == NULL)
        {
            drur_realse_user_cert(user->cert);
            drur_realse_ru_sk(user->cask);
            goto user_destory;
        }
        g_ptr_array_add(user->attributes, attr);
    }
    len = unserialize_uint32(b, offset);
    for (int i = 0; i < len; i ++)
    {
        char *aid;
        drur_prv_item_t* item;
        aid = unserialize_string(b, offset);
        if (aid == NULL)
        {
            drur_realse_user_cert(user->cert);
            drur_realse_ru_sk(user->cask);
            goto user_destory;
        }
        item = drur_unserialize_prv_item(pub, b, offset);
        if (item == NULL)
        {
            drur_realse_user_cert(user->cert);
            drur_realse_ru_sk(user->cask);
            free(aid);
            goto user_destory;
        }
        g_hash_table_insert(user->sks, aid, item);
        g_ptr_array_add(user->aids, aid);
    }
    return user;
    
user_destory:

    element_clear(user->gpk);
    element_clear(user->gsk);
    
    for (int i = 0; i < user->attributes -> len; i ++)
    {
        char *p;
        p = g_ptr_array_index(user->attributes, i);
        free(p);
    }
    g_ptr_array_free(user->attributes, 1);
    
    for (int i = 0; i < user->aids; i ++)
    {
        char *aid;
        aid = g_ptr_array_index(user->aids, i);
        drur_realse_prv_item(
            g_hash_table_lookup(user->sks, aid));
        free(aid);
    }
    g_ptr_array_free(user->aids, 1);
    g_hash_table_destroy(user->sks);

    free(user);
    return NULL;
}

void
drur_serialize_auth_pub(GByteArray* b, drur_auth_pub_t* pub)
{
    serialize_element(b, pub->egg_alpha);
    serialize_element(b, pub->g_beta_invert);
    serialize_element(b, pub->g_beta_gamma);
}

drur_auth_pub_t*
drur_unserialize_auth_pub(drur_capub_t* pub,
                     GByteArray* b,
                     int *offset)
{
    drur_auth_pub_t* auth_pub;
    auth_pub = malloc(sizeof(drur_auth_pub_t));
    
    element_init_GT(auth_pub->egg_alpha, pub->p);
    element_init_G1(auth_pub->g_beta_invert, pub->p);
    element_init_G1(auth_pub->g_beta_gamma, pub->p);

    if (unserialize_element(b, offset, auth_pub->egg_alpha) ||
        unserialize_element(b, offset, auth_pub->g_beta_invert) ||
        unserialize_element(b, offset, auth_pub->g_beta_gamma))
        goto auth_pub_destory;
    return auth_pub;

auth_pub_destory:
    element_clear(auth_pub->egg_alpha);
    element_clear(auth_pub->g_beta_invert);
    element_clear(auth_pub->g_beta_gamma);
    free(auth_pub);
    return NULL;
}

void
drur_serialize_auth_msk(GByteArray* b, drur_auth_msk_t* msk)
{
    serialize_element(b, msk->alpha);
    serialize_element(b, msk->beta);
    serialize_element(b, msk->gamma);
}

drur_auth_msk_t*
drur_unserialize_auth_msk(drur_capub_t* pub,
                          GByteArray* b,
                          int *offset)
{
    drur_auth_msk_t* msk;
    msk = malloc(sizeof(drur_auth_msk_t));
    element_init_Zr(msk->alpha, pub->p);
    element_init_Zr(msk->beta, pub->p);
    element_init_Zr(msk->gamma, pub->p);

    if (unserialize_element(b, offset, msk->alpha) ||
        unserialize_element(b, offset, msk->beta) ||
        unserialize_element(b, offset, msk->gamma))
        goto auth_msk_destory;
    return msk;

auth_msk_destory:
    
    element_clear(msk->alpha);
    element_clear(msk->beta);
    element_clear(msk->gamma);
    free(msk);

    return NULL;
}

void
drur_serialize_attr_pk(GByteArray* b, drur_attr_pk_t* pk)
{
    serialize_uint32(b, (uint32_t)pk->version);
    serialize_element(b, pk->pk);
}

drur_attr_pk_t*
drur_unserialize_attr_pk(drur_capub_t* pub,
                         GByteArray* b,
                         int *offset)
{
    drur_attr_pk_t* pk;
    pk = malloc(sizeof(drur_attr_pk_t));
    pk->version = unserialize_uint32(b, offset);
    element_init_G1(pk->pk, pub->p);
    if (unserialize_element(b, offset, pk->pk))
        goto attr_pk_destory;
    return pk;
attr_pk_destory:

    element_clear(pk->pk);
    free(pk);

    return NULL;
}

void 
drur_seriallize_attr_msk(GByteArray* b, drur_attr_msk_t* msk)
{
    serialize_uint32(b, (uint32_t)msk->version);
    serialize_element(b, msk->vx);
    serialize_element(b, msk->auk);
    serialize_element(b, msk->kuk);
    serialize_element(b, msk->cuk);
}

drur_attr_msk_t*
drur_unserialize_attr_msk(drur_capub_t* pub,
                          GByteArray* b,
                          int *offset)
{
    drur_attr_msk_t* msk;
    msk = malloc(sizeof(drur_attr_msk_t));
    msk->version = unserialize_uint32(b, offset);
    element_init_Zr(msk->vx, pub->p);
    element_init_Zr(msk->auk, pub->p);
    element_init_Zr(msk->kuk, pub->p);
    element_init_Zr(msk->cuk, pub->p);

    if (unserialize_element(b, offset, msk->vx) ||
        unserialize_element(b, offset, msk->auk) ||
        unserialize_element(b, offset, msk->kuk) ||
        unserialize_element(b, offset, msk->cuk))
        goto attr_msk_destory;
    return msk;
attr_msk_destory:

    element_clear(msk->vx);
    element_clear(msk->auk);
    element_clear(msk->kuk);
    element_clear(msk->cuk);

    free(msk);

    return NULL;
}

void
drur_serialize_auth(GByteArray* b, drur_auth_t* auth)
{
    serialize_string(b, auth->aid);
    drur_serialize_auth_pub(b, auth->auth_pk);
    drur_serialize_auth_msk(b, auth->auth_msk);
    serialize_uint32(b, (uint32_t)auth->attributes->len);
    for (int i = 0; i < auth->attributes->len; i ++)
    {
        char *attr;
        drur_attr_pk_t* pk;
        drur_attr_msk_t* pmsk;
        attr = g_ptr_array_index(auth->attributes, i);
        serialize_string(b, attr);
        pk = g_hash_table_lookup(auth->attr_pks, attr);
        pmsk = g_hash_table_lookup(auth->attr_msks, attr);

        drur_serialize_attr_pk(b, pk);
        drur_seriallize_attr_msk(b, pmsk);
    }
}

drur_auth_t*
drur_unserialize_auth(drur_capub_t* pub,
                      GByteArray* b,
                      int *offset)
{
    drur_auth_t* auth;
    int len;
    auth = malloc(sizeof(drur_auth_t));
    auth->attributes = g_ptr_array_new();
    auth->attr_pks = g_hash_table_new(g_str_hash, g_str_equal);
    auth->attr_msks = g_hash_table_new(g_str_hash, g_str_equal);

    auth->aid = unserialize_string(b, offset);
    if (auth->aid == NULL) goto auth_destory;
    auth->auth_pk = drur_unserialize_auth_pub(pub, b, offset);
    if (auth->auth_pk == NULL)
    {
        free(auth->aid);
        goto auth_destory;
    }
    auth->auth_msk = drur_unserialize_auth_msk(pub, b, offset);
    if (auth->auth_msk == NULL)
    {
        free(auth->aid);
        drur_realse_auth_pub(auth->auth_pk);
        goto auth_destory;
    }

    len = unserialize_uint32(b, offset);
    for (int i = 0; i < len; i ++)
    {
        drur_attr_pk_t* pk;
        drur_attr_msk_t* pmsk;
        char *attr;
        attr = unserialize_string(b, offset);
        if (attr == NULL)
        {
            free(auth->aid);
            drur_realse_auth_pub(auth->auth_pk);
            drur_realse_auth_msk(auth->auth_msk);
            goto auth_destory;
        }
        pk = drur_unserialize_attr_pk(pub, b, offset);
        if (pk == NULL)
        {
            free(auth->aid);
            drur_realse_auth_pub(auth->auth_pk);
            drur_realse_auth_msk(auth->auth_msk);
            free(attr);
            goto auth_destory;
        }
        pmsk = drur_unserialize_attr_msk(pub, b, offset);
        if (pmsk == NULL)
        {
            free(auth->aid);
            drur_realse_auth_pub(auth->auth_pk);
            drur_realse_auth_msk(auth->auth_msk);
            free(attr);
            drur_realse_attr_pk(pk);
            goto auth_destory;
        }
        g_hash_table_insert(auth->attr_pks, attr, pk);
        g_hash_table_insert(auth->attr_msks, attr, pmsk);
        g_ptr_array_add(auth->attributes, attr);
    }

    return auth;
auth_destory:

    for (int i = 0; i < auth->attributes->len; i ++)
    {
        char * attr;
        drur_attr_msk_t* pmsk;
        drur_attr_pk_t* pk;

        attr = g_ptr_array_index(auth->attributes, i);
        
        pmsk = g_hash_table_lookup(auth->attr_msks, attr);
        pk = g_hash_table_lookup(auth->attr_pks, attr);

        drur_realse_attr_msk(pmsk);
        drur_realse_attr_pk(pk);
        free(attr);
    }

    g_ptr_array_free(auth->attributes, 1);
    g_hash_table_destroy(auth->attr_msks);
    g_hash_table_destroy(auth->attr_pks);

    free(auth);

    return NULL;
}

void
drur_serialize_auth_item(GByteArray* b, drur_auth_item_t* item)
{
    serialize_string(b, item->aid);
    drur_serialize_auth_pub(b, item->pks);
    serialize_uint32(b, (uint32_t)item->attributes->len);
    for (int i = 0; i < item->attributes->len; i ++)
    {
        char *attr;
        attr = g_ptr_array_index(item->attributes, i);
        serialize_string(b, attr);
        drur_serialize_attr_pk(b, g_hash_table_lookup(item->attr_pks, attr));
    }
}

drur_auth_item_t*
drur_unserialize_auth_item(drur_capub_t* pub,
                           GByteArray* b,
                           int *offset)
{
    drur_auth_item_t* item;
    int len;
    item = malloc(sizeof(drur_auth_item_t));

    item->attr_pks = g_hash_table_new(g_str_hash, g_str_equal);
    item->attributes = g_ptr_array_new();

    item->aid = unserialize_string(b, offset);
    if (item->aid == NULL)
        goto auth_item_destory;    
    item->pks = drur_unserialize_auth_pub(pub, b, offset);
    if (item->pks == NULL)
    {
        free(item->aid);
        goto auth_item_destory;
    }
    len = unserialize_uint32(b, offset);
    for (int i = 0; i < len; i ++)
    {
        char* attr;
        drur_attr_pk_t* pk;
        attr = unserialize_string(b, offset);
        if (attr == NULL)
        {
            free(item->aid);
            drur_realse_auth_pub(item->pks);
            goto auth_item_destory;
        }
        pk = drur_unserialize_attr_pk(pub, b, offset);
        if (pk == NULL)
        {
            free(item->aid);
            drur_realse_auth_pub(item->pks);
            drur_realse_attr_pk(pk);
            free(attr);
            goto auth_item_destory;
        }
        g_hash_table_insert(item->attr_pks, attr, pk);
        g_ptr_array_add(item->attributes, attr);
    }

    return item;
auth_item_destory:
    
    for (int i = 0; i < item->attributes->len; i ++)
    {
        char * attr;
        drur_attr_pk_t* pk;
        
        attr = g_ptr_array_index(item->attributes, i);
        pk = g_hash_table_lookup(item->attr_pks, attr);
        drur_realse_attr_pk(pk);
        free(attr);
    }

    g_hash_table_destroy(item->attr_pks);
    g_ptr_array_free(item->attributes, 1);

    free(item);
    
    return NULL;

}

void 
drur_serialize_cph_item(GByteArray* b, drur_cph_item_t* item)
{
    serialize_uint32(b, (uint32_t)item->version);
    serialize_element(b, item->C3);
    serialize_element(b, item->D1);
    serialize_element(b, item->D2);
    serialize_element(b, item->P0);
    serialize_element(b, item->P1);
    serialize_element(b, item->P2);
}

drur_cph_item_t*
drur_unserialize_cph_item(drur_capub_t* pub,
                          GByteArray* b,
                          int *offset)
{
    drur_cph_item_t* item;
    item = malloc(sizeof(drur_cph_item_t));
    item->version = unserialize_uint32(b, offset);
    element_init_G1(item->C3, pub->p);
    element_init_G1(item->D1, pub->p);
    element_init_G1(item->D2, pub->p);
    element_init_G1(item->P0, pub->p);
    element_init_G1(item->P1, pub->p);
    element_init_G1(item->P2, pub->p);

    if (unserialize_element(b, offset, item->C3) ||
        unserialize_element(b, offset, item->D1) ||
        unserialize_element(b, offset, item->D2) ||
        unserialize_element(b, offset, item->P0) ||
        unserialize_element(b, offset, item->P1) ||
        unserialize_element(b, offset, item->P2))
        goto cph_item_destory;
    return item;

cph_item_destory:

    element_clear(item->C3);
    element_clear(item->D1);
    element_clear(item->D2);
    element_clear(item->P0);
    element_clear(item->P1);
    element_clear(item->P2);
    free(item);

    return NULL;
}

void
drur_serialize_rucph(GByteArray* b, drur_rucph_t* cph)
{
    serialize_uint32(b, cph->m);
    for (int i = 0; i < cph->m; i++)
        serialize_polynomial(b, cph->R0[i]);
    for (int i = 0; i < cph->m; i++)
        serialize_polynomial(b, cph->R1[i]);
    
    for (int i = 0; i < cph->m; i ++)
        serialize_element(b, cph->Q0[i]);
    for (int i = 0; i < cph->m; i ++)
        serialize_element(b, cph->Q1[i]);
    for (int i = 0; i < cph->m; i ++)
        serialize_element(b, cph->Q2[i]);
    for (int i = 0; i < cph->m; i ++)
        serialize_element(b, cph->T[i]);

    for (int i = 0; i < cph->m; i++)
        serialize_polynomial(b, cph->C0[i]);
    for (int i = 0; i < cph->m; i++)
        serialize_polynomial(b, cph->C1[i]);

    serialize_uint32(b, (uint32_t)cph->revo_arr->len);

    for (int i = 0; i < cph->revo_arr->len; i ++)
        serialize_uint32(b, (uint32_t)g_array_index(cph->revo_arr, int, i));
}

drur_rucph_t*
drur_unserialize_rucph(drur_capub_t* pub,
                       GByteArray* b,
                       int *offset)
{
    drur_rucph_t* cph;
    int len;
    cph = malloc(sizeof(drur_rucph_t));
    cph->m = unserialize_uint32(b, offset);

    cph->R0 = malloc(sizeof(bswabe_polynomial_t*)*cph->m); 
    cph->R1 = malloc(sizeof(bswabe_polynomial_t*)*cph->m); 
    cph->C0 = malloc(sizeof(bswabe_polynomial_t*)*cph->m); 
    cph->C1 = malloc(sizeof(bswabe_polynomial_t*)*cph->m); 
    cph->Q0 = malloc(sizeof(element_t)*cph->m);
    cph->Q1 = malloc(sizeof(element_t)*cph->m);
    cph->Q2 = malloc(sizeof(element_t)*cph->m);
    cph->T = malloc(sizeof(element_t)*cph->m);

    cph->revo_arr = g_array_new(0, 1, sizeof(int));
    cph->revo_list = g_hash_table_new(g_str_hash, g_str_equal);

    for (int i = 0; i < cph -> m; i ++)
    {
        element_init_G1(cph->Q0[i], pub->p);
        element_init_G1(cph->Q1[i], pub->p);
        element_init_G1(cph->Q2[i], pub->p);
        element_init_GT(cph->T[i], pub->p);
    }

    for (int i = 0; i < cph->m; i ++)
        cph->R0[i] = unserialize_polynomial(pub->p, G1, b, offset);
    for (int i = 0; i < cph->m; i ++)
        cph->R1[i] = unserialize_polynomial(pub->p, G1, b, offset);
    

    for (int i = 0; i < cph->m; i ++)
        unserialize_element(b, offset, cph->Q0[i]);
    
    for (int i = 0; i < cph->m; i ++)
        unserialize_element(b, offset, cph->Q1[i]);
    
    for (int i = 0; i < cph->m; i ++)
        unserialize_element(b, offset, cph->Q2[i]);
    for (int i = 0; i < cph->m; i ++)
        unserialize_element(b, offset, cph->T[i]);

    for (int i = 0; i < cph->m; i ++)
        cph->C0[i] = unserialize_polynomial(pub->p, G1, b, offset);
    for (int i = 0; i < cph->m; i ++)
        cph->C1[i] = unserialize_polynomial(pub->p, G1, b, offset);

    len = unserialize_uint32(b, offset);
    for (int i = 0; i < len; i ++)
    {
        int c = unserialize_uint32(b, offset);
        g_array_append_val(cph->revo_arr, c);
        g_hash_table_add(cph->revo_list, &g_array_index(cph->revo_arr, int, i));
    }

    return cph;
rucph_destory:

    for (int i = 0; i < cph->m && cph->R0[i] != NULL; i ++)
        free_polynomial(cph->R0[i]);
    free(cph->R0);
    for (int i = 0; i < cph->m && cph->R1[i] != NULL; i ++)
        free_polynomial(cph->R1[i]);
    free(cph->R1);

    for (int i = 0; i < cph -> m; i ++)
    {
        element_clear(cph->Q0[i]);
        element_clear(cph->Q1[i]);
        element_clear(cph->Q2[i]);
        element_clear(cph->T[i]);
    }

    free(cph->Q0);
    free(cph->Q1);
    free(cph->Q2);
    free(cph->T);

    for (int i = 0; i < cph->m && cph->C0[i] != NULL; i ++)
        free_polynomial(cph->C0[i]);
    free(cph->C0);
    for (int i = 0; i < cph->m && cph->C1[i] != NULL; i ++)
        free_polynomial(cph->C1[i]);
    free(cph->C1);

    g_array_free(cph->revo_arr, 1);
    g_hash_table_destroy(cph->revo_list);

    free(cph);

    return NULL;
}

void
drur_serialize_cph(GByteArray* b, drur_cph_t* cph)
{
    serialize_element(b, cph->C1);
    serialize_policy(b, cph->p, drur_serialize_cph_item);
    serialize_string(b, cph->policy);
    drur_serialize_rucph(b, cph->ru_cph);
    
    serialize_uint32(b, (uint32_t)cph->aids->len);
    for (int i = 0; i < cph->aids->len; i ++)
    {
        char *aids;
        aids = g_ptr_array_index(cph->aids, i);
        serialize_string(b, aids);
        serialize_element(b, 
            *((element_t*)g_hash_table_lookup(cph->C2, aids)));
    }
}

drur_cph_t*
drur_unserialize_cph(drur_capub_t* pub,
                     GByteArray* b,
                     int *offset)
{
    drur_cph_t* cph;
    int len;
    cph = malloc(sizeof(drur_cph_t));
    cph->aids = g_ptr_array_new();
    element_init_G1(cph->C1, pub->p);
    cph->C2 = g_hash_table_new(g_str_hash, g_str_equal);
    
    unserialize_element(b, offset, cph->C1);
    cph->p = unserialize_policy(pub->p, 
        b, offset, drur_unserialize_cph_item);
    if (cph->p == NULL)
        goto cph_destory;
    cph->policy = unserialize_string(b, offset);
    if (cph->policy == NULL)
        goto cph_destory_policy;
    // printf("%s\n", cph->policy);
    cph->ru_cph = drur_unserialize_rucph(pub, b, offset);
    if (cph->ru_cph == NULL)
        goto cph_destory_policy_string;
    len = unserialize_uint32(b, offset);
    for (int i = 0; i < len; i ++)
    {
        char *aid;
        element_t *C2;
        C2 = malloc(sizeof(element_t));
        aid = unserialize_string(b, offset);
        // printf("%s\t", aid);
        element_init_G1(*C2, pub->p);
        // element_printf("%B\n", *C2);
        if(unserialize_element(b, offset, *C2))
        {
            free(aid);
            element_clear(*C2);
            free(C2);
            goto cph_destory_rucph;
        }
        g_hash_table_insert(cph->C2, aid, C2);
        g_ptr_array_add(cph->aids, aid);
    }

    return cph;
    
cph_destory_rucph:
    drur_realse_rucph(cph->ru_cph);
cph_destory_policy_string:
    free(cph->policy);
cph_destory_policy:
    bswabe_policy_free(cph->p, drur_realse_cph_item); 
cph_destory:

    element_clear(cph->C1);
    

    for (int i = 0; i < cph->aids->len; i ++)
    {
        char *aid;
        element_t * C2;

        aid = g_ptr_array_index(cph->aids, i);
        C2 = g_hash_table_lookup(cph->C2, aid);
        element_clear(*C2);
        free(C2);
        free(aid);
    }
    g_hash_table_destroy(cph->C2);
    g_ptr_array_free(cph->aids, 1);

    return NULL;
}