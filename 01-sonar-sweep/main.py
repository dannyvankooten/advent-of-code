sum = 0
i = 0
with open("input.txt") as file:
    prev = 0
    a = int(file.readline())
    b = int(file.readline())
    c = int(file.readline())

    for line in file:
        cur = a + b + c

        if prev > -1 and cur > prev:
            sum += 1

        prev = cur

        a = b
        b = c
        c = int(line)
        i += 1


print(sum)