
#ifndef MY_VECTORS_H
#define MY_VECTORS_H

#include "zvec.h"

typedef struct {
    float x, y;
} Point;

#define REGISTER_TYPES(X) \
    X(int, int)           \
    X(float, float)       \
    X(Point, Point)

REGISTER_TYPES(DEFINE_VEC_TYPE)

#endif
