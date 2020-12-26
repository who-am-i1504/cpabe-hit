/*
 * @Author: your name
 * @Date: 2020-12-17 17:04:03
 * @LastEditTime: 2020-12-24 16:35:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/vector.c
 */
#include "vector.h"


/**
 * @description: generate the random vector
 * @param {n}   the vectore contains n element
 * @param {p}   the multiMap params
 * @param {type} the element belong to 
 * @return {bswabe_polynomial_t*} the pointer of the vector
 */
bswabe_polynomial_t *
vector_init_random(int n, pairing_t p, element_type type)
{
    bswabe_polynomial_t* res;
    res = malloc(sizeof(bswabe_polynomial_t));
    res -> deg = n;
    res -> coef = malloc(sizeof(element_t) * n);
    for (int i = 0; i < n; i ++)
    {
        switch (type)
        {
            case G1:
                element_init_G1(res -> coef[i], p);
                break;
            case Zr:
                element_init_Zr(res -> coef[i], p);
                break;
            case GT:
                element_init_GT(res -> coef[i], p);
                break;
        }
        element_random(res -> coef[i]);
    }
    return res;
}
/**
 * @description: initital the vector
 * @param {n} the number of the vector
 * @param {e} set the element of vector to e
 * @return {bswabe_polynomial_t*} the pointor of vector
 */
bswabe_polynomial_t*
vector_init(int n, element_t e)
{
    bswabe_polynomial_t * res;
    res = malloc(sizeof(bswabe_polynomial_t));

    res -> deg = n;
    res -> coef = malloc(sizeof(element_t) * n);

    for (int i = 0; i < n; i ++ )
    {
        element_init_same_as(res -> coef[i], e);
        element_set(res -> coef[i], e);
    }

    return res;
}
/**
 * @description: initial the vector x1
 * @param {rx} the first element of vector
 * @param {rz} the third element of vector
 * @return {bswabe_polynomial_t*} the pointor of vector
 */
bswabe_polynomial_t*
vector_init_x1(element_t rx, element_t rz)
{
    bswabe_polynomial_t * res;
    res = malloc(sizeof(bswabe_polynomial_t));

    res -> deg = 3;
    res -> coef = malloc(sizeof(element_t) * 3);

    element_init_same_as(res -> coef[0], rx);
    element_set(res -> coef[0], rx);

    element_init_same_as(res -> coef[1], rx);
    element_set0(res -> coef[1]);

    element_init_same_as(res -> coef[2], rx);
    element_set(res -> coef[2], rz);

    return res;
}
/**
 * @description: initial the vector x2
 * @param {ry} the second element of vector
 * @param {rz}  the third element of vector
 * @return {bswabe_polynomial_t*}   the pointor of vector
 */
bswabe_polynomial_t*
vector_init_x2(element_t ry, element_t rz)
{
    bswabe_polynomial_t * res;
    res = malloc(sizeof(bswabe_polynomial_t));

    res -> deg = 3;
    res -> coef = malloc(sizeof(element_t) * 3);

    element_init_same_as(res -> coef[0], ry);
    element_set0(res -> coef[0]);

    element_init_same_as(res -> coef[1], ry);
    element_set(res -> coef[1], ry);

    element_init_same_as(res -> coef[2], ry);
    element_set(res -> coef[2], rz);

    return res;
}
/**
 * @description: initial the vector x3 (-ry*rz, -rx*rz, rx*ry)
 * @param {rx}
 * @param {ry}
 * @param {rz}
 * @return {bswabe_polynomial_t*}
 */
bswabe_polynomial_t*
vector_init_x3(element_t rx, element_t ry, element_t rz)
{
    bswabe_polynomial_t * res;
    res = malloc(sizeof(bswabe_polynomial_t));

    res -> deg = 3;
    res -> coef = malloc(sizeof(element_t) * 3);

    element_init_same_as(res -> coef[0], ry);
    element_mul(res->coef[0], ry, rz);
    element_neg(res -> coef[0], res -> coef[0]);

    element_init_same_as(res -> coef[1], ry);
    element_mul(res -> coef[1], rx, rz);
    element_neg(res -> coef[1], res -> coef[1]);

    element_init_same_as(res -> coef[2], ry);
    element_mul(res -> coef[2], rx, ry);

    return res;
}

/**
 * @description: mul1 * mul2 -> out
 * @param {out} the result
 * @param {mul1}
 * @param {mul2}
 * @return 
 */
void 
vector_mul_out(bswabe_polynomial_t * out, 
               bswabe_polynomial_t* mul1, 
               bswabe_polynomial_t * mul2)
{
    for (int i = 0; i < out->deg; i ++)
    {
        element_mul(out -> coef[i], mul1 -> coef[i], mul2 -> coef[i]);
    }
}

/**
 * @description: vector mul1 * element e
 * @param {out} the result
 * @param {mul1} the vector
 * @param {e}
 * @return {*}
 */
void 
vector_mul_element(bswabe_polynomial_t * out, 
               bswabe_polynomial_t* mul1, 
               element_t e)
{
    for (int i = 0; i < out->deg; i ++)
    {
        element_mul(out -> coef[i], mul1 -> coef[i], e);
    }
}

/**
 * @description: generate the copy of the vector
 * @param {x}   vector
 * @return {bswabe_polynomial_t*} the copy
 */
bswabe_polynomial_t *
vector_duplicate(bswabe_polynomial_t* x)
{
    bswabe_polynomial_t* res;
    res = malloc(sizeof(bswabe_polynomial_t));
    res -> deg = x -> deg;
    res -> coef = malloc(sizeof(element_t) * res -> deg);
    for (int i = 0; i < res -> deg; i ++)
    {
        element_init_same_as(res -> coef[i], x -> coef[i]);
        element_set(res -> coef[i], x -> coef[i]);
    }
    return res;
}

/**
 * @description: vector add1 + vector add2
 * @param {out} the result
 * @param {add1}
 * @param {add2}
 * @return {*}
 */
void 
vector_add_out(bswabe_polynomial_t * out, 
               bswabe_polynomial_t* add1, 
               bswabe_polynomial_t * add2)
{
    for (int i = 0; i < out->deg; i ++)
    {
        element_add(out -> coef[i], add1 -> coef[i], add2 -> coef[i]);
    }
}

bswabe_polynomial_t*
vector_mul(bswabe_polynomial_t * x1, bswabe_polynomial_t* x2)
{
    if (x1 -> deg != x2 -> deg){
        raise_error("x1's deg not equal x2's deg\n");
        return NULL;
    }
    bswabe_polynomial_t * res;
    res = malloc(sizeof(bswabe_polynomial_t));

    res -> deg = x1 -> deg;
    res -> coef = malloc(sizeof(element_t) * x1 -> deg);

    for (int i = 0; i < res -> deg; i ++)
        element_init_same_as(res -> coef[i], x1 -> coef[i]);
    vector_mul_out(res, x1, x2);
    return res;
}

/**
 * @description: the result of (mul1 . mul2)
 * @param {res} the result
 * @param {mul1}
 * @param {mul2}
 * @return 
 */
void 
vector_inner_mul_out(element_t res,
                     bswabe_polynomial_t * mul1,
                     bswabe_polynomial_t * mul2)
{
    element_t t;
    element_init_same_as(t, res);
    element_set0(res);
    for (int i = 0; i < mul1 -> deg; i ++)
    {
        element_mul(t, mul1 -> coef[i], mul2 -> coef[i]);
        element_add(res, res, t);
    }
    element_clear(t);
}

/**
 * @description: g^v . g^w
 * @param {out} the result
 * @param {x1} g^v
 * @param {x2} g^w
 * @return
 */

void
vector_inner_base_out(bswabe_polynomial_t * out, 
                      bswabe_polynomial_t * x1,
                      bswabe_polynomial_t * x2)
{
    for (int i = 0; i < x1 -> deg; i ++)
        element_mul(out -> coef[i], x1 -> coef[i], x2 -> coef[i]);
}

bswabe_polynomial_t*
vector_add(bswabe_polynomial_t * x1, bswabe_polynomial_t* x2)
{
    if (x1 -> deg != x2 -> deg){
        raise_error("x1's deg not equal x2's deg\n");
        return NULL;
    }
    bswabe_polynomial_t * res;
    res = malloc(sizeof(bswabe_polynomial_t));

    res -> deg = x1 -> deg;
    res -> coef = malloc(sizeof(element_t) * x1 -> deg);

    for (int i = 0; i < res -> deg; i ++)
        element_init_same_as(res -> coef[i], x1 -> coef[i]);
    vector_add_out(res, x1, x2);
    return res;
}
/**
 * @description: e ^ v
 * @param {out} the result
 * @param {x} vector
 * @param {e}
 * @return 
 */
void
vector_pow_base(bswabe_polynomial_t* out, 
                bswabe_polynomial_t* x, 
                element_t e)
{
    for (int i = 0; i < out -> deg; i ++)
        element_pow_zn(out -> coef[i], e, x -> coef[i]);
}

/**
 * @description: the copy of e ^ x
 * @param {x} vector
 * @param {e}
 * @return {bswabe_polynomial_t *}
 */

bswabe_polynomial_t*
vector_pow_base_new(bswabe_polynomial_t *x, element_t e)
{
    bswabe_polynomial_t * res;
    res = malloc(sizeof(bswabe_polynomial_t));
    res -> deg = x -> deg;
    res -> coef = malloc(sizeof(element_t) * res -> deg);

    for (int i = 0; i < res -> deg; i ++)
        element_init_same_as(res -> coef[i], e);
    vector_pow_base(res, x, e);
    return res;
}

/**
 * @description: e(x, y)
 * @param {r} the result
 * @param {mul1}
 * @param {mul2}
 * @param {p}   the Group 
 * @return {*}
 */
void vector_e_mul(element_t r, 
                  bswabe_polynomial_t *mul1, 
                  bswabe_polynomial_t * mul2,
                  pairing_t p)
{
    element_t t;
    element_init_same_as(t, r);
    element_set1(r);
    for (int i = 0; i < mul1 -> deg; i ++)
    {
        pairing_apply(t, mul1 -> coef[i], mul2 -> coef[i], p);

        element_mul(r, r, t);
    }
    element_clear(t);
}

void printf_vector(bswabe_polynomial_t * p)
{
    for (int i = 0; i < p -> deg; i ++)
        element_printf("%B\n", p -> coef[i]);
}

bswabe_polynomial_t*
random_vector_by_x1_x2_x3( bswabe_polynomial_t* x1,
                           bswabe_polynomial_t* x2,
                           bswabe_polynomial_t* x3,
                           element_t c1,
                           element_t c2,
                           element_t c3)
{
    bswabe_polynomial_t* m1;
    bswabe_polynomial_t* m2;
    bswabe_polynomial_t* m3;

    m1 = vector_duplicate(x1);
    m2 = vector_duplicate(x2);
    m3 = vector_duplicate(x3);

    element_random(c1);
    element_random(c2);
    element_random(c3);

    vector_mul_element(m1, m1, c1);
    vector_mul_element(m2, m2, c2);
    vector_mul_element(m3, m3, c3);
    vector_add_out(m1, m1, m2);
    vector_add_out(m1, m1, m2);
    free_polynomial(m2);
    free_polynomial(m3);
    return m1;
}

bswabe_polynomial_t*
random_vector_by_x1_x2( bswabe_polynomial_t* x1,
                        bswabe_polynomial_t* x2,
                        element_t c1,
                        element_t c2)
{
    bswabe_polynomial_t* m1;
    bswabe_polynomial_t* m2;
    m1 = vector_duplicate(x1);
    m2 = vector_duplicate(x2);
    element_random(c1);
    element_random(c2); 
    vector_mul_element(m1, m1, c1);
    vector_mul_element(m2, m2, c2);
    vector_add_out(m1, m1, m2);
    free_polynomial(m2);
    return m1;
}