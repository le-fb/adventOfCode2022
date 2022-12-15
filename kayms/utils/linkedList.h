#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct linkedList linkedList;
typedef int (*listEntryCmp)(void *obj0, void *obj1); // compare function for sorting
typedef void (*printObj)(void *obj);                 // for printing to obj in the node

typedef enum entryType entryType;
typedef struct entry entry;

struct entry
{
    int type;
    int number;
    linkedList *arr;
    int flag;
};

enum entryType
{
    NUMBER,
    ARRAY
};

struct linkedList
{
    int isRoot;
    int size;

    entry *obj;
    int type;

    linkedList *end;
    linkedList *pre;
    linkedList *next;
};

linkedList *createList();

int listSize(linkedList *root);

int listContains(linkedList *root, linkedList *node);

void listRemoveEntry(linkedList *root, linkedList *node);

void listPushBack(linkedList *root, void *obj);

void *listPop(linkedList *root);

int listGetIndex(linkedList *node);

void deleteList(linkedList *list);

void swapListEntry(linkedList *n0, linkedList *n1);

linkedList *partitionList(linkedList *low, linkedList *high, listEntryCmp cmp);

void listQuickSort(linkedList *low, linkedList *high, listEntryCmp cmp);

/**
 * sorts list
 */
void sortList(linkedList *root, listEntryCmp cmp);

/**
 * prints linkedListList
 */
void *printList(linkedList *root, printObj p);

#endif