
#ifndef ZVEC_H
#define ZVEC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define VEC_OK 0
#define VEC_ERR -1

#define DEFINE_VEC_TYPE(T, Name)                                                            \
                                                                                            \
typedef T zvec_T_##Name;                                                                    \
                                                                                            \
typedef struct {                                                                            \
    T *data;                                                                                \
    size_t length;                                                                          \
    size_t capacity;                                                                        \
} vec_##Name;                                                                               \
                                                                                            \
static inline vec_##Name vec_init_capacity_##Name(size_t cap) {                             \
    vec_##Name v = {0};                                                                     \
    if (cap > 0) {                                                                          \
        v.data = calloc(cap, sizeof(T));                                                    \
        v.capacity = v.data ? cap : 0;                                                      \
    }                                                                                       \
    return v;                                                                               \
}                                                                                           \
                                                                                            \
static inline vec_##Name vec_from_array_##Name(const T *arr, size_t count) {                \
    vec_##Name v = vec_init_capacity_##Name(count);                                         \
    if (v.data) {                                                                           \
        memcpy(v.data, arr, count * sizeof(T));                                             \
        v.length = count;                                                                   \
    }                                                                                       \
    return v;                                                                               \
}                                                                                           \
                                                                                            \
static inline int vec_reserve_##Name(vec_##Name *v, size_t new_cap) {                       \
    if (new_cap <= v->capacity) return VEC_OK;                                              \
    T *new_data = realloc(v->data, new_cap * sizeof(T));                                    \
    if (!new_data) return VEC_ERR;                                                          \
    v->data = new_data;                                                                     \
    v->capacity = new_cap;                                                                  \
    return VEC_OK;                                                                          \
}                                                                                           \
                                                                                            \
static inline int vec_is_empty_##Name(vec_##Name *v) {                                      \
    return v->length == 0;                                                                  \
}                                                                                           \
                                                                                            \
static inline T* vec_push_slot_##Name(vec_##Name *v) {                                      \
    if (v->length >= v->capacity) {                                                         \
        size_t new_cap = v->capacity == 0 ? 8 : v->capacity * 2;                            \
        if (vec_reserve_##Name(v, new_cap) != VEC_OK) return NULL;                          \
    }                                                                                       \
    return &v->data[v->length++];                                                           \
}                                                                                           \
                                                                                            \
static inline int vec_push_##Name(vec_##Name *v, T value) {                                 \
    T *slot = vec_push_slot_##Name(v);                                                      \
    if (!slot) return VEC_ERR;                                                              \
    *slot = value;                                                                          \
    return VEC_OK;                                                                          \
}                                                                                           \
                                                                                            \
static inline int vec_extend_##Name(vec_##Name *v, const T *items, size_t count) {          \
    if (v->length + count > v->capacity) {                                                  \
        size_t new_cap = v->capacity == 0 ? 8 : v->capacity;                                \
        while (new_cap < v->length + count) new_cap *= 2;                                   \
        if (vec_reserve_##Name(v, new_cap) != VEC_OK) return VEC_ERR;                       \
    }                                                                                       \
    memcpy(v->data + v->length, items, count * sizeof(T));                                  \
    v->length += count;                                                                     \
    return VEC_OK;                                                                          \
}                                                                                           \
                                                                                            \
static inline void vec_pop_##Name(vec_##Name *v) {                                          \
    assert(v->length > 0 && "Popping empty vector");                                        \
    v->length--;                                                                            \
}                                                                                           \
                                                                                            \
static inline T vec_pop_get_##Name(vec_##Name *v) {                                         \
    assert(v->length > 0 && "Vector is empty, cannot pop!");                                \
    return v->data[--v->length];                                                            \
}                                                                                           \
static inline void vec_shrink_to_fit_##Name(vec_##Name *v) {                                \
    if (v->length == 0) {                                                                   \
        free(v->data);                                                                      \
        *v = (vec_##Name){0};                                                               \
        return;                                                                             \
    }                                                                                       \
    if (v->length == v->capacity) return;                                                   \
    T *new_data = realloc(v->data, v->length * sizeof(T));                                  \
    if (!new_data) return;                                                                  \
    v->data = new_data;                                                                     \
    v->capacity = v->length;                                                                \
}                                                                                           \
                                                                                            \
static inline T* vec_at_##Name(vec_##Name *v, size_t index) {                               \
    return (index < v->length) ? &v->data[index] : NULL;                                    \
}                                                                                           \
                                                                                            \
static inline T* vec_data_##Name(vec_##Name *v) {                                           \
    return v->data;                                                                         \
}                                                                                           \
                                                                                            \
static inline T* vec_last_##Name(vec_##Name *v) {                                           \
    return (v->length > 0) ? &v->data[v->length - 1] : NULL;                                \
}                                                                                           \
                                                                                            \
static inline void vec_remove_##Name(vec_##Name *v, size_t index) {                         \
    if (index >= v->length) return;                                                         \
    memmove(&v->data[index], &v->data[index + 1],                                           \
            (v->length - index - 1) * sizeof(T));                                           \
    v->length--;                                                                            \
}                                                                                           \
                                                                                            \
static inline void vec_swap_remove_##Name(vec_##Name *v, size_t index) {                    \
    if (index >= v->length) return;                                                         \
    v->data[index] = v->data[--v->length];                                                  \
}                                                                                           \
                                                                                            \
static inline void vec_clear_##Name(vec_##Name *v) {                                        \
    v->length = 0;                                                                          \
}                                                                                           \
                                                                                            \
static inline void vec_free_##Name(vec_##Name *v) {                                         \
    free(v->data);                                                                          \
    *v = (vec_##Name){0};                                                                   \
}                                                                                           \
                                                                                            \
static inline void vec_reverse_##Name(vec_##Name *v) {                                      \
    if (v->length < 2) return;                                                              \
    size_t i = 0, j = v->length - 1;                                                        \
    while (i < j) {                                                                         \
        T temp = v->data[i];                                                                \
        v->data[i] = v->data[j];                                                            \
        v->data[j] = temp;                                                                  \
        i++; j--;                                                                           \
    }                                                                                       \
}                                                                                           \
                                                                                            \
static inline void vec_sort_##Name(vec_##Name *v, int (*compar)(const T *, const T *)) {    \
    if (v->length > 1) {                                                                    \
        int (*qsort_cmp)(const void *, const void *) =                                      \
            (int (*)(const void *, const void *))compar;                                    \
        qsort(v->data, v->length, sizeof(T), qsort_cmp);                                    \
    }                                                                                       \
}                                                                                           \
                                                                                            \
static inline T* vec_bsearch_##Name(vec_##Name *v, const T *key,                            \
                                    int (*compar)(const T *, const T *)) {                  \
    if (v->length == 0) return NULL;                                                        \
    int (*bs_cmp)(const void *, const void *) =                                             \
        (int (*)(const void *, const void *))compar;                                        \
    return (T*) bsearch(key, v->data, v->length, sizeof(T), bs_cmp);                        \
}                                                                                           \
                                                                                            \
static inline T* vec_lower_bound_##Name(vec_##Name *v, const T *key,                        \
                                        int (*compar)(const T *, const T *)) {              \
    size_t l = 0;                                                                           \
    size_t r = v->length;                                                                   \
    while (l < r) {                                                                         \
        size_t mid = l + (r - l) / 2;                                                       \
        if (compar(&v->data[mid], key) < 0) {                                               \
            l = mid + 1;                                                                    \
        } else {                                                                            \
            r = mid;                                                                        \
        }                                                                                   \
    }                                                                                       \
    if (l == v->length) return NULL;                                                        \
    return &v->data[l];                                                                     \
}

#define PUSH_ENTRY(T, Name)     vec_##Name*: vec_push_##Name,
#define PUSH_SLOT_ENTRY(T, Name) vec_##Name*: vec_push_slot_##Name,
#define EXTEND_ENTRY(T, Name)   vec_##Name*: vec_extend_##Name,
#define RESERVE_ENTRY(T, Name)  vec_##Name*: vec_reserve_##Name,
#define IS_EMPTY_ENTRY(T, Name) vec_##Name*: vec_is_empty_##Name,
#define AT_ENTRY(T, Name)       vec_##Name*: vec_at_##Name,
#define DATA_ENTRY(T, Name)     vec_##Name*: vec_data_##Name,
#define LAST_ENTRY(T, Name)     vec_##Name*: vec_last_##Name,
#define FREE_ENTRY(T, Name)     vec_##Name*: vec_free_##Name,
#define POP_ENTRY(T, Name)      vec_##Name*: vec_pop_##Name,
#define POP_GET_ENTRY(T, Name)  vec_##Name*: vec_pop_get_##Name,
#define SHRINK_ENTRY(T, Name)   vec_##Name*: vec_shrink_to_fit_##Name,
#define REMOVE_ENTRY(T, Name)   vec_##Name*: vec_remove_##Name,
#define SWAP_REM_ENTRY(T, Name) vec_##Name*: vec_swap_remove_##Name,
#define CLEAR_ENTRY(T, Name)    vec_##Name*: vec_clear_##Name,
#define REVERSE_ENTRY(T, Name)  vec_##Name*: vec_reverse_##Name,
#define SORT_ENTRY(T, Name)     vec_##Name*: vec_sort_##Name,
#define BSEARCH_ENTRY(T, Name)  vec_##Name*: vec_bsearch_##Name,
#define LOWER_BOUND_ENTRY(T, Name) vec_##Name*: vec_lower_bound_##Name,

#define vec_push(v, val)          _Generic((v), REGISTER_TYPES(PUSH_ENTRY)      default: 0)       (v, val)
#define vec_push_slot(v)          _Generic((v), REGISTER_TYPES(PUSH_SLOT_ENTRY) default: (void*)0)(v)
#define vec_extend(v, arr, count) _Generic((v), REGISTER_TYPES(EXTEND_ENTRY)    default: 0)       (v, arr, count)
#define vec_reserve(v, cap)       _Generic((v), REGISTER_TYPES(RESERVE_ENTRY)   default: 0)       (v, cap)
#define vec_is_empty(v)           _Generic((v), REGISTER_TYPES(IS_EMPTY_ENTRY)  default: 0)       (v)
#define vec_at(v, idx)            _Generic((v), REGISTER_TYPES(AT_ENTRY)        default: (void*)0)(v, idx)
#define vec_data(v)               _Generic((v), REGISTER_TYPES(DATA_ENTRY)      default: (void*)0)(v)
#define vec_last(v)               _Generic((v), REGISTER_TYPES(LAST_ENTRY)      default: (void*)0)(v)
#define vec_free(v)               _Generic((v), REGISTER_TYPES(FREE_ENTRY)      default: (void)0) (v)
#define vec_pop(v)                _Generic((v), REGISTER_TYPES(POP_ENTRY)       default: (void)0) (v)
#define vec_pop_get(v)            _Generic((v), REGISTER_TYPES(POP_GET_ENTRY)   default: (void)0) (v)
#define vec_shrink_to_fit(v)      _Generic((v), REGISTER_TYPES(SHRINK_ENTRY)    default: (void)0) (v)
#define vec_remove(v, i)          _Generic((v), REGISTER_TYPES(REMOVE_ENTRY)    default: (void)0) (v, i)
#define vec_swap_remove(v, i)     _Generic((v), REGISTER_TYPES(SWAP_REM_ENTRY)  default: (void)0) (v, i)
#define vec_clear(v)              _Generic((v), REGISTER_TYPES(CLEAR_ENTRY)     default: (void)0) (v)
#define vec_reverse(v)            _Generic((v), REGISTER_TYPES(REVERSE_ENTRY)   default: (void)0) (v)
#define vec_sort(v, cmp)          _Generic((v), REGISTER_TYPES(SORT_ENTRY)      default: (void)0) (v, cmp)
#define vec_bsearch(v, k, c)      _Generic((v), REGISTER_TYPES(BSEARCH_ENTRY)     default: (void*)0)(v, k, c)
#define vec_lower_bound(v, k, c)  _Generic((v), REGISTER_TYPES(LOWER_BOUND_ENTRY) default: (void*)0)(v, k, c)

#define vec_from(Name, ...) \
    vec_from_array_##Name((zvec_T_##Name[])__VA_ARGS__, sizeof((zvec_T_##Name[])__VA_ARGS__) / sizeof(zvec_T_##Name))

#define vec_init(Name) {0}
#define vec_init_with_cap(Name, cap) vec_init_capacity_##Name(cap)

#define VEC_CAT(a, b) a##b
#define VEC_NAME(a, b) VEC_CAT(a, b)

#define vec_foreach(v, iter) \
    for (size_t VEC_NAME(_i_, __LINE__) = 0; \
         VEC_NAME(_i_, __LINE__) < (v)->length && ((iter) = &(v)->data[VEC_NAME(_i_, __LINE__)]); \
         ++VEC_NAME(_i_, __LINE__))

#endif