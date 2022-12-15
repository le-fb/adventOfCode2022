package main

import (
	"bufio"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
)

type packet struct {
	value    int
	hasValue bool
	packets  []*packet
}

func compare(left, right *packet) int {
	for i, innerLeft := range left.packets {
		if i >= len(right.packets) {
			// right side is shorter -> right wins
			return -1
		}
		innerRight := right.packets[i]
		// Both integers
		if innerLeft.hasValue && innerRight.hasValue {
			if innerLeft.value == innerRight.value {
				continue
			} else {
				if innerLeft.value < innerRight.value {
					return 1
				} else {
					return -1
				}
			}
		}
		// One integer, one list -> convert integer to list and continue
		if innerRight.hasValue && !innerLeft.hasValue {
			temp := &packet{
				hasValue: false,
				packets:  make([]*packet, 1),
			}
			temp.packets[0] = innerRight
			innerRight = temp
		} else if innerLeft.hasValue && !innerRight.hasValue {
			temp := &packet{
				hasValue: false,
				packets:  make([]*packet, 1),
			}
			temp.packets[0] = innerLeft
			innerLeft = temp
		}
		// Both lists
		if !innerLeft.hasValue && !innerRight.hasValue {
			innerComp := compare(innerLeft, innerRight)
			if innerComp != 0 {
				return innerComp
			} else {
				continue
			}
		}
		log.Fatal("Reached unreachable part of the compare function")
	}
	// left side is shorter -> left wins
	if len(right.packets) > len(left.packets) {
		return 1
	}
	return 0
}

func contains(char rune, set []rune) bool {
	for _, compareChar := range set {
		if char == compareChar {
			return true
		}
	}
	return false
}

func main() {
	args := os.Args[1:]
	input := ""
	if len(args) < 1 {
		input = "./testinput13.txt"
	} else {
		input = args[0]
	}

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	scanner := bufio.NewScanner(f)
	allPackets := make([]*packet, 0)

	stopChars := [3]rune{'[', ']', ','}

	for scanner.Scan() {
		trimmed := strings.TrimSpace(scanner.Text())
		if trimmed == "" {
			continue
		}

		if trimmed[0] == '[' {
			linePacket := packet{hasValue: false, packets: make([]*packet, 0)}
			allPackets = append(allPackets, &linePacket)

			stack := make([]*packet, 1)
			stack[0] = &linePacket

			cursor := 1
			curText := ""
			appendTextAsNumber := func() {
				if len(curText) > 0 {
					val, err := strconv.Atoi(curText)
					if err != nil {
						log.Fatal(err)
					}
					pack := packet{hasValue: true, value: val}
					stack[len(stack)-1].packets = append(stack[len(stack)-1].packets, &pack)
				}
			}

			for len(stack) > 0 && cursor < len(trimmed) {
				curChar := rune(trimmed[cursor])
				if !contains(rune(curChar), stopChars[:]) {
					curText += trimmed[cursor : cursor+1]
					cursor++
					continue
				}
				switch curChar {
				case '[':
					pack := packet{hasValue: false, packets: make([]*packet, 0)}
					stack[len(stack)-1].packets = append(stack[len(stack)-1].packets, &pack)
					stack = append(stack, &pack)
				case ',':
					appendTextAsNumber()
				case ']':
					appendTextAsNumber()
					stack = stack[:len(stack)-1]
				default:
					log.Fatal("Fell through to unintended default case")
				}
				cursor++
				curText = ""
			}

			if len(stack) > 0 {
				log.Fatal("Parser error. Hit line end before fully parsed")
			}
		}
	}

	if scanner.Err() != nil {
		log.Fatal(scanner.Err())
	}

	log.Println("Successfully parsed", len(allPackets), "packets")

	totalRightPairNum := 0

	for i := 0; i < len(allPackets)/2; i++ {
		compResult := compare(allPackets[2*i], allPackets[(2*i)+1])
		if compResult == 0 {
			log.Fatal("Comparison was indefinite")
		} else if compResult == 1 {
			totalRightPairNum += i + 1
			// log.Println("Pair", i+1, "is in right order")
		} else if compResult == -1 {
			// log.Println("Pair", i+1, "is in wrong order")
		} else {
			log.Fatal("Pffffffft")
		}
	}

	log.Println("Total index-sum is", totalRightPairNum)

	smallerThanFunc := func(i, j int) bool {
		comp := compare(allPackets[i], allPackets[j])
		if comp == 0 {
			log.Fatal("Could not compare packets.")
		}
		return comp == 1
	}

	divider1 := packet{
		hasValue: false,
		packets: []*packet{{
			hasValue: false,
			packets: []*packet{{
				hasValue: true,
				value:    2,
			}},
		}},
	}
	divider2 := packet{
		hasValue: false,
		packets: []*packet{{
			hasValue: false,
			packets: []*packet{{
				hasValue: true,
				value:    6,
			}},
		}},
	}

	allPackets = append(allPackets, &divider1, &divider2)

	sort.Slice(allPackets, smallerThanFunc)

	decoderPacketNumbers := [2]int{-1, -1}
	for i := 0; i < len(allPackets); i++ {
		if compare(&divider1, allPackets[i]) == 0 {
			log.Println("Found divider at packet number", i+1)
			decoderPacketNumbers[0] = i + 1
		}
		if compare(&divider2, allPackets[i]) == 0 {
			log.Println("Found divider at packet number", i+1)
			decoderPacketNumbers[1] = i + 1
		}
	}

	log.Println("Decoder key is", decoderPacketNumbers[0]*decoderPacketNumbers[1])
}
