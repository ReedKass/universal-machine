/* Comp 40 - HW6 - run.h
 * Reed Kass-Mullet (rkassm01) and Jerry Wang (jwang35)
 * November 17, 2020
 * Purpose: Interface of run module. This module contains all the functions 
 *          and helper functions needed to initialize and then run UM.
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "commands.h"


/* run
 * Gets:  A file pointer that points to the input file containing 
 *              the series of commands. 
 *         Char* of the name of the input file
 * Returns: nothing
 * Does: Runs UM with given commands in input file. 
 */
void run(FILE* input_file, char* filename);

/* load
 * Gets: Struct Memory that is our memory of the UM
 *        File pointer than points to the input file that already
 *            is open
 *        Char* of the name of the input file
 * Returns: nothing
 * Does: Initializes memory of the UM 
 */
void load(Memory memory, FILE* input_file, char* filename);

/* bitpack_word
 * Gets: 4 ints, that user wants to bitpack into a word
 * Returns: uint32_t word that is bitpacked with the 4 input bytes
 * Does: Bitpacks the 4 integers into a uint32_t word 
 */
uint32_t *bitpack_word(int a, int b, int c, int d);

/* get_file_size
 * Gets: char* of filename that you want the size of
 * Returns: int of file size
 * Does: Gets the size of a file, with name of input filename
 */
int get_file_size(char* filename);