with open("tim/10/input.txt", "r") as f:
    file = f.read().splitlines()

def check_cycle(cycle, x):
    cycles = [20, 60, 100, 140, 180, 220]
    strength = 0
    if cycle in cycles:
        strength = cycle*x
        #print(cycle, x, strength)
    return strength

def draw(cycle, x):
    CRT_pos = cycle%40
    if CRT_pos == 0:
        print()
    if x-1 <= CRT_pos <= x+1:
        print("#", end="")
    else:
        print(".", end="")

strength_sum = 0
x=1
cycle = 0
for line in file:
    if line == "noop":
        draw(cycle, x)
        cycle += 1
        strength_sum += check_cycle(cycle, x)
    else:
        val = int(line.split()[-1])
        draw(cycle, x)
        cycle += 1
        strength_sum += check_cycle(cycle, x)
        draw(cycle, x)
        cycle +=1
        strength_sum += check_cycle(cycle, x)
        x += val

print(f"\nTotal signal strength: {strength_sum}")



