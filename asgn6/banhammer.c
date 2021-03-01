#define _GNU_SOURCE

#include "bf.h"
#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "node.h"
#include "parser.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

// filenames
#define BAD_SPEAK_FILE "badspeak.txt"
#define NEW_SPEAK_FILE "newspeak.txt"

// regular expression for a word
#define WORD "[a-zA-Z0-9\\_\\-\\']+"

static void print_thoughtcrime_message() {
    printf("Dear Comrade,\n\n"
           "You have chosen to use degenerate words that may cause hurt\n"
           "feelings or cause your comrades to think unpleasant thoughts.\n"
           "This is doubleplus bad. To correct your wrongthink and\n"
           "preserve community consensus we will be sending you to joycamp\n"
           "administered by Medellin's Miniluv. Beware of the hippos.\n");
}

static void print_rightspeak_message() {
    printf("Dear Comrade,\n\n"
           "Submitting your text helps to preserve feelings and prevent\n"
           "badthink. Some of the words that you used are not goodspeak.\n"
           "The list shows how to turn the oldspeak words into newspeak.\n");
}

// Read "badspeak" file
void read_bad_speak(HashTable *ht, BloomFilter *bf) {
    char oldspeak[64]; // buffer for a word
    FILE *f = fopen(BAD_SPEAK_FILE, "r");
    if (!f) {
        fprintf(stderr, " Failed to open %s.\n", BAD_SPEAK_FILE);
        return;
    }
    // scan file and put each word to ht and bf
    while (fscanf(f, "%s", oldspeak) == 1) {
        ht_insert(ht, oldspeak, NULL);
        bf_insert(bf, oldspeak);
    }
    fclose(f);
}

// Read "newspeak" file
void read_new_speak(HashTable *ht, BloomFilter *bf) {
    char oldspeak[64], newspeak[64]; // buffers for two words
    FILE *f = fopen(NEW_SPEAK_FILE, "r");
    if (!f) {
        fprintf(stderr, " Failed to open %s.\n", NEW_SPEAK_FILE);
        return;
    }
    // scan file and put each word to ht and bf
    while (fscanf(f, "%s %s", oldspeak, newspeak) == 2) {
        ht_insert(ht, oldspeak, newspeak);
        bf_insert(bf, oldspeak);
    }
    fclose(f);
}

// convert a word to lower case.
void to_lower(char *word) {
    while (*word) {
        if (*word >= 'A' && *word <= 'Z') {
            *word += 32;
        }
        ++word;
    }
}

int main(int argc, char *argv[]) {

    // default values
    uint32_t h_size = 10000;
    uint32_t f_size = (1 << 20);
    bool mtf = false;

    // parse command line
    int opt;
    while ((opt = getopt(argc, argv, "h:f:m")) != -1) {
        switch (opt) {
        case 'h':
            h_size = atoi(optarg);
            // should be positive
            if (h_size <= 0) {
                fprintf(stderr, "Invalid hash table size parameter\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'f':
            // should be positive
            f_size = atoi(optarg);
            if (f_size <= 0) {
                fprintf(stderr, "Invalid filter size parameter\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'm': mtf = true; break;
        default: fprintf(stderr, "Unknown option: %c\n", opt); break;
        }
    }

    // Initialize Bloom filter and hash table
    BloomFilter *bf = bf_create(f_size);
    if (!bf) {
        fprintf(stderr, " Failed to create Bloom Filter.\n");
        return 1;
    }
    HashTable *ht = ht_create(h_size, mtf);
    if (!ht) {
        fprintf(stderr, " Failed to create Hash Table.\n");
        return 1;
    }

    // Read in a list of badspeak words
    read_bad_speak(ht, bf);

    // Read in a list of oldspeak and newspeak pairs
    read_new_speak(ht, bf);

    // Compile regular expression
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, " Failed to compile regex .\n");
        return 1;
    }

    // Create two list for "badspeak" and "oldspeak" words
    LinkedList *bad_speak_list = ll_create(false);
    LinkedList *old_speak_list = ll_create(false);

    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        // printf("Word: %s\n", word);

        to_lower(word); // convert to lower case

        // If the word has most likely been added to the Bloom filter
        if (bf_probe(bf, word)) {
            // 1. If the hash table contains the word and
            // the word does not have a newspeak translation
            if (ht_lookup(ht, word) && !ht_lookup(ht, word)->newspeak) {
                // thoughtcrime
                ll_insert(bad_speak_list, word, NULL);
                continue;
            }

            // 2. If the hash table contains the word and
            // the word does have a newspeak translation
            if (ht_lookup(ht, word) && ht_lookup(ht, word)->newspeak) {
                // requires counseling on proper Rightspeak.
                ll_insert(old_speak_list, word, ht_lookup(ht, word)->newspeak);
                continue;
            }

            // If the hash table does not contain the word
            // then all is good since the Bloom filter issued a false positive.
        }
    }

    // If the citizen is accused of thoughtcrime and requires counseling on proper
    // Rightspeak,
    if (ll_length(bad_speak_list) > 0 && ll_length(old_speak_list) > 0) {
        print_thoughtcrime_message();
        printf("\nYour errors:\n\n");
        ll_print(bad_speak_list);
        printf("\nThink on these words during your vacation!\n\n");
        ll_print(old_speak_list);
    }

    // If the citizen is accused solely of thoughtcrime
    else if (ll_length(bad_speak_list) > 0) {
        print_thoughtcrime_message();
        printf("\nYour errors:\n\n");
        ll_print(bad_speak_list);
    }

    // If the citizen only requires counseling
    else if (ll_length(old_speak_list) > 0) {
        print_rightspeak_message();
        printf("\n");
        ll_print(old_speak_list);
    }

    // clean-up
    bf_delete(&bf);
    ht_delete(&ht);
    ll_delete(&bad_speak_list);
    ll_delete(&old_speak_list);
    clear_words();
    regfree(&re);

    return 0;
}
