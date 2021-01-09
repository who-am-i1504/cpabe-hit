/*
 * @Author: your name
 * @Date: 2020-12-31 15:19:39
 * @LastEditTime: 2021-01-08 10:21:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /thrift-example/authority.h
 */
#ifndef ATTRIBUTE_AUTHORITY_H
#define ATTRIBUTE_AUTHORITY_H
#include <glib-object.h>
#include <glib.h>
#include <thrift/c_glib/thrift.h>
#include "gen-c_glib/aa_authority.h"
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

#define TYPE_MAIN_AA_AUTHORITY_HANDLER \
  (main_aa_authority_handler_get_type ())

#define MAIN_AA_AUTHORITY_HANDLER(obj)         \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                   \
                              TYPE_MAIN_AA_AUTHORITY_HANDLER,        \
                              MainAaAuthorityHandler))
#define MAIN_AA_AUTHORITY_HANDLER_CLASS(c)                    \
  (G_TYPE_CHECK_CLASS_CAST ((c),                                \
                            TYPE_MAIN_AA_AUTHORITY_HANDLER,   \
                            MainAaAuthorityHandlerClass))
#define IS_MAIN_AA_AUTHORITY_HANDLER(obj)                             \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                   \
                               TYPE_MAIN_AA_AUTHORITY_HANDLER))
#define IS_MAIN_AA_AUTHORITY_HANDLER_CLASS(c)                 \
  (G_TYPE_CHECK_CLASS_TYPE ((c),                                \
                            TYPE_MAIN_AA_AUTHORITY_HANDLER))
#define MAIN_AA_AUTHORITY_HANDLER_GET_CLASS(obj)              \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                            \
                              TYPE_MAIN_AA_AUTHORITY_HANDLER, \
                              MainAaAuthorityHandlerClass))

struct _MainAaAuthorityHandler {
  aaAuthorityHandler parent_instance;

  GHashTable *log;
};
typedef struct _MainAaAuthorityHandler MainAaAuthorityHandler;

struct _MainAaAuthorityHandlerClass {
  aaAuthorityHandlerClass parent_instance;
};

typedef struct _MainAaAuthorityHandlerClass MainAaAuthorityHandlerClass;

GType main_aa_authority_handler_get_type(void);

G_END_DECLS

static void
main_aa_authority_handler_finalize(GObject* object);

static void
main_aa_authority_handler_init(MainAaAuthorityHandler * self);

static void
main_aa_authority_handler_class_init(MainAaAuthorityHandlerClass *klass);

static gboolean
main_aa_authority_handler_setup(aaAuthorityIf   *iface,
                                aaAuthParamter  **_return,
                                GByteArray      *pub,
                                GPtrArray       *attributes,
                                gchar           *aid,
                                GError          **error);

static gboolean
main_aa_authority_handler_keygen(aaAuthorityIf  *iface,
                                 aaARevoKey     **_return,
                                 aaAuthParamter *auth,
                                 GByteArray     *pub,
                                 GByteArray     *cert,
                                 GByteArray     *attributes,
                                 GError         **error);

static gboolean
main_aa_authority_handler_ukeygen(aaAuthorityIf *iface,
                                  aaAttrPair    **_return,
                                  GByteArray    **auth_msk,
                                  GByteArray    *pub,
                                  GByteArray    *attr_pk,
                                  GByteArray    *attr_msk,
                                  gchar         *attribute,
                                  GError        **error);

#endif