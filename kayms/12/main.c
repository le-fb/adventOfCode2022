#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <utils.h>

typedef struct temp temp;

struct temp
{
    int i;
};

int cmpT(void *t0, void *t1)
{
    return ((temp *)t0)->i - ((temp *)t1)->i;
}

void printTemp(void *t)
{
    printf("%i ", ((temp *)t)->i);
}

int main()
{
    temp *t = malloc(sizeof(temp));
    t->i = 0;
    linkedNode *test = createLinkedNode(t);

    int b[10] = {5, 2, 1, 7, 3, 2, 65, 76, 7, 5};

    for (int i = 1; i < 10; i++)
    {
        temp *a = malloc(sizeof(temp));
        a->i = b[i];
        pushBackNode(test, createLinkedNode(a));
    }

    printNodes(test, printTemp);

    sortNodes(test, cmpT);

    printNodes(test, printTemp);

    return 0;
}
