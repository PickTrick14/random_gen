#include "random_source.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

static int fd = -1;
static const double mod = 0xffffffffU;
static const char urandom[] = "/dev/urandom";

RandomSource *
rand_destroy(RandomSource *src)
{
    if (src != NULL) {
        free(src->ops);
        free(src);
        if (close(fd) < 0) {
            fprintf(stderr, "Error closing random source\n");
            exit(1);
        }
    }
    return NULL;
}

double
rand_next(RandomSource *src)
{
    unsigned int ans = 0;
    if (read(fd, &ans, sizeof(ans)) != sizeof(ans)) {
        fprintf(stderr, "Error reading from random source\n");
        rand_destroy(src);
        exit(1);
    }
    return ans / mod;
}

RandomSource *
random_random_factory(void)
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

    fd = open(urandom, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error opening random source\n");
        free(obj->ops);
        free(obj);
        return NULL;
    }

    obj->ops->destroy = (void *) rand_destroy;
    obj->ops->next = (void *) rand_next;
    return obj;
}
