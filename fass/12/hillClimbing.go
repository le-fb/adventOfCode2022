package main

import (
	"bufio"
	"hillClimbing/dijkstra"
	"log"
	"os"
	"strings"
)

func main() {
	args := os.Args[1:]
	input := args[0]

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	scanner := bufio.NewScanner(f)

	heightGrid := make([][]rune, 0)

	var startRow, startCol int = -1, -1
	var targetRow, targetCol int = -1, -1
	pathNetwork := dijkstra.Network{
		Nodes: make([]*dijkstra.Node, 0),
		Edges: make([]*dijkstra.Edge, 0),
	}

	rowIndex := 0
	for scanner.Scan() {
		trimmedLine := strings.TrimSpace(scanner.Text())
		if trimmedLine == "" {
			continue
		}
		heightGrid = append(heightGrid, make([]rune, len(trimmedLine)))
		for colIndex, char := range trimmedLine {
			if char == 'S' {
				startRow, startCol = rowIndex, colIndex
				char = 'a'
			}
			if char == 'E' {
				targetRow, targetCol = rowIndex, colIndex
				char = 'z'
			}
			heightGrid[rowIndex][colIndex] = char
			node := dijkstra.Node{
				EdgesIn:  make([]*dijkstra.Edge, 0),
				EdgesOut: make([]*dijkstra.Edge, 0),
			}
			pathNetwork.Nodes = append(pathNetwork.Nodes, &node)
		}
		rowIndex++
	}

	if startRow == -1 || startCol == -1 || targetRow == -1 || targetCol == -1 {
		log.Fatal("Error during parsing. Start and/or end could not be determined")
	}
	log.Println("Finshed parsing input. Start point: (", startRow, startCol, "), End point: (", targetRow, targetCol, ")")
	log.Println("Now building network...")

	toSingleIndex := func(row, col int) int {
		return row*len(heightGrid[0]) + col
	}

	connectIfNecessary := func(row1, col1, row2, col2 int) {
		height1 := heightGrid[row1][col1]
		height2 := heightGrid[row2][col2]
		node1 := pathNetwork.Nodes[toSingleIndex(row1, col1)]
		node2 := pathNetwork.Nodes[toSingleIndex(row2, col2)]
		if height1+1 >= height2 { // h2 is 1 higher or lower than h1, so h1 -> h2 is possible
			if !dijkstra.AreConnected(node1, node2) {
				edge := dijkstra.Edge{Weight: 1}
				edge.Nodes[0] = node1
				edge.Nodes[1] = node2
				node1.EdgesOut = append(node1.EdgesOut, &edge)
				node2.EdgesIn = append(node2.EdgesIn, &edge)
				pathNetwork.Edges = append(pathNetwork.Edges, &edge)
			}
		}
		if height2+1 >= height1 {
			if !dijkstra.AreConnected(node2, node1) {
				edge := dijkstra.Edge{Weight: 1}
				edge.Nodes[0] = node1
				edge.Nodes[1] = node2
				node1.EdgesIn = append(node1.EdgesIn, &edge)
				node2.EdgesOut = append(node2.EdgesOut, &edge)
				pathNetwork.Edges = append(pathNetwork.Edges, &edge)
			}
		}
	}

	for row := 0; row < len(heightGrid); row++ {
		for col := 0; col < len(heightGrid[row]); col++ {
			// Check up
			if row > 0 {
				connectIfNecessary(row, col, row-1, col)
			}
			// Check right
			if col < len(heightGrid[row])-1 {
				connectIfNecessary(row, col, row, col+1)
			}
			// Check down
			if row < len(heightGrid)-1 {
				connectIfNecessary(row, col, row+1, col)
			}
			// Check left
			if col > 0 {
				connectIfNecessary(row, col, row, col-1)
			}
		}
	}

	log.Println("Finshed building network. Total number of nodes:", len(pathNetwork.Nodes), "Total number of edges:", len(pathNetwork.Edges))
	log.Println("Starting Dijkstra to determine shortest path...")

	distances, _ := dijkstra.Dijkstra(&pathNetwork, toSingleIndex(startRow, startCol))
	log.Println("Dijkstra algorithm completed.")

	// for col := 0; col < len(heightGrid[0]); col++ {
	// 	fmt.Printf("%3d", col)
	// 	fmt.Print(",")
	// }
	// fmt.Println()
	// for row := 0; row < len(heightGrid); row++ {
	// 	for col := 0; col < len(heightGrid[row]); col++ {
	// 		fmt.Printf("%3d", distances[pathNetwork.Nodes[toSingleIndex(row, col)]])
	// 		fmt.Print(",")
	// 	}
	// 	fmt.Println()
	// }

	// startNode := pathNetwork.Nodes[toSingleIndex(startRow, startCol)]
	endNode := pathNetwork.Nodes[toSingleIndex(targetRow, targetCol)]
	minDistance := distances[endNode]
	log.Println("Distance from original start point is", minDistance)
	numChecked := 0
	minRow, minCol := startRow, startCol

	for row := 0; row < len(heightGrid); row++ {
		for col := 0; col < len(heightGrid[row]); col++ {
			if heightGrid[row][col] == 'a' {
				distances, _ := dijkstra.Dijkstra(&pathNetwork, toSingleIndex(row, col))
				if distances[endNode] != -1 && distances[endNode] < minDistance {
					minDistance = distances[endNode]
					minRow, minCol = row, col
				}
				numChecked++
				if numChecked%100 == 0 {
					log.Println("Checked", numChecked, "possible starting points")
				}
			}
		}
	}

	log.Println("After checking", numChecked, "possible starting points the point with the shortest path to the target is at",
		"(", minRow, minCol, "), with distance", minDistance)

}
