#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#include <sys/types.h>
#include <utils.h>
#include <linkedList.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define INPUT_FILE_NAME "../../kayms/17/input"
#define MAX_HEIGHT 6000
#define MAX_SCAPE_COUNT (MAX_HEIGHT * 2)

typedef struct stone stone;
typedef struct scape scape;
typedef enum stoneType stoneType;

scape *unitScape;

enum stoneType
{
    H_LINE_STONE,
    CROSS_STONE,
    L_STONE,
    V_LINE_STONE,
    BLOCK_STONE
};

struct scape
{
    // x,y is top left
    int x;
    int y;
    int width;
    int height;
};

struct stone
{
    int x;
    int y;
    int isMoving;
    scape *scapes;
    int scapeCount;
    int height;
};

stone *makeStone(stoneType type)
{
    stone *s = malloc(sizeof(stone));
    s->isMoving = 1;

    switch (type)
    {
    case H_LINE_STONE:
    {
        s->scapes = malloc(sizeof(scape));
        s->scapes->x = 0;
        s->scapes->y = 0;
        s->scapes->height = 1;
        s->scapes->width = 4;

        s->x = 0;
        s->y = 0;
        s->scapeCount = 1;
        s->height = 1;
    }
    break;
    case CROSS_STONE:
    {
        s->scapes = malloc(2 * sizeof(scape));
        // vertical
        s->scapes[0].x = 1;
        s->scapes[0].y = 0;
        s->scapes[0].height = 3;
        s->scapes[0].width = 1;
        // horizial
        s->scapes[1].x = 0;
        s->scapes[1].y = -1;
        s->scapes[1].height = 1;
        s->scapes[1].width = 3;

        s->x = 0;
        s->y = 0;
        s->scapeCount = 2;
        s->height = 3;
    }
    break;
    case L_STONE:
    {
        s->scapes = malloc(2 * sizeof(scape));
        // vertical
        s->scapes[0].x = 2;
        s->scapes[0].y = 0;
        s->scapes[0].height = 3;
        s->scapes[0].width = 1;
        // horizial
        s->scapes[1].x = 0;
        s->scapes[1].y = -2;
        s->scapes[1].height = 1;
        s->scapes[1].width = 3;

        s->x = 0;
        s->y = 0;
        s->scapeCount = 2;
        s->height = 3;
    }
    break;
    case V_LINE_STONE:
    {
        s->scapes = malloc(sizeof(scape));
        s->scapes->x = 0;
        s->scapes->y = 0;
        s->scapes->height = 4;
        s->scapes->width = 1;

        s->x = 0;
        s->y = 0;
        s->scapeCount = 1;
        s->height = 4;
    }
    break;
    case BLOCK_STONE:
    {
        s->scapes = malloc(sizeof(scape));
        s->scapes->x = 0;
        s->scapes->y = 0;
        s->scapes->height = 2;
        s->scapes->width = 2;

        s->x = 0;
        s->y = 0;
        s->scapeCount = 1;
        s->height = 2;
    }
    break;

    default:
        assert(0);
        break;
    }

    return s;
}

int checkCollision(scape *a, scape *b)
{
    assert(a->height > 0 && a->width > 0 && b->height > 0 && b->width > 0);

    // b rechts von a oder a rechts von b

    if ((a->x + a->width) <= b->x || (b->x + b->width) <= a->x)
    {
        return 0;
    }

    // a over b oder b over a

    if ((a->y - a->height) >= b->y || (b->y - b->height) >= a->y)
    {
        return 0;
    }
    return 1;
}

void moveStone(int x, int y, stone *s)
{

    int deltaX = x - s->x;
    int deltaY = y - s->y;

    s->x = x;
    s->y = y;

    for (int i = 0; i < s->scapeCount; i++)
    {
        s->scapes[i].x += deltaX;
        s->scapes[i].y += deltaY;
    }
}

int decyptCom(char c)
{
    switch (c)
    {
    case '<':
        return -1;
    case '>':
        return 1;

    default:
        assert(0);
        break;
    }
}
int posIsScape(int x, int y, scape **playField, int playfieldCount)
{
    for (int i = 0; i < playfieldCount; i++)
    {
        if (checkCollision(unitScape, playField[i]))
        {
            return 1;
        }
    }
    return 0;
}

unsigned long hashScape(scape *s)
{
}

unsigned long makePlayFieldHash(int maxY, scape **playField, int pFIndex)
{
    char *r = malloc((1000) * sizeof(char));
    int charIndex = 0;
    for (int y = maxY + 10; y >= maxY - 70; y--)
    {
        for (int x = -1; x < 8; x++)
        {
            unitScape->x = x;
            unitScape->y = y;

            if (posIsScape(x, y, playField, pFIndex))
            {
                r[charIndex] = '#';
            }
            else
            {
                r[charIndex] = '.';
            }
            charIndex++;
        }
        r[charIndex] = '\n';
        charIndex++;
    }
    r[charIndex] = '\0';
    // printf("%s \n", r);

    unsigned long ret = hash(r);
    // printf("%lu \n", ret);

    free(r);
    return ret;
}

void printPlayField(int maxY, scape **playField, int pFIndex, stone *currStone)
{
    for (int y = maxY + 10; y >= maxY - 30; y--)
    {
        for (int x = -1; x < 8; x++)
        {
            unitScape->x = x;
            unitScape->y = y;

            if (posIsScape(x, y, playField, pFIndex))
            {
                printf("#");
            }
            else
            {
                if (currStone != NULL)
                {
                    int foundColl = 0;
                    for (int i = 0; i < currStone->scapeCount; i++)
                    {
                        if (checkCollision(unitScape, &currStone->scapes[i]))
                        {
                            foundColl = 1;
                        }
                    }
                    if (foundColl)
                    {
                        printf("#");
                    }
                    else
                    {
                        printf(".");
                    }
                }
                else
                {
                    printf(".");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{

    char **lines;
    int lCount = 0;

    u_int64_t rockCount = 5000;
    u_int64_t currRockCount = 0;

    int r = readFile(INPUT_FILE_NAME, &lines, &lCount);

    assert(lCount == 1);
    char *commands = lines[0];
    int comIndex = 0;
    int lineEnd = strlen(lines[0]);
    int maxY = 0;

    unitScape = malloc(sizeof(scape));
    unitScape->x = 1;
    unitScape->y = 1;
    unitScape->height = 1;
    unitScape->width = 1;

    stoneType currStType = H_LINE_STONE;

    scape **playField = malloc(MAX_SCAPE_COUNT * sizeof(void *));
    int pFIndex = 0;

    // make playfield
    scape *floor = malloc(sizeof(scape));
    floor->x = 0;
    floor->y = 0;
    floor->height = 1;
    floor->width = 7;

    scape *l_wall = malloc(sizeof(scape));
    l_wall->x = -1;
    l_wall->y = (rockCount * 10) + 10;
    l_wall->height = (rockCount * 10) + 10;
    l_wall->width = 1;

    scape *r_wall = malloc(sizeof(scape));
    r_wall->x = 7;
    r_wall->y = (rockCount * 10) + 10;
    r_wall->height = (rockCount * 10) + 10;
    r_wall->width = 1;

    playField[0] = floor;
    playField[1] = l_wall;
    playField[2] = r_wall;

    pFIndex = 3;

    // printPlayField(maxY, playField, pFIndex, NULL);

    int currX = 0;
    int currY = 0;
    int nextX = 0;
    int nextY = 0;

    int stoneIsFalling = 0;

    u_int64_t *hashBuffer = malloc(10000 * sizeof(u_int64_t));
    u_int64_t hashBufferIndex = 0;
    u_int64_t dHash = 0;
    u_int64_t firstFoundHeight = 0;
    u_int64_t firstRockcount = 0;
    u_int64_t simCount = 1000000000000;
    u_int64_t simHeight = 0;
    u_int64_t analyzed = 0;
    u_int64_t simStartHeight = 0;
    u_int64_t simDeltaHeight = 0;

    while (currRockCount < rockCount)
    {
        stone *currS = makeStone(currStType);
        currStType = (currStType + 1) % 5;
        // move to start location
        currX = 2;
        currY = (maxY + currS->height + 3);
        nextX = currX;
        nextY = currY;
        moveStone(currX, currY, currS);
        stoneIsFalling = 1;
        // printf("curr rock %i\n", currRockCount);

        while (stoneIsFalling)
        {

            // make com input
            // printf("com %c index %i\n", lines[0][comIndex], comIndex);
            nextX += decyptCom(lines[0][comIndex]);
            comIndex = (comIndex + 1) % lineEnd;
            moveStone(nextX, nextY, currS);
            // check for collision
            int collision = 0;
            for (int i = 0; i < pFIndex; i++)
            {
                for (int j = 0; j < currS->scapeCount; j++)
                {
                    if (checkCollision(playField[i], &currS->scapes[j]))
                    {
                        collision = 1;
                        goto COL_0;
                    }
                }
            }
        COL_0:
            if (collision)
            {
                nextX = currX;
                // move back
                moveStone(nextX, nextY, currS);
            }

            // stone drop
            nextY--;
            moveStone(nextX, nextY, currS);
            collision = 0;
            for (int i = 0; i < pFIndex; i++)
            {
                for (int j = 0; j < currS->scapeCount; j++)
                {
                    if (checkCollision(playField[i], &currS->scapes[j]))
                    {
                        collision = 1;
                        goto COL_1;
                    }
                }
            }
        COL_1:
            if (collision)
            {
                nextY = currY;
                moveStone(nextX, nextY, currS);
                // stone cannot fall any more
                int scapeMaxY = 0;
                for (int i = 0; i < currS->scapeCount; i++)
                {
                    if (currS->scapes[i].y > scapeMaxY)
                    {
                        scapeMaxY = currS->scapes[i].y;
                    }
                    playField[pFIndex] = &currS->scapes[i];
                    pFIndex++;
                }
                if (maxY < scapeMaxY)
                {
                    maxY = scapeMaxY;
                }
                stoneIsFalling = 0;
                currRockCount++;
                free(currS);
                currS = NULL;
            }

            currX = nextX;
            currY = nextY;
            assert(currX > -2 && currX < 8 && currY > -1);
            // printPlayField(maxY, playField, pFIndex, currS);
        }

        if (!analyzed)
        {
            unsigned long h = makePlayFieldHash(maxY, playField, pFIndex);

            for (int i = 0; i < hashBufferIndex; i++)
            {
                if (dHash > 0 && dHash == h)
                {

                    printf("pattern found\n");
                    printf("pattern rock height %i count %i\n", maxY - firstFoundHeight, currRockCount - firstRockcount);
                    simDeltaHeight = maxY - firstFoundHeight;

                    printf("offset height %i, rockcount %i \n\n", firstFoundHeight, firstRockcount);

                    simCount -= firstRockcount;
                    unsigned long steps = (u_int64_t)simCount / (currRockCount - firstRockcount);
                    simCount = simCount % (currRockCount - firstRockcount);

                    simHeight += firstFoundHeight + simDeltaHeight * steps;

                    printf("sim rest stones %lu\nsteps %lu\nsimHeight %lu\n\n", simCount, steps, simHeight);

                    currRockCount = rockCount - (u_int64_t)simCount;
                    printf("current rock count %i\n", currRockCount);
                    break;
                }
                if (dHash == 0 && h == hashBuffer[i])
                {
                    firstFoundHeight = maxY;
                    firstRockcount = currRockCount;

                    printf("found offset, height %i rockCount %i\n", maxY, currRockCount);

                    dHash = h;
                    break;
                }
            }
            hashBuffer[hashBufferIndex] = h;
            hashBufferIndex++;
        }
        // printPlayField(maxY, playField, pFIndex, currS);
    }

    // printPlayField(maxY, playField, pFIndex, NULL);
    printf("rock count %i, max y %i\n", currRockCount, maxY);
    printf("sim to count 1e12 height %lu\n", simHeight + (maxY - firstFoundHeight) - simDeltaHeight);

    return 0;
}