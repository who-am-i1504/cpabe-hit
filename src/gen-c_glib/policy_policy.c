/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include <string.h>
#include <thrift/c_glib/thrift.h>
#include <thrift/c_glib/thrift_application_exception.h>
#include "policy_policy.h"

gboolean
policy_policy_if_parse_attributes (policyPolicyIf *iface, GPtrArray ** _return, const gchar * attribute, GError **error)
{
  return POLICY_POLICY_IF_GET_INTERFACE (iface)->parse_attributes (iface, _return, attribute, error);
}

gboolean
policy_policy_if_parse_policy (policyPolicyIf *iface, gchar ** _return, const gchar * policy, GError **error)
{
  return POLICY_POLICY_IF_GET_INTERFACE (iface)->parse_policy (iface, _return, policy, error);
}

GType
policy_policy_if_get_type (void)
{
  static GType type = 0;
  if (type == 0)
  {
    static const GTypeInfo type_info =
    {
      sizeof (policyPolicyIfInterface),
      NULL,  /* base_init */
      NULL,  /* base_finalize */
      NULL,  /* class_init */
      NULL,  /* class_finalize */
      NULL,  /* class_data */
      0,     /* instance_size */
      0,     /* n_preallocs */
      NULL,  /* instance_init */
      NULL   /* value_table */
    };
    type = g_type_register_static (G_TYPE_INTERFACE,
                                   "policyPolicyIf",
                                   &type_info, 0);
  }
  return type;
}

static void 
policy_policy_if_interface_init (policyPolicyIfInterface *iface);

G_DEFINE_TYPE_WITH_CODE (policyPolicyClient, policy_policy_client,
                         G_TYPE_OBJECT, 
                         G_IMPLEMENT_INTERFACE (POLICY_TYPE_POLICY_IF,
                                                policy_policy_if_interface_init))

enum _policyPolicyClientProperties
{
  PROP_0,
  PROP_POLICY_POLICY_CLIENT_INPUT_PROTOCOL,
  PROP_POLICY_POLICY_CLIENT_OUTPUT_PROTOCOL
};

void
policy_policy_client_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  policyPolicyClient *client = POLICY_POLICY_CLIENT (object);

  THRIFT_UNUSED_VAR (pspec);

  switch (property_id)
  {
    case PROP_POLICY_POLICY_CLIENT_INPUT_PROTOCOL:
      client->input_protocol = g_value_get_object (value);
      break;
    case PROP_POLICY_POLICY_CLIENT_OUTPUT_PROTOCOL:
      client->output_protocol = g_value_get_object (value);
      break;
  }
}

void
policy_policy_client_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  policyPolicyClient *client = POLICY_POLICY_CLIENT (object);

  THRIFT_UNUSED_VAR (pspec);

  switch (property_id)
  {
    case PROP_POLICY_POLICY_CLIENT_INPUT_PROTOCOL:
      g_value_set_object (value, client->input_protocol);
      break;
    case PROP_POLICY_POLICY_CLIENT_OUTPUT_PROTOCOL:
      g_value_set_object (value, client->output_protocol);
      break;
  }
}

gboolean policy_policy_client_send_parse_attributes (policyPolicyIf * iface, const gchar * attribute, GError ** error)
{
  gint32 cseqid = 0;
  ThriftProtocol * protocol = POLICY_POLICY_CLIENT (iface)->output_protocol;

  if (thrift_protocol_write_message_begin (protocol, "parse_attributes", T_CALL, cseqid, error) < 0)
    return FALSE;

  {
    gint32 ret;
    gint32 xfer = 0;

    
    if ((ret = thrift_protocol_write_struct_begin (protocol, "parse_attributes_args", error)) < 0)
      return 0;
    xfer += ret;
    if ((ret = thrift_protocol_write_field_begin (protocol, "attribute", T_STRING, 1, error)) < 0)
      return 0;
    xfer += ret;
    if ((ret = thrift_protocol_write_string (protocol, attribute, error)) < 0)
      return 0;
    xfer += ret;

    if ((ret = thrift_protocol_write_field_end (protocol, error)) < 0)
      return 0;
    xfer += ret;
    if ((ret = thrift_protocol_write_field_stop (protocol, error)) < 0)
      return 0;
    xfer += ret;
    if ((ret = thrift_protocol_write_struct_end (protocol, error)) < 0)
      return 0;
    xfer += ret;

  }

  if (thrift_protocol_write_message_end (protocol, error) < 0)
    return FALSE;
  if (!thrift_transport_flush (protocol->transport, error))
    return FALSE;
  if (!thrift_transport_write_end (protocol->transport, error))
    return FALSE;

  return TRUE;
}

gboolean policy_policy_client_recv_parse_attributes (policyPolicyIf * iface, GPtrArray ** _return, GError ** error)
{
  gint32 rseqid;
  gchar * fname = NULL;
  ThriftMessageType mtype;
  ThriftProtocol * protocol = POLICY_POLICY_CLIENT (iface)->input_protocol;
  ThriftApplicationException *xception;

  if (thrift_protocol_read_message_begin (protocol, &fname, &mtype, &rseqid, error) < 0) {
    if (fname) g_free (fname);
    return FALSE;
  }

  if (mtype == T_EXCEPTION) {
    if (fname) g_free (fname);
    xception = g_object_new (THRIFT_TYPE_APPLICATION_EXCEPTION, NULL);
    thrift_struct_read (THRIFT_STRUCT (xception), protocol, NULL);
    thrift_protocol_read_message_end (protocol, NULL);
    thrift_transport_read_end (protocol->transport, NULL);
    g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR,xception->type, "application error: %s", xception->message);
    g_object_unref (xception);
    return FALSE;
  } else if (mtype != T_REPLY) {
    if (fname) g_free (fname);
    thrift_protocol_skip (protocol, T_STRUCT, NULL);
    thrift_protocol_read_message_end (protocol, NULL);
    thrift_transport_read_end (protocol->transport, NULL);
    g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR, THRIFT_APPLICATION_EXCEPTION_ERROR_INVALID_MESSAGE_TYPE, "invalid message type %d, expected T_REPLY", mtype);
    return FALSE;
  } else if (strncmp (fname, "parse_attributes", 16) != 0) {
    thrift_protocol_skip (protocol, T_STRUCT, NULL);
    thrift_protocol_read_message_end (protocol,error);
    thrift_transport_read_end (protocol->transport, error);
    g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR, THRIFT_APPLICATION_EXCEPTION_ERROR_WRONG_METHOD_NAME, "wrong method name %s, expected parse_attributes", fname);
    if (fname) g_free (fname);
    return FALSE;
  }
  if (fname) g_free (fname);

  {
    gint32 ret;
    gint32 xfer = 0;
    gchar *name = NULL;
    ThriftType ftype;
    gint16 fid;
    guint32 len = 0;
    gpointer data = NULL;
    

    /* satisfy -Wall in case these aren't used */
    THRIFT_UNUSED_VAR (len);
    THRIFT_UNUSED_VAR (data);

    /* read the struct begin marker */
    if ((ret = thrift_protocol_read_struct_begin (protocol, &name, error)) < 0)
    {
      if (name) g_free (name);
      return 0;
    }
    xfer += ret;
    if (name) g_free (name);
    name = NULL;

    /* read the struct fields */
    while (1)
    {
      /* read the beginning of a field */
      if ((ret = thrift_protocol_read_field_begin (protocol, &name, &ftype, &fid, error)) < 0)
      {
        if (name) g_free (name);
        return 0;
      }
      xfer += ret;
      if (name) g_free (name);
      name = NULL;

      /* break if we get a STOP field */
      if (ftype == T_STOP)
      {
        break;
      }

      switch (fid)
      {
        case 0:
          if (ftype == T_LIST)
          {
            {
              guint32 size;
              guint32 i;
              ThriftType element_type;

              if ((ret = thrift_protocol_read_list_begin (protocol, &element_type,&size, error)) < 0)
                return 0;
              xfer += ret;

              /* iterate through list elements */
              for (i = 0; i < size; i++)
              {
                gchar * _elem2 = NULL;
                if (_elem2 != NULL)
                {
                  g_free(_elem2);
                  _elem2 = NULL;
                }

                if ((ret = thrift_protocol_read_string (protocol, &_elem2, error)) < 0)
                  return 0;
                xfer += ret;
                g_ptr_array_add (*_return, _elem2);
              }
              if ((ret = thrift_protocol_read_list_end (protocol, error)) < 0)
                return 0;
              xfer += ret;
            }
          } else {
            if ((ret = thrift_protocol_skip (protocol, ftype, error)) < 0)
              return 0;
            xfer += ret;
          }
          break;
        default:
          if ((ret = thrift_protocol_skip (protocol, ftype, error)) < 0)
            return 0;
          xfer += ret;
          break;
      }
      if ((ret = thrift_protocol_read_field_end (protocol, error)) < 0)
        return 0;
      xfer += ret;
    }

    if ((ret = thrift_protocol_read_struct_end (protocol, error)) < 0)
      return 0;
    xfer += ret;

  }

  if (thrift_protocol_read_message_end (protocol, error) < 0)
    return FALSE;

  if (!thrift_transport_read_end (protocol->transport, error))
    return FALSE;

  return TRUE;
}

gboolean policy_policy_client_parse_attributes (policyPolicyIf * iface, GPtrArray ** _return, const gchar * attribute, GError ** error)
{
  if (!policy_policy_client_send_parse_attributes (iface, attribute, error))
    return FALSE;
  if (!policy_policy_client_recv_parse_attributes (iface, _return, error))
    return FALSE;
  return TRUE;
}

gboolean policy_policy_client_send_parse_policy (policyPolicyIf * iface, const gchar * policy, GError ** error)
{
  gint32 cseqid = 0;
  ThriftProtocol * protocol = POLICY_POLICY_CLIENT (iface)->output_protocol;

  if (thrift_protocol_write_message_begin (protocol, "parse_policy", T_CALL, cseqid, error) < 0)
    return FALSE;

  {
    gint32 ret;
    gint32 xfer = 0;

    
    if ((ret = thrift_protocol_write_struct_begin (protocol, "parse_policy_args", error)) < 0)
      return 0;
    xfer += ret;
    if ((ret = thrift_protocol_write_field_begin (protocol, "policy", T_STRING, 1, error)) < 0)
      return 0;
    xfer += ret;
    if ((ret = thrift_protocol_write_string (protocol, policy, error)) < 0)
      return 0;
    xfer += ret;

    if ((ret = thrift_protocol_write_field_end (protocol, error)) < 0)
      return 0;
    xfer += ret;
    if ((ret = thrift_protocol_write_field_stop (protocol, error)) < 0)
      return 0;
    xfer += ret;
    if ((ret = thrift_protocol_write_struct_end (protocol, error)) < 0)
      return 0;
    xfer += ret;

  }

  if (thrift_protocol_write_message_end (protocol, error) < 0)
    return FALSE;
  if (!thrift_transport_flush (protocol->transport, error))
    return FALSE;
  if (!thrift_transport_write_end (protocol->transport, error))
    return FALSE;

  return TRUE;
}

gboolean policy_policy_client_recv_parse_policy (policyPolicyIf * iface, gchar ** _return, GError ** error)
{
  gint32 rseqid;
  gchar * fname = NULL;
  ThriftMessageType mtype;
  ThriftProtocol * protocol = POLICY_POLICY_CLIENT (iface)->input_protocol;
  ThriftApplicationException *xception;

  if (thrift_protocol_read_message_begin (protocol, &fname, &mtype, &rseqid, error) < 0) {
    if (fname) g_free (fname);
    return FALSE;
  }

  if (mtype == T_EXCEPTION) {
    if (fname) g_free (fname);
    xception = g_object_new (THRIFT_TYPE_APPLICATION_EXCEPTION, NULL);
    thrift_struct_read (THRIFT_STRUCT (xception), protocol, NULL);
    thrift_protocol_read_message_end (protocol, NULL);
    thrift_transport_read_end (protocol->transport, NULL);
    g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR,xception->type, "application error: %s", xception->message);
    g_object_unref (xception);
    return FALSE;
  } else if (mtype != T_REPLY) {
    if (fname) g_free (fname);
    thrift_protocol_skip (protocol, T_STRUCT, NULL);
    thrift_protocol_read_message_end (protocol, NULL);
    thrift_transport_read_end (protocol->transport, NULL);
    g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR, THRIFT_APPLICATION_EXCEPTION_ERROR_INVALID_MESSAGE_TYPE, "invalid message type %d, expected T_REPLY", mtype);
    return FALSE;
  } else if (strncmp (fname, "parse_policy", 12) != 0) {
    thrift_protocol_skip (protocol, T_STRUCT, NULL);
    thrift_protocol_read_message_end (protocol,error);
    thrift_transport_read_end (protocol->transport, error);
    g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR, THRIFT_APPLICATION_EXCEPTION_ERROR_WRONG_METHOD_NAME, "wrong method name %s, expected parse_policy", fname);
    if (fname) g_free (fname);
    return FALSE;
  }
  if (fname) g_free (fname);

  {
    gint32 ret;
    gint32 xfer = 0;
    gchar *name = NULL;
    ThriftType ftype;
    gint16 fid;
    guint32 len = 0;
    gpointer data = NULL;
    

    /* satisfy -Wall in case these aren't used */
    THRIFT_UNUSED_VAR (len);
    THRIFT_UNUSED_VAR (data);

    /* read the struct begin marker */
    if ((ret = thrift_protocol_read_struct_begin (protocol, &name, error)) < 0)
    {
      if (name) g_free (name);
      return 0;
    }
    xfer += ret;
    if (name) g_free (name);
    name = NULL;

    /* read the struct fields */
    while (1)
    {
      /* read the beginning of a field */
      if ((ret = thrift_protocol_read_field_begin (protocol, &name, &ftype, &fid, error)) < 0)
      {
        if (name) g_free (name);
        return 0;
      }
      xfer += ret;
      if (name) g_free (name);
      name = NULL;

      /* break if we get a STOP field */
      if (ftype == T_STOP)
      {
        break;
      }

      switch (fid)
      {
        case 0:
          if (ftype == T_STRING)
          {
            if (*_return != NULL)
            {
              g_free(*_return);
              *_return = NULL;
            }

            if ((ret = thrift_protocol_read_string (protocol, &*_return, error)) < 0)
              return 0;
            xfer += ret;
          } else {
            if ((ret = thrift_protocol_skip (protocol, ftype, error)) < 0)
              return 0;
            xfer += ret;
          }
          break;
        default:
          if ((ret = thrift_protocol_skip (protocol, ftype, error)) < 0)
            return 0;
          xfer += ret;
          break;
      }
      if ((ret = thrift_protocol_read_field_end (protocol, error)) < 0)
        return 0;
      xfer += ret;
    }

    if ((ret = thrift_protocol_read_struct_end (protocol, error)) < 0)
      return 0;
    xfer += ret;

  }

  if (thrift_protocol_read_message_end (protocol, error) < 0)
    return FALSE;

  if (!thrift_transport_read_end (protocol->transport, error))
    return FALSE;

  return TRUE;
}

gboolean policy_policy_client_parse_policy (policyPolicyIf * iface, gchar ** _return, const gchar * policy, GError ** error)
{
  if (!policy_policy_client_send_parse_policy (iface, policy, error))
    return FALSE;
  if (!policy_policy_client_recv_parse_policy (iface, _return, error))
    return FALSE;
  return TRUE;
}

static void
policy_policy_if_interface_init (policyPolicyIfInterface *iface)
{
  iface->parse_attributes = policy_policy_client_parse_attributes;
  iface->parse_policy = policy_policy_client_parse_policy;
}

static void
policy_policy_client_init (policyPolicyClient *client)
{
  client->input_protocol = NULL;
  client->output_protocol = NULL;
}

static void
policy_policy_client_class_init (policyPolicyClientClass *cls)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (cls);
  GParamSpec *param_spec;

  gobject_class->set_property = policy_policy_client_set_property;
  gobject_class->get_property = policy_policy_client_get_property;

  param_spec = g_param_spec_object ("input_protocol",
                                    "input protocol (construct)",
                                    "Set the client input protocol",
                                    THRIFT_TYPE_PROTOCOL,
                                    G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class,
                                   PROP_POLICY_POLICY_CLIENT_INPUT_PROTOCOL, param_spec);

  param_spec = g_param_spec_object ("output_protocol",
                                    "output protocol (construct)",
                                    "Set the client output protocol",
                                    THRIFT_TYPE_PROTOCOL,
                                    G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class,
                                   PROP_POLICY_POLICY_CLIENT_OUTPUT_PROTOCOL, param_spec);
}

static void
policy_policy_handler_policy_if_interface_init (policyPolicyIfInterface *iface);

G_DEFINE_TYPE_WITH_CODE (policyPolicyHandler, 
                         policy_policy_handler,
                         G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (POLICY_TYPE_POLICY_IF,
                                                policy_policy_handler_policy_if_interface_init))

gboolean policy_policy_handler_parse_attributes (policyPolicyIf * iface, GPtrArray ** _return, const gchar * attribute, GError ** error)
{
  g_return_val_if_fail (POLICY_IS_POLICY_HANDLER (iface), FALSE);

  return POLICY_POLICY_HANDLER_GET_CLASS (iface)->parse_attributes (iface, _return, attribute, error);
}

gboolean policy_policy_handler_parse_policy (policyPolicyIf * iface, gchar ** _return, const gchar * policy, GError ** error)
{
  g_return_val_if_fail (POLICY_IS_POLICY_HANDLER (iface), FALSE);

  return POLICY_POLICY_HANDLER_GET_CLASS (iface)->parse_policy (iface, _return, policy, error);
}

static void
policy_policy_handler_policy_if_interface_init (policyPolicyIfInterface *iface)
{
  iface->parse_attributes = policy_policy_handler_parse_attributes;
  iface->parse_policy = policy_policy_handler_parse_policy;
}

static void
policy_policy_handler_init (policyPolicyHandler *self)
{
  THRIFT_UNUSED_VAR (self);
}

static void
policy_policy_handler_class_init (policyPolicyHandlerClass *cls)
{
  cls->parse_attributes = NULL;
  cls->parse_policy = NULL;
}

enum _policyPolicyProcessorProperties
{
  PROP_POLICY_POLICY_PROCESSOR_0,
  PROP_POLICY_POLICY_PROCESSOR_HANDLER
};

G_DEFINE_TYPE (policyPolicyProcessor,
               policy_policy_processor,
               THRIFT_TYPE_DISPATCH_PROCESSOR)

typedef gboolean (* policyPolicyProcessorProcessFunction) (policyPolicyProcessor *, 
                                                           gint32,
                                                           ThriftProtocol *,
                                                           ThriftProtocol *,
                                                           GError **);

typedef struct
{
  gchar *name;
  policyPolicyProcessorProcessFunction function;
} policy_policy_processor_process_function_def;

static gboolean
policy_policy_processor_process_parse_attributes (policyPolicyProcessor *,
                                                  gint32,
                                                  ThriftProtocol *,
                                                  ThriftProtocol *,
                                                  GError **);
static gboolean
policy_policy_processor_process_parse_policy (policyPolicyProcessor *,
                                              gint32,
                                              ThriftProtocol *,
                                              ThriftProtocol *,
                                              GError **);

static policy_policy_processor_process_function_def
policy_policy_processor_process_function_defs[2] = {
  {
    "parse_attributes",
    policy_policy_processor_process_parse_attributes
  },
  {
    "parse_policy",
    policy_policy_processor_process_parse_policy
  }
};

static gboolean
policy_policy_processor_process_parse_attributes (policyPolicyProcessor *self,
                                                  gint32 sequence_id,
                                                  ThriftProtocol *input_protocol,
                                                  ThriftProtocol *output_protocol,
                                                  GError **error)
{
  gboolean result = TRUE;
  ThriftTransport * transport;
  ThriftApplicationException *xception;
  policyPolicyParseAttributesArgs * args =
    g_object_new (POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_ARGS, NULL);

  g_object_get (input_protocol, "transport", &transport, NULL);

  if ((thrift_struct_read (THRIFT_STRUCT (args), input_protocol, error) != -1) &&
      (thrift_protocol_read_message_end (input_protocol, error) != -1) &&
      (thrift_transport_read_end (transport, error) != FALSE))
  {
    gchar * attribute;
    GPtrArray * return_value;
    policyPolicyParseAttributesResult * result_struct;

    g_object_get (args,
                  "attribute", &attribute,
                  NULL);

    g_object_unref (transport);
    g_object_get (output_protocol, "transport", &transport, NULL);

    result_struct = g_object_new (POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_RESULT, NULL);
    g_object_get (result_struct, "success", &return_value, NULL);

    if (policy_policy_handler_parse_attributes (POLICY_POLICY_IF (self->handler),
                                                &return_value,
                                                attribute,
                                                error) == TRUE)
    {
      g_object_set (result_struct, "success", return_value, NULL);
      if (return_value != NULL)
        g_ptr_array_unref (return_value);

      result =
        ((thrift_protocol_write_message_begin (output_protocol,
                                               "parse_attributes",
                                               T_REPLY,
                                               sequence_id,
                                               error) != -1) &&
         (thrift_struct_write (THRIFT_STRUCT (result_struct),
                               output_protocol,
                               error) != -1));
    }
    else
    {
      if (*error == NULL)
        g_warning ("Policy.parse_attributes implementation returned FALSE "
                   "but did not set an error");

      xception =
        g_object_new (THRIFT_TYPE_APPLICATION_EXCEPTION,
                      "type",    *error != NULL ? (*error)->code :
                                 THRIFT_APPLICATION_EXCEPTION_ERROR_UNKNOWN,
                      "message", *error != NULL ? (*error)->message : NULL,
                      NULL);
      g_clear_error (error);

      result =
        ((thrift_protocol_write_message_begin (output_protocol,
                                               "parse_attributes",
                                               T_EXCEPTION,
                                               sequence_id,
                                               error) != -1) &&
         (thrift_struct_write (THRIFT_STRUCT (xception),
                               output_protocol,
                               error) != -1));

      g_object_unref (xception);
    }

    if (attribute != NULL)
      g_free (attribute);
    g_object_unref (result_struct);

    if (result == TRUE)
      result =
        ((thrift_protocol_write_message_end (output_protocol, error) != -1) &&
         (thrift_transport_write_end (transport, error) != FALSE) &&
         (thrift_transport_flush (transport, error) != FALSE));
  }
  else
    result = FALSE;

  g_object_unref (transport);
  g_object_unref (args);

  return result;
}

static gboolean
policy_policy_processor_process_parse_policy (policyPolicyProcessor *self,
                                              gint32 sequence_id,
                                              ThriftProtocol *input_protocol,
                                              ThriftProtocol *output_protocol,
                                              GError **error)
{
  gboolean result = TRUE;
  ThriftTransport * transport;
  ThriftApplicationException *xception;
  policyPolicyParsePolicyArgs * args =
    g_object_new (POLICY_TYPE_POLICY_PARSE_POLICY_ARGS, NULL);

  g_object_get (input_protocol, "transport", &transport, NULL);

  if ((thrift_struct_read (THRIFT_STRUCT (args), input_protocol, error) != -1) &&
      (thrift_protocol_read_message_end (input_protocol, error) != -1) &&
      (thrift_transport_read_end (transport, error) != FALSE))
  {
    gchar * policy;
    gchar * return_value;
    policyPolicyParsePolicyResult * result_struct;

    g_object_get (args,
                  "policy", &policy,
                  NULL);

    g_object_unref (transport);
    g_object_get (output_protocol, "transport", &transport, NULL);

    result_struct = g_object_new (POLICY_TYPE_POLICY_PARSE_POLICY_RESULT, NULL);
    g_object_get (result_struct, "success", &return_value, NULL);

    if (policy_policy_handler_parse_policy (POLICY_POLICY_IF (self->handler),
                                            &return_value,
                                            policy,
                                            error) == TRUE)
    {
      g_object_set (result_struct, "success", return_value, NULL);
      if (return_value != NULL)
        g_free (return_value);

      result =
        ((thrift_protocol_write_message_begin (output_protocol,
                                               "parse_policy",
                                               T_REPLY,
                                               sequence_id,
                                               error) != -1) &&
         (thrift_struct_write (THRIFT_STRUCT (result_struct),
                               output_protocol,
                               error) != -1));
    }
    else
    {
      if (*error == NULL)
        g_warning ("Policy.parse_policy implementation returned FALSE "
                   "but did not set an error");

      xception =
        g_object_new (THRIFT_TYPE_APPLICATION_EXCEPTION,
                      "type",    *error != NULL ? (*error)->code :
                                 THRIFT_APPLICATION_EXCEPTION_ERROR_UNKNOWN,
                      "message", *error != NULL ? (*error)->message : NULL,
                      NULL);
      g_clear_error (error);

      result =
        ((thrift_protocol_write_message_begin (output_protocol,
                                               "parse_policy",
                                               T_EXCEPTION,
                                               sequence_id,
                                               error) != -1) &&
         (thrift_struct_write (THRIFT_STRUCT (xception),
                               output_protocol,
                               error) != -1));

      g_object_unref (xception);
    }

    if (policy != NULL)
      g_free (policy);
    g_object_unref (result_struct);

    if (result == TRUE)
      result =
        ((thrift_protocol_write_message_end (output_protocol, error) != -1) &&
         (thrift_transport_write_end (transport, error) != FALSE) &&
         (thrift_transport_flush (transport, error) != FALSE));
  }
  else
    result = FALSE;

  g_object_unref (transport);
  g_object_unref (args);

  return result;
}

static gboolean
policy_policy_processor_dispatch_call (ThriftDispatchProcessor *dispatch_processor,
                                       ThriftProtocol *input_protocol,
                                       ThriftProtocol *output_protocol,
                                       gchar *method_name,
                                       gint32 sequence_id,
                                       GError **error)
{
  policy_policy_processor_process_function_def *process_function_def;
  gboolean dispatch_result = FALSE;

  policyPolicyProcessor *self = POLICY_POLICY_PROCESSOR (dispatch_processor);
  ThriftDispatchProcessorClass *parent_class =
    g_type_class_peek_parent (POLICY_POLICY_PROCESSOR_GET_CLASS (self));

  process_function_def = g_hash_table_lookup (self->process_map, method_name);
  if (process_function_def != NULL)
  {
    g_free (method_name);
    dispatch_result = (*process_function_def->function) (self,
                                                         sequence_id,
                                                         input_protocol,
                                                         output_protocol,
                                                         error);
  }
  else
  {
    dispatch_result = parent_class->dispatch_call (dispatch_processor,
                                                   input_protocol,
                                                   output_protocol,
                                                   method_name,
                                                   sequence_id,
                                                   error);
  }

  return dispatch_result;
}

static void
policy_policy_processor_set_property (GObject *object,
                                      guint property_id,
                                      const GValue *value,
                                      GParamSpec *pspec)
{
  policyPolicyProcessor *self = POLICY_POLICY_PROCESSOR (object);

  switch (property_id)
  {
    case PROP_POLICY_POLICY_PROCESSOR_HANDLER:
      if (self->handler != NULL)
        g_object_unref (self->handler);
      self->handler = g_value_get_object (value);
      g_object_ref (self->handler);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
policy_policy_processor_get_property (GObject *object,
                                      guint property_id,
                                      GValue *value,
                                      GParamSpec *pspec)
{
  policyPolicyProcessor *self = POLICY_POLICY_PROCESSOR (object);

  switch (property_id)
  {
    case PROP_POLICY_POLICY_PROCESSOR_HANDLER:
      g_value_set_object (value, self->handler);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
policy_policy_processor_dispose (GObject *gobject)
{
  policyPolicyProcessor *self = POLICY_POLICY_PROCESSOR (gobject);

  if (self->handler != NULL)
  {
    g_object_unref (self->handler);
    self->handler = NULL;
  }

  G_OBJECT_CLASS (policy_policy_processor_parent_class)->dispose (gobject);
}

static void
policy_policy_processor_finalize (GObject *gobject)
{
  policyPolicyProcessor *self = POLICY_POLICY_PROCESSOR (gobject);

  thrift_safe_hash_table_destroy (self->process_map);

  G_OBJECT_CLASS (policy_policy_processor_parent_class)->finalize (gobject);
}

static void
policy_policy_processor_init (policyPolicyProcessor *self)
{
  guint index;

  self->handler = NULL;
  self->process_map = g_hash_table_new (g_str_hash, g_str_equal);

  for (index = 0; index < 2; index += 1)
    g_hash_table_insert (self->process_map,
                         policy_policy_processor_process_function_defs[index].name,
                         &policy_policy_processor_process_function_defs[index]);
}

static void
policy_policy_processor_class_init (policyPolicyProcessorClass *cls)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (cls);
  ThriftDispatchProcessorClass *dispatch_processor_class =
    THRIFT_DISPATCH_PROCESSOR_CLASS (cls);
  GParamSpec *param_spec;

  gobject_class->dispose = policy_policy_processor_dispose;
  gobject_class->finalize = policy_policy_processor_finalize;
  gobject_class->set_property = policy_policy_processor_set_property;
  gobject_class->get_property = policy_policy_processor_get_property;

  dispatch_processor_class->dispatch_call = policy_policy_processor_dispatch_call;
  cls->dispatch_call = policy_policy_processor_dispatch_call;

  param_spec = g_param_spec_object ("handler",
                                    "Service handler implementation",
                                    "The service handler implementation "
                                    "to which method calls are dispatched.",
                                    POLICY_TYPE_POLICY_HANDLER,
                                    G_PARAM_READWRITE);
  g_object_class_install_property (gobject_class,
                                   PROP_POLICY_POLICY_PROCESSOR_HANDLER,
                                   param_spec);
}
