#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <utils.h>
#include <linkedList.h>

#define INPUT_FILE_NAME "../../kayms/13/input"

void printType(void *obj);
void printType(void *obj)
{
    if (((entry *)obj)->type == NUMBER)
    {
        printf("%i,", ((entry *)obj)->number);
    }
    else if (((entry *)obj)->type == ARRAY)
    {
        // printf(",");
        printList(((entry *)obj)->arr, printType);
    }
}

int compare(linkedList *l, linkedList *r)
{

    linkedList *left = l;
    linkedList *right = r;

    if (left->isRoot)
    {
        left = left->next;
    }
    if (right->isRoot)
    {
        right = right->next;
    }

    while (left != NULL && right != NULL)
    {

        /*printf("left: ");
        printList(l, printType);
        printf("\nright: ");
        printList(r, printType);
        printf("\n\n");*/

        entry *lEntry = ((entry *)left->obj);
        entry *rEntry = ((entry *)right->obj);

        if (lEntry->type == ARRAY && rEntry->type == ARRAY)
        {
            // printf("array vs array\n");
            int ret = compare(lEntry->arr, rEntry->arr);
            if (ret != 0)
            {
                return ret;
            }
            left = left->next;
            right = right->next;
            continue;
        }
        if (lEntry->type == ARRAY)
        {
            linkedList *temp = createList();
            listPushBack(temp, rEntry);
            int ret = compare(lEntry->arr, temp);
            deleteList(temp);
            return ret;
        }
        if (rEntry->type == ARRAY)
        {
            linkedList *temp = createList();
            listPushBack(temp, lEntry);
            int ret = compare(temp, rEntry->arr);
            deleteList(temp);
            return ret;
        }
        int r = (rEntry->number - lEntry->number);
        if (r != 0)
        {
            return r;
        }

        left = left->next;
        right = right->next;
    }

    linkedList *tempL = l;
    linkedList *tempR = r;

    assert(tempL->isRoot == 1 && tempR->isRoot == 1);
    if (tempL->size == tempR->size && tempL->size == 0)
    {
        return 0;
    }
    return (listSize(tempR) - listSize(tempL));
}

int sortCompare(void *obj0, void *obj1)
{
    int r = compare(obj0, obj1);
    r = r * -1;
    return r;
}

int main()
{
    char **lines;
    int lCount = 0;

    int r = readFile(INPUT_FILE_NAME, &lines, &lCount);

    linkedList *allPairs = createList();
    linkedList *currPair = createList();

    linkedList *prev = createList();
    linkedList *curr = createList();

    for (int i = 0; i < lCount; i++)
    {
        if (lines[i][0] == 0)
        {
            entry *e = malloc(sizeof(entry));
            e->type = ARRAY;
            e->arr = currPair;
            e->flag = 0;
            listPushBack(allPairs, e);
            currPair = createList();
        }
        else
        {
            int digit = -1;
            for (int j = 1; j < strlen(lines[i]) - 1; j++)
            {
                char c = lines[i][j];

                if (isdigit(c))
                {
                    if (digit < 0)
                    {
                        digit = 0;
                    }
                    digit = (digit * 10) + cToI(c);
                }

                switch (c)
                {
                case '[':
                {
                    linkedList *temp = createList();

                    entry *e = malloc(sizeof(entry));
                    e->type = ARRAY;
                    e->arr = temp;
                    e->flag = 0;

                    listPushBack(curr, e);
                    listPushBack(prev, curr);

                    curr = temp;
                }
                break;
                case ']':
                {

                    if (digit >= 0)
                    {
                        entry *e = malloc(sizeof(entry));
                        e->type = NUMBER;
                        e->number = digit;
                        e->flag = 0;
                        listPushBack(curr, e);
                        digit = -1;
                    }

                    curr = (linkedList *)listPop(prev);
                    assert(curr != NULL);
                }
                break;

                case ',':
                {
                    if (digit >= 0)
                    {
                        entry *e = malloc(sizeof(entry));
                        e->type = NUMBER;
                        e->number = digit;
                        e->flag = 0;
                        listPushBack(curr, e);
                        digit = -1;
                    }
                }
                break;

                default:
                    break;
                }
            }
            if (digit >= 0)
            {
                entry *e = malloc(sizeof(entry));
                e->type = NUMBER;
                e->number = digit;
                e->flag = 0;
                listPushBack(curr, e);
                digit = -1;
            }
            assert(listSize(prev) == 0);
            entry *e = malloc(sizeof(entry));
            e->type = ARRAY;
            e->arr = curr;
            e->flag = 0;
            listPushBack(currPair, e);
            curr = createList();
        }
    }

    int rightOrderCounter = 0;
    int index = 1;
    linkedList *c = allPairs->next;

    while (c != NULL)
    {
        linkedList *pair = ((entry *)c->obj)->arr;
        linkedList *left = createList();
        linkedList *right = createList();

        listPushBack(left, pair->next->obj);
        listPushBack(right, pair->next->next->obj);

        int r = compare(left, right);
        // printf("r : %i\n", r);
        if (r > 0)
        {
            rightOrderCounter += index;
        }
        index++;
        c = c->next;
    }
    // printf("count of pairs %i\n", listSize(allPairs));
    printf("right order count %i\n", rightOrderCounter);

    linkedList *packets = createList();

    // build temp packet
    entry *a = malloc(sizeof(entry));
    a->type = NUMBER;
    a->number = 6;
    a->flag = 2;
    entry *b = malloc(sizeof(entry));
    b->type = NUMBER;
    b->number = 2;
    b->flag = 1;

    linkedList *al = createList();
    listPushBack(al, a);

    linkedList *bl = createList();
    listPushBack(bl, b);

    entry *aList = malloc(sizeof(entry));
    aList->type = ARRAY;
    aList->arr = al;
    aList->flag = 2;

    entry *bList = malloc(sizeof(entry));
    bList->type = ARRAY;
    bList->arr = bl;
    bList->flag = 1;

    linkedList *t = allPairs->next;
    while (t != NULL)
    {
        linkedList *p = ((entry *)t->obj)->arr;
        listPushBack(packets, p->next->obj);
        listPushBack(packets, p->next->next->obj);
        t = t->next;
    }

    listPushBack(packets, aList);
    listPushBack(packets, bList);

    sortList(packets, sortCompare);

    // printList(packets, printType);
    // printf("\n");

    t = packets->next;
    int divider = 1;
    index = 1;
    while (t != NULL)
    {
        if (((entry *)t->obj)->flag > 0)
        {
            printf("%i %i\n", index, ((entry *)t->obj)->flag);
            divider *= index;
        }
        index++;
        t = t->next;
    }

    printf("part 2 %i\n", divider);

    return 0;
}
