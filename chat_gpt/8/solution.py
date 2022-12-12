# Read the input from the puzzle
trees = []
with open('./chat_gpt/8/input.txt') as f:
    for line in f:
        trees.append(list(map(int, line.strip())))

counted = set()

# Function that returns the number of trees visible from a given direction
# (up, down, left, or right) from a given starting point
def count_trees(trees, x, y, dx, dy):
    max_height = 0
    count = 0
    global counted
    while 0 <= x < len(trees) and 0 <= y < len(trees[0]):
        if (x, y) not in counted and trees[x][y] > max_height:
            max_height = trees[x][y]
            count += 1
            counted.add((x, y))
        x += dx
        y += dy
    return count

# Count the number of trees visible from the edge of the grid
# (looking in each direction)
count = 0

# Look right from each row along the top and bottom edges of the grid
for i in range(len(trees)):
    count += count_trees(trees, i, 0, 0, 1) # Looking right from the top edge
    count += count_trees(trees, i, len(trees[0]) - 1, 0, -1) # Looking left from the bottom edge

# Look down from each column along the left and right edges of the grid
for j in range(len(trees[0])):
    count += count_trees(trees, 0, j, 1, 0) # Looking down from the left edge
    count += count_trees(trees, len(trees) - 1, j, -1, 0) # Looking up from the right edge

# Print the result
print(count)
