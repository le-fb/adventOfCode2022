#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int readFile(char *filename, char ***content, int *lines)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0;
    }

    // get file size
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    // make space
    char *temp = malloc((size + 1) * sizeof(char));
    // read
    int i = 0;
    int tempLines = 0;
    char ch;
    while (!feof(file))
    {
        ch = fgetc(file);
        if ((int)ch > 0 && (int)ch < 128)
        {
            if (ch == '\n')
            {
                tempLines++;
            }
            temp[i] = ch;
            i++;
        }
    }
    tempLines++;
    temp[i] = 0;

    *lines = tempLines;

    // build lines
    int lastIndex = 0;
    int lineIndex = 0;
    (*content) = malloc(tempLines * sizeof(void *));
    char *tchar = malloc((size + 1) * sizeof(char));
    for (int i = 0; i < strlen(temp); i++)
    {
        if (temp[i] == '\n')
        {
            tchar[i - lastIndex] = 0;                                           // set end char
            (*content)[lineIndex] = malloc((i - lastIndex + 1) * sizeof(char)); // space for string
            strcpy((*content)[lineIndex], tchar);                               // copy string to space
            lastIndex = i + 1;
            lineIndex++;
        }
        else
        {
            tchar[i - lastIndex] = temp[i];
        }
    }
    // for last
    tchar[i - lastIndex] = 0;
    (*content)[lineIndex] = malloc((i - lastIndex + 1) * sizeof(char));
    strcpy((*content)[lineIndex], tchar);

    fclose(file);
    return size;
}

int cToI(char c)
{
    int temp = (int)c;
    if (temp > 47 && temp < 58)
    {
        return temp - '0';
    }
    return -1;
}

linkedNode *createLinkedNode(void *obj)
{
    linkedNode *t = malloc(sizeof(linkedNode));
    t->end = t;
    t->next = NULL;
    t->pre = NULL;
    t->obj = obj;
    return t;
}

int getNodeSize(linkedNode *root)
{
    linkedNode *curr = root;
    int size = 0;
    while (curr != NULL)
    {
        curr = curr->next;
        size++;
    }
    return size;
}

linkedNode *removeNode(linkedNode *root, linkedNode *node)
{
    assert(node != NULL);

    if (root == NULL || (node == root && node->pre == NULL && node->next == NULL))
    {
        // list empty after move;
        return NULL;
    }

    if (node == root)
    {
        // remove root element
        root->next->end = root->end;
        root->next->pre = NULL;
        return root->next;
    }

    if (node == root->end)
    {
        // removing end of list
        node->pre->next = NULL;
        root->end = node->pre;
        return root;
    }

    node->next->pre = node->pre;
    node->pre->next = node->next;
    return root;
}

linkedNode *pushBackNode(linkedNode *root, linkedNode *node)
{
    assert(node != NULL);
    if (root == NULL)
    {
        node->next = NULL;
        node->pre = NULL;
        node->end = node;
        return node;
    }
    root->end->next = node;
    node->pre = root->end;
    node->next = NULL;
    root->end = node;
    return root;
}

void swapNodes(linkedNode *n0, linkedNode *n1)
{
    assert(n0 != NULL && n1 != NULL);
    void *temp = n0->obj;
    n0->obj = n1->obj;
    n1->obj = temp;
}

linkedNode *partition(linkedNode *low, linkedNode *high, nodeCmp cmp)
{

    linkedNode *pivot = high;

    linkedNode *i = low->pre;

    linkedNode *j = low;

    while (j != high)
    {
        if (cmp(j->obj, pivot->obj) <= 0)
        {
            if (i == NULL)
            {
                i = low;
            }
            else
            {
                i = i->next;
            }
            swapNodes(i, j);
        }
        j = j->next;
    }

    if (i == NULL)
    {
        i = low;
    }
    else
    {
        i = i->next;
    }

    swapNodes(i, high);
    return i;
}

void quickSort(linkedNode *low, linkedNode *high, nodeCmp cmp)
{
    if (cmp(low->obj, high->obj) < 0)
    {
        linkedNode *p = partition(low, high, cmp);
        quickSort(low, p->pre, cmp);
        quickSort(p, high, cmp);
    }
}

linkedNode *sortNodes(linkedNode *root, nodeCmp cmp)
{
    quickSort(root, root->end, cmp);
}

void *printNodes(linkedNode *root, printObj p)
{
    linkedNode *curr = root;
    printf("[");
    while (curr != NULL)
    {
        p(curr->obj);
        curr = curr->next;
    }
    printf("]\n");
}