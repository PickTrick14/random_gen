#ifndef RANDOM_SRC
#define RANDOM_SRC

typedef struct
{
    void (*destroy)(void *src);
    double (*next)(void *src);
} RandomSourceOperations;

typedef struct
{
    RandomSourceOperations *ops;
} RandomSource;

#endif