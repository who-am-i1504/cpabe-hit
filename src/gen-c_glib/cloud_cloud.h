/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef CLOUD_CLOUD_H
#define CLOUD_CLOUD_H

#include <thrift/c_glib/processor/thrift_dispatch_processor.h>

#include "cloud_cloud_types.h"

/* Cloud service interface */
typedef struct _cloudCloudIf cloudCloudIf;  /* dummy object */

struct _cloudCloudIfInterface
{
  GTypeInterface parent;

  gboolean (*updatecph) (cloudCloudIf *iface, GByteArray ** _return, const GByteArray * cph, const GByteArray * pub, const GByteArray * cuk, const gint32 version, const gchar * attribute, GError **error);
};
typedef struct _cloudCloudIfInterface cloudCloudIfInterface;

GType cloud_cloud_if_get_type (void);
#define CLOUD_TYPE_CLOUD_IF (cloud_cloud_if_get_type())
#define CLOUD_CLOUD_IF(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLOUD_TYPE_CLOUD_IF, cloudCloudIf))
#define CLOUD_IS_CLOUD_IF(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLOUD_TYPE_CLOUD_IF))
#define CLOUD_CLOUD_IF_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE ((inst), CLOUD_TYPE_CLOUD_IF, cloudCloudIfInterface))

gboolean cloud_cloud_if_updatecph (cloudCloudIf *iface, GByteArray ** _return, const GByteArray * cph, const GByteArray * pub, const GByteArray * cuk, const gint32 version, const gchar * attribute, GError **error);

/* Cloud service client */
struct _cloudCloudClient
{
  GObject parent;

  ThriftProtocol *input_protocol;
  ThriftProtocol *output_protocol;
};
typedef struct _cloudCloudClient cloudCloudClient;

struct _cloudCloudClientClass
{
  GObjectClass parent;
};
typedef struct _cloudCloudClientClass cloudCloudClientClass;

GType cloud_cloud_client_get_type (void);
#define CLOUD_TYPE_CLOUD_CLIENT (cloud_cloud_client_get_type())
#define CLOUD_CLOUD_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLOUD_TYPE_CLOUD_CLIENT, cloudCloudClient))
#define CLOUD_CLOUD_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), CLOUD_TYPE_CLOUD_CLIENT, cloudCloudClientClass))
#define CLOUD_CLOUD_IS_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLOUD_TYPE_CLOUD_CLIENT))
#define CLOUD_CLOUD_IS_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), CLOUD_TYPE_CLOUD_CLIENT))
#define CLOUD_CLOUD_CLIENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CLOUD_TYPE_CLOUD_CLIENT, cloudCloudClientClass))

gboolean cloud_cloud_client_updatecph (cloudCloudIf * iface, GByteArray ** _return, const GByteArray * cph, const GByteArray * pub, const GByteArray * cuk, const gint32 version, const gchar * attribute, GError ** error);
gboolean cloud_cloud_client_send_updatecph (cloudCloudIf * iface, const GByteArray * cph, const GByteArray * pub, const GByteArray * cuk, const gint32 version, const gchar * attribute, GError ** error);
gboolean cloud_cloud_client_recv_updatecph (cloudCloudIf * iface, GByteArray ** _return, GError ** error);
void cloud_client_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
void cloud_client_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/* Cloud handler (abstract base class) */
struct _cloudCloudHandler
{
  GObject parent;
};
typedef struct _cloudCloudHandler cloudCloudHandler;

struct _cloudCloudHandlerClass
{
  GObjectClass parent;

  gboolean (*updatecph) (cloudCloudIf *iface, GByteArray ** _return, const GByteArray * cph, const GByteArray * pub, const GByteArray * cuk, const gint32 version, const gchar * attribute, GError **error);
};
typedef struct _cloudCloudHandlerClass cloudCloudHandlerClass;

GType cloud_cloud_handler_get_type (void);
#define CLOUD_TYPE_CLOUD_HANDLER (cloud_cloud_handler_get_type())
#define CLOUD_CLOUD_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLOUD_TYPE_CLOUD_HANDLER, cloudCloudHandler))
#define CLOUD_IS_CLOUD_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLOUD_TYPE_CLOUD_HANDLER))
#define CLOUD_CLOUD_HANDLER_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), CLOUD_TYPE_CLOUD_HANDLER, cloudCloudHandlerClass))
#define CLOUD_IS_CLOUD_HANDLER_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), CLOUD_TYPE_CLOUD_HANDLER))
#define CLOUD_CLOUD_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CLOUD_TYPE_CLOUD_HANDLER, cloudCloudHandlerClass))

gboolean cloud_cloud_handler_updatecph (cloudCloudIf *iface, GByteArray ** _return, const GByteArray * cph, const GByteArray * pub, const GByteArray * cuk, const gint32 version, const gchar * attribute, GError **error);

/* Cloud processor */
struct _cloudCloudProcessor
{
  ThriftDispatchProcessor parent;

  /* protected */
  cloudCloudHandler *handler;
  GHashTable *process_map;
};
typedef struct _cloudCloudProcessor cloudCloudProcessor;

struct _cloudCloudProcessorClass
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
typedef struct _cloudCloudProcessorClass cloudCloudProcessorClass;

GType cloud_cloud_processor_get_type (void);
#define CLOUD_TYPE_CLOUD_PROCESSOR (cloud_cloud_processor_get_type())
#define CLOUD_CLOUD_PROCESSOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLOUD_TYPE_CLOUD_PROCESSOR, cloudCloudProcessor))
#define CLOUD_IS_CLOUD_PROCESSOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLOUD_TYPE_CLOUD_PROCESSOR))
#define CLOUD_CLOUD_PROCESSOR_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), CLOUD_TYPE_CLOUD_PROCESSOR, cloudCloudProcessorClass))
#define CLOUD_IS_CLOUD_PROCESSOR_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), CLOUD_TYPE_CLOUD_PROCESSOR))
#define CLOUD_CLOUD_PROCESSOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CLOUD_TYPE_CLOUD_PROCESSOR, cloudCloudProcessorClass))

#endif /* CLOUD_CLOUD_H */