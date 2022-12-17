#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#include <sys/types.h>

#include <linkedList.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define INPUT_FILE_NAME "../../kayms/16/sampleInput"
#define MAX_NEIGHBOR 10

typedef struct graphNode graphNode;

struct graphNode
{
    char name[100];
    int flowRate;
    graphNode **neighbor;
    int *pathcost;
    int neighborCount;
};

graphNode *graph;
int nodeCount = 0;

int main()
{
    char **lines;
    int lCount = 0;

    int r = readFile(INPUT_FILE_NAME, &lines, &lCount);

    graph = malloc(lCount * sizeof(graphNode));

    for (int i = 0; i < lCount; i++)
    {
        char *buff0;
        char *buff1;

        char *split = strtok_r(lines[i], " ", &buff0);
        if (split != NULL)
        {
            char *name = strtok_r(NULL, " ", &buff0);
            strtok_r(NULL, " ", &buff0);
            strtok_r(NULL, " ", &buff0);
            char *rate = strtok_r(NULL, " ", &buff0);
            strtok_r(rate, "=", &buff1);
            rate = strtok_r(NULL, "=;", &buff1);

            // printf("name %s rate %s\n", name, rate);
            int nodeIndex = -1;
            // search if node allready exsists
            for (int j = 0; j < nodeCount; j++)
            {
                if (strcmp(graph[j].name, name) == 0)
                {
                    nodeIndex = j;
                    break;
                }
            }
            if (nodeIndex < 0)
            {
                nodeIndex = nodeCount;
                strcpy(graph[nodeIndex].name, name);
                graph[nodeIndex].neighbor = malloc(MAX_NEIGHBOR * sizeof(void *));
                graph[nodeIndex].neighborCount = 0;
                nodeCount++;
            }
            graph[nodeIndex].flowRate = atoi(rate);

            char *n = strtok_r(NULL, " ,", &buff0);
            n = strtok_r(NULL, " ,", &buff0);
            n = strtok_r(NULL, " ,", &buff0);
            n = strtok_r(NULL, " ,", &buff0);
            n = strtok_r(NULL, " ,", &buff0);
            while (n != NULL)
            {
                // printf("%s\n", n);

                int nIndex = -1;
                // search if node allready exsists
                for (int j = 0; j < nodeCount; j++)
                {
                    if (strcmp(graph[j].name, n) == 0)
                    {
                        nIndex = j;
                        break;
                    }
                }

                if (nIndex < 0)
                {
                    nIndex = nodeCount;
                    strcpy(graph[nodeCount].name, n);
                    graph[nodeCount].neighborCount = 0;
                    graph[nodeCount].neighbor = malloc(MAX_NEIGHBOR * sizeof(void *));
                    nodeCount++;
                }
                graph[nodeIndex].neighbor[graph[nodeIndex].neighborCount] = &graph[nIndex];
                graph[nodeIndex].neighborCount++;

                n = strtok_r(NULL, " ,", &buff0);
            }
        }
    }

    return 0;
}