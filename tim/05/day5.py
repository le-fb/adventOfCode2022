with open("input.txt", "r") as f:
    file = f.read().splitlines()

def parseCommand(line: str) -> tuple[int, int, int]:
    s = line.split()
    return int(s[1]), int(s[3]), int(s[5])

# parse input
def parseInput(file):
    stacks = {}
    commands = []
    for line in file:
        if "[" not in line:
            # parse commands
            if "move" in line:
                commands.append(parseCommand(line))
            continue
        # parse stacks
        for ind, i in enumerate(range(0, len(line), 4), start=1):
            crate = line[i:i+3]
            if crate == "   ":
                #print(crate.__repr__())
                continue
            if ind in stacks:
                stacks[ind].append(crate)
            else:
                stacks[ind] = [crate]
    return stacks, commands

def parseSolution(stacks):
    solution = ""
    for ind, i in enumerate(stacks, start=1):
        solution += stacks[ind][0][1]
    return solution

stacks, commands = parseInput(file)
#print(stacks)
#print(commands)
# execute commands ; part1
for count, from_, to in commands:
    for _ in range(count):
        crate = stacks[from_][0]
        del stacks[from_][0]
        stacks[to].insert(0, crate)


print(f"The solution for part 1 is {parseSolution(stacks)}")

#execute commands ; part2
stacks, commands = parseInput(file)
for count, from_, to in commands:
    crates = stacks[from_][:count]
    del stacks[from_][:count]
    stacks[to] = crates + stacks[to]


print(f"The solution for part 2 is {parseSolution(stacks)}")



