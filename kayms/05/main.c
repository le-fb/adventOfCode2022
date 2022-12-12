#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/05/input"

typedef struct crate
{
    int notEmpty;
    char identifier;
} crate;

typedef struct ship
{
    int cargoSlots;
    int cargoLevels;
    int *slotLevelIndex;
    crate **crates;
} ship;

ship *loadShip(char **levelInput, int levelCount, int currentIndex)
{
    assert(levelCount > 0);
    int cargoSlotCount = ((strlen(levelInput[currentIndex]) - 1) >> 2) + 1;

    // create ship
    ship *elfShip = malloc(sizeof(ship));
    elfShip->cargoSlots = cargoSlotCount;
    elfShip->cargoLevels = levelCount;
    elfShip->crates = malloc(cargoSlotCount * sizeof(void *));
    elfShip->slotLevelIndex = malloc(cargoSlotCount * sizeof(int));
    for (int i = 0; i < cargoSlotCount; i++)
    {
        elfShip->slotLevelIndex[i] = 0;
        elfShip->crates[i] = malloc(levelCount * cargoSlotCount * sizeof(crate)); // max count in one slot level count * slot count
    }
    // load ship
    for (int i = 0; i < levelCount; i++)
    {
        int k = 1;
        for (int j = 0; j < cargoSlotCount; j++)
        {
            if (levelInput[i + currentIndex][k] > 64 && levelInput[i + currentIndex][k] < 91)
            {
                // load it into ship
                elfShip->crates[j][elfShip->slotLevelIndex[j]].identifier = levelInput[i + currentIndex][k];
                elfShip->crates[j][elfShip->slotLevelIndex[j]].notEmpty = 1;
                elfShip->slotLevelIndex[j]++;
            }
            else
            {
                elfShip->crates[j][i].notEmpty = 0;
            }
            k += 4;
        }
    }
    return elfShip;
}

void printShip(ship *elfShip)
{
    int maxheight = 0;
    for (int i = 0; i < elfShip->cargoSlots; i++)
    {
        if (maxheight < elfShip->slotLevelIndex[i])
        {
            maxheight = elfShip->slotLevelIndex[i];
        }
    }

    for (int i = maxheight - 1; i >= 0; i--)
    {
        for (int j = 0; j < elfShip->cargoSlots; j++)
        {
            if (elfShip->crates[j][i].notEmpty == 0)
            {
                printf("    ");
            }
            else
            {
                printf("[%c] ", elfShip->crates[j][i].identifier);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void moveCrate(ship *elfShip, int fromSlot, int toSlot)
{
    elfShip->slotLevelIndex[fromSlot]--;
    assert(elfShip->slotLevelIndex[fromSlot] >= 0);

    elfShip->crates[toSlot][elfShip->slotLevelIndex[toSlot]].identifier = elfShip->crates[fromSlot][elfShip->slotLevelIndex[fromSlot]].identifier;
    elfShip->crates[toSlot][elfShip->slotLevelIndex[toSlot]].notEmpty = 1;
    elfShip->crates[fromSlot][elfShip->slotLevelIndex[fromSlot]].notEmpty = 0;

    elfShip->slotLevelIndex[toSlot]++;
}

void moveCrates(ship *elfShip, int crateCount, int fromSlot, int toSlot)
{
    int tempIndex = elfShip->slotLevelIndex[fromSlot] - crateCount;
    elfShip->slotLevelIndex[fromSlot] -= crateCount;

    assert(elfShip->slotLevelIndex[fromSlot] >= 0);

    for (int i = 0; i < crateCount; i++)
    {
        elfShip->crates[toSlot][elfShip->slotLevelIndex[toSlot]].identifier = elfShip->crates[fromSlot][tempIndex].identifier;
        elfShip->crates[toSlot][elfShip->slotLevelIndex[toSlot]].notEmpty = 1;
        elfShip->crates[fromSlot][tempIndex].notEmpty = 0;
        tempIndex++;
        elfShip->slotLevelIndex[toSlot]++;
    }
}

int main()
{
    char **content;
    int lines = 0;

    readFile(INPUT_FILE_NAME, &content, &lines);

    char **shipLevels = malloc(lines * sizeof(void *));
    int shipLevelIndex = lines;

    int foundEndOfShipInput = 0;
    int startOfInstructions = 0;

    for (int i = 0; i < lines; i++)
    {
        if (strlen(content[i]) == 0)
        {
            foundEndOfShipInput = 1;
            startOfInstructions = i + 1;
        }
        if (!foundEndOfShipInput)
        {
            shipLevels[shipLevelIndex] = malloc((strlen(content[i]) + 1) * sizeof(char));

            for (int j = 0; j < (strlen(content[i]) + 1); j++)
            {
                shipLevels[shipLevelIndex][j] = content[i][j];
            }

            shipLevelIndex--;
            assert(shipLevelIndex > 0);
        }
    }
    shipLevelIndex += 2;

    ship *elfShip = loadShip(shipLevels, (lines - shipLevelIndex + 1), shipLevelIndex);
    ship *elfShip9001 = loadShip(shipLevels, (lines - shipLevelIndex + 1), shipLevelIndex);

    printf("Start condition:\n");
    printShip(elfShip);
    for (int i = startOfInstructions; i < lines; i++)
    {
        // get instruction
        int values[3] = {0};
        int tempIndex = 0;
        int lastCharIsDigit = 0;

        for (int j = 0; j < strlen(content[i]); j++)
        {
            if (isdigit(content[i][j]))
            {
                values[tempIndex] = (values[tempIndex] * 10) + (content[i][j] - '0');
                lastCharIsDigit = 1;
            }
            else
            {
                if (lastCharIsDigit == 1)
                {
                    tempIndex++;
                }
                lastCharIsDigit = 0;
            }
        }
        assert(tempIndex == 2);

        // do instruction CrateMover9000

        for (int j = 0; j < values[0]; j++)
        {
            moveCrate(elfShip, values[1] - 1, values[2] - 1);
        }

        // do instruction CrateMover9001
        moveCrates(elfShip9001, values[0], values[1] - 1, values[2] - 1);
        // printf("%i %s\n", i, content[i]);
        // printShip(elfShip9001);
    }
    printf("after move crates with CrateMover 9000\n");
    printShip(elfShip);

    printf("after move crates with CrateMover 9001\n");
    printShip(elfShip9001);
}