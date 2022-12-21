#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#include <utils.h>
#include <sys/types.h>
#include <linkedList.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/18/input"

#define MAX_DIMENSION_HEIGHT 50
#define MAX_CUBES MAX_DIMENSION_HEIGHT *MAX_DIMENSION_HEIGHT *MAX_DIMENSION_HEIGHT

typedef struct cube cube;
typedef enum dir dir;

enum dir
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    BACK,
    FRONT
};

struct cube
{
    cube **neighbor;
    int isAir;
    int pos[3]; // x,y,z
    int exposed;
};

cube *allCubes;

void updateExposedCubes(cube *c)
{
    assert(c->isAir);
    // printf("%i %i %i\n", c->pos[0], c->pos[1], c->pos[2]);
    c->exposed = 1;
    for (int i = 0; i < 6; i++)
    {
        if (c->neighbor[i] != NULL)
        {
            if (c->neighbor[i]->isAir && !c->neighbor[i]->exposed)
                updateExposedCubes(c->neighbor[i]);
        }
    }
}

int main()
{
    allCubes = malloc(MAX_CUBES * sizeof(cube));
    int cubeIndex = 0;

    char **lines;
    int lCount = 0;

    int r = readFile(INPUT_FILE_NAME, &lines, &lCount);
    assert(r > 0);

    int **tempStones = malloc(lCount * sizeof(void *));
    int tStoneIndex = 0;

    for (int i = 0; i < lCount; i++)
    {
        tempStones[tStoneIndex] = malloc(3 * sizeof(int));

        tempStones[tStoneIndex][0] = atoi(strtok(lines[i], ","));
        tempStones[tStoneIndex][1] = atoi(strtok(NULL, ","));
        tempStones[tStoneIndex][2] = atoi(strtok(NULL, ","));

        // printf("%i %i %i\n", tempStones[tStoneIndex][0], tempStones[tStoneIndex][1], tempStones[tStoneIndex][2]);
        tStoneIndex++;
    }

    // make area
    for (int z = -1; z < MAX_DIMENSION_HEIGHT - 1; z++)
    {
        for (int y = -1; y < MAX_DIMENSION_HEIGHT - 1; y++)
        {
            for (int x = -1; x < MAX_DIMENSION_HEIGHT - 1; x++)
            {
                int found = 0;
                for (int i = 0; i < tStoneIndex; i++)
                {
                    if (tempStones[i][0] == x && tempStones[i][1] == y && tempStones[i][2] == z)
                    {
                        found = 1;
                        break;
                    }
                }

                allCubes[cubeIndex].pos[0] = x;
                allCubes[cubeIndex].pos[1] = y;
                allCubes[cubeIndex].pos[2] = z;

                if (found)
                {
                    allCubes[cubeIndex].isAir = 0;
                }
                else
                {
                    allCubes[cubeIndex].isAir = 1;
                }
                allCubes[cubeIndex].exposed = 0;

                // neighbor

                allCubes[cubeIndex].neighbor = malloc(6 * sizeof(void *));

                // top down
                allCubes[cubeIndex].neighbor[UP] = NULL;
                if (z < 0)
                {
                    allCubes[cubeIndex].neighbor[DOWN] = NULL;
                }
                else
                {
                    allCubes[cubeIndex].neighbor[DOWN] = &allCubes[cubeIndex - (MAX_DIMENSION_HEIGHT * MAX_DIMENSION_HEIGHT)];
                    allCubes[cubeIndex - (MAX_DIMENSION_HEIGHT * MAX_DIMENSION_HEIGHT)].neighbor[UP] = &allCubes[cubeIndex];
                }

                // front back

                allCubes[cubeIndex].neighbor[FRONT] = NULL;
                if (y < 0)
                {
                    allCubes[cubeIndex].neighbor[BACK] = NULL;
                }
                else
                {
                    allCubes[cubeIndex].neighbor[BACK] = &allCubes[cubeIndex - MAX_DIMENSION_HEIGHT];
                    allCubes[cubeIndex - MAX_DIMENSION_HEIGHT].neighbor[FRONT] = &allCubes[cubeIndex];
                }

                // left right
                allCubes[cubeIndex].neighbor[RIGHT] = NULL;
                if (x < 0)
                {
                    allCubes[cubeIndex].neighbor[LEFT] = NULL;
                }
                else
                {
                    allCubes[cubeIndex].neighbor[LEFT] = &allCubes[cubeIndex - 1];
                    allCubes[cubeIndex - 1].neighbor[RIGHT] = &allCubes[cubeIndex];
                }
                cubeIndex++;
            }
        }
    }

    assert(allCubes[0].pos[0] == -1 && allCubes[0].pos[1] == -1 && allCubes[0].pos[2] == -1);
    updateExposedCubes(&allCubes[0]);

    // look thought space
    int sum = 0;
    int sumOutside = 0;
    for (int i = 0; i < cubeIndex; i++)
    {
        if (!allCubes[i].isAir)
        {
            for (int j = 0; j < 6; j++)
            {
                if (allCubes[i].neighbor[j]->isAir)
                {
                    sum++;
                }

                if (allCubes[i].neighbor[j]->isAir && allCubes[i].neighbor[j]->exposed)
                {
                    sumOutside++;
                }
            }
        }
    }

    printf("sides facing air %i\n", sum);
    printf("sides facing outside %i\n", sumOutside);

    return 0;
}