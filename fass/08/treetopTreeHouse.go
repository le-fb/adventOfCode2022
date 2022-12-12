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

type tree struct {
	height       int
	visible      bool
	viewingScore int
}

func main() {
	args := os.Args[1:]
	input := args[0]

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	// Close the file after function completes
	defer f.Close()

	scanner := bufio.NewScanner(f)
	var grid [][]tree

	init := true
	for scanner.Scan() {
		if init {
			grid = make([][]tree, len(scanner.Text()))
			for i := 0; i < len(scanner.Text()); i++ {
				grid[i] = make([]tree, 0)
			}
			init = false
		}
		for colIndex, char := range scanner.Text() {
			height, err := strconv.ParseInt(string(char), 10, 32)
			if err != nil {
				log.Fatal(err)
			}
			grid[colIndex] = append(grid[colIndex], tree{height: int(height), visible: false, viewingScore: 1})
		}
	}

	if scanner.Err() != nil {
		log.Fatal(scanner.Err())
	}

	for colIndex := 0; colIndex < len(grid); colIndex++ {
		curMax := -1
		curMaxIndex := -1
		col := grid[colIndex]

		for rowIndex := 0; rowIndex < len(col); rowIndex++ {
			tree := &col[rowIndex]
			if tree.height > curMax {
				tree.visible = true
				curMax = tree.height
				curMaxIndex = rowIndex
			}
		}
		curMax = -1
		// Tree at curMaxIndex is heighest when viewed from other direction -> only search up to that point (not including)
		for rowIndex := len(col) - 1; rowIndex > curMaxIndex; rowIndex-- {
			tree := &col[rowIndex]
			if tree.height > curMax {
				tree.visible = true
				curMax = tree.height
			}
		}
	}

	for rowIndex := 0; rowIndex < len(grid[0]); rowIndex++ {
		curMax := -1
		curMaxIndex := -1
		for colIndex := 0; colIndex < len(grid); colIndex++ {
			tree := &grid[colIndex][rowIndex]
			if tree.height > curMax {
				tree.visible = true
				curMax = tree.height
				curMaxIndex = colIndex
			}
		}
		curMax = -1
		for colIndex := len(grid) - 1; colIndex > curMaxIndex; colIndex-- {
			tree := &grid[colIndex][rowIndex]
			if tree.height > curMax {
				tree.visible = true
				curMax = tree.height
			}
		}
	}

	// log.Println(grid)

	numVisible := 0
	maxScenic := -1
	for colIndex, col := range grid {
		for rowIndex := 0; rowIndex < len(col); rowIndex++ {
			tree := &col[rowIndex]
			if tree.visible {
				numVisible++
			}
			if colIndex == 0 || rowIndex == 0 {
				tree.viewingScore = 0
				continue
			}
			curCoordinate := coordinate{row: rowIndex, col: colIndex - 1}
			for curCoordinate.col > 0 {
				if grid[curCoordinate.col][curCoordinate.row].height >= tree.height {
					break
				}
				curCoordinate.col--
			}
			rightScore := colIndex - curCoordinate.col
			curCoordinate = coordinate{row: rowIndex, col: colIndex + 1}
			for curCoordinate.col < len(col)-1 {
				if grid[curCoordinate.col][curCoordinate.row].height >= tree.height {
					break
				}
				curCoordinate.col++
			}
			leftScore := curCoordinate.col - colIndex
			curCoordinate = coordinate{row: rowIndex - 1, col: colIndex}
			for curCoordinate.row > 0 {
				if grid[curCoordinate.col][curCoordinate.row].height >= tree.height {
					break
				}
				curCoordinate.row--
			}
			upScore := rowIndex - curCoordinate.row
			curCoordinate = coordinate{row: rowIndex + 1, col: colIndex}
			for curCoordinate.row < len(grid)-1 {
				if grid[curCoordinate.col][curCoordinate.row].height >= tree.height {
					break
				}
				curCoordinate.row++
			}
			downScore := curCoordinate.row - rowIndex
			viewingScore := upScore * downScore * leftScore * rightScore
			tree.viewingScore = viewingScore
			if viewingScore > maxScenic {
				maxScenic = viewingScore
			}
		}
	}

	log.Println("The number of visible trees is", numVisible)
	log.Println("The highest possible scenic score is", maxScenic)
}
