#ifndef REVOCATION_CORE_H
#define REVOCATION_CORE_H
#include "./private.h"

arpu_pub_t *
global_setup(int lambda);

void 
aa_setup(arpu_auth_apk_t** pk, 
         arpu_auth_ask_t** sk, 
         arpu_pub_t * pub);

#endif