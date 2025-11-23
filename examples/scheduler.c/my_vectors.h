
#ifndef MY_VECTORS_H
#define MY_VECTORS_H

#include "zvec.h"

typedef struct 
{
    int id;
    int priority;       // 0 = Low, 100 = Critical
    char payload[1024]; // Heavy data chunk (1KB)
} Task;

#define REGISTER_TYPES(X)      \
    X(int, int)                \
    X(Task, Task)

REGISTER_TYPES(DEFINE_VEC_TYPE)

#endif