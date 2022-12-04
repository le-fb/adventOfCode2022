with open("input2.txt", "r") as f:
    file = f.read().splitlines()

#A for Rock, B for Paper, and C for Scissors
#X for Rock, Y for Paper, and Z for Scissors
#1 for Rock, 2 for Paper, and 3 for Scissors
#0 if you lost, 3 if the round was a draw, and 6 if you won

beat = {"A":"C", "B":"A", "C":"B"}
trans = {"X":"A", "Y":"B", "Z":"C"}
points = {"A":1, "B":2, "C":3}

total_score = 0
for i in file:
    score = 0
    op, pl = i.split()
    pl = trans[pl]
    score += points[pl]
    if pl == op:
        score += 3
    elif beat[pl] == op:
        score += 6
    total_score += score

print(total_score)

#X means you need to lose, Y means you need to end the round in a draw, and Z means you need to win.
total_score = 0
for i in file:
    op, pl = i.split()
    if pl == "X":
        pl = beat[op]
    elif pl == "Y":
        pl = op
    else:
        for x in ["A", "B", "C"]: #määääää
            if beat[x] == op:
                pl = x
                break

    total_score += points[pl]
    if pl == op:
        total_score += 3
    elif beat[pl] == op:
        total_score += 6

print(total_score)
