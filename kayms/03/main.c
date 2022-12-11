#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define INPUT_FILE_NAME "../../kayms/03/input"
#define LINE_BUFFER_SIZE 1000

int main()
{
    int bufferIndex = 0;
    char *buffer = malloc(LINE_BUFFER_SIZE * sizeof(char));
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
            buffer[bufferIndex] = ch;
            bufferIndex++;
        }
        else
        {
            int half = bufferIndex >> 1;
            int done = 0;
            assert((bufferIndex) % 2 == 0);
            for (int i = 0; i < half; i++)
            {
                for (int j = half; j < bufferIndex; j++)
                {
                    if (buffer[i] == buffer[j])
                    {
                        if (buffer[i] > 96)
                        {
                            sumOfPrio += (int)buffer[i] - 96;
                        }
                        else
                        {
                            sumOfPrio += (int)buffer[i] - 64 + 26;
                        }
                        done = 1;
                        break;
                    }
                }
                if (done)
                {
                    done = 0;
                    break;
                }
            }
            bufferIndex = 0;
        }
        ch = getc(file);
    }

    free(buffer);
    fclose(file);

    printf("sum of prio's %i\n", sumOfPrio);
}