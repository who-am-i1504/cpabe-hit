/*
 * @Author: your name
 * @Date: 2021-01-08 10:41:27
 * @LastEditTime: 2021-01-08 15:32:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/cloud.h
 */
#ifndef CLOUD_STORGE_H
#define CLOUD_STORGE_H
#include <glib-object.h>
#include <glib.h>
#include <thrift/c_glib/thrift.h>
#include "gen-c_glib/cloud_cloud.h"
#include "drurevo/serialize.h"
#include "drurevo/core.h"

G_BEGIN_DECLS

/* In the C (GLib) implementation of Thrift, the actual work done by a
   server---that is, the code that runs when a client invokes a
   service method---is defined in a separate "handler" class that
   implements the service interface. Here we define the
   TutorialCalculatorHandler class, which implements the CalculatorIf
   interface and provides the behavior expected by tutorial clients.
   (Typically this code would be placed in its own module but for
   clarity this tutorial is presented entirely in a single file.)

   For each service the Thrift compiler generates an abstract base
   class from which handler implementations should inherit. In our
   case TutorialCalculatorHandler inherits from CalculatorHandler,
   defined in gen-c_glib/calculator.h.

   If you're new to GObject, try not to be intimidated by the quantity
   of code here---much of it is boilerplate and can mostly be
   copied-and-pasted from existing work. For more information refer to
   the GObject Reference Manual, available online at
   https://developer.gnome.org/gobject/. */

#define TYPE_MAIN_CLOUD_HANDLER \
  (main_cloud_handler_get_type ())

#define MAIN_CLOUD_HANDLER(obj)         \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                   \
                              TYPE_MAIN_CLOUD_HANDLER, \
                              MainCloudHandler))
#define MAIN_CLOUD_HANDLER_CLASS(c)                    \
  (G_TYPE_CHECK_CLASS_CAST ((c),                       \
                            TYPE_MAIN_CLOUD_HANDLER,   \
                            MainCloudHandlerClass))
#define IS_MAIN_CLOUD_HANDLER(obj)                     \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                   \
                            TYPE_MAIN_CLOUD_HANDLER))
#define IS_MAIN_CLOUD_HANDLER_CLASS(c)                 \
  (G_TYPE_CHECK_CLASS_TYPE ((c),                       \
                            TYPE_MAIN_CLOUD_HANDLER))
#define MAIN_CLOUD_HANDLER_GET_CLASS(obj)              \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                   \
                            TYPE_MAIN_CLOUD_HANDLER, \
                            MainCloudHandlerClass))

struct _MainCloudHandler {
  cloudCloudHandler parent_instance;
  GHashTable *log;
};
typedef struct _MainCloudHandler MainCloudHandler;

struct _MainCloudHandlerClass {
  cloudCloudHandlerClass parent_instance;
};

typedef struct _MainCloudHandlerClass MainCloudHandlerClass;

GType main_cloud_handler_get_type(void);

G_END_DECLS

static void
main_cloud_handler_finalize(GObject* object);

static void
main_cloud_handler_init(MainCloudHandler * self);

static void
main_cloud_handler_class_init(MainCloudHandlerClass *klass);

static gboolean
main_cloud_handler_updatecph(cloudCloudIf *iface,
                             GByteArray    **_return,
                             GByteArray    *cph,
                             GByteArray    *pub,
                             GByteArray    *cuk,
                             const gint32  version,
                             gchar         *attribute,
                             GError        **error);

#endif