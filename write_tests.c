#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void build_halt_test(Seq_T instructions);
extern void build_verbose_halt_test(Seq_T instructions);
extern void build_output_test(Seq_T instructions);
extern void build_load_test(Seq_T instructions);
extern void build_addition_test(Seq_T instructions);
extern void build_multiplication_test(Seq_T instructions);
extern void build_division_test(Seq_T instructions);
extern void build_NAND_test(Seq_T instructions);
extern void build_cmov_test(Seq_T instructions);
extern void build_sload_test(Seq_T instructions);
extern void build_mapseg_test(Seq_T instructions);
extern void build_sstore_test(Seq_T instructions);
extern void build_unmap_test(Seq_T instructions);
extern void build_input_test(Seq_T instructions);
extern void build_lprogram_test(Seq_T instructions);

/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
        { "halt-verbose", NULL, "", build_verbose_halt_test },
        { "output", NULL, "6", build_output_test},
        { "load", NULL, "F", build_load_test},
        { "addition", NULL, "2", build_addition_test},
        { "multiplication", NULL, "Z", build_multiplication_test},
        { "division", NULL, "<", build_division_test},
        { "nand", NULL, "1", build_NAND_test},
        { "cmov", NULL, "339", build_cmov_test},
        { "mapseg", NULL, "8", build_mapseg_test},
        { "sload", NULL, "7", build_sload_test},
        { "segstore", NULL, "i", build_sstore_test},
        { "unmap", NULL, "U", build_unmap_test},
        { "input", "J", "J", build_input_test},
        { "lprogram", NULL, "K", build_lprogram_test},
};

  
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
