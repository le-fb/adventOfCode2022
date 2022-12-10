with open("tim/9/input.txt", "r") as f:
    file = f.read().splitlines()

instructions = [(line.split()[0], int(line.split()[1]))  for line in file]

def move(x, y, direction):
    if direction == "U":
        y += 1
    elif direction == "D":
        y -= 1
    elif direction == "R":
        x += 1
    elif direction == "L":
        x -= 1
    return x,y

def move_tail(head, tail):
    xh,yh = head
    xt, yt = tail
    xdist = xh-xt
    ydist = yh-yt
    move_x = 0
    move_y = 0
    if -2 < xdist < 2 and -2 < ydist < 2:
        # if adjacent return
        return xt, yt
    if xdist == 0:
        #check move y
        move_y = -1 if yh < yt else 1
    elif ydist == 0:
        #check move x
        move_x = -1 if xh < xt else 1
    else:
        move_x = -1 if xh < xt else 1
        move_y = -1 if yh < yt else 1

    return xt+move_x, yt+move_y

xh, yh = 0, 0
xt, yt = 0, 0
visited = {(xt, yt)}
for direction, count in instructions:
    for _ in range(count):
        xh, yh = move(xh, yh, direction)
        xt, yt = move_tail((xh,yh), (xt, yt))
        visited.add((xt,yt))

print(f"Part#1: The tail of the rope visit at least once {len(visited)} positions")

x_head, y_head = 0, 0
knots = [(0,0) for _ in range(9)]
visited = {(0,0)}
for direction, count in instructions:
    for _ in range(count):
        x_head, y_head = move(x_head,y_head, direction)
        xh, yh = x_head, y_head
        for knot_ind, knot in enumerate(knots):
            new_knot = move_tail((xh, yh), knot)
            knots[knot_ind] = new_knot
            xh, yh = new_knot
        visited.add(new_knot)

print(f"Part#2: The tail of the rope visit at least once {len(visited)} positions")
