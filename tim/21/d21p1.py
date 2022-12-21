from __future__ import annotations
import re

with open("tim/21/input.txt", "r") as f:
    file = f.read().splitlines()


class Monkey:
    def __init__(self, name, val) -> None:
        self.name = name
        self.val: int = val

    def __repr__(self) -> str:
        return self.name

    def __add__(self, other):
        return self.val + other.val

    def __mul__(self, other):
        return self.val * other.val

    def __sub__(self, other):
        return self.val - other.val

    def __truediv__(self, other):
        return self.val / other.val

monkeys = {}
def find_values(open_monkeys):
    n_open_monkeys = []
    for line in open_monkeys:
        names = re.findall("[a-z]+", line)
        if len(names) == 1:
            val = int(re.findall("[0-9]+", line)[0])
            monkey = Monkey(names[0], val)
        else:
            if names[1] not in monkeys or names[2] not in monkeys:
                n_open_monkeys.append(line)
                continue
            else:
                operation = re.findall("[-+*/]", line)[0]
                if operation == "+":
                    value = monkeys[names[1]]+monkeys[names[2]]
                elif operation == "-":
                    value = monkeys[names[1]]-monkeys[names[2]]
                elif operation == "*":
                    value = monkeys[names[1]]*monkeys[names[2]]
                elif operation == "/":
                    value = monkeys[names[1]]/monkeys[names[2]]
                monkey = Monkey(names[0], value)
        monkeys[names[0]] = monkey
    if len(n_open_monkeys) > 0:
        find_values(n_open_monkeys)

find_values(file)

#print(monkeys)
print(monkeys["root"].val)
