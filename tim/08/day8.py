import numpy as np

with open("tim/8/input.txt", "r") as f:
    file = f.read().splitlines()

data = []
boolArr = []
scores = []
for line in file:
    data.append([int(c) for c in line])
    boolArr.append([False for _ in line])
    scores.append([1 for _ in line])

arr = np.array(data)
boolArr = np.array(boolArr)
scores = np.array(scores)

def check_visibility_row(row, boolRow):
    for tree_ind, tree in enumerate(row):
        neighbors = row[:tree_ind]
        if len(neighbors) == 0:
            boolRow[tree_ind] = True
        elif tree > max(neighbors):
            boolRow[tree_ind] = True
    return boolRow

def check_visibility_matrix(matrix, boolMatrix):
    for row_ind, row in enumerate(matrix):
        boolMatrix[row_ind] = check_visibility_row(row, boolMatrix[row_ind])
        boolMatrix[row_ind] = check_visibility_row(row[::-1], boolMatrix[row_ind][::-1])[::-1]

    return boolMatrix

boolArr = check_visibility_matrix(arr, boolArr)
arr = arr.transpose()
boolArr = boolArr.transpose()
boolArr = check_visibility_matrix(arr, boolArr)

visible_trees = 0
for row in boolArr:
    visible_trees += sum(row)

print(f"There are {visible_trees} visible Trees")

def view_len(height, neighbors):
    count = 0
    for n in neighbors[::-1]:
        count += 1
        if n >= height:
            break
    return count

def scenic_score_row(row, scoreRow):
    for tree_ind, tree in enumerate(row):
        neighbors = row[:tree_ind]
        scoreRow[tree_ind] *= view_len(tree, neighbors)
    return scoreRow

def scenic_score_matrix(matrix, scoreMatrix):
    for row_ind, row in enumerate(matrix):
        scoreMatrix[row_ind] = scenic_score_row(row, scoreMatrix[row_ind])
        scoreMatrix[row_ind] = scenic_score_row(row[::-1], scoreMatrix[row_ind][::-1])[::-1]
    return scoreMatrix

scores = scenic_score_matrix(arr, scores)
arr = arr.transpose()
scores = scores.transpose()
scores = scenic_score_matrix(arr, scores)

highest = 0
for row in scores:
    if max(row) > highest:
        highest = max(row)

print(f"The highest scenic score possible is {highest}")
