package main

import (
	"bufio"
	"fass/tools/dijkstra"
	"log"
	"os"
	"strconv"
	"strings"
)

type coordinate struct {
	x, y, z int
}

type airTile struct {
	node      *dijkstra.Node
	nodeIndex int
}

func main() {

	args := os.Args[1:]
	input := "input16.txt"
	if len(args) >= 1 {
		input = args[0]
	}

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}
	scanner := bufio.NewScanner(f)

	max := 0
	droplets := make([]coordinate, 0)
	for scanner.Scan() {
		coords := strings.Split(scanner.Text(), ",")
		if len(coords) != 3 {
			continue
		}
		x, _ := strconv.Atoi(coords[0])
		y, _ := strconv.Atoi(coords[1])
		z, _ := strconv.Atoi(coords[2])
		if x+1 > max {
			max = x + 1
		}
		if y+1 > max {
			max = y + 1
		}
		if z+1 > max {
			max = z + 1
		}
		droplets = append(droplets, coordinate{x + 1, y + 1, z + 1})

	}

	gridSize := max + 2
	grid := make([][][]bool, gridSize)
	for x := range grid {
		grid[x] = make([][]bool, gridSize)
		for y := range grid[x] {
			grid[x][y] = make([]bool, gridSize)
		}
	}

	for _, droplet := range droplets {
		grid[droplet.x][droplet.y][droplet.z] = true
	}

	log.Println("Parsed", len(droplets), "droplets")
	log.Println("Grid size is", gridSize)

	numFreeSides := 0

	for _, droplet := range droplets {
		uncoveredSides := 6
		if grid[droplet.x+1][droplet.y][droplet.z] {
			uncoveredSides--
		}
		if grid[droplet.x-1][droplet.y][droplet.z] {
			uncoveredSides--
		}
		if grid[droplet.x][droplet.y+1][droplet.z] {
			uncoveredSides--
		}
		if grid[droplet.x][droplet.y-1][droplet.z] {
			uncoveredSides--
		}
		if grid[droplet.x][droplet.y][droplet.z+1] {
			uncoveredSides--
		}
		if grid[droplet.x][droplet.y][droplet.z-1] {
			uncoveredSides--
		}
		numFreeSides += uncoveredSides
	}
	log.Println("Total number of open sides is", numFreeSides)

	pathNetwork := dijkstra.Network{
		Nodes: make([]*dijkstra.Node, 0, len(grid)*len(grid)*len(grid)),
		Edges: make([]*dijkstra.Edge, 0, len(grid)*len(grid)*len(grid)),
	}
	nodeMap := make(map[coordinate]airTile)
	nodeMapReverse := make(map[*dijkstra.Node]coordinate)
	iNode := 0
	for x := 0; x < len(grid); x++ {
		for y := 0; y < len(grid); y++ {
			for z := 0; z < len(grid); z++ {
				if !grid[x][y][z] {
					n := dijkstra.Node{
						EdgesOut: make([]*dijkstra.Edge, 0, 6),
					}
					nodeMap[coordinate{x, y, z}] = airTile{&n, iNode}
					nodeMapReverse[&n] = coordinate{x, y, z}
					pathNetwork.Nodes = append(pathNetwork.Nodes, &n)
					iNode++
				}
			}
		}
	}
	for x := 0; x < len(grid); x++ {
		for y := 0; y < len(grid); y++ {
			for z := 0; z < len(grid); z++ {
				if !grid[x][y][z] {
					n := nodeMap[coordinate{x, y, z}].node
					if x > 0 && !grid[x-1][y][z] {
						e := dijkstra.Edge{
							Weight: 1,
							Nodes:  [2]*dijkstra.Node{n, nodeMap[coordinate{x - 1, y, z}].node},
						}
						n.EdgesOut = append(n.EdgesOut, &e)
					}
					if x+1 < len(grid) && !grid[x+1][y][z] {
						e := dijkstra.Edge{
							Weight: 1,
							Nodes:  [2]*dijkstra.Node{n, nodeMap[coordinate{x + 1, y, z}].node},
						}
						n.EdgesOut = append(n.EdgesOut, &e)
					}
					if y > 0 && !grid[x][y-1][z] {
						e := dijkstra.Edge{
							Weight: 1,
							Nodes:  [2]*dijkstra.Node{n, nodeMap[coordinate{x, y - 1, z}].node},
						}
						n.EdgesOut = append(n.EdgesOut, &e)
					}
					if y+1 < len(grid) && !grid[x][y+1][z] {
						e := dijkstra.Edge{
							Weight: 1,
							Nodes:  [2]*dijkstra.Node{n, nodeMap[coordinate{x, y + 1, z}].node},
						}
						n.EdgesOut = append(n.EdgesOut, &e)
					}
					if z > 0 && !grid[x][y][z-1] {
						e := dijkstra.Edge{
							Weight: 1,
							Nodes:  [2]*dijkstra.Node{n, nodeMap[coordinate{x, y, z - 1}].node},
						}
						n.EdgesOut = append(n.EdgesOut, &e)
					}
					if z+1 < len(grid) && !grid[x][y][z+1] {
						e := dijkstra.Edge{
							Weight: 1,
							Nodes:  [2]*dijkstra.Node{n, nodeMap[coordinate{x, y, z + 1}].node},
						}
						n.EdgesOut = append(n.EdgesOut, &e)
					}
				}
			}
		}
	}

	outNode := nodeMap[coordinate{0, 0, 0}]
	dists, _ := dijkstra.Dijkstra(&pathNetwork, outNode.nodeIndex)
	for n, d := range dists {
		if d == -1 {
			c := nodeMapReverse[n]
			grid[c.x][c.y][c.z] = true
		}
	}

	// for x := 0; x < len(grid); x++ {
	// 	for y := 0; y < len(grid); y++ {
	// 		for z := 0; z < len(grid); z++ {
	// 			if !grid[x][y][z] {
	// 				n := nodeMap[coordinate{x, y, z}]
	// 				dists, _ := dijkstra.Dijkstra(&pathNetwork, n.nodeIndex)
	// 				if dists[outNode.node] == -1 {
	// 					grid[x][y][z] = true
	// 					for n2, d2 := range dists {
	// 						if d2 > 0 {
	// 							c2 := nodeMapReverse[n2]
	// 							grid[c2.x][c2.y][c2.z] = true
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	numFreeSidesNoPockets := 0

	for _, droplet := range droplets {
		uncoveredSides := 6
		if grid[droplet.x+1][droplet.y][droplet.z] {
			uncoveredSides--
		}
		if grid[droplet.x-1][droplet.y][droplet.z] {
			uncoveredSides--
		}
		if grid[droplet.x][droplet.y+1][droplet.z] {
			uncoveredSides--
		}
		if grid[droplet.x][droplet.y-1][droplet.z] {
			uncoveredSides--
		}
		if grid[droplet.x][droplet.y][droplet.z+1] {
			uncoveredSides--
		}
		if grid[droplet.x][droplet.y][droplet.z-1] {
			uncoveredSides--
		}
		numFreeSidesNoPockets += uncoveredSides
	}
	log.Println("Total number of open sides without pockets is", numFreeSidesNoPockets)

}
