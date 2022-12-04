#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INPUT_FILE_NAME "../../2/input"
#define MAX_COUNT_ELF 1000
#define LINE_BUFFER_SIZE 100

int main()
{

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
            int test = 0;
        }
        else
        {
        }
        ch = getc(file);
    }
    fclose(file);
}