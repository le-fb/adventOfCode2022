
import math


file1 = open('./sidney/3/data.txt', 'r')
lines = file1.read().splitlines()
file1.close()

def get_priority(char: str):
    priority = ord(char.upper()) - 64 # The priority a lower letter would have
    if char.isupper():
        priority = priority + 26
    return priority

priority_sum = 0
for line in lines:
    
    half = math.floor(len(line)/2)
    pouch_1 = set(line[0:half])
    pouch_2 = set(line[half:])

    intersection = pouch_1.intersection(pouch_2)

    for item in intersection:
        priority_sum = priority_sum + get_priority(item)

print(f'Sum of all priorities: {priority_sum}')

groups = math.floor(len(lines)/3)
group_priority_sum = 0
all_badges = []

for i in range(0, groups):
    elf_1 = set(lines[(i*3)+0])
    elf_2 = set(lines[(i*3)+1])
    elf_3 = set(lines[(i*3)+2])

    badges = elf_1.intersection(elf_2).intersection(elf_3)

    for badge in badges:
        group_priority_sum += get_priority(badge)
        all_badges.append(badge)


print(f'Sum of all group priorities: {group_priority_sum}')

