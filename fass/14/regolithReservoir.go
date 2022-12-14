package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

const ROCK, SAND, FALLING_SAND, AIR = 'r', 's', 'x', 0

type coordinate struct {
	x, y int
}

func main() {

	args := os.Args[1:]
	input := "./testinput14.txt"
	mode := 2
	if len(args) >= 1 {
		input = args[0]
	}
	if len(args) >= 2 {
		mode, _ = strconv.Atoi(args[1])
	}

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	scanner := bufio.NewScanner(f)
	maxX, maxY := -1, -1
	minX, minY := -1, -1
	rocklines := make([][]coordinate, 0)

	for scanner.Scan() {
		coordStrings := strings.Split(scanner.Text(), " -> ")
		if len(coordStrings) < 2 {
			log.Fatal("At least two coordinates expected")
		}
		rockline := make([]coordinate, 0)

		for _, coordCombinedString := range coordStrings {

			coordStrings := strings.Split(coordCombinedString, ",")
			x, errX := strconv.Atoi(coordStrings[0])
			y, errY := strconv.Atoi(coordStrings[1])

			if errX != nil {
				log.Fatal(errX)
			}
			if errY != nil {
				log.Fatal(errY)
			}

			if x > maxX {
				maxX = x
			}
			if x < minX || minX == -1 {
				minX = x
			}
			if y > maxY {
				maxY = y
			}
			if y < minY || minY == -1 {
				minY = y
			}
			rockline = append(rockline, coordinate{x, y})
		}
		rocklines = append(rocklines, rockline)
	}

	cave := make([][]rune, maxX+(maxY-minY)+4) // Make cave one wider than the sand pyramid to the top would be right
	for i := 0; i < len(cave); i++ {
		cave[i] = make([]rune, maxY+2) // make cave one lower than lowest rock
		for j := 0; j < len(cave[i]); j++ {
		}
	}

	for _, rockline := range rocklines {
		curCoord := rockline[0]
		for _, coord := range rockline {
			if curCoord.x == coord.x {
				for y := curCoord.y; y <= coord.y; y++ {
					cave[curCoord.x][y] = ROCK
				}
				for y := coord.y; y <= curCoord.y; y++ {
					cave[coord.x][y] = ROCK
				}
				curCoord = coord
				continue
			}
			if curCoord.y == coord.y {
				for x := curCoord.x; x <= coord.x; x++ {
					cave[x][coord.y] = ROCK
				}
				for x := coord.x; x <= curCoord.x; x++ {
					cave[x][coord.y] = ROCK
				}
				curCoord = coord
				continue
			}
			log.Fatal("Subsequent coordinates have to match x or y")
		}
	}

	numRestingSandPieces := 0
	lastParticlePath := make([]coordinate, 0)
	for {
		fellThrough := false
		// Simulate sand particle
		coord := coordinate{x: 500, y: 0}
		for {
			lastParticlePath = append(lastParticlePath, coord)
			if coord.y > maxY {
				// Hit bottom
				if mode == 1 {
					fellThrough = true
				}
				break
			}
			if cave[coord.x][coord.y+1] == AIR {
				coord.y++
				continue
			}
			// directly above blocked tile
			if cave[coord.x-1][coord.y+1] == AIR {
				coord.y++
				coord.x--
				continue
			}
			if cave[coord.x+1][coord.y+1] == AIR {
				coord.y++
				coord.x++
				continue
			}
			// SAND sits on sand and can't move diagonally down
			break
		}
		if fellThrough {
			break
		}
		cave[coord.x][coord.y] = SAND
		numRestingSandPieces++

		if coord.x == 500 && coord.y == 0 {
			break
		}
		lastParticlePath = make([]coordinate, 0)
	}

	log.Println("Total number of resting sand pieces is", numRestingSandPieces)
	if mode == 1 {
		for _, coord := range lastParticlePath {
			cave[coord.x][coord.y] = FALLING_SAND
		}
	}

	minXPrint := 0
	foundMin := false
	for x := range cave {
		for y := range cave[x] {
			if cave[x][y] != AIR {
				minXPrint = x
				foundMin = true
				break
			}
		}
		if foundMin {
			break
		}
	}

	for y := 0; y <= maxY+1; y++ {
		for x := minXPrint; x < len(cave); x++ {
			letter := cave[x][y]
			if letter == AIR {
				fmt.Print(".")
			}
			if letter == ROCK {
				fmt.Print("#")
			}
			if letter == SAND {
				fmt.Print("o")
			}
			if letter == FALLING_SAND {
				fmt.Print("~")
			}
		}
		fmt.Println()
	}
}
