
#include <stdio.h>
#include "my_vectors.h"

int compare_ints(const void *a, const void *b) 
{
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return (int_a > int_b) - (int_a < int_b);
}

int compare_points_x(const void *a, const void *b)
{
    const Point *p_a = (const Point*)a;
    const Point *p_b = (const Point*)b;
    if (p_a->x < p_b->x) return -1;
    if (p_a->x > p_b->x) return 1;
    return 0;
}

int main(void) 
{
    vec_int nums = vec_init(int);
    vec_push(&nums, 42);
    vec_push(&nums, 7);
    vec_push(&nums, 19);
    vec_push(&nums, 1);

    printf("Before sort: ");
    int *n;
    vec_foreach(&nums, n) printf("%d ", *n);
    printf("\n");

    vec_sort(&nums, compare_ints);

    printf("After sort:  ");
    vec_foreach(&nums, n) printf("%d ", *n);
    printf("\n\n");

    vec_Point points = vec_init(Point);
    vec_push(&points, ((Point){10.0f, 2.0f}));
    vec_push(&points, ((Point){3.5f, 1.0f}));
    vec_push(&points, ((Point){7.0f, 5.0f}));

    printf("Points before sort (by X):\n");
    Point *p;
    vec_foreach(&points, p) printf("{%.1f, %.1f} ", p->x, p->y);
    printf("\n");

    vec_sort(&points, compare_points_x);

    printf("Points after sort (by X):\n");
    vec_foreach(&points, p) printf("{%.1f, %.1f} ", p->x, p->y);
    printf("\n");

    vec_free(&nums);
    vec_free(&points);
    return 0;
}
