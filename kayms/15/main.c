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

#define INPUT_FILE_NAME "../../kayms/15/input"

#define ROTATION (M_PI / 4.0) // 45°
#define PRECISION 0.05

typedef struct sensor sensor;
typedef struct beacon beacon;
double roationMatix[2][2] = {
    {cos(ROTATION), -sin(ROTATION)},
    {sin(ROTATION), cos(ROTATION)}};

double roationMatixRev[2][2] = {
    {cos(-ROTATION), -sin(-ROTATION)},
    {sin(-ROTATION), cos(-ROTATION)}};

sensor **allSensors;
beacon **allBeacons;
int sensorIndex = 0;

struct beacon
{
    double *pos;
};

struct sensor
{
    int dist;
    int *orgPos;
    int **orgCorners;
    double *pos;      // x,y
    double **corners; // top(x,y), right, bottom, left
    int isTra;        // just for debugging
};

beacon *createBeacon(int x, int y)
{
    beacon *b = malloc(sizeof(beacon));
    b->pos = malloc(2 * sizeof(double));
    b->pos[0] = x;
    b->pos[1] = y;
    return b;
}

sensor *createSensor(int x, int y, int dist)
{
    sensor *s = malloc(sizeof(sensor));
    s->pos = malloc(2 * sizeof(double));
    s->orgPos = malloc(2 * sizeof(int));
    s->orgCorners = malloc(4 * sizeof(void));

    s->dist = dist;

    s->orgPos[0] = x;
    s->orgPos[1] = y;

    s->pos[0] = x;
    s->pos[1] = y;

    s->corners = malloc(4 * sizeof(void *));
    for (int i = 0; i < 4; i++)
    {
        s->corners[i] = malloc(2 * sizeof(double));
        s->orgCorners[i] = malloc(2 * sizeof(int));
    }

    // top
    s->orgCorners[0][0] = x;
    s->orgCorners[0][1] = dist + y;
    s->corners[0][0] = x;
    s->corners[0][1] = dist + y;
    // right
    s->orgCorners[1][0] = x + dist;
    s->orgCorners[1][1] = y;
    s->corners[1][0] = x + dist;
    s->corners[1][1] = y;
    // bottom
    s->orgCorners[2][0] = x;
    s->orgCorners[2][1] = y - dist;
    s->corners[2][0] = x;
    s->corners[2][1] = y - dist;
    // left
    s->orgCorners[3][0] = x - dist;
    s->orgCorners[3][1] = y;
    s->corners[3][0] = x - dist;
    s->corners[3][1] = y;

    s->isTra = 0;

    return s;
}
void printSensor(sensor *s)
{
    printf("mid x %f y %f\n", s->pos[0], s->pos[1]);

    for (int i = 0; i < 4; i++)
    {
        printf("corner %i: x %f y %f\n", i, s->corners[i][0], s->corners[i][1]);
    }
    printf("\n");
}

void updateLimits(int *xMax, int *xMin, int *yMin, int *yMax, double *pos)
{
    if (*xMin > pos[0])
    {
        *xMin = pos[0];
    }

    if (*xMax < pos[0])
    {
        *xMax = pos[0];
    }

    if (*yMin > pos[1])
    {
        *yMin = pos[1];
    }

    if (*yMax < pos[1])
    {
        *yMax = pos[1];
    }
}

void sensorUpdateLimits(int *xMax, int *xMin, int *yMin, int *yMax, sensor *s)
{
    updateLimits(xMax, xMin, yMin, yMax, s->pos);
    for (int i = 0; i < 4; i++)
    {
        updateLimits(xMax, xMin, yMin, yMax, s->corners[i]);
    }
}

// math pos dir is left!
//  assume pos is x,y
void transformPos(double *pos)
{
    double x = (roationMatix[0][0] * pos[0]) + (roationMatix[0][1] * pos[1]);
    double y = (roationMatix[1][0] * pos[0]) + (roationMatix[1][1] * pos[1]);
    // printf("%f %f\n", x, y);
    pos[0] = x;
    pos[1] = y;
}
void transformBackPos(double *pos)
{
    double x = (roationMatixRev[0][0] * pos[0]) + (roationMatixRev[0][1] * pos[1]);
    double y = (roationMatixRev[1][0] * pos[0]) + (roationMatixRev[1][1] * pos[1]);
    // printf("%f %f\n", x, y);
    pos[0] = x;
    pos[1] = y;
}

void transformSensor(sensor *s)
{
    transformPos(s->pos);

    for (int i = 0; i < 4; i++)
    {
        transformPos(s->corners[i]);
    }
    s->isTra = 1;

    assert(fabs(s->corners[0][1] - s->corners[1][1]) < PRECISION && fabs(s->corners[0][0] - s->corners[3][0]) < PRECISION);
}

int isInSensorRange(double *pos, sensor *s)
{
    assert(s->isTra == 1);
    // assume the sensor range is allways square and is parallel to x and y

    // check
    return (s->corners[0][1] >= pos[1] && s->corners[2][1] <= pos[1] && s->corners[0][0] <= pos[0] && s->corners[1][0] >= pos[0]);
}
int isSensorPos(double *pos, sensor *s)
{
    return (fabs(pos[0] - s->pos[0])) < PRECISION && (fabs(pos[1] - s->pos[1])) < PRECISION;
}

int isBeaconPos(double *pos, beacon *b)
{
    return (fabs(pos[0] - b->pos[0])) < PRECISION && (fabs(pos[1] - b->pos[1])) < PRECISION;
}

int manhattanDis(int pos0[2], int pos1[2])
{
    return abs(pos0[0] - pos1[0]) + abs(pos0[1] - pos1[1]);
}

int isInOrgSensorRange(int *pos, sensor *s)
{
    return (manhattanDis(pos, s->orgPos) <= s->dist);
}

int main()
{
    char **lines;
    int lCount = 0;

    int xMin = INT_MAX - 1;
    int xMax = 0;
    int yMin = INT_MAX - 1;
    int yMax = 0;

    int r = readFile(INPUT_FILE_NAME, &lines, &lCount);

    allSensors = malloc(lCount * sizeof(void *));
    allBeacons = malloc(lCount * sizeof(void *));
    sensorIndex = 0;

    for (int i = 0; i < lCount - 1; i++)
    {
        int coords[4];
        int coordIndex = 0;
        char *buf0;
        char *buf1;

        char *split = strtok_r(lines[i], "=", &buf0);
        split = strtok_r(NULL, "=", &buf0);

        while (split != NULL)
        {
            char *num = strtok_r(split, ",:", &buf1);
            coords[coordIndex] = atoi(num);
            coordIndex++;
            split = strtok_r(NULL, "=", &buf0);
            assert(coordIndex < 5);
        }

        int pos0[2] = {coords[0], coords[1]};
        int pos1[2] = {coords[2], coords[3]};

        double *p = malloc(2 * sizeof(double));
        p[0] = coords[2];
        p[1] = coords[3];

        updateLimits(&xMax, &xMin, &yMin, &yMax, p);
        free(p);

        int dist = manhattanDis(pos0, pos1);
        // printf("dist %i\n", dist);
        allSensors[sensorIndex] = createSensor(coords[0], coords[1], dist);
        allBeacons[sensorIndex] = createBeacon(coords[2], coords[3]);

        sensorUpdateLimits(&xMax, &xMin, &yMin, &yMax, allSensors[sensorIndex]);

        // printSensor(allSensors[sensorIndex]);
        transformSensor(allSensors[sensorIndex]);
        // printf("tansformed:\n");
        // printSensor(allSensors[sensorIndex]);
        transformPos(allBeacons[sensorIndex]->pos);
        sensorIndex++;
    }

    printf("xMin %i, xMax %i, yMin %i, yMax %i\n", xMin, xMax, yMin, yMax);

    double *currPos = malloc(2 * sizeof(double));
    int sum = 0;

    // go one line
    for (int x = xMin; x <= xMax; x++)
    {
        currPos[0] = x;
        currPos[1] = 2000000.0;
        // printf("x %f,y %f\n", currPos[0], currPos[1]);
        transformPos(currPos);
        // printf("x %f,y %f\n\n", currPos[0], currPos[1]);

        for (int i = 0; i < sensorIndex; i++)
        {

            if (isInSensorRange(currPos, allSensors[i]) && !isSensorPos(currPos, allSensors[i]))
            {
                for (int j = 0; j < sensorIndex; j++)
                {
                    if (isBeaconPos(currPos, allBeacons[j]))
                    {
                        goto NEXT_STEP;
                    }
                }
                sum++;
                break;
            }
        NEXT_STEP:
            // printf("%i\n", i);
        }
    }
    printf("sum %i\n", sum);

    // beacon search

    int *pos = malloc(2 * sizeof(int));

    for (int y = 0; y < 4000000; y++)
    {
        for (int x = 0; x < 4000000; x++)
        {
            pos[0] = x;
            pos[1] = y;

            int found = 0;
            for (int i = 0; i < sensorIndex; i++)
            {
                if (isInOrgSensorRange(pos, allSensors[i]))
                {
                    int deltaJump = abs(y - allSensors[i]->orgPos[1]);

                    x = allSensors[i]->orgCorners[1][0] - deltaJump;
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                double temp = (((double)x) * 4000000) + y;
                printf("x %i, y %i, sum %f \n", x, y, temp);
            }
        }
    }

    return 0;
}