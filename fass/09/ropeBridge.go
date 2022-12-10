package main

import (
	"bufio"
	"log"
	"os"
	"strconv"
)

type coordinate struct {
	row int
	col int
}

func abs(num int) int {
	if num >= 0 {
		return num
	}
	return -num
}

func moveTail(head coordinate, tail *coordinate) {
	colDist := abs(head.col - (*tail).col)
	rowDist := abs(head.row - (*tail).row)

	if colDist <= 1 && rowDist <= 1 {
		// touching
		return
	}

	if rowDist != 0 {
		if (*tail).row < head.row {
			(*tail).row++
		} else {
			(*tail).row--
		}
	}
	if colDist != 0 {
		if (*tail).col < head.col {
			(*tail).col++
		} else {
			(*tail).col--
		}
	}
}

func main() {
	args := os.Args[1:]
	input := args[0]
	knotSize := args[1]

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	numKnots, err := strconv.ParseInt(knotSize, 10, 32)
	if err != nil {
		log.Fatal(err)
	}
	if numKnots < 2 {
		log.Fatal("Please use 2 or more knots")
	}

	// Close the file after function completes
	defer f.Close()

	scanner := bufio.NewScanner(f)

	knots := make([]coordinate, numKnots)

	tailCoords := make([]coordinate, 0)

	for scanner.Scan() {
		if len(scanner.Text()) == 0 {
			continue
		}

		command := scanner.Text()[0:1]
		amountS := scanner.Text()[2:]
		amount, err := strconv.ParseInt(amountS, 10, 32)
		if err != nil {
			log.Fatal(err)
		}
		for i := 0; i < int(amount); i++ {
			switch command {
			case "U":
				knots[0].row++
			case "D":
				knots[0].row--
			case "R":
				knots[0].col++
			case "L":
				knots[0].col--
			}
			for k := 1; k < int(numKnots); k++ {
				moveTail(knots[k-1], &knots[k])
			}
			skip := false
			tail := knots[numKnots-1]
			for _, c := range tailCoords {
				if c == tail {
					skip = true
					break
				}
			}
			if !skip {
				tailCoords = append(tailCoords, tail)
			}
			// log.Println(h, t)
		}
	}

	log.Println("Number of points visited by the tail:", len(tailCoords))
}
