with open("tim/17/input.txt", "r") as f:
    file = f.read()
#print(file)
file = file.replace("\n", "")
#matrix = [["." for _ in range(7)] for _ in range(len(file))]
matrix = [["." for _ in range(7)] for _ in range(10000*4)]

def cut_matrix(matrix):
    # cutoff empty rows for better printing
    new_matrix = []
    start_found = False
    for line in matrix:
        if all(tile == "." for tile in line) and not start_found:
            continue
        start_found = True
        new_matrix.append(line)
    return new_matrix

def count_height(matrix):
    count = 0
    for line in matrix:
        if "#" in line:
            count += 1
    return count

def print_matrix(matrix):
    for i in matrix:
        for j in i:
            print(j, end=" ")
        print()

shapes = [
    [
        ["#","#","#","#"]
    ],[
        [".","#","."],
        ["#","#","#"],
        [".","#","."]
    ],[
        [".",".","#"],
        [".",".","#"],
        ["#","#","#"]
    ],[
        ["#"],
        ["#"],
        ["#"],
        ["#"]
    ],[
        ["#","#"],
        ["#","#"]
    ]
]

def check_collision(x,y, tile, matrix):
    # returns true if collision detected
    for r_ind, row in enumerate(tile[::-1]):
        for ind, t in enumerate(row):
            if t == "#":
                if matrix[y-r_ind][x+ind] == "#":
                    return True
    return False

print_m = False
all_rocks = 2000
rocks = 1585
current_shape = 0
current_y = -4
current_x = 2
current_move = 0
seen = {}
while rocks > 0:
    move = file[current_move]
    if print_m:
        #add tile to matrix
        #cleanup @s
        for i in matrix:
            for j in range(len(i)):
                i[j] = "." if i[j] == "." or i[j] == "@" else "#"
        y = current_y
        for row in shapes[current_shape][::-1]:
            x = current_x
            for tile in row:
                #print(x,y)
                matrix[y][x] = "@" if tile == "#" else "."
                x+=1
            y-=1
        print(move.__repr__(), current_move)
        print_matrix(cut_matrix(matrix))
        #print_matrix(matrix)
        print("---")

    if move == "<":
        current_x -= 1
        if current_x < 0: # wall check
            current_x = 0
        elif check_collision(current_x, current_y, shapes[current_shape], matrix):
            current_x += 1
    elif move == ">":
        current_x += 1
        if current_x+ len(shapes[current_shape][0]) > 7: # wall check
            current_x -= 1
        elif check_collision(current_x, current_y, shapes[current_shape], matrix):
            current_x -= 1
    else:
        raise ValueError("INVALID MOVE")
    current_y += 1
    # check if hit ground/other tile
    if current_y >= 0 or check_collision(current_x, current_y, shapes[current_shape], matrix):
        current_y -= 1

        #add tile to matrix
        y = current_y
        for row in shapes[current_shape][::-1]:
            x = current_x
            for tile in row:
                #print(x,y)
                matrix[y][x] = tile if matrix[y][x] == "." else matrix[y][x]
                x+=1
            y-=1

        # setup for new tile
        current_x = 2
        current_y = -(count_height(matrix)+4)
        current_shape += 1
        current_shape %= len(shapes)
        state = hash(f"{current_shape}{(current_move+1)%len(file)}{matrix[current_y+8:current_y+3]}")
        if state in seen:
            seen[state].append(all_rocks-rocks)
        else:
            seen[state] = [all_rocks-rocks]
        rocks -= 1
        #print(rocks)

    current_move += 1
    current_move %= len(file)

    # Integrity check
    if current_y >= 0:
        raise Exception("ERROR ERROR ERROR")
    if current_x < 0:
        raise Exception("ERROR ERROR ERROR")

#print_matrix(cut_matrix(matrix))

print(f"Part#1: {count_height(matrix)}")
"""
with open("tim/17/output_sample.txt", "a") as f:
    for k,v in seen.items():
        if len(v)>1:
            print(k,v, file=f)
"""
