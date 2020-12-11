#ifndef REVOCATION_CORE_H
#define REVOCATION_CORE_H
#include "./private.h"
void 
fgadr_setup(fgadr_pub_t ** pub, fgadr_msk_t ** msk);

void 
fgadr_attr_pair(fgadr_pub_t *pub,
                     fgadr_msk_t * msk, 
                     char *attribute);

fgadr_auth_sk *
fgadr_keygen_auth(fgadr_pub_t * pub,
                  element_t h,
                  element_t t,
                  element_t alpha,
                  element_t lambda,
                  char *attribute);


#endif