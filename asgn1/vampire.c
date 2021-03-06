#include "names.h"

#include <stdio.h>
#include <stdlib.h>

// Returns the position of the player to the left .
// pos: The position of the current player .
// players : The number of players in the game .
//
u_int32_t left(u_int32_t pos, u_int32_t players) {
    return (pos + players - 1) % players;
}

// Returns the position of the player to the right .
// pos: The position of the current player .
// players : The number of players in the game .
u_int32_t right(u_int32_t pos, u_int32_t players) {
    return (pos + 1) % players;
}

int main(void) {
    int vampires = 0;
    printf("Number of players:\n");
    do {
        scanf("%d", &vampires);
        if (!(vampires >= 2 && vampires <= 10)) {
            fprintf(stderr, "Invalid number of players.\n");
        }
    } while (!(vampires >= 2 && vampires <= 10));

    int lives[vampires];
    for (int i = 0; i < vampires; i++) {
        lives[i] = 3;
    }
    int seed = 0;
    printf("Random seed:\n");
    do {
        scanf("%d", &seed);
        if (seed < 1) {
            fprintf(stderr, "Invalid random seed.\n");
        }
    } while (seed < 1);
    int dead = 0;
    int round = 0;
    while (1) {
        round++;
        printf("Round :%d\n", round);
        srandom(seed);
        int scores[vampires];
        for (int i = 0; i < vampires; i++) {
            scores[i] = 13;
        }
        for (int i = 0; i < vampires; i++) {
            if (lives[i] > 0) {
                // Sets the random seed.
                int first = random() % 6; // First roll .
                int second = random() % 6; // Second roll .
                scores[i] = first + second;
                printf("- %s roles %s\n", names[i], rolls[first][second]);
            }
        }
        // Find the minimum score
        int min = scores[0];
        int index = 0;
        for (int i = 1; i < vampires; i++) {
            if (scores[i] < min) {
                min = scores[i];
            }
        }
        // Find the index of player who has minimum score
        for (int i = 1; i < vampires; i++) {
            if (scores[i] == min) {
                index = i;
            }
        }

        if (lives[index] > 0) {
            lives[index] = lives[index] - 1;
            printf("%s is forced to eat garlic!\n", names[index]);
            if (lives[index] > 0) {
                printf("%s has %d lives remaining.\n", names[index], lives[index]);
            } else {
                printf("%s has died.\n", names[index]);
                dead++;
            }
            if (scores[index] == 12) {

                int l = left(index, vampires);
                int r = right(index, vampires);
                if (lives[l] < 3) {
                    lives[l] = lives[l] + 1;
                }
                if (lives[r] < 3) {
                    lives[r] = lives[r] + 1;
                }
            }
        }

        if (dead == (vampires - 1))
            break;
    }
    for (int i = 0; i < vampires; i++) {
        if (lives[i] > 0) {
            printf("%s has winner.\n", names[i]);
            break;
        }
    }
    return 0;
}
