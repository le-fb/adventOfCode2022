package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"regexp"
	"sort"
	"strconv"
)

type coordinate struct {
	x, y int
}

type sensor struct {
	location *coordinate
	radius   int
}

// for empty range use min = 0, max = -1
type intRange struct {
	min, max int
}
type yIntRange struct {
	y int
	r *intRange
}

func isEmpty(r *intRange) bool {
	return r.min == 0 && r.max == -1
}

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func dist(c1, c2 *coordinate) int {
	return abs(c1.x-c2.x) + abs(c1.y-c2.y)
}

func overlapsOrTouches(r1, r2 *intRange) bool {
	if r1.max < r2.min-1 {
		return false // Range is lower
	}
	if r1.min > r2.max+1 {
		return false // Range is higher
	}
	return true // Neither higher or lower -> Overlap
}

func combineIfPossible(r1, r2 *intRange) (bool, *intRange) {
	if !overlapsOrTouches(r1, r2) {
		return false, nil
	}

	result := intRange{}
	if r1.min < r2.min {
		result.min = r1.min
	} else {
		result.min = r2.min
	}

	if r1.max > r2.max {
		result.max = r1.max
	} else {
		result.max = r2.max
	}

	return true, &result
}

func union(r1, r2 *intRange) *intRange {
	if !overlapsOrTouches(r1, r2) {
		return &intRange{min: 0, max: -1}
	}

	var min, max int
	if r1.min > r2.min {
		min = r1.min
	} else {
		min = r2.min
	}
	if r1.max < r2.max {
		max = r1.max
	} else {
		max = r2.max
	}
	return &intRange{min, max}
}

func getCoveredRange(sensors []*sensor, searchY int) []intRange {

	coveredRanges := make([]intRange, 0, len(sensors))

	for _, sens := range sensors {
		closestOnLine := coordinate{sens.location.x, searchY}
		delta := sens.radius - dist(&closestOnLine, sens.location)
		if delta < 0 {
			continue
		}
		coveredRanges = append(coveredRanges, intRange{sens.location.x - delta, sens.location.x + delta})
	}

	// log.Println("Covered ranges:")
	// for _, covered := range coveredRanges {
	// 	fmt.Println(covered.min, "to", covered.max)
	// }

	coveredRangesCombined := make([]intRange, 0, len(coveredRanges))
	for _, toMerge := range coveredRanges {
		mergeComplete := false
		for !mergeComplete {
			mergeComplete = true
			for i := 0; i < len(coveredRangesCombined); i++ {
				overlaps, combined := combineIfPossible(&toMerge, &coveredRangesCombined[i])
				if !overlaps {
					continue
				}
				mergeComplete = false
				toMerge = *combined
				// remove merged range
				coveredRangesCombined[i] = coveredRangesCombined[len(coveredRangesCombined)-1]
				coveredRangesCombined = coveredRangesCombined[:len(coveredRangesCombined)-1]
				i--
			}
		}
		coveredRangesCombined = append(coveredRangesCombined, toMerge)
	}

	return coveredRangesCombined
}

func main() {

	args := os.Args[1:]
	input := "./input15.txt"
	if len(args) >= 1 {
		input = args[0]
	}
	searchY := 2000000
	if len(args) >= 2 {
		searchY, _ = strconv.Atoi(args[1])
	}
	maxSearchI := 4000000
	if len(args) >= 3 {
		maxSearchI, _ = strconv.Atoi(args[2])
	}

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}

	scanner := bufio.NewScanner(f)
	sensors := make([]*sensor, 0)
	beacons := make([]*coordinate, 0)
	xCoordRegex, _ := regexp.Compile("x=-?[0-9]+")
	yCoordRegex, _ := regexp.Compile("y=-?[0-9]+")

	for scanner.Scan() {
		xCoords := xCoordRegex.FindAllString(scanner.Text(), 2)
		yCoords := yCoordRegex.FindAllString(scanner.Text(), 2)

		sensorX, _ := strconv.Atoi(xCoords[0][2:])
		sensorY, _ := strconv.Atoi(yCoords[0][2:])
		beaconX, _ := strconv.Atoi(xCoords[1][2:])
		beaconY, _ := strconv.Atoi(yCoords[1][2:])
		sensorLoc := coordinate{sensorX, sensorY}
		beaconLoc := coordinate{beaconX, beaconY}

		if len(xCoords) < 2 || len(yCoords) < 2 {
			continue
		}

		sensors = append(sensors, &sensor{location: &sensorLoc, radius: dist(&sensorLoc, &beaconLoc)})
		beaconInList := false
		for _, bLoc := range beacons {
			if *bLoc == beaconLoc {
				beaconInList = true
				break
			}
		}
		if !beaconInList {
			beacons = append(beacons, &beaconLoc)
		}
	}

	log.Println("Sensors:")
	for _, sens := range sensors {
		fmt.Println(sens.location, "r:", sens.radius)
	}

	log.Println("Beacons:")
	for _, beac := range beacons {
		fmt.Println(beac)
	}

	targetLineCombined := getCoveredRange(sensors, searchY)
	log.Println("Covered ranges combined:")
	totalCovered := 0
	for _, covered := range targetLineCombined {
		fmt.Println(covered.min, "to", covered.max)
		totalCovered += covered.max - covered.min
	}

	log.Println("Total distance covered by sensors on line", searchY, ":", totalCovered)

	searchRange := intRange{0, maxSearchI}
	coveredInSearchSpace := make([][]*intRange, maxSearchI+1)

	for i := 0; i <= maxSearchI; i++ {
		allCoveredInLine := getCoveredRange(sensors, i)
		for _, covered := range allCoveredInLine {
			coveredInSearchRange := union(&searchRange, &covered)
			if isEmpty(coveredInSearchRange) {
				continue
			}
			coveredInSearchSpace[i] = append(coveredInSearchSpace[i], coveredInSearchRange)
		}
	}

	uncoveredInSearchSpace := make([]*yIntRange, 0)

	for i := 0; i < maxSearchI; i++ {
		compareList := coveredInSearchSpace[i]
		if len(compareList) == 0 {
			uncoveredInSearchSpace = append(uncoveredInSearchSpace, &yIntRange{i, &searchRange})
		}
		if len(compareList) == 1 && *compareList[0] == searchRange {
			continue
		}

		sort.Slice(compareList, func(i, j int) bool { return compareList[i].min < compareList[j].min })
		cursor := searchRange.min
		for _, compare := range compareList {
			if cursor < compare.min {
				uncoveredInSearchSpace = append(uncoveredInSearchSpace, &yIntRange{i, &intRange{cursor, compare.min}})
			}
			cursor = compare.max + 1
		}

		if cursor <= searchRange.max {
			uncoveredInSearchSpace = append(uncoveredInSearchSpace, &yIntRange{i, &intRange{cursor, searchRange.max}})
		}
	}

	var lostBeaconLoc *coordinate
	if len(uncoveredInSearchSpace) != 1 && uncoveredInSearchSpace[0].r.min != uncoveredInSearchSpace[0].r.max {
		log.Println("Did not find lost beacon :(")
	} else {
		lostBeaconLoc = &coordinate{uncoveredInSearchSpace[0].r.max, uncoveredInSearchSpace[0].y}
		log.Println("Found lost beacon at", *lostBeaconLoc)
		log.Println("Tuning frequency:", int64(lostBeaconLoc.x)*4000000+int64(lostBeaconLoc.y))
	}

	log.Println("Found", len(uncoveredInSearchSpace), "uncovered ranges in the search space ( <=", maxSearchI, "):")

	for _, r := range uncoveredInSearchSpace {
		log.Println("y:", r.y, "x:(", r.r.min, "to", r.r.max, ")")
	}

}
