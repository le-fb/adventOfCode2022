numStacks = 9

MoveCratesAtOnce = True

stacks = [[] for dummy in range(numStacks)]

endIndexStacks = 8
startIndexMoves = 10

with open("input5.txt") as rawData:
    input = rawData.readlines()

for i in range(endIndexStacks).__reversed__():
    stackInput = input[i]
    for j in range(len(stacks)):
        charIndex = 1 + 4*j
        if stackInput[charIndex] != " ":
            stacks[j].append(stackInput[charIndex])

for move in input[startIndexMoves:]:
    if(move == ""):
        continue
    # 'move 3 from 2 to 1'
    moveParts = move.split(" ")
    numToMove = int(moveParts[1])
    fromStack = int(moveParts[3]) - 1
    toStack   = int(moveParts[5]) - 1

    if MoveCratesAtOnce:
        toMove = stacks[fromStack][-numToMove:]
        for crate in toMove:
            stacks[toStack].append(crate)
            stacks[fromStack].pop()
    else:
        for i in range(numToMove):
            crate = stacks[fromStack].pop()
            stacks[toStack].append(crate)
    
print("Final stacks:")
maxStackHeight = max(len(stack) for stack in stacks)
for i in range(maxStackHeight).__reversed__():
    output = ""
    for j in range(numStacks):
        if i < len(stacks[j]):
            output += f"[{stacks[j][i]}]"
        else:
            output += "   "
        output += " "
    print(output)

print(f"Solution part {'2' if MoveCratesAtOnce else '1'}: ")
solution1 = ""
for i in range(numStacks):
    solution1 += stacks[i].pop()
print(solution1)