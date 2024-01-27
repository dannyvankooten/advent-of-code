package main

import (
  "fmt"
  "log"
  "os"
  "time"
  "bytes"
  "strings"
)



func findReflectionColumn(grid []string, pt2 bool) int {
  width := len(grid[0])
  height := len(grid)

  for mid := 0; mid < width-1; mid++ {
    diff := 0
    for row := 0; row < height; row++ {
      for offset := 0;; offset++ {
        leftCol := mid - offset
        rightCol := mid + offset + 1
        if leftCol < 0 || rightCol >= width {
          break
        }

        if grid[row][leftCol] != grid[row][rightCol] {
          diff++
        }
      }
    }

    if pt2 && diff == 1  {
      return mid + 1
    } else if !pt2 && diff == 0 {
      return mid + 1
    }

  }

  // no reflection across any column
  return 0
}

func findReflectionRow(grid []string, pt2 bool) int {
  width := len(grid[0])
  height := len(grid)

  for mid := 0; mid < height-1; mid++ {
    diff := 0
    for col := 0; col < width; col++ {
      for offset := 0;; offset++ {
        leftRow := mid - offset
        rightRow := mid + offset + 1
        if leftRow < 0 || rightRow >= height {
          break;
        }

        if grid[leftRow][col] != grid[rightRow][col] {
          diff++
        }
      }
    }

    if pt2 && diff == 1  {
      return mid + 1
    } else if !pt2 && diff == 0 {
      return mid + 1
    }

  }

  // no reflection across any row
  return 0
}

func main() {
  timeStart := time.Now()

  b, err := os.ReadFile("input.txt")
  if err != nil {
    log.Fatal(err)
  }

  pt1 := 0
  pt2 := 0
  sections  := strings.Split(string(bytes.TrimSpace(b)), "\n\n")
  for _, s := range sections {
    lines := strings.Split(s, "\n")
    pt1 += findReflectionColumn(lines, false) + findReflectionRow(lines, false) * 100
    pt2 += findReflectionColumn(lines, true) + findReflectionRow(lines, true) * 100
  }

  fmt.Printf("--- Day 13: Point of Incidence ---\n")
  fmt.Printf("Part 1: %d\n", pt1)
  fmt.Printf("Part 2: %d\n", pt2)
  fmt.Printf("Time: %.2fms\n", float64(time.Since(timeStart).Microseconds())/1000)
}
