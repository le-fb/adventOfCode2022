with open("input3.txt", "r") as f:
    file = f.read().splitlines()

prio_sum = 0
for rucksack in file:
    c1 = rucksack[:int(len(rucksack)/2)]
    c2 = rucksack[int(len(rucksack)/2):]
    for c in c1:
        if c in c2:
            if c.islower():
                prio_sum += ord(c)-96
            else:
                prio_sum += ord(c)-64+26
            break

print(prio_sum)

prio_sum = 0
for i in range(0, len(file), 3):
    group = file[i:i+3]
    for c in group[0]:
        if c in group[1] and c in group[2]:
            if c.islower():
                prio_sum += ord(c)-96
            else:
                prio_sum += ord(c)-64+26
            break

print(prio_sum)


