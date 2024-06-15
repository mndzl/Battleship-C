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

/*
    Coordinates translation
    A - 1
    B - 2
    C - 3
    D - 4
    E - 5
    F - 6
    G - 7
    H - 8
    I - 9
    J - 10
*/

/*
    * A - Ape
    * B - Bear
    * C - Cat
    * D - Dog
    * E - Elephant
    * N - None
*/

typedef struct{
    char def;
    int sunk; // true or false
    int length; // full length
    int hits; // hits received
} SHIP;

typedef struct{
    SHIP* ship; // pointer to ship or null if not ship
    int hit; // true or false
} POSITION;


// Prints current status of game
void displayGameTable(POSITION[]);

// Creates table and places ships randomly
void createTable(POSITION[]);

void placeShip(POSITION[], SHIP* ship);

// Prompts for coordinates and gives feedback on attempt
void attack(POSITION[], int*, int ePos);

// Gets coordinates from user and returns translated coordinates for 1D array
int getCoordinates(POSITION[]);

void pause();

/*
    overnight notes

    need to place random ships on table
    input verification on coordination
    binary file for saving game

*/

int main(){
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

void createTable(POSITION gametable[100]){
    // Create table
    for(int i=0; i<100; i++){
        gametable[i].hit = false;
        gametable[i].ship = NULL;
    }

    // Place ships
    SHIP ape = {'A', false, 3, 0};
}

void placeShip(POSITION gametable[], SHIP* ship){
    gametable[5].ship = ship;
    gametable[6].ship = ship;
    gametable[7].ship = ship;
}


int getCoordinates(POSITION gametable[]){
    int valid = true;
    char coordinates[2];
    int ePos;
    do{
        printf("Please enter coordinates: "); scanf(" %s", coordinates);

        // Parsing
        int row = coordinates[0] - 65;
        int column = coordinates[1]-'0';
        // Calculating actual position in 1D array
        ePos = (ROWS * row) + column;

        if (gametable[ePos].hit){
            printf("Position already hit, please select a different one.\n");
            valid = false;
        } else{
            valid = true;
        }
    } while(!valid);

    return ePos;
}

void displayGameTable(POSITION gametable[100]){
    printf("\n    0 1 2 3 4 5 6 7 8 9\n");
    printf("   --------------------\n");
    for(int i=0; i<10; i++){
        printf("%c | ", i+65 );
        for(int j=0; j<10; j++){
            int ePos = (ROWS * i) + j;
            POSITION currentPos = gametable[ePos];
            if(currentPos.hit){
                if (currentPos.ship) {
                    if (currentPos.ship->sunk) printf("%c ", currentPos.ship->def);
                    else printf("H ");
                }
                else printf("M ");
            }
            else printf("O ");
        }
        printf("\n");
    }
}

void attack(POSITION gametable[], int* attempts, int ePos){
    // Update attempt number
    (*attempts)++;

    // Determines effectiveness of shot
    POSITION* currentPos = &gametable[ePos];
    currentPos->hit = true;
    if (currentPos->ship){
        printf("\nSHIP %c HIT!\n", currentPos->ship->def);
        currentPos->ship->hits++;
        if(currentPos->ship->hits == currentPos->ship->length){
            printf("SHIP SUNK!\n");
            currentPos->ship->sunk = true;
            printf("Tries: %i", *attempts);
        }
    }else{
        printf("\nMISSED!\n");
    }
    pause();
}

void pause() {
	printf("\nPress ENTER to continue....\n");
	getchar();
	getchar();
}