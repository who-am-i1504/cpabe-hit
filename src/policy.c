#include "policy.h"

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


char * generate_a1_params(int lambda)
{
    char *res;
    res = malloc(sizeof(char) * MAX_PARAMS_LENGTH);
    memset(res, 0, sizeof(res));
    mpz_t primes[NUM_PRIMES];
    mpz_t p;
    mpz_t order;
    mpz_t n;
    long l;
    char *prime;
    BIGNUM *bn = NULL;
    bn = BN_new();
    
    mpz_init(n);
    mpz_init(order);
    mpz_init(p);
    for (int i = 0; i < NUM_PRIMES; i ++)mpz_init(primes[i]);
    while(TRUE)
    {
        while(TRUE)
        {
            mpz_set_ui(order, 1);
            for (int i = 0; i < NUM_PRIMES; i ++)
            {
                int isNew = 0;
                while(!isNew)
                {
                    
                    if(!BN_generate_prime(bn, lambda, 1, NULL, NULL, NULL, NULL))
                    {
                        return NULL;
                    }
                    prime = BN_bn2dec(bn);
                    mpz_set_str(primes[i], prime, 10);
                    isNew = 1;
                    for (int j = 0; j < i; j ++)
                        if(!mpz_cmp(primes[i], primes[j]))
                        {
                            isNew = FALSE;
                            break;
                        }
                    free(prime);
                }
                mpz_mul(order, order, primes[i]);
            }
            break;
        }

        l = 4;
        mpz_mul_ui(n, order, 4);
        mpz_sub_ui(p, n, 1);

        while(!mpz_probab_prime_p(p, 10))
        {
            mpz_add(p, p, n);
            l += 4;
        }
        break;

    }

    BN_clear(bn);
    strcat(res, "type a1\n");
    strcat(res, "p ");
    int size;
    gmp_sprintf(res, "%s %Zd\n", res, p);
    gmp_sprintf(res, "%sn %Zd\n", res, order);
    for (int i = 0; i < NUM_PRIMES; i ++)
        gmp_sprintf(res, "%sn%d %Zd\n", res, i, primes[i]);
    gmp_sprintf(res, "%sl %ld", res, l);

    mpz_clear(p);
    mpz_clear(n);
    mpz_clear(order);
    for (int i = 0; i < NUM_PRIMES; i++)mpz_clear(primes[i]);
    return res;
}

void
element_from_string( element_t h, char* s )
{
	unsigned char* r;

	r = malloc(SHA_DIGEST_LENGTH);
	SHA1((unsigned char*) s, strlen(s), r);
	element_from_hash(h, r, SHA_DIGEST_LENGTH);

	free(r);
}

bswabe_policy_t*
base_node( int k, char* s )
{
	bswabe_policy_t* p;

	p = (bswabe_policy_t*) malloc(sizeof(bswabe_policy_t));
	p->k = k;
	p->attr = s ? strdup(s) : 0;
	p->children = g_ptr_array_new();
	p->q = 0;

	return p;
}

/*
	TODO convert this to use a GScanner and handle quotes and / or
	escapes to allow attributes with whitespace or = signs in them
*/

bswabe_policy_t*
parse_policy_postfix( char* s )
{
	char** toks;
	char** cur_toks;
	char*  tok;
	GPtrArray* stack; /* pointers to bswabe_policy_t's */
	bswabe_policy_t* root;
	
	toks     = g_strsplit(s, " ", 0);
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
			bswabe_policy_t* node;

			/* parse "kofn" operator */

			if( k < 1 )
			{
				raise_error("error parsing \"%s\": trivially satisfied operator \"%s\"\n", s, tok);
				return 0;
			}
			else if( k > n )
			{
				raise_error("error parsing \"%s\": unsatisfiable operator \"%s\"\n", s, tok);
				return 0;
			}
			else if( n == 1 )
			{
				raise_error("error parsing \"%s\": identity operator \"%s\"\n", s, tok);
				return 0;
			}
			else if( n > stack->len )
			{
				raise_error("error parsing \"%s\": stack underflow at \"%s\"\n", s, tok);
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
		raise_error("error parsing \"%s\": extra tokens left on stack\n", s);
		return 0;
	}
	else if( stack->len < 1 )
	{
		raise_error("error parsing \"%s\": empty policy\n", s);
		return 0;
	}

	root = g_ptr_array_index(stack, 0);

 	g_strfreev(toks);
 	g_ptr_array_free(stack, 0);

	return root;
}

void
fill_policy( bswabe_policy_t* p, 
             void* pub, 
             element_t e , 
             void* cph,
			 enc_detail enc_function)
{
	int i;
	element_t r;
	element_t t;
	
	element_init_same_as(r, e);
	element_init_same_as(t, e);

	p->q = rand_poly(p->k - 1, e);

	if( p->children->len == 0 )
	{
		enc_function(p, pub, cph);
	}
	else
		for( i = 0; i < p->children->len; i++ )
		{
			element_set_si(r, i + 1);
			eval_poly(t, p->q, r);
			fill_policy(g_ptr_array_index(p->children, i), pub, t, cph, enc_function);
		}
	free_polynomial(p->q);
	element_clear(r);
	element_clear(t);
	
}

bswabe_polynomial_t*
rand_poly( int deg, element_t zero_val )
{
	int i;
	bswabe_polynomial_t* q;

	q = (bswabe_polynomial_t*) malloc(sizeof(bswabe_polynomial_t));
	q->deg = deg;
	q->coef = (element_t*) malloc(sizeof(element_t) * (deg + 1));

	for( i = 0; i < q->deg + 1; i++ )
		element_init_same_as(q->coef[i], zero_val);

	element_set(q->coef[0], zero_val);

	for( i = 1; i < q->deg + 1; i++ )
 		element_random(q->coef[i]);

	return q;
}

void
eval_poly( element_t r,
           bswabe_polynomial_t* q, 
           element_t x )
{
	int i;
	element_t s, t;

	element_init_same_as(s, r);
	element_init_same_as(t, r);

	element_set0(r);
	element_set1(t);

	for( i = 0; i < q->deg + 1; i++ )
	{
		/* r += q->coef[i] * t */
		element_mul(s, q->coef[i], t);
		element_add(r, r, s);

		/* t *= x */
		element_mul(t, t, x);
	}

	element_clear(s);
	element_clear(t);
}

void
check_sat( bswabe_policy_t* p, GPtrArray* prv_attrs )
{
	int i, l;

	p->satisfiable = 0;
	if( p->children->len == 0 )
	{
		for( i = 0; i < prv_attrs->len; i++ )
			if( !strcmp(prv_attrs->pdata[i],
									    p->attr) )
			{
				p->satisfiable = 1;
				p->attri = i;
				break;
			}
	}
	else
	{
		for( i = 0; i < p->children->len; i++ )
			check_sat(g_ptr_array_index(p->children, i), prv_attrs);

		l = 0;
		for( i = 0; i < p->children->len; i++ )
			if( ((bswabe_policy_t*) g_ptr_array_index(p->children, i))->satisfiable )
				l++;
		if( l >= p->k )
			p->satisfiable = 1;
	}
}

int cmp2int(const void *a, const void *b)
{
	int k, l;
	k = *(int *)(a + 1);
	l = *(int *)(b + 1);
	return 
		k < l ? -1 :
		k == l ? 0 : 1;
}

void q_sort(int *c, bswabe_policy_t * p)
{
	int* mid = malloc(sizeof(int) * ( p -> children -> len) * 2);
	for (int i = 0; i <  p -> children -> len; i ++)
	{
		*(mid + i * 2) = c[i];
		*(mid + i * 2 + 1) = ((bswabe_policy_t*) g_ptr_array_index(p->children, i))->min_leaves;
	}

	qsort(mid, p -> children -> len, sizeof(int) * 2, cmp2int);
	for (int i = 0; i < p -> children -> len; i ++)
		c[i] = *(mid + i * 2);
	free(mid);
}

void
pick_sat_min_leaves( bswabe_policy_t* p )
{
	int i, k, l;
	int* c;

	assert(p->satisfiable == 1);

	if( p->children->len == 0 )
		p->min_leaves = 1;
	else
	{
		for( i = 0; i < p->children->len; i++ )
			if( ((bswabe_policy_t*) g_ptr_array_index(p->children, i))->satisfiable )
				pick_sat_min_leaves(g_ptr_array_index(p->children, i));

		c = alloca(sizeof(int) * p->children->len);
		for( i = 0; i < p->children->len; i++ )
			c[i] = i;

		q_sort(c, p);
		// cur_comp_pol = p;
		// qsort(c, p->children->len, sizeof(int), cmp_int);

		p->satl = g_array_new(0, 0, sizeof(int));
		p->min_leaves = 0;
		l = 0;

		for( i = 0; i < p->children->len && l < p->k; i++ )
			if( ((bswabe_policy_t*) g_ptr_array_index(p->children, c[i]))->satisfiable )
			{
				l++;
				p->min_leaves += ((bswabe_policy_t*) g_ptr_array_index(p->children, c[i]))->min_leaves;
				k = c[i] + 1;
				g_array_append_val(p->satl, k);
			}
		assert(l == p->k);
	}
}

void
lagrange_coef( element_t r, GArray* s, int i )
{
	int j, k;
	element_t t;

	element_init_same_as(t, r);

	element_set1(r);
	for( k = 0; k < s->len; k++ )
	{
		j = g_array_index(s, int, k);
		if( j == i )
			continue;
		element_set_si(t, - j);
		element_mul(r, r, t); /* num_muls++; */
		element_set_si(t, i - j);
		element_invert(t, t);
		element_mul(r, r, t); /* num_muls++; */
	}

	element_clear(t);
}



int
dec_internal_flatten( element_t r, 
					  element_t exp,
					  bswabe_policy_t* p,
					  void* prv, 
					  void* pub,
					  dec_leaf leaf_func)
{
	int res;
	int i;
	element_t t;
	element_t expnew;

	// element_init_Zr(t, pub->p);
    element_init_same_as(t, exp);
	element_init_same_as(expnew, exp);

	for( i = 0; i < p->satl->len; i++ )
	{
 		lagrange_coef(t, p->satl, g_array_index(p->satl, int, i));
		element_mul(expnew, exp, t); /* num_muls++; */
		res = dec_node_flatten(r, expnew, g_ptr_array_index
										 (p->children, g_array_index(p->satl, int, i) - 1), prv, pub, leaf_func);
		if(res)break;
	}

	element_clear(t);
	element_clear(expnew);
	return res;
}

int
dec_node_flatten( element_t r, 
				  element_t exp,
				  bswabe_policy_t* p,
				  void* prv, 
				  void* pub,
				  dec_leaf leaf_func)
{
	int res;
	res = 0;
	assert(p->satisfiable);
	if( p->children->len == 0 )
		res = leaf_func(r, exp, p, prv, pub);
	else
		res = dec_internal_flatten(r, exp, p, prv, pub, leaf_func);
	return res;
}

int
dec_flatten( element_t r, 
			 bswabe_policy_t* p, 
			 void* prv, 
			 void* pub,
             pairing_t pair,
			 dec_leaf leaf_func)
{
	int res;
	element_t one;

	element_init_Zr(one, pair);

	element_set1(one);
	element_set1(r);
	// element_set0(r);

	res = dec_node_flatten(r, one, p, prv, pub, leaf_func);

	element_clear(one);

	return res;
}

void free_polynomial(bswabe_polynomial_t * q)
{
	for (int i = 0; i < sizeof(q) / sizeof(bswabe_polynomial_t); i ++)
	{
		for (int j = 0; j < (q + i) -> deg; j ++)
		{
			element_clear(*(element_t *) ((q + i) -> coef[j]));
			free((q + i) -> coef[j]);
		}
	}
	free(q);
}

void
bswabe_policy_free( bswabe_policy_t* p , func realse_cph)
{
	int i;

	if( p->attr )
	{
		free(p->attr);
		realse_cph(p->cph);
	}

	for( i = 0; i < p->children->len; i++ )
		bswabe_policy_free(g_ptr_array_index(p->children, i), realse_cph);

	g_ptr_array_free(p->children, 1);

	free(p);
}
