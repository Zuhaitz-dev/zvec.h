
#ifndef ZVEC_H
#define ZVEC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VEC_OK 0
#define VEC_ERR -1

#define DEFINE_VEC_TYPE(T)                                                                  \
typedef struct {                                                                            \
    T *data;                                                                                \
    size_t length;                                                                          \
    size_t capacity;                                                                        \
} vec_##T;                                                                                  \
                                                                                            \
static inline vec_##T vec_init_capacity_##T(size_t cap) {                                   \
    vec_##T v = {0};                                                                        \
    if (cap > 0) {                                                                          \
        v.data = calloc(cap, sizeof(T));                                                    \
        v.capacity = v.data ? cap : 0;                                                      \
    }                                                                                       \
    return v;                                                                               \
}                                                                                           \
                                                                                            \
static inline int vec_reserve_##T(vec_##T *v, size_t new_cap) {                             \
    if (new_cap <= v->capacity) return VEC_OK;                                              \
    T *new_data = realloc(v->data, new_cap * sizeof(T));                                    \
    if (!new_data) return VEC_ERR;                                                          \
    v->data = new_data;                                                                     \
    v->capacity = new_cap;                                                                  \
    return VEC_OK;                                                                          \
}                                                                                           \
                                                                                            \
static inline int vec_is_empty_##T(vec_##T *v) {                                            \
    return v->length == 0;                                                                  \
}                                                                                           \
                                                                                            \
static inline int vec_push_##T(vec_##T *v, T value) {                                       \
    if (v->length >= v->capacity) {                                                         \
        size_t new_cap = v->capacity == 0 ? 8 : v->capacity * 2;                            \
        if (vec_reserve_##T(v, new_cap) != VEC_OK) return VEC_ERR;                          \
    }                                                                                       \
    v->data[v->length++] = value;                                                           \
    return VEC_OK;                                                                          \
}                                                                                           \
                                                                                            \
static inline void vec_pop_##T(vec_##T *v) {                                                \
    if (v->length > 0) v->length--;                                                         \
}                                                                                           \
                                                                                            \
static inline void vec_shrink_to_fit_##T(vec_##T *v) {                                      \
    if (v->length == 0) {                                                                   \
        free(v->data);                                                                      \
        *v = (vec_##T){0};                                                                  \
        return;                                                                             \
    }                                                                                       \
    if (v->length == v->capacity) return;                                                   \
    T *new_data = realloc(v->data, v->length * sizeof(T));                                  \
    if (!new_data) return;                                                                  \
    v->data = new_data;                                                                     \
    v->capacity = v->length;                                                                \
}                                                                                           \
                                                                                            \
static inline T* vec_at_##T(vec_##T *v, size_t index) {                                     \
    return (index < v->length) ? &v->data[index] : NULL;                                    \
}                                                                                           \
                                                                                            \
static inline T* vec_last_##T(vec_##T *v) {                                                 \
    return (v->length > 0) ? &v->data[v->length - 1] : NULL;                                \
}                                                                                           \
                                                                                            \
static inline void vec_remove_##T(vec_##T *v, size_t index) {                               \
    if (index >= v->length) return;                                                         \
    memmove(&v->data[index], &v->data[index + 1],                                           \
            (v->length - index - 1) * sizeof(T));                                           \
    v->length--;                                                                            \
}                                                                                           \
                                                                                            \
static inline void vec_swap_remove_##T(vec_##T *v, size_t index) {                          \
    if (index >= v->length) return;                                                         \
    v->data[index] = v->data[--v->length];                                                  \
}                                                                                           \
                                                                                            \
static inline void vec_clear_##T(vec_##T *v) {                                              \
    v->length = 0;                                                                          \
}                                                                                           \
                                                                                            \
static inline void vec_free_##T(vec_##T *v) {                                               \
    free(v->data);                                                                          \
    *v = (vec_##T){0};                                                                      \
}                                                                                           \
static inline void vec_sort_##T(vec_##T *v, int (*compar)(const void *, const void *)) {    \
    if (v->length > 1) {                                                                    \
        qsort(v->data, v->length, sizeof(T), compar);                                       \
    }                                                                                       \
}

#define PUSH_ENTRY(T)     vec_##T*: vec_push_##T,
#define RESERVE_ENTRY(T)  vec_##T*: vec_reserve_##T,
#define IS_EMPTY_ENTRY(T) vec_##T*: vec_is_empty_##T,
#define AT_ENTRY(T)       vec_##T*: vec_at_##T,
#define LAST_ENTRY(T)     vec_##T*: vec_last_##T,
#define FREE_ENTRY(T)     vec_##T*: vec_free_##T,
#define POP_ENTRY(T)      vec_##T*: vec_pop_##T,
#define SHRINK_ENTRY(T)   vec_##T*: vec_shrink_to_fit_##T,
#define REMOVE_ENTRY(T)   vec_##T*: vec_remove_##T,
#define SWAP_REM_ENTRY(T) vec_##T*: vec_swap_remove_##T,
#define CLEAR_ENTRY(T)    vec_##T*: vec_clear_##T,
#define SORT_ENTRY(T)     vec_##T*: vec_sort_##T,

#define vec_push(v, val)      _Generic((v), REGISTER_TYPES(PUSH_ENTRY)      default: 0)      (v, val)
#define vec_reserve(v, cap)   _Generic((v), REGISTER_TYPES(RESERVE_ENTRY)   default: 0)      (v, cap)
#define vec_is_empty(v)       _Generic((v), REGISTER_TYPES(IS_EMPTY_ENTRY)  default: 0)      (v)
#define vec_at(v, idx)        _Generic((v), REGISTER_TYPES(AT_ENTRY)        default: (void)0)(v, idx)
#define vec_last(v)           _Generic((v), REGISTER_TYPES(LAST_ENTRY)      default: (void)0)(v)
#define vec_free(v)           _Generic((v), REGISTER_TYPES(FREE_ENTRY)      default: (void)0)(v)
#define vec_pop(v)            _Generic((v), REGISTER_TYPES(POP_ENTRY)       default: (void)0)(v)
#define vec_shrink_to_fit(v)  _Generic((v), REGISTER_TYPES(SHRINK_ENTRY)    default: (void)0)(v)
#define vec_remove(v, i)      _Generic((v), REGISTER_TYPES(REMOVE_ENTRY)    default: (void)0)(v, i)
#define vec_swap_remove(v, i) _Generic((v), REGISTER_TYPES(SWAP_REM_ENTRY)  default: (void)0)(v, i)
#define vec_clear(v)          _Generic((v), REGISTER_TYPES(CLEAR_ENTRY)     default: (void)0)(v)
#define vec_sort(v, cmp)      _Generic((v), REGISTER_TYPES(SORT_ENTRY)      default: (void)0)(v, cmp)

#define vec_init(T) {0}
#define vec_init_with_cap(T, cap) vec_init_capacity_##T(cap)

#define VEC_CAT(a, b) a##b
#define VEC_NAME(a, b) VEC_CAT(a, b)

#define vec_foreach(v, iter) \
    for (size_t VEC_NAME(_i_, __LINE__) = 0; \
         VEC_NAME(_i_, __LINE__) < (v)->length && ((iter) = &(v)->data[VEC_NAME(_i_, __LINE__)]); \
         ++VEC_NAME(_i_, __LINE__))

#endif
