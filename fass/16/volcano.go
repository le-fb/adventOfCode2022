package main

import (
	"bufio"
	"fass/tools/dijkstra"
	"log"
	"os"
	"regexp"
	"strconv"
	"strings"
)

type valve struct {
	uuid                   string
	flowRate               int
	node, nodeCondensed    *dijkstra.Node
	networkNodeID          int
	networkCondensedNodeID int
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

	nodesToValves := make(map[*dijkstra.Node]*valve, 0)
	valves := make(map[string]*valve, 0)
	notZeroFlow := make([]string, 0)

	largeNetwork := dijkstra.Network{
		Nodes: make([]*dijkstra.Node, 0),
		Edges: make([]*dijkstra.Edge, 0),
	}

	nodeID := 0
	flowRateRegex, _ := regexp.Compile("rate=-?[0-9]+")
	for scanner.Scan() {
		words := strings.Split(scanner.Text(), " ")
		valveID := words[1]
		sourceValve, ok := valves[valveID]
		if !ok {
			sourceNode := &dijkstra.Node{EdgesOut: make([]*dijkstra.Edge, 0)}
			sourceValve = &valve{node: sourceNode, networkNodeID: nodeID, uuid: valveID}
			valves[valveID] = sourceValve
			nodesToValves[sourceNode] = sourceValve
			largeNetwork.Nodes = append(largeNetwork.Nodes, sourceNode)
			nodeID++
		}
		sourceNode := sourceValve.node

		// first target valve is at word 9
		for _, word := range words[9:] {
			targetID := strings.Trim(word, " ,")
			targetValve, ok := valves[targetID]
			if !ok {
				targetNode := &dijkstra.Node{EdgesOut: make([]*dijkstra.Edge, 0)}
				targetValve = &valve{node: targetNode, networkNodeID: nodeID, uuid: targetID}
				valves[targetID] = targetValve
				nodesToValves[targetNode] = targetValve
				largeNetwork.Nodes = append(largeNetwork.Nodes, targetNode)
				nodeID++
			}
			targetNode := targetValve.node
			edge := &dijkstra.Edge{Weight: 1, Nodes: [2]*dijkstra.Node{sourceNode, targetNode}}
			sourceNode.EdgesOut = append(sourceNode.EdgesOut, edge)
			largeNetwork.Edges = append(largeNetwork.Edges, edge)
		}

		flowRate, _ := strconv.Atoi(flowRateRegex.FindString(scanner.Text())[5:])
		sourceValve.flowRate = flowRate
		if flowRate != 0 {
			notZeroFlow = append(notZeroFlow, valveID)
		}
	}

	// Build new condensed network
	distancesFromStart, _ := dijkstra.Dijkstra(&largeNetwork, valves["AA"].networkNodeID)

	condensedNetwork := dijkstra.Network{Nodes: make([]*dijkstra.Node, len(notZeroFlow)+1), Edges: make([]*dijkstra.Edge, 0)}
	startNode := dijkstra.Node{EdgesOut: make([]*dijkstra.Edge, 0)}
	condensedNetwork.Nodes[len(notZeroFlow)] = &startNode
	valves["AA"].networkCondensedNodeID = 0
	valves["AA"].nodeCondensed = &startNode
	nodesToValves[&startNode] = valves["AA"]
	for i := 0; i < len(notZeroFlow); i++ {
		nodeCondensed := &dijkstra.Node{EdgesOut: make([]*dijkstra.Edge, 0)}
		condensedNetwork.Nodes[i] = nodeCondensed
		valves[notZeroFlow[i]].networkCondensedNodeID = i
		valves[notZeroFlow[i]].nodeCondensed = nodeCondensed
		nodesToValves[nodeCondensed] = valves[notZeroFlow[i]]
	}

	for i := 0; i < len(notZeroFlow); i++ {
		ownNode := condensedNetwork.Nodes[i]
		startDist := distancesFromStart[valves[notZeroFlow[i]].node]
		startEdge := &dijkstra.Edge{Weight: startDist, Nodes: [2]*dijkstra.Node{&startNode, ownNode}}
		startNode.EdgesOut = append(startNode.EdgesOut, startEdge)
		dists, _ := dijkstra.Dijkstra(&largeNetwork, valves[notZeroFlow[i]].networkNodeID)

		for j := i + 1; j < len(notZeroFlow); j++ {
			nodeJ := condensedNetwork.Nodes[j]
			dist := dists[valves[notZeroFlow[j]].node]
			edge := &dijkstra.Edge{Weight: dist, Nodes: [2]*dijkstra.Node{nodeJ, ownNode}}
			condensedNetwork.Edges = append(condensedNetwork.Edges, edge)
			ownNode.EdgesOut = append(ownNode.EdgesOut, edge)
			nodeJ.EdgesOut = append(nodeJ.EdgesOut, edge)
		}
	}

	maxScore := valves["AA"].getMaxScore(make([]string, 0), 0, 30, nodesToValves)
	log.Println("Max score possible in 30 turns is", maxScore)

	maxScore2 := getMaxScoreMultiAgent([]*valve{valves["AA"], valves["AA"]}, make([]string, 0), []int{0, 0}, 26, nodesToValves, 0)
	log.Println("Max score possible with 2 agents in 26 turns is", maxScore2)
}

func (v valve) getMaxScore(exclusionIDs []string, startTurnIndex, maxTurns int, nodesToValves map[*dijkstra.Node]*valve) int {
	vNode := v.nodeCondensed
	// for turnIndex 29 and maxTurns 30 (last turn) one turn left
	// This also catches the turn limit conditions
	turnsLeft := (maxTurns - startTurnIndex)
	if turnsLeft < 1 {
		return 0
	}
	if turnsLeft == 1 {
		return v.flowRate
	}
	ownScore := turnsLeft * v.flowRate
	maxChildScore := 0
	for _, edge := range vNode.EdgesOut {

		otherNode := edge.Nodes[0]
		if otherNode == vNode {
			otherNode = edge.Nodes[1]
		}
		otherValve := nodesToValves[otherNode]

		skip := false
		for _, exclude := range exclusionIDs {
			if exclude == otherValve.uuid {
				skip = true
				break
			}
		}
		if skip {
			continue
		}

		// Increase turn index by 1 (opening child valve) + weigh (num turns to move there)
		childTurnIndex := startTurnIndex + 1 + edge.Weight
		childScore := otherValve.getMaxScore(append(exclusionIDs, v.uuid), childTurnIndex, maxTurns, nodesToValves)
		if childScore > maxChildScore {
			maxChildScore = childScore
		}
	}

	return ownScore + maxChildScore
}

func getMaxScoreMultiAgent(curValves []*valve, exclusionIDs []string, startTurnIndexes []int, maxTurns int, nodesToValves map[*dijkstra.Node]*valve, depth int) int {
	// for turnIndex 29 and maxTurns 30 (last turn) one turn left
	// This also catches the turn limit conditions

	if depth < 2 {
		toCheckEstimate := 1
		for i := 0; i < len(curValves); i++ {
			toCheckEstimate *= len(curValves[i].nodeCondensed.EdgesOut)
		}

		log.Println("Starting node at depth", depth, "Estimating total to check:", toCheckEstimate)
	}
	ownScore := 0
	anyLeft := false
	for i := 0; i < len(startTurnIndexes); i++ {
		startTurnIndex := startTurnIndexes[i]
		turnsLeft := (maxTurns - startTurnIndex)
		if turnsLeft < 1 {
			continue
		}
		anyLeft = true
		ownScore += turnsLeft * curValves[i].flowRate
	}
	if !anyLeft {
		return ownScore
	}
	maxChildScore := 0
	curEdgeIndices := make([]int, len(curValves))

	incIndices := func() {
		curEdgeIndices[0]++
		for i := 0; i < len(curEdgeIndices)-1; i++ {
			if curEdgeIndices[i] == len(curValves[i].nodeCondensed.EdgesOut) {
				curEdgeIndices[i] = 0
				curEdgeIndices[i+1]++
			}
		}
	}
	if depth == 0 {
		incIndices = func() {
			curEdgeIndices[0]++
			for i := 0; i < len(curEdgeIndices)-1; i++ {
				if curEdgeIndices[i] == len(curValves[i].nodeCondensed.EdgesOut) {
					curEdgeIndices[i+1]++
					curEdgeIndices[i] = curEdgeIndices[i+1]
				}
			}
		}
	}

	for curEdgeIndices[len(curEdgeIndices)-1] < len(curValves[len(curEdgeIndices)-1].nodeCondensed.EdgesOut) {
		skip := false
		nextValveParams := make([]struct {
			v          *valve
			startIndex int
		}, 0, len(curValves))

		for i, valveI := range curValves {
			if startTurnIndexes[i] >= maxTurns {
				continue
			}
			edge := valveI.nodeCondensed.EdgesOut[curEdgeIndices[i]]
			otherNode := edge.Nodes[0]
			if otherNode == valveI.nodeCondensed {
				otherNode = edge.Nodes[1]
			}
			otherValve := nodesToValves[otherNode]

			for _, exclude := range exclusionIDs {
				if exclude == otherValve.uuid {
					skip = true
					break
				}
			}
			for _, exclude := range nextValveParams {
				if otherValve == exclude.v {
					skip = true
					break
				}
			}
			if skip {
				break
			}
			// Increase turn index by 1 (opening child valve) + weight (num turns to move there)
			childTurnIndex := startTurnIndexes[i] + 1 + edge.Weight
			if childTurnIndex < maxTurns {
				nextValveParams = append(nextValveParams, struct {
					v          *valve
					startIndex int
				}{otherValve, childTurnIndex})
			}
		}

		if skip {
			incIndices()
			continue
		}
		childExclusions := exclusionIDs
		childIndices := make([]int, 0, len(nextValveParams))
		nextValves := make([]*valve, 0, len(nextValveParams))
		for _, childParams := range nextValveParams {
			if childParams.v == nil {
				continue
			}
			nextValves = append(nextValves, childParams.v)
			childExclusions = append(childExclusions, childParams.v.uuid)
			childIndices = append(childIndices, childParams.startIndex)
		}
		if len(nextValves) > 0 {
			childScore := getMaxScoreMultiAgent(nextValves, childExclusions, childIndices, maxTurns, nodesToValves, depth+1)
			if childScore > maxChildScore {
				maxChildScore = childScore
			}
		}
		incIndices()
	}
	if depth < 2 {
		log.Println("Completed node at depth", depth)
	}
	return ownScore + maxChildScore
}
