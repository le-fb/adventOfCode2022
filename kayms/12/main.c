#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/12/input"
#define MAX_GRID_SIZE 600

typedef struct heightObj heightObj;

struct heightObj
{
    int level;
    int start;
    int end;
    char identifier;
};

void buildTree(gridNode *currNode, gridNode *endPos, aStarObj *currAStarObj, int aStarArraySize, aStarObj *aStarObjArray, int *aStartArrayIndex)
{
    if (((heightObj *)currNode->obj)->end)
    {
        int blub = 0;
        return;
    }
    for (int i = 0; i < 4; i++)
    {

        if (currNode->neighbor[i] != NULL)
        {
            int heightDelta = ((heightObj *)currNode->neighbor[i]->obj)->level - ((heightObj *)currNode->obj)->level;
            if (heightDelta > 1)
            {
                continue;
            }

            // check if pos is already in tree
            int found = 0;
            for (int j = 0; j < aStarArraySize; j++)
            {
                if (aStarObjArray[j].pos == currNode->neighbor[i])
                {
                    found = 1;
                    // aStarObjArray[j].aStarNeighbour = pushBackNode(aStarObjArray[j].aStarNeighbour, createLinkedNode(currAStarObj));
                    currAStarObj->aStarNeighbour = pushBackNode(currAStarObj->aStarNeighbour, createLinkedNode(&aStarObjArray[j]));
                    break;
                }
            }
            if (!found)
            {
                ((heightObj *)currNode->neighbor[i]->obj)->identifier = '.';

                aStarObj *n = &aStarObjArray[*aStartArrayIndex];
                n->currPathCost = 0;
                n->currWeight = -1;
                n->distToGo = gridDist(currNode->neighbor[i], endPos);
                n->isStart = 0;
                n->isEnd = ((heightObj *)currNode->neighbor[i]->obj)->end;
                n->pathCost = 1;
                n->pos = currNode->neighbor[i];
                n->prev = currAStarObj;

                // add star obj to curr star obj
                currAStarObj->aStarNeighbour = pushBackNode(currAStarObj->aStarNeighbour, createLinkedNode(n));
                (*aStartArrayIndex)++;

                buildTree(currNode->neighbor[i], endPos, n, aStarArraySize, aStarObjArray, aStartArrayIndex);
            }
        }
    }
}

void printGrid(gridNode *gridArray, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%c", ((heightObj *)gridArray[i].obj)->identifier);
        if (gridArray[i].neighbor[1] == 0)
        {
            printf("\n");
        }
    }
}

void printGridNode(void *obj)
{
    printf("[%i,%i]", ((gridNode *)obj)->x, ((gridNode *)obj)->y);
}

gridNode *gridArray;

int main()
{
    char **lines;
    int lCount = 0;

    int r = readFile(INPUT_FILE_NAME, &lines, &lCount);

    gridArray = malloc(MAX_GRID_SIZE * MAX_GRID_SIZE * sizeof(gridNode));

    gridNode *startPos;
    linkedNode *possibleStartPos = NULL;
    gridNode *endPos;

    // inti grid
    // top left is 0,0 x,y
    int index = 0;
    assert(lCount > 0);
    int xSize = strlen(lines[0]);
    for (int y = 0; y < lCount; y++)
    {
        for (int x = 0; x < xSize; x++)
        {
            gridArray[index].obj = malloc(sizeof(heightObj));
            ((heightObj *)gridArray[index].obj)->level = (int)lines[y][x];
            ((heightObj *)gridArray[index].obj)->identifier = lines[y][x];
            ((heightObj *)gridArray[index].obj)->start = 0;
            ((heightObj *)gridArray[index].obj)->end = 0;

            if (lines[y][x] == 'a')
            {
                possibleStartPos = pushBackNode(possibleStartPos, createLinkedNode(&gridArray[index]));
            }

            if (lines[y][x] == 'S')
            {
                startPos = &gridArray[index];
                //((heightObj *)gridArray[index].obj)->start = 1; // for question 1
                //((heightObj *)gridArray[index].obj)->level = (int)'a';
                printf("Start pos found %i %i\n", x, y);
            }

            if (lines[y][x] == 'E')
            {
                endPos = &gridArray[index];
                ((heightObj *)gridArray[index].obj)->level = (int)'z';
                ((heightObj *)gridArray[index].obj)->end = 1;
                printf("End pos found %i %i\n", x, y);
            }

            gridArray[index].x = x;
            gridArray[index].y = y;
            gridArray[index].neighbor = malloc(4 * sizeof(void *));

            // north
            if (y == 0)
            {
                gridArray[index].neighbor[0] = NULL;
            }
            else
            {
                gridArray[index].neighbor[0] = &gridArray[index - xSize];
                gridArray[index - xSize].neighbor[2] = &gridArray[index];
            }
            // east
            gridArray[index].neighbor[1] = NULL;
            // south
            gridArray[index].neighbor[2] = NULL;
            // west
            if (x == 0)
            {
                gridArray[index].neighbor[3] = NULL;
            }
            else
            {
                gridArray[index].neighbor[3] = &gridArray[index - 1];
                gridArray[index - 1].neighbor[1] = &gridArray[index];
            }
            index++;
        }
    }

    // printNodes(possibleStartPos, printGridNode);

    int s = getNodeSize(possibleStartPos);
    // printf("%i\n", s);

    int *score = malloc(s * sizeof(int));
    aStarObj **buffer = malloc(s * sizeof(void *));
    int scoreIndex = 0;
    int i = 0;

    linkedNode *c = possibleStartPos;

    while (c != NULL)
    {
        // printf("%i \n", i);
        startPos = (gridNode *)c->obj;
        // printGrid(gridArray, index);
        // printf("\n");

        // build A* Tree
        aStarObj *aStarObjArray = malloc(index * sizeof(aStarObj));
        int aStartArraySize = index;
        int aStartArrayIndex = 0;

        gridNode *currNode = startPos;
        ((heightObj *)startPos->obj)->identifier = '#';

        aStarObjArray[0].aStarNeighbour = NULL;
        aStarObjArray[0].currPathCost = 0;
        aStarObjArray[0].currWeight = -1;
        aStarObjArray[0].distToGo = gridDist(currNode, endPos);
        aStarObjArray[0].isStart = 1;
        aStarObjArray[0].isEnd = 0;
        aStarObjArray[0].pathCost = 0;
        aStarObjArray[0].pos = currNode;
        aStarObjArray[0].prev = &aStarObjArray[0];
        aStartArrayIndex++;

        buildTree(currNode, endPos, &aStarObjArray[0], aStartArraySize, aStarObjArray, &aStartArrayIndex);

        // printGrid(gridArray, index);
        // printf("\n");

        aStarObj *e = aStarSearch(&aStarObjArray[0]);
        if (e != NULL)
        {
            score[scoreIndex] = e->currPathCost;
            buffer[scoreIndex] = e;
            scoreIndex++;
        }
        c = c->next;
        i++;
    }

    // search for shortest path
    int min = -1;
    int minIndex = 0;
    for (int i = 0; i < scoreIndex; i++)
    {
        if (score[i] < min || min < 0)
        {
            min = score[i];
            minIndex = i;
        }
    }

    aStarObj *curr = buffer[minIndex];

    while (curr->isStart == 0)
    {
        char c;
        if (curr->pos->x == curr->prev->pos->x && curr->pos->y >= curr->prev->pos->y)
        {
            c = 'v';
        }
        else if (curr->pos->x == curr->prev->pos->x && curr->pos->y <= curr->prev->pos->y)
        {
            c = '^';
        }
        else if (curr->pos->x >= curr->prev->pos->x && curr->pos->y == curr->prev->pos->y)
        {
            c = '>';
        }
        else
        {
            c = '<';
        }
        ((heightObj *)curr->pos->obj)->identifier = c;
        curr = curr->prev;
    }

    printf("%i\n", min);
    printGrid(gridArray, index);

    printf("\n");
    printf("steps %i\n", buffer[minIndex]->currPathCost);

    return 0;
}
