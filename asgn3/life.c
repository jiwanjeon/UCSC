
#define _GNU_SOURCE

#include "universe.h"

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h> // For usleep ().

#define DELAY 50000

#define GENERATIONS 100

int main(int argc, char *argv[]) {

    int generations = GENERATIONS;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    bool toroidal = false;
    bool silence = false;
    int rows, cols;
    int opt;

    // parse command line
    while ((opt = getopt(argc, argv, "tsn:i:o:")) != -1)
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': silence = true; break;
        case 'n':
            generations = atoi(optarg);
            if (generations <= 0) {
                generations = GENERATIONS;
            }
            break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        default: fprintf(stderr, "Unknown option: %c\n", opt); break;
        }

    if (!infile) {
        fprintf(stderr, "Can't read input file.\n");
        return -1;
    }

    if (fscanf(infile, "%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Invlaid input file format.\n");
        return -1;
    }

    Universe *uA = uv_create(rows, cols, toroidal);
    Universe *uB = uv_create(rows, cols, toroidal);

    if (!uv_populate(uA, infile)) {
        fprintf(stderr, "Invlaid input file format.\n");
        return -1;
    }
    fclose(infile);

    if (!silence) {
        initscr(); // Initialize the screen .
        curs_set(FALSE); // Hide the cursor .
    }

    // For each generation up to the set number of generations:
    for (int gen = 0; gen < generations; gen++) {
        // If ncurses isn’t silenced by the -s option
        if (!silence) {
            clear(); // Clear the window.
            // display universe A
            for (int r = 0; r < uv_rows(uA); ++r) {
                for (int c = 0; c < uv_cols(uA); ++c) {
                    if (uv_get_cell(uA, r, c)) {
                        mvprintw(r, c, "o");
                    } /*else {
            mvprintw(r, c, "."); // uncomment to print dead cells
          }*/
                }
            }
            refresh(); // Refresh the window.
            usleep(DELAY); // Sleep for 50000 microseconds .
        }

        // Perform one generation.
        for (int r = 0; r < uv_rows(uA); ++r) {
            for (int c = 0; c < uv_cols(uA); ++c) {
                int census = uv_census(uA, r, c);

                if (uv_get_cell(uA, r, c) && (census == 2 || census == 3)) {
                    // first rule
                    uv_live_cell(uB, r, c);
                } else if (!uv_get_cell(uA, r, c) && (census == 3)) {
                    // second rule
                    uv_live_cell(uB, r, c);
                } else {
                    // third rule
                    uv_dead_cell(uB, r, c);
                }
            }
        }

        // swap
        Universe *uT = uA;
        uA = uB;
        uB = uT;
    }

    if (!silence) {
        endwin(); // Close the screen
    }

    // Output universe A to the speciﬁed file using uv_print()
    uv_print(uA, outfile);
    fclose(outfile);

    // Cleanup
    uv_delete(uA);
    uv_delete(uB);

    return 0;
}
