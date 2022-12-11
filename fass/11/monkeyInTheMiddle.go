package main

import (
	"bufio"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
)

type Item struct {
	uid        string // mainly for debugging
	WorryLevel int64
}

type Monkey struct {
	NumInspections int
	TestDivisor    int
	Operation      func(int64) int64
	HeldItems      []*Item
	ThrowTargets   *[2]int // index 0 for condition true, index 1 for false
}

func main() {
	args := os.Args[1:]
	input := args[0]
	exercisePart := args[1]
	part, err := strconv.Atoi(exercisePart)
	if err != nil {
		log.Fatal(err)
	}

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	scanner := bufio.NewScanner(f)
	var monkeys []Monkey = make([]Monkey, 0, 8)
	var monkeyIndex int

	for scanner.Scan() {
		if scanner.Text() == "" {
			continue
		}
		words := strings.Split(strings.TrimSpace(scanner.Text()), " ")
		if words[0] == "Monkey" {
			monkeyIndex_, err := strconv.ParseInt(words[1][:len(words[1])-1], 10, 0)
			monkeyIndex = int(monkeyIndex_)
			if err != nil {
				log.Fatal(err)
			}
			m := Monkey{
				ThrowTargets: new([2]int),
			}
			monkeys = append(monkeys, m)
			continue
		}
		// Parse "Starting itmes: ..."
		curMonkey := &monkeys[monkeyIndex]
		if words[0] == "Starting" {
			worryList := strings.Split(scanner.Text(), ":")[1]
			numbers := strings.Split(worryList, ",")
			for i, numS := range numbers {
				num, err := strconv.Atoi(strings.TrimSpace(numS))
				if err != nil {
					log.Fatal(err)
				}
				item := Item{
					uid:        strconv.Itoa(monkeyIndex) + "." + strconv.Itoa(i),
					WorryLevel: int64(num)}
				curMonkey.HeldItems = append(curMonkey.HeldItems, &item)
			}
			continue
		}
		// Parse Operation: ...""
		if words[0] == "Operation:" {
			opString := strings.Split(scanner.Text(), ":")[1]
			rightHandside := strings.Split(opString, "=")[1]
			opParts := strings.Split(strings.TrimSpace(rightHandside), " ")
			if opParts[0] == "old" && opParts[2] == "old" {
				switch opParts[1] {
				case "+":
					curMonkey.Operation = func(i int64) int64 {
						return i + i
					}
				case "*":
					curMonkey.Operation = func(i int64) int64 {
						return i * i
					}
				}
				continue
			}
			opConst, err := strconv.ParseInt(opParts[2], 10, 64)
			if err != nil {
				log.Fatal(err)
			}

			switch opParts[1] {
			case "+":
				curMonkey.Operation = func(i int64) int64 {
					return i + opConst
				}
			case "*":
				curMonkey.Operation = func(i int64) int64 {
					return i * opConst
				}
			}
			continue
		}
		// Parse Test: ...
		if words[0] == "Test:" {
			divisor, err := strconv.Atoi(words[3])
			if err != nil {
				log.Fatal(err)
			}
			curMonkey.TestDivisor = divisor
			continue
		}
		// Parse If true:... ; If false: ...
		if words[0] == "If" {
			if words[1] == "true:" {
				target, err := strconv.Atoi(words[5])
				if err != nil {
					log.Fatal(err)
				}
				curMonkey.ThrowTargets[0] = target
				continue
			}
			if words[1] == "false:" {
				target, err := strconv.Atoi(words[5])
				if err != nil {
					log.Fatal(err)
				}
				curMonkey.ThrowTargets[1] = target
				continue
			}
		}
	}

	commonMultiple := 1
	for _, m := range monkeys {
		log.Println(m, m.ThrowTargets)
		commonMultiple *= m.TestDivisor
	}

	log.Println("Found Common Multiple", commonMultiple)

	numRounds := 20
	if part == 2 {
		numRounds = 10_000
	}

	for r := 0; r < numRounds; r++ {
		for i := 0; i < len(monkeys); i++ {
			monkey := &monkeys[i]
			for itemIndex := range monkey.HeldItems {
				item := monkey.HeldItems[itemIndex]
				item.WorryLevel = monkey.Operation(item.WorryLevel)
				if part == 1 {
					item.WorryLevel = item.WorryLevel / 3
				}
				if item.WorryLevel < 0 {
					log.Fatal("overflow during round ", r)
				}
				item.WorryLevel = item.WorryLevel % int64(commonMultiple)
				var throwTarget *Monkey
				if item.WorryLevel%int64(monkey.TestDivisor) == 0 {
					throwTarget = &monkeys[monkey.ThrowTargets[0]]
				} else {
					throwTarget = &monkeys[monkey.ThrowTargets[1]]
				}
				throwTarget.HeldItems = append(throwTarget.HeldItems, item)
				monkey.NumInspections++
			}
			monkey.HeldItems = make([]*Item, 0)
		}

		// if r%1000 == 999 || r == 19 {
		// 	log.Println("Simulated", r+1, "rounds")
		// 	for i, m := range monkeys {
		// 		log.Println("Monkey", i, "inspected items", m.NumInspections, "times")
		// 	}
		// }
	}

	log.Println("Simulated", numRounds, "rounds")

	for i, m := range monkeys {
		log.Println("Monkey", i, "inspected items", m.NumInspections, "times")
		// log.Println("Held Items:")
		// for _, item := range m.HeldItems {
		// 	log.Print(item)
		// }
	}

	sortedMonkeys := monkeys
	sort.SliceStable(sortedMonkeys, func(i, j int) bool { return sortedMonkeys[i].NumInspections > sortedMonkeys[j].NumInspections })
	log.Println("Total Monkey-Business:", sortedMonkeys[0].NumInspections*sortedMonkeys[1].NumInspections)
}
