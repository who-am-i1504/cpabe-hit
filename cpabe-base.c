#include <pbc/pbc.h>
#include <glib-2.0/glib.h>
#include <string.h>
#include "cpabe-base.h"

#define TYPE_A_PARAMS                                          \
    "type a\n"                                                 \
    "q 87807107996633125224377819847540498158068831994142082"  \
    "1102865339926647563088022295707862517942266222142315585"  \
    "8769582317459277713367317481324925129998224791\n"         \
    "h 12016012264891146079388821366740534204802954401251311"  \
    "822919615131047207289359704531102844802183906537786776\n" \
    "r 730750818665451621361119245571504901405976559617\n"     \
    "exp2 159\n"                                               \
    "exp1 107\n"                                               \
    "sign1 1\n"                                                \
    "sign0 1\n"

/*

// 初始化公共参水和密钥
void setup(pub_p **gp, msk_p **msk char **attributes)
{
    // 分配空间
    *gp = malloc(sizeof(pub_p));
    *msk = malloc(sizeof(msk_p));

    // 保存群的生成参数
    *(gp)->pair_desc = strdup(TYPE_A_PARAMS);
    // 生成对称群
    pairing_init_set_buf((*gp)->p, (*gp)->pair_desc, strlen((*gp)->pair_desc));
    // 绑定参数与群
    element_init_Zr((*msk)->alpha, (*gp)->p);
    element_init_G1((*gp)->g, (*gp)->p);
    element_init_GT((*gp)->egg_alpha, (*gp)->p);

    // 随机生成相关参数
    element_random((*gp)->g);
    element_random((*msk)->alpha);

    // 初始化相关参数
    (*gp)->t_arr = g_array_new(0, 1, sizeof(element_t));
    (*gp)->u_arr = g_array_new(0, 1, sizeof(element_t));
    (*gp)->attrs = g_array_new(0, 1, sizeof(char *));
    (*msk)->t_arr = g_array_new(0, 1, sizeof(element_t));

    while (*attributes)
    {
        char *attr;
        attr = strdup(*(attributes ++));

        element_t t;
        element_t t_i;
        element_t u_i;

        element_init_Zr(t, (*gp)->p);
        element_init_GT(t_i, (*gp)->p);
        element_init_GT(u_i, (*gp)->p);

        element_random(t);

        element_invert(u_i, t);
        element_pow2_zn(u_i, (*gp)->g, u_i);

        element_pow2_zn(t_i, (*gp)->g, t);

        g_array_append_val((*gp)->t_arr, t_i);
        g_array_append_val((*gp)->u_arr, u_i);
        g_array_append_val((*gp)->attrs, attr);
        g_array_append_val((*msk)->t_arr, t);
    }
}


int dk_gen(prv_s **prv, pub_p * gp, msk_p * msk, long uid, char ** attributes)
{
    *prv = malloc(sizeof(prv_s));
    element_t r;
    element_init_Zr(r, gp->p);
    element_init_GT((*prv) -> d, gp -> p);

    element_random(r);
    
    element_sub((*prv) -> d, msk->alpha, r);
    element_pow2_zn((*prv) -> d, gp -> g, (*prv) -> d);

    while(*attributes)
    {
        char ** attrL = gp -> attrs;
        int tag = 0;
        while(*attrL)
        {
            if (!strcmp(*(attrL ++), *attributes))
                tag = 1;
        }
        if (!tag) continue;
        element_t r_i;
        element_init_Zr(r_i, gp -> p);
        element_random(r_i);
    }

}
*/



void setup(pub_s ** gp)
{
    *gp = malloc(sizeof(pub_s));

    (*gp) -> pairing_desc = strdup(TYPE_A_PARAMS);

    pairing_init_set_buf((*gp)->p, (*gp) -> pairing_desc, strlen((*gp) -> pairing_desc));

    element_init_G1((*gp) -> g, (*gp) -> p);
    element_init_G1((*gp) -> h, (*gp) -> p);
    element_init_G1((*gp) -> v, (*gp) -> p);
    element_init_G1((*gp) -> u, (*gp) -> p);
    element_init_G1((*gp) -> w, (*gp) -> p);
    element_init_G1((*gp) -> alpha, (*gp) -> p);
    
    element_random((*gp) -> g);
    element_random((*gp) -> h);
    element_random((*gp) -> v);
    element_random((*gp) -> u);
    element_random((*gp) -> w);
    element_random((*gp) -> alpha);

}


