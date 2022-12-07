#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/6/input"
#define MARKER_SIZE 13

int getStartOfPackage(char *input)
{
    int len = strlen(input) - MARKER_SIZE;
    for (int i = 0; i < len; i++)
    {
        int doubleFound = 0;
        int dict[128] = {0};
        for (int j = 0; j < MARKER_SIZE; j++)
        {
            // check for double entries
            if (dict[(int)input[j]])
                doubleFound = 1;
            // fill dict
            dict[(int)input[j]] = 1;
        }
        if (!doubleFound && !dict[(int)input[MARKER_SIZE]])
        {
            return (i + MARKER_SIZE + 1);
        }
        input++; // pointer "magic"
    }
}

int main()
{
    char **content;
    int lines = 0;

    readFile(INPUT_FILE_NAME, &content, &lines);

    for (int i = 0; i < lines; i++)
    {
        printf("%i\n", getStartOfPackage(content[i]));
    }
}