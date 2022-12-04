#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INPUT_FILE_NAME "../../1/input"
#define MAX_COUNT_ELF 1000
#define LINE_BUFFER_SIZE 100

int main()
{
    int topCals[3] = {0};

    int i = 0; // elf index
    int elfCurrentCal = 0;
    int firstLineBreak = 0;

    int elfCals[MAX_COUNT_ELF] = {0};
    int lineNumber = 0;

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
            if (isdigit(ch))
            {
                lineNumber = (lineNumber * 10) + atoi(&ch);
                firstLineBreak = 0;
            }
            else
            {
                printf("char is not a number");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (firstLineBreak)
            {
                // store elf -> next elf
                elfCals[i] = elfCurrentCal;
                if (elfCurrentCal > topCals[0])
                {
                    topCals[2] = topCals[1];
                    topCals[1] = topCals[0];
                    topCals[0] = elfCurrentCal;
                }

                elfCurrentCal = 0;
                firstLineBreak = 0;
                lineNumber = 0;
                i++;
            }
            else
            {
                // add number to elf current Cal
                elfCurrentCal += lineNumber;
                lineNumber = 0;
                firstLineBreak = 1;
            }
        }
        ch = getc(file);
    }
    fclose(file);

    printf("TOP Elf Cal. : %i %i %i \n", topCals[0], topCals[1], topCals[2]);
    printf("Sum : %i\n", (topCals[0] + topCals[1] + topCals[2]));
}