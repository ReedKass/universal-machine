/* Comp 40 - HW6 - commands.c
 * Reed Kass-Mullet (rkassm01) and Jerry Wang (jwang35)
 * November 17, 2020
 * Purpose: Implementation of Commands module
 *          See interface of Commands module for purpose of module and 
 *          functions.
 */

#include "commands.h"

void command_center(uint32_t *registers, uint32_t word, Memory memory)
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t opcode;

    /* Unpack word into 4 variables */
    unpack_word(word, &opcode, &a, &b, &c);

    /* Call the function corresponding to opcode value */
    if (opcode == 0) {
        conditonal_move(registers, a, b, c);
    } else if (opcode == 1) {
        segmented_load(registers, a, b, c, memory);
    } else if (opcode == 2) {
        segmented_store(registers, a, b, c, memory);
    } else if (opcode == 3) {
        addition(registers, a, b, c);
    } else if (opcode == 4) {
        multiplication(registers, a, b, c);
    } else if (opcode == 5) {
        division(registers, a, b, c);
    } else if (opcode == 6) {
        bitwise_nand(registers, a, b, c);
    } else if (opcode == 7) {
        halt(memory);
    } else if (opcode == 8) {
        map_segment(registers, b, c, memory);
    } else if (opcode == 9) {
        unmap_segment(registers, c, memory);
    } else if (opcode == 10) {
        output(registers, c);
    } else if (opcode == 11) {
        input(registers, c);
    } else if (opcode == 12) {
        load_program(registers, b, c, memory);
    } else if (opcode == 13) {
        load_value(registers, a, b); 
    }
}

void unpack_word(uint32_t word, uint32_t* opcode, uint32_t* a, 
                 uint32_t* b, uint32_t* c)
{
    /* Unpack opcode from word */
    *opcode = (uint32_t) Bitpack_getu(word, 4, 28);

    /* Unpack a, b, and c, depending on opcode value */
    if (*opcode == 13) {
        *a = (uint32_t) Bitpack_getu(word, 3, 25);
        uint32_t val = (uint32_t) Bitpack_getu(word, 25, 0);
        *b = val;
        *c = 0; 
    } else {
        *a = (uint32_t) Bitpack_getu(word, 3, 6);
        *b = (uint32_t) Bitpack_getu(word, 3, 3);
        *c = (uint32_t) Bitpack_getu(word, 3, 0);
    }
}

void conditonal_move(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c)
{
    if (registers[c] != 0) {
        registers[a] = registers[b];
    }
}

void segmented_load(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c, 
                    Memory memory)
{
    registers[a] = load_segment(memory, registers[b], registers[c]);
}

void segmented_store(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c, 
                    Memory memory)
{
    store_segment(memory, registers[a], registers[b], registers[c]);
}

void addition(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c)
{
    registers[a] = registers[b] + registers[c];
    registers[a] = registers[a] % (4294967296);
}

void multiplication(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c)
{
    registers[a] = registers[b] * registers[c];
    registers[a] = registers[a] % (4294967296);
}

void division(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c)
{
    registers[a] = registers[b] / registers[c];
}

void bitwise_nand(uint32_t *registers, uint32_t a, uint32_t b, uint32_t c)
{
    registers[a] = ~(registers[b] & registers[c]);
}

void halt(Memory memory)
{
    assert(memory != NULL);
    memory_free(memory);
    exit(0);
}

void map_segment(uint32_t *registers, uint32_t b, uint32_t c, Memory memory)
{
    assert(memory != NULL);
    registers[b] = mem_map_segment(memory, registers[c]);
}

void unmap_segment(uint32_t *registers, uint32_t c, Memory memory)
{
    assert(memory != NULL);
    mem_unmap_segment(memory, registers[c]);
}

void output(uint32_t *registers, uint32_t c)
{
    assert(registers[c] < 256);
    putchar(registers[c]);
}

void input(uint32_t *registers, uint32_t c)
{
    uint32_t val = getc(stdin);
    if (val == (uint32_t) EOF) {
        val = ~((uint32_t) 0);
    }
    registers[c] = val;
}

void load_program(uint32_t *registers, uint32_t b, uint32_t c, Memory memory)
{
    assert(memory != NULL);
    /* Copies the segment and frees the segment that is being replaced */
    if (registers[b] != 0) {
        Seq_T to_copy = (Seq_T) Seq_get(memory->segments, registers[b]);
        Seq_T duplicate = Seq_new(Seq_length(to_copy));
        for (int i = 0; i < Seq_length(to_copy); i++) {
            uint32_t* word_copies = malloc(sizeof(uint32_t*));
            *word_copies = *(uint32_t*) Seq_get(to_copy, i);
            Seq_addhi(duplicate, word_copies);
        }
        Seq_T to_be_freed = (Seq_T) Seq_put(memory->segments, 0, duplicate);
        for (int j = 0; j < Seq_length(to_be_freed); j++) {
            free((uint32_t*) Seq_get(to_be_freed, j));
        }
        Seq_free(&to_be_freed);
    }

    memory->pos_counter = registers[c] - 1; 
}

void load_value(uint32_t *registers, uint32_t a, uint32_t value)
{
    registers[a] = value;
}