/*
 * @Author: your name
 * @Date: 2020-12-31 15:19:45
 * @LastEditTime: 2021-01-13 21:35:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /thrift-example/authority.c
 */
#include "user.h"


G_DEFINE_TYPE (MainUserAuthorityHandler,
              main_user_authority_handler,
              USER_TYPE_USER_HANDLER)

static void
main_user_authority_handler_finalize(GObject* object)
{
    MainUserAuthorityHandler *self = 
        MAIN_USER_AUTHORITY_HANDLER(object);
    
    g_hash_table_unref(self->log);
    
    self->log = NULL;

    G_OBJECT_CLASS (main_user_authority_handler_parent_class)->
        finalize(object);
}

static void
main_user_authority_handler_init(MainUserAuthorityHandler * self)
{
    self->log = g_hash_table_new_full(g_int_hash,
                                      g_int_equal, 
                                      g_free, 
                                      g_object_unref);
}

static void
main_user_authority_handler_class_init(MainUserAuthorityHandlerClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    userUserHandlerClass *user_class =
        USER_USER_HANDLER_CLASS(klass);
    
    gobject_class->finalize = main_user_authority_handler_finalize;

    user_class->encrypt = main_user_authority_handler_encrypt;
    user_class->decrypt = main_user_authority_handler_decrypt;
}

static drur_auth_item_t* 
transfer(userAuthPK* apk, drur_capub_t *pub)
{
    drur_auth_item_t *item;
    int offset;

    item = malloc(sizeof(drur_auth_item_t));
    
    item->aid = strdup(apk->aid);
    offset = 0;
    item->pks = 
        drur_unserialize_auth_pub(pub, apk->pk, &offset);
    item->attributes = g_ptr_array_new();
    item->attr_pks = 
        g_hash_table_new(g_str_hash, g_str_equal);
    for (int i = 0; i < apk->attributes->len; i ++)
    {
        char *attr;
        attr = 
            g_ptr_array_index(apk->attributes, i);
        
        attr = strdup(attr);
        offset = 0;
        g_hash_table_insert(item->attr_pks, 
            attr, drur_unserialize_attr_pk(pub, 
            g_hash_table_lookup(apk->attr_pks, attr), &offset));
        
        g_ptr_array_add(item->attributes, attr);
    }

    return item;
}

static gboolean
main_user_authority_handler_encrypt(userUserIf   *iface,
                                    GByteArray   **_return,
                                    GByteArray   *pub,
                                    GPtrArray    *attr_pks,
                                    gchar        *policy,
                                    GArray    *revo_list,
                                    const gint32 uindex,
                                    GByteArray   *mes,
                                    GError       **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);

    drur_capub_t *spub;
    GPtrArray   *apks;
    drur_cph_t  *cph;
    element_t m;
    int offset;

    *_return = 
        g_byte_array_new();

    offset = 0;
    spub = drur_unserialize_capub(pub, &offset);

    apks = g_ptr_array_new();
    for (int i = 0; i < attr_pks->len; i ++)
        g_ptr_array_add(apks, transfer(
            g_ptr_array_index(attr_pks, i), spub));

    char *policy_parse;
    policy_parse = parse_policy_lang(policy);
    cph = drur_encrypt(spub, 
        apks, m, policy_parse, revo_list, uindex);
    free(policy_parse);
    
    GByteArray  *cph_data;
    cph_data = aes_128_cbc_encrypt(mes, m);
    GByteArray *cph_byte;

    cph_byte = g_byte_array_new();
    drur_serialize_cph(cph_byte, cph);
    g_byte_array_append(*_return, 
        cph_byte->data, cph_byte->len);
    g_byte_array_free(cph_byte, 1);

    g_byte_array_append(*_return, 
        cph_data->data, cph_data->len);
    g_byte_array_free(cph_data, 1);

    for (int i = 0; i < apks->len; i ++)
        drur_realse_auth_item(
            g_ptr_array_index(apks, i));
    g_ptr_array_free(apks, 0);
    element_clear(m);

    drur_realse_cph(cph);
    drur_realse_capub(spub);
    
    g_message("user encryption!");
    return TRUE;
    
}

static drur_prv_item_t*
transferItem(drur_capub_t *pub, aaARevoKey *key)
{
    drur_prv_item_t *item;
    GList* l;
    int offset;
    item = malloc(sizeof(drur_prv_item_t));

    item->aid = strdup(key->aid);
    item->attributes = g_ptr_array_new();
    for (int i = 0; i < key->attributes->len; i ++)
        g_ptr_array_add(item->attributes, 
            strdup(g_ptr_array_index(key->attributes, i)));
    
    element_init_G1(item->K, pub->p);
    element_init_G1(item->L, pub->p);
    element_init_G1(item->R, pub->p);

    offset = 0;
    unserialize_element(key->K, &offset, item->K);
    offset = 0;
    unserialize_element(key->L, &offset, item->L);
    offset = 0;
    unserialize_element(key->R, &offset, item->R);

    item->kx = 
        g_hash_table_new(g_str_hash, g_str_equal);
    item->attributes = g_ptr_array_new();
    l = g_hash_table_get_keys(key->kx);
    for (GList* p = l; p != NULL; p = p->next)
    {
        offset = 0;
        g_hash_table_insert(item->kx, 
            p->data, drur_unserialize_prv_kx(pub, 
            g_hash_table_lookup(key->kx, p->data), &offset));
        
        g_ptr_array_add(
            item->attributes, strdup(p->data));
    }
    g_list_free(l);
    return item;
}

static GHashTable*
transferSK(drur_capub_t *pub, GHashTable *sks)
{
    GHashTable *res;
    GList      *l;
    drur_prv_item_t *sk;
    int offset;

    res = g_hash_table_new(g_str_hash, g_str_equal);

    l = g_hash_table_get_keys(sks);
    
    for (GList *p = l; p != NULL; p = p->next)
    {
        aaARevoKey* key = 
            g_hash_table_lookup(sks, p->data);
        g_hash_table_insert(res, 
            p->data, transferItem(pub, key));
    }
    g_list_free(l);
    return res;
}

static drur_ru_sk_t*
transferRUSK(drur_capub_t *pub, caURevoKey* key)
{
    drur_ru_sk_t *sk;
    int offset;

    sk = malloc(sizeof(drur_ru_sk_t));
    sk->m = key->m;
    element_init_G1(sk->K0, pub->p);
    element_init_G1(sk->K1, pub->p);
    element_init_G1(sk->K2, pub->p);
    sk->K_j = malloc(sizeof(element_t)*sk->m);
    
    offset = 0;
    unserialize_element(key->K0, &offset, sk->K0);
    offset = 0;
    unserialize_element(key->K1, &offset, sk->K1);
    offset = 0;
    unserialize_element(key->K2, &offset, sk->K2);

    offset = 0;
    for (int i = 0; i < sk->m; i ++)
    {
        element_init_G1(sk->K_j[i], pub->p);
        unserialize_element(key->KJ, &offset, sk->K_j[i]);
    }

    GList* l;
    l = g_hash_table_get_keys(key->Kijx);
    sk->attributes = g_ptr_array_new();
    sk->Kijx = 
        g_hash_table_new(g_str_hash, g_str_equal);
    for (GList *p = l; p != NULL; p = p->next)
    {
        element_t *kx;
        GByteArray *b;
        kx = malloc(sizeof(element_t)*2);
        element_init_G1(kx[0], pub->p);
        element_init_G1(kx[1], pub->p);
        b = g_hash_table_lookup(key->Kijx, p->data);
        offset = 0;
        unserialize_element(b, &offset, kx[0]);
        unserialize_element(b, &offset, kx[1]);
        // element_printf("%s:\t%B\t%B\n", p->data, kx[0], kx[1]);

        g_hash_table_insert(sk->Kijx, p->data, kx);
        g_ptr_array_add(sk->attributes, strdup(p->data));
    }
    return sk;
}

static gboolean
main_user_authority_handler_decrypt(userUserIf     *iface,
                                    GPtrArray      **_return,
                                    GByteArray     *cph,
                                    GByteArray     *pub,
                                    userUserPrvKey *prv,
                                    GError         **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);
    int offset, res;
    drur_cph_t *scph;
    drur_capub_t *spub;
    drur_user_t  *sprv;
    GByteArray *data;
    element_t m;

    res = TRUE;
    data = g_byte_array_new();

    offset = 0;
    spub = drur_unserialize_capub(pub, &offset);

    offset = 0;
    scph = drur_unserialize_cph(spub, cph, &offset);

    g_byte_array_append(data, 
        cph->data + offset, cph->len - offset);
    
    sprv = malloc(sizeof(drur_user_t));

    sprv->aids = g_ptr_array_new();
    for (int i = 0; i < prv->aids->len; i ++)
    {
        g_ptr_array_add(sprv->aids, 
            strdup(g_ptr_array_index(prv->aids, i)));
    }

    offset = 0;
    element_init_G1(sprv->gpk, spub->p);
    unserialize_element(prv->gpk, &offset, sprv->gpk);
    

    offset = 0;
    element_init_Zr(sprv->gsk, spub->p);
    unserialize_element(prv->gsk, &offset, sprv->gsk);

    sprv->sks = transferSK(spub, prv->sks);
    
    offset = 0;
    sprv->cask = transferRUSK(spub, prv->cask);
    offset = 0;
    sprv->cert = drur_unserialize_cert(spub, prv->cert, &offset);

    sprv->attributes = g_ptr_array_new();
    for (int i = 0; i < prv->attributes->len; i ++)
        g_ptr_array_add(sprv->attributes, 
            strdup(g_ptr_array_index(prv->attributes, i)));
    
    if (!drur_decrypt(scph, spub, sprv, m))
    {
        *_return = aes_128_cbc_decrypt(data, m);
        element_clear(m);
    }
    else
    {
        res = FALSE;
    }
    g_byte_array_free(data, 1);
    drur_realse_user(sprv);
    drur_realse_cph(scph);
    drur_realse_capub(spub);
    
    
    g_message("user decryption!");
    
    return res;
}