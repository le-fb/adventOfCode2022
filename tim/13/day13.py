with open("tim/13/input.txt", "r") as f:
    file = f.read()

data = file.split("\n\n")

data = [[eval(line.split("\n")[0]), eval(line.split("\n")[1])] for line in data]

def compare_values(left, right):
    for l, r in zip(left, right):
        if type(l) is list and type(r) is list:
            ret = compare_values(l, r)
            if ret is not None:
                return ret
            continue
        if type(l) is list:
            return compare_values(l, [r])
        if type(r) is list:
            return compare_values([l], r)

        if l == r:
            continue
        if l < r:
            return True
        if l > r:
            return False
    if len(left) == len(right):
        return
    return len(left) < len(right)


ind_sum = 0
for ind, pair in enumerate(data, start=1):
    left, right = pair
    if compare_values(left, right):
        ind_sum += ind

print(f"Part1: {ind_sum}")

class Paket:
    def __init__(self, val) -> None:
        self.value = val

    def __lt__(self, other) -> bool:
        val = compare_values(self.value, other.value)
        return val if val is not None else True # i have no idea why but it works so don't touch it!

    def __repr__(self) -> str:
        return f"{self.value}"


all_packets = []
for left, right in data:
    all_packets.append(Paket(left))
    all_packets.append(Paket(right))

devider1 = Paket([[2]])
devider2 = Paket([[6]])
all_packets.append(devider1)
all_packets.append(devider2)

all_packets.sort()

ind1 = all_packets.index(devider1)+1
ind2 = all_packets.index(devider2)+1

print(f"Part2: {ind1*ind2}")

#for i in all_packets:
    #print(i)
