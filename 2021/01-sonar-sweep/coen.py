i = -1 # start at minus one, so the first one is automatically subtracted

with open("input.txt") as file:
    prev = 0

    for line in file:
        a = int(line)

        if ( a > prev ):
            i = i + 1

        prev = a

print(i)