/*
 * @Author: your name
 * @Date: 2020-12-31 15:19:39
 * @LastEditTime: 2021-01-04 16:45:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /thrift-example/authority.h
 */
#ifndef CENTER_AUTHORITY_H
#define CENTER_AUTHORITY_H
#include <glib-object.h>
#include <glib.h>
#include <thrift/c_glib/thrift.h>
#include "gen-c_glib/ca_center.h"
#include "drurevo/serialize.h"
#include "drurevo/core.h"
#include "policy_lang.h"

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

#define TYPE_MAIN_CA_CENTER_HANDLER \
  (main_ca_center_handler_get_type ())

#define MAIN_CA_CENTER_HANDLER(obj)         \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                   \
                              TYPE_MAIN_CA_CENTER_HANDLER,        \
                              MainCaCenterHandlerClass))
#define MAIN_CA_CENTER_HANDLER_CLASS(c)                    \
  (G_TYPE_CHECK_CLASS_CAST ((c),                                \
                            TYPE_MAIN_CA_CENTER_HANDLER,   \
                            MainCaCenterHandlerClass))
#define IS_MAIN_CA_CENTER_HANDLER(obj)                             \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                   \
                               TYPE_MAIN_CA_CENTER_HANDLER))
#define IS_MAIN_CA_CENTER_HANDLER_CLASS(c)                 \
  (G_TYPE_CHECK_CLASS_TYPE ((c),                                \
                            TYPE_MAIN_CA_CENTER_HANDLER))
#define MAIN_CA_CENTER_HANDLER_GET_CLASS(obj)              \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                            \
                              TYPE_MAIN_CA_CENTER_HANDLER, \
                              MainCaCenterHandlerClass))

struct _MainCaCenterHandler {
  caCenterHandler parent_instance;

  GHashTable *log;
};
typedef struct _MainCaCenterHandler MainCaCenterHandler;

struct _MainCaCenterHandlerClass {
  caCenterHandlerClass parent_instance;
};

typedef struct _MainCaCenterHandlerClass MainCaCenterHandlerClass;

GType main_ca_center_handler_get_type(void);

G_END_DECLS

static void
main_ca_center_handler_finalize(GObject* object);

static void
main_ca_center_handler_init(MainCaCenterHandler * self);

static void
main_ca_center_handler_class_init(MainCaCenterHandlerClass *klass);

static gboolean
main_ca_center_handler_setup(caCenterIf     *iface,
                             caCAParamter   **_return,
                             const gint32   N,
                             const gint32   byte_size,
                             GError         **error);

static gboolean
main_ca_center_handler_keygen(caCenterIf     *iface,
                              caURevoKey     **_return,
                              caCAParamter   *param,
                              GPtrArray      *attributes,
                              const gint32   uid,
                              GError         **error);

static gboolean
main_ca_center_handler_user_register(caCenterIf     *iface,
                                     caUserRegister **_return,
                                     GByteArray     *pub,
                                     gchar          *uid,
                                     GPtrArray*     attributes,
                                     GError         **error);

#endif