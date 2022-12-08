#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/8/input"

typedef enum direction direction;

enum direction
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

typedef struct tree tree;

struct tree
{
    tree **neighbour;

    int scenicScore;

    int height;
    int isVisible;
};

/**
 * neighbour [north, east, south, west]
 */

tree *plantTree(tree **neighbour, int height)
{
    tree *t = malloc(sizeof(tree));

    t->height = height;
    t->isVisible = 0;

    t->neighbour = malloc(4 * sizeof(void *));

    for (int i = 0; i < 4; i++)
    {
        t->neighbour[i] = neighbour[i];

        if (neighbour[i] != NULL)
        {
            neighbour[i]->neighbour[(i + 2) % 4] = t;
        }
    }

    return t;
}

void printForest(tree *rootTree)
{
    int index = 0;
    while (rootTree->neighbour[EAST] != NULL)
    {

        printf("%i ", rootTree->scenicScore);
        rootTree = rootTree->neighbour[EAST];
    }
    printf("%i ", rootTree->scenicScore);
    printf("\n");
    // back
    while (rootTree->neighbour[WEST] != NULL)
    {
        rootTree = rootTree->neighbour[WEST];
    }

    if (rootTree->neighbour[SOUTH] != NULL)
    {
        printForest(rootTree->neighbour[SOUTH]);
    }
}

int checkDir(tree *root, tree *t, direction dir)
{
    if (root != t)
    {
        if (root->height <= t->height)
        {
            return 0;
        }
    }
    if (t->neighbour[dir] == NULL)
    {
        return 1;
    }

    return checkDir(root, t->neighbour[dir], dir);
}

void checkViewDis(tree *root, tree *t, direction dir, int *vDis)
{
    if (t == NULL)
    {
        return;
    }

    (*vDis)++;

    if (root->height > t->height)
    {
        checkViewDis(root, t->neighbour[dir], dir, vDis);
    }
}

int searchForView(tree **forest, int size)
{
    int top = 0;
    for (int i = 0; i < size; i++)
    {
        int score = 1;
        for (int j = 0; j < 4; j++)
        {
            int t = 0;
            if (forest[i]->neighbour[j] != NULL)
            {
                checkViewDis(forest[i], forest[i]->neighbour[j], j, &t);
            }
            score *= t;
        }
        forest[i]->scenicScore = score;
        if (score > top)
        {
            top = score;
        }
    }
    return top;
}

void scoutForest(tree *rootTree, int *vCounter, direction moveDir)
{
    if (rootTree == NULL)
    {
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        int v = checkDir(rootTree, rootTree, i);
        if (v)
        {
            rootTree->isVisible = 1;
            (*vCounter)++;
            break;
        }
    }

    direction next = moveDir;
    if (rootTree->neighbour[EAST] == NULL && moveDir == EAST || rootTree->neighbour[WEST] == NULL && moveDir == WEST)
    {
        next = SOUTH;
    }
    else if (rootTree->neighbour[EAST] == NULL && moveDir == SOUTH)
    {
        next = WEST;
    }
    else if (rootTree->neighbour[WEST] == NULL && moveDir == SOUTH)
    {
        next = EAST;
    }

    scoutForest(rootTree->neighbour[next], vCounter, next);
}

int main()
{
    char **content;
    int lines = 0;

    int r = readFile(INPUT_FILE_NAME, &content, &lines);

    // make forest
    int forestIndex = 0;
    tree **forest = malloc((r + 1) * sizeof(void *));

    tree **tempNeighbour = malloc(4 * sizeof(void *));

    assert(lines > 0);

    int forestWidth = strlen(content[0]);

    for (int i = 0; i < lines; i++)
    {
        assert(strlen(content[i]) == forestWidth);
        for (int j = 0; j < forestWidth; j++)
        {
            if (i == 0)
            {
                // first row north
                tempNeighbour[0] = NULL;
            }
            else
            {
                // assign north
                tempNeighbour[0] = forest[forestIndex - forestWidth];
            }

            // east
            tempNeighbour[1] = NULL;

            // south
            tempNeighbour[2] = NULL;

            // west
            if (j == 0)
            {
                tempNeighbour[3] = NULL;
            }
            else
            {
                tempNeighbour[3] = forest[forestIndex - 1];
            }

            // plant tree
            forest[forestIndex] = plantTree(tempNeighbour, cToI(content[i][j]));
            forestIndex++;
        }
    }

    int visibleCount = 0;

    scoutForest(forest[0], &visibleCount, EAST);

    printf("solution 1: %i\n", visibleCount);

    printf("solution 2: %i\n", searchForView(forest, forestIndex));

    // printForest(forest[0]);

    return 0;
}