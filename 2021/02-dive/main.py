

def part_1():
    pos = 0
    depth = 0
    with open("input.txt") as file:
        for line in file:
            [ins, n] = line.strip().split(" ")

            match ins:
                case "up":
                    depth -= int(n)

                case "down":
                    depth += int(n)

                case "forward":
                    pos = pos + int(n)
    print(pos*depth)


def part_2():
    pos = 0
    depth = 0
    aim = 0
    with open("input.txt") as file:
        for line in file:
            [ins, n] = line.strip().split(" ")

            match ins:
                case "up":
                    aim -= int(n)

                case "down":
                    aim += int(n)

                case "forward":
                    pos = pos + int(n)
                    depth += aim * int(n)
    print(pos*depth)

part_1()
part_2()