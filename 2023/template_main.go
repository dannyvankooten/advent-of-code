package main

import (
	"log"
	"os"
)

func main() {
	input, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	log.Printf("Input: %s\n", input)
}
