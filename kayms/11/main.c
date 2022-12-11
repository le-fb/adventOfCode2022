#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <utils.h>

typedef struct item item;
typedef struct monkey monkey;
typedef void (*Operation)(monkey *m, item *i);

struct item
{
    u_int64_t worryLevel;
    item *next;
    item *pre;
    int newItem;
};

struct monkey
{
    int number;
    int sumInspectedItems;
    item *root;
    Operation op;
    int test[3];
};

int monkeyTest(monkey *m, item *i)
{
    if ((i->worryLevel % m->test[0]) == 0)
    {
        return m->test[1];
    }
    return m->test[2];
}

void operation(monkey *m, item *i)
{
    /*
        switch (m->number)
        {
        case 0:
            i->worryLevel *= 19;
            break;
        case 1:
            i->worryLevel += 6;
            break;
        case 2:
            i->worryLevel = (i->worryLevel * i->worryLevel);
            break;
        case 3:
            i->worryLevel += 3;
            break;

        default:
            printf("err");
            exit(EXIT_FAILURE);
            break;
        }*/

    switch (m->number)
    {
    case 0:
        i->worryLevel *= 5;
        break;
    case 1:
        i->worryLevel = i->worryLevel * i->worryLevel;
        break;
    case 2:
        i->worryLevel += 1;
        break;
    case 3:
        i->worryLevel += 6;
        break;
    case 4:
        i->worryLevel *= 17;
        break;
    case 5:
        i->worryLevel += 8;
        break;
    case 6:
        i->worryLevel += 7;
        break;
    case 7:
        i->worryLevel += 5;
        break;
    default:
        printf("err");
        exit(EXIT_FAILURE);
        break;
    }

    i->worryLevel = i->worryLevel % (2 * 7 * 13 * 3 * 19 * 5 * 11 * 17);
}

monkey *createMonkey(int number, int *startItems, int startItemCount, Operation op, int test[3])
{
    monkey *m = malloc(sizeof(monkey));

    m->number = number;
    m->sumInspectedItems = 0;
    m->root = malloc(sizeof(item));

    item *pre = NULL;
    item *curr = m->root;
    for (int i = 0; i < startItemCount; i++)
    {
        curr->worryLevel = startItems[i];
        curr->pre = pre;
        curr->newItem = 0;
        curr->next = malloc(sizeof(item));

        pre = curr;
        curr = curr->next;
    }
    pre->next = NULL;

    m->op = op;
    for (int i = 0; i < 3; i++)
    {
        m->test[i] = test[i];
    }

    return m;
}

int main()
{
    /*
        int mCount = 4;

        int count[8] = {2, 4, 3, 1};

        int it[8][8] =
            {
                {79, 98},
                {54, 65, 75, 74},
                {79, 60, 97},
                {74},
            };

        int test[8][3] =
            {
                {23, 2, 3},
                {19, 2, 0},
                {13, 1, 3},
                {17, 0, 1}};*/

    int mCount = 8;

    int count[8] = {6, 1, 8, 4, 2, 7, 5, 3};

    int it[8][8] =
        {
            {50, 70, 89, 75, 66, 66},
            {85},
            {66, 51, 71, 76, 58, 55, 58, 60},
            {79, 52, 55, 51},
            {69, 92},
            {71, 76, 73, 98, 67, 79, 99},
            {82, 76, 69, 69, 57},
            {65, 79, 86}};

    int test[8][3] =
        {
            {2, 2, 1},
            {7, 3, 6},
            {13, 1, 3},
            {3, 6, 4},
            {19, 7, 5},
            {5, 0, 2},
            {11, 7, 4},
            {17, 5, 0}};

    // create zoo
    monkey **zoo = malloc(mCount * sizeof(void *));

    for (int i = 0; i < mCount; i++)
    {
        zoo[i] = createMonkey(i, it[i], count[i], operation, test[i]);
    }
    /*
    printf("start \n");
    for (int j = 0; j < mCount; j++)
    {
        printf("monkey %i : ", zoo[j]->number);
        item *t = zoo[j]->root;
        while (t != NULL)
        {
            printf("%i, ", t->worryLevel);
            t = t->next;
        }
        printf("\n");
    }
    printf("\n");*/

    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < mCount; j++)
        {
            item *curr = zoo[j]->root;
            item *next = NULL;
            while (curr != NULL)
            {
                next = curr->next;

                zoo[j]->op(zoo[j], curr);
                zoo[j]->sumInspectedItems++;
                int target = monkeyTest(zoo[j], curr);

                // throw item

                // end end of list temp -> end of list
                item *temp = zoo[target]->root;
                while (temp != NULL && temp->next != NULL)
                {
                    temp = temp->next;
                }

                // alte liste neu verlinken
                //  neu verlinken
                if (curr->pre != NULL)
                {
                    curr->pre->next = curr->next;
                }
                if (curr->next != NULL)
                {
                    curr->next->pre = curr->pre;
                }
                // wenn die liste der source leer wird
                if (curr->pre == NULL && curr->pre == NULL)
                {
                    zoo[j]->root = NULL;
                }

                // beim neuen einfügen

                if (temp != NULL)
                {
                    // listen elemente vorhanden
                    temp->next = curr;
                    curr->pre = temp;
                }
                else
                {
                    // kein element in der liste
                    zoo[target]->root = curr;
                    curr->pre = NULL;
                }
                curr->next = NULL;
                curr->newItem = 1;

                curr = next;
            }

            curr = zoo[j]->root;

            while (curr != NULL)
            {
                curr->newItem = 0;
                curr = curr->next;
            }
        }

        printf("round %i\n", i);
        for (int j = 0; j < mCount; j++)
        {
            printf("monkey inspect sum %i \n", zoo[j]->sumInspectedItems);
        }
        printf("\n");
    }

    return 0;
}
