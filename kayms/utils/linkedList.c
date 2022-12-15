
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "linkedList.h"

int printCall = 0;

linkedList *createList()
{
    linkedList *root = malloc(sizeof(linkedList));
    root->isRoot = 1;
    root->end = root;
    root->next = NULL;
    root->pre = NULL;
    root->obj = NULL;
    root->size = 0;
}

void listPushBack(linkedList *root, void *obj)
{
    assert(root != NULL);
    linkedList *e = malloc(sizeof(linkedList));
    if (e == NULL)
    {
        perror("Unable to allocate memory\n");
    }
    e->isRoot = 0;
    e->end = NULL;
    e->next = NULL;
    e->pre = root->end;
    e->obj = obj;
    root->end->next = e;

    root->size++;
    root->end = e;
    if (root->next == NULL)
    {
        root->next = e;
    }
}

int listSize(linkedList *root)
{
    return root->size;
}

int listContains(linkedList *root, linkedList *node)
{
    assert(root != NULL && node != NULL);

    if (root->size <= 0)
        return 0;
    linkedList *curr = root->next;

    while (curr != NULL)
    {
        if (curr->obj == node->obj)
        {
            return 1;
        }
    }
}

void listRemoveEntry(linkedList *root, linkedList *node)
{
    root->size--;
    node->pre->next = node->next;
    if (node->next != NULL)
    {
        node->next->pre = node->pre;
    }
    else
    {
        // if ele is end of the list
        root->end = node->pre;
    }
}

void *listPop(linkedList *root)
{
    if (root->size <= 0)
        return NULL;
    void *obj = root->end->obj;
    linkedList *e = root->end;
    root->end->pre->next = NULL;
    root->end = root->end->pre;
    root->size--;
    free(e); // mem leak dont know to fix that jet
    return obj;
}

int listGetIndex(linkedList *node)
{
    if (node->isRoot)
    {
        return -1;
    }
    linkedList *temp = node->pre;
    int i = 0;
    while (temp != NULL)
    {
        if (temp->isRoot)
        {
            return i;
        }
        i++;
        temp = temp->pre;
    }
    return -1;
}

void deleteList(linkedList *list)
{
    linkedList *temp = list;
    linkedList *next;
    while (temp != NULL)
    {
        next = temp->next;
        free(temp);
        temp = next;
    }
}

void swapListEntry(linkedList *n0, linkedList *n1)
{
    void *temp = n0->obj;
    int tType = n0->type;
    n0->obj = n1->obj;
    n0->type = n1->type;
    n1->obj = temp;
    n1->type = tType;
}

linkedList *partitionList(linkedList *low, linkedList *high, listEntryCmp cmp)
{
    linkedList *pivot = high;

    linkedList *i = low->pre;

    linkedList *j = low;

    while (j != high)
    {
        if (cmp(((entry *)(j->obj))->arr, ((entry *)pivot->obj)->arr) < 0)
        {
            i = i->next;

            swapListEntry(i, j);
        }
        j = j->next;
    }

    i = i->next;
    swapListEntry(i, high);
    return i;
}

void listQuickSort(linkedList *low, linkedList *high, listEntryCmp cmp)
{
    if (low != NULL && (listGetIndex(low) < listGetIndex(high)))
    {
        linkedList *p = partitionList(low, high, cmp);
        listQuickSort(low, p->pre, cmp);
        listQuickSort(p->next, high, cmp);
    }
}

/**
 * sorts list
 */
void sortList(linkedList *root, listEntryCmp cmp)
{
    listQuickSort(root->next, root->end, cmp);
}

/**
 * prints linkedListList
 */
void *printList(linkedList *root, printObj p)
{
    printCall++;
    if (printCall == 2)
    {
        printf("  ");
    }
    linkedList *curr = root->next;
    printf("[");
    while (curr != NULL)
    {
        p(curr->obj);
        curr = curr->next;
    }
    printf("]");
    if (printCall == 2)
    {
        printf("\n");
    }
    printCall--;
}