/*
 * @Author: your name
 * @Date: 2020-12-17 16:59:45
 * @LastEditTime: 2020-12-17 17:05:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/vector.h
 */

#include "policy.h"

typedef enum{
    G1, Zr, GT
} element_type;

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

void printf_vector(bswabe_polynomial_t * p);

