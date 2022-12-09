from __future__ import annotations

class Tree:
    def __init__(self, height: int, left: Tree=None, right: Tree=None, up: Tree=None, down: Tree=None) -> None:
        self.height: int = height
        self.sides = {"left": left, "right":right, "up":up, "down":down}

    def seen_from_outside(self) -> bool:
        return any([
            self.seen_from_side("left"),
            self.seen_from_side("right"),
            self.seen_from_side("up"),
            self.seen_from_side("down")
        ])

    def seen_from_side(self, side: str) -> bool:
        current = self.sides[side]
        while current is not None:
            if current.height >= self.height:
                return False
            current = current[side]
        return True

    def scenic_score(self) -> int:
        up = self.look_distance("up")
        down = self.look_distance("down")
        left = self.look_distance("left")
        right = self.look_distance("right")
        return up*down*left*right

    def look_distance(self, side:str) -> int:
        distance = 0
        current = self[side]
        while current is not None:
            distance += 1
            if current.height >= self.height:
                break
            current = current[side]
        return distance

    def __getitem__(self, key: str) -> Tree:
        return self.sides[key]

    def __setitem__(self, key:str, value: Tree) -> None:
        self.sides[key] = value

    def __lt__(self, other: Tree) -> bool:
        return self.height < other.height

    def __repr__(self) -> str:
        return str(self.height)

class Forrest:
    def __init__(self) -> None:
        self.forrest_grid: list[list[Tree]] = []

    def __getitem__(self, keys) -> Tree:
        x,y = keys
        return self.forrest_grid[y][x]

    def add_row(self, row:list[Tree]):
        self.forrest_grid.append(row)

    def seen_from_outside_count(self) -> int:
        count = 0
        for row in self.forrest_grid:
            for tree in row:
                count += tree.seen_from_outside()
        return count

    def highest_scenic_score(self) -> int:
        highest = 0
        for row in self.forrest_grid:
            for tree in row:
                score = tree.scenic_score()
                if score > highest:
                    highest = score
        return highest

with open("tim/8/input.txt", "r") as f:
    file = f.read().splitlines()

forrest = Forrest()
for row_ind, row in enumerate(file):
    forrest_row = []
    latest = None
    for col_ind, col in enumerate(row):
        new_tree = Tree(int(col), latest)
        forrest_row.append(new_tree)
        if latest is not None:
            latest["right"] = new_tree
        if row_ind-1 >= 0:
            new_tree["up"] = forrest[col_ind, row_ind-1]
            forrest[col_ind, row_ind-1]["down"] = new_tree
        latest = new_tree
    forrest.add_row(forrest_row)

print(f"There are {forrest.seen_from_outside_count()} visible Trees")
print(f"The highest scenic score possible is {forrest.highest_scenic_score()}")
