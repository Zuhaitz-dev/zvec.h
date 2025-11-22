# zvec.h

`zvec.h` provides dynamic arrays (vectors) for C projects. Unlike typical C vector implementations that rely on `void*` casting and lose type safety, `zvec.h` uses C11 `_Generic` selection and X-Macros to generate fully typed, type-safe implementations for your specific data structures.

## Features

* **Type Safety**: Compiler errors if you try to push a float into a vec_int.
* **Native Performance**: Data is stored in contiguous arrays of the actual type (no pointer indirection overhead).
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
// List every type you want a vector for here:
#define REGISTER_TYPES(X) \
    X(int)                \
    X(float)              \
    X(Point)

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
    // Initialize (allocated on stack, internal data on heap)
    vec_int nums = vec_init(int);

    // Push values
    vec_push(&nums, 10);
    vec_push(&nums, 20);

    // Iterate
    int *n;
    vec_foreach(&nums, n)
    {
        printf("%d ", *n);
    }

    // Cleanup
    vec_free(&nums);
    return 0;
}
```
