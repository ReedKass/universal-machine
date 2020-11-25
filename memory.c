/* Comp 40 - HW6 - memory.c
 * Reed Kass-Mullet (rkassm01) and Jerry Wang (jwang35)
 * November 17, 2020
 * Purpose: Implementation of the Memory Module.
 *          See interface of Memory for purpose of module and functions.
 */


#include "memory.h"


Memory initialize_memory()
{
    /* Create a Memory struct on heap and initialize everything in it */
    Memory new_memory = malloc(sizeof(struct Memory));
    new_memory->segments = Seq_new(10);
    new_memory->unmapped = Stack_new();
    new_memory->pos_counter = (uint32_t) 0;
    return new_memory;
}

uint32_t mem_map_segment(Memory memory, uint32_t size)
{
    assert(memory != NULL);
    /* Create a new segment on the heap */
    Seq_T new_segment = Seq_new(size);
    uint32_t index;
    uint32_t* temp; 

    /* Initialize the new segment with 0s created on the heap */
    for (uint32_t i = 0; i < size; i++) {
        uint32_t *to_store = malloc(sizeof(uint32_t*));
        *to_store = (uint32_t) 0;
        Seq_addhi(new_segment, (void *) to_store);
    }

    /* Check if segment identifier can be reused */
    if (!Stack_empty(memory->unmapped)) {
        temp = (uint32_t*) Stack_pop(memory->unmapped);
        index = *temp;
        Seq_T old_segment = Seq_put(memory->segments, index, 
                            (void *) new_segment);
        free(temp);
        for (int j = 0; j < Seq_length(old_segment); j++) {
            free((uint32_t*) Seq_get(old_segment, j));
        }
        Seq_free(&old_segment);
    } else {
        /* If segment identifier can't be reused, add new one to Memory */
        Seq_addhi(memory->segments, (void *) new_segment);
        index = Seq_length(memory->segments) - 1;
    }
    return index; 
}

void mem_unmap_segment(Memory memory, uint32_t index) 
{
    assert(memory != NULL);
    uint32_t *to_store = malloc(sizeof(uint32_t*));
    *to_store = index;
    Stack_push(memory->unmapped, (void *) to_store); 
}

uint32_t load_segment(Memory memory, uint32_t b, uint32_t c)
{
    assert(memory != NULL);
    Seq_T first_seg = (Seq_T) Seq_get(memory->segments, b);
    uint32_t word = *(uint32_t*) Seq_get(first_seg, c);
    return word;
}

void store_segment(Memory memory, uint32_t a, uint32_t b, uint32_t c_value)
{
    assert(memory != NULL);
    uint32_t *to_store = malloc(sizeof(uint32_t*));
    *to_store = c_value;
    free(Seq_put((Seq_T) Seq_get(memory->segments, a), b, 
        (void*) to_store));
}

void memory_free(Memory memory)
{
    assert(memory != NULL);
    /* Loop through all segments in Memory */
    for (int i = 0; i < Seq_length(memory->segments); i++) {
        /* Free all words in the segment */
        for (int j = 0; j < Seq_length((Seq_T) Seq_get(memory->segments, i));
             j++) {
            free((uint32_t*) Seq_get((Seq_T) 
                 Seq_get(memory->segments, i), j));
        }
        /* Free the segment itself */
        Seq_T to_be_freed = Seq_get(memory->segments, i);
        Seq_free(&to_be_freed);
    }
    /* Free the Seq_T of segments*/
    Seq_free(&(memory->segments));

    /* Free everything in the Stack_T holding unmapped identifiers */
    while (!Stack_empty(memory->unmapped)) {
        free((uint32_t*) Stack_pop(memory->unmapped));
    }
    /* Free the Stack*/
    Stack_free(&(memory->unmapped));

    /* Free the memory struct */
    free(memory);
}