import re
from tqdm import tqdm
with open("tim/15/input.txt", "r") as f:
    file = f.read().splitlines()

def manhattan_distance(pos1,pos2):
    x1,y1 = pos1
    x2,y2 = pos2
    return abs(x1-x2) + abs(y1-y2)

def print_matrix(matrix):
    for i in matrix:
        for j in i:
            print(j, end=" ")
        print()

def within_range(coords, coord_range):
    x,y = coords
    upper, lower, left, right = coord_range
    return left <= x <= right and upper <= y <= lower


data: list[tuple[tuple[int,int], tuple[int,int]]] = [] # xD
max_x = 0
max_y = 0
min_x = float("inf")
min_y = float("inf")
for line in file:
    x1,y1,x2,y2 = re.findall("-?[0-9]+", line)
    x1,y1,x2,y2 = int(x1),int(y1),int(x2),int(y2)
    data.append(((x1,y1),(x2,y2)))
    max_x = max(x1,x2,max_x)
    max_y = max(y1,y2,max_y)
    min_x = min(x1,x2,min_x)
    min_y = min(y1,y2,min_y)

#print(max_x, max_y)
#print(min_x, min_y)

covered = []
for sensor, beacon in data:
    distance = manhattan_distance(sensor, beacon)
    x,y = sensor
    upper = y-distance
    lower = y+distance
    left = x-distance
    right = x+distance
    covered.append((upper, lower, left, right))

#print(covered)
def fully_contain(sec1, sec2):
    if sec1[0] <= sec2[0] and sec1[1] >= sec2[1]:
        return True
    return False

def filter_overlap(cover):
    i = 0
    while i < len(cover)-1:
        if fully_contain(cover[i], cover[i+1]):
            cover.pop(i+1)
            i-=1
        i+=1
    return cover

def chain_overlap(cover):
    latest = None
    for i in cover:
        if latest is not None:
            if latest[1]+1 < i[0]:
                return False, latest[1]+1
        latest = i
    return True, 0


for y in tqdm(range(4000000)):
    cover = []
    for c in covered:
        if c[0] <= y <= c[1]:
            y_offset = min(abs(c[0]-y), c[1]-y)
            distance = abs(c[0]-c[1])/2
            x1 = c[2] + (distance-y_offset)
            covering = 2*y_offset
            x2 =  x1+covering
            cover.append((int(x1),int(x2)))
    cover.sort()
    #print(cover)
    cover = filter_overlap(cover)
    #print(cover)
    chain = chain_overlap(cover)
    if not chain[0]:
        print(f"({chain[1]}|{y}) - {chain[1]*4000000+y}")
        break
