from __future__ import annotations
import time
import os

with open("tim/9/samplep2.txt", "r") as f:
    file = f.read().splitlines()

instructions = [(line.split()[0], int(line.split()[1]))  for line in file]

class Knot:
    def __init__(self, name: str, x: int, y: int, tail: Knot=None) -> None:
        self.name: str = name
        self.x: int = x
        self.y: int = y
        self.tail = tail
        self.visited = {(x, y)}

    def move(self, direction: str):
        if direction == "U":
            self.y -= 1
        elif direction == "D":
            self.y += 1
        elif direction == "R":
            self.x += 1
        elif direction == "L":
            self.x -= 1
        self.visited.add((self.x, self.y))

    def move_tail(self):
        xdist = self.x-self.tail.x
        ydist = self.y-self.tail.y
        if -2 < xdist < 2 and -2 < ydist < 2:
            # if adjacent return
            # return
            pass
        elif xdist == 0:
            #check move y
            self.tail.y += -1 if self.y < self.tail.y else 1
        elif ydist == 0:
            #check move x
            self.tail.x += -1 if self.x < self.tail.x else 1
        else:
            self.tail.x += -1 if self.x < self.tail.x else 1
            self.tail.y += -1 if self.y < self.tail.y else 1

        self.tail.visited.add((self.tail.x, self.tail.y))

class Knot_Grid:
    def __init__(self, size:int, rope_head: Knot, empty_field: str = ".") -> None:
        self.size: int = size
        self.rope: Knot = rope_head
        self.empty_field: str = empty_field

    def execute_instruction(self, direction:str, show:bool=True):
        self.rope.move(direction)
        if show:
            self.print_grid()
        current = self.rope
        while current.tail is not None:
            current.move_tail()
            current = current.tail
            if show:
                time.sleep(0.1)
                self.print_grid()

    def model_grid(self) -> list[str]:
        grid = [[self.empty_field for _ in range(self.size)] for _ in range(self.size)]
        current = self.rope
        while current is not None:
            if grid[current.y][current.x] == self.empty_field:
                grid[current.y][current.x] = current.name
            current = current.tail
        return grid

    def print_grid(self):
        grid = self.model_grid()
        os.system("cls")
        for line in grid:
            for c in line:
                print(c, end=" ")
            print()
        print("")



names = [str(i) for i in range(1,10)]
start_x = 15
start_y = 15

knot_head = Knot("H", start_x, start_y)
current = knot_head
for name in names:
    current.tail = Knot(name, start_x, start_y)
    current = current.tail

grid = Knot_Grid(start_x*2, knot_head)

for direction, count in instructions:
    for _ in range(count):
        grid.execute_instruction(direction, show=True)
print(f"First Knot visited {len(grid.rope.tail.visited)} positions at least once")
current = grid.rope
while current.tail is not None:
    current = current.tail
print(f"Last Knot visited {len(current.visited)} positions at least once")
