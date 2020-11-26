#include<pbc/pbc.h>
#include<glib-2.0/glib.h>

/*

struct pub_param {
    char * pair_desc;
    pairint_t p;
    GArray * t_arr;
    GArray * u_arr;
    GArray * attrs;
    element_t egg_alpha;
    element_t g;
};

typedef struct pub_param pub_p;

struct master_key {
    element_t alpha;
    GArray * t_arr;
};

typedef struct master_key msk_p;

struct prv_secert_key {
    GAarry * d_arr;
    GArray * d_arr1;
    element_t d;
};

typedef struct prv_secert_key prv_s;
*/

struct pub_params {
    element_t h;
    element_t v;
    element_t w;
    element_t u;
    element_t alpha;
    pairing_t p;
    element_t g;

    char * pairing_desc;
};

typedef struct pub_params pub_s;
