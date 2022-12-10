import re

file1 = open('./sidney/1/data.txt', 'r')
lines = file1.readlines()

curSum = 0
max = [0, 0, 0]

for line in lines:

    # Check if the current is a number
    match = re.search(r'[0-9]+', line)

    # If it's not a number check if the current elf
    # has more calories than any of previous elfs that
    # had a lot of calories
    if(match == None):
        for i in range(0, 3):
            if curSum > max[i]:
                max[i] = curSum
                break
        curSum = 0
        continue

    # If it's a number add it to the sum
    curSum = curSum + int(match.group())

print(f'The max is {max[0] + max[1] + max[2]}')    
