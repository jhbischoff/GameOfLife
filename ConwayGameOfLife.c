/****************************************************************
|   Command line implementation of Conway's Game of Life in C   |
|                                                               |
|                   Jake Bischoff, v1.1, 2022                   |
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define STYLE 5                     //Which board style (0-5)
#define GENERATION_FREQUENCY 10     //How many generations per second



const char gameStyles[2][6] = {
                                {'X', '8', 'M', '@', '#', 'N'}, 
                                {'.', '-', '-', '`', '\'', ' '}
                              };
int BOARD_SIZE;

void resetCursor();
char getNextState(char board[BOARD_SIZE][BOARD_SIZE], int x, int y);
int isNumber(char number[]);
void randomGenerator(int boardSize);
void emptyGenerator(int size);
int error();
void help();

int main(int argc, char* argv[]) {
    //Display instructions if run with no arguments
    if(argc == 1){
        if(fork() == 0){
            char *command[] = {"cat", "readme.txt", NULL};
            execvp("cat", command);
        }
        return -1;
    }
    //Handle usage cases
    FILE* inputBoard;
    if(isNumber(argv[1])){
        randomGenerator(atoi(argv[1]));
        inputBoard = fopen("Board.txt", "r");
        BOARD_SIZE = atoi(argv[1]);
    } else if(strcmp("Help", argv[1]) == 0){
        help();
        return -1;
    } else if(strcmp("Generate", argv[1]) == 0){
        if(argc != 3 || !isNumber(argv[2])){
            return error();
        }
        emptyGenerator(atoi(argv[2]));
        printf("\nFile \"Board.txt\" of size %d generated successfully.\n", atoi(argv[2]));
        printf("Edit file using '1' to indicate live cell starting positions.\n\n");
        return 0;
    } else {
        inputBoard = fopen(argv[1], "r");
        if(argc != 3 || !isNumber(argv[2])){
            return error();
        }
        BOARD_SIZE = atoi(argv[2]);
    }
    int generation = 0;
    char currentGen[BOARD_SIZE][BOARD_SIZE]; 
    char nextGen[BOARD_SIZE][BOARD_SIZE]; 

    //Read in the initial state from file
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            currentGen[i][j] = fgetc(inputBoard);
            if(currentGen[i][j] == '\n'){
                currentGen[i][j] = fgetc(inputBoard);
            }
        }
    }
    fclose(inputBoard);

    while(1){
        //Print generation
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++){
                if(currentGen[i][j] == '1'){
                    printf("%c ", gameStyles[0][STYLE]);
                } else {
                    printf("%c ", gameStyles[1][STYLE]);
                }
            }
            printf("\n\r");
        }

        //Determine next generation
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++){
                nextGen[i][j] = getNextState(currentGen, i, j);
            }
        }
        //Update current generation for next iteration
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++){
                currentGen[i][j] = nextGen[i][j];
            }
        }
        
        printf("\n\nGeneration: %d", generation);
        resetCursor();
        fflush(stdout);
        usleep(1000000 / (double)GENERATION_FREQUENCY);
        generation++;
    }
    return 0;
}

void resetCursor(){
    //Return cursor to beginning of board using ANSI code "\033[F"
    for(int i = 0; i < BOARD_SIZE + 2; i++){
            printf("\033[F\r");
    }
}

char getNextState(char board[BOARD_SIZE][BOARD_SIZE], int x, int y){
    //Determine next state by checking current state of each neighbor
    //   . . .
    //   . X .
    //   . . .
    int liveNeighbors = 0;   

    if(x != 0 && y != 0 && board[x - 1][y - 1] == '1'){
        liveNeighbors++;
    }
    if(x != 0 && board[x - 1][y] == '1'){
        liveNeighbors++;
    }
    if(x != 0 && y != BOARD_SIZE - 1 && board[x - 1][y + 1] == '1'){
        liveNeighbors++;
    }
    if(y != 0 && board[x][y - 1] == '1'){
        liveNeighbors++;
    }
    if(y != BOARD_SIZE - 1 && board[x][y + 1] == '1'){
        liveNeighbors++;
    }
    if(x != BOARD_SIZE - 1 && y != 0 && board[x + 1][y - 1] == '1'){
        liveNeighbors++;
    }
    if(x != BOARD_SIZE - 1 && board[x + 1][y] == '1'){
        liveNeighbors++;
    }
    if(x != BOARD_SIZE - 1 && y != BOARD_SIZE - 1 && board[x + 1][y + 1] == '1'){
        liveNeighbors++;
    }

    //Define the state according to the rules of Conway's Game of Life:
    //1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
    //2. Any live cell with two or three live neighbours lives on to the next generation.
    //3. Any live cell with more than three live neighbours dies, as if by overpopulation.
    //4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    if(board[x][y] == '1' && (liveNeighbors == 2 || liveNeighbors == 3)){
        return '1';
    } else if (board[x][y] == '0' && liveNeighbors == 3){
        return '1';
    }
    return '0';
}

int isNumber(char number[]){
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-')
        i = 1;
    for (; number[i] != 0; i++)
    {
        if (number[i] > '9' || number[i] < '0')
            return 0;
    }
    return 1;
    //Borrowed from niyasc on stackoverflow
}

void randomGenerator(int boardSize){
    srand(time(NULL));
    FILE* board = fopen("Board.txt", "w");
    for(int i = 0; i < boardSize; i++){
        for(int j = 0 ; j < boardSize; j++){
            int num = rand();
            if(num % 5 == 0){
                fprintf(board, "%s", "1");
            } else {
                fprintf(board, "%s", "0");
            }
        }
        if(i < boardSize - 1){
            fprintf(board, "\n");
        }
    }
    fclose(board);
}

void emptyGenerator(int size){
    FILE* board = fopen("Board.txt", "w");
    for(int i = 0; i < size; i++){
        for(int j = 0 ; j < size; j++){
            fprintf(board, "%s", "0");
        }
        if(i < size - 1){
            fprintf(board, "\n");
        }
    }
    fclose(board);
}

int error(){
    printf("\nERROR: incorrect usage. See below for usage instructions.\n");
    help();
    printf("\nReturn to this page with [ .out Help ]\n\n");
    return -1;
}

void help(){
    printf("\n");
    printf("~~~~~~~~~CONWAY'S GAME OF LIFE~~~~~~~~~\n");
    printf("|  Implemented in C by Jake Bischoff  |\n");
    printf("~~~~~~~~~~~~~~~~~[v1.1]~~~~~~~~~~~~~~~~\n");
    printf("|USAGE INSTRUCTIONS:                  |\n");
    printf("|                                     |\n");
    printf("|Usage 1: [ .out File.txt -s ]        |\n");
    printf("|            s: size of input file    |\n");
    printf("|             *File must be a square  |\n");
    printf("|              board of '0' and '1'   |\n");
    printf("|              1 is alive, 0 is dead  |\n");
    printf("|Usage 2: [ .out -r ]                 |\n");
    printf("|            r: size of game board    |\n");
    printf("|             *Generates a random     |\n");
    printf("|              sequence of live cells |\n");
    printf("|              on a board of size r   |\n");
    printf("|                                     |\n");
    printf("|Usage 3: [ .out Generate -s ]        |\n");
    printf("|             *Generates a clear board|\n");
    printf("|              of size 's' written as |\n");
    printf("|              file \"Board.txt\"       |\n");
    printf("|             *Enter '1' in desired   |\n");
    printf("|              live cell starting     |\n");
    printf("|              positions              |\n");
    printf("|             *Use usage 1 to run     |\n");
    printf("|             *Usage 2 will overwrite |\n");
    printf("|              this file, save with   |\n");
    printf("|              different name if      |\n");
    printf("|              necessary              |\n");
    printf("|                                     |\n");
    printf("|Change style and generation frequency|\n");
    printf("|in definitions in the source code    |\n");
    printf("|                                     |\n");
    printf("|Exit with CTRL+C                     |\n");
    printf("|-------------------------------------|\n");
}