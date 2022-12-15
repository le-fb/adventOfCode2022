import re
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

desired_y = 2000000
blocked = []
for sensor, beacon in data:
    distance = manhattan_distance(sensor, beacon)
    if sensor[1]-distance <= desired_y <= sensor[1]+distance:
        #print(sensor,distance, (sensor[0]+(distance-abs(desired_y-sensor[1])), sensor[0]-(distance-abs(desired_y-sensor[1]))))
        blocked.append((sensor[0]+(distance-abs(desired_y-sensor[1])), sensor[0]-(distance-abs(desired_y-sensor[1]))))

x = []
for i in blocked:
    x.append(i[0])
    x.append(i[1])
l = ["." for _ in range(max(x))]
l = l+l
for x1,x2 in blocked:
    for i in range(min(x1,x2), max(x1,x2)+1):
        l[i] = "#"
for sensor, beacon in data:
    x,y = beacon
    if y == desired_y:
        l[x] = "B"

print(l.count("#"))
