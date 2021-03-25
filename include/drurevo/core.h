/*
 * @Author: white john
 * @Date: 2020-12-21 15:40:31
 * @LastEditTime: 2021-01-22 17:10:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/drurevo/core.h
 */
#ifndef DRUREVO_CORE_H
#define DRUREVO_CORE_H
#include <pbc.h>
#include <glib.h>
#include <math.h>
#include "./private.h"
void 
getij(int *i, 
      int *j, 
      int id, 
      int m);

void
drur_ca_rusetup(drur_capub_t* pub,
                drur_camsk_t* msk);

void
drur_casetup(drur_capub_t** pub, 
             drur_camsk_t** msk,
             int N,
             int lambda);

drur_user_t*
drur_user_register(drur_capub_t* pub,
                   char *uid,
                   GPtrArray* attributes);

drur_auth_t *
drur_aasetup(drur_capub_t* pub,
             GPtrArray* attributes,
             char* aid);

void
drur_add_attribute(drur_attr_pk_t** pkx, 
                   drur_attr_msk_t** vx, 
                   drur_capub_t* pub,
                   drur_auth_msk_t* pmsk,
                   char* attr);

drur_ru_sk_t* 
drur_cakeygen(drur_capub_t* pub,
              drur_camsk_t* msk,
              GPtrArray* attributes,
              int userid);

drur_prv_item_t*
drur_aakeygen(drur_auth_t* auth,
              drur_capub_t* pub,
              drur_user_cert_t* cert,
              GPtrArray* attributes);

void
drur_fill_policy(bswabe_policy_t* p,
                 drur_capub_t* pub,
                 element_t s1,
                 element_t s2,
                 GPtrArray* params);

drur_cph_t*
drur_encrypt(drur_capub_t* pub,
             GPtrArray* attr_pks,
             element_t m,
             char* policy,
             GArray* revo_list,
             int userindex);

int
drur_dec_detail_aa(element_t r,
                element_t exp,
                bswabe_policy_t* p,
                drur_user_t* prv,
                drur_capub_t* pub);

int
drur_dec_detail_ca(element_t r,
                element_t exp,
                bswabe_policy_t* p,
                drur_user_t* prv,
                drur_capub_t* pub);

int
drur_decrypt(drur_cph_t* cph,
             drur_capub_t* pub,
             drur_user_t* prv,
             element_t m);

void
drur_ukeygen(drur_auth_msk_t* auth_msk,
             drur_capub_t* pub,
             drur_attr_pk_t* attr_pks,
             drur_attr_msk_t* attr_msks,
             char* attribtue);

void 
drur_secret_key_update(drur_prv_item_t* prv_item,
                       drur_capub_t* pub,
                       drur_user_cert_t* cert,
                       drur_attr_msk_t* attr_msk,
                       char *attribute);

void
drur_cph_update(drur_cph_t* cph,
                drur_capub_t* pub,
                element_t cuk,
                int version,
                char *attribute);

void
drur_realse_rupub(drur_rupub_t* pub);

void 
drur_realse_capub(drur_capub_t* pub);

void
drur_realse_rumsk(drur_rumsk_t* msk);

void
drur_realse_camsk(drur_camsk_t* msk);

void
drur_realse_user_cert(drur_user_cert_t* cert);

void
realse_element_arr(element_t * tar);

void
realse_element_arr_by_size(element_t * tar, int n);

void
realse_hash_table(GHashTable* hash, func f);

void
drur_realse_ru_sk(drur_ru_sk_t* sk);

void
drur_realse_prv_kx(drur_prv_kx_t* kx);

void
drur_realse_prv_item(drur_prv_item_t* item);

void
drur_realse_user(drur_user_t* user);

void
drur_realse_auth_pub(drur_auth_pub_t* pub);

void 
drur_realse_auth_msk(drur_auth_msk_t* msk);

void
drur_realse_attr_pk(drur_attr_pk_t* pk);

void
drur_realse_attr_msk(drur_attr_msk_t* msk);

void
drur_realse_auth(drur_auth_t * auth);

void
drur_realse_auth_item(drur_auth_item_t* auth);

void
drur_realse_cph_item(drur_cph_item_t* item);

void 
drur_realse_rucph(drur_rucph_t* cph);

void
drur_realse_cph(drur_cph_t * cph);
#endif