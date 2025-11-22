# zvec.h

`zvec.h` provides dynamic arrays (vectors) for C projects. Unlike typical C vector implementations that rely on `void*` casting and lose type safety, `zvec.h` uses C11 `_Generic` selection and X-Macros to generate fully typed, type-safe implementations for your specific data structures.

## Features

* **Type Safety**: Compiler errors if you try to push a float into a vec_int.
* **Native Performance**: Data is stored in contiguous arrays of the actual type (no boxing or pointer indirection overhead).
* **Header Only**: No build scripts or linking required.
* **C11 Generics**: One API (`vec_push`, `vec_at`, etc.) works for all registered types.
* **Zero Dependencies**: Only standard C headers used.

## Installation & Setup

Since `zvec.h` generates code for your specific types, you don't just include the library: you create a Registry Header.

> You can include the logic inside the source file, but if you are going to use the library in more than one, this is implementation prevents code duplication.

### 1. Add the library

Copy `zvec.h` into your project's include directory.

### 2. Create a Registry Header

Create a file named `my_vectors.h` (or similar) to define which types need vectors.

```c
// my_vectors.h
#ifndef MY_VECTORS_H
#define MY_VECTORS_H

#include "zvec.h"

// You can keep custom struct definitions, but it's optional.
typedef struct {
    float x, y;
} Point;

// Register Types (The X-Macro):
// Syntax: X(ActualType, ShortName).
// - ActualType: The C type (e.g., 'unsigned long', 'struct Point').
// - ShortName:  Suffix for the generated functions (e.g., 'ulong', 'Point').
#define REGISTER_TYPES(X)     \
    X(int, int)               \
    X(unsigned long, ulong)   \
    X(Point, Point)

// This generates the implementation for you.
REGISTER_TYPES(DEFINE_VEC_TYPE)

#endif
```

### 3. Use in your code

Include your **registry header** (`my_vectors.h`), not `zvec.h`.

```c
#include <stdio.h>
#include "my_vectors.h"

int main(void)
{
    // Initialize (allocated on stack, internal data on heap).
    vec_int nums = vec_init(int);

    // Push values.
    vec_push(&nums, 10);
    vec_push(&nums, 20);

    // Iterate.
    int *n;
    vec_foreach(&nums, n)
    {
        printf("%d ", *n);
    }

    // Cleanup.
    vec_free(&nums);
    return 0;
}
```

## API Reference

`zvec.h` uses C11 `_Generic` to automatically select the correct function implementation based on the vector type you pass.

### Initialization & Management

| Macro | Description |
| :--- | :--- |
| `vec_init(Type)` | Returns an empty vector structure initialized to zero. |
| `vec_init_with_cap(Type, n)` | Returns a vector with initial memory reserved for `n` elements. |
| `vec_free(v)` | Frees the internal memory array and zeroes the vector structure. |
| `vec_clear(v)` | Sets the length to 0 but keeps the allocated memory capacity. |
| `vec_reserve(v, n)` | Ensures the vector has capacity for at least `n` elements total. |
| `vec_shrink_to_fit(v)` | Reallocates the internal memory to match exactly the current length. |

### Data Access

| Macro | Description |
| :--- | :--- |
| `vec_at(v, index)` | Returns a **pointer** to the element at `index`, or `NULL` if out of bounds. |
| `vec_last(v)` | Returns a **pointer** to the last element in the vector. |
| `vec_data(v)` | Returns the raw underlying array pointer (`T*`). |
| `vec_is_empty(v)` | Returns `1` (true) if the vector length is 0, otherwise `0`. |

### Modification

| Macro | Description |
| :--- | :--- |
| `vec_push(v, value)` | Appends value to the end. Copies data twice (stack -> heap). Best for simple types. |
| `vec_push_slot(v, value)` | Reserves space at the end and returns a **pointer** to it. Zero overhead. Best for large structs. |
| `vec_pop(v)` | Removes the last element. Decrements length. |
| `vec_pop_get(v)` | Removes the last element and **returns** its value. |
| `vec_extend(v, arr, count)` | Appends `count` elements from the raw array `arr` to the end of the vector. |
| `vec_remove(v, index)` | Removes the element at `index`, shifting all subsequent elements left (preserves order). |
| `vec_swap_remove(v, index)` | Removes the element at `index` by swapping it with the last element (O(1), order not preserved). |
| `vec_reverse(v)` | Reverses the elements of the vector in-place. |

### Algorithms & Iteration

| Macro | Description |
| :--- | :--- |
| `vec_foreach(v, iter)` | A loop helper. `iter` must be a pointer variable; it is assigned to each element in the vector sequentially. |
| `vec_sort(v, cmp)` | Sorts the vector in-place using standard `qsort`. `cmp` is a function pointer: `int (*)(const T*, const T*)`. |
| `vec_bsearch(v, key, cmp)` | Performs a binary search. Returns a pointer to the found element or `NULL`. `key` is `const void*`. |

## Notes

### Why do I need to provide a "Short Name"?

In `REGISTER_TYPES(X)`, you must provide two arguments: the **Actual Type** and a **Short Name**.

```c
//      Actual Type   Short Name
X(unsigned long,        ulong)
```

The reason is that C macros cannot handle spaces when generating names. The library tries to create functions by gluing `vec_` + `Name`.

If you used `unsigned long` as the name, the macro would try to generate `vec_unsigned long`, which is a syntax error (variable names cannot contain spaces). But, by passing `ulong`, it correctly generates `vec_ulong`.