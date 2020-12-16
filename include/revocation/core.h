#ifndef REVOCATION_CORE_H
#define REVOCATION_CORE_H
#include "./private.h"


bswabe_polynomial_t *
vector_init_random(int n, pairing_t p, element_type type);


bswabe_polynomial_t*
vector_init(int n, element_t e);


bswabe_polynomial_t*
vector_init_x1(element_t rx, element_t rz);

bswabe_polynomial_t*
vector_init_x2(element_t ry, element_t rz);

bswabe_polynomial_t*
vector_init_x3(element_t rx, element_t ry, element_t rz);

void 
vector_mul_out(bswabe_polynomial_t * out, 
               bswabe_polynomial_t* mul1, 
               bswabe_polynomial_t * mul2);

void 
vector_mul_element(bswabe_polynomial_t * out, 
               bswabe_polynomial_t* mul1, 
               element_t e);

bswabe_polynomial_t *
vector_duplicate(bswabe_polynomial_t* x);

void 
vector_add_out(bswabe_polynomial_t * out, 
               bswabe_polynomial_t* mul1, 
               bswabe_polynomial_t * mul2);

bswabe_polynomial_t*
vector_mul(bswabe_polynomial_t * x1, bswabe_polynomial_t* x2);

void 
vector_inner_mul_out(element_t res,
                     bswabe_polynomial_t * mul1,
                     bswabe_polynomial_t * mul2);

bswabe_polynomial_t*
vector_add(bswabe_polynomial_t * x1, bswabe_polynomial_t* x2);

void
vector_pow_base(bswabe_polynomial_t* out, 
                bswabe_polynomial_t* x, 
                element_t e);

bswabe_polynomial_t*
vector_pow_base_new(bswabe_polynomial_t *x, element_t e);

void vector_e_mul(element_t r, 
                  bswabe_polynomial_t *mul1, 
                  bswabe_polynomial_t * mul2,
                  pairing_t p);

void
arcp_setup(arcp_pub_t **pub, 
           arcp_msk_t ** msk, 
           int N);


arcp_prv_t *
arcp_keygen(arcp_pub_t *pub,
            arcp_msk_t *msk,
            char **attributes);

void arcp_enc(bswabe_policy_t* p,
              arcp_pub_t *pub,
              arcp_cph_t* cph);

arcp_cph_t *
arcp_encrypt(arcp_pub_t *pub,
             element_t m,
             GArray * revo_list,
             char *policy,
             int user_index);

void arcp_dec(element_t r,
              element_t exp,
              bswabe_policy_t* p,
              arcp_prv_t* prv,
              arcp_pub_t* pub);

int
arcp_decrypt(arcp_pub_t *pub, 
             arcp_cph_t *cph, 
             arcp_prv_t* prv, 
             element_t m);

#endif