from __future__ import annotations
import re

class Monkey:
    def __init__(self, id:int, items: list[Item], operation:str, test:int) -> None:
        self.id = id
        self.items: list[Item] = items
        self.operation: str = operation
        self.test: int = test
        self.target_true: Monkey = None
        self.target_false: Monkey = None
        self.items_inspected = 0
        self.operation_l = eval("lambda old: "+self.operation)
        self.common_multiple: int = 0

    def add_item(self, item:Item):
        self.items.append(item)

    def inspect_item(self, item:Item, worry_relief: bool):
        # print(f"Monkey {self.id} inspects an item with a worry level of {item.worry_level}")
        self.items_inspected += 1
        old = item.worry_level
        #new = eval(self.operation)
        new = self.operation_l(old)
        if worry_relief:
            item.worry_level = int(new/3)
        else:
            item.worry_level = new%self.common_multiple

    def throw_item(self, item: Item):
        if item.worry_level % self.test == 0:
            self.target_true.add_item(item)
        else:
            self.target_false.add_item(item)

    def turn(self, relief:bool):
        while len(self.items) > 0:
            item = self.items.pop(0)
            self.inspect_item(item, relief)
            self.throw_item(item)

    def __repr__(self) -> str:
        s = f"""Monkey {self.id}:\n
        \tItems: {", ".join([str(i) for i in self.items])}\n
        \tOperation: new = {self.operation}\n
        \tTest: divisible by {self.test}\n
        \t\tIf true: throw to monkey {self.target_true.id}\n
        \t\tIf false: throw to monkey {self.target_false.id}\n
        """
        return s

class Item:
    def __init__(self, worry_level:int) -> None:
        self.worry_level: int = worry_level

    def __repr__(self) -> str:
        return str(self.worry_level)

def initialize_monkeys() -> list[Monkey]:
    with open("tim/11/input.txt", "r") as f:
        file = f.read()
    data = file.split("\n\n")
    monkeys: list[Monkey] = []
    throw_targets: list[tuple[int,int]] = []
    common_multiple = 1
    for ind, monkey_data in enumerate(data):
        splitted = monkey_data.split("\n")
        items_nr = re.findall("[0-9]+", splitted[1])
        items = [Item(int(item_nr)) for item_nr in items_nr]
        operation = splitted[2].split("= ")[1]
        test = int(re.findall("[0-9]+", splitted[3])[0])
        common_multiple *= test
        true = int(re.findall("[0-9]+", splitted[4])[0])
        false = int(re.findall("[0-9]+", splitted[5])[0])
        monkeys.append(Monkey(ind, items, operation, test))
        throw_targets.append((true, false))
    # set throw target pointers
    # set common multiple
    for monkey, throws in zip(monkeys, throw_targets):
        monkey.target_true = monkeys[throws[0]]
        monkey.target_false = monkeys[throws[1]]
        monkey.common_multiple = common_multiple
    return monkeys


monkeys = initialize_monkeys()
for i in (range(20)):
    for monkey in monkeys:
        monkey.turn(relief=True)

inspections = sorted([monkey.items_inspected for monkey in monkeys])
print(f"Part#1: The level of monkey business is {inspections[-1]*inspections[-2]}")

monkeys = initialize_monkeys()
for i in range(10000):
    for monkey in monkeys:
        monkey.turn(relief=False)

inspections = sorted([monkey.items_inspected for monkey in monkeys])
print(f"Part#2: The level of monkey business is {inspections[-1]*inspections[-2]}")

