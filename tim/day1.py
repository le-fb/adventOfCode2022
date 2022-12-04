with open("input1.txt", "r") as f:
    data = f.read().splitlines()
current = 0
highest = 0
for i in data:
    if i == "":
        if current > highest:
            highest = current
        current = 0
    else:
        current += int(i)
print("part1:", highest)

counts = []
current = 0
for i in data:
    if i == "":
        counts.append(current)
        current = 0
    else:
        current += int(i)
result = sum(sorted(counts, reverse=True)[:3])
print("part2:", result)
