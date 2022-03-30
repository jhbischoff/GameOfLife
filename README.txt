

                    Conway's Game of Life
                      By Jake Bischoff
                      Version 1.1, 2022

Compile in your terminal using whichever C compiler you use.
GCC compilation I used:

            gcc -Wall ConwayGameOfLife.c -std=c99

This program can:
    1. Execute an initial generation from an input file
       formatted as a square array of any size with '0'
       indicating dead cells and '1' indicating live cells.
       Be sure to include the size of the board, s, after 
       declaring the file when running the program.

                [ .out File.txt -s ]   

    2. Execute a randomly generated initial generation of
       size r. A file "Board.txt" is created and computed
       on the backend. 

                [ .out -r ]

    3. Generate an empty generation of size s so that the
       user can manipulate it to create a custom intial
       generation. The file "Board.txt" is created and 
       must be opened and set by the user. It can then be
       called with usage 1 in order to run. Remember to 
       include size declaration.
       NOTE: The output file named "Board.txt" will be 
             overwritten if the program runs a random
             call. Save with a different name if needed.

                [ .out Generate -s ]

    4. Output usage information if needed.

                [ .out Help ]
            
There are multiple board styles available that can be chosen
by changing the STYLE definition in the C code file. You can 
also add your own by manipulating the gameStyles array.

The generation frequency can be altered by changing the
GENERATION_FREQUENCY definition in the C code file. It is in 
units of generations per second.

Example files are included for some interesting starting
positions found in the Game of Life. These files are:

File name              | Size
-----------------------|-----------
GosperGliderGun.txt    | 50
Pulsar.txt             | 17


Example compilation & running from file (Linux):

            gcc -Wall ConwayGameOfLife.c -std=c99
            ./a.out Boards/GosperGliderGun.txt 50

                                     
Exit the program with CTRL+C
