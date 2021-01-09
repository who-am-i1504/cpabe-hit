/*
 * @Author: your name
 * @Date: 2021-01-08 15:57:01
 * @LastEditTime: 2021-01-08 16:33:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/policy_service.h
 */
/*
 * @Author: your name
 * @Date: 2021-01-08 10:41:27
 * @LastEditTime: 2021-01-08 15:32:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/cloud.h
 */
#ifndef POLICY_SERVICE_H
#define POLICY_SERVICE_H
#include <glib-object.h>
#include <glib.h>
#include <thrift/c_glib/thrift.h>
#include "gen-c_glib/policy_policy.h"
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

#define TYPE_MAIN_POLICY_HANDLER \
  (main_policy_handler_get_type ())

#define MAIN_POLICY_HANDLER(obj)         \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                   \
                              TYPE_MAIN_POLICY_HANDLER, \
                              MainPolicyHandler))
#define MAIN_POLICY_HANDLER_CLASS(c)                    \
  (G_TYPE_CHECK_CLASS_CAST ((c),                       \
                            TYPE_MAIN_POLICY_HANDLER,   \
                            MainPolicyHandlerClass))
#define IS_MAIN_POLICY_HANDLER(obj)                     \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                   \
                            TYPE_MAIN_POLICY_HANDLER))
#define IS_MAIN_POLICY_HANDLER_CLASS(c)                 \
  (G_TYPE_CHECK_CLASS_TYPE ((c),                       \
                            TYPE_MAIN_POLICY_HANDLER))
#define MAIN_POLICY_HANDLER_GET_CLASS(obj)              \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                   \
                            TYPE_MAIN_POLICY_HANDLER, \
                            MainPolicyHandlerClass))

struct _MainPolicyHandler {
  policyPolicyHandler parent_instance;
  GHashTable *log;
};
typedef struct _MainPolicyHandler MainPolicyHandler;

struct _MainPolicyHandlerClass {
  policyPolicyHandlerClass parent_instance;
};

typedef struct _MainPolicyHandlerClass MainPolicyHandlerClass;

GType main_policy_handler_get_type(void);

G_END_DECLS

static void
main_policy_handler_finalize(GObject* object);

static void
main_policy_handler_init(MainPolicyHandler * self);

static void
main_policy_handler_class_init(MainPolicyHandlerClass *klass);

static gboolean
main_policy_handler_parse_attributes(policyPolicyIf *iface,
                                     GPtrArray      **_return,
                                     const gchar    *attribute,
                                     GError         **error);

static gboolean
main_policy_handler_parse_policy(policyPolicyIf     *iface,
                                 gchar              **_return,
                                 const gchar        *policy,
                                 GError             **error);

#endif