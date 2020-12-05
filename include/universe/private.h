/*
	Include glib.h, pbc.h, and bswabe.h before including this file.
*/
#ifndef UNIVERSE_PRIVATE_H
#define UNIVERSE_PRIVATE_H
#include <pbc.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include "policy.h"
#include <assert.h>

struct bswabe_pub_s
{
	char* pairing_desc;
	pairing_t p;
	element_t g;           /* G_1 */
	element_t u;
	element_t h;           /* G_1 */
	element_t w;
	element_t v;
	element_t egg_alpha; /* G_T */
};

struct bswabe_msk_s
{
	element_t alpha; /* G_2 */
};

typedef struct
{
	/* these actually get serialized */
	char* attr;
	element_t *k2i;
	element_t *k3i;

	/* only used during dec (only by dec_merge) */
	int used;
	element_t z;  /* G_1 */
	element_t zp; /* G_1 */
}
bswabe_prv_comp_t;

struct bswabe_prv_s
{
	element_t k0;
	element_t k1;
	GHashTable * k2s;
	GHashTable * k3s;
	GPtrArray* comps; /* bswabe_prv_comp_t's */
};

struct bswabe_cph_s
{
	element_t cs; /* G_T */
	element_t c0;  /* G_1 */
	GHashTable * c1;
	GHashTable * c2;
	GHashTable * c3;
	bswabe_policy_t* p;
};


/*
  A public key.
*/
typedef struct bswabe_pub_s bswabe_pub_t;

/*
  A master secret key.
*/
typedef struct bswabe_msk_s bswabe_msk_t;

/*
  A private key.
*/
typedef struct bswabe_prv_s bswabe_prv_t;

/*
  A ciphertext. Note that this library only handles encrypting a
  single group element, so if you want to encrypt something bigger,
  you will have to use that group element as a symmetric key for
  hybrid encryption (which you do yourself).
*/
typedef struct bswabe_cph_s bswabe_cph_t;

#endif