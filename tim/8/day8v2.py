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
    def __init__(self, root: Tree=None) -> None:
        self.root: Tree = None

    def seen_from_outside_count(self) -> int:
        count = 0
        first = self.root
        current = self.root
        while current is not None:
            count += current.seen_from_outside()
            current = current["right"]
            if current is None:
                current = first["down"]
                first = first["down"]
        return count

    def highest_scenic_score(self) -> int:
        highest = 0
        first = self.root
        current = self.root
        while current is not None:
            score = current.scenic_score()
            if score > highest:
                highest = score
            current = current["right"]
            if current is None:
                current = first["down"]
                first = first["down"]
        return highest

with open("tim/8/input.txt", "r") as f:
    file = f.read().splitlines()

forrest = Forrest()
latest_row = None
for row_ind, row in enumerate(file):
    current_row = []
    for col_ind, col in enumerate(row):
        new_tree = Tree(int(col))
        current_row.append(new_tree)
        if len(current_row)>1:
            new_tree["left"] = current_row[col_ind-1]
            current_row[col_ind-1]["right"] = new_tree

        if latest_row is not None:
            new_tree["up"] = latest_row[col_ind]
            latest_row[col_ind]["down"] = new_tree
    latest_row = current_row

# get upper left tree as root
while new_tree["up"] is not None:
    while new_tree["left"] is not None:
        new_tree = new_tree["left"]
    new_tree = new_tree["up"]
upper_left = new_tree

forrest.root = upper_left

print(f"There are {forrest.seen_from_outside_count()} visible Trees")
print(f"The highest scenic score possible is {forrest.highest_scenic_score()}")
