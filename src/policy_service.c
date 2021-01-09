/*
 * @Author: your name
 * @Date: 2021-01-08 15:57:15
 * @LastEditTime: 2021-01-08 16:47:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/policy_service.c
 */

#include "policy_service.h"

G_DEFINE_TYPE (MainPolicyHandler,
               main_policy_handler,
               POLICY_TYPE_POLICY_HANDLER)

static void
main_policy_handler_finalize(GObject *object)
{
    MainPolicyHandler   *self =
        MAIN_POLICY_HANDLER(object);
    
    g_hash_table_unref(self->log);

    self->log = NULL;

    G_OBJECT_CLASS (main_policy_handler_parent_class)
        -> finalize(object);
}

static void
main_policy_handler_init(MainPolicyHandler  *self)
{
    self->log = g_hash_table_new_full(g_int_hash,
                                      g_int_equal,
                                      g_free,
                                      g_object_unref);
}

static void
main_policy_handler_class_init(MainPolicyHandlerClass *klass)
{
    GObjectClass    *gobject_class = G_OBJECT_CLASS(klass);
    policyPolicyHandlerClass *policy_class =
        POLICY_POLICY_HANDLER_CLASS(klass);
    
    gobject_class->finalize = main_policy_handler_finalize;

    policy_class->parse_attributes = 
        main_policy_handler_parse_attributes;
    policy_class->parse_policy = 
        main_policy_handler_parse_policy;
}

static gboolean
main_policy_handler_parse_attributes(policyPolicyIf *iface,
                                     GPtrArray      **_return,
                                     const gchar    *attribute,
                                     GError         **error)
{
    THRIFT_UNUSED_VAR(iface);
    THRIFT_UNUSED_VAR(error);

    GSList *list;
    list = 0;
    parse_attribute(&list, attribute);

    *_return = g_ptr_array_new();
 
    for (GList* p = list; p != NULL; p = p->next)
    {
        g_ptr_array_add(*_return, p->data);
    }
    g_slist_free(list);

    return TRUE;
}

static gboolean
main_policy_handler_parse_policy(policyPolicyIf     *iface,
                                 gchar              **_return,
                                 const gchar        *policy,
                                 GError             **error)
{
    THRIFT_UNUSED_VAR(iface);
    THRIFT_UNUSED_VAR(error);
    *_return = parse_policy_lang(policy);
    return TRUE;
}