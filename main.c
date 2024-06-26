/*
    Written by Luis Mendez
    Date Written: Jun 13, 2023
    Purpose: 
        Exam 2: Battleship
        5 missiles will be randomly placed by the computer in the board.
        Player will fire missiles until all ships have been sunk
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

// Executes game and corresponding functions
void runGame(POSITION[], int, int);

// Prints current status of game
void displayGameTable(POSITION[], int);

// Creates table and places ships randomly
void createTable(POSITION[], int*);

// Places an individual ship in a random position in the board
void placeShip(POSITION gametable[100], SHIP* ship);

// Helper for "placeship()", it verifies ship can be placed and stores ship positions.
int checkShipValid(POSITION gametable[100], int row, int column, int up, int length, int shipPositions[]);

// Prompts for coordinates and gives feedback on attempt
void attack(POSITION[], int*, char[], int*, int*);

// Returns translated coordinates for 1D array
int get1DCoordinate(char coordinates[]);

// Releases ship references from memory
void freeShips(POSITION[]);

// Returns true if coordinates are valid
void getCoordinates(char coordinates[], POSITION gametable[], int* shipsStanding, int attempts);

// Exits game and clears ships
void exitGame(POSITION[], int, int);

// Saves game on file
int saveGame(POSITION gametable[], int shipsStanding, int attempts);

// Retrieves game from file
void retrieveGame(POSITION gametable[], int* shipsStanding, int* attempts);

// Output Coloringgcc
void red();
void green();
void blue();
void yellow();
void purple();
void reset();

void pause();

int main(){
    POSITION gametable[100];
    int attempts = 0;
    int shipsStanding = 0;


    runGame(gametable, attempts, shipsStanding);


    return 0;
}

int saveGame(POSITION gametable[100], int shipsStanding, int attempts){
    FILE* file = fopen("gamefile.bin", "wb");
    if(file == NULL){
        printf("Could not save game.");
        return 0;
    }
    printf("Saving game...\n");

    fwrite(&attempts, sizeof(int), 1, file);
    fwrite(&shipsStanding, sizeof(int), 1, file);

    for(int i=0; i<100; i++){
        fwrite(&gametable[i], sizeof(POSITION), 1, file);
        if(gametable[i].ship != NULL) fwrite(gametable[i].ship, sizeof(SHIP), 1, file);
    }

    fclose(file);

    printf("Game saved.\n");

    return 1;
}

void retrieveGame(POSITION gametable[], int* attempts, int* shipsStanding){
    printf("Loading file...\n");
    FILE* file  = fopen("gamefile.bin", "rb");
    if (file == NULL) {
        printf("Could not open file.\n");
        exit(0);
    }
    fread(attempts, sizeof(int), 1, file);
    fread(shipsStanding, sizeof(int), 1, file);

    SHIP *ape = calloc(1, sizeof(SHIP));
    SHIP *bear = calloc(1, sizeof(SHIP));
    SHIP *cat = calloc(1, sizeof(SHIP));
    SHIP *dog = calloc(1, sizeof(SHIP));
    SHIP *elephant = calloc(1, sizeof(SHIP));

    for(int i=0; i<100; i++){
        fread(&gametable[i], sizeof(POSITION), 1, file);

        if(gametable[i].ship != NULL){
            gametable[i].ship = calloc(1, sizeof(SHIP));
            SHIP shipHolder;
            fread(&shipHolder, sizeof(SHIP), 1, file);

            switch (shipHolder.def)
            {
                case 'A':
                    memcpy(ape, &shipHolder, sizeof(SHIP));
                    gametable[i].ship = ape;
                    break;
                case 'B':
                    memcpy(bear, &shipHolder, sizeof(SHIP));
                    gametable[i].ship = bear;

                    break;
                case 'C':
                    memcpy(cat, &shipHolder, sizeof(SHIP));
                    gametable[i].ship = cat;
                    break;
                case 'D':
                    memcpy(dog, &shipHolder, sizeof(SHIP));
                    gametable[i].ship = dog;
                    break;
                case 'E':
                    memcpy(elephant, &shipHolder, sizeof(SHIP));
                    gametable[i].ship = elephant;
                    break;
            }
        }
    }
    fclose(file);
}   

void exitGame(POSITION gametable[], int shipsStanding, int attempts){
    if (shipsStanding) saveGame(gametable, shipsStanding, attempts);
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

void createTable(POSITION gametable[100], int* shipsStanding){
    printf("Creating new game...\n");
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
        (*shipsStanding)++;
    }else{
        printf("Could not create ship.");
    }
    if (bear != NULL){
        bear->def = 'B';
        bear->sunk = false;
        bear->hits = 0;
        bear->length = 3;
        placeShip(gametable, bear);
        (*shipsStanding)++;
    }else{
        printf("Could not create ship.");
    }
    if (cat != NULL){
        cat->def = 'C';
        cat->sunk = false;
        cat->hits = 0;
        cat->length = 3;
        placeShip(gametable, cat);
        (*shipsStanding)++;
    }else{
        printf("Could not create ship.");
    }
    if (dog != NULL){
        dog->def = 'D';
        dog->sunk = false;
        dog->hits = 0;
        dog->length = 4;
        placeShip(gametable, dog);
        (*shipsStanding)++;
    }else{
        printf("Could not create ship.");
    }
    if (elephant != NULL){
        elephant->def = 'E';
        elephant->sunk = false;
        elephant->hits = 0;
        elephant->length = 5;
        placeShip(gametable, elephant);
        (*shipsStanding)++;

    }else{
        printf("Could not create ship.");
    }
}

int checkShipValid(POSITION gametable[100], int row, int column, int up, int length, int shipPositions[]){
    // Recursive function towards ship direction to check if it's valid
    // Get 2 random numbers and multiply them to get the leading coordinate
    // printf("Checking position [%i,%i] with length %i\n", row, column, length);
    int randomPos = ((ROWS * row) + column);

    // Ship can be placed entirely
    if(length <= 0) {
        // printf("works!\n");
        return true;
    }
    // Either ship detected or out of bounds of table
    if(row < 0 || column >= COLUMNS || gametable[randomPos].ship){
        // printf("Does not work!\n");
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


        // printf("Checking if valid... ship %i length\n", ship->length);
        int shipPositions[ship->length];
        int valid = checkShipValid(gametable, randomRow, randomColumn, up, ship->length, shipPositions);
        if (valid) {
            // printf("ship placement valid from [%i,%i] going %s\n", randomRow, randomColumn, up?"up":"right");qq
            for(int i = 0; i < ship->length; i++) gametable[shipPositions[i]].ship = ship;
            placed = true;
        }
        // else printf("Sorry, not valid.\n");
    }while(!placed);
}

void getCoordinates(char coordinates[], POSITION gametable[], int* shipsStanding, int attempts){
    char str[10];
    printf("Please enter coordinates (e.g., A5): "); scanf(" %s", str);

    if (!strcmp(str, "QQ") || !strcmp(str, "qq")) exitGame(gametable, *shipsStanding, attempts);

    str[0] = toupper(str[0]);

    char row = str[0];
    char column = str[1];


    while(strlen(str) != 2 || row > 'J' || row < 'A' || column < '0' ){
        printf("Coordinates are not valid, please try again: "); scanf(" %s", str);

        if (!strcmp(str, "QQ") || !strcmp(str, "qq")) exitGame(gametable, *shipsStanding, attempts);

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

void displayGameTable(POSITION gametable[100], int attempts){
    // Floating ships
    SHIP* ships[6];
    int eSizeShips = 0;
    for(int i=0;i<100;i++){
        if(gametable[i].ship){
            int found = false;
            for(int j=0; j<eSizeShips;j++){
                if (ships[j] == gametable[i].ship){
                    found = true;
                    break;
                }
            }
            if(!found)ships[eSizeShips++] = gametable[i].ship;
        }
    }
    printf("\n");
    for(int i=0; i<eSizeShips; i++){
        printf("%c: %s\n", ships[i]->def, ships[i]->sunk? "Sunk" : "Floating" );
    }

    // Missiles Fired
    printf("\nMissiles Fired: %i\n", attempts);

    // Table
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

void attack(POSITION gametable[], int* attempts, char coordinates[], int* shipsStanding, int* winning){
    // Getting coordinates
    int ePos = get1DCoordinate(coordinates); 
    while(gametable[ePos].hit){
        printf("Position %s already hit, please select a different one.\n", coordinates);
        getCoordinates(coordinates, gametable, shipsStanding, *attempts);
        ePos = get1DCoordinate(coordinates); 
    }

    // Updating attempt number
    (*attempts)++;

    // Determines effectiveness of shot
    POSITION* currentPos = &gametable[ePos];
    currentPos->hit = true;
    if (currentPos->ship){
        currentPos->ship->hits = currentPos->ship->hits + 1;
        printf("\nSHIP HIT!\n");
        if(currentPos->ship->hits == currentPos->ship->length){
            printf("SHIP SUNK!\n");
            currentPos->ship->sunk = true;
            (*shipsStanding)--;
            printf("Ships standing: %i\n", *shipsStanding);
            if((*shipsStanding) == 0) {
                *winning = true;
                return;
            }
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

void runGame(POSITION gametable[], int attempts, int shipsStanding){
    FILE* file  = fopen("gamefile.bin", "rb");
    printf("WELCOME TO BATTLESHIP\n");

    if (file != NULL){
        char option;
        printf("Looks like you have a saved game, would you like to load it? (Y/N): "); scanf(" %c",&option);
        option = toupper(option);
        while (option != 'Y' && option != 'N'){
            printf("Sorry, please try again: "); scanf(" %c", &option);
        }
        if (option == 'Y') retrieveGame(gametable, &attempts, &shipsStanding);
        else createTable(gametable, &shipsStanding);
    }else 
        createTable(gametable, &shipsStanding);

    int winning = false;
    while (!winning){
        displayGameTable(gametable, attempts);  
        char coordinates[3];
        getCoordinates(coordinates, gametable, &shipsStanding, attempts);

        attack(gametable, &attempts, coordinates, &shipsStanding, &winning);
    };
    
    // Winner
    char option;
    printf("\nGood job! You have won.\n");
    printf("Would you like to start another game? (Y/N): "); scanf(" %c", &option);
    option = toupper(option);
    while (option != 'Y' && option != 'N'){
        printf("Sorry, please try again: "); scanf(" %c", &option);
    }
    if (option == 'Y') {
        remove("gamefile.bin");
        runGame(gametable, 0, 0);
    }
    else {
        remove("gamefile.bin");
        printf("Thank you for playing!");
    }

    exitGame(gametable, shipsStanding, attempts);
    fclose(file);
}

// Coloring
void purple(){printf("\033[1;35m");};
void blue(){printf("\033[1;34m");}
void green(){ printf("\033[1;32m"); }
void red () { printf("\033[1;31m"); }
void yellow (){ printf("\033[1;33m"); }
void reset () { printf("\033[0m"); }