
#include <stdio.h>
#include <string.h>
#include "my_vectors.h"

int compare_priority_desc(const Task *a, const Task *b)
{
    return b->priority - a->priority;
}

int compare_for_search(const Task *key, const Task *elem) 
{
    return elem->priority - key->priority;
}

int main(void) 
{
    printf("-> Initializing Job Queue...\n");
    vec_Task queue = vec_from(Task, {
        {101, 50, "Regular Maintenance"},
        {102, 99, "CRITICAL SERVER PATCH"},
        {103, 10, "Log Rotation"}
    });

    printf("-> Receiving Heavy Task (Zero-Copy)...\n");
    
    Task *heavy = vec_push_slot(&queue);
    if (heavy) 
    {
        heavy->id = 104;
        heavy->priority = 85;
        strcpy(heavy->payload, "Heavy Simulation Data [1KB]...");
    }

    printf("-> Sorting by Priority (Descending)...\n");
    vec_sort(&queue, compare_priority_desc);
    
    Task threshold_key = { .priority = 50 };
    
    Task *cutoff = vec_lower_bound(&queue, &threshold_key, compare_for_search);
    
    long high_priority_count = cutoff - vec_data(&queue);

    printf("-> Found %ld High-Priority Tasks:\n", high_priority_count);

    for (int i = 0; i < high_priority_count; i++) 
    {
        Task *t = vec_at(&queue, i);
        printf("    [EXEC] ID:%d (Prio:%d) - %s\n", t->id, t->priority, t->payload);
    }

    printf("-> Skipped Low-Priority Tasks:\n");
    Task *it = cutoff;
    while (it < vec_last(&queue) + 1) 
    {
        printf("    [SKIP] ID:%d (Prio:%d)\n", it->id, it->priority);
        it++;
    }

    vec_free(&queue);
    return 0;
}