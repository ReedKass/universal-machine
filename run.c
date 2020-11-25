/* Comp 40 - HW6 - run.c
 * Reed Kass-Mullet (rkassm01) and Jerry Wang (jwang35)
 * November 17, 2020
 * Purpose: Implementation of the Run Module.
 *          See interface of Run for purpose of module and functions.
 */


#include "run.h"


void run(FILE* input_file, char* filename)
{
    /* Initialize memory and registers */
    uint32_t registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    Memory memory = initialize_memory();
    load(memory, input_file, filename);

    /* Retrieves the next command from memory and executes it */
    while (1) {
        uint32_t word = load_segment(memory, 0, memory->pos_counter);
        command_center(registers, word, memory);
        memory->pos_counter++;
    }
}

void load(Memory memory, FILE* input_file, char* filename)
{
    assert(memory != NULL);
    int a, b, c, d;
    uint32_t* word;

    /* Check for faulty file */
    int size = get_file_size(filename);
    if (size % 4 != 0 || size == 0) {
        fprintf(stderr, "Improper File\n");
        exit(EXIT_FAILURE);
    } 

    /* Initialize first segment with words by bitpacking bytes 
     *  from input file */
    Seq_T initial_seg = Seq_new(10);
    while ((a = getc(input_file)) != EOF) {
        b = getc(input_file);
        c = getc(input_file);
        d = getc(input_file);
        word = bitpack_word(a, b, c, d);
        Seq_addhi(initial_seg, (void *) word);
    }

    Seq_addhi(memory->segments, (void *) initial_seg);
    fclose(input_file); 
}

uint32_t *bitpack_word(int a, int b, int c, int d)
{
    /* Create and initialize a word on the heap */
    uint32_t *new_word = malloc(sizeof(uint32_t*));
    uint32_t word = 0;

    /* Bitpack the values into the new word */
    word = Bitpack_newu(word, 8, 24, a);
    word = Bitpack_newu(word, 8, 16, b);
    word = Bitpack_newu(word, 8, 8, c);
    word = Bitpack_newu(word, 8, 0, d);
    *new_word = word;

    return new_word; 
}

int get_file_size(char* filename)
{
     struct stat fileInfo;
    stat(filename, &fileInfo);
    int size = fileInfo.st_size;
    return size;
}