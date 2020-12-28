/*
 * @Author: your name
 * @Date: 2020-12-23 20:33:29
 * @LastEditTime: 2020-12-27 17:04:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/serial.h
 */
#ifndef SERIALIZE_BASE_H
#define SERIALIZE_BASE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <pbc.h>
#include "policy.h"
#include "vector.h"

typedef void (*spln_cph)( GByteArray* b,
			                void* cph );
typedef void* (*uspln_cph)(pairing_t p,
                           GByteArray* b,
                           int *offset);

void
serialize_uint32( GByteArray* b, uint32_t k );

uint32_t
unserialize_uint32( GByteArray* b, int* offset );

void
serialize_element( GByteArray* b, element_t e );

void
unserialize_element( GByteArray* b, int* offset, element_t e );

void
serialize_string( GByteArray* b, char* s );

char*
unserialize_string( GByteArray* b, int* offset );

void
serialize_policy( GByteArray* b, 
                  bswabe_policy_t* p, 
                  spln_cph serial_cph_func);

bswabe_policy_t*
unserialize_policy( pairing_t pairing, 
					GByteArray* b, 
					int* offset, 
					uspln_cph userial_cph_func);


void 
serialize_polynomial(GByteArray* b, bswabe_polynomial_t* s);

bswabe_polynomial_t*
unserialize_polynomial(pairing_t* p, 
					   element_type type, 
					   GByteArray* b, 
					   int *offset);

#endif