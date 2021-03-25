/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef AA_AA_TYPES_H
#define AA_AA_TYPES_H

/* base includes */
#include <glib-object.h>
#include <thrift/c_glib/thrift_struct.h>
#include <thrift/c_glib/protocol/thrift_protocol.h>
/* custom thrift includes */

/* begin types */

/* struct AuthParamter */
struct _aaAuthParamter
{ 
  ThriftStruct parent; 

  /* public */
  gchar * aid;
  gboolean __isset_aid;
  GByteArray * auth_pk;
  gboolean __isset_auth_pk;
  GByteArray * auth_msk;
  gboolean __isset_auth_msk;
  GHashTable * attr_pks;
  gboolean __isset_attr_pks;
  GHashTable * attr_msks;
  gboolean __isset_attr_msks;
  GPtrArray * attributes;
  gboolean __isset_attributes;
};
typedef struct _aaAuthParamter aaAuthParamter;

struct _aaAuthParamterClass
{
  ThriftStructClass parent;
};
typedef struct _aaAuthParamterClass aaAuthParamterClass;

GType aa_auth_paramter_get_type (void);
#define AA_TYPE_AUTH_PARAMTER (aa_auth_paramter_get_type())
#define AA_AUTH_PARAMTER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTH_PARAMTER, aaAuthParamter))
#define AA_AUTH_PARAMTER_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_AUTH_PARAMTER, aaAuthParamterClass))
#define AA_IS_AUTH_PARAMTER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTH_PARAMTER))
#define AA_IS_AUTH_PARAMTER_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTH_PARAMTER))
#define AA_AUTH_PARAMTER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTH_PARAMTER, aaAuthParamterClass))

/* struct ARevoKey */
struct _aaARevoKey
{ 
  ThriftStruct parent; 

  /* public */
  gchar * aid;
  gboolean __isset_aid;
  GByteArray * K;
  gboolean __isset_K;
  GByteArray * L;
  gboolean __isset_L;
  GByteArray * R;
  gboolean __isset_R;
  GHashTable * kx;
  gboolean __isset_kx;
  GPtrArray * attributes;
  gboolean __isset_attributes;
};
typedef struct _aaARevoKey aaARevoKey;

struct _aaARevoKeyClass
{
  ThriftStructClass parent;
};
typedef struct _aaARevoKeyClass aaARevoKeyClass;

GType aa_a_revo_key_get_type (void);
#define AA_TYPE_A_REVO_KEY (aa_a_revo_key_get_type())
#define AA_A_REVO_KEY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_A_REVO_KEY, aaARevoKey))
#define AA_A_REVO_KEY_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_A_REVO_KEY, aaARevoKeyClass))
#define AA_IS_A_REVO_KEY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_A_REVO_KEY))
#define AA_IS_A_REVO_KEY_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_A_REVO_KEY))
#define AA_A_REVO_KEY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_A_REVO_KEY, aaARevoKeyClass))

/* struct AttrPair */
struct _aaAttrPair
{ 
  ThriftStruct parent; 

  /* public */
  GByteArray * attr_pk;
  gboolean __isset_attr_pk;
  GByteArray * attr_msk;
  gboolean __isset_attr_msk;
  GByteArray * cuk;
  gboolean __isset_cuk;
  gint32 version;
  gboolean __isset_version;
};
typedef struct _aaAttrPair aaAttrPair;

struct _aaAttrPairClass
{
  ThriftStructClass parent;
};
typedef struct _aaAttrPairClass aaAttrPairClass;

GType aa_attr_pair_get_type (void);
#define AA_TYPE_ATTR_PAIR (aa_attr_pair_get_type())
#define AA_ATTR_PAIR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_ATTR_PAIR, aaAttrPair))
#define AA_ATTR_PAIR_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_ATTR_PAIR, aaAttrPairClass))
#define AA_IS_ATTR_PAIR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_ATTR_PAIR))
#define AA_IS_ATTR_PAIR_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_ATTR_PAIR))
#define AA_ATTR_PAIR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_ATTR_PAIR, aaAttrPairClass))

/* constants */

/* struct AuthorityAasetupArgs */
struct _aaAuthorityAasetupArgs
{ 
  ThriftStruct parent; 

  /* public */
  GByteArray * pub;
  gboolean __isset_pub;
  GPtrArray * attributes;
  gboolean __isset_attributes;
  gchar * aid;
  gboolean __isset_aid;
};
typedef struct _aaAuthorityAasetupArgs aaAuthorityAasetupArgs;

struct _aaAuthorityAasetupArgsClass
{
  ThriftStructClass parent;
};
typedef struct _aaAuthorityAasetupArgsClass aaAuthorityAasetupArgsClass;

GType aa_authority_aasetup_args_get_type (void);
#define AA_TYPE_AUTHORITY_AASETUP_ARGS (aa_authority_aasetup_args_get_type())
#define AA_AUTHORITY_AASETUP_ARGS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_AASETUP_ARGS, aaAuthorityAasetupArgs))
#define AA_AUTHORITY_AASETUP_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_AUTHORITY_AASETUP_ARGS, aaAuthorityAasetupArgsClass))
#define AA_IS_AUTHORITY_AASETUP_ARGS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_AASETUP_ARGS))
#define AA_IS_AUTHORITY_AASETUP_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_AASETUP_ARGS))
#define AA_AUTHORITY_AASETUP_ARGS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_AASETUP_ARGS, aaAuthorityAasetupArgsClass))

/* struct AuthorityAasetupResult */
struct _aaAuthorityAasetupResult
{ 
  ThriftStruct parent; 

  /* public */
  aaAuthParamter * success;
  gboolean __isset_success;
};
typedef struct _aaAuthorityAasetupResult aaAuthorityAasetupResult;

struct _aaAuthorityAasetupResultClass
{
  ThriftStructClass parent;
};
typedef struct _aaAuthorityAasetupResultClass aaAuthorityAasetupResultClass;

GType aa_authority_aasetup_result_get_type (void);
#define AA_TYPE_AUTHORITY_AASETUP_RESULT (aa_authority_aasetup_result_get_type())
#define AA_AUTHORITY_AASETUP_RESULT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_AASETUP_RESULT, aaAuthorityAasetupResult))
#define AA_AUTHORITY_AASETUP_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_AUTHORITY_AASETUP_RESULT, aaAuthorityAasetupResultClass))
#define AA_IS_AUTHORITY_AASETUP_RESULT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_AASETUP_RESULT))
#define AA_IS_AUTHORITY_AASETUP_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_AASETUP_RESULT))
#define AA_AUTHORITY_AASETUP_RESULT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_AASETUP_RESULT, aaAuthorityAasetupResultClass))

/* struct AuthorityAakeygenArgs */
struct _aaAuthorityAakeygenArgs
{ 
  ThriftStruct parent; 

  /* public */
  aaAuthParamter * auth;
  gboolean __isset_auth;
  GByteArray * pub;
  gboolean __isset_pub;
  GByteArray * cert;
  gboolean __isset_cert;
  GPtrArray * attribtues;
  gboolean __isset_attribtues;
};
typedef struct _aaAuthorityAakeygenArgs aaAuthorityAakeygenArgs;

struct _aaAuthorityAakeygenArgsClass
{
  ThriftStructClass parent;
};
typedef struct _aaAuthorityAakeygenArgsClass aaAuthorityAakeygenArgsClass;

GType aa_authority_aakeygen_args_get_type (void);
#define AA_TYPE_AUTHORITY_AAKEYGEN_ARGS (aa_authority_aakeygen_args_get_type())
#define AA_AUTHORITY_AAKEYGEN_ARGS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_AAKEYGEN_ARGS, aaAuthorityAakeygenArgs))
#define AA_AUTHORITY_AAKEYGEN_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_AUTHORITY_AAKEYGEN_ARGS, aaAuthorityAakeygenArgsClass))
#define AA_IS_AUTHORITY_AAKEYGEN_ARGS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_AAKEYGEN_ARGS))
#define AA_IS_AUTHORITY_AAKEYGEN_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_AAKEYGEN_ARGS))
#define AA_AUTHORITY_AAKEYGEN_ARGS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_AAKEYGEN_ARGS, aaAuthorityAakeygenArgsClass))

/* struct AuthorityAakeygenResult */
struct _aaAuthorityAakeygenResult
{ 
  ThriftStruct parent; 

  /* public */
  aaARevoKey * success;
  gboolean __isset_success;
};
typedef struct _aaAuthorityAakeygenResult aaAuthorityAakeygenResult;

struct _aaAuthorityAakeygenResultClass
{
  ThriftStructClass parent;
};
typedef struct _aaAuthorityAakeygenResultClass aaAuthorityAakeygenResultClass;

GType aa_authority_aakeygen_result_get_type (void);
#define AA_TYPE_AUTHORITY_AAKEYGEN_RESULT (aa_authority_aakeygen_result_get_type())
#define AA_AUTHORITY_AAKEYGEN_RESULT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_AAKEYGEN_RESULT, aaAuthorityAakeygenResult))
#define AA_AUTHORITY_AAKEYGEN_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_AUTHORITY_AAKEYGEN_RESULT, aaAuthorityAakeygenResultClass))
#define AA_IS_AUTHORITY_AAKEYGEN_RESULT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_AAKEYGEN_RESULT))
#define AA_IS_AUTHORITY_AAKEYGEN_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_AAKEYGEN_RESULT))
#define AA_AUTHORITY_AAKEYGEN_RESULT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_AAKEYGEN_RESULT, aaAuthorityAakeygenResultClass))

/* struct AuthorityUpdatekeyArgs */
struct _aaAuthorityUpdatekeyArgs
{ 
  ThriftStruct parent; 

  /* public */
  GByteArray * auth_msk;
  gboolean __isset_auth_msk;
  GByteArray * pub;
  gboolean __isset_pub;
  GByteArray * attr_pk;
  gboolean __isset_attr_pk;
  GByteArray * attr_msk;
  gboolean __isset_attr_msk;
  gchar * attribute;
  gboolean __isset_attribute;
};
typedef struct _aaAuthorityUpdatekeyArgs aaAuthorityUpdatekeyArgs;

struct _aaAuthorityUpdatekeyArgsClass
{
  ThriftStructClass parent;
};
typedef struct _aaAuthorityUpdatekeyArgsClass aaAuthorityUpdatekeyArgsClass;

GType aa_authority_updatekey_args_get_type (void);
#define AA_TYPE_AUTHORITY_UPDATEKEY_ARGS (aa_authority_updatekey_args_get_type())
#define AA_AUTHORITY_UPDATEKEY_ARGS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_UPDATEKEY_ARGS, aaAuthorityUpdatekeyArgs))
#define AA_AUTHORITY_UPDATEKEY_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_AUTHORITY_UPDATEKEY_ARGS, aaAuthorityUpdatekeyArgsClass))
#define AA_IS_AUTHORITY_UPDATEKEY_ARGS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_UPDATEKEY_ARGS))
#define AA_IS_AUTHORITY_UPDATEKEY_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_UPDATEKEY_ARGS))
#define AA_AUTHORITY_UPDATEKEY_ARGS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_UPDATEKEY_ARGS, aaAuthorityUpdatekeyArgsClass))

/* struct AuthorityUpdatekeyResult */
struct _aaAuthorityUpdatekeyResult
{ 
  ThriftStruct parent; 

  /* public */
  aaAttrPair * success;
  gboolean __isset_success;
};
typedef struct _aaAuthorityUpdatekeyResult aaAuthorityUpdatekeyResult;

struct _aaAuthorityUpdatekeyResultClass
{
  ThriftStructClass parent;
};
typedef struct _aaAuthorityUpdatekeyResultClass aaAuthorityUpdatekeyResultClass;

GType aa_authority_updatekey_result_get_type (void);
#define AA_TYPE_AUTHORITY_UPDATEKEY_RESULT (aa_authority_updatekey_result_get_type())
#define AA_AUTHORITY_UPDATEKEY_RESULT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), AA_TYPE_AUTHORITY_UPDATEKEY_RESULT, aaAuthorityUpdatekeyResult))
#define AA_AUTHORITY_UPDATEKEY_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), AA__TYPE_AUTHORITY_UPDATEKEY_RESULT, aaAuthorityUpdatekeyResultClass))
#define AA_IS_AUTHORITY_UPDATEKEY_RESULT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AA_TYPE_AUTHORITY_UPDATEKEY_RESULT))
#define AA_IS_AUTHORITY_UPDATEKEY_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), AA_TYPE_AUTHORITY_UPDATEKEY_RESULT))
#define AA_AUTHORITY_UPDATEKEY_RESULT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), AA_TYPE_AUTHORITY_UPDATEKEY_RESULT, aaAuthorityUpdatekeyResultClass))

#endif /* AA_AA_TYPES_H */
