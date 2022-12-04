Signs = {
    "Rock": "r",
    "Paper": "p",
    "Scissors": "s"
}

DecryptMap = {
    "A": "r", # Rock
    "X": "l", # Loss

    "B": "p", # Paper
    "Y": "d", # Draw

    "C": "s", # Scissors
    "Z": "w"  # Win
}

# maps opponent sign -> sign needed to win
GameLossMap = {
    "r": "s",
    "s": "p",
    "p": "r"
}

# maps opponent sign -> sign needed to lose
GameWinMap = {
    "r": "p",
    "p": "s",
    "s": "r"
}

# Lose: 0, Draw 3, Win 6
def getOutcomeScore(oppSign, playerSign):
    if oppSign == playerSign:
        return 3
    if playerSign == GameLossMap[oppSign]:
        return 0
    if playerSign == GameWinMap[oppSign]:
        return 6
    raise("Faulty implementation of game rules")

def getSignScore(sign):
    if(sign == "r"):
        return 1
    if(sign == "p"):
        return 2
    if(sign == "s"):
        return 3
    raise(f"Sign has to be one of {Signs.values()}. See {Signs}")

def getGameScore(inputOpp, inputPlayer):
    oppSign = DecryptMap[inputOpp]
    desiredOutcome = DecryptMap[inputPlayer]
    playerSign = "?"
    if desiredOutcome == "w":
        playerSign = GameWinMap[oppSign]
    if desiredOutcome == "d":
        playerSign = oppSign
    if desiredOutcome == "l":
        playerSign = GameLossMap[oppSign]

    return getOutcomeScore(oppSign, playerSign) + getSignScore(playerSign)

with open("input2", "r") as input:
    totalPoints = 0
    for line in input:
        totalPoints += getGameScore(line[0], line[2])
    print(f"Total points: {totalPoints}")
