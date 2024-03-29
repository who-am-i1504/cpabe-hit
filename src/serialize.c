/*
 * @Author: white john
 * @Date: 2020-12-23 20:33:38
 * @LastEditTime: 2021-01-25 14:33:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/src/serical.c
 */
#include "serialize.h"

void
serialize_uint32( GByteArray* b, uint32_t k )
{
	int i;
	guint8 byte;

	for( i = 3; i >= 0; i-- )
	{
		byte = (k & 0xff<<(i*8))>>(i*8);
		g_byte_array_append(b, &byte, 1);
	}
}

uint32_t
unserialize_uint32( GByteArray* b, int* offset )
{
	int i;
	uint32_t r;
	if ((*offset) + 4 > b->len) return 0;
	r = 0;
	for( i = 3; i >= 0; i-- )
		r |= (b->data[(*offset)++])<<(i*8);

	return r;
}

void
serialize_element( GByteArray* b, element_t e )
{
	uint32_t len;
	unsigned char* buf;

	len = element_length_in_bytes(e);
	serialize_uint32(b, len);

	buf = (unsigned char*) malloc(len);
	element_to_bytes(buf, e);
	g_byte_array_append(b, buf, len);
	free(buf);
}

int
unserialize_element( GByteArray* b, int* offset, element_t e )
{
	uint32_t len;
	unsigned char* buf;

	len = unserialize_uint32(b, offset);
	if (len + *(offset) > b->len) return 1;
	buf = (unsigned char*) malloc(len);
	memcpy(buf, b->data + *offset, len);
	*offset += len;

	element_from_bytes(e, buf);
	free(buf);
	return 0;
}

void
serialize_string( GByteArray* b, char* s )
{
	g_byte_array_append(b, (unsigned char*) s, strlen(s) + 1);
}

char*
unserialize_string( GByteArray* b, int* offset )
{
	GString* s;
	char* r;
	char c;

	s = g_string_sized_new(32);
	while( 1 )
	{
		if (*offset >= b->len) {
			g_string_free(s, 1);
			return NULL; 
		}
		c = b->data[(*offset)++];
		if( c && c != EOF )
			g_string_append_c(s, c);
		else
			break;
	}

	r = s->str;
	g_string_free(s, 0);

	return r;
}

void
serialize_policy( GByteArray* b, 
				  bswabe_policy_t* p, 
				  spln_cph serial_cph_func)
{
	int i;

	serialize_uint32(b, (uint32_t) p->k);

	serialize_uint32(b, (uint32_t) p->children->len);
	if( p->children->len == 0 )
	{
		serialize_string( b, p->attr);
		if (serial_cph_func != NULL)
			serial_cph_func(b, p->cph);
		
	}
	else
		for( i = 0; i < p->children->len; i++ )
			serialize_policy(b, g_ptr_array_index(p->children, i), serial_cph_func);
}

bswabe_policy_t*
unserialize_policy( pairing_t pairing, 
					GByteArray* b, 
					int* offset, 
					uspln_cph userial_cph_func)
{
	int i;
	int n;
	bswabe_policy_t* p;
	if (*offset >= b->len) return NULL;
	
	p = (bswabe_policy_t*) malloc(sizeof(bswabe_policy_t));

	p->k = (int) unserialize_uint32(b, offset);
	p->attr = 0;
	p->children = g_ptr_array_new();

	n = unserialize_uint32(b, offset);
	if( n == 0 )
	{
		p->attr = unserialize_string(b, offset);
		if (p->attr == NULL)
		{
			g_ptr_array_free(p->children, 1);
			free(p);
			return NULL;
		}
		if (userial_cph_func!=NULL)
			p->cph = userial_cph_func(pairing, b, offset);
	}
	else
		for( i = 0; i < n; i++ )
			g_ptr_array_add(p->children, unserialize_policy(pairing, b, offset, userial_cph_func));

	return p;
}

void 
serialize_polynomial(GByteArray* b, bswabe_polynomial_t* s)
{
	serialize_uint32(b, (uint32_t)s->deg);
	for (int i = 0; i < s->deg; i ++)
		serialize_element(b, s->coef[i]);
}

bswabe_polynomial_t*
unserialize_polynomial(pairing_t* p, 
					   element_type type, 
					   GByteArray* b, 
					   int *offset)
{
	bswabe_polynomial_t* res;
	res = malloc(sizeof(bswabe_polynomial_t));
	res->deg = unserialize_uint32(b, offset);
	res->coef = malloc(sizeof(element_t)*res->deg);
	for (int i = 0; i < res->deg; i ++)
	{
		switch (type)
		{
		case Zr:
			element_init_Zr(res->coef[i], p);
			break;
		case G1:
			element_init_G1(res->coef[i], p);
			break;
		case GT:
			element_init_GT(res->coef[i], p);
			break;
		}
		unserialize_element(b, offset, res->coef[i]);
	}
	return res;
}
