from __future__ import annotations

class Directory:
    def __init__(self, name: str, parent:Directory|None) -> None:
        self.name = name
        self.parent: Directory|None = parent
        self.files: list[File] = []
        self.dirs: list[Directory] = []

    def add_file(self, file:File):
        self.files.append(file)

    def add_dir(self, dir:Directory):
        self.dirs.append(dir)

    def dir_size(self):
        return sum(self.files)+sum([dir.dir_size() for dir in self.dirs])

    def __getitem__(self, key: str) -> Directory:
        for dir in self.dirs:
            if dir.name == key:
                return dir
        raise KeyError(key)

    def __contains__(self, key) -> bool:
        for dir in self.dirs:
            if dir.name == key:
                return True
        return False

    def __repr__(self) -> str:
        return self.name

    def __add__(self, obj: Directory) -> int:
        return self.dir_size()+obj.dir_size()

    def __radd__(self, num: int|float) -> int:
        return self.dir_size()+num

    def __lt__(self, obj: Directory) -> bool:
        return self.dir_size()<obj.dir_size()

class File:
    def __init__(self, name: str, size:int, dir:Directory) -> None:
        self.name = name
        self.size: int = size
        self.dir: Directory = dir

    def __add__(self, obj: File) -> int:
        return self.size+obj.size

    def __radd__(self, num: int|float) -> int:
        return self.size+num

    def __repr__(self) -> str:
        return self.name

def cd(current_dir: Directory, cmd: str) -> Directory:
    # initial handling
    if current_dir is None:
        return head_dir
    cmd = cmd[5:]
    # go back one level
    if cmd == "..":
        return current_dir.parent
    # change to dir
    else:
        return current_dir[cmd]

def parse_content(current_dir: Directory, line:str):
    parts = line.split(" ")
    # add dir
    if parts[0] == "dir":
        dir = Directory(parts[1], current_dir)
        current_dir.add_dir(dir)
    # add file
    elif parts[0].isnumeric():
        file = File(parts[1], int(parts[0]), current_dir)
        current_dir.add_file(file)

    else:
        print(f"Something has gone terribly wrong here... '{line}'")


with open("tim/07/input.txt", "r") as f:
    file = f.read().splitlines()

# parse input file
head_dir = Directory("/", None)
current_dir =  None
for line in file:
    if line.startswith("$ cd"):
        current_dir = cd(current_dir, line)
        continue
    if line.startswith("$ ls"):
        continue

    parse_content(current_dir, line)

def find_max_size(head_dir: Directory, max_size:int):
    dirs = []
    for dir in head_dir.dirs:
        if dir.dir_size() <= max_size:
            dirs.append(dir)
        dirs += find_max_size(dir, max_size)
    return dirs

def find_min_size(head_dir: Directory, min_size:int):
    dirs = []
    for dir in head_dir.dirs:
        if dir.dir_size() >= min_size:
            dirs.append(dir)
        dirs += find_min_size(dir, min_size)
    return dirs

at_most_100_000_dirs = find_max_size(head_dir, 100000)
print(f"There are {len(at_most_100_000_dirs)} dirs with at most 100.000 Size, their sum is {sum(at_most_100_000_dirs)}")
used_space = head_dir.dir_size()
total_space = 70_000_000
desired_unused = 30_000_000
unused_space = total_space - used_space
need_to_free = desired_unused - unused_space
possible_free = find_min_size(head_dir, need_to_free)
print(f"There are {len(possible_free)} candidates for deletion, the smallest is size {min(possible_free).dir_size()}")
