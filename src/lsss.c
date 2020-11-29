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
            // printf("%s\n", tok);
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

static GArray * array_add_element(GPtrArray * matrix, int *col, t_node * node)
{
    GArray * arr;
    arr = g_array_new(0, 1, sizeof(int));

    node -> num = *col;

    g_array_append_val(arr, node -> children -> len);
    g_array_append_val(arr, node -> k);

    g_ptr_array_insert(matrix, (*col) ++, arr);

    return arr;
}

GPtrArray * generate_access_array(t_node * policy)
{
    GPtrArray * matrix;
    matrix = g_ptr_array_new();

    int col = 0;

    GQueue * queue;
    t_node * pre = NULL;

    int leaf_node_number = -1;
    queue = g_queue_new();

    if(policy -> attr == NULL){
        array_add_element(matrix, &col, policy);
    }
    else
    {
        GArray * arr;
        arr = g_array_new(0, 1, sizeof(int));
        int val = 1;
        g_array_append_val(arr, val);
        g_array_append_val(arr, val);
        val = -1;
        g_array_append_val(arr, val);
        
        g_ptr_array_insert(matrix, arr, col ++);
    }

    g_queue_push_tail(queue, policy);

    // 层次遍历访问树结构
    while(!g_queue_is_empty(queue))
    {
        t_node * node = g_queue_pop_head(queue);
        
        GArray * parent = (GArray *)g_ptr_array_index(matrix, node -> num);
        for (int i = 0; i < node -> children -> len; i ++)
        {
            pre = (t_node *)((void *)(node -> children -> pdata[i]));
            pre -> num = col;
            if (pre -> attr != 0)
            {
                g_array_append_val(parent, leaf_node_number);
                leaf_node_number --;
            }
            else
            {
                g_array_append_val(parent, pre -> num);
                array_add_element(matrix, &col, pre);
                g_queue_push_tail(queue, pre);
            }
        }
    }
    g_queue_clear(queue);
    return matrix;
}

int visit(GPtrArray * matrix, int i, int j)
{
    GArray * arr = (GArray *)(g_ptr_array_index(matrix, i));
    return g_array_index(arr, int, j);
}

void print_matrix(GPtrArray * matrix)
{

    for (int i = 0; i < matrix -> len; i ++)
    {
        GArray * arr = (GArray *)(g_ptr_array_index(matrix, i));
        for (int j = 0; j < arr -> len; j ++)
        {
            printf("%d\t", g_array_index(arr, int, j));
        }
        printf("\n");
    }
}

char ** generate_rhos(t_node* policy)
{
    GQueue * queue;
    GArray * arr;
    t_node * pre = NULL;
    arr = g_array_new(0, 1, sizeof(char *));
    queue = g_queue_new();
    g_queue_push_tail(queue, policy);
    while(!g_queue_is_empty(queue))
    {
        t_node *node = g_queue_pop_head(queue);
        if (node -> attr != NULL)
        {
            char *att;
            att = strdup(node -> attr);
            g_array_append_val(arr, att);
        }
        else
            for (int i = 0; i < node -> children -> len; i ++)
            {
                pre = (t_node *)((void *)(node -> children -> pdata[i]));
                g_queue_push_tail(queue, pre);
            }
    }
    char ** rhos;
    rhos = (char *)malloc(sizeof(char *)*(arr->len + 1));
    GHashTable * hash = g_hash_table_new(g_str_hash, g_str_equal);
    for (int i = 0; i < arr->len; i ++)
    {
        rhos[i] = g_array_index(arr, char*, i);
        if (g_hash_table_contains(rhos[i]))
        {
            raise_error("Invalid access policy, rhso containing identical string: %s\n", rhos[i]);
            g_ptr_array_free(arr, TRUE);
            g_queue_clear(queue);
            g_hash_table_destroy(hash);
            return NULL;
        }
        g_hash_table_insert(hash, rhos[i], 1);
    }
    *(rhos + arr -> len) = NULL;
    g_hash_table_destroy(hash);
    g_ptr_array_free(arr, FALSE);
    g_queue_clear(queue);
    return rhos;
}

void print_rhos(char ** rhos)
{
    while(*rhos)
        printf("%s\n", *(rhos ++));
}

element_t* evaluate(pairing_t p, element_t x, int degree, element_t* coef)
{
    element_t *result;
    element_t temp;
    result = malloc(sizeof(element_t));
    element_init_Zr(*result, p);
    element_init_Zr(temp, p);

    element_set0(result);
    element_set1(temp);

    for (int i = 0; i < degree + 1; i ++)
    {
        for (int j = 0; j < i; j ++)
        {
            element_mul(temp, temp, x);
        }

        element_mul(temp, temp, *(coef + i));
        element_add(*result, *result, temp);
        element_set1(temp);
    }

    element_clear(temp);

    return result;
}

element_t *generate_coef(pairing_t p, int degree, element_t zero_value)
{
    element_t *res;

    res = malloc(sizeof(element_t) * (degree + 1));

    element_init_Zr(*res, p);

    element_set(*res, zero_value);

    for (int i = 0; i < degree + 1; i ++)
    {
        element_init_Zr(*(res + i), p);
        element_random(*(res + i));
    }

    return res;
}