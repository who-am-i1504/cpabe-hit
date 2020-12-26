/*
 * @Author: your name
 * @Date: 2020-12-23 20:33:29
 * @LastEditTime: 2020-12-26 21:02:25
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

typedef void (*spln_cph)( GByteArray* b,
			                void* cph );
typedef void* (*uspln_cph)(void* pub,
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



#endif