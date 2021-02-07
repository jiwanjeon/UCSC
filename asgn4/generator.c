#include "bm.h"
#include "hamming.h"

#include <stdio.h>
#include <unistd.h>

#define OPTIONS "i:o:"
/**
 * Main method of the generator
 */
int main(int argc, char **argv) {
    int opt = 0;
    // Create the generator
    BitMatrix *generator;
    generator = bm_create(8, 4);
    char *inputFileName = NULL;
    char *outputFileName = NULL;

    // Parse the command line option
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': inputFileName = optarg; break;
        case 'o': outputFileName = optarg; break;
        default: printf("%s\n", "Invalid option");
        }
    }
    if (NULL == inputFileName) {
        printf("%s\n", "Input file path is required");
    } else {
        printf("%s\n", inputFileName);
    }
    if (NULL == outputFileName) {
        printf("%s\n", "Output file path is required");
    } else {
        printf("%s\n", outputFileName);
    }
    return 0;
}
