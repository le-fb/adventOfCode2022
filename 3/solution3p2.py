# There will be some bit magic going on in the following parts. Tread with caution.

asciiLetterMask = 0b00011111 # In ascii last 5 bits are the letters of the alphabet (starting at a) regardless of capitalization
asciiSmallLetterMask = 0b00100000 # Third bit set indicates a small letter
alphabetOffset = 26


def getPriority(singleChar):
    if(len(singleChar) != 1):
        raise Exception("you fucked up")
    charbyte = bytes(singleChar, "ascii")[0]
    lowerCaseValue = charbyte & asciiLetterMask
    if asciiSmallLetterMask & charbyte == 0b0: # lowercase-bit not set -> upper case letter
        return lowerCaseValue + alphabetOffset
    return lowerCaseValue

rucksackGroup = []
with open("input3", "r") as input:
    totalPrio = 0
    for rucksack in input:        
        if rucksack[-1:] == "\n":
            rucksack = rucksack[:-1] 
        
        rucksackGroup.append(rucksack)
        if len(rucksackGroup) < 3:
            continue

        commonLetter = ""
        for letter in rucksackGroup[0]:
            Mismatch = True
            for compareSack in rucksackGroup[1:]:
                if not letter in compareSack:
                    Mismatch = True
                    break
                Mismatch = False
            if Mismatch:
                continue
            commonLetter = letter

        if commonLetter == "":
            raise Exception("Dude there is nothing in both compartments, I checked!")
        totalPrio += getPriority(commonLetter)
        rucksackGroup = []

    print(f"Total Priority: {totalPrio}")
