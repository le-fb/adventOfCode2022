#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * reads a file
 * returns 1 for success
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
linkedNode *sortNodes(linkedNode *root, nodeCmp cmp);

/**
 * prints linkedNodeList
 */
void *printNodes(linkedNode *root, printObj p);

#endif