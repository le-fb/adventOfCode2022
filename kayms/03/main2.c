#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define INPUT_FILE_NAME "../../kayms/03/input"
#define LINE_BUFFER_SIZE 1000

int main()
{
    int lineIndex = 0;
    int bufferIndex = 0;
    int candidatesIndex = 0;
    char *buffer = malloc(LINE_BUFFER_SIZE * sizeof(char));
    char *candidates = malloc(LINE_BUFFER_SIZE * sizeof(char));
    int sumOfPrio = 0;

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
            if (lineIndex == 0)
            {
                // read first line
                candidates[candidatesIndex] = ch;
                candidatesIndex++;
            }
            else
            {
                // read 2 & 3
                buffer[bufferIndex] = ch;
                bufferIndex++;
            }
        }
        else
        {
            if (lineIndex != 0)
            {
                for (int i = 0; i < candidatesIndex; i++)
                {
                    int found = 0;
                    for (int j = 0; j < bufferIndex; j++)
                    {
                        if (candidates[i] == buffer[j])
                        {
                            found = 1;
                            break;
                        }
                    }
                    if (!found)
                    {
                        // remove candidate
                        candidates[i] = 0;
                    }
                }
                bufferIndex = 0;
                if (lineIndex > 1)
                {
                    for (int i = 0; i < candidatesIndex; i++)
                    {
                        if (candidates[i] != 0)
                        {
                            if (candidates[i] > 96)
                            {
                                sumOfPrio += (int)candidates[i] - 96;
                            }
                            else
                            {
                                sumOfPrio += (int)candidates[i] - 64 + 26;
                            }
                            break;
                        }
                    }
                    candidatesIndex = 0;
                }
            }
            lineIndex = (lineIndex + 1) % 3;
        }

        ch = getc(file);
    }
    free(candidates);
    free(buffer);
    fclose(file);

    printf("sum : %i\n", sumOfPrio);
}