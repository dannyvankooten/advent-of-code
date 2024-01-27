from pathlib import Path

def part_1(lines):
    pos, depth = 0, 0
    for line in lines:
        ins, n = line.strip().split(" ")

        match ins:
            case "up":
                depth -= int(n)

            case "down":
                depth += int(n)

            case "forward":
                pos = pos + int(n)
                
    return pos*depth


def part_2(lines):
    pos, depth, aim = 0, 0, 0
    for line in lines:
        ins, n = line.strip().split(" ")

        match ins:
            case "up":
                aim -= int(n)

            case "down":
                aim += int(n)

            case "forward":
                pos = pos + int(n)
                depth += aim * int(n)

    return pos*depth

if __name__ == '__main__':
    lines = Path("input.txt").read_text().split("\n")
    print("--- Day 2: Dive! ---")
    print("Part 1: ", part_1(lines))
    print("Part 2: ", part_2(lines))