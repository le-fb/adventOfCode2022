with open("tim/14/sample.txt", "r") as f:
    file = f.read().splitlines()

def print_matrix(matrix):
    for i in matrix:
        for j in i:
            print(j, end=" ")
        print()
def calc_path(start:tuple[int, int], end:tuple[int, int]):
    sx,sy = start
    ex,ey = end
    path = []
    for x in range(min(sx,ex), max(sx,ex)+1):
        for y in range(min(sy,ey), max(sy,ey)+1):
            path.append((x,y))
    return path
def parse_matrix():
    data = []
    max_x, max_y = 0,0
    min_x, min_y = float("inf"), float("inf")
    for line in file:
        path = []
        for xy in line.split(" -> "):
            x,y = xy.split(",")
            x,y = int(x), int(y)
            max_x = x if x>max_x else max_x
            max_y = y if y>max_y else max_y
            min_x = x if x<min_x else min_x
            min_y = y if y<min_y else min_y
            path.append((x,y))
        data.append(path)

    print(max_x, max_y)
    print(min_x, min_y)
    matrix = [["." for _ in range(min_x, max_x+1)] for _ in range(0, max_y+1)]

    matrix[0][500-min_x] = "+" # sand point
    #print_matrix(matrix)
    for path in data:
        latest = None
        for coors in path:
            if latest is not None:
                rock_pos = calc_path(latest, coors)
                for pos in rock_pos:
                    x,y = pos
                    matrix[y][x-min_x] = "#"
            latest = coors
    #print_matrix(matrix)
    return matrix, min_x, max_x, min_y, max_y

def simulate_sand(x,y, show=False):
    if show:
        print_matrix(matrix)
    if y+1 >= len(matrix):
        return False
    if matrix[y+1][x-min_x] == ".":
        matrix[y][x-min_x] = "."
        matrix[y+1][x-min_x] = "o"
        return simulate_sand(x, y+1)
    elif matrix[y+1][x-min_x-1] == ".":
        if x-min_x-1 < 0:
            return False
        matrix[y][x-min_x] = "."
        matrix[y+1][x-min_x-1] = "o"
        return simulate_sand(x-1, y+1)
    elif matrix[y+1][x-min_x+1] == ".":
        if x+1-min_x >= len(matrix[0]):
            return False
        matrix[y][x-min_x] = "."
        matrix[y+1][x-min_x+1] = "o"
        return simulate_sand(x+1, y+1)
    else:
        return True

matrix, min_x, max_x, min_y, max_y = parse_matrix()
sand = True
c = 0
while sand:
    sand = simulate_sand(500, 0)
    c+=1
print(c-1)
