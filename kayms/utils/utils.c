#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int readFile(char *filename, char ***content, int *lines)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0;
    }

    // get file size
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    // make space
    char *temp = malloc((size + 1) * sizeof(char));
    // read
    int i = 0;
    int tempLines = 0;
    char ch;
    while (!feof(file))
    {
        ch = fgetc(file);
        if ((int)ch > 0 && (int)ch < 128)
        {
            if (ch == '\n')
            {
                tempLines++;
            }
            temp[i] = ch;
            i++;
        }
    }
    tempLines++;
    temp[i] = 0;

    *lines = tempLines;

    // makes points
    int lastIndex = 0;
    int lineIndex = 0;
    (*content) = malloc(tempLines * sizeof(void *));
    char *tchar = malloc((size + 1) * sizeof(char));
    for (int i = 0; i < strlen(temp); i++)
    {
        if (temp[i] == '\n')
        {
            tchar[i - lastIndex] = 0;                                           // set end char
            (*content)[lineIndex] = malloc((i - lastIndex + 1) * sizeof(char)); // space for string
            strcpy((*content)[lineIndex], tchar);                               // copy string to space
            lastIndex = i + 1;
            lineIndex++;
        }
        else
        {
            tchar[i - lastIndex] = temp[i];
        }
    }
    // for last
    tchar[i - lastIndex] = 0;
    (*content)[lineIndex] = malloc((i - lastIndex + 1) * sizeof(char));
    strcpy((*content)[lineIndex], tchar);

    fclose(file);
    return 1;
}