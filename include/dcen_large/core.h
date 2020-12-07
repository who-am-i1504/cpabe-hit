#ifndef DCEN_LARGE_CORE_H
#define DCEN_LARGE_CORE_H
#include "./private.h"

void global_setup(dcen_large_pub_t ** pub, char *pairing_desc);

dcen_large_authority_keys* 
auth_setup(dcen_large_pub_t* pub, char *attribute);

dcen_large_person_key *
keygen(dcen_large_public_key* pub, char *gid, char *attribute, dcen_large_secret_key* keys);

#endif