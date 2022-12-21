package dijkstra

import "log"

type Node struct {
	EdgesOut []*Edge
	EdgesIn  []*Edge
}

func AreConnected(nodeFrom, nodeTo *Node) bool {
	for _, e := range nodeFrom.EdgesOut {
		for _, n := range e.Nodes {
			if n == nodeTo {
				return true
			}
		}
	}
	return false
}

type Edge struct {
	Weight int
	Nodes  [2]*Node
}

type Network struct {
	Nodes []*Node
	Edges []*Edge
}

func Dijkstra(network *Network, startNodeIndex int) (map[*Node]int, map[*Node]*Node) {
	distances := make(map[*Node]int, len(network.Nodes))
	predecessors := make(map[*Node]*Node, len(network.Nodes))
	workingSet := make([]*Node, len(network.Nodes))

	for i := 0; i < len(network.Nodes); i++ {
		distances[network.Nodes[i]] = -1
		workingSet[i] = network.Nodes[i]
	}

	distances[network.Nodes[startNodeIndex]] = 0

	for len(workingSet) > 0 {
		// log.Println(len(workingSet))
		nodeDistance := -1
		nodeIndex := -1
		for i := 0; i < len(workingSet); i++ {
			dist := distances[workingSet[i]]
			if dist == -1 {
				continue
			}
			if nodeDistance == -1 || dist < nodeDistance {
				nodeIndex = i
				nodeDistance = dist
			}
		}

		if nodeIndex == -1 {
			// log.Println("Breaking since all remaining nodes can not be reached from the given start point")
			break
		}

		node := workingSet[nodeIndex]
		workingSet[nodeIndex] = workingSet[len(workingSet)-1]
		workingSet = workingSet[:len(workingSet)-1]
		// log.Println("Length of working set is now", len(workingSet))

		for _, edge := range node.EdgesOut {
			var neighborNode *Node
			if edge.Nodes[0] == node {
				neighborNode = edge.Nodes[1]
			} else if edge.Nodes[1] == node {
				neighborNode = edge.Nodes[0]
			} else {
				log.Fatal("Node connected to an edge which is not connected to that same node. Fix the network.")
			}
			if neighborNode == nil {
				log.Fatal("Edge connected to only one node. Fix the network")
			}

			inWorkingSet := false
			for _, compareNode := range workingSet {
				if compareNode == neighborNode {
					inWorkingSet = true
					break
				}
			}

			if inWorkingSet {
				// distance at node is never "infinity" (represented as -1) since we only select non-infinity-distance nodes
				if nodeDistance == -1 {
					log.Fatal("Encountered node which has own distance of infinity")
				}
				alternativeDist := nodeDistance + edge.Weight
				neighborDist := distances[neighborNode]
				if neighborDist == -1 || alternativeDist < neighborDist {
					distances[neighborNode] = alternativeDist
					predecessors[neighborNode] = node
				}
			}
		}
	}
	return distances, predecessors
}
