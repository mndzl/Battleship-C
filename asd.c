/*
    Written by Luis Mendez
    Date Written: Jun 13, 2023
    Purpose: 
        Exam 2: Battleship
        5 missiles will be randomly placed by the computer in the board.
        Player will fire missiles until all ships have been sunk
        Register winner/top player
        Allow user to play new game if won

    ---------- SHIPS ----------
    * Ape - 2
    * Bear - 3
    * Cat - 3
    * Dog - 4
    * Elephant - 5

    ------- ERROR PREVENTION -------
    * Missile coordinates is letter + number
    * Validate that letter is between A and J, and number is between 0 and 9.
    
    ------ OTHER REQUIREMENTS ------
    * Load/save game
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 10
#define COLUMNS 10
#define true 1
#define false 0

typedef struct {
    char def;
    int sunk; // true or false
    int length; // full length
    int hits; // hits received
} SHIP;

typedef struct {
    SHIP* ship; // pointer to ship or null if not ship
    int hit; // true or false
} POSITION;

// Function prototypes
void displayGameTable(POSITION gametable[]);
void createTable(POSITION gametable[]);
void attack(POSITION gametable[], int* attempts, int ePos);
int getCoordinates(POSITION gametable[]);
void pause();

int main() {
    POSITION gametable[100];
    int attempts = 0;
    createTable(gametable);
    displayGameTable(gametable);
    for(int i=0; i<4; i++){
        int coordinates = getCoordinates(gametable);
        attack(gametable, &attempts, coordinates);
        displayGameTable(gametable);
    }

    return 0;
}

void createTable(POSITION gametable[100]) {
    // Create table
    for (int i = 0; i < 100; i++) {
        gametable[i].hit = false;
        gametable[i].ship = NULL;
    }

    // Place ships
    SHIP ape = {'A', false, 3, 0};
    gametable[5].ship = &ape;
    gametable[6].ship = &ape;
    gametable[7].ship = &ape;
}

int getCoordinates(POSITION gametable[]) {
    int valid = true;
    char coordinates[3]; // Allow space for the null terminator
    int ePos;
    do {
        printf("Please enter coordinates: ");
        scanf(" %2s", coordinates);

        // Parsing
        int row = coordinates[0] - 'A';
        int column = coordinates[1] - '0';

        // Check if input is valid
        if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS) {
            printf("Invalid coordinates, please select again.\n");
            valid = false;
            continue;
        }

        // Calculating actual position in 1D array
        ePos = (ROWS * row) + column;

        if (gametable[ePos].hit) {
            printf("Position already hit, please select a different one.\n");
            valid = false;
        } else {
            valid = true;
        }
    } while (!valid);

    return ePos;
}

void displayGameTable(POSITION gametable[100]) {
    printf("\n    0 1 2 3 4 5 6 7 8 9\n");
    printf("   --------------------\n");
    for (int i = 0; i < 10; i++) {
        printf("%c | ", i + 65);
        for (int j = 0; j < 10; j++) {
            int ePos = (ROWS * i) + j;
            POSITION currentPos = gametable[ePos];
            if (currentPos.hit) {
                if (currentPos.ship) {
                    if (currentPos.ship->sunk) {
                        printf("%c ", currentPos.ship->def);
                    } else {
                        printf("H ");
                    }
                } else {
                    printf("M ");
                }
            } else {
                printf("O ");
            }
        }
        printf("\n");
    }
}

void attack(POSITION gametable[], int* attempts, int ePos) {
    // Update attempt number
    (*attempts)++;

    // Determine effectiveness of shot
    POSITION* currentPos = &gametable[ePos];
    currentPos->hit = true;
    if (currentPos->ship) {
        printf("\nSHIP %c HIT!\n", currentPos->ship->def);
        currentPos->ship->hits++;
        if (currentPos->ship->hits == currentPos->ship->length) {
            printf("SHIP SUNK!\n");
            currentPos->ship->sunk = true;
            printf("Tries: %i\n", *attempts);
        }
    } else {
        printf("\nMISSED!\n");
    }
    pause();
}

void pause() {
    printf("\nPress ENTER to continue....\n");
    getchar();
    getchar();
}
