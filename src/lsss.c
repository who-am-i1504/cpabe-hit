#include <string.h>
#include <glib.h>
#include "abe.h"
#include "lsss.h"

char last_error[256];

char*
bswabe_error()
{
	return last_error;
}

void
raise_error(char* fmt, ...)
{
	va_list args;

#ifdef BSWABE_DEBUG
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
#else
	va_start(args, fmt);
	vsnprintf(last_error, 256, fmt, args);
	va_end(args);
#endif
}


GString * string_policy_format(char * policy)
{
    GString * str;

    str = g_string_sized_new(strlen(policy));

    while (*policy)
    {
        if (*policy == '\\')
        {
            if (*(policy + 1) == '(')
            {
                g_string_append_c(str, '(');
                g_string_append_c(str, ' ');
                policy ++;
            }
            else if (*(policy + 1) == ')')
            {
                g_string_append_c(str, ' ');
                g_string_append_c(str, ')');
                policy ++;
            }
            else
            {
                g_string_append_c(str, '\\');
            }
        }
        else
        {
            g_string_append_c(str, *policy);
        }
        policy ++;
    }
    g_string_append_c(str, 0);
    return str;
}

t_node * base_node(int k, char *s)
{
    t_node * p;
    p = (t_node *)malloc(sizeof(t_node));

    p -> k = k;

    p -> attr = s ? strdup(s) : 0;
    p -> children = g_ptr_array_new();

    return p;
}

t_node * generate_tree(char *policy)
{
    char** toks;
	char** cur_toks;
	char*  tok;
	GPtrArray* stack; /* pointers to bswabe_policy_t's */
	t_node* root;

	toks     = g_strsplit(policy, " ", 0);
	cur_toks = toks;
	stack    = g_ptr_array_new();

	while( *cur_toks )
	{
		int i, k, n;

		tok = *(cur_toks++);

		if( !*tok )
			continue;

		if( sscanf(tok, "%dof%d", &k, &n) != 2 )
			/* push leaf token */
			g_ptr_array_add(stack, base_node(1, tok));
		else
		{
			t_node* node;

			/* parse "kofn" operator */

			if( k < 1 )
			{
				raise_error("error parsing \"%s\": trivially satisfied operator \"%s\"\n", policy, tok);
				return 0;
			}
			else if( k > n )
			{
				raise_error("error parsing \"%s\": unsatisfiable operator \"%s\"\n", policy, tok);
				return 0;
			}
			else if( n == 1 )
			{
				raise_error("error parsing \"%s\": identity operator \"%s\"\n", policy, tok);
				return 0;
			}
			else if( n > stack->len )
			{
				raise_error("error parsing \"%s\": stack underflow at \"%s\"\n", policy, tok);
				return 0;
			}
			
			/* pop n things and fill in children */
			node = base_node(k, 0);
			g_ptr_array_set_size(node->children, n);
			for( i = n - 1; i >= 0; i-- )
				node->children->pdata[i] = g_ptr_array_remove_index(stack, stack->len - 1);
			
			/* push result */
			g_ptr_array_add(stack, node);
		}
	}

	if( stack->len > 1 )
	{
		raise_error("error parsing \"%s\": extra tokens left on stack\n", policy);
		return 0;
	}
	else if( stack->len < 1 )
	{
		raise_error("error parsing \"%s\": empty policy\n", policy);
		return 0;
	}

	root = g_ptr_array_index(stack, 0);

 	g_strfreev(toks);
 	g_ptr_array_free(stack, 0);

	return root;
}

GArray * generate_access_array(char * policy)
{
    GHashTable * map;
    map = 
}