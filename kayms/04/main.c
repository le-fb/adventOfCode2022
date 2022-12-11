#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/04/input"

void getElfSpace(char *input, int *spaces)
{
    int numberIndex = 0;
    int temp = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (isdigit(input[i]))
        {
            temp = (temp * 10) + ((int)input[i] - (int)'0');
        }
        else
        {
            spaces[numberIndex] = temp;
            temp = 0;
            numberIndex++;
        }
    }

    spaces[numberIndex] = temp;
    numberIndex++;

    assert(numberIndex == 4);
}

int fullOverlap(int space[])
{
    /**
     * case 1
     * |---+++++---|
     * |----++-----|
     *
     * case 2
     * |---++++----|
     * |-++++++++--|
     */
    if ((space[0] >= space[2] && space[1] <= space[3])     // first in sec
        || (space[0] <= space[2] && space[1] >= space[3])) // sec in first
    {
        return 1;
    }
    return 0;
}
int partialOverlap(int space[])
{
    /**
     * case 1
     * |---++++----|
     * |--------++-|
     *
     * case 2
     * |------+----|
     * |-++++------|
     */
    assert(space[0] <= space[1]);
    assert(space[2] <= space[3]);

    if (space[1] < space[2] || space[3] < space[0])
    {
        return 0;
    }
    return 1;
}

int main()
{
    char **content;
    int lines = 0;

    readFile(INPUT_FILE_NAME, &content, &lines);

    int *space = malloc(4 * sizeof(int));

    int sumFull = 0;
    int sumPartial = 0;

    for (int i = 0; i < lines; i++)
    {
        // printf("%s\n", content[i]);
        getElfSpace(content[i], space);
        sumFull += fullOverlap(space);
        sumPartial += partialOverlap(space);
    }
    printf("sum full: %i\n", sumFull);
    printf("sum partial: %i\n", sumPartial);
}