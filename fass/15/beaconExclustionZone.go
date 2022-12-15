package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"regexp"
	"strconv"
)

type coordinate struct {
	x, y int
}

type nullableInt struct {
	hasValue bool
	value    int
}

type sensor struct {
	location *coordinate
	radius   int
}

type intRange struct {
	min, max int
}

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func dist(c1, c2 coordinate) int {
	return abs(c1.x-c2.x) + abs(c1.y-c2.y)
}

func overlapsOrTouches(r1, r2 intRange) bool {
	if r1.max < r2.min-1 {
		return false // Range is lower
	}
	if r1.min > r2.max+1 {
		return false // Range is higher
	}
	return true // Neither higher or lower -> Overlap
}

func combineIfPossible(r1, r2 intRange) (bool, *intRange) {
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

func getCoveredRange(sensors []*sensor, searchY int) []intRange {

	coveredRanges := make([]intRange, 0, len(sensors))

	for _, sens := range sensors {
		closestOnLine := coordinate{sens.location.x, searchY}
		delta := sens.radius - dist(closestOnLine, *sens.location)
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
				overlaps, combined := combineIfPossible(toMerge, coveredRangesCombined[i])
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
	input := "./testinput14.txt"
	if len(args) >= 1 {
		input = args[0]
	}
	searchY := 10
	if len(args) >= 2 {
		searchY, _ = strconv.Atoi(args[1])
	}
	maxSearchI := 20
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

		sensors = append(sensors, &sensor{location: &sensorLoc, radius: dist(sensorLoc, beaconLoc)})
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
	var lostBeaconLoc *coordinate
	for i := 0; i <= maxSearchI; i++ {
		allCovered := getCoveredRange(sensors, i)
		for _, covered := range allCovered {
			if covered.max > 0 && covered.max < maxSearchI {
				lostBeaconLoc = &coordinate{covered.max + 1, i}
				break
			}
		}
	}
	if lostBeaconLoc == nil {
		log.Fatal("Did not find lost beacon :(")
	}

	log.Println("Found lost beacon at", *lostBeaconLoc)
	log.Println("Tuning frequency:", int64(lostBeaconLoc.x)*4000000+int64(lostBeaconLoc.y))
}
