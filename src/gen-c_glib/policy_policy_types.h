/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef POLICY_POLICY_TYPES_H
#define POLICY_POLICY_TYPES_H

/* base includes */
#include <glib-object.h>
#include <thrift/c_glib/thrift_struct.h>
#include <thrift/c_glib/protocol/thrift_protocol.h>
/* custom thrift includes */

/* begin types */

/* constants */

/* struct PolicyParseAttributesArgs */
struct _policyPolicyParseAttributesArgs
{ 
  ThriftStruct parent; 

  /* public */
  gchar * attribute;
  gboolean __isset_attribute;
};
typedef struct _policyPolicyParseAttributesArgs policyPolicyParseAttributesArgs;

struct _policyPolicyParseAttributesArgsClass
{
  ThriftStructClass parent;
};
typedef struct _policyPolicyParseAttributesArgsClass policyPolicyParseAttributesArgsClass;

GType policy_policy_parse_attributes_args_get_type (void);
#define POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_ARGS (policy_policy_parse_attributes_args_get_type())
#define POLICY_POLICY_PARSE_ATTRIBUTES_ARGS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_ARGS, policyPolicyParseAttributesArgs))
#define POLICY_POLICY_PARSE_ATTRIBUTES_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), POLICY__TYPE_POLICY_PARSE_ATTRIBUTES_ARGS, policyPolicyParseAttributesArgsClass))
#define POLICY_IS_POLICY_PARSE_ATTRIBUTES_ARGS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_ARGS))
#define POLICY_IS_POLICY_PARSE_ATTRIBUTES_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_ARGS))
#define POLICY_POLICY_PARSE_ATTRIBUTES_ARGS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_ARGS, policyPolicyParseAttributesArgsClass))

/* struct PolicyParseAttributesResult */
struct _policyPolicyParseAttributesResult
{ 
  ThriftStruct parent; 

  /* public */
  GPtrArray * success;
  gboolean __isset_success;
};
typedef struct _policyPolicyParseAttributesResult policyPolicyParseAttributesResult;

struct _policyPolicyParseAttributesResultClass
{
  ThriftStructClass parent;
};
typedef struct _policyPolicyParseAttributesResultClass policyPolicyParseAttributesResultClass;

GType policy_policy_parse_attributes_result_get_type (void);
#define POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_RESULT (policy_policy_parse_attributes_result_get_type())
#define POLICY_POLICY_PARSE_ATTRIBUTES_RESULT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_RESULT, policyPolicyParseAttributesResult))
#define POLICY_POLICY_PARSE_ATTRIBUTES_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), POLICY__TYPE_POLICY_PARSE_ATTRIBUTES_RESULT, policyPolicyParseAttributesResultClass))
#define POLICY_IS_POLICY_PARSE_ATTRIBUTES_RESULT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_RESULT))
#define POLICY_IS_POLICY_PARSE_ATTRIBUTES_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_RESULT))
#define POLICY_POLICY_PARSE_ATTRIBUTES_RESULT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), POLICY_TYPE_POLICY_PARSE_ATTRIBUTES_RESULT, policyPolicyParseAttributesResultClass))

/* struct PolicyParsePolicyArgs */
struct _policyPolicyParsePolicyArgs
{ 
  ThriftStruct parent; 

  /* public */
  gchar * policy;
  gboolean __isset_policy;
};
typedef struct _policyPolicyParsePolicyArgs policyPolicyParsePolicyArgs;

struct _policyPolicyParsePolicyArgsClass
{
  ThriftStructClass parent;
};
typedef struct _policyPolicyParsePolicyArgsClass policyPolicyParsePolicyArgsClass;

GType policy_policy_parse_policy_args_get_type (void);
#define POLICY_TYPE_POLICY_PARSE_POLICY_ARGS (policy_policy_parse_policy_args_get_type())
#define POLICY_POLICY_PARSE_POLICY_ARGS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), POLICY_TYPE_POLICY_PARSE_POLICY_ARGS, policyPolicyParsePolicyArgs))
#define POLICY_POLICY_PARSE_POLICY_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), POLICY__TYPE_POLICY_PARSE_POLICY_ARGS, policyPolicyParsePolicyArgsClass))
#define POLICY_IS_POLICY_PARSE_POLICY_ARGS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), POLICY_TYPE_POLICY_PARSE_POLICY_ARGS))
#define POLICY_IS_POLICY_PARSE_POLICY_ARGS_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), POLICY_TYPE_POLICY_PARSE_POLICY_ARGS))
#define POLICY_POLICY_PARSE_POLICY_ARGS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), POLICY_TYPE_POLICY_PARSE_POLICY_ARGS, policyPolicyParsePolicyArgsClass))

/* struct PolicyParsePolicyResult */
struct _policyPolicyParsePolicyResult
{ 
  ThriftStruct parent; 

  /* public */
  gchar * success;
  gboolean __isset_success;
};
typedef struct _policyPolicyParsePolicyResult policyPolicyParsePolicyResult;

struct _policyPolicyParsePolicyResultClass
{
  ThriftStructClass parent;
};
typedef struct _policyPolicyParsePolicyResultClass policyPolicyParsePolicyResultClass;

GType policy_policy_parse_policy_result_get_type (void);
#define POLICY_TYPE_POLICY_PARSE_POLICY_RESULT (policy_policy_parse_policy_result_get_type())
#define POLICY_POLICY_PARSE_POLICY_RESULT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), POLICY_TYPE_POLICY_PARSE_POLICY_RESULT, policyPolicyParsePolicyResult))
#define POLICY_POLICY_PARSE_POLICY_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), POLICY__TYPE_POLICY_PARSE_POLICY_RESULT, policyPolicyParsePolicyResultClass))
#define POLICY_IS_POLICY_PARSE_POLICY_RESULT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), POLICY_TYPE_POLICY_PARSE_POLICY_RESULT))
#define POLICY_IS_POLICY_PARSE_POLICY_RESULT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), POLICY_TYPE_POLICY_PARSE_POLICY_RESULT))
#define POLICY_POLICY_PARSE_POLICY_RESULT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), POLICY_TYPE_POLICY_PARSE_POLICY_RESULT, policyPolicyParsePolicyResultClass))

#endif /* POLICY_POLICY_TYPES_H */
