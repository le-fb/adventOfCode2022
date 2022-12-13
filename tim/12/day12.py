from __future__ import annotations
import os
import time

class Square:
    def __init__(self, x:int, y:int, height:int, left=None, right=None,up=None, down=None) -> None:
        self.x: int = x
        self.y: int = y
        self.height: int = height
        self.right: Square = right
        self.left: Square = left
        self.up: Square = up
        self.down: Square = down

        self.distance = float("inf")
        self.latest: Square = None
        self.latest_candidate: Square = None

    def __lt__(self, other: Square):
        return self.distance < other.distance

    def __repr__(self) -> str:
        return f"{chr(self.height+97)}"

def read_map():
    with open("tim/12/input.txt", "r") as f:
        file = f.read().splitlines()
    start = None
    end = None
    first = None
    latest_up = None
    for y, line in enumerate(file):
        latest = None
        latest_up = first
        for x, char in enumerate(line):
            # catch start and end
            if char == "S":
                start = (x,y)
                height = 0
            elif char == "E":
                end = (x,y)
                height = ord("z")-97
            else:
                height = ord(char)-97
            current = Square(x, y, height, left=latest, up=latest_up)
            if latest is not None:
                latest.right = current
            latest = current
            if x == 0:
                first = current
            if latest_up is not None:
                latest_up.down = current
                latest_up = latest_up.right

    #moving to upper right corner for standard entry point into grid
    entry = first
    while entry.up is not None:
        entry = entry.up
    while entry.left is not None:
        entry = entry.left

    return entry, start, end

def print_map(square_head: Square, start, end, visited=None, show_visited=False):
    first = square_head
    current = square_head
    while current is not None:
        ch = f"\u001b[1;37m{chr(current.height+97)}"
        if show_visited and current.latest is not None:
            ch = f"\u001b[1;33m{chr(current.height+97)}\u001b[1;37m"
        if visited is not None:
            if (current.x, current.y) in visited:
                ch = f"\u001b[1;31m{chr(current.height+97)}\u001b[1;37m"
        if (current.x, current.y) == start:
            ch = "S"
        if (current.x, current.y) == end:
            ch = "E"
        print(ch, end="")
        current = current.right
        if current is None:
            print()
            first = first.down
            current = first

def calculate_path(grid, start, end, print_grid=False):
    # move to start square coordinates
    start_square = grid
    for _ in range(start[0]):
        start_square = start_square.right
    for _ in range(start[1]):
        start_square = start_square.down

    #visible: list[tuple[int, Square, Square]] = []
    visible: dict[int, list[tuple[Square, Square]]] = {}
    start_square.distance = 0
    current: Square = start_square
    no_path_exist = False
    # terminate if designated square found
    while current.x != end[0] or current.y != end[1]:
        #print(current, current.latest)
        temp = [current.up, current.left, current.right, current.down]
        for square in temp:
            if square is None or square.latest is not None or square.height - current.height > 1:
                continue
            distance = current.distance+1
            #visible.append((square.distance, square, current))
            if distance < square.distance:
                if distance in visible:
                    visible[distance].append(square)
                else:
                    visible[distance] = [square]

                if square.distance in visible:
                    if square in visible[square.distance]:
                        visible[square.distance].remove(square)
                        if len(visible[square.distance]) == 0:
                            del visible[square.distance]

                square.distance = distance
                square.latest_candidate = current

        # break if no more path available -> No existing path
        if len(visible.keys()) == 0:
            no_path_exist = True
            break
        m = min(visible.keys())
        next = visible[m].pop()
        if len(visible[m]) == 0:
            del visible[m]
        current = next
        current.latest = next.latest_candidate
        #print_map(entry, start, end, show_visited=True)
        #time.sleep(0.05)
        #os.system("cls")
    if not no_path_exist:
        steps = 0
        visited = []
        while current != start_square:
            visited.append((current.x, current.y))
            steps += 1
            current = current.latest
        if print_grid:
            print_map(entry, start, end, visited=visited, show_visited=False)
    else:
        steps = float("inf")

    return steps

def find_shortest_path(square_head: Square, height=0):
    lowest = float("inf")
    l_square = None
    first = square_head
    current = square_head
    while current is not None:
        if current.height == height:
            entry, start, end = read_map() # reset probably faster then re initialize?
            dist = calculate_path(entry, (current.x, current.y), end)
            if dist < lowest:
                lowest = dist
                l_square = current
        current = current.right
        if current is None:
            first = first.down
            current = first
    return lowest, l_square

st = time.process_time()
entry, start, end = read_map()
print(f"There are {calculate_path(entry, start, end, print_grid=True)} stepts from start S to destination E")

entry, start, end = read_map()
lowest, lowest_square = find_shortest_path(entry, height=0)
print(f"The shortest Path from a elevation level 'a' sqaure is {lowest}, the square coordinates are ({lowest_square.x}|{lowest_square.y})")

et = time.process_time()
print(f"{et-st} seconds")
