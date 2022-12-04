#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define INPUT_FILE_NAME "../../kayms/2/input"
#define MAX_COUNT_ELF 1000
#define LINE_BUFFER_SIZE 100

#define ROCK 1
#define PAPER 2
#define SCISSOR 3

int main()
{

    int elfMove = -1;
    int myMove = -1;
    int outcome = -1;
    int myPoints = 0;

    FILE *file = fopen(INPUT_FILE_NAME, "r");
    if (file == NULL)
    {
        printf("cant open file \n");
        exit(EXIT_FAILURE);
    }

    char ch = getc(file);
    while (ch != EOF)
    {
        if (ch != '\n')
        {
            if (ch != ' ')
            {
                // read move
                if (elfMove < 0)
                {
                    // reading elf move
                    elfMove = (int)ch - 64; // ascii A -> 65
                }
                else
                {
                    // reading my move
                    outcome = (int)ch - 88; // 0 -> lose , 1 -> draw, 2 -> win
                    if (outcome == 2)
                    {
                        // win
                        myMove = elfMove + 1;
                        if (myMove == 4)
                        {
                            myMove = 1;
                        }
                    }
                    else if (outcome == 1)
                    {
                        // draw
                        myMove = elfMove;
                    }
                    else
                    {
                        // lose
                        myMove = elfMove - 1;
                        if (myMove == 0)
                        {
                            myMove = 3;
                        }
                    }
                }
            }
        }
        else
        {
            assert(myMove > 0 && myMove < 4);
            assert(elfMove > 0 && elfMove < 4);

            // analysis move
            if (myMove == elfMove)
            {
                myPoints += myMove + 3;
            }
            else if ((myMove == PAPER && elfMove == ROCK) || (myMove == SCISSOR && elfMove == PAPER) || (myMove == ROCK && elfMove == SCISSOR))
            {
                // i win
                myPoints += myMove + 6;
            }
            else
            {
                // i lose
                myPoints += myMove;
            }
            elfMove = -1;
            myMove = -1;
        }
        ch = getc(file);
    }
    fclose(file);

    printf("points %i\n", myPoints);
}