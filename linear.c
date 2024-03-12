#include "random_source.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum
{
    MULT = 1103515245,
    INC = 12345,
    MOD = 2147483648,
    MASK = 0x7fffffff,
    BASE = 10
};

static unsigned int seed = 0;

RandomSource *
lin_destroy(RandomSource *src)
{
    if (src != NULL) {
        free(src->ops);
        free(src);
    }
    return NULL;
}

double
lin_next(void)
{
    seed = (seed * MULT + INC) % MOD;
    return (double) seed / (double) MOD;
}

RandomSource *
random_linear_factory(const char *params)
{
    RandomSource *obj = calloc(1, sizeof(*obj));
    if (obj == NULL) {
        return NULL;
    }

    obj->ops = calloc(1, sizeof(*(obj->ops)));
    if (obj->ops == NULL) {
        free(obj);
        return NULL;
    }

    obj->ops->destroy = (void *) lin_destroy;
    obj->ops->next = (void *) lin_next;

    char *ctrl = NULL;
    errno = 0;
    long long tmp = strtoll(params, &ctrl, BASE);
    if (errno || *ctrl || ctrl == params || tmp <= 0) {
        fprintf(stderr, "Invalid seed\n");
        return lin_destroy(obj);
    }

    seed = tmp & MASK;

    return obj;
}
