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

with open("input3.txt", "r") as input:
    totalPrio = 0
    for rucksack in input:
        if rucksack[-1:] == "\n":
            rucksack = rucksack[:-1] 
        
        compartmentLength = len(rucksack) >> 1 # If we are doing bitmagic anyway lets do it right lmao
        comp1 = rucksack[:compartmentLength]
        comp2 = rucksack[compartmentLength:]
        
        if(len(comp1) != len(comp2)):
            raise Exception(f"Dude you gotta split the rucksack in the MIDDLE ({len(comp1)} != {len(comp2)})")
        commonLetter = ""
        for letter in comp1:
            if letter in comp2:
                commonLetter = letter
                break
        if commonLetter == "":
            raise Exception("Dude there is nothing in both compartments, I checked!")
        totalPrio += getPriority(commonLetter)

    print(f"Total Priority: {totalPrio}")
