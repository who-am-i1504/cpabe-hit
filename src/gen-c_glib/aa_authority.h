/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef AA_AUTHORITY_H
#define AA_AUTHORITY_H

#include <thrift/c_glib/processor/thrift_dispatch_processor.h>

#include "aa_aa_types.h"

/* Authority service interface */
typedef struct _aaAuthorityIf aaAuthorityIf;  /* dummy object */

struct _aaAuthorityIfInterface
{
  GTypeInterface parent;

  gboolean (*aasetup) (aaAuthorityIf *iface, aaAuthParamter ** _return, const GByteArray * pub, const GPtrArray * attributes, const gchar * aid, GError **error);
  gboolean (*aakeygen) (aaAuthorityIf *iface, aaARevoKey ** _return, const aaAuthParamter * auth, const GByteArray * pub, const GByteArray * cert, const GPtrArray * attribtues, GError **error);
  gboolean (*updatekey) (aaAuthorityIf *iface, aaAttrPair ** _return, const GByteArray * auth_msk, const GByteArray * pub, const GByteArray * attr_pk, const GByteArray * attr_msk, const gchar * attribute, GError **error);
};
typedef struct _aaAuthorityIfInterface aaAuthorityIfInterface;

GType aa_authority_if_get_type (void);
#define AA_TYPE_AUTHORITY_IF (aa_authority_if_get_type())
#define AA_AUTHORITY_IF(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_IF, aaAuthorityIf))
#define AA_IS_AUTHORITY_IF(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_IF))
#define AA_AUTHORITY_IF_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE ((inst), AA_TYPE_AUTHORITY_IF, aaAuthorityIfInterface))

gboolean aa_authority_if_aasetup (aaAuthorityIf *iface, aaAuthParamter ** _return, const GByteArray * pub, const GPtrArray * attributes, const gchar * aid, GError **error);
gboolean aa_authority_if_aakeygen (aaAuthorityIf *iface, aaARevoKey ** _return, const aaAuthParamter * auth, const GByteArray * pub, const GByteArray * cert, const GPtrArray * attribtues, GError **error);
gboolean aa_authority_if_updatekey (aaAuthorityIf *iface, aaAttrPair ** _return, const GByteArray * auth_msk, const GByteArray * pub, const GByteArray * attr_pk, const GByteArray * attr_msk, const gchar * attribute, GError **error);

/* Authority service client */
struct _aaAuthorityClient
{
  GObject parent;

  ThriftProtocol *input_protocol;
  ThriftProtocol *output_protocol;
};
typedef struct _aaAuthorityClient aaAuthorityClient;

struct _aaAuthorityClientClass
{
  GObjectClass parent;
};
typedef struct _aaAuthorityClientClass aaAuthorityClientClass;

GType aa_authority_client_get_type (void);
#define AA_TYPE_AUTHORITY_CLIENT (aa_authority_client_get_type())
#define AA_AUTHORITY_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_CLIENT, aaAuthorityClient))
#define AA_AUTHORITY_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA_TYPE_AUTHORITY_CLIENT, aaAuthorityClientClass))
#define AA_AUTHORITY_IS_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_CLIENT))
#define AA_AUTHORITY_IS_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_CLIENT))
#define AA_AUTHORITY_CLIENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_CLIENT, aaAuthorityClientClass))

gboolean aa_authority_client_aasetup (aaAuthorityIf * iface, aaAuthParamter ** _return, const GByteArray * pub, const GPtrArray * attributes, const gchar * aid, GError ** error);
gboolean aa_authority_client_send_aasetup (aaAuthorityIf * iface, const GByteArray * pub, const GPtrArray * attributes, const gchar * aid, GError ** error);
gboolean aa_authority_client_recv_aasetup (aaAuthorityIf * iface, aaAuthParamter ** _return, GError ** error);
gboolean aa_authority_client_aakeygen (aaAuthorityIf * iface, aaARevoKey ** _return, const aaAuthParamter * auth, const GByteArray * pub, const GByteArray * cert, const GPtrArray * attribtues, GError ** error);
gboolean aa_authority_client_send_aakeygen (aaAuthorityIf * iface, const aaAuthParamter * auth, const GByteArray * pub, const GByteArray * cert, const GPtrArray * attribtues, GError ** error);
gboolean aa_authority_client_recv_aakeygen (aaAuthorityIf * iface, aaARevoKey ** _return, GError ** error);
gboolean aa_authority_client_updatekey (aaAuthorityIf * iface, aaAttrPair ** _return, const GByteArray * auth_msk, const GByteArray * pub, const GByteArray * attr_pk, const GByteArray * attr_msk, const gchar * attribute, GError ** error);
gboolean aa_authority_client_send_updatekey (aaAuthorityIf * iface, const GByteArray * auth_msk, const GByteArray * pub, const GByteArray * attr_pk, const GByteArray * attr_msk, const gchar * attribute, GError ** error);
gboolean aa_authority_client_recv_updatekey (aaAuthorityIf * iface, aaAttrPair ** _return, GError ** error);
void authority_client_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
void authority_client_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/* Authority handler (abstract base class) */
struct _aaAuthorityHandler
{
  GObject parent;
};
typedef struct _aaAuthorityHandler aaAuthorityHandler;

struct _aaAuthorityHandlerClass
{
  GObjectClass parent;

  gboolean (*aasetup) (aaAuthorityIf *iface, aaAuthParamter ** _return, const GByteArray * pub, const GPtrArray * attributes, const gchar * aid, GError **error);
  gboolean (*aakeygen) (aaAuthorityIf *iface, aaARevoKey ** _return, const aaAuthParamter * auth, const GByteArray * pub, const GByteArray * cert, const GPtrArray * attribtues, GError **error);
  gboolean (*updatekey) (aaAuthorityIf *iface, aaAttrPair ** _return, const GByteArray * auth_msk, const GByteArray * pub, const GByteArray * attr_pk, const GByteArray * attr_msk, const gchar * attribute, GError **error);
};
typedef struct _aaAuthorityHandlerClass aaAuthorityHandlerClass;

GType aa_authority_handler_get_type (void);
#define AA_TYPE_AUTHORITY_HANDLER (aa_authority_handler_get_type())
#define AA_AUTHORITY_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_HANDLER, aaAuthorityHandler))
#define AA_IS_AUTHORITY_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_HANDLER))
#define AA_AUTHORITY_HANDLER_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA_TYPE_AUTHORITY_HANDLER, aaAuthorityHandlerClass))
#define AA_IS_AUTHORITY_HANDLER_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_HANDLER))
#define AA_AUTHORITY_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_HANDLER, aaAuthorityHandlerClass))

gboolean aa_authority_handler_aasetup (aaAuthorityIf *iface, aaAuthParamter ** _return, const GByteArray * pub, const GPtrArray * attributes, const gchar * aid, GError **error);
gboolean aa_authority_handler_aakeygen (aaAuthorityIf *iface, aaARevoKey ** _return, const aaAuthParamter * auth, const GByteArray * pub, const GByteArray * cert, const GPtrArray * attribtues, GError **error);
gboolean aa_authority_handler_updatekey (aaAuthorityIf *iface, aaAttrPair ** _return, const GByteArray * auth_msk, const GByteArray * pub, const GByteArray * attr_pk, const GByteArray * attr_msk, const gchar * attribute, GError **error);

/* Authority processor */
struct _aaAuthorityProcessor
{
  ThriftDispatchProcessor parent;

  /* protected */
  aaAuthorityHandler *handler;
  GHashTable *process_map;
};
typedef struct _aaAuthorityProcessor aaAuthorityProcessor;

struct _aaAuthorityProcessorClass
{
  ThriftDispatchProcessorClass parent;

  /* protected */
  gboolean (*dispatch_call) (ThriftDispatchProcessor *processor,
                             ThriftProtocol *in,
                             ThriftProtocol *out,
                             gchar *fname,
                             gint32 seqid,
                             GError **error);
};
typedef struct _aaAuthorityProcessorClass aaAuthorityProcessorClass;

GType aa_authority_processor_get_type (void);
#define AA_TYPE_AUTHORITY_PROCESSOR (aa_authority_processor_get_type())
#define AA_AUTHORITY_PROCESSOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_PROCESSOR, aaAuthorityProcessor))
#define AA_IS_AUTHORITY_PROCESSOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_PROCESSOR))
#define AA_AUTHORITY_PROCESSOR_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA_TYPE_AUTHORITY_PROCESSOR, aaAuthorityProcessorClass))
#define AA_IS_AUTHORITY_PROCESSOR_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_PROCESSOR))
#define AA_AUTHORITY_PROCESSOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_PROCESSOR, aaAuthorityProcessorClass))

#endif /* AA_AUTHORITY_H */
