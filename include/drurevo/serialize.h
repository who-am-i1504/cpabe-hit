/*
 * @Author: your name
 * @Date: 2020-12-26 20:53:18
 * @LastEditTime: 2021-02-16 15:43:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /cpabe-hit/include/drurevo/serialize.h
 */
#ifndef DRUREVO_SERIALIZE_H
#define DRUREVO_SERIALIZE_H
#include "./private.h"
#include "../serialize.h"
#include "./core.h"
#include "../policy.h"
void
drur_serialize_rupub(GByteArray* b,
                     drur_rupub_t* pub);

drur_rupub_t*
drur_unserialize_rupub(drur_capub_t* pub,
                       GByteArray* b,
                       int *offset);

void
drur_serialize_capub(GByteArray* b, 
                     drur_capub_t* pub);

drur_capub_t*
drur_unserialize_capub(GByteArray* b, int* offset);

void
drur_serialize_rumsk(GByteArray* b, drur_rumsk_t* msk);

drur_rumsk_t*
drur_unserialize_rumsk(drur_capub_t* pub,
                       GByteArray* b, 
                       int *offset);

void
drur_serialize_camsk(GByteArray* b, drur_camsk_t* msk);

drur_camsk_t*
drur_unserialize_camsk(drur_capub_t* pub,
                       GByteArray* b, 
                       int *offset);

void
drur_serialize_cert(GByteArray* b,
                    drur_user_cert_t* cert);

drur_user_cert_t*
drur_unserialize_cert(drur_capub_t* pub,
                      GByteArray* b, 
                      int *offset);

void 
drur_serialize_prv_kx(GByteArray* b, drur_prv_kx_t* kx);

drur_prv_kx_t* 
drur_unserialize_prv_kx(drur_capub_t* pub,
                        GByteArray* b,
                        int *offset);

void
drur_serialize_ru_sk(GByteArray* b,
                     drur_ru_sk_t* sk);

drur_ru_sk_t*
drur_unserialize_ru_sk(drur_capub_t* pub,
                       GByteArray* b,
                       int *offset);

void
drur_serialize_prv_item(GByteArray* b, drur_prv_item_t* item);

drur_prv_item_t*
drur_unserialize_prv_item(drur_capub_t* pub,
                          GByteArray* b,
                          int *offset);

void
drur_serialize_user(GByteArray* b, drur_user_t* user);

drur_user_t*
drur_unserialize_user(drur_capub_t* pub,
                      GByteArray* b,
                      int *offset);

void
drur_serialize_auth_pub(GByteArray* b, drur_auth_pub_t* pub);

drur_auth_pub_t*
drur_unserialize_auth_pub(drur_capub_t* pub,
                     GByteArray* b,
                     int *offset);

void
drur_serialize_auth_msk(GByteArray* b, drur_auth_msk_t* msk);

drur_auth_msk_t*
drur_unserialize_auth_msk(drur_capub_t* pub,
                          GByteArray* b,
                          int *offset);

void
drur_serialize_attr_pk(GByteArray* b, drur_attr_pk_t* pk);

drur_attr_pk_t*
drur_unserialize_attr_pk(drur_capub_t* pub,
                         GByteArray* b,
                         int *offset);

void 
drur_seriallize_attr_msk(GByteArray* b, drur_attr_msk_t* msk);

drur_attr_msk_t*
drur_unserialize_attr_msk(drur_capub_t* pub,
                          GByteArray* b,
                          int *offset);

void
drur_serialize_auth(GByteArray* b, drur_auth_t* auth);

drur_auth_t*
drur_unserialize_auth(drur_capub_t* pub,
                      GByteArray* b,
                      int *offset);

void
drur_serialize_auth_item(GByteArray* b, drur_auth_item_t* item);

drur_auth_item_t*
drur_unserialize_auth_item(drur_capub_t* pub,
                           GByteArray* b,
                           int *offset);

void 
drur_serialize_cph_item(GByteArray* b, drur_cph_item_t* item);

drur_cph_item_t*
drur_unserialize_cph_item(drur_capub_t* pub,
                          GByteArray* b,
                          int *offset);

void
drur_serialize_rucph(GByteArray* b, drur_rucph_t* cph);

drur_rucph_t*
drur_unserialize_rucph(drur_capub_t* pub,
                       GByteArray* b,
                       int *offset);

void
drur_serialize_cph(GByteArray* b, drur_cph_t* cph);

drur_cph_t*
drur_unserialize_cph(drur_capub_t* pub,
                     GByteArray* b,
                     int *offset);
#endif