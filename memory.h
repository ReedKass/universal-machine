/* Comp 40 - HW6 - memory.h
 * Reed Kass-Mullet (rkassm01) and Jerry Wang (jwang35)
 * November 17, 2020
 * Purpose: Interface for the Memory Module. This module contains all
 *          the functions needed to initialize, modify, or retrieve 
 *          information from UM memory. 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stack.h>
#include <seq.h>
#include <assert.h>

typedef struct Memory {
    Seq_T segments;
    Stack_T unmapped;
    uint32_t pos_counter;
} *Memory;

/* initialize_memory()
 * Gets:  Nothing
 * Returns: Memory (struct Memory*)
 * Does: Initializes the Memory structure for UM
 */
Memory initialize_memory();

/* mem_map_segment
 * Gets:  Memory memory, a Memory struct containing all allocated segments
 *        Uint32_t size, a unsigned int containing the size of the new
 *        segment to be initialized.
 * Returns: uint32_t representing the index the segment was mapped to
 * Does: Initializes a new memory segment, sets all values to 0, checks if
 *       there is space to re-use a section by checking the unmapped stack. 
 *       If there is nothing on the unmapped stack, it adds a new segment to
 *       the end. If there are indexes on the stack, it pops the index and uses
 *       it as a guide to re-use a section/segment location for the new segment
 *       being mapped.
 */
uint32_t mem_map_segment(Memory memory, uint32_t size);

/* mem_unmap_segment
 * Gets:  Memory memory, a Memory struct containing all allocated segments
 *        uint32_t index, an unsigned int representing the index of the 
 *        segment to be unmapped
 * Returns: Nothing
 * Does: Unmaps the segment at given index in memory
 */
void mem_unmap_segment(Memory memory, uint32_t index);

/* load_segment
 * Gets:  Memory memory, a Memory struct containing all allocated segments
 *        Uint32_t b, represents the value of register[b]. in this function
 *                    it is the value of the segment index being loaded
 *        uint32_t c, represents the value of register[c]. In this function
 *                    it is the value of the index of the word in segment
 *                    being loaded
 * Returns: uint32_t representing the index being loaded
 * Does: Returns the uint32_t word in segment number b and index number c.
 */
uint32_t load_segment(Memory memory, uint32_t b, uint32_t c);

/* store_segment
 * Gets:  Memory memory, a Memory struct containing all allocated segments
 *        uint32_t a, represents the value of register[a]. In this function
 *                    it is the index value of the affected memory segment
 *        Uint32_t b, represents the value of register[b]. in this function
 *                    it is the word index of the affected memory segment
 *        uint32_t c, represents the value of register[c]. In this function
 *                    it is the value being stored in memory.
 * Returns: nothing
 * Does: Stores the uint32_t word c in segment number a at index number b.
 */
void store_segment(Memory memory, uint32_t a, uint32_t b, uint32_t c_value);

/* memory_free
 * Gets:  Memory memory, a Memory struct containing all allocated segments
 * Returns: nothing
 * Does: Frees all of the heap memory being stored in the memory struct.
 */
void memory_free(Memory memory);
