#include "random_source.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>

enum
{
    POS_AMOUNT = 2,
    MIN_AMOUNT_ARGS,
    MAX_AMOUNT_NUM = 100000,
    BASE = 10
};

const char lin_type[] = "linear";
const char rand_type[] = "random";
const char lin_factory[] = "random_linear_factory";
const char rand_factory[] = "random_random_factory";

int
main(int argc, char *argv[])
{
    if (argc < MIN_AMOUNT_ARGS + 1) {
        fprintf(stderr, "Not enough arguments\n");
        return 0;
    }

    int flag_type = 0;
    if (strcmp(argv[1], lin_type) == 0) {
        flag_type = 1;
    } else if (strcmp(argv[1], rand_type) == 0) {
        flag_type = 2;
    } else {
        fprintf(stderr, "Wrong type\n");
        return 0;
    }

    void *handle = dlopen(NULL, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "Error opening library\n");
        return 0;
    }

    errno = 0;
    char *ctrl = NULL;
    long amount = strtol(argv[POS_AMOUNT], &ctrl, BASE);
    if (errno || *ctrl || ctrl == argv[POS_AMOUNT] || amount < 0 || amount > MAX_AMOUNT_NUM) {
        fprintf(stderr, "Wrong amount\n");
        return 0;
    }

    RandomSource *src = NULL;
    RandomSource *(*func)(const char *);
    if (flag_type == 1) {
        func = (RandomSource * (*) (const char *) )(dlsym(handle, lin_factory));
        src = func(argv[MIN_AMOUNT_ARGS]);
    } else if (flag_type == 2) {
        func = (RandomSource * (*) (const char *) )(dlsym(handle, lin_factory));
        src = func(argv[MIN_AMOUNT_ARGS]);
    }

    if (src == NULL) {
        dlclose(handle);
        fprintf(stderr, "Error creating random source\n");
        return 0;
    }

    for (int i = 0; i < amount; i++) {
        printf("%.10g\n", src->ops->next(src));
    }

    src->ops->destroy(src);

    return 0;
}
