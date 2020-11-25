/* Comp 40 - HW6 - commands.h
 * Reed Kass-Mullet (rkassm01) and Jerry Wang (jwang35)
 * November 17, 2020
 * Purpose: Interface for the Memory Module. This module contains all
 *          the functions of the commands needed to be called by UM.
 *      	Also contains helper function to help execute a command. 
 */


#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "bitpack.h"


/* command_center
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t word, representing the command to be executed
 *        Memory memory, containing all segmented memory
 * Returns: nothing
 * Does: Calls the the command indicated by the word, updates registers and
 *       memory accordingly.
 */
void command_center(uint32_t *registers, uint32_t word, Memory memory);

/* unpack_word
 * Gets:  uint32_t word of command
 *		  uint32_t* opcode, pointer to uninitialized opcode
 *        uint32_t* a, pointer to uninitialized a
 *        Uint32_t* b, pointer to uninitialized b
 *        uint32_t* c, pointer to uninitialized c
 * Returns: nothing
 * Does: Unpacks the word into opcode, a, b, and c, setting the pointers to
 *       their respective values.
 */
void unpack_word(uint32_t word, uint32_t* opcode, uint32_t* a, uint32_t* b,
                 uint32_t* c);

/* conditional_move
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: If c is not equal to 0, the value of R[a] is set to the value of R[b]
 */
void conditonal_move(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/* segmented_load
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: sets the value of R[a] to be the word in segment b, index c
 */
void segmented_load(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c, 
                    Memory memory);

/* segmented_store
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: If c is not equal to 0, the value of R[a] is set to the value of R[b]
 */
void segmented_store(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c, 
                    Memory memory);

/* addition
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: Puts the value of b + c % 2^32 into register a 
 */
void addition(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/* multiplication
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: Puts the value of R[b] * R[c] % 2^32 into register a 
 */
void multiplication(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/* division
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: Puts the value of R[b] / R[c] into register a 
 */
void division(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/* bitwise_nand
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: Puts the value of !(R[b] & R[c]) into register a 
 */
void bitwise_nand(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c);

/* halt
 * Gets:  Memory memory, a structure containing all of the memory segments
 * Returns: nothing
 * Does: Frees all memory and ends the program
 */
void halt(Memory memory);

/* map_segment
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: Maps a new memory segment of size c, initializes all words to be 0,
 *       sets register b to be the index of the new memory.
 */
void map_segment(uint32_t *registers, uint32_t b, uint32_t c, Memory memory);

/* unmap_segment
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t c, represents register index c
 *        Memory memory, a structure containing all memory segments
 * Returns: nothing
 * Does: Unamaps the segment indexed by register c
 */
void unmap_segment(uint32_t *registers, uint32_t c, Memory memory);

/* output
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: Ouputs the word in register c. 
 */
void output(uint32_t *registers, uint32_t c);

/* intput
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t c, represents register index c
 * Returns: nothing
 * Does: Reads in an input and puts the word in register c. 
 */
void input(uint32_t *registers, uint32_t c);

/* load_program
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        Uint32_t b, represents register index b
 *        uint32_t c, represents register index c
 *        Memory memory, a structure containing all memory segments
 * Returns: nothing
 * Does: Loads the segment indexed by the value in register b into segment 0
 *       freeing the segment in segment 0 in the process. Original segment b
 *       is left where it is. The program counter is set to the value in 
 *       register c.
 */
void load_program(uint32_t *registers, uint32_t b, uint32_t c, Memory memory);

/* load_value
 * Gets:  uint32_t array registers, an array containing all 8 um registers
 *        uint32_t a, represents register index a
 *        uint32_t value, representing the value being loaded
 * Returns: nothing
 * Does: Loads value into register a 
 */
void load_value(uint32_t *registers, uint32_t a, uint32_t value);
