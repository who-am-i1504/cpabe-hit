/*
 * @Author: your name
 * @Date: 2021-01-08 10:41:35
 * @LastEditTime: 2021-01-08 15:22:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/cloud.c
 */
#include "cloud.h"


G_DEFINE_TYPE (MainCloudHandler,
              main_cloud_handler,
              CLOUD_TYPE_CLOUD_HANDLER)

static void
main_cloud_handler_finalize(GObject* object)
{
    MainCloudHandler *self = 
        MAIN_CLOUD_HANDLER(object);
    
    g_hash_table_unref(self->log);
    
    self->log = NULL;

    G_OBJECT_CLASS (main_cloud_handler_parent_class)->
        finalize(object);
}

static void
main_cloud_handler_init(MainCloudHandler * self)
{
    self->log = g_hash_table_new_full(g_int_hash,
                                      g_int_equal, 
                                      g_free, 
                                      g_object_unref);
}

static void
main_cloud_handler_class_init(MainCloudHandlerClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    cloudCloudHandlerClass *cloud_class =
        CLOUD_CLOUD_HANDLER_CLASS(klass);
    
    gobject_class->finalize = main_cloud_handler_finalize;

    cloud_class->updatecph = main_cloud_handler_updatecph;
}

static gboolean
main_cloud_handler_updatecph(cloudCloudIf *iface,
                             GByteArray    **_return,
                             GByteArray    *cph,
                             GByteArray    *pub,
                             GByteArray    *cuk,
                             const gint32  version,
                             gchar         *attribute,
                             GError        **error)
{
    THRIFT_UNUSED_VAR(iface);
    THRIFT_UNUSED_VAR(error);
    
    drur_cph_t* scph;
    drur_capub_t *spub;
    element_t scuk;
    int offset;

    offset = 0;
    spub = drur_unserialize_capub(pub, &offset);
    element_init_Zr(scuk, spub->p);
    offset = 0;
    unserialize_element(cuk, &offset, scuk);
    offset = 0;
    scph = drur_unserialize_cph(spub, cph, &offset);

    drur_cph_update(scph, spub, cuk, version, attribute);
    
    *_return = g_byte_array_new();
    drur_serialize_cph(*_return, scph);
    g_byte_array_append(*_return, 
        cph->data + offset, cph->len - offset);

    return TRUE;
}