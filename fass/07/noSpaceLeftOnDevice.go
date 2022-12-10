package main

import (
	"bufio"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
)

var TotalDiskSize int = 70_000_000
var TotalRequiredFreeSpace int = 30_000_000

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
	curPathStack := []string{"/"}
	curCommand := ""

	var fileMap map[string]int64 = make(map[string]int64)

	for {
		if !scanner.Scan() {
			if scanner.Err() != nil {
				log.Fatal(scanner.Err())
			}
			break
		}

		line := scanner.Text()
		if len(line) == 0 {
			continue
		}

		lineParts := strings.Split(line, " ")
		if lineParts[0] == "$" {
			// Command found
			curCommand = lineParts[1]
			if curCommand == "cd" {
				if lineParts[2] == "/" {
					curPathStack = []string{"/"}
					continue
				} else if lineParts[2] == ".." {
					curPathStack = curPathStack[:len(curPathStack)-1]
					continue
				}
				curPathStack = append(curPathStack, curPathStack[len(curPathStack)-1]+lineParts[2]+"/")
				continue
			} else if curCommand == "ls" {
				continue
			} else {
				log.Fatal("Unknown command:", curCommand)
			}
		}

		// Parsing output of ls-command
		// Opt A: SIZE123 filename
		// Opt B: dir dirname
		if lineParts[0] == "dir" {
			continue
		}

		size, err := strconv.ParseInt(lineParts[0], 10, 32)
		if err != nil {
			log.Fatal(err)
		}
		curPath := curPathStack[len(curPathStack)-1]
		storedSize, exists := fileMap[curPath+lineParts[1]]
		if exists {
			if storedSize != size {
				log.Fatal("Found two different sizes for same file", curPath+lineParts[1], "Found Previously:", storedSize, "Now:", size)
			}
			continue
		} else {
			fileMap[curPath+lineParts[1]] = size
		}
		for i := 0; i < len(curPathStack); i++ {
			fileMap[curPathStack[i]] = fileMap[curPathStack[i]] + size
		}
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	freeSpace := TotalDiskSize - int(fileMap["/"])
	sizeToReclaim := TotalRequiredFreeSpace - freeSpace

	var deletionCandidates []string = make([]string, 0, len(fileMap))
	cutOffSizeSum := 0
	var cutoffDirSize int64 = 100_000
	for dirName, dirSize := range fileMap {
		// Ignore files
		if dirName[len(dirName)-1] != '/' {
			continue
		}

		if dirSize >= int64(sizeToReclaim) {
			deletionCandidates = append(deletionCandidates, dirName)
		}

		// Ignore large directories
		if dirSize > cutoffDirSize {
			continue
		}

		// // Check if containing directory was already considered
		// nameParts := strings.Split(dirName, "/")
		// parentDir := strings.Join(nameParts[:len(nameParts)-2], "/") + "/"

		// parentSize := fileMap[parentDir]
		// if parentSize <= cutoffDirSize {
		// 	// Already counted as part of parent
		// 	continue
		// }

		cutOffSizeSum += int(dirSize)
	}

	log.Println("Total size of all filtered directories is", cutOffSizeSum)

	sort.SliceStable(deletionCandidates, func(i, j int) bool {
		return fileMap[deletionCandidates[i]] < fileMap[deletionCandidates[j]]
	})

	log.Println("The current total occupied size is", fileMap["/"])
	log.Println("Size needed to be reclaimed:", sizeToReclaim)
	log.Println("Smallest directory large enough to free up enough space:", deletionCandidates[0], fileMap[deletionCandidates[0]])
}
