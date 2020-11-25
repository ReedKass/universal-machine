/* Comp 40 - HW6 - um.c
 * Reed Kass-Mullet (rkassm01) and Jerry Wang (jwang35)
 * November 17, 2020
 * Purpose: The main function that runs UM. Combines all the 
 *          modules and calls run UM. 
 */

#include "run.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Improper number of arguments.\n");
        exit(EXIT_FAILURE);
    }

    FILE* file = fopen(argv[1], "r");
    assert(file != NULL);

    /* Run UM */
    run(file, argv[1]);

    return 0;
}