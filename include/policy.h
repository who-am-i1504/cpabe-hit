#ifndef POLICY_H
#define POLICY_H
#include <pbc.h>
#include <glib.h>
#include <openssl/sha.h>
#include <assert.h>

typedef struct
{
	int deg;
	/* coefficients from [0] x^0 to [deg] x^deg */
	element_t* coef; /* G_T (of length deg + 1) */
}
bswabe_polynomial_t;

typedef struct
{
    void * cph;
	/* serialized */
	int k;            /* one if leaf, otherwise threshold */
	char* attr;       /* attribute string if leaf, otherwise null */
	
	GPtrArray* children; /* pointers to bswabe_policy_t's, len == 0 for leaves */

	/* only used during encryption */
	bswabe_polynomial_t* q;

	/* only used during decryption */
	int satisfiable;
	int min_leaves;
	int attri;
	GArray* satl;
}
bswabe_policy_t;

typedef void (*dec_leaf)(element_t r, 
                         element_t exp, 
                         bswabe_policy_t *p,
                         void * prv,
                         void * pub);
typedef void (*enc_detail)( bswabe_policy_t* p, 
                            void* pub, 
			                void* cph );
char* bswabe_error();
void raise_error(char* fmt, ...);
void element_from_string( element_t h, char* s );

bswabe_policy_t* base_node( int k, char* s );
bswabe_policy_t* parse_policy_postfix( char* s );
void fill_policy( bswabe_policy_t* p, 
             void* pub, 
             element_t e , 
             void* cph,
			 enc_detail enc_function);
void eval_poly( element_t r, bswabe_polynomial_t* q, element_t x );
bswabe_polynomial_t* rand_poly( int deg, element_t zero_val );

void check_sat( bswabe_policy_t* p, GPtrArray* prv_attrs );

void pick_sat_min_leaves( bswabe_policy_t* p );
void q_sort(int *c, bswabe_policy_t * p);
int cmp2int(const void *a, const void *b);
void lagrange_coef( element_t r, GArray* s, int i );
void dec_internal_flatten( element_t r, element_t exp, bswabe_policy_t* p, void* prv, void* pub, dec_leaf leaf_func);
void dec_node_flatten( element_t r, element_t exp, bswabe_policy_t* p, void* prv, void* pub, dec_leaf leaf_func);
void dec_flatten( element_t r, bswabe_policy_t* p, void* prv, void* pub, pairing_t pair, dec_leaf leaf_func);

#endif