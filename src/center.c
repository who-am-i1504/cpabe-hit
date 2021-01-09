/*
 * @Author: your name
 * @Date: 2020-12-31 15:19:45
 * @LastEditTime: 2021-01-07 20:23:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /thrift-example/authority.c
 */
#include "center.h"

G_DEFINE_TYPE (MainCaCenterHandler,
              main_ca_center_handler,
              CA_TYPE_CENTER_HANDLER)

static void
main_ca_center_handler_finalize(GObject* object)
{
    MainCaCenterHandler *self = 
        MAIN_CA_CENTER_HANDLER(object);
    
    g_hash_table_unref(self->log);
    
    self->log = NULL;

    G_OBJECT_CLASS (main_ca_center_handler_parent_class)->
        finalize(object);
}

static void
main_ca_center_handler_init(MainCaCenterHandler * self)
{
    self->log = g_hash_table_new_full(g_int_hash,
                                      g_int_equal, 
                                      g_free, 
                                      g_object_unref);
}

static void
main_ca_center_handler_class_init(MainCaCenterHandlerClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    caCenterHandlerClass *center_class =
        CA_CENTER_HANDLER_CLASS(klass);
    
    gobject_class->finalize = main_ca_center_handler_finalize;

    center_class->cakeygen = main_ca_center_handler_keygen;
    center_class->casetup = main_ca_center_handler_setup;
    center_class->user_register = main_ca_center_handler_user_register;
}

static gboolean
main_ca_center_handler_setup(caCenterIf     *iface,
                             caCAParamter   **_return,
                             const gint32   N,
                             const gint32   byte_size,
                             GError         **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);
    drur_capub_t *pub;
    drur_camsk_t *msk;
    GByteArray *b;
    drur_casetup( &pub, &msk, N, byte_size);
    (*_return) = 
        g_object_new(CA_TYPE_C_A_PARAMTER,
                     NULL);
    b = g_byte_array_new();    
    drur_serialize_capub(b, pub);
    (*_return)->pub = b;
    b = g_byte_array_new();
    drur_serialize_camsk(b, msk);
    (*_return)->msk = b;

    drur_realse_camsk(msk);
    drur_realse_capub(pub);

    g_message("center authority setup");
    return TRUE;
    
}

static gboolean
main_ca_center_handler_keygen(caCenterIf     *iface,
                              caURevoKey     **_return,
                              caCAParamter   *param,
                              GPtrArray      *attributes,
                              const gint32   uid,
                              GError         **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);
    
    drur_capub_t* pub;
    drur_camsk_t* msk;
    drur_ru_sk_t* sk;
    int offset;
    offset = 0;
    pub = drur_unserialize_capub(param->pub, &offset);
    offset = 0;
    msk = drur_unserialize_camsk(pub, param->msk, &offset);
    
    *_return = 
        g_object_new(CA_TYPE_U_REVO_KEY,
                     NULL);
    sk = drur_cakeygen(pub, msk, attributes, uid);
    (*_return)->K0 = g_byte_array_new();
    (*_return)->K1 = g_byte_array_new();
    (*_return)->K2 = g_byte_array_new();
    (*_return)->KJ = g_byte_array_new();
    (*_return)->Kijx = 
        g_hash_table_new(g_str_hash, g_str_equal);
    (*_return)->K0 = g_byte_array_new();
    (*_return)->attributes = g_ptr_array_new();

    (*_return)->m = pub->rupub->m;

    serialize_element((*_return)->K0, sk->K0);
    serialize_element((*_return)->K1, sk->K1);
    serialize_element((*_return)->K2, sk->K2);

    for (int i = 0; i < (*_return)->m; i ++)
        serialize_element((*_return)->KJ, sk->K_j[i]);
    
    for (int i = 0; i < sk->attributes->len; i ++)
    {
        char *attr;
        GByteArray *b;
        element_t *kijx;

        b = g_byte_array_new();
        attr = strdup(g_ptr_array_index(attributes, i));
        kijx = g_hash_table_lookup(sk->Kijx, attr);
        serialize_element(b, kijx[0]);
        serialize_element(b, kijx[1]);
        g_ptr_array_add((*_return)->attributes, attr);
        g_hash_table_insert((*_return)->Kijx, attr, b);
    }
    
    drur_realse_camsk(msk);
    drur_realse_ru_sk(sk);
    drur_realse_capub(pub);
    g_message("center authority keygen");
    return TRUE;
}


static gboolean
main_ca_center_handler_user_register(caCenterIf     *iface,
                                     caUserRegister **_return,
                                     GByteArray     *pub,
                                     gchar          *uid,
                                     GPtrArray      *attributes,
                                     GError         **error)
{
    THRIFT_UNUSED_VAR (iface);
    THRIFT_UNUSED_VAR (error);

    drur_capub_t *spub;
    drur_user_t *user;
    GPtrArray* attrs;
    GHashTable* set;
    GSList *alist;
    int offset;
    set = g_hash_table_new(g_str_hash, g_str_equal);

    attrs = g_ptr_array_new();
    alist = 0;
    for (int i = 0; i < attributes->len; i ++)
        parse_attribute(&alist, g_ptr_array_index(attributes, i));
    for (GSList* p = alist; p != NULL; p = p->next)
    {
        if (!p->data)continue;
        if (g_hash_table_contains(set, p->data))
        {
            free(p->data);
            continue;
        }
        g_hash_table_add(set, p->data);
        g_ptr_array_add(attrs, p->data);
    }
    g_slist_free(alist);
        
    
    g_hash_table_destroy(set);

    *_return = 
        g_object_new(CA_TYPE_USER_REGISTER,
                     NULL);
    offset = 0;
    spub = drur_unserialize_capub(pub, &offset);
    user = drur_user_register(spub, uid, attrs);

    (*_return)->gpk = g_byte_array_new();
    (*_return)->gsk = g_byte_array_new();
    (*_return)->UserCert = g_byte_array_new();
    (*_return)->attributes = user->attributes;
    (*_return)->uid = user->cert->id;

    serialize_element((*_return)->gpk, user->gpk);
    serialize_element((*_return)->gsk, user->gsk);
    drur_serialize_cert((*_return)->UserCert, user->cert);
    
    drur_realse_user_cert(user->cert);
    element_clear(user->gsk);
    element_clear(user->gpk);
    free(user);

    for (int i = 0; i < attrs->len; i ++)
        free(g_ptr_array_index(attrs, i));
    g_ptr_array_free(attrs, 0);
    drur_realse_capub(spub);
    
    g_message("center authority user register!");

    return TRUE;
}