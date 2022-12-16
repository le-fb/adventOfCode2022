#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/**
 * reads a file
 * returns size of file and -1 for an error
 */

int readFile(char *filename, char ***content, int *lines);

/**
 * char to int
 * returns -1 if char is not a number
 */

int cToI(char c);

// double linked list
typedef struct linkedNode linkedNode;
typedef int (*nodeCmp)(void *obj0, void *obj1); // compare function for sorting
typedef void (*printObj)(void *obj);            // for printing to obj in the node

struct linkedNode
{
    void *obj;
    int type;
    // int currSize;    // only for root node
    linkedNode *end; // only for root node
    linkedNode *pre;
    linkedNode *next;
};

/**
 * init linked node and returns it
 */
linkedNode *createLinkedNode(void *obj);

/**
 * return size of linkedNode List
 */
int getNodeSize(linkedNode *root);

/**
 * return 1 if the list contains the node else 0
 */
int linkedNodeContains(linkedNode *root, linkedNode *node);

/**
 * return new root of list after removing one element
 */
linkedNode *removeNode(linkedNode *root, linkedNode *node);

/**
 * add new node on the end off the list
 * returns new root element
 */
linkedNode *pushBackNode(linkedNode *root, linkedNode *node);

/**
 * swaps to elements
 */
void swapNodes(linkedNode *n0, linkedNode *n1);

linkedNode *partition(linkedNode *low, linkedNode *high, nodeCmp cmp);

void quickSort(linkedNode *low, linkedNode *high, nodeCmp cmp);

/**
 * sorts list
 */
void sortNodes(linkedNode *root, nodeCmp cmp);

/**
 * prints linkedNodeList
 */
void *printNodes(linkedNode *root, printObj p);

/**
 * grid stuff
 */

typedef struct gridNode gridNode;
typedef void (*printGridObj)(void *obj);

typedef enum gridDirection gridDirection;
enum gridDirection
{
    N = 0,
    NE = 1,
    E,
    SE,
    S,
    SW,
    W,
    NW
};

struct gridNode
{
    gridNode **neighbor;
    int x;
    int y;
    void *obj;
};

int gridDist(gridNode *a, gridNode *b);

void showGrid(gridNode *grid, printGridObj print);

/**
 * A* stuff
 */
typedef struct aStarObj aStarObj;

struct aStarObj
{
    int isStart;
    int isEnd;

    int pathCost;
    int distToGo;
    int currPathCost;
    int currWeight;

    gridNode *pos;

    linkedNode *aStarNeighbour;

    aStarObj *prev;
};

int aStarObjCmp(void *obj0, void *obj1);

/**
 * makes a start spreeding returns a pointer to the last object
 * including the track back to start
 * returns NULL if no way has been found
 */

aStarObj *aStarSearch(aStarObj *root);

#endif