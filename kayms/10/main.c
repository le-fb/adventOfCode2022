#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/10/input"
#define ARR_LEN 1000

int main()
{
    char **content;
    int lines = 0;

    int fileLen = readFile(INPUT_FILE_NAME, &content, &lines);

    int x = 1;
    int historyIndex = 0;
    int *history = malloc(ARR_LEN * sizeof(int));
    char *screen = malloc(245 * sizeof(char));

    history[historyIndex] = x;
    historyIndex++;

    for (int i = 0; i < lines; i++)
    {
        char *split = strtok(content[i], " ");

        if (split[0] == 'n')
        {
            history[historyIndex] = x;
            historyIndex++;
        }
        else if (split[0] == 'a')
        {
            int a = atoi(strtok(NULL, " "));

            history[historyIndex] = x;
            historyIndex++;

            x += a;
            history[historyIndex] = x;
            historyIndex++;
        }
        else
        {
            printf("error %s\n", split);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < historyIndex; i++)
    {
        printf("%i ", history[i]);
    }
    printf("\n");

    int sum = 0;
    for (int i = 20; i < 221; i += 40)
    {
        // printf("%i\n", history[i - 1]);
        sum += history[i - 1] * (i);
    }
    printf("sum %i\n", sum);

    // make screen

    for (int i = 0; i < historyIndex; i++)
    {
        if (i % 40 == history[i] || (i + 1) % 40 == history[i] || (i - 1) % 40 == history[i])
        {
            screen[i] = '#';
        }
        else
        {
            screen[i] = '.';
        }
    }
    for (int i = 0; i < strlen(screen); i++)
    {

        if (i % 40 == 0)
        {
            printf("\n");
        }
        printf("%c", screen[i]);
    }

    return 0;
}
