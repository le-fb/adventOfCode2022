class IntRange:
    LowerBound: int
    UpperBound: int

    def init(self, lower: int, upper: int):
        self.LowerBound = lower
        self.UpperBound = upper

    def __init__(self, lower: int, upper: int) -> None:
        self.init(lower, upper)

    def __init__(self, serial: str) -> None:
        (lower, upper) = serial.split("-")
        self.init(int(lower), int(upper)) 

    def Contains(self, other) -> bool:
        return self.LowerBound <= other.LowerBound and self.UpperBound >= other.UpperBound
    
    def Overlaps(self, other) -> bool:
        if self.UpperBound < other.LowerBound: # Range is lower
            return False
        if self.LowerBound > other.UpperBound: # Range is higher
            return False
        return True # Neither higher or lower -> Overlap

with open("input4", "r") as input:
    numContainments = 0
    numOverlaps = 0
    for pair in input:
        if pair[-1] == "\n":
            pair = pair[:-1]
        ass1, ass2 = pair.split(",")
        range1, range2 = IntRange(ass1), IntRange(ass2)

        if range1.Contains(range2) or range2.Contains(range1):
            numContainments += 1
        if range1.Overlaps(range2):
            numOverlaps += 1

    print(f"There are {numContainments} contained assignments")
    print(f"There are {numOverlaps} overlapping assignments")