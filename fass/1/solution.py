with open("adventOfCode1.input", mode = 'r') as input:

    calorieCounts = {0: 0}

    elfIndex = 0

    for line in input:
        foodcalories = 0
        if line != "\n":
            foodcalories = int(line[:-1]) 
        else:
            # End of Elf. Move to next elf.
            elfIndex += 1
            calorieCounts[elfIndex] = 0

        calorieCounts[elfIndex] += foodcalories

    top1 = -1
    top2 = -1
    top3 = -1

    for elfCalories in calorieCounts.values():
        if elfCalories > top3:
            top3 = elfCalories
        if top3 > top2:
            buffer = top2
            top2 = top3
            top3 = buffer
        if top2 > top1:
            buffer = top1
            top1 = top2
            top2 = buffer
        
    print(f"The top 3 calorie counts are: {top1}, {top2} and {top3} for a total of {top1 + top2 + top3} calories")
        