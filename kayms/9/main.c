#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/9/input"
#define GRID_SIZE 900

typedef struct point point;
typedef struct knot knot;
typedef enum dir dir;

enum dir
{
    UP = 0,
    UPRIGHT = 1,
    RIGHT = 2,
    DOWNRIGHT = 3,
    DOWN = 4,
    DOWNLEFT = 5,
    LEFT = 6,
    UPLEFT = 7,
    NONE = 8
};

struct knot
{
    point *currPos;
    point *lastPos;
    dir lastMove;
    char name;
};

struct point
{

    /**
     *neighbor indexes
     *
     * 7 0 1
     * 6 + 2
     * 5 4 3
     *
     */
    point **neighbor;
    int tailVisit;
};

int tailInRange(knot *h, knot *t)
{
    // check same pos
    if (h->currPos == t->currPos)
    {
        return 1;
    }
    // check all neighbor
    for (int i = 0; i < 8; i++)
    {
        if (h->currPos->neighbor[i] == t->currPos)
        {
            return 1;
        }
    }
    return 0;
}

void physics(knot *h, knot *t)
{
    dir move = NONE;
    for (int i = 0; i < 8; i += 2)
    {
        if (t->currPos->neighbor[i] == NULL || t->currPos->neighbor[i]->neighbor[i] == NULL)
        {
            continue;
        }
        if (h->currPos == t->currPos->neighbor[i]->neighbor[i])
        {
            move = i;
            break;
        }
    }
    if (move == NONE)
    {
        for (int i = 0; i < 8; i += 2)
        {
            for (int j = 1; j < 8; j += 2)
            {
                if (h->currPos->neighbor[(i + 4) % 8] == t->currPos->neighbor[j])
                {
                    move = j;
                    goto GoON;
                }
            }
        }
    }
GoON:
    if (move == NONE)
    {
        move = h->lastMove;
    }

    t->lastMove = move;
    t->lastPos = t->currPos;
    t->currPos = t->currPos->neighbor[move];
}

point *makeGrid()
{
    int size = GRID_SIZE * GRID_SIZE;
    point *grid = malloc((size) * sizeof(point));

    int index = 0;

    // startpoint bottom left -> should be index 0

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {

            /**
             *neighbor indexes
             *   +
             * 7 0 1
             * 6 + 2
             * 5 4 3
             *   +
             */
            // space for neighbor

            grid[index].neighbor = malloc(8 * sizeof(void *));

            // bottom
            if (i == 0)
            {
                grid[index].neighbor[3] = NULL;
                grid[index].neighbor[4] = NULL;
                grid[index].neighbor[5] = NULL;
            }
            else
            {
                grid[index].neighbor[3] = &grid[index - GRID_SIZE + 1];

                grid[index].neighbor[4] = &grid[index - GRID_SIZE];

                if ((index - GRID_SIZE - 1) < 0)
                {
                    grid[index].neighbor[5] = NULL;
                }
                else
                {
                    grid[index].neighbor[5] = &grid[index - GRID_SIZE - 1];
                }

                // init below top

                if ((index - 1) < 0)
                {
                    grid[index - GRID_SIZE].neighbor[7] = NULL;
                }
                else
                {
                    grid[index - GRID_SIZE].neighbor[7] = &grid[index - 1];
                }

                grid[index - GRID_SIZE].neighbor[0] = &grid[index];
                grid[index - GRID_SIZE].neighbor[1] = &grid[index + 1];
            }

            // right
            grid[index].neighbor[2] = NULL;

            // left
            if (j == 0)
            {
                grid[index].neighbor[6] = NULL;
            }
            else
            {
                grid[index].neighbor[6] = &grid[index - 1];
                grid[index - 1].neighbor[2] = &grid[index];
            }

            // top

            grid[index].neighbor[7] = NULL;
            grid[index].neighbor[0] = NULL;
            grid[index].neighbor[1] = NULL;

            // mark
            grid[index].tailVisit = 0;

            index++;
        }
    }

    return grid;
}

void printGrid(point *grid, knot **arr, int arrLen, int onlyTail)
{
    int index = GRID_SIZE * GRID_SIZE;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        index -= GRID_SIZE;
        for (int j = 0; j < GRID_SIZE; j++)
        {
            int found = -1;
            for (int k = 0; k < arrLen; k++)
            {
                if (&grid[index] == arr[k]->currPos && !onlyTail)
                {
                    found = k;
                    break;
                }
            }
            if (found >= 0)
            {
                printf("%c", arr[found]->name);
            }
            else if (grid[index].tailVisit)
            {
                printf("#");
            }
            else
            {
                printf(".");
            }
            index++;
        }
        printf("\n");
        index -= GRID_SIZE;
    }
}

int main()
{
    char **content;
    int lines = 0;

    int tailCount = 10; // set this to 2 for question 1;

    point *grid = makeGrid();

    int fileLen = readFile(INPUT_FILE_NAME, &content, &lines);

    // start pos mid
    int mid = ((GRID_SIZE >> 1) * GRID_SIZE) - (GRID_SIZE >> 1);
    // int mid = (100 * GRID_SIZE) - (GRID_SIZE - 100);

    knot **tailArr = malloc(tailCount * sizeof(void *));

    for (int i = 0; i < tailCount; i++)
    {
        tailArr[i] = malloc(sizeof(knot));
        if (i == 0)
        {
            tailArr[i]->name = 'H';
        }
        else
        {
            tailArr[i]->name = '0' + (i);
        }
        tailArr[i]->currPos = &grid[mid];
        tailArr[i]->lastPos = &grid[mid];
        tailArr[i]->lastMove = NONE;
    }

    int sumVisits = 0;

    for (int i = 0; i < lines; i++)
    {
        // parsing
        char d = content[i][0];

        if (d != 'U' && d != 'R' && d != 'L' && d != 'D')
        {
            continue;
        }

        assert(d == 'U' || d == 'R' || d == 'L' || d == 'D');
        int a = atoi(&content[i][2]);
        assert(a >= 0);

        // move

        /**
         *neighbor indexes
         *
         * 7 0 1
         * 6 + 2
         * 5 4 3
         *
         */
        dir currMove;
        switch (d)
        {
        case 'U':
            currMove = UP;
            break;
        case 'R':
            currMove = RIGHT;
            break;
        case 'D':
            currMove = DOWN;
            break;
        case 'L':
            currMove = LEFT;
            break;
        default:
            exit(EXIT_FAILURE);
            break;
        }
        for (int j = 0; j < a; j++)
        {
            if (tailArr[0]->currPos->neighbor[currMove] == NULL)
            {
                printf("grid not big enough \n");
                exit(EXIT_FAILURE);
            }
            assert(tailArr[0]->currPos->neighbor[currMove] != NULL);

            // make head move
            tailArr[0]->lastPos = tailArr[0]->currPos;
            tailArr[0]->currPos = tailArr[0]->currPos->neighbor[currMove];
            tailArr[0]->lastMove = currMove;

            for (int k = 0; k < tailCount - 1; k++)
            {
                if (!tailInRange(tailArr[k], tailArr[k + 1]))
                {
                    physics(tailArr[k], tailArr[k + 1]);
                }

                if (k == (tailCount - 2))
                {
                    if (!tailArr[k + 1]->currPos->tailVisit)
                    {
                        sumVisits++;
                        tailArr[k + 1]->currPos->tailVisit = 1;
                    }
                }
            }
            // printGrid(grid, tailArr, tailCount, 0);
            // printf("%i\n", i);
        }
    }

    printGrid(grid, tailArr, tailCount, 1);
    printf("tail visits: %i\n", sumVisits);
    return 0;
}
