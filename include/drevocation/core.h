/*
 * @Author: your name
 * @Date: 2020-12-17 09:30:46
 * @LastEditTime: 2020-12-20 20:41:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/drevocation/core.h
 */
#ifndef DREVOCATION_CORE_H
#define DREVOCATION_CORE_H
#include "./private.h"

void 
drevo_setup(drevo_pub_t** pub, 
            drevo_msk_t** msk);

drevo_pkx_t*
drevo_pk_setup(drevo_pub_t* pub, 
               drevo_msk_t* msk,
               char *attribute);

drevo_prv_t*
drevo_keygen(drevo_pub_t* pub,
             drevo_msk_t* msk,
             GHashTable* pkxs,
             char **attributes);

void 
drevo_enc_detail(bswabe_policy_t* p,
                 drevo_pub_t* pub,
                 GHashTable* pkxs);

drevo_cph_t*
drevo_encrypt(drevo_pub_t* pub,
              element_t m,
              GHashTable* pkxs,
              char *policy);

void
drevo_dec_detail(element_t r,
                 element_t exp,
                 bswabe_policy_t* p,
                 drevo_prv_t* prv,
                 drevo_pub_t* pub);

int
drevo_decrypt(drevo_pub_t* pub,
              drevo_cph_t* cph,
              drevo_prv_t* prv,
              element_t m);

void
drevo_ukeygen(drevo_uk_t** uk, 
              drevo_msk_t* msk,
              element_t vx);

void 
drevo_update_pkx(drevo_pkx_t* pkx, 
                 element_t vx,
                 drevo_uk_t* uk);

void
drevo_update_prv_key(drevo_prv_t* prv,
                     drevo_msk_t* msk,
                     drevo_uk_t* uk,
                     char *attribute);

void
drevo_cph_update(drevo_cph_t* cph,
                 drevo_uk_t* uk,
                 drevo_pub_t* pub,
                 char * attribute);

#endif