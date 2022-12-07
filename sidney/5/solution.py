
import math

file1 = open('./sidney/5/data.txt', 'r')
lines = file1.read().splitlines()
file1.close()

# Find the indexing line
i = 0
for line in lines:
    if not line.startswith('['):
        break
    i += 1

stacks = list()

to_allocate = lines[0:i]
to_allocate.reverse()

# Go over the indexing line to find the columns with the crate data
# and push it to the stacks
k = 0
j = 0
for c in lines[i]:
    # If column has no index continue
    if c == ' ':
        j += 1
        continue

    stacks.append(list())
    for l in to_allocate:
        if j >= len(l) or l[j] == ' ':
            break
        stacks[k].append(l[j])
    k += 1
    j += 1

while not lines[i].startswith('move'):
    i += 1

def move_crates(individual: bool):

    for command in lines[i:]:
        split = command.split(' ')
        quantity = int(split[1])
        source = int(split[3])
        target = int(split[5])

        if individual:
            for n in range(0, quantity):
                stacks[target-1].append(stacks[source-1].pop())
        else:
            for n in range(quantity, 0, -1):
                stacks[target-1].append(stacks[source-1][-n])
            for n in range(0, quantity):
                stacks[source-1].pop()

move_crates(False)
print(f'Items on top {str().join([s[-1] for s in stacks])}')
