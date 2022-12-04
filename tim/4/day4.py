with open("input4.txt", "r") as f:
    file = f.read().splitlines()

def parseSection_(line: str) -> tuple[tuple[int, int], tuple[int, int]]:
    sec1, sec2 = line.split(",")
    sec1 = tuple(int(i) for i in sec1.split("-"))
    sec2 = tuple(int(i) for i in sec2.split("-"))
    return sec1, sec2

def parseSection(line: str) -> tuple[tuple[int, int], tuple[int, int]]:
    sec1, sec2 = map(lambda x: tuple(int(i) for i in x.split("-")), line.split(","))
    return sec1, sec2

data = [parseSection(elv) for elv in file]

fully_contains = 0
for sec1, sec2 in data:
    if sec1[0] <= sec2[0] and sec1[1] >= sec2[1]:
        fully_contains += 1
    elif sec2[0] <= sec1[0] and sec2[1] >= sec1[1]:
        fully_contains += 1

print(fully_contains)

overlap = 0
for sec1, sec2 in data:
    if sec1[0] <= sec2[0] <= sec1[1]:
        overlap += 1
    elif sec2[0] <= sec1[0] <= sec2[1]:
        overlap += 1

print(overlap)
