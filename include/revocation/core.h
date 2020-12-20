/*
 * @Author: your name
 * @Date: 2020-12-07 20:24:47
 * @LastEditTime: 2020-12-17 17:00:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/revocation/core.h
 */
#ifndef REVOCATION_CORE_H
#define REVOCATION_CORE_H
#include "./private.h"

void
arcp_setup(arcp_pub_t **pub, 
           arcp_msk_t ** msk, 
           int N);


arcp_prv_t *
arcp_keygen(arcp_pub_t *pub,
            arcp_msk_t *msk,
            char **attributes);

void arcp_enc(bswabe_policy_t* p,
              arcp_pub_t *pub,
              arcp_cph_t* cph);

arcp_cph_t *
arcp_encrypt(arcp_pub_t *pub,
             element_t m,
             GArray * revo_list,
             char *policy,
             int user_index);

void arcp_dec(element_t r,
              element_t exp,
              bswabe_policy_t* p,
              arcp_prv_t* prv,
              arcp_pub_t* pub);

int
arcp_decrypt(arcp_pub_t *pub, 
             arcp_cph_t *cph, 
             arcp_prv_t* prv, 
             element_t m);

int arcp_reencrypt( arcp_pub_t *pub, 
                    arcp_cph_t *cph, 
                    int user);
#endif