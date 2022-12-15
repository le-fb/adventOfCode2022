#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <utils.h>
#include <linkedList.h>

#define INPUT_FILE_NAME "../../kayms/14/input"

typedef enum element element;
typedef struct cell cell;

#define getCell(obj) ((cell *)obj)

enum element
{
    SOURCE = 0,
    AIR,
    STONE,
    SAND,
    PATH
};

struct cell
{
    int isMoving;
    element e;
};

void printCell(void *obj)
{
    char c;
    switch (((cell *)obj)->e)
    {
    case SOURCE:
        c = '+';
        break;
    case AIR:
        c = '.';
        break;
    case STONE:
        c = '#';
        break;
    case SAND:
        c = 'o';
        break;
    case PATH:
        c = '~';
        break;
    default:
        c = 'E';
        break;
    }
    printf("%c", c);
}

void moveSand(gridNode **movingSandCell, gridDirection dir, gridNode *sandSource)
{
    if ((*movingSandCell) == sandSource)
    {
        getCell((*movingSandCell)->obj)->e = SOURCE;
    }
    else
    {
        getCell((*movingSandCell)->obj)->e = PATH;
    }
    getCell((*movingSandCell)->neighbor[dir]->obj)->e = SAND;
    (*movingSandCell) = (*movingSandCell)->neighbor[dir];
}

int main()
{
    char **lines;
    int lCount = 0;

    int r = readFile(INPUT_FILE_NAME, &lines, &lCount);
    int allSize = 100000;
    gridNode *allGirdElements = malloc(allSize * sizeof(gridNode));
    gridNode *stonesGrid = malloc(allSize * sizeof(gridNode));
    gridNode *stoneLineGrid = malloc(allSize * sizeof(gridNode));
    gridNode *sandSource = NULL;
    int currGridIndex = 0;
    int currStoneGridIndex = 0;
    int currStoneLineIndex = 0;
    printf("size %i\n", allSize);
    for (int i = 0; i < allSize; i++)
    {
        allGirdElements[i].obj = malloc(sizeof(cell));
        ((cell *)allGirdElements[i].obj)->e = AIR;
        ((cell *)allGirdElements[i].obj)->isMoving = 0;

        stonesGrid[i].obj = malloc(sizeof(cell));
        ((cell *)stonesGrid[i].obj)->e = STONE;

        stoneLineGrid[i].obj = malloc(sizeof(cell));
        ((cell *)stoneLineGrid[i].obj)->e = STONE;
    }

    for (int i = 0; i < lCount; i++)
    {
        if (strlen(lines[i]) <= 0)
        {
            continue;
        }
        // printf("%s\n", lines[i]);
        char *split = strtok(lines[i], " -> ");
        linkedList *cordsPairs = createList();
        while (split != NULL)
        {
            listPushBack(cordsPairs, split);
            split = strtok(NULL, " -> ");
        }
        linkedList *t = cordsPairs->next;
        int firstElement = 1;
        while (t != NULL)
        {
            char *cX = strtok(t->obj, ",");
            char *cY = strtok(NULL, ",");
            assert(cX != NULL && cY != NULL);

            // printf("x %s y %s\n", cX, cY);

            stonesGrid[currStoneGridIndex].x = atoi(cX);
            stonesGrid[currStoneGridIndex].y = atoi(cY);
            ((cell *)stonesGrid[currStoneGridIndex].obj)->e = STONE;
            currStoneGridIndex++;

            if (!firstElement && currStoneGridIndex > 1)
            {
                gridNode *target = &stonesGrid[currStoneGridIndex - 1];
                // draw line between this and last point;
                if (stonesGrid[currStoneGridIndex - 1].y == stonesGrid[currStoneGridIndex - 2].y)
                {
                    // x line

                    int currX = stonesGrid[currStoneGridIndex - 2].x;
                    assert((target->x - currX) != 0);
                    int dir = (target->x - currX) / abs(target->x - currX); // positive -> x+
                    currX += dir;
                    while (target->x != currX)
                    {
                        // printf("make x line; x %i y %i\n", currX, target->y);
                        stoneLineGrid[currStoneLineIndex].x = currX;
                        stoneLineGrid[currStoneLineIndex].y = target->y;
                        ((cell *)stoneLineGrid[currStoneLineIndex].obj)->e = STONE;
                        currStoneLineIndex++;
                        currX += dir;
                    }
                }
                else
                {
                    // y line
                    int currY = stonesGrid[currStoneGridIndex - 2].y;
                    assert((target->y - currY) != 0);
                    int dir = (target->y - currY) / abs(target->y - currY); // positive -> y+
                    currY += dir;
                    while (target->y != currY)
                    {
                        // printf("make y line; x %i y %i\n", target->x, currY);
                        stoneLineGrid[currStoneLineIndex].x = target->x;
                        stoneLineGrid[currStoneLineIndex].y = currY;
                        ((cell *)stoneLineGrid[currStoneLineIndex].obj)->e = STONE;
                        currStoneLineIndex++;
                        currY += dir;
                    }
                }
            }
            else
            {
                firstElement = 0;
            }

            t = t->next;
        }
        deleteList(cordsPairs);
    }

    // search for max x and y
    int xMax = 0;
    int yMax = 0;
    int xMin = -1;
    int yMin = 0;
    for (int i = 0; i < currStoneGridIndex; i++)
    {
        if (stonesGrid[i].x > xMax)
        {
            xMax = stonesGrid[i].x;
        }
        if (stonesGrid[i].x < xMin || xMin == -1)
        {
            xMin = stonesGrid[i].x;
        }
        if (stonesGrid[i].y > yMax)
        {
            yMax = stonesGrid[i].y;
        }
    }
    xMax++;
    yMax += 3;
    if (yMin > 0)
    {
        yMin--;
    }
    if (xMin > 0)
    {
        xMin--;
    }

    printf("xMax %i, yMax %i, xMin %i, yMin %i\n", xMax, yMax, xMin, yMin);

    // make floor for part 2
    // 500 because the source is at x 500

    assert((yMax - 1) <= 500);

    for (int x = (500 - (yMax - 1)); x < (500 + yMax); x++)
    {
        stoneLineGrid[currStoneLineIndex].x = x;
        stoneLineGrid[currStoneLineIndex].y = yMax - 1;
        ((cell *)stoneLineGrid[currStoneLineIndex].obj)->e = STONE;
        currStoneLineIndex++;
    }

    xMin = (500 - (yMax - 1));
    xMax = (500 + yMax);

    // init grid
    int currX = 0;
    int currY = 0;

    int deltaX = xMax - xMin;

    for (int y = yMin; y < yMax; y++)
    {
        for (int x = xMin; x < xMax; x++)
        {

            // search for rooks edge
            for (int k = 0; k < currStoneGridIndex; k++)
            {
                if (stonesGrid[k].x == x && stonesGrid[k].y == y)
                {
                    allGirdElements[currGridIndex].x = stonesGrid->x;
                    allGirdElements[currGridIndex].y = stonesGrid->y;
                    ((cell *)allGirdElements[currGridIndex].obj)->e = STONE;
                    allGirdElements[currGridIndex].neighbor = malloc((8 * sizeof(void *)));

                    goto MAKE_NEIGHBOR;
                }
            }
            // search for line
            for (int k = 0; k < currStoneLineIndex; k++)
            {
                if (stoneLineGrid[k].x == x && stoneLineGrid[k].y == y)
                {
                    allGirdElements[currGridIndex].x = stoneLineGrid->x;
                    allGirdElements[currGridIndex].y = stoneLineGrid->y;
                    ((cell *)allGirdElements[currGridIndex].obj)->e = STONE;
                    allGirdElements[currGridIndex].neighbor = malloc((8 * sizeof(void *)));

                    goto MAKE_NEIGHBOR;
                }
            }
            // create air

            allGirdElements[currGridIndex].x = x;
            allGirdElements[currGridIndex].y = y;
            ((cell *)allGirdElements[currGridIndex].obj)->e = AIR;
            allGirdElements[currGridIndex].neighbor = malloc((8 * sizeof(void *)));

        MAKE_NEIGHBOR:

            // pointer grid

            // North, NE, NW

            if (y == yMin)
            {
                allGirdElements[currGridIndex].neighbor[N] = NULL;
                allGirdElements[currGridIndex].neighbor[NE] = NULL;
                allGirdElements[currGridIndex].neighbor[NW] = NULL;
            }
            else
            {
                // North<->South
                allGirdElements[currGridIndex].neighbor[N] = &allGirdElements[currGridIndex - deltaX];
                allGirdElements[currGridIndex - deltaX].neighbor[S] = &allGirdElements[currGridIndex];
                // NE<->SW
                allGirdElements[currGridIndex].neighbor[NE] = &allGirdElements[currGridIndex - deltaX + 1];
                allGirdElements[currGridIndex - deltaX + 1].neighbor[SW] = &allGirdElements[currGridIndex];
                // NW->SE
                if (x > xMin)
                {
                    allGirdElements[currGridIndex].neighbor[NW] = &allGirdElements[currGridIndex - deltaX - 1];
                    allGirdElements[currGridIndex - deltaX - 1].neighbor[SE] = &allGirdElements[currGridIndex];
                }
                else
                {
                    allGirdElements[currGridIndex].neighbor[NW] = NULL;
                }
            }
            // W <-> E
            if (x > xMin)
            {
                allGirdElements[currGridIndex].neighbor[W] = &allGirdElements[currGridIndex - 1];
                allGirdElements[currGridIndex - 1].neighbor[E] = &allGirdElements[currGridIndex];
            }
            else
            {
                allGirdElements[currGridIndex].neighbor[W] = NULL;
            }
            // S, SW, SE, E
            allGirdElements[currGridIndex].neighbor[S] = NULL;
            allGirdElements[currGridIndex].neighbor[SW] = NULL;
            allGirdElements[currGridIndex].neighbor[SE] = NULL;
            allGirdElements[currGridIndex].neighbor[E] = NULL;

            if (x == 500 && y == 0)
            {
                sandSource = &allGirdElements[currGridIndex];
            }

            currGridIndex++;
        }
    }

    ((cell *)sandSource->obj)->e = SOURCE;

    // showGrid(&allGirdElements[0], printCell);

    gridNode *movingSandCell = NULL;
    int restedSand = 0;

    while (movingSandCell != NULL || ((cell *)sandSource->obj)->e != SAND)
    {
        if (movingSandCell == 0)
        {
            // spawn new sand
            ((cell *)sandSource->obj)->e = SAND;
            movingSandCell = sandSource;
        }
        else
        {
            if (movingSandCell->neighbor[S] == NULL)
            {
                goto SAND_FALL_INTO_VOID;
            }

            switch (getCell(movingSandCell->neighbor[S]->obj)->e)
            {
            case PATH:
            case AIR:
            {
                moveSand(&movingSandCell, S, sandSource);
            }
            break;
            case STONE:
            case SAND:
            {
                if (getCell(movingSandCell->neighbor[SW]->obj)->e == AIR || getCell(movingSandCell->neighbor[SW]->obj)->e == PATH)
                {
                    moveSand(&movingSandCell, SW, sandSource);
                }
                else if (getCell(movingSandCell->neighbor[SE]->obj)->e == AIR || getCell(movingSandCell->neighbor[SE]->obj)->e == PATH)
                {
                    moveSand(&movingSandCell, SE, sandSource);
                }
                else
                {
                    // path blocked
                    movingSandCell = NULL;
                    restedSand++;
                }
            }
            break;
            }
            // showGrid(&allGirdElements[0], printCell);
        }
    }
SAND_FALL_INTO_VOID:
    showGrid(&allGirdElements[0], printCell);
    printf("rested Sand %i\n", restedSand);
    return 0;
}
