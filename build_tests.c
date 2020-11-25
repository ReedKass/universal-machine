/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction bitpacked = 0;
        bitpacked = Bitpack_newu(bitpacked, 3, 0, rc);
        bitpacked = Bitpack_newu(bitpacked, 3, 3, rb);
        bitpacked = Bitpack_newu(bitpacked, 3, 6, ra);
        bitpacked = Bitpack_newu(bitpacked, 4, 28, op);

        return bitpacked;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        Um_instruction bitpacked = 0;
        bitpacked = Bitpack_newu(bitpacked, 4, 28, LV);
        bitpacked = Bitpack_newu(bitpacked, 3, 25, ra);
        bitpacked = Bitpack_newu(bitpacked, 25, 0, val);

        return bitpacked;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b, 
                                      Um_register c) 
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction divide(Um_register a, Um_register b, 
                                    Um_register c) 
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c) 
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction cmov(Um_register a, Um_register b, Um_register c) 
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction sload(Um_register a, Um_register b, Um_register c) 
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction mapseg(Um_register b, Um_register c) 
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction sstore(Um_register a, Um_register b, 
                                    Um_register c) 
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction unmap(Um_register c) 
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction input(Um_register c) 
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction output(Um_register c) 
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction lprogram(Um_register b, Um_register c) 
{
        return three_register(LOADP, 0, b, c);
}

/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}


/* Unit tests for the UM */

void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

void build_output_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));

        append(stream, output(r3));
        append(stream, halt());
}

void build_load_test(Seq_T stream)
{
        append(stream, loadval(r1, 70));
        append(stream, output(r1));
        append(stream, halt());
}

void build_addition_test(Seq_T stream)
{
        append(stream, loadval(r1, 45));
        append(stream, loadval(r2, 5));
        append(stream, add(r0, r1, r2));
        append(stream, output(r0));
        append(stream, halt());
}

void build_multiplication_test(Seq_T stream)
{
        append(stream, loadval(r1, 10));
        append(stream, loadval(r2, 9));
        append(stream, multiply(r0, r1, r2));
        append(stream, output(r0));
        append(stream, halt());
}

void build_division_test(Seq_T stream)
{
        append(stream, loadval(r1, 120));
        append(stream, loadval(r2, 2));
        append(stream, divide(r0, r1, r2));
        append(stream, output(r0));
        append(stream, halt());
}

void build_NAND_test(Seq_T stream)
{
        append(stream, loadval(r1, 49));
        append(stream, loadval(r2, 51));
        append(stream, nand(r0, r1, r2));
        append(stream, nand(r3, r1, r2));
        append(stream, nand(r4, r3, r0));
        append(stream, output(r4));
        append(stream, halt());
}

void build_cmov_test(Seq_T stream)
{
        append(stream, loadval(r1, 57));
        append(stream, loadval(r2, 0)); 
        append(stream, loadval(r3, 51));
        append(stream, output(r3));

        append(stream, cmov(r3, r1, r2));
        append(stream, output(r3));

        append(stream, cmov(r3, r1, r1));
        append(stream, output(r3));

        append(stream, halt());    
}

void build_mapseg_test(Seq_T stream)
{
    append(stream, loadval(r3, 55));
    append(stream, mapseg(r2, r3));
    append(stream, add(r5, r2, r3));
    append(stream, output(r5));
    append(stream, halt()); 
}

void build_sload_test(Seq_T stream)
{
    append(stream, loadval(r0, 4));
    append(stream, loadval(r3, 55));
    append(stream, loadval(r2, 0));
    append(stream, mapseg(r1, r0));

    append(stream, sload(r0, r1, r2));
    append(stream, add(r4, r0, r3));
    append(stream, output(r4));
    append(stream, halt()); 
}

void build_sstore_test(Seq_T stream)
{
    append(stream, loadval(r0, 4));
    append(stream, loadval(r3, 0));
    append(stream, loadval(r5, 105));
    append(stream, mapseg(r1, r0));
    append(stream, sstore(r1, r3, r5));
    append(stream, sload(r5, r1, r3));
    append(stream, output(r5));
    append(stream, halt()); 
}

void build_unmap_test(Seq_T stream)
{
    append(stream, loadval(r0, 4));
    append(stream, loadval(r5, 85));
    append(stream, loadval(r1, 0));
    append(stream, mapseg(r2, r0));
    append(stream, loadval(r7, 8));
    append(stream, sstore(r2, r1, r7));
    append(stream, unmap(r2));
    append(stream, mapseg(r2, r0));
    append(stream, sload(r3, r2, r1));
    append(stream, add(r4, r3, r5));
    append(stream, output(r4));
    append(stream, halt());
}

void build_input_test(Seq_T stream)
{
    append(stream, loadval(r0, 4));
    append(stream, input(r0));

    append(stream, output(r0));
    append(stream, halt());
}

void build_lprogram_test(Seq_T stream)
{
    append(stream, loadval(r0, 55));
    append(stream, loadval(r1, 2));
    append(stream, loadval(r2, 0));
    append(stream, loadval(r5, 16));
    append(stream, mapseg(r3, r0));
    append(stream, sload(r4, r2, r5));
    append(stream, sstore(r3, r2, r4));

    append(stream, loadval(r5, 14));
    append(stream, sload(r4, r2, r5));
    append(stream, sstore(r3, r3, r4));

    append(stream, loadval(r5, 15));
    append(stream, sload(r4, r2, r5));
    append(stream, sstore(r3, r1, r4));

    append(stream, lprogram(r3, r2));

    append(stream, output(r0));
    append(stream, halt());
    append(stream, loadval(r0, 75));
}