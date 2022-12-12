
import math
import numpy as np

class Explorer:

    root = dict()
    path = list()

    def __init__(self) -> None:
        self.path.append(self.root)

    def current(self):
        return self.path[-1]

    def execute_cd(self, args):

        cur = self.current()
        dir = args[0]

        # Navigate up the dir, pop the current read path
        if dir == '..':
            self.path.pop()
            return
            
        # Create the dir in the virtual tree if needed
        if dir not in cur:
            cur[dir] = dict()
        
        # Navigate into it
        self.path.append(cur[dir])

    def execute_command(self, args):
        # Cd command
        if args[0] == 'cd':
            self.execute_cd(args[1:])
            return

    def read_ls_output(self, args):

        cur = self.current()
        dir_or_filename = args[1]

        if args[0] == 'dir':
            if dir_or_filename not in cur:
                cur[dir_or_filename] = dict()
            return

        cur[dir_or_filename] = int(args[0])

    def read(self, lines):

        for l in lines[1:]:
            args = l.split(' ')

            # If it's a command execute it
            if args[0] == '$':
                self.execute_command(args[1:])
                continue

            # Otherwise read in the output
            self.read_ls_output(args)

    def for_all_in_dir_recursive(self, fun, dir = root):
        for key in dir:
            elem = dir[key]
            if not isinstance(elem, dict):
                yield fun((key, elem))
                continue
            for c in self.for_all_in_dir_recursive(fun, elem):
                yield c

    def for_dirs_recursive(self, fun, dir = root):
        for key in dir:
            elem = dir[key]
            if isinstance(elem, dict):
                yield fun((key, elem))
                for x in self.for_dirs_recursive(fun, elem):
                    yield x

# Get file contents
file1 = open('./sidney/7/data.txt', 'r')
lines = file1.read().splitlines()
file1.close()

# Read file contents into explorer
explorer = Explorer()
explorer.read(lines)

# Get the sizes of all files in a given dir
def count_size(elem):
    return elem[1]
files_in_dirs = list(explorer.for_dirs_recursive(lambda elem: (elem[0], list(explorer.for_all_in_dir_recursive(count_size, elem[1])))))

# Sum them up
sums = list((x[0], np.sum(x[1])) for x in files_in_dirs)
sum = 0
for s in sums:
    if s[1] <= 100000:
        sum += s[1]

print(f'Total size {sum}')

total_size = 70000000
free = total_size - np.sum(explorer.for_all_in_dir_recursive(count_size))
update = 30000000
needed = update - free

candidates = list()
for s in sums:
    if s[1] > needed:
        candidates.append(s[1])

best = np.min(candidates)

print(f'Delete {best}')
