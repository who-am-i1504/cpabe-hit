#include "decentral/core.h"

char * generate_a1_params(int lambda)
{
    char *res;
    res = malloc(sizeof(char) * MAX_PARAMS_LENGTH);
    memset(res, 0, sizeof(res));
    strcat(res, "type a1\n");
    
}