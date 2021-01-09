/*
 * @Author: your name
 * @Date: 2020-12-31 15:19:39
 * @LastEditTime: 2021-01-05 21:52:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /thrift-example/authority.h
 */
#ifndef USER_AUTHORITY_H
#define USER_AUTHORITY_H
#include <glib-object.h>
#include <glib.h>
#include <thrift/c_glib/thrift.h>
#include "gen-c_glib/user_user.h"
#include "drurevo/serialize.h"
#include "drurevo/core.h"
#include "common.h"
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

#define TYPE_MAIN_USER_AUTHORITY_HANDLER \
  (main_user_authority_handler_get_type ())

#define MAIN_USER_AUTHORITY_HANDLER(obj)         \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                   \
                              TYPE_MAIN_USER_AUTHORITY_HANDLER,        \
                              MainUserAuthorityHandler))
#define MAIN_USER_AUTHORITY_HANDLER_CLASS(c)                    \
  (G_TYPE_CHECK_CLASS_CAST ((c),                                \
                            TYPE_MAIN_USER_AUTHORITY_HANDLER,   \
                            MainUserAuthorityHandlerClass))
#define IS_MAIN_USER_AUTHORITY_HANDLER(obj)                             \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                   \
                               TYPE_MAIN_USER_AUTHORITY_HANDLER))
#define IS_MAIN_USER_AUTHORITY_HANDLER_CLASS(c)                 \
  (G_TYPE_CHECK_CLASS_TYPE ((c),                                \
                            TYPE_MAIN_USER_AUTHORITY_HANDLER))
#define MAIN_USER_AUTHORITY_HANDLER_GET_CLASS(obj)              \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                            \
                              TYPE_MAIN_USER_AUTHORITY_HANDLER, \
                              MainUserAuthorityHandlerClass))

struct _MainUserAuthorityHandler {
  userUserHandler parent_instance;

  GHashTable *log;
};
typedef struct _MainUserAuthorityHandler MainUserAuthorityHandler;

struct _MainUserAuthorityHandlerClass {
  userUserHandlerClass parent_instance;
};

typedef struct _MainUserAuthorityHandlerClass MainUserAuthorityHandlerClass;

GType main_user_authority_handler_get_type(void);

G_END_DECLS

static void
main_user_authority_handler_finalize(GObject* object);

static void
main_user_authority_handler_init(MainUserAuthorityHandler * self);

static void
main_user_authority_handler_class_init(MainUserAuthorityHandlerClass *klass);

static drur_auth_item_t* 
transfer(userAuthPK* apk, drur_capub_t *pub);

static gboolean
main_user_authority_handler_encrypt(userUserIf   *iface,
                                    GByteArray   **_return,
                                    GByteArray   *pub,
                                    GPtrArray    *attr_pks,
                                    gchar        *policy,
                                    GArray       *revo_list,
                                    const gint32 uindex,
                                    GByteArray   *mes,
                                    GError       **error);

static gboolean
main_user_authority_handler_decrypt(userUserIf     *iface,
                                    GPtrArray      **_return,
                                    GByteArray     *cph,
                                    GByteArray     *pub,
                                    userUserPrvKey *prv,
                                    GError         **error);

#endif