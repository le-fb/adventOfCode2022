from __future__ import annotations

class Cube:
    def __init__(self, x,y,z) -> None:
        self.x = x
        self.y = y
        self.z = z
        self.left: Cube = None #x-1
        self.right: Cube = None #x+1

        self.up: Cube = None #z-1
        self.down: Cube = None #z+1

        self.front: Cube = None #y-1
        self.back: Cube = None #y+1

with open("tim/18/input.txt", "r") as f:
    file = f.read().splitlines()

cubes: dict[str, Cube] = {}
# key str(xyz) value Cube()

for line in file:
    x,y,z = line.split(",")
    x,y,z = int(x), int(y), int(z)
    cube = Cube(x,y,z)
    if line in cubes:
        print(line)
        raise ValueError("ALARM")
    cubes[line] = cube

for cube in cubes.values():
    x,y,z = cube.x, cube.y, cube.z
    up = f"{x},{y},{z+1}"
    down = f"{x},{y},{z-1}"
    left = f"{x-1},{y},{z}"
    right = f"{x+1},{y},{z}"
    front = f"{x},{y-1},{z}"
    back = f"{x},{y+1},{z}"

    if up in cubes:
        cube.up = cubes[up]
    if down in cubes:
        cube.down = cubes[down]
    if left in cubes:
        cube.left = cubes[left]
    if right in cubes:
        cube.right = cubes[right]
    if front in cubes:
        cube.front = cubes[front]
    if back in cubes:
        cube.back = cubes[back]

# wip
# works only with 1x1x1 air pockets
def is_air_pocket(x,y,z) -> bool:
    up = f"{x},{y},{z+1}"
    down = f"{x},{y},{z-1}"
    left = f"{x-1},{y},{z}"
    right = f"{x+1},{y},{z}"
    front = f"{x},{y-1},{z}"
    back = f"{x},{y+1},{z}"

    check = [
        up in cubes,
        down in cubes,
        left in cubes,
        right in cubes,
        front in cubes,
        back in cubes
    ]

    return all(check)


count = 0
count_p2 = 0
for cube in cubes.values():
    x,y,z = cube.x, cube.y, cube.z
    if cube.up is None:
        count += 1
        if not is_air_pocket(x,y,z+1):
            count_p2 += 1
    if cube.down is None:
        count += 1
        if not is_air_pocket(x,y,z-1):
            count_p2 += 1
    if cube.left is None:
        count += 1
        if not is_air_pocket(x-1,y,z):
            count_p2 += 1
    if cube.right is None:
        count += 1
        if not is_air_pocket(x+1,y,z):
            count_p2 += 1
    if cube.front is None:
        count += 1
        if not is_air_pocket(x,y-1,z):
            count_p2 += 1
    if cube.back is None:
        count += 1
        if not is_air_pocket(x,y+1,z):
            count_p2 += 1

print(count)
print(count_p2)
