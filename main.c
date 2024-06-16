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
#include <time.h>
#include <ctype.h>

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

// Places an individual ship in a random position in the board
void placeShip(POSITION gametable[100], SHIP* ship);

// Helper for "placeship()", it verifies ship can be placed and stores ship positions.
int checkShipValid(POSITION gametable[100], int row, int column, int up, int length, int shipPositions[]);

// Prompts for coordinates and gives feedback on attempt
void attack(POSITION[], int*, char[]);

// Returns translated coordinates for 1D array
int get1DCoordinate(char coordinates[]);

// Releases ship references from memory
void freeShips(POSITION[]);

// Returns true if coordinates are valid
void getCoordinates(char coordinates[], POSITION gametable[]);

// Exits game and clears ships
void exitGame(POSITION[]);

void red();
void green();
void blue();
void yellow();
void purple();
void reset();

// Output coloring

void pause();

/*
    to-do

    input verification on coordinates
    tries number
    binary file for saving game


*/

int main(){
    POSITION gametable[100];
    int attempts = 0;

    createTable(gametable);
    displayGameTable(gametable);

    while (true){
        char coordinates[3];
        getCoordinates(coordinates, gametable);

        attack(gametable, &attempts, coordinates);
        displayGameTable(gametable);
    };

    printf("END!");
    
    freeShips(gametable);
    return 0;
}

void exitGame(POSITION gametable[]){
    printf("END!");
    freeShips(gametable);

    exit(1);
}

void freeShips(POSITION gametable[]){
    // Freeing ships from positions
    SHIP* ships[5];
    int eShips=0;
    for(int i=0; i<100; i++) 
        if(gametable[i].ship){
            // Check if viewed
            int viewed = false;
            for(int j=0; j<5; j++){
                if(gametable[i].ship == ships[j]) {
                    viewed = true;
                    break;
                }
            }
            if(!viewed) {
                ships[eShips++] = gametable[i].ship;
            }
        }

    for(int i=0; i<eShips; i++){
        // printf("Ship %c freed\n",ships[i]->def);
        free(ships[i]);
    }
}

void createTable(POSITION gametable[100]){
    srand((unsigned) time(NULL));

    // Create table
    for(int i=0; i<100; i++){
        gametable[i].hit = false;
        gametable[i].ship = NULL;
    }
    
    // Allocating ship dynamically to stay in memory through the game
    SHIP *ape = calloc(1, sizeof(SHIP));
    SHIP *bear = calloc(1, sizeof(SHIP));
    SHIP *cat = calloc(1, sizeof(SHIP));
    SHIP *dog = calloc(1, sizeof(SHIP));
    SHIP *elephant = calloc(1, sizeof(SHIP));

    if (ape != NULL){
        ape->def = 'A';
        ape->sunk = false;
        ape->hits = 0;
        ape->length = 2;
        placeShip(gametable, ape);
    }else{
        printf("Could not create ship.");
    }
    if (bear != NULL){
        bear->def = 'B';
        bear->sunk = false;
        bear->hits = 0;
        bear->length = 3;
        placeShip(gametable, bear);
    }else{
        printf("Could not create ship.");
    }
    if (cat != NULL){
        cat->def = 'C';
        cat->sunk = false;
        cat->hits = 0;
        cat->length = 3;
        placeShip(gametable, cat);
    }else{
        printf("Could not create ship.");
    }
    if (dog != NULL){
        dog->def = 'D';
        dog->sunk = false;
        dog->hits = 0;
        dog->length = 4;
        placeShip(gametable, dog);
    }else{
        printf("Could not create ship.");
    }
    if (elephant != NULL){
        elephant->def = 'E';
        elephant->sunk = false;
        elephant->hits = 0;
        elephant->length = 5;
        placeShip(gametable, elephant);

    }else{
        printf("Could not create ship.");
    }
}

int checkShipValid(POSITION gametable[100], int row, int column, int up, int length, int shipPositions[]){
    // Recursive function towards ship direction to check if it's valid
    // Get 2 random numbers and multiply them to get the leading coordinate
    printf("Checking position [%i,%i] with length %i\n", row, column, length);
    int randomPos = ((ROWS * row) + column);

    // Ship can be placed entirely
    if(length <= 0) {
        printf("works!\n");
        return true;
    }
    // Either ship detected or out of bounds of table
    if(row < 0 || column >= COLUMNS || gametable[randomPos].ship){
        printf("Does not work!\n");
        return false;
    } 
    // Add position of ship positions
    shipPositions[length-1] = randomPos;

    // Recurse to up or right
    if(up) return true && checkShipValid(gametable, row-1, column, up, length-1, shipPositions);
    
    return true && checkShipValid(gametable, row, column+1, up, length-1, shipPositions);
    
}

void placeShip(POSITION gametable[100], SHIP* ship){
    int placed = false;
    do{
        int LB = 0;
        int UB = 9;
        int randomRow = LB + rand() % (UB - LB + 1);
        int randomColumn = LB + rand() % (UB - LB + 1);

        // Calculate effective position
        // Get random number for direction: up or right: 1 is up, 0 is right
        int up = rand() % 2;


        printf("Checking if valid... ship %i length\n", ship->length);
        int shipPositions[ship->length];
        int valid = checkShipValid(gametable, randomRow, randomColumn, up, ship->length, shipPositions);
        if (valid) {
            printf("ship placement valid from [%i,%i] going %s\n", randomRow, randomColumn, up?"up":"right");
            for(int i = 0; i < ship->length; i++) gametable[shipPositions[i]].ship = ship;
            placed = true;
        }
        else printf("Sorry, not valid.\n");
    }while(!placed);
}

void getCoordinates(char coordinates[], POSITION gametable[]){
    char str[10];
    printf("Please enter coordinates (e.g., A5): "); scanf(" %s", str);

    if (!strcmp(str, "QQ") || !strcmp(str, "qq")) exitGame(gametable);

    str[0] = toupper(str[0]);

    char row = str[0];
    char column = str[1];


    while(strlen(str) != 2 || row > 'J' || row < 'A' || column < '0' ){
        printf("Coordinates are not valid, please try again: "); scanf(" %s", str);

        if (!strcmp(str, "QQ") || !strcmp(str, "qq")) exitGame(gametable);

        str[0] = toupper(str[0]);

        row = str[0];
        column = str[1];
    }

    strcpy(coordinates, str);
    printf("\nSELECTED: %s\n", coordinates);
}

int get1DCoordinate(char coordinates[]){
    int row = coordinates[0] - 65;
    int column = coordinates[1]-'0';

    // Return actual position in array
    return (ROWS * row) + column;
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
                    if (currentPos.ship->sunk) {
                        blue();
                        printf("%c ", currentPos.ship->def);
                        reset();
                    }
                    else {
                        green();
                        printf("H ");
                        reset();
                    }
                }
                else {
                    red();
                    printf("M ");
                    reset();
                }
            }
            else printf("O ");
        }
        printf("\n");
    }
}

void attack(POSITION gametable[], int* attempts, char coordinates[]){
    // Getting coordinates
    int ePos = get1DCoordinate(coordinates); 
    while(gametable[ePos].hit){
        printf("Position %s already hit, please select a different one.\n", coordinates);
        getCoordinates(coordinates, gametable);
        ePos = get1DCoordinate(coordinates); 
    }

    // Updating attempt number
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

// Coloring
void purple(){printf("\033[1;35m");};
void blue(){printf("\033[1;34m");}
void green(){ printf("\033[1;32m"); }
void red () { printf("\033[1;31m"); }
void yellow (){ printf("\033[1;33m"); }
void reset () { printf("\033[0m"); }