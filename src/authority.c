/*
 * @Author: your name
 * @Date: 2020-12-31 15:19:45
 * @LastEditTime: 2021-01-25 12:03:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /thrift-example/authority.c
 */
#include "authority.h"


G_DEFINE_TYPE (MainAaAuthorityHandler,
              main_aa_authority_handler,
              AA_TYPE_AUTHORITY_HANDLER)

static void
main_aa_authority_handler_finalize(GObject* object)
{
    MainAaAuthorityHandler *self = 
        MAIN_AA_AUTHORITY_HANDLER(object);
    
    g_hash_table_unref(self->log);
    
    self->log = NULL;

    G_OBJECT_CLASS (main_aa_authority_handler_parent_class)->
        finalize(object);
}

static void
main_aa_authority_handler_init(MainAaAuthorityHandler * self)
{
    self->log = g_hash_table_new_full(g_int_hash,
                                      g_int_equal, 
                                      g_free, 
                                      g_object_unref);
}

static void
main_aa_authority_handler_class_init(MainAaAuthorityHandlerClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    aaAuthorityHandlerClass *authority_class =
        AA_AUTHORITY_HANDLER_CLASS(klass);
    
    gobject_class->finalize = main_aa_authority_handler_finalize;

    authority_class->aakeygen = main_aa_authority_handler_keygen;
    authority_class->aasetup = main_aa_authority_handler_setup;
    authority_class->updatekey = main_aa_authority_handler_ukeygen;
}

static gboolean
main_aa_authority_handler_setup(aaAuthorityIf   *iface,
                                aaAuthParamter  **_return,
                                GByteArray      *pub,
                                GPtrArray       *attributes,
                                gchar           *aid,
                                GError          **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);

    drur_capub_t *spub;
    drur_auth_t  *auth;
    int offset;

    offset = 0;
    spub = drur_unserialize_capub(pub, &offset);
    
    auth = drur_aasetup(spub, attributes, aid);

    *_return = 
        g_object_new(AA_TYPE_AUTH_PARAMTER,
                     NULL);
    
    (*_return)->aid = strdup(auth->aid);
    (*_return)->auth_msk = g_byte_array_new();
    (*_return)->auth_pk = g_byte_array_new();
    (*_return)->attr_pks = 
        g_hash_table_new(g_str_hash, g_str_equal);
    (*_return)->attr_msks = 
        g_hash_table_new(g_str_hash, g_str_equal);
    
    drur_serialize_auth_msk(
        (*_return)->auth_msk, auth->auth_msk);

    drur_serialize_auth_pub(
        (*_return)->auth_pk, auth->auth_pk);
    
    (*_return)->attributes = g_ptr_array_new();
    
    for (int i = 0; i < auth->attributes->len; i ++)
    {
        char * attr;
        drur_attr_pk_t  *pk;
        drur_attr_msk_t *pmsk;
        GByteArray      *b;
        
        attr = strdup(g_ptr_array_index(auth->attributes, i));
        if (g_hash_table_contains((*_return)->attr_pks, attr))
        {
            free(attr);
            continue;
        }
        pk = g_hash_table_lookup(auth->attr_pks, attr);
        pmsk = g_hash_table_lookup(auth->attr_msks, attr);

        b = g_byte_array_new();
        drur_serialize_attr_pk(b, pk);
        g_hash_table_insert((*_return)->attr_pks, attr, b);

        b = g_byte_array_new();
        drur_seriallize_attr_msk(b, pmsk);
        g_hash_table_insert((*_return)->attr_msks, attr, b);

        g_ptr_array_add(
            (*_return)->attributes, attr);
    }

    drur_realse_auth(auth);
    drur_realse_capub(spub);

    g_message("attribute authority setup!");
    return TRUE;
    
}

static gboolean
main_aa_authority_handler_keygen(aaAuthorityIf  *iface,
                                 aaARevoKey     **_return,
                                 aaAuthParamter *auth,
                                 GByteArray     *pub,
                                 GByteArray     *cert,
                                 GByteArray     *attributes,
                                 GError         **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);

    drur_capub_t *spub;
    drur_user_cert_t *scert;
    drur_auth_t *sauth;
    drur_prv_item_t *sk;
    int offset;

    offset = 0;
    spub = drur_unserialize_capub(pub, &offset);

    offset = 0;
    scert = drur_unserialize_cert(spub, cert, &offset);

    sauth = malloc(sizeof(drur_auth_t));

    sauth->aid = strdup(auth->aid);
    sauth->attributes = g_ptr_array_new();
    sauth->attr_msks = 
        g_hash_table_new(g_str_hash, g_str_equal);
    sauth->attr_pks = 
        g_hash_table_new(g_str_hash, g_str_equal);

    for (int i = 0; i < auth->attributes->len; i ++)
    {
        char *attr;
        attr = strdup(
            g_ptr_array_index(auth->attributes, i));
        offset = 0;
        g_hash_table_insert(sauth->attr_pks, attr,
            drur_unserialize_attr_pk(spub, 
            g_hash_table_lookup(auth->attr_pks, attr), &offset));
        
        offset = 0;
        g_hash_table_insert(sauth->attr_msks, attr,
            drur_unserialize_attr_msk(spub,
            g_hash_table_lookup(auth->attr_msks, attr), &offset));
        
        g_ptr_array_add(sauth->attributes, attr);
    }
    offset = 0;
    sauth->auth_pk = 
        drur_unserialize_auth_pub(
        spub, auth->auth_pk, &offset);
    offset = 0;
    sauth->auth_msk = 
        drur_unserialize_auth_msk(
        spub, auth->auth_msk, &offset);

    sk = drur_aakeygen(sauth, spub, scert, attributes);

    *_return = 
        g_object_new(AA_TYPE_A_REVO_KEY,
                     NULL);
    
    (*_return)->aid = strdup(auth->aid);
    (*_return)->attributes = g_ptr_array_new();
    (*_return)->K = g_byte_array_new();
    serialize_element((*_return)->K, sk->K);
    (*_return)->L = g_byte_array_new(); 
    serialize_element((*_return)->L, sk->L);
    (*_return)->R = g_byte_array_new();
    serialize_element((*_return)->R, sk->R);
    (*_return)->kx = 
        g_hash_table_new(g_str_hash, g_str_equal);
    
    for (int i = 0; i < sk->attributes->len; i ++)
    {
        char *attr;
        GByteArray *b;

        attr = strdup(
            g_ptr_array_index(sk->attributes, i));
        
        b = g_byte_array_new();
        drur_serialize_prv_kx(b, g_hash_table_lookup(sk->kx, attr));
        g_hash_table_insert(
            (*_return)->kx, strdup(attr), b);
        
        g_ptr_array_add((*_return)->attributes, attr);
    }
    drur_realse_prv_item(sk);
    drur_realse_user_cert(scert);
    drur_realse_auth(sauth);
    drur_realse_capub(spub);
    g_message("attribute authority keygen!");
    
    return TRUE;
}


static gboolean
main_aa_authority_handler_ukeygen(aaAuthorityIf *iface,
                                  aaAttrPair    **_return,
                                  GByteArray    **auth_msk,
                                  GByteArray    *pub,
                                  GByteArray    *attr_pk,
                                  GByteArray    *attr_msk,
                                  gchar         *attribute,
                                  GError        **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);
    drur_capub_t *spub;
    drur_auth_msk_t *akey;
    drur_attr_pk_t  *apk;
    drur_attr_msk_t *amsk;
    int offset;

    offset = 0;
    spub = drur_unserialize_capub(pub, &offset);

    offset = 0;
    akey = drur_unserialize_auth_msk(spub, auth_msk, &offset);

    offset = 0;
    apk = drur_unserialize_attr_pk(spub, attr_pk, &offset);

    offset = 0;
    amsk = drur_unserialize_attr_msk(spub, attr_msk, &offset);
    drur_ukeygen(akey, spub, apk, amsk, attribute);

    *_return = 
        g_object_new(AA_TYPE_ATTR_PAIR,
                     NULL);

    (*_return)->attr_msk = g_byte_array_new();
    (*_return)->attr_pk = g_byte_array_new();
    (*_return)->cuk = g_byte_array_new();
    
    drur_serialize_attr_pk((*_return)->attr_pk, apk);
    drur_seriallize_attr_msk((*_return)->attr_msk, amsk);
    serialize_element((*_return)->cuk, amsk->cuk);
    (*_return)->version = amsk->version;
    
    drur_realse_attr_pk(apk);
    drur_realse_attr_msk(amsk);
    drur_realse_auth_msk(akey);
    drur_realse_capub(spub);

    return TRUE;
}

static gboolean
main_aa_authority_handler_add_attribute(aaAuthorityIf *iface,
                                        aaAttrPair    **_return,
                                        GByteArray    **auth_msk,
                                        GByteArray    *pub,
                                        gchar         *attribute,
                                        GError        **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);

    *_return = g_object_new(
        AA_TYPE_ATTR_PAIR,
        NULL);
    drur_capub_t *spub;
    drur_auth_msk_t *akey;
    drur_attr_pk_t* pk;
    drur_attr_msk_t* pmsk;
    int offset;

    offset = 0;
    spub = drur_unserialize_capub   (pub, &offset);
    
    offset = 0;
    akey = drur_unserialize_auth_msk(pub, auth_msk, &offset);

    drur_add_attribute(&pk, &pmsk, spub, akey, attribute);

    (*_return)->attr_pk;

    
    return TRUE;
    
}