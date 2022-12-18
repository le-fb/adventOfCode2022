package main

import (
	"bufio"
	"fass/tools/utils"
	"log"
	"os"
)

const ROCK = rune('#')

// Checks whether collision has occurred downward (coordinate is x = leftmost col index, y = lowest row index)
func checkCollisionDown(room [][]rune, rock [][]rune, coord utils.Coordinate) bool {
	if coord.Y == 0 { // Floor
		return true
	}
	for iCol, rockCol := range rock {
		for iRow, rockPart := range rockCol {
			if rockPart == ROCK {
				colInRoom := iCol + coord.X
				rowInRoom := iRow + coord.Y
				if colInRoom >= len(room) {
					log.Fatal("rock drifted out of room?")
				}
				if rowInRoom-1 >= len(room[colInRoom]) { // Rock is higher than anything in the room currently
					continue
				}
				if room[colInRoom][rowInRoom-1] == ROCK {
					return true
				}
				// Checked the most bottom rock of this coloumn
				break
			}
		}
	}
	return false
}

func checkCollisionSides(room [][]rune, rock [][]rune, coord utils.Coordinate, offset int) bool {
	if coord.X+offset < 0 || coord.X+len(rock)-1+offset >= len(room) { // Walls
		return true
	}

	for iCol, rockCol := range rock {
		for iRow, rockPart := range rockCol {
			if rockPart == ROCK {
				colInRoom := iCol + coord.X + offset
				rowInRoom := iRow + coord.Y
				if colInRoom >= len(room) {
					log.Fatal("rock drifted out of room?")
				}
				if rowInRoom >= len(room[0]) { // No rock collisions above the room
					continue
				}
				if room[colInRoom][rowInRoom] == ROCK {
					return true
				}
			}
		}
	}
	return false
}

func main() {
	args := os.Args[1:]
	input := "testinput17.txt"
	if len(args) >= 1 {
		input = args[0]
	}

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	jetOffsets := make([]int, 0)

	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		for _, r := range scanner.Text() {
			if r == rune('<') {
				jetOffsets = append(jetOffsets, -1)
			}
			if r == rune('>') {
				jetOffsets = append(jetOffsets, 1)
			}
		}
	}

	rock0 := [][]rune{{ROCK}, {ROCK}, {ROCK}, {ROCK}}
	rock1 := [][]rune{{0, ROCK, 0}, {ROCK, ROCK, ROCK}, {0, ROCK, 0}}
	rock2 := [][]rune{{ROCK, 0, 0}, {ROCK, 0, 0}, {ROCK, ROCK, ROCK}}
	rock3 := [][]rune{{ROCK, ROCK, ROCK, ROCK}}
	rock4 := [][]rune{{ROCK, ROCK}, {ROCK, ROCK}}

	rocks := [][][]rune{rock0, rock1, rock2, rock3, rock4}
	fallingRockType := 0
	highestRowIndex := 0
	jetIndex := 0

	numRocks := 2022
	room := make([][]rune, 7)
	for i := 0; i < len(room); i++ {
		room[i] = make([]rune, 0, 4*numRocks)
	}
	// cleanups := make([]int, 0)

	for iRock := 0; iRock < numRocks; iRock++ {
		// Spawn rock
		rock := rocks[fallingRockType]
		fallingRockType = (fallingRockType + 1) % len(rocks)
		coord := utils.Coordinate{X: 2, Y: highestRowIndex + 3}

		// Falling loop: first move left/right, then move down
		for {
			offset := jetOffsets[jetIndex]
			if !checkCollisionSides(room, rock, coord, offset) {
				coord.X += offset
			}
			jetIndex = (jetIndex + 1) % len(jetOffsets)
			if checkCollisionDown(room, rock, coord) {
				break
			}
			coord.Y--
		}

		// Rock came to rest -> commit to current room
		rockHeight := len(rock[0])
		missingSpaceAtTop := (coord.Y + rockHeight) - highestRowIndex
		for i := 0; i < missingSpaceAtTop; i++ {
			for j := 0; j < len(room); j++ {
				room[j] = append(room[j], 0)
			}
		}

		highestRowIndex = len(room[0])

		for iCol, rockCol := range rock {
			for iRow, rockPart := range rockCol {
				if rockPart == ROCK {
					colInRoom := iCol + coord.X
					rowInRoom := iRow + coord.Y
					room[colInRoom][rowInRoom] = ROCK
				}
			}
		}

		// for iRow := coord.Y +
	}

	log.Println("Highest rock is at index", highestRowIndex)
}
