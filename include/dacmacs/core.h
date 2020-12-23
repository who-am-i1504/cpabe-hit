/*
 * @Author: your name
 * @Date: 2020-12-22 15:33:05
 * @LastEditTime: 2020-12-23 20:06:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/dacmacs/core.h
 */

#ifndef DACMACS_CORE_H
#define DACMACS_CORE_H
#include "./private.h"
void 
dacamacs_casetup(dacmacs_capub_t** pub,
                 dacmacs_camsk_t** msk);

dacmacs_user_t*
dacmacs_user_register(dacmacs_capub_t* pub,
                      char *uid,
                      GPtrArray* attributes);

dacmacs_auth_t*
dacmacs_aasetup(dacmacs_capub_t* pub,
                GPtrArray* attributes,
                char * aid);

dacmacs_prv_item_t *
dacmacs_keygen(dacmacs_auth_t* auth,
               dacmacs_capub_t* pub,
               dacmacs_user_cert_t* cert,
               GPtrArray *attributes);

void
dacmacs_enc_detail(bswabe_policy_t* p,
                   dacmacs_capub_t* pub,
                   GPtrArray* params);

dacmacs_cph_t*
dacmacs_encrypt(dacmacs_capub_t* pub,
                GPtrArray* attr_pks,
                element_t m,
                char *policy);

int
dacmacs_dec_detail(element_t r,
                   element_t exp,
                   bswabe_policy_t* p,
                   dacmacs_user_t* prv,
                   dacmacs_capub_t* pub);

int
dacmacs_decrypt(dacmacs_cph_t* cph,
                dacmacs_capub_t* pub,
                dacmacs_user_t* prv,
                element_t m);

void
dacmacs_ukeygen(dacmacs_auth_msk_t* auth_msk,
                dacmacs_capub_t* pub,
                dacmacs_attr_pk_t* attr_pks,
                dacmacs_attr_msk_t* attr_msks,
                char* attribute);

void
dacmacs_secret_key_update(dacmacs_prv_item_t *prv_item,
                          dacmacs_capub_t* pub,
                          dacmacs_user_cert_t* cert,
                          dacmacs_attr_msk_t* attr_msk,
                          char *attribute);

void
dacmacs_cph_update(dacmacs_cph_t* cph,
                   dacmacs_capub_t* pub,
                   element_t cuk,
                   int version,
                   char *attribute);

#endif

