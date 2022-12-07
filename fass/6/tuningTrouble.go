package main

import (
	"log"
	"os"
	"strconv"
)

func max(a int, b int) int {
	if a >= b {
		return a
	}
	return b
}

func main() {
	args := os.Args[1:]
	input := args[0]
	bufferSize, err := strconv.ParseInt(args[1], 10, 32)
	if err != nil {
		log.Fatal("Please provide buffer size as second cmd-line argument")
	}

	f, err := os.Open(input)
	if err != nil {
		log.Fatal(err)
	}
	// Close the file after function completes
	defer f.Close()

	var markerBuffer []byte = make([]byte, bufferSize)
	var cursor int64 = 0

	for {
		numRead, err := f.ReadAt(markerBuffer[:], cursor)
		if err != nil {
			log.Fatal(err)
		}

		foundMatch := false

		for i := 0; i < numRead-1; i++ {
			// Last character can be excluded, since it is checked against all others anyway
			char := markerBuffer[i]
			for j := i + 1; j < numRead; j++ {
				if char == markerBuffer[j] {
					foundMatch = true
					break
				}
			}
			if foundMatch {
				break
			}
		}

		// If no match found -> This is a marker,
		if !foundMatch {
			break
		}
		cursor++
	}

	log.Println("Found", bufferSize, "different characters after reading", cursor+bufferSize, "characters.")
}
