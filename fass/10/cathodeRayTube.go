package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func tick(cycleIndex *int, register int, totalStrength *int, outputBuffer *[]string) {
	// part one
	cycleNumber := (*cycleIndex) + 1
	if (cycleNumber+20)%40 == 0 {
		signalStrength := cycleNumber * register
		log.Println("Signal strength during cycle", cycleNumber, ":", signalStrength)
		(*totalStrength) += signalStrength
	}
	// part two
	rowIndex := (*cycleIndex) / 40
	lineIndex := (*cycleIndex) % 40

	if lineIndex == 0 {
		*outputBuffer = append(*outputBuffer, "")
	}

	if register-1 <= lineIndex && lineIndex <= register+1 {
		(*outputBuffer)[rowIndex] += "#"
	} else {
		(*outputBuffer)[rowIndex] += "."
	}

	(*cycleIndex)++
}

func main() {

	args := os.Args[1:]
	input := args[0]

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	register := 1
	var cycleIndex *int = new(int)
	*cycleIndex = 0
	var totalStrength *int = new(int)
	*totalStrength = 0

	outputBuffer := make([]string, 0)

	scanner := bufio.NewScanner(f)

	for scanner.Scan() {
		if len(scanner.Text()) == 0 {
			continue
		}

		var lineParts = strings.Split(scanner.Text(), " ")
		switch lineParts[0] {
		case "addx":
			inc, err := strconv.ParseInt(lineParts[1], 10, 32)
			if err != nil {
				log.Fatal(err)
			}
			tick(cycleIndex, register, totalStrength, &outputBuffer)
			tick(cycleIndex, register, totalStrength, &outputBuffer)
			register += int(inc)
		case "noop":
			tick(cycleIndex, register, totalStrength, &outputBuffer)
		}
	}

	log.Println("Sum of signal strengths is", *totalStrength)

	log.Println("Output buffer is:")
	for _, s := range outputBuffer {
		fmt.Println(s)
	}
}
